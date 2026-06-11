#pragma once

#include <cstdint>
#include <string>

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
namespace x4n::platform {
using ModuleHandle = HMODULE;
using SymbolPtr = FARPROC;

inline ModuleHandle get_main_module() { return GetModuleHandleA(nullptr); }
inline ModuleHandle get_loaded_module(const char* name) { return GetModuleHandleA(name); }
inline ModuleHandle load_module(const char* path) { return LoadLibraryA(path); }
inline SymbolPtr get_symbol(ModuleHandle module, const char* name) {
    return module ? GetProcAddress(module, name) : nullptr;
}
inline bool unload_module(ModuleHandle module) { return module ? FreeLibrary(module) != 0 : true; }
inline std::string module_filename(ModuleHandle module) {
    char path[MAX_PATH] = {};
    if (!GetModuleFileNameA(module, path, MAX_PATH)) return {};
    return path;
}
inline const char* framework_binary_name() { return "x4native_64.dll"; }
inline const char* core_binary_name() { return "x4native_core.dll"; }
inline const char* extension_binary_suffix() { return ".dll"; }
inline bool hooks_supported() { return true; }
}
#else
#include <dlfcn.h>
#include <limits.h>
#include <unistd.h>
namespace x4n::platform {
using ModuleHandle = void*;
using SymbolPtr = void*;

inline ModuleHandle get_main_module() { return dlopen(nullptr, RTLD_NOW | RTLD_LOCAL); }
inline ModuleHandle get_loaded_module(const char* name) { return dlopen(name, RTLD_NOW | RTLD_NOLOAD); }
inline ModuleHandle load_module(const char* path) { return dlopen(path, RTLD_NOW | RTLD_LOCAL); }
inline SymbolPtr get_symbol(ModuleHandle module, const char* name) {
    return module ? dlsym(module, name) : nullptr;
}
inline bool unload_module(ModuleHandle module) { return module ? dlclose(module) == 0 : true; }
inline const char* last_error() {
    const char* err = dlerror();
    return err ? err : "unknown dlerror";
}
inline std::string main_executable_filename() {
    char path[PATH_MAX] = {};
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (len <= 0) return {};
    path[len] = 0;
    return path;
}

inline std::string module_filename(ModuleHandle module) {
    if (module) {
        Dl_info info{};
        if (dladdr(module, &info) != 0 && info.dli_fname) {
            return info.dli_fname;
        }
    }
    return main_executable_filename();
}
inline const char* framework_binary_name() { return "x4native_64.so"; }
inline const char* core_binary_name() { return "x4native_core.so"; }
inline const char* extension_binary_suffix() { return ".so"; }
inline bool hooks_supported() { return false; }
}
#endif
