# AD200 Pro 固件验证脚手架（sim-re / task-2）

> 目录：`keypatch/dev/ad200_ui/sim/`（本目录只放验证脚本；**不改 `devices/ad200pro/` 源码**）
> 解释器：`devices\ad200pro\python\python.exe`（Python 3.12 + unicorn 2 + capstone 5）
> 固件基址 `0x08003000`；原厂 `devices/ad200pro/firmware/AD200pro_V1.60.bin`（46380B，
> sha256 `fc0f77f1…798bd0`）；补丁默认 `keypatch/out/AD200pro_V1.60_keyD798.bin`。

本脚手架用 devices/ad200pro 的**整机模拟器**（`sim.machine.Machine`，Unicorn 跑真实固件）验证
AD200 Pro v4「F8 私有密钥开关」方案：

1. 固件能启动到主循环（原厂 vs 补丁对比，无 fault/stall）；
2. C.Fn 菜单项 F1..F8 都能渲染，段码位图逐项 dump；F8 的 ON/OFF 两个状态值区不同；
3. `settings[7]` 走真实固件函数保存到 flash 参数页、重新加载、掉电重上电后保持（roundtrip）；
4. 补丁镜像启动/主循环任务计数回归，PASS/FAIL。

## 0. 文件

| 文件 | 作用 |
|---|---|
| `_sim_common.py` | 公共库：路径/常量、启动到主循环、固件函数调用、C.Fn 渲染、字体解码、3x3 键盘注入 |
| `boot_to_main.py` | 加载任意固件 → 跑到主循环；记录 insns/PC/无 fault；`--compare` 原厂 vs 补丁 |
| `cfn_probe.py` | 进入 C.Fn（**真实按键**或 `--direct`）→ 遍历 item 0..7 渲染 → dump 16B 段码位图；`settings[7]=0/1` 对比 F8 页 |
| `persist_test.py` | `settings[7]` → `0x0800B820(slot)` → flash `0x0801F000+slot*100` 记录字节31 → `0x0800AC58` 重新加载 → roundtrip（=1 与 =0），并做掉电重上电 |
| `regress.py` | 原厂 vs 补丁：启动到主循环指令数、主循环 200 万条任务/中断计数、无 fault，PASS/FAIL |
| `log_*.txt` | 上述脚本的实际运行日志（见 §4） |

## 1. 怎么跑（一条命令一个脚本）

```powershell
$PY = 'devices\ad200pro\python\python.exe'
cd <repo>

# 1) 启动到主循环（单镜像 / 对比）
& $PY keypatch\dev\ad200_ui\sim\boot_to_main.py
& $PY keypatch\dev\ad200_ui\sim\boot_to_main.py --compare
& $PY keypatch\dev\ad200_ui\sim\boot_to_main.py --bin keypatch\out\AD200pro_V1.60_keyD798.bin

# 2) C.Fn 菜单 + 段码位图（默认走真实按键路径；--direct 直接置 UI 状态）
& $PY keypatch\dev\ad200_ui\sim\cfn_probe.py
& $PY keypatch\dev\ad200_ui\sim\cfn_probe.py --bin keypatch\out\AD200pro_V1.60_keyD798.bin

# 3) settings[7] 保存/加载/掉电保持 roundtrip
& $PY keypatch\dev\ad200_ui\sim\persist_test.py
& $PY keypatch\dev\ad200_ui\sim\persist_test.py --bin keypatch\out\AD200pro_V1.60_keyD798.bin

# 4) 原厂 vs 补丁 启动/主循环回归（缺补丁时打印 SKIP 并只做原厂自检）
& $PY keypatch\dev\ad200_ui\sim\regress.py
```

日志重定向（示例，UTF-8 已由脚本强制）：

```powershell
& $PY keypatch\dev\ad200_ui\sim\cfn_probe.py > keypatch\dev\ad200_ui\sim\log_cfn_probe.txt
```

所有脚本退出码 `0 = PASS`、`1 = FAIL`；`--json out.json` 可额外导出结构化结果。

## 2. 关键地址（脚本里已固化为常量，见 `_sim_common.py`）

| 项 | 地址 |
|---|---|
| 主循环入口 / 区间 | `0x0800CE40` / `[0x0800CE40,0x0800CEB0)` |
| C.Fn 打开标志 | `UI+0x12 = 0x20000162` == 1 |
| C.Fn 当前项索引 | `UI+0x14 = 0x20000164`（0..7 = F1..F8） |
| C.Fn 编辑/渲染态 | `UI+0x13 = 0x20000163`；`UI+0x15/0x16 = 0x20000165/66` |
| C.Fn 渲染函数 | `0x08003FD0`（`(code,bitpos)` 字段函数 `0x08008DEC…0x080091BC`） |
| 段码位图 | `0x20000408..0x20000417`（16B）；置位函数 `0x0800CCAC`；字模表 `0x0800D01C` |
| settings[0..7] | `0x20000418..0x2000041F`（**settings[7]=F8=0x2000041F**） |
| 保存 | `0x0800B820(slot)` → 构建 `0x080043E4` → `flash_write_params 0x08005974` |
| 加载 | `0x0800AC58` → `flash_read_params 0x0800590C` → 展开 `0x0800CCC8` |
| 参数记录 | `flash 0x0801F000 + slot*100`，32B；字节0=magic `0x5C`，字节24..31=settings[0..7] |
| 键盘矩阵 | 行 PB3/PB4/PB5，列 PC10/PC11/PC12；键码 `1<<(3*row+col)` |

## 3. 实测结果（原厂镜像，日志见 §4）

### 3.1 `boot_to_main.py`
- reset：`pc=0x080030D4 sp=0x200008B0`；
- 到主循环入口 `0x0800CE40`：原厂 **78610** 条指令（wall ≈0.85s，单步精确计数）；
- 主循环再跑 200 万条无 fault/stall，最终 `pc=0x0800CE4C (main+0x9c)`；
- `--compare`（原厂 vs 当前 `keypatch/out/AD200pro_V1.60_keyD798.bin`）：两镜像复位 PC/SP
  与主循环入口一致，启动指令数 **78610 vs 78310（Δ=300）**，主循环各跑 200 万条后总指令数差
  仍为 300（运行路径未分叉）→ **PASS**。

### 3.2 `cfn_probe.py`
- **真实按键路径可打开 C.Fn**：注入 3x3 矩阵 `row2,col2`（键码 `0x100`）后，固件自己扫描/去抖，
  `UI+0x12` 变为 1（约 50 万条指令 ≈ 50 次去抖扫描）。
- item 0..7（写 `UI+0x14` + 调 `0x08003FD0`）各自位图（16B hex）：

```
item0 F1: 40 05 F7 8E 80 18 00 00 50 27 00 00 00 00 00 00
item1 F2: 40 05 F7 8E 80 10 C8 00 35 27 00 00 00 00 00 00
item2 F3: 40 00 02 FA F0 12 40 80 71 27 00 00 00 00 00 00
item3 F4: 40 05 FF 8E 80 14 00 00 72 27 00 00 00 00 00 00
item4 F5: D8 05 F7 8E 80 10 40 00 63 27 00 00 00 00 00 00
item5 F6: 60 05 F7 8E 80 10 40 00 67 27 00 00 00 00 00 00
item6 F7: 40 05 F7 8E 80 10 00 00 51 27 00 00 00 00 00 08
item7 F8: 40 05 F7 8E 80 10 40 00 77 27 00 00 00 00 40 00
```

  每项 `char8`（byte8）渲染 `F(index+1)` 的标签数字（item0→'1' … item7→'8'），全部 PASS。
- **F8 页 `settings[7]=0` vs `=1`（密钥开关 ON/OFF）**：

```
settings[7]=0（OFF/普通）: 40 05 F7 8E 80 10 40 00 77 27 00 00 00 00 40 00
settings[7]=1（ON/密钥）: 40 05 F5 E0 00 10 40 00 77 27 00 00 00 00 40 00
差异字节: byte[2] F7→F5, byte[3] 8E→E0, byte[4] 80→00    （byte8 标签 0x77 不变）
```

  字段渲染调用：OFF = 值区 chars1..3 `O/F/F`（`field#5 code15 'F'`、`field#6 code15 'F'`）；
  ON = 值区 chars1..2 `O/N`（`field#5 code26 'N'`，`field#6` 不再调用）。
  即 **"OFF" → "ON"**，标签仍为 `F8`。39 项检查全 PASS。

### 3.3 `persist_test.py`（roundtrip，原厂 + 补丁镜像都 PASS）
- `settings[7]=1` → `0x0800B820(19)` → flash `0x0801F76C`（= `0x0801F000+19*100`）记录：

```
5C 00 00 05 05 00 01 14 50 50 50 50 50 50 00 00
50 50 50 50 50 50 00 00 00 00 01 00 00 00 00 01
                                  ^^^^^^^^^^^ byte24..31 = settings[0..7]
                                              byte31 = settings[7] = 1
```

- 清空 settings RAM → `0x0800AC58` 重新加载 → `settings[7]=0x01`；`=0` 方向同样 PASS；
- **掉电重上电**（把模拟器里真实 flash 参数页预置进新 Machine 的 flash 再冷启动到主循环）：
  `settings[7]` 分别保持 1 / 0；
- slot 寻址校验：slot 0/1/7/19 记录地址 = `0x0801F000/0801F064/0801F2BC/0801F76C`，magic/byte31 均正确。

### 3.4 `regress.py`（原厂 vs 当前 keyD798 补丁）
- 两镜像复位 PC/SP 一致、精确命中主循环入口；
- 启动指令数差 **300**（|Δ| < 2000）；主循环各 200 万条无 fault/stall；
- 主循环任务计数同一量级：`display_task 28/32`、`ui_key_scan_task 28/33`、
  `flash_power_task 28/33`、`rf_poll_settings 28/32`、`charge_enable_task 28/32`、
  `measurement_task 28/32`、`tim14_irq 961/931`；总指令数差保持 300 → **PASS**。
- 补丁镜像不存在时：打印 `[SKIP]` 并只做原厂自检，退出码 0（便于 Lead 产出前先跑通）。

## 4. 日志

| 日志 | 内容 |
|---|---|
| `log_boot_to_main.txt` | `boot_to_main.py --compare`（原厂 vs keyD798）：PASS |
| `log_cfn_probe.txt` | `cfn_probe.py`（原厂，真实按键路径）：39 项 PASS |
| `log_cfn_probe_patched.txt` | `cfn_probe.py --bin keypatch/out/AD200pro_V1.60_keyD798.bin`：39 项 PASS |
| `log_persist_test.txt` | `persist_test.py`（原厂）：PASS |
| `log_persist_test_patched.txt` | `persist_test.py --bin …keyD798.bin`：PASS |
| `log_regress.txt` | `regress.py`：PASS（Δ=300） |

## 5. 实现要点/踩坑（给后续复用的人）

1. **按键注入**：devices/ad200pro 的 `Device.press()` 只改 `Device.buttons`，**不会**驱动 3x3 矩阵 GPIO，
   所以 WebUI 的按键对固件无实际作用。本脚手架在不改源码的前提下，运行时包装
   `m.bus._refresh_idr`：固件自己按 `ui_key_scan_task` 的时序把某一行（PB3/4/5）设为输出拉低、
   其余行输入上拉，我们只在读 `GPIOC IDR` 时按"当前被拉低的行 + 按下的键"返回列电平
   （PC10/11/12）。这样走的是**固件真实扫描 → 50 次去抖 → 分派**路径。
   - 实测 C.Fn 入口 = `(row2,col2)` = 键码 `0x100`；C.Fn 内 `(row1,col2)=0x20` 可切换
     `UI+0x13`（编辑/渲染态）；但"导航/改值"的完整真实按键序列尚未打通（见 §6）。
2. **C.Fn 渲染**：不需要跑 `display_task`。清 0x20000408 的 16B → 写 `UI+0x12=1`、`UI+0x14=index`
   → 调用 `0x08003FD0` 即可；它会调用 10 个字段函数 `0x08008DEC…0x080091BC`，每个画 char
   `bitpos>>1`（偶/奇 bitpos 决定两半段码顺序）。
3. **模拟器 flash 是"写透"的**：`flash_write_params` 直接写 `0x0801F000` 内存即生效，
   `bus._flash_write/flash_data_write` 只维护 FLASH_SR 状态位，所以保存链路可以真跑。
   但 `Machine.reset()` 会用镜像覆盖整片 flash（0xFF），所以**掉电重上电**要先 `Machine()`、
   把真实参数页 `m.write(0x0801F000, page)` 预置好，再用 `boot_to_main(..., do_reset=False)` 启动。
4. **unicorn hook 小坑**：`FieldTrace`（10 个字段函数上的 UC_HOOK_CODE）必须在"长跑/按键路径"
   **之前**安装；若在跑了几十万条指令之后再装，本模拟器上只会命中最后一个字段函数
   （cfn_probe 已按此顺序实现）。
5. **字体解码**：字模表 `0x0800D01C` 32 项，code 0..9=数字、10..15=A/b/C/d/E/F、16=空、
   26 在本固件的 F8 页里画 "N"（其段码不是标准 N，但上下文即 ON 的 N）；code0 同时可读作 O/0。
6. **时序**：模拟器外设是行为级近似（`1ms ≈ 2000 条指令`，TIM/SysTick 非周期精确），
   所以任务计数只做"同一量级（±25%）"判定，不逐周期对齐；不据此判断真实硬件时序。

## 6. 限制 / 未验证项

- **真实按键的"导航/改值"未走通**：目前只验证了 `MENU(row2,col2/0x100)` 打开 C.Fn、
  `SET(row1,col2/0x20)` 切换 `UI+0x13`；C.Fn 内 `UI+0x14` 的导航与 settings 值修改在模拟器里
  没有找到可注入的按键序列（其余键码在 C.Fn 内多表现为退出/无操作）。因此 cfn_probe 对
  item 遍历和 F8 值采用**直接写 RAM**（任务允许的"直接置 UI 状态 + 调处理函数"路线）。
  该交互路径的静态逆向由 `keypatch/dev/ad200_ui/menu/`（cfn-re）负责。
- 段码"参考图像"用 16B 位图 hex + 每字节二进制展开表示（任务允许 hex），未生成 PNG 截图；
  需要位图与屏幕像素的映射时另行扩展。
- 补丁镜像本身的行为（同步字 v4 公式、立即生效补丁）不在本脚手架断言范围，由 Lead 的
  `keypatch/devices/ad200.py` + `unicorn_test` 负责；本目录只保证"能启动/菜单能渲染/开关能持久化/无回归"。
- 只对 `devices/ad200pro/firmware|devices/x3c/firmware` 的原厂 V1.60 与 `keypatch/out/*.bin` 设计；
  其它版本固件需先更新 `_sim_common.py` 里的地址常量。

## 7. 给 Lead 的用法建议（拿到 v4 keyD798 后）

```powershell
$PY = 'devices\ad200pro\python\python.exe'
$B  = 'keypatch\out\AD200pro_V1.60_keyD798.bin'
& $PY keypatch\dev\ad200_ui\sim\boot_to_main.py --bin $B
& $PY keypatch\dev\ad200_ui\sim\cfn_probe.py --bin $B
& $PY keypatch\dev\ad200_ui\sim\persist_test.py --bin $B
& $PY keypatch\dev\ad200_ui\sim\regress.py --patched $B
```

四项全 `PASS`（退出码 0）即可认为：固件能启动、C.Fn（含 F8）渲染正常、F8 开关值能保存并
掉电保持、补丁未引入启动/主循环回归。任一项 FAIL，日志里有具体地址/字节/检查项。
