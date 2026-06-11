# Linux 900-611726 Provisional Offsets

This file is a working approximation map built before the Linux game update to
9.0 has actually been applied locally. Treat every entry here as provisional
until revalidated against the updated Linux ELF.

## Status

- `confirmed-export`: resolved directly from the current Linux ELF export table
- `candidate-string`: recovered from unique string/xref workflow on the current Linux ELF
- `candidate-approx`: plausible current-binary match, needs recheck after update

## Current Provisional Map

| Function | Provisional Linux RVA | Status | Notes |
|---|---:|---|---|
| `SetCommander` | `0x021DD940` | `confirmed-export` | direct ELF export |
| `SetAssignment` | `0x021DD820` | `confirmed-export` | direct ELF export |
| `GetFactionBuildMethod` | `0x021A02C0` | `confirmed-export` | direct ELF export |
| `CreateOrderInternal` | `0x00F5B6A0` | `candidate-string` | from `CreateOrderFromUI()` string anchor |
| `SetOrderParam` | `0x00ADAF30` | `confirmed-current-linux` | exact updated Linux handler recovered from `SetOrderParam():` string xrefs |
| `RemoveOrderListParam` | `0x00ADB260` | `confirmed-current-linux` | exact updated Linux handler recovered from adjacent registration/error path |
| `SetOrderParamInternal` | `0x010B88B0` | `confirmed-current-linux` | exact updated Linux mutation helper reached from `SetOrderParam` handler at `0x00ADAF30` |
| `CreateDynamicInterior` | `0x00FA49C0` | `candidate-string` | pre-truth candidate; updated Linux 9.0 anchors now at `0x02E0ADA8`, `0x02E0AE20`, and `0x02E0AEA8` |
| `Component_GetCombinedSeed` | `0x00EF0E80` | `candidate-approx` | current binary prologue candidate matching known 611726 Windows RVA family |
| `ComponentRegistry_Find` | `0x00FCED70` | `candidate-approx` | current binary candidate from nearby internal references |
| `SetObjectRadarVisible_Action` | `0x016FD880` | `candidate-approx` | current-binary radar-visible action neighborhood; contains `mov [rbx+0x400], al` write site at `0x016FD4BF` |
| `IsRadarVisible_ReadByte` | `0x0168D880` | `candidate-approx` | current-binary read neighborhood; contains `movzx eax, byte ptr [rax+0x400]` at `0x0168D8AB` |
| `X4_FrameTick` | `0x00FA2D70` | `candidate-approx` | pre-truth timing/FNV candidate; needs re-derivation on updated Linux 9.0 |
| `MacroRegistry_Lookup` | `0x01FDF300` | `candidate-string` | updated Linux 9.0 has exact error anchor `Cannot find XML file component macro '%s' in index '%s'` at `0x02D63B58` |
| `ConstructionDB_AddPlan` | `0x01B96010` | `candidate-string` | updated Linux 9.0 has exact error anchor `ConstructionDB::AddPlan: Construciton plan is null` at `0x02D7CBB8` |
| `MD_CreateDynamicInterior_Handler` | `0x00FA4A00` | `candidate-approx` | pre-truth larger caller neighborhood around `CreateDynamicInterior`; updated Linux 9.0 still needs exact caller recovery |
| `ConstructionDB_CreatePlanDirect` | `0x01B95FB8` | `candidate-approx` | pre-truth alloc-and-register wrapper candidate; keep for quick recheck |
| `PlanEntry_Construct` | `0x01B95F40` | `candidate-approx` | pre-truth plan-entry init neighborhood; keep for quick recheck |
| `GameAlloc` | `0x02578130` | `candidate-approx` | pre-truth allocation hub candidate; still needs exact current-linux proof |
| `RadarVisibilityChanged_BuildEvent` | `0x01C1CDC1` | `candidate-neighborhood` | updated Linux 9.0 RTTI anchors now at `0x02CE49A0` and `0x02D0D800`; exact event-builder still unresolved |
| `EventQueue_InsertOrDispatch` | `0x01CB8485` | `candidate-neighborhood` | updated Linux 9.0 still anchored by MD event families like `event_object_destroyed` at `0x02CF60E9`; exact funnel entry unresolved |
| `FactionRelation_LookupReasonID` | `0x02156520` | `candidate-neighborhood` | updated Linux 9.0 reason workflow still anchored through `SetFactionRelationToPlayerFaction` export; exact lookup helper unresolved |
| `FactionRelation_GetFloat` | `0x022C4160` | `candidate-neighborhood` | updated Linux 9.0 faction-evaluation neighborhood still anchored by `GetFactionRelationStatus`; exact float-reader unresolved |

## Still To Approximate


No unresolved items remain in the approximation pass.

## Revalidation Rule

When the Linux 9.0 update finishes downloading and is applied:

1. Re-derive every `candidate-string` and `candidate-approx` entry from the updated Linux ELF.
2. Only after that, move confirmed values into `native/version_db/internal_functions.json` as `rva_linux` for `900-611726`.
