#include "logger.h"
#include "game_api.h"

#include <x4_game_func_table.h>

#include <chrono>
#include <filesystem>
#include <format>
#include <system_error>

namespace fs = std::filesystem;

namespace x4n {

FILE*      Logger::s_handle = nullptr;
std::mutex Logger::s_mutex;

std::vector<std::pair<LogLevel, std::string>> Logger::s_buffer;
bool        Logger::s_buffering = true;
std::string Logger::s_mod_root;
std::string Logger::s_profile_dir;

static constexpr const char* level_tag(LogLevel lv) {
    switch (lv) {
        case LogLevel::Debug: return "debug";
        case LogLevel::Info:  return "info";
        case LogLevel::Warn:  return "warn";
        case LogLevel::Error: return "error";
    }
    return "?";
}

static void debug_out(const std::string& msg) {
    std::fputs(msg.c_str(), stderr);
}

static void log_rotate_error(const char* op, const std::string& path) {
    std::error_code ec;
    if (fs::exists(path, ec) || ec) {
        debug_out(std::string("X4Native: log ") + op + "  + path +  failed\n");
    }
}

FILE* Logger::open_log(const std::string& log_path) {
    static constexpr int MAX_BACKUPS = 4;
    std::string base = log_path;
    if (base.size() >= 4 && base.compare(base.size() - 4, 4, ".log") == 0)
        base.resize(base.size() - 4);

    std::error_code ec;
    fs::remove(base + ".4.log", ec);

    for (int i = MAX_BACKUPS - 1; i >= 1; --i) {
        std::string src = base + "." + std::to_string(i) + ".log";
        std::string dst = base + "." + std::to_string(i + 1) + ".log";
        std::error_code rename_ec;
        fs::rename(src, dst, rename_ec);
        if (rename_ec && fs::exists(src)) log_rotate_error("rotate", src);
    }

    std::error_code backup_ec;
    fs::rename(log_path, base + ".1.log", backup_ec);
    if (backup_ec && fs::exists(log_path)) log_rotate_error("rotate", log_path);

    FILE* h = std::fopen(log_path.c_str(), "w");
    if (!h) log_rotate_error("open", log_path);
    return h;
}

void Logger::init(const std::string& mod_root) {
    std::lock_guard lock(s_mutex);
    s_mod_root = mod_root;
    s_buffering = true;
    s_buffer.clear();
    s_handle = nullptr;
}

std::string Logger::profile_log_dir() {
    if (!s_profile_dir.empty()) return s_profile_dir;

    auto* game = GameAPI::table();
    if (!game || !game->GetSaveFolderPath) return {};

    const char* raw = game->GetSaveFolderPath();
    if (!raw || !raw[0]) return {};

    fs::path save_folder(raw);
    fs::path profile = save_folder.has_filename()
        ? save_folder.parent_path()
        : save_folder.parent_path().parent_path();

    fs::path dir = profile / "x4native";
    std::error_code ec;
    fs::create_directories(dir, ec);
    if (ec) return {};

    s_profile_dir = dir.string();
    if (!s_profile_dir.empty() && s_profile_dir.back() != '/')
        s_profile_dir += '/';
    return s_profile_dir;
}

std::string Logger::profile_ext_dir(const std::string& extension_id) {
    if (extension_id.empty()) return {};
    auto base = profile_log_dir();
    if (base.empty()) return {};

    fs::path dir = fs::path(base) / extension_id;
    std::error_code ec;
    fs::create_directories(dir, ec);
    if (ec) return {};

    std::string out = dir.string();
    if (!out.empty() && out.back() != '/')
        out += '/';
    return out;
}

bool Logger::is_safe_relative_name(const std::string& name, const char* ctx) {
    if (name.empty()) {
        Logger::warn("{}: empty filename rejected", ctx ? ctx : "path");
        return false;
    }
    fs::path p(name);
    if (p.is_absolute()) {
        Logger::warn("{}: absolute path rejected: {}", ctx ? ctx : "path", name);
        return false;
    }
    for (const auto& part : p) {
        if (part == "..") {
            Logger::warn("{}: .. in path rejected: {}", ctx ? ctx : "path", name);
            return false;
        }
    }
    return true;
}

void Logger::open_files() {
    std::string dir = profile_log_dir();
    if (dir.empty()) dir = s_mod_root;

    FILE* h = open_log(dir + "x4native.log");

    std::vector<std::pair<LogLevel, std::string>> pending;
    {
        std::lock_guard lock(s_mutex);
        s_handle = h;
        s_buffering = false;
        pending.swap(s_buffer);
    }

    if (!h) {
        debug_out("X4Native: Failed to open framework log file\n");
        return;
    }

    for (auto& [lv, msg] : pending)
        write(lv, msg);
}

void Logger::shutdown() {
    std::lock_guard lock(s_mutex);
    if (s_handle) {
        std::fflush(s_handle);
        std::fclose(s_handle);
        s_handle = nullptr;
    }
    s_buffer.clear();
    s_buffering = false;
    s_profile_dir.clear();
    s_mod_root.clear();
}

static void write_handle(std::mutex& mtx, FILE* h, LogLevel level, std::string_view line) {
    bool should_flush = false;
    if (h) {
        std::lock_guard lock(mtx);
        std::fwrite(line.data(), 1, line.size(), h);
        should_flush = (level >= LogLevel::Info);
    }
    if (should_flush) std::fflush(h);
}

void Logger::write(LogLevel level, std::string_view msg) {
    auto now = std::chrono::system_clock::now();
    auto line = std::format("[{:%Y-%m-%d %H:%M:%S}] [{}] {}\n", now, level_tag(level), msg);

    FILE* h = nullptr;
    {
        std::lock_guard lock(s_mutex);
        if (s_buffering) {
            s_buffer.emplace_back(level, std::string(msg));
            debug_out(line);
            return;
        }
        h = s_handle;
    }

    write_handle(s_mutex, h, level, line);
    debug_out(line);
}

void Logger::write_to(FILE* h, LogLevel level, std::string_view msg) {
    auto now = std::chrono::system_clock::now();
    auto line = std::format("[{:%Y-%m-%d %H:%M:%S}] [{}] {}\n", now, level_tag(level), msg);
    write_handle(s_mutex, h, level, line);
    debug_out(line);
}

} // namespace x4n
