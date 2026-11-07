# 固件调试模拟机（AD200 Pro Emulator）设计规范 / API 契约

> 本文件是 `sim/` 与 `webui/` 的实现契约。所有路径相对于项目根 `devices\ad200pro`。
> Python：`..\..\tools\python\python.exe`（已装 capstone 5.0.7 / unicorn 2.1.4，无网络安装权限问题，仅此环境）。

## 1. 目标

把 `firmware/AD200pro_V1.60.bin`（Cortex-M0，基址 0x08003000）跑起来：

1. reset → SystemInit → scatter load → main → 主循环；
2. SysTick 中断按 1ms 节拍触发，中断注入/返回正确；
3. ADC/DMA/GPIO/EXTI/TIM/FLASH/RCC/PWR 等外设用行为级模型兜住，固件不卡死；
4. 提供**调试能力**：单步/连续/断点/内存与寄存器读写、反汇编、函数调用追踪、MMIO 日志、中断日志、调用栈（启发式）；
5. 提供**虚拟外设/整机模型**：电池电压、电容电压、按键、2.4G 收发数据、温度等，能人工注入事件并在 UI 上看到固件反应（GPIO 输出、LCD/RF 引脚活动、状态变量）；
6. 提供 **Web 调试 UI** 与 CLI 两种入口。

非目标：不追求周期级精确（外设时序可近似）；不实现 bootloader（0x08000000–0x08002FFF 不存在）。

## 2. 目录与职责

```
sim/
  __init__.py
  firmware.py      # 加载 bin，基址映射、符号表、字面量表；可选从 analysis/symbols.json 读名字
  machine.py       # Machine：unicorn 封装、内存映射、异常注入/返回、run/step/reset
  bus.py           # 外设地址路由（MMIO 分发），未实现地址默认读写行为
  peripherals/
    __init__.py
    base.py        # 寄存器文件基类 + 日志
    rcc.py flash.py gpio.py exti.py syscfg.py adc.py dma.py tim.py pwr.py iwdg.py rtc.py scs.py
  device.py        # 整机虚拟设备（输入量/输出量/事件队列）
  debugger.py      # Debugger（对外稳定 API，见 §4）
  tracer.py        # 事件环形缓冲（call/ret/mmio/irq/exception/uart...）
  cli.py           # 交互式调试控制台（cmd.Cmd）
  server.py        # HTTP JSON API（stdlib http.server），供 webui 使用
  boot_trace.py    # 启动自检脚本：跑到 main 循环并打印 PC 直方图/最后 MMIO/卡死诊断
webui/
  index.html app.js style.css   # 单页调试器（无构建步骤，纯静态）
docs/usage.md                  # 用户使用说明（webui-dev 负责）
tests/
  test_boot.py                # 验收：启动成功
  test_debug.py               # 断点/单步/内存
  test_peripherals.py         # 外设行为单测
```

## 3. 固件事实（实现依据）

- 镜像：文件偏移 f ↔ 地址 `0x08003000 + f`；长度 0xB52C；文件尾之后到 0x08010000 填 0xFF。
- 向量表在 0x08003000，48 项。复位向量 0x080030D4→`SystemInit`；SP 初值 0x200008B0。
- RAM：映射 0x20000000–0x20002000（8 KiB，够用；实际固件用到 0x200008B0）。
- 运行时固件把向量表复制到 RAM 0x20000000 并置 SYSCFG_CFGR1[1:0]=11（SRAM 映射），
  所以**异常入口地址应从 0x20000000 的 RAM 向量表取**（也可兼容 0x08003000 的表）。
- 主循环等待 `*(volatile uint8_t*)0x200001CF == 1`。
- 关键符号见 `analysis/symbols.json`（`FUNC_xxxxxxxx` 为未命名）。

### 必须实现的外设语义（按重要性）
1. **SCS/SysTick/NVIC**
   - SysTick：CSR(ENABLE/TICKINT/COUNTFLAG)、RVR、CVR、CALIB；每 1 个 SysTick 时钟 CVR-1，到 0 时重装并置 COUNTFLAG，若 TICKINT 且 NVIC 允许 → pending SysTick(异常号 -1)。
   - NVIC：ISER/ICER/ISPR/ICPR/IPR，pending 位。
   - 异常注入：PRIMASK=0 且有 pending 时：按 ARMv6-M 压栈 {R0-R3,R12,LR,PC,xPSR}（8 字，栈 8 字节对齐），LR=0xFFFFFFF9（线程/主栈），PC=RAM 向量表[16+irq]（SysTick 为 [15]），IPSR=irq+16。
   - 异常返回：在代码 hook 中检测 `bx lr` 且 LR∈{0xFFFFFFF9,0xFFFFFFFD}（考虑 CPU 无 FP，返回 8 字帧），弹栈恢复。也要处理 `pop {pc}` 前 LR 的情况。
   - SVC：可简化成异常注入（SVC 号入栈），或记录日志不跳转（先用日志）。
2. **RCC**：CR 的 HSION/HSIRDY、HSEON/HSERDY、PLLON/PLLRDY、CSSON、HSEBYP；CFGR 的 SW/SWS、HPRE/PPRE、MCO、PLL*；CIR（清标志）；AHBENR/APB2ENR/APB1ENR 门控（写即可，读回原值）；BDCR/CSR；CFGR2/CFGR3；CR2。
   - **关键**：任何 `while(!(RCC->CR & bit))` 等待必须在 1–3 次读后满足（HSIRDY/HSERDY/PLLRDY/LSERDY 自动置位），SWS 必须反映 SW，否则固件死循环。
3. **FLASH**：ACR、KEYR（写 0x45670123/0xCDEF89AB 解锁）、CR(PG/PER/MER/STRT/LOCK)、SR(BSY/EOP/WRPRTERR/PGERR)、AR、OBR、WRPR。
   - 写 CR 触发 STRT 后：立即完成（BSY 先 1 一拍再 0，EOP=1），擦除/编程不真正改 flash（除非调试选项），但记录日志；写 flash 时也更新映射内存方便观察。
   - 固件很可能把参数写到内部 flash 高地址。
4. **GPIO A/B/C/D/F**：MODER/OTYPER/OSPEEDR/PUPDR/IDR/ODR/BSRR/BRR/LCKR/AFR0/AFR1。
   - ODR 写、BSRR 置位/清位、BRR 清位；IDR：输出脚回读 ODR，输入脚取自 `device` 的引脚电平配置。
   - 记录输出变化到 tracer（可被 UI 显示，如 LCD/RF 引脚波形）。
5. **EXTI**：IMR/EMR/RTSR/FTSR/SWIER/PR；当 `device` 改变输入引脚时，按 RTSR/FTSR 生成 PR 位并向 NVIC 投递对应 IRQ（EXTI0_1→IRQ5, EXTI2_3→IRQ6, EXTI4_15→IRQ7）；PR 可写 1 清除。
6. **SYSCFG**：EXTICR1–4（端口选择）、CFGR1（含 MEM_MODE，读取须返回写入值）。
7. **ADC**：ISR(EOC/EOS/OVR/AWD)、IER、CR(ADEN/ADSTART/ADSTP/ADCAL)、CFGR1、CFGR2、SMPR、TR、CHSELR、DR、CCR(0x40012708)。
   - ADEN 后 ADRDY 置位；ADSTART 后按 CHSELR 通道从 `device.adc_value(ch)` 取值写 DR，置 EOC，若 DMA 使能则搬运；转换时间可用固定几条指令/循环粒度近似（例如在 `tick()` 中每 N 周期完成一次）。
   - 通道映射默认：CH0=电池电压, CH1=电容电压, CH2=温度, 其它=0x800，可在 device 配置。
8. **DMA1**：每通道 CCR/CNDTR/CPAR/CMAR；ISR/IFCR 全局标志（GIF/TCIF/HTIF/TEIF 每通道 4 位）。支持 ADC→内存单次/循环模式搬运即可（在 ADC 完成时执行）。
9. **TIM17**（以及按需 TIM1/3/6/7/14/15/16）：CR1(CEN/OPM/ARPE)、DIER(UIE)、SR(UIF)、EGR(UG)、CNT/PSC/ARR/CCR1。CEN 后按周期递增 CNT，溢出置 UIF，UIE 且 NVIC 允许则 pending 对应 IRQ（STM32F0: TIM1=IRQ13, TIM2=15, TIM3=16, TIM6=17, TIM7=18, TIM14=19, TIM15=20, TIM16=21, TIM17=22）。**注意**：向量表里 13/15/19 有真实 handler，需支持对应中断。
10. **PWR**：CR(LPDS/PDDS/CWUF)、CSR(WUF/PVDO/…)；写入记录，WUF 读后可清。
11. **IWDG**：KR(0xAAAA 喂狗/0x5555 解锁/0xCCCC 启动)、PR/RLR/SR；默认**不**因看门狗复位（可通过 device 选项开启）。
12. **RTC**：TR/DR/CR/ISR/PRER/WPR/SSR；简化：CR 写后 ISR 的 RSF/INITS 就绪，秒计数器随 SysTick 增加；日期时间可初始为固定值。
13. **未实现地址**：读返回 0（或 0xFFFFFFFF 视区域），写忽略并记日志 `unimpl`，便于发现遗漏。

### 兜底：卡死诊断
`run()` 若在 N 步内 PC 只在 ≤4 个地址间循环且其中包含 MMIO 读，判定为“等待外设”，生成诊断：
- 循环 PC 范围、等待的地址/寄存器、当前寄存器值；
- 自动执行 `device.resolve_stall(addr)`（RCC/ADC/FLASH/DMA 默认置就绪位）后重试，最多 20 次；
- 仍失败则抛 `StallError` 并输出 `sim/stall_report.json`（含最后 1000 条 trace）。
这是保证“一定能跑到主循环”的关键机制，必须实现。

## 4. 调试器 Python API（稳定契约，webui/server.py 与 cli.py 都只依赖它）

```python
class Machine:
    def __init__(self, bin_path: str, device: Device | None = None, symbols: dict | None = None)
    def reset(self) -> None
    def run(self, max_insns: int = 2_000_000, until_pc: int | None = None,
            until_bp: bool = True, stop_on_stall: bool = True) -> str  # 'breakpoint'|'insn_limit'|'until_pc'|'stall'|'fault'
    def step(self, n: int = 1) -> None
    @property
    def regs(self) -> dict           # {'r0':..,'sp':..,'lr':..,'pc':..,'xpsr':..,'primask':..}
    def read(self, addr: int, size: int) -> bytes
    def write(self, addr: int, data: bytes) -> None
    def set_breakpoint(self, addr: int) -> None
    def clear_breakpoint(self, addr: int) -> None
    def breakpoints(self) -> list[int]
    def watch(self, addr: int, size: int, on: bool) -> None
    def backtrace(self, max_depth: int = 16) -> list[dict]   # 启发式：扫描栈上的代码地址
    def disasm(self, addr: int, count: int = 16) -> list[dict]  # {'addr','bytes','text'}
    def stats(self) -> dict          # insn_count, cycle_estimate, uptime_ms
    def trace(self, since: int = 0, limit: int = 500) -> list[dict]
    def device_state(self) -> dict
    def device_set(self, **kwargs) -> None
    def device_event(self, kind: str, payload: dict) -> None   # 'button'|'rf_rx'|'power'
    def reset_trace(self) -> None
```

事件（tracer）统一格式：`{'seq':int,'type':str,'pc':int,'addr':int|None,'name':str|None,'data':...}`，
type ∈ `call, ret, mmio_read, mmio_write, irq_enter, irq_exit, exception, gpio_change, uart_tx, uart_rx, unimpl, fault, note`。

## 5. CLI

`python -m sim.cli firmware/AD200pro_V1.60.bin` 进入交互式：
```
(dbg) run            # 跑到主循环
(dbg) bp 0x800CDB0
(dbg) c / s [n] / regs / mem 0x20000000 64 / dis 0x800CDB0 16
(dbg) irq            # 当前 NVIC 使能/pending
(dbg) mmio           # 最近 MMIO 访问
(dbg) dev set battery=3.7 cap=330
(dbg) rf 01 23 45
(dbg) trace 50
```

## 6. Web UI（`sim/server.py` + `webui/`）

`python -m sim.server firmware/AD200pro_V1.60.bin --port 8765` → http://127.0.0.1:8765
REST（`Content-Type: application/json`；错误返回 `{"error": "..."}` + 4xx/5xx）：

| 方法/路径 | 请求 | 响应 |
|---|---|---|
| GET `/api/status` | – | `{state, pc, regs, stats, breakpoints, irq:{enabled,pending}}` |
| POST `/api/reset` | – | status |
| POST `/api/run` | `{max_insns?}` | status + `stop_reason` |
| POST `/api/step` | `{n?}` | status |
| POST `/api/pause` | – | status |
| GET/POST `/api/breakpoints` | `{add:[...],remove:[...]}` | `{breakpoints:[...]}` |
| GET `/api/mem?addr=0x..&len=256` | – | `{addr, bytes:[...]}` (+`hex`) |
| POST `/api/mem` | `{addr, bytes:[...]}` | ok |
| GET `/api/disasm?addr=0x..&n=20` | – | `{insns:[{addr,bytes,text}]}` |
| GET `/api/trace?since=0&limit=500` | – | `{events:[...],seq}` |
| GET `/api/symbols` | – | `{symbols:{addr:name}, vectors:[...]}` |
| GET/POST `/api/device` | POST `{battery,cap_voltage,temp,button,...}` | device 状态 |
| POST `/api/device/event` | `{kind,payload}` | device 状态 |
| GET `/api/mmio?addr=` | – | 该地址最近访问 |
| GET `/api/backtrace` | – | 调用栈 |
| GET `/api/console?since=` | – | 固件侧日志文本（若能从 UART/GPIO 解出） |

UI 布局（单页，深浅色均可）：
- 顶部工具条：Reset / Run / Pause / Step / Step N / 状态灯 / 指令数与估算 ms。
- 左：寄存器 + NVIC/中断状态 + 断点列表。
- 中：反汇编视图（当前 PC 高亮，可点击设断点）+ 内存 hex 视图（可编辑）。
- 右：虚拟整机面板（电池/电容电压滑条、按键、RF 发送框、GPIO 波形/状态、ADC 读数）。
- 底：Trace 日志（可过滤类型）、console。
- 轮询 `/api/status` 与 `/api/trace`（200–500ms），暂停时按需刷新内存/反汇编。

## 7. 验收标准

1. `python tests/test_boot.py`：reset 后在 500 万条指令内到达主循环并稳定运行 ≥ 5 个“tick 周期”，
   无未处理异常，`stats().uptime_ms` 随 SysTick 增长（≥10ms）。
2. `python sim/cli.py` 能 `run`、`bp`、`step`、`mem`、`dis`、`dev set`、`trace`。
3. `python -m sim.server` 启动后，浏览器访问能看到 PC/寄存器/反汇编/内存/trace/虚拟面板；Reset/Run/Step/断点可用。
4. `python -m pytest tests/ -q`（或 `python tests/run_all.py`）全部通过。
5. 全程不依赖网络、不需要额外 pip 安装；只用 `..\..\tools\python\python.exe` 里的 capstone/unicorn + stdlib。
