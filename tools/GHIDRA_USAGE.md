# GHIDRA_USAGE.md — how to use the installed Ghidra for the X3_C firmware

Installed by **re-ghidra** (task-1). All local/offline.

## 1. What is installed (and why it lives outside <repo>)

| What | Path |
|---|---|
| Temurin JDK 25 (Ghidra 12.2 **requires JDK 25+**) | `D:\jdk25` (`bin\java.exe`) |
| Temurin JDK 21 (kept for other tools) | `tools\jdk\jdk-21.0.13+9` |
| Ghidra 12.2 PUBLIC | `D:\ghidra_12.2_PUBLIC` (`support\analyzeHeadless.bat`) |
| Ghidra project (persistent) | `D:\ghidra_project` — project name `X3C` |
| Script sources (canonical) | `devices\x3c\tools\ghidra_scripts\` → copied to `D:\ghidra_scripts\` |
| Analysis database (exports) | `devices\x3c\re\ghidra_exports\main\` and `...\hotshoe\` |

**Why not inside tools?** Ghidra 12.2 builds `jar:file:/...` URLs from its install path
without percent-encoding; a CJK path (`神牛`) makes log4j/class loading fail
(`SAXParseException: Opaque part contains invalid character`). It also needs JDK 25 (JDK 21 is
rejected). Therefore Ghidra+JDK25 were moved to ASCII paths. The team-visible driver and docs
stay in `devices/x3c/tools/` as required.

ASCII junctions used so scripts and inputs stay readable (created once, recreate if missing):
```
mklink /J C:\x3c            "devices\x3c\firmware"
mklink /J C:\x3c_tools      "tools"
mklink /J D:\ghidra_out     "devices\x3c\re\ghidra_exports"
```

**Address mapping** (see `devices/x3c/re/GHIDRA_REPORT.md`):
* main app: `vaddr = 0x08004800 + file_offset` (NOT 0x08000000 — that is bootloader space)
* hot-shoe blob: file 0x0F3800..EOF → `vaddr = 0x08002000 + (file_offset - 0x0F3800)`

## 2. Decompile arbitrary addresses (fast path — reuses the analyzed project)

```powershell
cd <repo>
tools\python\python.exe devices\x3c\tools\decompile.py 0x08033e20 0x08004af8
tools\python\python.exe devices\x3c\tools\decompile.py --program hotshoe 0x080020cc
```
Prints decompiled C + disassembly of the containing function (~10–30 s).

Raw headless invocation:
```powershell
$env:JAVA_HOME='D:\jdk25'; $env:PATH="D:\jdk25\bin;$env:PATH"
& D:\ghidra_12.2_PUBLIC\support\analyzeHeadless.bat `
  D:\ghidra_project X3C -process X3_C_v1.38_app.bin -noanalysis -readOnly `
  -scriptPath D:\ghidra_scripts -postScript "DumpAddr.java C:\temp\out.txt 0x08033e20"
```

## 3. Re-run the full import + analysis + export

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File devices\x3c\tools\ghidra_run.ps1 -Main
# or -Blob / no switch = both
```
Import: raw binary (`-loader BinaryLoader`), `ARM:LE:32:Cortex`, base `0x08004800` (main) /
`0x08002000` (hot-shoe). Pre-script `PreSetup.java <vecBase> <count> <resetEntry>`:
1. reads the 131 / 46 vector-table entries,
2. creates Pointer32 data + data references for them,
3. creates a function start for every vector target and disassembles it,
4. seeds disassembly from the reset handler.

Post-script `ExportAll.java <outdir>` writes:
`functions.csv`, `decompiled/<addr>_<name>.c` (one decompiled C file per function),
`strings.csv` (with xrefs), `datarefs.csv` (instruction/literal values classified
flash/RAM/peripheral/SCS; string pointers), `callgraph.json`, `memory_map.csv`,
`vector_table.csv` (per-vector check: is the target a Ghidra function start? is it a
`b .` default handler?), `gaps.csv` (unanalyzed regions >64 B), `overview.json`.

## 4. Files provided to the team

```
devices/x3c/re/ghidra_exports/main/functions.csv     addr,name,size,thunk,external,insn_count,param_count,callers,callees
devices/x3c/re/ghidra_exports/main/decompiled/*.c    <0xADDR>_<name>.c for every function
devices/x3c/re/ghidra_exports/main/strings.csv       addr,length,kind,text,xref_count,xrefs
devices/x3c/re/ghidra_exports/main/datarefs.csv      src_addr,kind,value,value_class,target_addr,target_kind,target_symbol
devices/x3c/re/ghidra_exports/main/callgraph.json    nodes[]/edges[]
devices/x3c/re/ghidra_exports/main/vector_table.csv  vector->function validation
devices/x3c/re/ghidra_exports/main/gaps.csv          uncovered byte ranges (>64 B)
devices/x3c/re/ghidra_exports/hotshoe/...            same set for the hot-shoe sub-image
devices/x3c/re/ghidra_exports/project.json           manifest used by decompile.py
devices/x3c/re/GHIDRA_REPORT.md                      findings with addresses (append-only)
```

## 5. Tips

* Grep the decompiled C: `Get-ChildItem devices\x3c\re\ghidra_exports\main\decompiled\*.c | Select-String "VTOR"`.
* Auto names are `FUN_<addr>`; the reset handler is renamed `Reset_Handler` by PreSetup.
* Fallback (no Ghidra needed, capstone-based): `devices/x3c/tools/ghidra_fallback.py` produces the same
  core CSV/JSON database — used while downloading and for cross-checking.
* If the project is lost, re-run §3 (analysis of the 1 MB image takes ~2–5 min).
* Headless heap: set `GHIDRA_HEADLESS_MAXMEM=4G` if decompiling very large functions.
