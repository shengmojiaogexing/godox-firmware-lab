# Ghidra headless 流水线 + capstone 伪反编译 — 神牛 AD100 Pro V1.2

本目录位于仓库 `devices/ad100pro/ghidra/`，是 AD100 Pro **固件反编译** 任务的脚本与说明。
最终交付物在 **`out/decompiled/`**（capstone 伪反编译，1199 个函数文件）；
原计划的 Ghidra 流水线没能跑成（网络下载不可行，见 `out/ghidra/WHY_NOT.md`），
但脚本已备好，Ghidra 一旦可用即可补跑。

## 固件事实（关键，勿用错基址）

| 项目 | 值 |
|---|---|
| 固件 | `firmware/AD100pro_V1.2.bin`（50248 字节 = 0xC448） |
| **加载基址** | **0x08003000**（0x08000000–0x08002FFF 是未提供的 bootloader；本文件是应用镜像） |
| 镜像范围 | 0x08003000 – 0x0800F447 |
| 向量表 | 0x08003000，48 项（16 系统 + 32 IRQ） |
| 初始 SP | 0x20000CD0 |
| Reset_Handler | 0x080030D4（向量[1] 存 0x080030D5，Thumb 位） |
| main | 0x0800CF74 |
| MCU | STM32F0 / Cortex-M0 / Thumb-1 |
| JDK | `tools\jdk\jdk-21.0.13+9`（Temurin 21.0.13+9，已就绪，`java/javac` 实测可用） |
| Ghidra（未落地） | `tools\ghidra.zip` → 解压后应为 `tools\ghidra_12.2_PUBLIC` |

## 一、实际采用的方案：capstone 伪反编译（已交付）

### 脚本

| 文件 | 作用 |
|---|---|
| `ghidra/decompile.py` | 读 `out/functions.json` + `out/disasm.txt` + `firmware/*.bin` + `out/periph_map.json`，逐条翻译 Thumb-1 为伪 C |
| `ghidra/compare_with_lead.py` | 与 Lead 的 capstone 清单做地址集合对照（可选） |
| `ghidra/repair_ghidra.ps1` | 尝试修复被并行下载损坏的 Ghidra zip（未成功，历史记录） |

### 运行

> 以下命令均从**仓库根**执行（路径相对仓库根）。

```powershell
tools\python\python.exe devices\ad100pro\ghidra\decompile.py
```

### 输出（`out/decompiled/`）

```
decompiled/
├── 0x080030C0.c ... 0x0800FFFF.c   1199 个函数伪 C（含地址/大小/调用者/被调用者/MMIO 头注释）
├── index.tsv          地址/大小/insns/调用数/名字建议/class(函数/碎片/hub…)/MMIO
├── CALLGRAPH.md       调用图：启动链、main 为根的树、in/out-degree 排行
├── SELFCHECK.md       固定种子随机抽 5 个函数，逐条核对地址+机器码与 disasm.txt 一致
├── ANNOTATED_KEY_FUNCS.md  9 组关键函数人工精读（Reset/启动桩/init_array/SystemInit/
│                           main/GPIO API/LCD 串口写/大函数/主循环分发器）
├── ISSUES.md          functions.json 的边界/数据问题清单（806 重叠、98 个 2 字节碎片等）
├── issues_auto.md     自动检测的统计明细
└── FINDINGS.md        子系统/外设/协议线索（中断架构、外设画像、LCD/Flash/RF 候选）
```

### 伪 C 的局限（重要）

- **不是真正反编译**：没有表达式重建、类型恢复、变量重命名；寄存器 `r0–r7/sp/lr/pc`
  原样保留，内存访问写成 `*(uint32_t*)(r0 + 4)`，控制流用 `goto L_xxxx` 表达。
- 依赖 `out/functions.json` 的函数边界；该表有已知问题（启动区被切碎、0x0800DA78
  实为字模数据、806 个碎片条目），详见 `out/decompiled/ISSUES.md`。
- 指令逐条来自 `out/disasm.txt`（capstone，19142 条已解码指令），地址可逐条核对；
  同一段代码若被多个重叠条目覆盖，会在多个 .c 中重复出现。
- 无 ASCII 字符串线索（本固件确实没有可读字符串），模块命名靠调用关系 + 外设推断。

## 二、Ghidra 流水线（脚本就绪，待 Ghidra 可用时补跑）

### 脚本

| 文件 | 作用 |
|---|---|
| `ghidra/scripts/InitFirmware.java` | **preScript**：读 48 项向量表，清 Thumb 位、置 `TMode=1`、反汇编并建函数、按 CMSIS 命名（Reset_Handler / SysTick_Handler / IRQn_Handler…）、加 external entry point |
| `ghidra/scripts/ExportAll.java` | **postScript**：`DecompInterface` 反编译全部函数（含 thunk），导出 functions.tsv / decompiled/<addr>_<name>.c / decompile_all.c / strings.tsv / refs.tsv / memory_map.md |
| `ghidra/run_analysis.ps1` | 一键复现（Ghidra、JDK、仓库、基址均可参数化） |

### 命令（等价于 run_analysis.ps1，均从仓库根执行）

```powershell
$env:JAVA_HOME = 'tools\jdk\jdk-21.0.13+9'
& tools\ghidra_12.2_PUBLIC\support\analyzeHeadless.bat `
    devices\ad100pro\ghidra\project AD100pro_V12 `
    -import devices\ad100pro\firmware\AD100pro_V1.2.bin `
    -processor ARM:LE:32:Cortex `
    -loader BinaryLoader `
    -loader-baseAddr 0x08003000 `
    -loader-blockName FLASH `
    -scriptPath devices\ad100pro\ghidra\scripts `
    -preScript InitFirmware.java `
    -postScript ExportAll.java devices\ad100pro\out\ghidra `
    -analysisTimeoutPerFile 1800 -max-cpu 4 -deleteProject `
    -log devices\ad100pro\ghidra\analyze.log `
    -scriptlog devices\ad100pro\ghidra\analyze_scripts.log
```

> 基址必须是 **0x08003000**；若用 0x08000000，向量表会被放到 0x08000000 而代码里的
> 绝对地址（0x0800xxxx）与内存布局错位，反编译引用全错。入口 0x080030D4（Thumb）。

### 为什么没跑成

两重原因（详见 `out/ghidra/WHY_NOT.md`）：

1. **Ghidra zip 损坏**：多次网络失败后最终拿到 569,712,708 字节（大小正确）的 zip，
   但 sha256 不符；逐个成员解压只有 6781/6823 成功，损坏区包含
   `Docking.jar / Generic.jar / Application.jar / Ghidra.jar` 等核心 jar，无法运行。
2. **JDK 版本不满足**：这套 Ghidra 12.2（build 20261016）要求 **JDK 25+**，
   本机只有 Temurin 21.0.13+9；且其 `analyzeHeadless.bat` 无法处理含中文的
   `JAVA_HOME` 路径（`tools\jdk\...` 传参乱码），需把 JDK 装到纯 ASCII 路径。
3. 只要拿到完整 zip + JDK 25（ASCII 路径），直接跑 `run_analysis.ps1` 即可补齐
   `out/ghidra/` 的真实反编译产物。

## 目录结构

```
ghidra/
├── decompile.py            # 实际采用的伪反编译脚本
├── compare_with_lead.py    # 与 capstone 清单对照（可选）
├── repair_ghidra.ps1       # Ghidra zip 修复尝试（失败，历史）
├── scripts/InitFirmware.java
├── scripts/ExportAll.java
├── run_analysis.ps1        # Ghidra 一键脚本（待 Ghidra 可用）
├── README.md
└── analyze.log / analyze_scripts.log  # 若跑过 Ghidra 才有
```

## 复现结果（本次）

- 1199 个 `.c` 文件（每个 functions.json 条目一个），伪 C 指令 19142 条；
- call edges (bl/blx) 1492 条，tail-call 5 条；
- SELFCHECK 5/5 PASS（固定种子抽 5 个 body≥5 条指令的函数，逐条核对地址+机器码）；
- 函数分类：function 313、tiny_fragment 688、hub 44、vector_handler 15、
  nested_block 68、empty_fragment 71（碎片问题见 ISSUES.md）。
