# AD200 Pro V1.60 灯端「私有密钥开关」固件 —— 共享任务简报

> 本文件是本次任务的共享背景。所有 teammate 先读本文件，再读自己任务里指定的材料。
> 工作区：``（Windows / PowerShell）。所有路径相对 ``。

## 0. 任务目标（Lead 已定方向）

给 **AD200 Pro V1.60** 灯端固件做「可与 X3-C 私有密钥配套」的 v4 固件：

- 灯端 C.Fn 菜单里有一个 **独立开关**（ON=只接受私有 KEY 同步字；OFF=原厂 `50024/(ID+1)`）；
- 开关 **持久化**（断电/换电池后保持）；
- 开关切换后 **同步字立即生效**；
- 原位补丁：固件长度 46380 字节保持不变，官方 G3 可刷；
- 输出：`keypatch/out/AD200pro_V1.60_keyD798.bin`（KEY=0xD798），模块 `keypatch/devices/ad200.py`。

**已定技术路线（Lead 决策，2026-11-07）**：AD200 的 C.Fn 本来就渲染 `F1..F8` 八项，
第 8 项 F8 = 「稳定色温模式 ON/OFF」，其值就存在 **settings[7] = RAM 0x2000041F**，
并随 flash 参数记录自动保存/加载。因此直接 **复用 F8 作为密钥开关**：
- 不新增菜单项、不改导航/渲染，不改存储结构（零注入空间需求）；
- 代价：原厂的「稳定色温模式」功能被密钥开关取代（需在文档中明确，可刷回原厂恢复）；
- `SYNC = settings[7] ? KEY : 50024/(id+1)`；切换后调用一次同步字函数立即生效。

> 如需回退/备选方案（新增 F9 或用 ID=OFF 当开关）见 `keypatch/DESIGN_TOGGLE.md`，本轮先按上面做。

## 1. 设备/固件事实（已核实）

| 项 | 值 |
|---|---|
| 原厂镜像 | `devices/ad200pro/firmware/AD200pro_V1.60.bin`，46380 字节 |
| sha256 | `fc0f77f1eb1c9f9d6f1f96d15daa5722ae33a57fceffe1da02739530dd798bd0` |
| 基址 | `0x08003000`（文件偏移 = 地址 − 0x08003000） |
| 向量表 | 48 项 @0x08003000，main 复制到 RAM 0x20000000 并 SYSCFG 重映射 |
| **同步字函数** | **0x0800B9AC..0x0800B9E4（56 字节，到下一函数 0x0800B9E4）**：`sync=50024/(id+1)` → WriteReg(4,SYNC1) / WriteReg(5,SYNC0) → delay_us(20) → SCAL(0x33) → delay 1ms |
| 同步字函数调用点 | 0x080045F2 / 0x080048C6（rf_init）/ 0x08008552 / **0x08008AB6（ui_key_scan_task，入参 = settings[6]=ID）** |
| 原厂公式 | `50024 // (id+1)`；50024 字面量在 0x0800B9E0（与原厂同址） |
| 辅助函数 | `__aeabi_uidiv`=0x080030F0；`WriteReg(reg=r0,val=r1)`=0x0800CAA2；`delay_us(n=r0)`=0x0800C828 |
| 灯端 C.Fn 渲染 | **0x08003FD0**（仅被 display_task 在 0x080093C4 调用；switch 处理 item 0..7 = F1..F8） |
| C.Fn 打开标志 | UI 结构基址 **0x20000150**；`UI+0x12 = 0x20000162` == 1 时 display_task 调 0x08003FD0 |
| C.Fn 当前项索引 | **`UI+0x14 = 0x20000164`**（0..7），渲染时画 `F(index+1)` |
| C.Fn 编辑/确认态 | `UI+0x13 = 0x20000163`（渲染函数里用）、`UI+0x15 = 0x20000165`（重复计数）、`UI+0x16 = 0x20000166`（编辑态）|
| 显示 | 段码屏：16 字节段位图 **RAM 0x20000408**，位设置函数 **0x0800CCAC(value, bitpos)**；字模表 **0x0800D01C**（32 项 7 段码）；10 个字段渲染函数 0x08008DEC/0x08008E58/0x08008EC4/0x08008F34/0x08008FA0/0x0800900C/0x08009078/0x080090E4/0x08009150/0x080091BC |
| C.Fn 项 → 设置字节 | F1..F8 = **settings[0..7] = RAM 0x20000418..0x2000041F**（F7=ID=settings[6]=0x2000041E；**F8=settings[7]=0x2000041F**） |
| C.Fn 项含义（官方手册） | F1 Beeper ON/OFF；F2 S1/S2 OFF/S1/S2；F3 Auto-Standby OFF/30/60/90；F4 闪光延迟 OFF,0.01~30s；F5 Mask OFF/N1/N2；F6 t0.1 显示 ON/OFF；F7 ID OFF,01-99；**F8 稳定色温模式 OFF/ON** |
| 参数记录 | flash 页 0x0801F000 + slot*100（slot 0..19），记录 32 字节；**记录字节 24..31 = settings[0..7]**（builder 0x080043E4 写 word6/7） |
| 记录构建 | **0x080043E4**(record_src=0x20000484)；**0x0800B820(slot)** = 调 builder → 0x20000484..0x200004A3 拷到 0x20000464 → `flash_write_params` 0x08005974(0x0801F000+slot*100) |
| 记录读取/展开 | 0x0800AC58 settings_load_task：扫描 slot 19..0 找首字节 magic 0x5C → `flash_read_params` 0x0800590C → **0x0800CCC8** 把记录 8 个 u32 展开：记录 word6/word7 的字节 → 0x20000418..0x2000041F |
| 恢复出厂默认 | 0x080048E4：settings[0..7] 全写 0（含 settings[7]=0），channel=0x14 等 |
| 段码显示任务 | display_task **0x08009228**（main 0x0800CE76 调用） |
| 主循环 | main 0x0800CDB0；TIM14 分频 25 置 0x200001CF=1 |
| 整机模拟器 | `devices/ad200pro/`（Python 3.12 + unicorn/capstone：`devices\ad200pro\python\python.exe`）；`sim/`、`tests/run_all.py`、`webui/` |
| keypatch 环境 | `tools\python\python.exe`（3.11，capstone/unicorn） |

RAM 关键地址：UI 状态 0x20000150；g_mode 0x2000015D；RF 使能 0x20000165；信道 0x2000016C；
settings 0x20000418；ADC DMA 缓冲 0x20000424；参数块镜像 0x20000464；记录源 0x20000484；
flash 参数页 0x0801F000；标志页 0x0801F800。

## 2. 任务分工（写入各自目录，勿越界）

- **cfn-re** → `keypatch/dev/ad200_ui/menu/`：C.Fn 交互路径（进入/导航/改值/保存）与「切换后立即生效」补丁点。
- **sim-re** → `keypatch/dev/ad200_ui/sim/`：整机模拟器验证脚手架（按键驱动 C.Fn、段码位图截图、保存/加载持久化、启动回归）。
- **flag-re** → `keypatch/dev/ad200_ui/store/`：F8/settings[7] 语义与存储链路核实（谁会读 settings[7]、切换是否有副作用、保存/加载 roundtrip）。
- **Lead** → `keypatch/devices/ad200.py`（v4 同步字函数整段改写）、集成、`keypatch/out/`、NOTES/README。

## 3. 交付格式

每个 teammate 在自己的目录里写：
1. `*.md` 结论（地址、字节、证据、建议补丁；不确定项如实标「未验证/推测」）；
2. 可复现脚本 + 日志（`python ... > log.txt`）；
3. 完成后给 Lead 发消息，250 字内摘要（关键地址、结论、风险、文件路径）。

## 4. 必须遵守

- 不修改其它目录的文件；不改 `keypatch/devices/*.py`（Lead 负责）、不改原厂 bin。
- 不启动/占用 8765/8766/8088/8899 等端口；需要后台服务用唯一端口 + 记录 PID。
- 不结束用户浏览器进程（见工作区 AGENTS.md）。
- 时间不要太长；每 10~15 分钟给 Lead 一条进度消息也行（可选）。
