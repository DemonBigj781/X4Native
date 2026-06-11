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
| `CreateDynamicInterior` | `0x00FA49C0` | `candidate-string` | `virtual_navcontext_macro` / `Controllable::CreateDynamicInterior()` anchors |
| `Component_GetCombinedSeed` | `0x00EF0E80` | `candidate-approx` | current binary prologue candidate matching known 611726 Windows RVA family |
| `ComponentRegistry_Find` | `0x00FCED70` | `candidate-approx` | current binary candidate from nearby internal references |
| `SetObjectRadarVisible_Action` | `0x016FD880` | `candidate-approx` | current-binary radar-visible action neighborhood; contains `mov [rbx+0x400], al` write site at `0x016FD4BF` |
| `IsRadarVisible_ReadByte` | `0x0168D880` | `candidate-approx` | current-binary read neighborhood; contains `movzx eax, byte ptr [rax+0x400]` at `0x0168D8AB` |
| `X4_FrameTick` | `0x00FA2D70` | `candidate-approx` | current-binary timing/FNV neighborhood matching the known `611726` family |
| `MacroRegistry_Lookup` | `0x01FDF300` | `candidate-string` | current-binary function neighborhood containing `Cannot find XML file component macro '%s' in index '%s'` xref at `0x01FDF412` |
| `ConstructionDB_AddPlan` | `0x01B96010` | `candidate-string` | current-binary construction-plan registry neighborhood with FNV-1a hashing and plan-entry scans |
| `MD_CreateDynamicInterior_Handler` | `0x00FA4A00` | `candidate-approx` | larger current-binary caller neighborhood surrounding provisional `CreateDynamicInterior` and both window-placement error paths |
| `ConstructionDB_CreatePlanDirect` | `0x01B95FB8` | `candidate-approx` | small current-binary construction-plan helper near provisional `ConstructionDB_AddPlan`; plausible alloc-and-register wrapper |
| `PlanEntry_Construct` | `0x01B95F40` | `candidate-approx` | nearby current-binary plan-entry field-initialization neighborhood in the construction-plan cluster |
| `GameAlloc` | `0x02578130` | `candidate-approx` | recurring current-binary allocation hub seen from multiple construction/interior error and object-creation paths |
| `RadarVisibilityChanged_BuildEvent` | `0x01C1CDC1` | `candidate-neighborhood` | current-binary radar/event family anchor; recheck against updated Linux 9.0 before using as exact entry |
| `EventQueue_InsertOrDispatch` | `0x01CB8485` | `candidate-neighborhood` | current-binary MD event fanout family anchor around repeated `0x4bef020` dispatch-table access |
| `FactionRelation_LookupReasonID` | `0x02156520` | `candidate-neighborhood` | current-binary reason-lookup family anchored by `SetFactionRelationToPlayerFaction` and `relationchangereason` workflow |
| `FactionRelation_GetFloat` | `0x022C4160` | `candidate-neighborhood` | current-binary faction-relation evaluation family near `GetFactionRelationStatus` export path |

## Still To Approximate


No unresolved items remain in the approximation pass.

## Revalidation Rule

When the Linux 9.0 update finishes downloading and is applied:

1. Re-derive every `candidate-string` and `candidate-approx` entry from the updated Linux ELF.
2. Only after that, move confirmed values into `native/version_db/internal_functions.json` as `rva_linux` for `900-611726`.
