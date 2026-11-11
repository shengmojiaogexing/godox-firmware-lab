# X3 Pro S 补丁点复核与证据（`X3pro_S_v1.11_app.bin` V1.11）

> 设备：Godox **X3 Pro S** 引闪器（Sony 热靴版）｜固件：`devices/x3pros/firmware/X3pro_S_v1.11_app.bin`
> 1,931,868 B（0x1D7A5C）｜sha256 `4b3465cab7975749dd3d109aa1b6f60ee065402869aa88d7b007325def90e103`
> 镜像基址：**`0x08020000`**（`file_offset = VA - 0x08020000`）
>
> 本文记录补丁假设的**静态复核证据**。可复跑脚本：[`scan_checks.py`](scan_checks.py)
> （仓库根执行：`tools\python\python.exe keypatch\dev\x3pros\scan_checks.py`）。
> 模块级 Unicorn 验证见 `keypatch/devices/x3pros.py` 的 `verify()`。

---

## 0. 结论

`scan_checks.py`：**37 项检查 37 PASS / 0 FAIL**；模块 `verify()`：**25 项 PASS / 0 FAIL**
（含原厂镜像自证）；studio UI e2e：**14/14 PASS**。**无整机模拟器、无实机验证。**

补丁设计（原位补丁 + 空闲闪存注入，输出长度不变）：

| # | 地址（VA） | 原始字节 | 改为 | 作用 |
|---|---|---|---|---|
| 1 | `0x0802F0C6` | `4c f2 68 34 13 44 b4 fb f3 f4`（10 B） | `B.W inj_sync` + 3×NOP | 同步字函数入口分流：`ID==99` → `r4 = KEY`；否则重放原厂 `movw r4,#0xC368; add r3,r2; udiv r4,r4,r3`，再 `B.W 0x0802F0D0` 回原厂流程 |
| 2 | `0x0802EE44` | `10 b5 ff f7` | `B.W inj_tx4` | 4 字节帧发送入口：密钥模式「KEY 帧 + ID99(0x01F4) 帧」双发，发完恢复 KEY |
| 3 | `0x0802EEAC` | `10 b5 ff f7` | `B.W inj_tx2` | 2 字节短命令发送入口，同上 |
| 4 | `0x081CA000` | 全 `0xFF`（0x400 B） | 注入代码（232 B） | `inj_sync` / `inj_tx4` / `inj_tx2` / `set_sync` / 两个蹦床 |

用户可见语义：**ID 菜单 = 99 → 密钥模式（双发）；ID = 0..98 → 完全原厂单发。**

---

## 1. 镜像基址 `0x08020000`（[A] 项）

关键证据：**只有 base=0x08020000 能同时让 Reset_Handler 与 fault stub 落到期望字节**。

| 锚点 | 期望字节（file offset） |
|---|---|
| 向量表[0] 初始 SP | `0x20270000`（file 0x0，小端 `00 00 27 20`） |
| 向量表[1] Reset | `0x0803139D` → 代码 `0x0803139C`（file 0x1139C） |
| Reset_Handler 首指令 | file 0x1139C = `df f8 34 d0`（`ldr.w sp,[pc,#0x34]`，取 `0x2001A1C8`） |
| 默认 handler | VA `0x080313EC`（file 0x113EC）= `fe e7`（`b .`） |
| NMI/HardFault/MemManage/BusFault/UsageFault | VA `0x0803022C/30/34/38/3C`（file 0x1022C…）= `fe e7 00 bf`（`b .` + NOP） |
| SVC/DebugMon/PendSV/SysTick | VA `0x08030240/44/48/4C`（file 0x10240…）= `70 47 00 bf`（`bx lr` + NOP） |

反例（同一文件按其它候选基址映射时，两个锚点都不成立）：

| 候选 base | Reset @0x0803139C 落在的字节 | fault stub @0x0803022C 落在的字节 |
|---|---|---|
| 0x08000000 | `cc 9b d3 f8`（非 Reset） | `fc 2f c3 e9`（非 stub） |
| 0x08030000 | `55 f9 6b 1e` | `ed 13 03 08`（向量表数据） |
| **0x08020000** | **`df f8 34 d0`（Reset 首指令）** | **`fe e7 00 bf`（stub）** |

Reset_Handler（0x0803139C）随后：`bl SystemInit(0x08030C48)` → 拷 `.data` → 清 `.bss` →
`bl 0x080AEA90` / `bl 0x0802B188` → `b .`（0x080313D2），是标准 C 启动链。

> 顺带确认：file `0x1A9378`–`0x1D4000` 为擦除态 `0xFF`；file `0x1D4000`–文件尾是
> **Sony 热靴子镜像**（SP=`0x200004F8`、Reset=`0x080020CD`，内含自己的启动代码，
> 独立链接在 `0x08000000`），**本补丁不动它**。

---

## 2. 同步字函数 `0x0802F0C0`（[B] 项）

函数头（file 0xF0C0）：

```text
0x0802F0C0  01 22        movs  r2, #1
0x0802F0C2  03 46        mov   r3, r0          ; r0 = 入参 ID
0x0802F0C4  10 b5        push  {r4, lr}
0x0802F0C6  4c f2 68 34  movw  r4, #0xC368     ; 50024  ← 补丁点从这里开始（10 B）
0x0802F0CA  13 44        add   r3, r2          ; r3 = ID + 1
0x0802F0CC  b4 fb f3 f4  udiv  r4, r4, r3      ; r4 = 50024/(ID+1)
0x0802F0D0  82 b0        sub   sp, #8
0x0802F0D2  23 0a        lsrs  r3, r4, #8
0x0802F0D4  0d f1 07 01  add.w r1, sp, #7
0x0802F0D8  04 20        movs  r0, #4          ; SYNC1
0x0802F0DA  8d f8 07 30  strb.w r3, [sp, #7]
0x0802F0DE  ff f7 5d fb  bl    write_reg(0x0802E79C)
0x0802F0E2  01 22        movs  r2, #1
0x0802F0E4  0d f1 07 01  add.w r1, sp, #7
0x0802F0E8  05 20        movs  r0, #5          ; SYNC0
0x0802F0EA  8d f8 07 40  strb.w r4, [sp, #7]
0x0802F0EE  ff f7 55 fb  bl    write_reg
0x0802F0F2  14 20        movs  r0, #0x14       ; 20 µs
0x0802F0F4  fc f7 c0 fa  bl    delay(0x0802B678)
0x0802F0F8  33 20        movs  r0, #0x33       ; SCAL
0x0802F0FA  ff f7 07 fc  bl    strobe(0x0802E90C)
0x0802F0FE  4f f4 7a 70  mov.w r0, #0x3E8      ; 1 ms
0x0802F102  fc f7 b9 fa  bl    delay
0x0802F106  36 20        movs  r0, #0x36       ; SIDLE
0x0802F108  ff f7 00 fc  bl    strobe
0x0802F10C  4f f4 2f 70  mov.w r0, #0x2BC      ; 700 µs（尾调）
0x0802F110  02 b0        add   sp, #8
0x0802F112  bd e8 10 40  pop.w {r4, lr}
0x0802F116  fc f7 af ba  b.w   delay
0x0802F11A  00 bf        nop
0x0802F11C  10 b5 ...    ← 下一函数开始（0x5C 字节可用）
```

- 补丁点 10 字节在 `0x0802F0C6`，正好覆盖 `movw + add + udiv`；`B.W inj_sync`(4B)+3×NOP 等长替换；
- 可用空间 **0x5C = 92 字节**（到 `0x0802F11C`），注入分流放在空闲闪存，不占函数空间；
- 6 个直接调用点（file）：`0xF1B4`、`0xF1C4`、`0xF2AE`、`0xF786`、`0xF7A6`、`0xF962`
  （设置应用 / 改 ID / RF 初始化路径；其中 `0x0802F1AA` 处 `ldrb r0,[r6,#1]` 从设置区取 ID）。

---

## 3. 发送入口 TX4 / TX2（[C] 项）

| 入口 | VA | 入口 4 字节 | 第 2 条指令 | 作用 |
|---|---|---|---|---|
| TX4 | `0x0802EE44` | `10 b5 ff f7`（push {r4,lr}; bl …） | `bl 0x0802ECE4`（radio_wake） | 4 字节帧：FIFO burst reg 0x7F len=4 → STX(0x35) → 600 µs → 0x3B → SIDLE(0x36) |
| TX2 | `0x0802EEAC` | `10 b5 ff f7` | `bl 0x0802ECE4` | 2 字节短命令：FIFO reg 0x7F len=2 → STX(0x35) → 500 µs → 0x3B → SIDLE(0x36) |

`0x0802ECE4` 为 radio_wake（置忙标志 RAM `0x20001D09` 并返回）；
两函数末尾 `strb r2,[r3]` 清忙标志；帧缓冲 RAM `0x20001D14`（`file 0xEEA4/0xEF14` 字面量）。
`B.W` 注入替换入口 4 字节后，蹦床重建 `push {r4,lr}; bl radio_wake` 再跳回
`0x0802EE4A` / `0x0802EEB2`，原厂组帧/时序 100% 保留。

---

## 4. 辅助函数（[D] 项）

| 函数 | VA | 说明 |
|---|---|---|
| `write_reg(reg, buf, len)` | `0x0802E79C` | file 0xE79C = `2d e9 f0 41`（push.w）；`reg | 0x40` = CC2500 burst 写；`reg 0x44 + len 2` 一次写 SYNC1/SYNC0 |
| `strobe(cmd)` | `0x0802E90C` | file 0xE90C = `70 b5`（push {r4,r5,r6,lr}） |
| `delay(n)` | `0x0802B678` | 忙等延时：file 0xB678 起 `ldr r3,=busy; cbz r0; loop: subs r0,#1; … bne` |

---

## 5. SYNC1/SYNC0 运行期写点唯一性（[E] 项）

用「所有 `bl write_reg(0x0802E79C)` 调用点 + 调用前 `r0` 立即数」全量统计（80 个调用点）：

| 寄存器 | 写点（file） | 值 |
|---|---|---|
| reg 4 (SYNC1) | `0xE9D0`（CC2500 初始化表） | `0xC3` |
| reg 4 (SYNC1) | `0xF0DE`（本同步字函数） | `sync>>8` |
| reg 5 (SYNC0) | `0xE9E2`（CC2500 初始化表） | `0x68` |
| reg 5 (SYNC0) | `0xF0EE`（本同步字函数） | `sync&0xFF` |

→ **全镜像只有初始化表 + 本函数写同步字**；初始化表写的是缺省值，随后被同步字函数覆盖。
（初始化表位于 `0x0802E9xx`：file 0xE9C2 `movs r2,#0xC3`、0xE9CC `movs r0,#4`、
0xE9D0 `bl write_reg`；0xE9D4 `movs r2,#0x68`、0xE9E2 `bl write_reg`。）

---

## 6. 设置结构与 ID 上限（[F] 项）

- 设置结构 RAM **`0x20001768`**：`[+0] = 频道`、`[+1] = ID`。
  证据：file `0xF1E8` 字面量 = `0x20001768`；`0x0802F1AA` `ldrb r0,[r6]`（频道）→
  `bl 0x0802F048`（设信道）；`ldrb r0,[r6,#1]`（ID）→ `bl 0x0802F0C0`（设同步字）。
- 无线帧里 ID 字段 = `settings[+1]+1`（`0x08025E2C` 附近 `ldrb r0,[r0,#1]; adds r1,#1; bfi …`），
  与原厂公式 `50024/(ID+1)` 一致。
- **ID 菜单上限 = 99**：file `0x9A0E` 起是 ID 加减 handler：`ldrb r3,[r2,#1]`（ID）→
  `+1`/`+10` 步进 → file `0x9A1C: cmp r3,#0x63` → `it hi; movhi r3,#0x63`（钳位）→
  `strb r3,[r2,#1]`。→ 用户可以把 ID 设到 99，作为密钥模式开关。

---

## 7. 注入区安全性（[G] 项）

- 注入区 `0x081CA000..0x081CA3FF`（file `0x1AA000..0x1AA3FF`）**1024 B 全 0xFF**；
- 所在大段 `0xFF` = file `0x1A9378..0x1D4000`（VA `0x081C9378..0x081F4000`）；
- **全镜像 4 字节对齐字面量**（0x08000000–0x08200000 范围）没有一个落在注入区（命中 0）；
- **全镜像 B/BL 分支目标**没有一个落在注入区（命中 0）；
- file `0x1D4000` 起是 Sony 热靴子镜像（SP=`0x200004F8`、Reset=`0x080020CD`）——
  与本镜像共享 flash 但独立运行，**不能覆盖**；注入区离它 0x8000 字节以上。

本模块实际只用注入区前 **232 字节**，其余保持 `0xFF`（`verify()` 有断言）。

---

## 8. 模块级 Unicorn 验证（`keypatch/devices/x3pros.py::verify`）

命令：

```powershell
tools\python\python.exe keypatch\keypatch.py patch --device x3pros --key 0xD798 --verify --out %TEMP%\kp_x3pros
```

**25 项 PASS / 0 FAIL**，覆盖：

- 长度不变 + 注入区字节/剩余 0xFF + 3 个入口补丁字节；
- 与原厂差异 **240 字节**，全部落在 4 个补丁点；
- Unicorn 调用级（桩替换 `write_reg/strobe/delay/radio_wake/GPIO`）：
  - `set_sync`：补丁版 ID=99 → `SYNC=0xD798`；ID=0/1/37 → 原厂 `0xC368/0x61B4/0x0524`；
    原厂镜像自证同 ID → 原厂公式（ID=99 → `0x01F4`）；
  - TX4（4 字节帧）ID=99：`SYNC=KEY → FIFO(4) → SYNC=0x01F4 → FIFO(4) → SYNC=KEY`；
    原厂镜像 ID=99：单发；
  - TX2（2 字节短命令）同序列（FIFO(2)）；
  - 普通模式 ID=0/5：补丁版与原厂一致，单发。
- 全设备总自检：`tools\python\python.exe keypatch\selftest.py --key 0xD798` → **ALL PASS**；
- studio API + UI e2e：`/api/generate`（x3pros，verify=True）25/25 PASS；
  `node studio\tests\test_ui.mjs --only x3pros` → **14/14 PASS**，截图
  [`docs/images/x3pros_studio.png`](../../docs/images/x3pros_studio.png)。

---

## 9. 风险与未验证项

1. **无实机验证**：同步字空口过滤必须靠真机 CC2500 硬件验证；本仓库只有静态 + Unicorn 调用级证据。
2. **无整机模拟器**：未做上电/菜单/发送路径的整机模拟。
3. **双发时序**：2 字节短命令原厂每帧连发 3 次，密钥模式每帧多一次约 500 µs 发射；
   若实机出现 HSS/连拍异常，优先怀疑此处（`DUAL_SEND=False` 可退回单发 KEY 模式）。
4. **两次发射之间改同步字未关中断**：射频中断发送路径由原厂忙标志接管，
   窗口约几十~几百 µs，属已知残余风险。
5. **SCAL/延时保留**：同步字函数原厂收尾（20 µs → SCAL(0x33) → 1 ms → SIDLE(0x36) → 700 µs）
   完全保留；注入的双发在两次发帧之间只写 SYNC、不跑 SCAL，与 C 版设计一致。
6. **G3 容器**：是否需要容器格式待实机确认；模块提供
   `X3pro_S_v1.11_app_key<KEY>_container.bin` 备用件（本机验证 MD5/魔数通过）。
7. **版本绑定**：`apply()` 强校验长度 + sha256 + 每处原始字节，换固件版本会直接拒绝。

---

## 10. 复跑

```powershell
# 静态复核（37 项）
tools\python\python.exe keypatch\dev\x3pros\scan_checks.py

# 模块补丁 + 离线验证（25 项）
tools\python\python.exe keypatch\keypatch.py patch --device x3pros --key 0xD798 --verify --out %TEMP%\kp_x3pros

# 全设备总自检
tools\python\python.exe keypatch\selftest.py --key 0xD798
```
