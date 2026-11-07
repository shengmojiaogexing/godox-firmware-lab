# webui ↔ emulator API 契约

本文档是 `webui/` 对 `emu/server.py` 的接口期望，与 `docs/INTERFACES.md` 及
`emu/README.md` 对齐。UI 对字段命名做了**双向兼容**（下文的"兼容写法"），因此
emulator 侧不必为了 UI 改字段名。

* 服务地址：默认 `http://127.0.0.1:8765`，可用页面参数 `?api=http://host:port` 覆盖。
* 传输：JSON；`POST` 使用 `Content-Type: application/json`。
* **CORS（必须）**：所有响应带 `Access-Control-Allow-Origin: *`，`OPTIONS` 预检返回 204
  且允许 `Content-Type`。否则页面（8080）无法访问 8765。
* 超时：UI 侧 3.5s；失败不清空界面，进入"未连接"横幅并可重试/切 mock。
* 轮询：运行中 200ms、暂停 800ms、失败 2s、页面隐藏 2.5s；`/api/screen` 每 1.5s。
* 地址参数一律用 `0x` 前缀十六进制字符串（如 `addr=0x080030D4`），对
  `int(x,16)` / `int(x,0)` 两种解析都安全。

---

## 1. `GET /api/state`

一次返回调试所需的全部状态。

```json
{
  "pc": 134230228, "sp": 536874192, "lr": 4294967295, "xpsr": 17825811,
  "regs": [0,0,0,0,0,0,0,0,0,0,0,0,0, 536874192, 4294967295, 134230228],
  "insnCount": 2000003, "running": false,
  "breakpoints": [134230314],
  "log": ["[  3145930] 0800A4BC  GPIOB       BSRR         W 0x00000040  sz=4  -"],
  "faults": [],
  "uart": "", "uartLen": 0,
  "func": { "addr": 134230228, "name": "func_0800A4B4" },
  "callstack": [{ "addr": 134230228, "name": "func_0800A4B4", "return_pc": 134230190 }],
  "lcd": { "w": 128, "h": 64, "format": "1bpp", "addr": null, "bytes_written": 12866,
           "frames": 13, "page": 1, "col": 2, "start_line": 0 },
  "stats": { "executed": 2000003, "runTime": 1.2 },
  "peripherals": { "...": "未使用，UI 不依赖" }
}
```

| 字段 | 必需 | UI 说明 |
|---|---|---|
| `pc` | 是 | 当前 PC（Thumb 地址，可为奇数；反汇编窗口按 `pc & ~1` 对齐） |
| `sp` / `lr` | 是 | 若缺失则回退 `regs[13]/regs[14]` |
| `regs` | 是 | **`regs[16]`**（r0–r15）或 **`regs[13]`**（仅 r0–r12，sp/lr/pc 用独立字段）都支持；也接受对象 `{r0:..,sp:..}` 以及顶层 `r0..r12` |
| `xpsr` | 是 | 标志取 bit31 N / bit30 Z / bit29 C / bit28 V；也兼容 `flags:{N,Z,C,V}` 或字符串 `"NZCV"` |
| `insnCount` | 是 | 兼容 `instructionCount` / `count` / `instructions` |
| `running` | 是 | 兼容 `isRunning` / `is_running` / `state:"running"` |
| `breakpoints` | 可选 | 数字或 `"0x..."` 字符串数组；也兼容 `bps` / `bp`。UI 添加/删除断点后以此为准 |
| `log` | 可选 | **外设访问日志**。UI 支持两种元素：① emu 的字符串行（UI 会解析成 时间/PC/外设.寄存器/读写/值）；② 对象 `{dir,addr/address,reg/register,value,pc,note}`。也兼容 `mmio` / `trace` / `events` 字段 |
| `faults` | 可选 | 数组（元素 `{type,pc,lr,addr,text}`）或数字；顶栏显示异常数，新增时弹提示 |
| `uart` | 可选 | 串口文本（字符串或字符串数组）；未提供时 UI 会从 `log` 里 USART1_TDR 写操作推导 |
| `func` | 可选 | `{addr,name}` 或字符串；缺失时 UI 用 `/api/functions` 或反汇编启发式求 PC 所在函数名 |
| `callstack` | 可选 | `[{addr,name}]`；缺失时显示"模拟器未提供调用栈" |
| `lcd` | 可选 | `{w,h,format,page,col,frames,addr,nonzero,bytes_written,...}`；供屏幕未就绪时的占位信息，`addr` 为真实帧缓冲（emu 当前 0x200004CC） |
| `stats` | 可选 | `{executed,runTime,ipc?}`；UI 优先用 `stats.ipc`，否则自行按两次轮询差值算 IPC |
| `pcName` | 可选 | 当前 PC 名字（如 `0x0800D008` / `func_0800A4B4`），UI 状态栏显示 |
| `taskPhase` | 可选 | `boot` / `scheduler` / `main_loop` / `fault`，UI 映射为「启动中/主循环 1ms/主循环/异常」 |
| `inMainLoop` | 可选 | `true/false`：是否已进入主循环（开机成功判据） |
| `virtualTicks` | 可选 | 虚拟 1ms tick 计数，UI 显示 `tick=N` |
| `rf` | 可选 | 虚拟 CC2500 状态：`{present,state,gdo0,rx_fifo,tx_fifo,rx_count,tx_count,last_rx,last_tx,freq2,pktlen,patable,recent}`；state 为 `IDLE/RX/TX` |
| `rfInitialized` | 可选 | RF 是否已初始化（boot_interactive 默认带 RF） |
| `irqCounts` / `irq_counts` | 可选 | IRQ 派发计数，key 为 Cortex-M0 异常号：**15=SysTick、35=TIM14、21=EXTI0_1、23=EXTI4_15/RF**；UI 两行显示 |
| `irqLog` | 可选 | 最近 IRQ 进入记录 `[{insn,irq,handler,source}]`，source 如 `SysTick/TIM14/GDO0`；UI 显示最后 2 条 |
| `inIsr` / `realIrq` / `tim14Count` | 可选 | 当前是否在 ISR / 真实 IRQ 是否开启 / TIM14 计数 |
| `inputs` | 可选 | **虚拟输入回显**（电量 / 按键状态）。见 §4.1；缺失时 UI 仍可发送命令，并显示"本地发送"状态；命令响应返回 `inputs` 时 UI 也会立即回显 |
| `lastScreenSave` | 可选 | 最近一次 `screen_save` 的 PNG 路径；UI 显示在 LCD 面板下方 |
| `nearestFunc` | 可选扩展 | `{addr,name}` 最近的下一个函数；缺失时 UI 从函数表推导 |

兼容的包装：`{state:{...}}`、`{data:{...}}`。

日志字符串行格式（UI 解析用的正则）：

```
[  3145930] 0800A4BC  GPIOB       BSRR         W 0x00000040  sz=4  func_08007426@08007426
   ^时间       ^PC       ^外设      ^寄存器      ^读写 ^值        ^注释
```

UI 渲染为 `W  GPIOB.BSRR  0x00000040  @0800A4BC`，过滤框可输入 `GPIOB`、`BSRR`、
`0x40020418` 等片段。

---

## 2. `GET /api/disasm?addr=0x080030D4&n=61`

> UI 同时发送 `count=61&n=61` 两个参数（兼容不同实现），任选其一解析即可；`n` 默认 61（PC ±30）。

```json
{
  "addr": 134230228,
  "lines": [
    { "addr": 134230228, "bytes": "0448", "mnem": "ldr", "op": "r0, [pc, #0x10]" },
    { "addr": 134230230, "bytes": "8047", "mnem": "blx", "op": "r0" }
  ]
}
```

* `addr` 为窗口中心地址（Thumb 奇数也行，UI 自行对齐）；`lines` 需覆盖 `addr` 附近 ±30 行。
* 兼容：`lines` 也可为字符串数组 `"080030D4  0448  ldr r0, [pc, #0x10]"`；
  字段名兼容 `mnemonic`、`op_str/operands`、`hex/encoding`；`bytes` 可为数组 `[0x04,0x48]`。
* UI 会按 `addr` 排序去重；行格式固定渲染为 `地址 字节 助记符 操作数`。

---

## 3. `GET /api/mem?addr=0x20000000&len=256`

```json
{ "addr": 536870912, "hex": "d00c0020d5300008...", "len": 256, "ascii": "... .0.." }
```

* `hex` 为连续十六进制串（大小写、空格皆可）；UI 也兼容 `bytes` 数组或 `{data:{hex}}` 包装。
* `len` 建议支持到 4096；UI 一次请求 256 字节，翻页 ±256B，默认显示 PC 附近。

---

## 4. `POST /api/cmd`

请求体：`{"cmd": "...", ...参数}`；响应：`{"ok": true, ...}`。HTTP 非 2xx 或 `ok:false`
视为失败（UI 弹 toast 并回滚乐观操作）。

| cmd | 参数 | UI 用途 |
|---|---|---|
| `run` | 可选 `count`（执行 N 条后自动暂停） | Run 按钮 / F5 |
| `pause` | — | Pause 按钮 / F5 |
| `step` | 可选 `n` | Step / F10 |
| `step_over` | — | Step Over；若服务器 404/不支持，UI 自动回退为 `step` |
| `reset` | — | Reset / F8 |
| `bp_add` | `addr`（数字） | 点断点列添加；UI 额外附带 `addr_hex`（可忽略） |
| `bp_del` | `addr` | 点断点列删除 |
| `set_pc` | `addr` | 预留（当前 UI 的跳转只移动视图，不改 PC） |
| `set_reg` / `set_mem` / `inject` | emu 扩展 | UI 暂未使用 |
| `battery` / `key` / `key_up` / `key_press` / `clear_inputs` / `screen_save` | 见 §4.1 | 虚拟输入面板 / 保存画面 / 清除输入 |
| `jump_main` / `boot_interactive` | 见 §4.1 | 工具栏「⏻ 开机」按钮（boot_interactive，失败时回退 jump_main/状态检查） |
| `rf_init` / `rf_rx` / `rf_status` | 见 §4.1 | 2.4G 射频面板（初始化 / 注入配对包 / 任意 hex / 刷新） |

断点地址 UI 以 JSON 数字发送（Python 侧 `int(body["addr"])` 可直接用）；也接受字符串。

---

## 4.1 虚拟输入（按键 / 电量 / 保存画面）

UI 的「虚拟输入」面板会发送下面这些命令。**命令名与 key 名以 `emu/server.py`
最终实现为准**；UI 侧已按此命名，并会在失败时 toast 提示后端错误。

| cmd | 请求体 | 说明 |
|---|---|---|
| `battery` | `{"cmd":"battery","mv":3700}` | 注入电池电压，整数 mV（2600–4200），UI 滑块步进 10mV；响应含 `battery_mv/battery_pct/raw` |
| `adc` | `{"cmd":"adc","ch":10,"mv":1000}` | 直接注入 ADC 通道电压（emu 扩展，UI 暂未使用） |
| `key` | `{"cmd":"key","key":"power","down":true}` | 按键按下（`down:false` 等价于松开，UI 用 `key_up`） |
| `key_up` | `{"cmd":"key_up","key":"power"}` | 按键松开 |
| `key_press` | `{"cmd":"key_press","key":"power","ms":80}` | 脉冲按键，默认 80ms，响应含 `release_at` |
| `clear_inputs` | `{"cmd":"clear_inputs"}` | 清空虚拟输入（UI「清除输入」按钮） |
| `jump_main` | `{"cmd":"jump_main","warmup":1000000}` | **emu 扩展（交互必需）**：跳过启动动画、预热后落到 1ms 主调度器 `0x0800D008`；之后再 `run` 按键扫描才会响应注入 |
| `boot_interactive` | `{"cmd":"boot_interactive","warmup":1000000}` | **emu 扩展（UI「⏻ 开机」按钮）**：一步 `reset → jump_main → run settle`，返回 `{ok,pc,insn,pcName,taskPhase,inputs,lcd,rf,rfInitialized,irqCounts}`；`{"rf":true}` 默认带 RF |
| `rf_init` | `{"cmd":"rf_init"}` | 手动调用固件 RF 初始化（0x08004AF0），返回 `{ok,rf}`；若后端无此命令，UI 会 1.5s 后查 `/api/state`，`rf.state==="RX"` 即算成功 |
| `rf_rx` | `{"cmd":"rf_rx","hex":"A9 50 B4 02"}` | 注入 2.4G 包（GDO0→EXTI15→IRQ7），返回 `{ok,len,rf,irqCounts}`，UI toast + 刷新 |
| `rf_status` | `{"cmd":"rf_status"}` | 返回 `{ok,rf}`；UI 面板「刷新」按钮 |
| `irq_status` | `{"cmd":"irq_status"}` | 返回 `{ok,irqCounts,irqLog,inIsr,realIrq,tim14Count}`（emu 扩展，UI 暂未直接使用） |
| `screen_save` | `{"cmd":"screen_save"}` | 把当前 LCD 帧保存为 PNG；响应含 `path/file/saved/bytes/frames/nonzero`，UI 显示路径 |

**key 名约定**（已按 `emu/core.py` 的 `VirtualInputs` 实现确认）：`power`（电源）、
`key2`（备用/EXTI）、`mode`（模式，映射矩阵 k0）、`set`（设置，k1）、`up`（上，k2）、
`down`（下，k3）；后端另支持原始矩阵键 `k0..k9` 及别名 `pwr/pwrkey/key1/aux/spare`
（`k8=PD4` 暂未被固件识别）。UI 的 6 个按钮即上述 6 个；若后端调整命名，
改 `app.js` 顶部 `KEY_DEFS` 即可。真实固件需要先 `jump_main` 进入按键扫描循环，
否则注入的键不会被固件处理（UI 仍会显示注入状态）。

**`/api/state.inputs` 实际形状（emu 最新）**：

```json
{
  "keys": {
    "power": false, "key2": false, "mode": true, "set": true, "up": false, "down": false,
    "matrix": { "k0": true, "k1": true },
    "all": { "k0": true, "k1": true }
  },
  "pressed": ["k0", "k1"],
  "battery_mv": 3900,
  "battery_pct": 81,
  "adc": { "ch10": 1000 },
  "last": { "type": "key_down", "key": "k0", "t": 1793985108.67 },
  "events": 5,
  "press_ms_scale": 8000
}
```

兼容写法：`batteryMv/battery/vbat_mv`、`keys` 数组、`pressed/pressed_keys/held`、
`last/last_event/lastKey`、`adc/adc_value`、`screen_save/last_save`。
UI 显示内容：电量 mV/%、按下键、最近事件、最近保存路径；后端未提供 `inputs` 时
显示"未提供"，并用「本地发送」行回显最近一次已发送的命令（便于联调）。
`screen_save` 保存成功时路径也会显示在 LCD 面板标题下方（下一次轮询会通过
`/api/state.lastScreenSave` 继续显示）。

---

## 5. `GET /api/screen`

```json
{
  "w": 128, "h": 64, "format": "1bpp",
  "hex": "00ff...（2048 hex 字符 = 1024 字节）",
  "data": [0, 0, 255, ...],
  "info": { "w": 128, "h": 64, "format": "1bpp", "addr": null,
            "bytes_written": 12866, "frames": 13, "page": 1, "col": 2, "start_line": 0 }
}
```

* UI 优先用 `data`（字节数组），其次 `hex` / `base64`。
* emu 当前：`addr` = 真实帧缓冲 `0x200004CC`，`w=128,h=64,format="1bpp"`，`hex/data`
  就是该 RAM 的 1024 字节 page/column 原始数据。`screen_save` 生成的 PNG 会做
  rotate90+mirror、RGB、6× 放大便于人眼查看，**UI 不读 PNG**，只画原始 1bpp。
* 支持格式：`gray8`（w×h）、`1bpp`、`rgb565`（2 字节/像素）、`rgb888`、`rgba`。
* **1bpp page/column（emu 当前格式）**：一屏 = `w × ceil(h/8)` 字节，布局为
  `byte_index = page * w + col`，字节内 `bit0` 是该 page 顶部的行（SSD1306 约定）。
  UI 通过 `info.page/info.col` 或 `format` 含 `page` 自动识别；若只给 `{w,h,format:"1bpp"}`，
  则按线性 MSB-first 位图处理。
* **面板方向**：UI 默认对 page/column 1bpp 应用 `rotate90 + mirror`（与
  `emu/main.py --screen` 默认一致，显示为 64×128 竖屏）；`?screen=raw` 或 LCD 面板
  「↻ 方向」按钮可切回未旋转的 128×64。emu 的 `screen_save` PNG 目前仍是未旋转的
  page/column 方向，UI 不读取该 PNG，只画 `/api/screen` 的原始字节。
* `lcd.frames` / `info.frames` 用来在屏幕标题显示帧数。
* 无数据（404 或字段不全）时显示占位提示，不影响其他面板。

---

## 6. `GET /api/functions`（可选扩展）

```json
[ { "addr": 134230208, "end": 134230210, "size": 2, "name": "func_080030C0" } ]
```

* UI 兼容数组或 `{functions:[...]}`；成功后在寄存器面板显示「PC 函数 / 最近函数」，
  并用于反汇编窗口的函数归属。
* 未提供时：优先用 `/api/state.func`，否则用反汇编窗口的 `pop {..., pc}`/`bx lr`
  边界做启发式命名 `sub_XXXXXXXX`（界面同样可用）。

---

## 7. Mock 模式（`?mock=1`）

`mock.js` 实现与上面完全相同的 6 个端点（`mock.fetch(path, opts)` 返回 fetch 风格的
Promise 响应），并额外提供 `mock.state()/disasm()/mem()/screen()/functions()/cmd()`
给 Node 自测直接调用。差异仅在于数据是 UI 演示夹具：

* 图像 128×64、`format:"1bpp"`、page/column、base64 `data`，`info.page=0`；
  mock 面板设计按 64×128 物理方向绘制后转置打包，UI 旋转后显示为正立；
* 反汇编 `0x08003000..` 为真实固件字节，`Reset_Handler=0x080030D4`、`main=0x0800CF74`；
  夹具范围外按地址哈希确定性合成（滚动/跳转时同一地址永远同一行）；
* 断点、单步、运行（到断点自动暂停）、复位、`boot_interactive`/`jump_main`、
  `battery/key/key_up/key_press/clear_inputs/screen_save`、
  `rf_init/rf_rx/rf_status/irq_status` 均有模拟（rf state 初始 RX，`rx_count/rx_log/irqCounts` 可增长）；
* `/api/state` 同样返回 `rf`、`irqCounts`/`irq_counts`、`irqLog`、`inIsr`、`realIrq`、`tim14Count`；
* `insnCount`、IPC、faults、callstack、`pcName/taskPhase/inMainLoop/virtualTicks` 为演示值。

---

## 8. 错误处理约定

| 场景 | UI 行为 |
|---|---|
| 首次连接失败 / 超时 | 顶部黄色横幅 + 重试按钮 + 切换 MOCK；各面板保留占位或最后一次数据，不白屏 |
| `/api/disasm` 失败 | 反汇编区显示错误文本，其他面板不受影响 |
| `/api/screen` 404 | LCD 显示占位（如 `/api/state.lcd` 已知则显示 "128×64 · 1bpp（等待帧数据）"） |
| 命令被拒绝（非 2xx / `ok:false`） | toast 提示；断点做乐观更新后失败会回滚 |
| `/api/functions` 404 | 自动退化到状态字段/启发式，不报错 |
| 页面隐藏 | 轮询降频到 2.5s；重新可见立即刷新 |

---

## 9. 已知扩展点（未来）

* `GET /api/regs`、断点条件、执行回溯等 UI 暂未使用；
* 若模拟器支持 `run_to(addr)`，Step Over 可改为"临时断点 + run"以覆盖无符号调用；
* 屏幕若改成 132 列 SSD1306（有 2 像素列偏移）需在 UI 侧加横向偏移参数。
