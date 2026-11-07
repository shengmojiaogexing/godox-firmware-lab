# X3-C（Godox X3 C 引闪器）

Godox **X3-C** TTL 引闪器的固件逆向资料与 Unicorn 模拟机。
本目录是整个多设备 monorepo 中 X3-C 这一台设备的研究工作区。

> ⚠️ 内置固件二进制不随仓库分发（`.gitignore` 已忽略 `*.bin`），请从官方渠道获取后放到
> `devices/x3c/firmware/`。仅对本人合法拥有的设备做互操作研究，见根目录 `DISCLAIMER.md`。

## 机型与固件

| 项目 | 值 |
|---|---|
| 设备 | Godox X3-C 无线引闪器（TTL） |
| 固件文件 | `devices/x3c/firmware/X3_C_v1.38_app.bin` |
| 版本 | v1.38 |
| 文件大小 | 1,021,468 字节 |
| SHA-256 | `45FDD9522C432F4BE60A470741D3E5C12A7EE629EB2B277AED5187639F85EF62` |
| MCU | Artery **AT32F435/437**（Cortex-M4F，1 MB Flash / 384 KB SRAM，非 STM32） |
| App 链接基址 | `0x08004800`（`file_offset = vaddr - 0x08004800`） |
| 内嵌子镜像 | 主固件内 0x0F3800 起是热靴 MCU 固件（独立链接基址 `0x08002000`） |
| 射频 | TI CC2500 类 2.4 GHz GFSK 收发器，GPIOC 软件位操作 SPI |

固件版本、镜像基址与 sha256 的完整清单见根目录 `docs/设备清单.md`。

## 快速开始

所有命令都以**仓库根目录**为当前目录执行（脚本内部用 `__file__` / `$PSScriptRoot` 推导仓库根，
因此在别的目录下用绝对路径调用也能正确找到固件与依赖）。

### 1. 准备 Python

```powershell
# 项目随附的便携运行时（windows，已含 capstone 5.0.7 / unicorn 2.1.4 / Pillow）
tools\python\python.exe --version

# 或者使用系统 Python 3.10+ 并安装依赖
pip install -r requirements.txt
```

### 2. 放入原厂固件

```text
devices/x3c/firmware/X3_C_v1.38_app.bin
```

### 3. 跑无头模拟机（boot log / 统计）

```powershell
tools\python\python.exe devices\x3c\emu\run.py --max-insn 200000
```

输出：执行指令数、停止原因、最终 PC/SP/LR、热点与未知 MMIO 表、寄存器快照、机器摘要；
完整日志写到 `devices/x3c/emu/logs/boot_<时间戳>.log`。

更长的启动（软件延时很长，可只跑前段细粒度、再用快进模式）：

```powershell
tools\python\python.exe devices\x3c\emu\run.py --max-insn 8000000
tools\python\python.exe devices\x3c\emu\run.py --max-insn 50000000 --quiet
tools\python\python.exe devices\x3c\emu\run.py --max-insn 200000000 --scan-fb
```

### 4. 还原开机画面的 PNG（选做）

```powershell
tools\python\python.exe devices\x3c\emu\demo_x3.py --max-insn 3000000
```

会解码 XMC 8080 总线上的 LCD 字节流并在 `devices/x3c/emu/lcd_out/` 生成 PNG 快照和总线日志。

### 5. 网页调试界面（浏览器）

```powershell
# 推荐：直接双击 scripts\start_x3_sim.bat
# 或者：
powershell -NoProfile -ExecutionPolicy Bypass -File devices\x3c\emu\start_x3_sim.ps1
```

浏览器打开 <http://127.0.0.1:18765/>：左侧是还原的 368x448 屏幕，右侧可看寄存器、反汇编
（支持断点）、内存、外设状态，并可 运行/暂停/单步/复位。

停止：双击 `scripts\stop_x3_sim.bat`，或

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File devices\x3c\emu\stop_x3_sim.ps1
```

> 两个 ps1 都只按端口/记录 PID 精确关闭本项目的 `web_server.py`，不会动其它浏览器进程。
> 交互式命令行调试器（无需浏览器）：`tools\python\python.exe devices\x3c\emu\debug.py`。

## 目录说明

| 目录 | 内容 |
|---|---|
| `firmware/` | 原厂固件（`*.bin` 被 gitignore，仅本地存在） |
| `re/` | 逆向报告：`HARDWARE.md`（MCU/启动链/内存/中断/LCD/射频/USB 识别）、`PROTOCOL.md`（2.4 GHz 协议证据）、`GHIDRA_REPORT.md`、`UI_STRINGS.md`、`CONTEXT.md`；`ghidra_exports/` 是 Ghidra 全量导出（函数表、反编译 C、字符串、交叉引用、调用图、向量表校验），`hwscan_strings.tsv` 为原始字符串转储 |
| `emu/` | Unicorn Cortex-M4 模拟机 + 命令行调试器 + 网页调试器（见 [`emu/README.md`](emu/README.md)） |
| `analysis/` | 早期静态分析产物（`functions.json` / `disasm.txt` / `mmio.json` / `literals.json` / `summary.txt`） |
| `lead/` | 项目早期 Lead 的启动/追踪试验脚本（历史材料，保留供参考） |
| `recon/` | 更早的散装逆向脚本（`*.py`）与证据文本（`strings.txt`、`t4.out` 等）；脚本按新布局读取 `devices/<设备>/firmware/` 下的固件，请在仓库根目录运行 |
| `tools/` | Ghidra 驱动、`hwscan_*` 硬件扫描脚本、Capstone 回退导出器；用法见 [`tools/GHIDRA_USAGE.md`](tools/GHIDRA_USAGE.md) |
| `py/` | 便携 Python 运行时（gitignore，仅本地；日常请优先用仓库根的 `tools/python`） |

## 关键文档

- [`re/HARDWARE.md`](re/HARDWARE.md) —— 硬件识别与地址地图（MCU、启动链、内存、IRQ、LCD、射频、USB）
- [`re/PROTOCOL.md`](re/PROTOCOL.md) —— 2.4 GHz 链路（CC2500 同步字/频点表/包格式）证据
- [`re/GHIDRA_REPORT.md`](re/GHIDRA_REPORT.md) —— Ghidra 分析记录与导出数据说明
- [`re/UI_STRINGS.md`](re/UI_STRINGS.md) —— UI 字符串清单与 ADR 引用分析
- [`re/CONTEXT.md`](re/CONTEXT.md) —— 早期任务上下文与工作划分
- [`emu/README.md`](emu/README.md) —— 模拟机架构、命令行用法、Python API、外设模型行为
- [`emu/FINDINGS.md`](emu/FINDINGS.md) —— 模拟机调试发现日志（append-only）
- [`tools/GHIDRA_USAGE.md`](tools/GHIDRA_USAGE.md) —— Ghidra 安装/反编译/重新导入导出流程

## 与仓库其它部分的关系

- 补丁引擎：`keypatch/devices/x3c.py`（原位替换固件中的「ID → CC2500 同步字」函数，设备模块
  读取的镜像路径为 `devices/x3c/firmware/X3_C_v1.38_app.bin`）。
- 可视化密钥工坊：`studio/`（浏览器里勾选 X3-C、生成并导出定制固件）。
- 整体介绍、快速开始与许可声明见仓库根目录 `README.md`。

## 备注

- 历史脚本（`recon/`、`lead/`、`analysis/`、`re/ghidra_exports/`）中的地址与日志是只读证据，
  可能保留了整理前的旧路径写法；**使用说明与可执行脚本**已全部更新为当前冻结布局。
- `devices/x3c/py/`、`devices/x3c/firmware/*.bin`、`**/lcd_out/`、`**/emu/logs/` 均被 `.gitignore`
  忽略，不会进入版本库。
