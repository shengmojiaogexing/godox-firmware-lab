# AD100 Pro 固件逆向工程工作区

固件：`firmware/AD100pro_V1.2.bin`（50248 字节，sha256 3a3d3b91…）
加载基址：0x08000000；目标 MCU：STM32F0 系列（Cortex-M0，Thumb-1）。

## 目录约定（各 agent 只在各自目录写，避免冲突）

| 目录 | 所有者 | 内容 |
|---|---|---|
| `tools/` | lead | capstone 静态分析流水线（`disasm.py`, `periph.py`） |
| `out/` | lead | 分析产物：`functions.json` `disasm.txt` `periph_map.json` |
| `ghidra/` | ghidra-ops | Ghidra 工程 + 反编译导出 |
| `emu/` | emu-engineer | Unicorn 模拟器、外设桩、CLI 调试器 |
| `webui/` | ui-engineer | 网页调试界面（纯 HTML/JS，无外部依赖） |
| `src/recovered/` | re-analysts | 逆向还原的 C 源码 |
| `docs/` | 全体 | 各自写独立文件，不覆盖他人文件 |

## 接口契约

1. 反汇编清单 `out/disasm.txt`：每行 `080030D5  4770          bx lr` 形式，函数头行 `; func_080030D5 ...`。
2. `out/functions.json`：`[{addr, size, name, callers[], callees[], lits[], insnCount}]`
3. 模拟器状态 JSON（供 webui 使用），`emu/server.py` 提供：
   - `GET /api/state` → `{pc, sp, regs[16], lr, xpsr, insnCount, running, breakpoints[], log[]}`
   - `GET /api/disasm?addr=` → `{addr, lines:[{addr, bytes, mnem, op}]}`
   - `GET /api/mem?addr=&len=` → `{addr, hex}`
   - `POST /api/cmd` `{cmd:"run"|"pause"|"step"|"reset"|"bp_add"|"bp_del"|"set_pc", ...}`
4. 屏幕渲染：emulator 在 LCD 帧缓冲地址/尺寸确定后写 `emu/screen.json`（`{w,h,addr,format}`），UI 读取后渲染 canvas。

## 常用命令

```powershell
# capstone 分析
..\..\tools\python\python.exe tools\disasm.py
# 模拟器
..\..\tools\python\python.exe emu\main.py --info
```
