# 神牛 X3 Pro S 固件逆向档案（V1.11 · 私有密钥补丁 · ID=99 开关 + 兼容双发版）

> 目标固件：`X3pro_S_v1.11_app.bin`（**1,931,868 字节**，sha256 `4b3465ca…`）
> 目标设备：神牛（Godox）**X3 Pro S** 无线引闪器（Sony 热靴版；Artery AT32F435/437 类
> Cortex-M4F，2.4 GHz CC2500 兼容射频）
> 本目录内容：X3 Pro S 的静态逆向证据、「ID → CC2500 同步字」补丁点分析、
> **ID 菜单 ID=99 = 密钥开关**语义、兼容普通灯的双发设计、补丁点总表、离线验证设计与实机待办。
>
> 本目录位于仓库 `devices/x3pros/`；仓库根的便携 Python 为 `tools\python\python.exe`
> （含 capstone 5.0.7 / unicorn 2.1.4）。
>
> ⚠️ **状态：静态 + Unicorn 调用级（离线）；无整机模拟器、无实机验证。**
> 只研究你本人合法拥有的设备；刷机前务必确认官方 G3 能识别设备、手上有同版本原厂固件可回滚。
> 原厂固件二进制**不随仓库分发**（`.gitignore` 已忽略 `*.bin`），见 [`firmware/README.md`](firmware/README.md)。

---

## 0. 一句话结论

X3 Pro S 的「ID → 同步字」函数是 **`0x0802F0C0`**（**92 字节**，到下一函数 `0x0802F11C` 为止），
它是**全镜像唯一**在运行期写 CC2500 `SYNC1/SYNC0`（寄存器 4/5）的地方
（另一处是初始化表 `0x0802E9D0`/`0x0802E9E2`，写的是缺省值 `0xC3/0x68`，随后会被它覆盖）。

**密钥开关复用设备自带的 ID 菜单**（不改 UI）：

- **`ID = 99` → 密钥模式**：每帧**双发** —— 先用 `SYNC = 私有 KEY` 发一遍
  （引闪已打补丁的密钥灯），再用 `SYNC = floor(50024/100) = 500 (0x01F4)` 发一遍
  （引闪同频道、`ID=99` 的原厂灯，兼容普通灯）；发完把同步字恢复为 KEY。
- **`ID = 0..98` → 普通模式**：完全原厂行为（`SYNC = 50024/(ID+1)`，单发）。

补丁分 **3 类、4 个写入点**（原位改写 + 空闲闪存注入，**输出长度不变**）：

1. 同步字函数 `0x0802F0C6` 处 **10 字节**改写为 `B.W inj_sync` + 3×NOP；
   `inj_sync` 在 ID==99 时把 `r4` 置为 KEY，否则重放原厂 3 条指令后跳回 `0x0802F0D0`；
2. 两个低层发送入口 `0x0802EE44`（TX4，4 字节帧）与 `0x0802EEAC`（TX2，2 字节短命令）
   各 4 字节改写为 `B.W` 注入包装（密钥模式双发 / 普通模式原厂单发）；
3. 注入代码写在主固件尾部空闲闪存 **`0x081CA000` 起 0x400 字节**（原厂全 `0xFF`）。

| 项目 | 结论 |
|---|---|
| 固件 | `devices/x3pros/firmware/X3pro_S_v1.11_app.bin`，1,931,868 B（0x1D7A5C） |
| sha256 | `4b3465cab7975749dd3d109aa1b6f60ee065402869aa88d7b007325def90e103` |
| 镜像基址 | **`0x08020000`**（`file_offset = vaddr - 0x08020000`） |
| 镜像范围 | `0x08020000–0x081F7A5C`（1,931,868 B） |
| 向量表 | 初始 SP = **`0x20270000`**；Reset = **`0x0803139D`** → 处理函数 **`0x0803139C`** |
| 补丁形态 | 同步字函数 10 B 分流改写 + 2 个发送入口 4 B `B.W` 注入 + `0x081CA000` 注入区 |
| 开关语义 | **ID=99 → 密钥模式（KEY 帧 + ID99 普通帧双发）；ID=0..98 → 原厂单发** |
| 输出命名 | `X3pro_S_v1.11_app_key{key:04X}.bin`；G3 容器备用件 `X3pro_S_v1.11_app_key{key:04X}_container.bin` |
| 设备元数据 | id `x3pros`、name `Godox X3 Pro (X3 Pro trigger, S)`、kind `trigger`、model `X3 Pro S`、firmware_version `V1.11` |
| 补丁模块 | [`keypatch/devices/x3pros.py`](../../keypatch/devices/x3pros.py)（S 版；行为设计与 C 版 `x3pro.py` 同源，地址常量不同） |
| 验证状态 | 静态 + Unicorn 调用级（离线）；--verify **25 项 PASS / 0 FAIL**（含原厂自证），静态复核 37 项 PASS；整机模拟器**未建立**；实机**未验证** |

---

## 1. 固件与镜像事实

| 项 | 结论 | 证据 |
|---|---|---|
| 文件 | `firmware/X3pro_S_v1.11_app.bin`，1,931,868 B（0x1D7A5C） | `Get-FileHash` / `keypatch.py list` |
| sha256 | `4b3465cab7975749dd3d109aa1b6f60ee065402869aa88d7b007325def90e103` | 同上 |
| 来源 | 用户提供的官方升级包固件（只读副本） | [`firmware/README.md`](firmware/README.md) |
| 镜像基址 | `0x08020000` | 向量表初值与代码地址全部落在 `[0x08020000, 0x081F7A5C]` |
| 镜像占用 | `0x08020000–0x081F7A5C`（1,931,868 B） | 文件长度 + 基址 |
| 前缀区 | `0x08000000–0x0801FFFF` **不在此 `.bin` 内**（本镜像从 `0x08020000` 开始） | 文件起点 + 向量表 |
| 向量表初值 | 初始 SP = **`0x20270000`**；Reset = **`0x0803139D`** → 处理函数 `0x0803139C` | 镜像前 8 字节（小端） |
| Reset_Handler | `0x0803139C`：`ldr.w sp,[pc,#0x34]` → `bl SystemInit(0x08030C48)` → 拷 `.data` → 清 `.bss` → `bl 0x080AEA90` / `bl 0x0802B188` → `b .`（`0x080313D2`） | 反汇编 |
| 默认 handler | `0x080313EC` 为 `b .`；NMI/HardFault/MemManage/BusFault/UsageFault stub 在 `0x0803022C/30/34/38/3C`（各 `b .`）；SVC/PendSV/SysTick 在 `0x08030240/44/48/4C`（`bx lr`） | 反汇编 |
| MCU | **Cortex-M4F 类**（Thumb-2 + 硬件 `UDIV` + FPU 指令）；Artery AT32F435/437 类，与 C 版 V1.22 同族 | 反汇编（`movw`/`udiv`/`mov.w` 等 32 位指令）+ 同族对照 |
| Flash | **2 MB flash 窗口**（镜像地址落在 `0x08020000–0x081F7A5C` 内；容量未实测，按向量表/地址空间判为 2 MB 窗口） | 向量表 + 地址范围 |
| RAM | 起始 `0x20000000`；初始 SP = `0x20270000` 落在 RAM 内（**容量未实测**） | 向量表初值 |
| 射频 | **CC2500 兼容**（`write_reg`/`strobe`/`delay` 软件 SPI；`SYNC1/SYNC0 = reg 4/5`；同步字公式 `50024/(ID+1)`） | §3、§4 |
| 设置结构 | RAM **`0x20001768`**：`[+0]=频道`、`[+1]=ID`；ID 菜单范围 **0..99**（上限钳位证据见 §3.4） | §3.4 |
| 固件内版本/机型串 | file offset **0x99E50 附近**：`v1.11`（0x99E54）、`X3Pro  S`（0x99E5D） | 字符串扫描 |
| 热靴子镜像 | file **0x1D4000–0x1D7A5C**（14,940 B）是 Sony 热靴 MCU 固件 blob，独立链接在 `0x08000000`，含自己的向量表/Reset/SystemInit；**本补丁不动它** | §9（注入区边界） |

> Flash/RAM 的「容量未实测」不是保守措辞：本轮只做了与密钥补丁有关的静态分析
> （启动链、同步字函数、CC2500 写点、发送入口、空闲注入区），没有做时钟/外设/显示/参数区的完整逆向。
> 「Cortex-M4F / AT32F435/437 类」是静态结论（32 位 Thumb-2 指令、硬件除法、栈顶在 0x2027xxxx、
> 与 C 版 V1.22 同族），**不是**从芯片丝印或 SWD 读取得到的。

---

## 2. 核心补丁点：同步字函数 `0x0802F0C0`

### 2.1 原厂反汇编（92 字节）

`0x0802F0C0` 起、到下一函数 `0x0802F11C` 为止，共 **92 字节**：

```asm
0x0802F0C0  01 22        movs  r2, #1
0x0802F0C2  03 46        mov   r3, r0             ; r3 = 入参 ID
0x0802F0C4  10 b5        push  {r4, lr}
0x0802F0C6  4c f2 68 34  movw  r4, #0xC368        ; 50024（原厂分子）
0x0802F0CA  13 44        add   r3, r2             ; r3 = ID + 1
0x0802F0CC  b4 fb f3 f4  udiv  r4, r4, r3         ; r4 = 50024 / (ID+1)
0x0802F0D0  82 b0        sub   sp, #8
0x0802F0D2  23 0a        lsrs  r3, r4, #8         ; SYNC1 = sync >> 8
0x0802F0D4  0d f1 07 01  add.w r1, sp, #7         ; 1 字节栈缓冲
0x0802F0D8  04 20        movs  r0, #4
0x0802F0DA  8d f8 07 30  strb.w r3, [sp, #7]
0x0802F0DE  ff f7 5d fb  bl    0x0802E79C         ; write_reg(4, [sp+7], 1)
0x0802F0E2  01 22        movs  r2, #1
0x0802F0E4  0d f1 07 01  add.w r1, sp, #7
0x0802F0E8  05 20        movs  r0, #5
0x0802F0EA  8d f8 07 40  strb.w r4, [sp, #7]      ; SYNC0 = sync & 0xFF
0x0802F0EE  ff f7 55 fb  bl    0x0802E79C         ; write_reg(5, [sp+7], 1)
0x0802F0F2  14 20        movs  r0, #0x14
0x0802F0F4  fc f7 c0 fa  bl    0x0802B678         ; delay_us(20)   ← 建立时间
0x0802F0F8  33 20        movs  r0, #0x33
0x0802F0FA  ff f7 07 fc  bl    0x0802E90C         ; strobe(SCAL 0x33)
0x0802F0FE  4f f4 7a 70  mov.w r0, #0x3E8
0x0802F102  fc f7 b9 fa  bl    0x0802B678         ; delay_us(1000)
0x0802F106  36 20        movs  r0, #0x36
0x0802F108  ff f7 00 fc  bl    0x0802E90C         ; strobe(SIDLE 0x36)
0x0802F10C  4f f4 2f 70  mov.w r0, #0x2BC
0x0802F110  02 b0        add   sp, #8
0x0802F112  bd e8 10 40  pop.w {r4, lr}
0x0802F116  fc f7 af ba  b.w   0x0802B678         ; 尾调用 delay_us(700)
0x0802F11A  00 bf        nop
0x0802F11C  10 b5        push  {r4, lr}          ; 下一函数（不是补丁区）
```

原厂 92 字节（`apply()` / 复核脚本用的基准）：

```text
01 22 03 46 10 b5 4c f2 68 34 13 44 b4 fb f3 f4 82 b0
23 0a 0d f1 07 01 04 20 8d f8 07 30 ff f7 5d fb 01 22
0d f1 07 01 05 20 8d f8 07 40 ff f7 55 fb 14 20 fc f7
c0 fa 33 20 ff f7 07 fc 4f f4 7a 70 fc f7 b9 fa 36 20
ff f7 00 fc 4f f4 2f 70 02 b0 bd e8 10 40 fc f7 af ba
00 bf
```

要点：

- **没有字面量池**：`50024` 通过 `movw r4,#0xC368` 立即数装入，函数体内没有 `ldr [pc]` 数据字；
  因此 10 字节补丁点 `0x0802F0C6..0x0802F0CF` 恰好覆盖 `movw + add + udiv` 三条指令；
- 函数**不是**只用寄存器：它在 `0x0802F0D0` 起用 `sub sp,#8` 分配 1 字节栈缓冲（`[sp+7]`）
  逐字节写 SYNC1/SYNC0，最后 `add sp,#8; pop {r4,lr}; b.w delay(700)`；
- 原厂收尾序列固定为 **`delay(20µs) → SCAL(0x33) → delay(1ms) → SIDLE(0x36) → delay(700µs)`**。

### 2.2 函数边界与安全约束

- 补丁区**必须止步于 `0x0802F11C`**：同步字函数 92 B（`0x0802F0C0..0x0802F11B`），
  下一函数 `0x0802F11C` 以 `push {r4,lr}` 开头；越过即破坏邻居代码；
- 10 字节补丁点 `0x0802F0C6..0x0802F0CF`：前 6 字节 `01 22 03 46 10 b5`
  （`movs r2,#1; mov r3,r0; push {r4,lr}`）**保持不变**，注入代码从这里取得
  `r2 = 1`、`r3 = ID`、以及已保存的 `{r4,lr}` 现场；
- 补丁后**同步字函数的其余部分（写 SYNC1/0 + SCAL/延时收尾）完全保留**，
  改动只是「`r4` 的值怎么来」；
- 固件版本不符时 `apply()` 逐字节拒绝（见 §5.4）。

---

## 3. 辅助函数、调用点与设置结构

### 3.1 辅助函数

| 符号 | 地址 | 说明 |
|---|---|---|
| `write_reg` | `0x0802E79C` | `r0`=寄存器地址、`r1`=缓冲区指针、`r2`=长度；`reg \| 0x40` = burst 方式（burst `0x44` 一次写 SYNC1/SYNC0 两个字节） |
| `strobe` | `0x0802E90C` | 命令选通（`r0`=命令字节，如 `0x33`=SCAL、`0x36`=SIDLE、`0x35`=STX、`0x3B`=SFTX） |
| `delay_us` | `0x0802B678` | 忙等延时（`r0`=计数值；同步字函数里 `0x14=20`、`0x3E8=1000`、`0x2BC=700` 都走它） |
| `radio_wake` | `0x0802ECE4` | 射频唤醒/置忙入口（TX4/TX2 的第一个调用；同步字必须在它之后写，避免休眠态写寄存器失效） |
| GPIO 辅助 | `0x08032A64` / `0x08032A58` | GPIO 读写辅助（发送路径里的状态切换会用到；本补丁不改它们） |

> 说明：TX4/TX2 函数体里还会调用 `0x08032ABC` 等 GPIO/状态辅助，语义未完全定性；
> 本补丁只改两个函数入口的 4 字节，函数体原厂逻辑 100% 保留。

### 3.2 同步字函数的调用点（静态扫描）

对镜像做线性反汇编（capstone，`CS_MODE_THUMB | CS_MODE_MCLASS`）后匹配 `bl` 目标，
**`bl 0x0802F0C0` 共 6 处**：

| 调用点 | 备注 |
|---|---|
| `0x0802F1B4` | 同步字函数之后的射频代码区（语义未逐一细化） |
| `0x0802F1C4` | 同上 |
| `0x0802F2AE` | RF 初始化/重同步路径 |
| `0x0802F786` | 设置/同步路径 |
| `0x0802F7A6` | 同上 |
| `0x0802F962` | 同上 |

> 扫描口径：线性反汇编 + `bl` 目标匹配；换固件版本后必须重扫（`docs/01` §10 有可复现脚本）。
> 这 6 处共享同一个同步字函数，**没有第二个绕开它的运行期同步字写入口**（见 §4 / `docs/01` §8），
> 所以「改写同步字函数 + 发送入口」即可覆盖全部发送路径，不必逐个调用点打补丁。

### 3.3 两个低层发送入口（TX4 / TX2）

| 符号 | 地址 | 帧类型 | 原厂 4 字节 | 函数体恢复点 |
|---|---|---|---|---|
| TX4 | `0x0802EE44` | **4 字节帧** | `10 b5 ff f7`（`push {r4,lr}; bl 0x0802ECE4`） | `0x0802EE4A` |
| TX2 | `0x0802EEAC` | **2 字节短命令** | `10 b5 ff f7`（同上） | `0x0802EEB2` |

TX4 函数体（摘要，反汇编）：唤醒 → GPIO 状态切换 → `FIFO burst 0x7F len 4` →
`strobe(0x35 STX)` → `delay(600µs)` → `strobe(0x3B SFTX)` → `strobe(0x36 SIDLE)` →
GPIO 恢复 → 清忙标志。

TX2 函数体（摘要）：唤醒 → GPIO 状态切换 → `delay(40µs)` → `FIFO burst 0x7F len 2` →
`strobe(0x35 STX)` → `delay(500µs)` → `strobe(0x3B SFTX)` → `strobe(0x36 SIDLE)` →
GPIO 恢复 → 清忙标志。

两个入口的 4 字节（`push {r4,lr}` + `bl radio_wake` 的前 4 字节）**完全相同**，
这是选择「入口 4 字节改 `B.W`」的前提；注入包装用「蹦床」重建这 4 字节后跳回函数体。

静态扫描（同 §3.2 口径）：`bl 0x0802EE44` 共 **18 处**、`bl 0x0802EEAC` 共 **10 处** ——
即几乎所有组帧/发射路径都经过这两个低层入口，改这两个入口即可覆盖。

### 3.4 设置结构与 ID 上限

- 设置结构基址 **RAM `0x20001768`**：`[+0] = 频道`、`[+1] = ID`（`[+1]` 的字段语义由
  `0x08029A0E` 的 ID 加减 handler 与 `0x08025E30` 的组帧代码印证）；
- ID 菜单范围 **0..99**，上限钳位证据（`0x08029A1C` 附近，ID 加减 handler `0x08029A0E`）：

```asm
0x08029A0E  07 4b        ldr   r3, [pc, #0x1c]      ; r3 = 某按键/状态字节指针
0x08029A10  07 4a        ldr   r2, [pc, #0x1c]      ; r2 = 设置结构（ID = [r2,#1]）
0x08029A12  19 78        ldrb  r1, [r3]
0x08029A14  53 78        ldrb  r3, [r2, #1]       ; r3 = 当前 ID
0x08029A16  31 b1        cbz   r1, 0x08029A26
0x08029A18  0a 33        adds  r3, #0xa          ; 「大步进」：ID + 10
0x08029A1A  db b2        uxtb  r3, r3
0x08029A1C  63 2b        cmp   r3, #0x63          ; 99
0x08029A1E  88 bf        it    hi
0x08029A20  63 23        movhi r3, #0x63          ; 上限钳位到 99
0x08029A22  53 70        strb  r3, [r2, #1]
0x08029A24  10 bd        pop   {r4, pc}
0x08029A26  01 33        adds  r3, #1             ; 小步进：ID + 1
0x08029A28  db b2        uxtb  r3, r3
0x08029A2A  f7 e7        b     0x08029A1C
```

- 也就是说 **`ID=99` 是菜单可达的合法值**（+1 或 +10 都能到 99），所以「复用 ID=99 当密钥开关」
  **不需要改 UI**；代价是 `ID=99` 在密钥模式下不再是普通 ID；
- 无线帧组包时 ID 字段由设置区派生（`0x08025E30` 附近有 `ldrb r0,[r0,#1]` 与 `adds r1,#1`
  的组帧序列），因此**双发的第二帧仍按原厂 ID 语义组帧**，只是同步字换成 `50024/(99+1)`。

---

## 4. 用户可见行为

| ID 菜单 | 模式 | 每帧发射行为 | 同步字 |
|---|---|---|---|
| **99** | **密钥模式** | **双发**：`SYNC=KEY` 发一帧（引闪密钥灯）→ `SYNC=0x01F4` 再发一帧（引闪同频道 ID=99 的原厂灯）→ 恢复 `SYNC=KEY` | `KEY` 与 `0x01F4`（= `50024/(99+1)`） |
| **0..98** | 普通模式 | **原厂单发**（行为与未打补丁的固件一致） | `50024/(ID+1)` |

要点：

- **为什么 ID=99 要双发**：`ID=99` 的原厂同步字是 `floor(50024/100) = 500 (0x01F4)`。
  密钥模式第一帧用私有 KEY（只有打过补丁的灯认），第二帧用 `0x01F4`（同频道、ID=99 的
  原厂灯认），因此**密钥灯与普通灯可以在同一套系统里一起闪**；
- **恢复 `SYNC=KEY` 的意义**：双发结束后把同步字写回 KEY，异步队列/中断路径随后发出的帧
  仍使用密钥同步字；
- **普通模式（ID=0..98）没有任何额外发射**，连时序都保持原厂；
- 菜单操作本身完全沿用原厂（ID 项、+1/+10 步进、上限 99），补丁**不新增/不修改任何 UI**。

---

## 5. 补丁设计（原位改写 + 空闲闪存注入，文件长度不变）

### 5.1 同步字函数 10 字节改写：`B.W inj_sync`

`0x0802F0C6` 起 10 字节原厂内容为 `movw r4,#0xC368; add r3,r2; udiv r4,r4,r3`，
改写为：

```text
0x0802F0C6  B.W <inj_sync>     ; 4 B，24 位相对偏移
0x0802F0CA  00 bf              ; NOP
0x0802F0CC  00 bf              ; NOP
0x0802F0CE  00 bf              ; NOP
```

注入区里的 `inj_sync`（进入时 `r0 = ID`、`r2 = 1`、`r3 = ID`、`{r4,lr}` 已入栈）：

```asm
inj_sync:
    cmp   r0, #99            ; 密钥模式？
    bne   inj_sync_orig
    movw  r4, #KEY           ; 是：r4 = 私有 KEY
    b.w   0x0802F0D0         ; 回到原厂：写 SYNC1/SYNC0 + SCAL/延时
inj_sync_orig:
    movw  r4, #0xC368        ; 否：原厂 10 字节原样重放
    add   r3, r2             ; r3 = ID + 1
    udiv  r4, r4, r3         ; r4 = 50024/(ID+1)
    b.w   0x0802F0D0         ; 回到原厂收尾
```

效果：**所有**调用同步字函数的路径（RF 初始化、改 ID、设置同步、中断队列等）在密钥模式下
都会得到 `SYNC1/SYNC0 = KEY`，普通模式下与原厂逐指令一致；原厂的
`delay(20µs) → SCAL(0x33) → delay(1ms) → SIDLE(0x36) → delay(700µs)` 收尾**完整保留**。

### 5.2 TX4 / TX2 入口 4 字节改 `B.W` 注入

`0x0802EE44`（TX4）与 `0x0802EEAC`（TX2）的入口原厂 4 字节都是
`10 b5 ff f7`（`push {r4,lr}` + `bl 0x0802ECE4` 的前 4 字节），改写为 `B.W <inj_tx4>` /
`B.W <inj_tx2>`。注入包装的逻辑（以 TX4 为例，TX2 同理）：

```asm
inj_tx4:
    push  {r4, lr}
    ldrb  r3, [设置结构+1]      ; r3 = 当前 ID
    cmp   r3, #99
    bne   inj_tx4_single
    ; ---- 密钥模式：双发 ----
    bl    tramp4_key            ; 蹦床：push {r4,lr}; bl radio_wake;
                                ;       写 SYNC=KEY; b.w 0x0802EE4A（原厂第一帧）
    bl    tramp4_ord            ; 蹦床：写 SYNC=0x01F4; b.w 0x0802EE4A（原厂第二帧）
    bl    set_key               ; 恢复 SYNC=KEY
    pop   {r4, pc}
inj_tx4_single:
    bl    tramp4_orig           ; 蹦床：只唤醒 + 原厂单帧
    pop   {r4, pc}
```

设计要点：

- **蹦床（trampoline）**重建被覆盖的 `push {r4,lr}; bl 0x0802ECE4`，再跳回原函数体
  （TX4 恢复点 `0x0802EE4A`、TX2 恢复点 `0x0802EEB2`），原厂组帧/FIFO/发射时序 100% 保留；
- **同步字在 `radio_wake` 之后、写 FIFO 之前写入**：避免休眠态写寄存器失效，并让同步字切换
  落在射频忙区间内；
- 注入侧的 `set_sync` 用 **burst 方式写 CC2500 `reg 0x44`**（`write_reg(0x44, buf, 2)`，
  buf = `[SYNC1, SYNC0]`）；
- 包装函数自己 `push {r4,lr}` / `pop {r4,pc}`，保证原函数入口被覆盖后现场一致。

### 5.3 注入区：`0x081CA000` 起 0x400 字节

- 位置：**`0x081CA000` 起 0x400 字节**（文件偏移 `0x1CA000`），原厂为**连续 `0xFF` 擦除态**；
- 内容：`inj_sync` / `inj_tx4` / `inj_tx2` / `set_sync` / `set_key` / `set_ord` / 各蹦床；
- 该区位于主固件尾部的大片 `0xFF` 空洞中（`0x081A9378–0x081D4000`，共 175,240 B），
  距离代码尾（最后非 `0xFF` 字节 `0x081A9377`）与热靴 blob 起点（`0x081D4000`）都很远；
- 本目录的静态扫描未发现任何 `B`/`B.W`/`LDR` 字面量/绝对地址引用落进该区（`docs/01` §9）；
  完整独立复核以 Lead / 补丁模块的结论为准；
- 注入代码实际长度由模块的 `build_injection()` 输出决定（**预留 0x400，未用部分保持 `0xFF`**），
  `verify()` 会断言「注入区剩余字节仍为 `0xFF`」。

### 5.4 补丁点总表

| # | 地址 | 文件偏移 | 原厂字节（长度） | 补丁后 | 说明 |
|---|---|---|---|---|---|
| 1 | `0x0802F0C6` | `0x0000F0C6` | `4c f2 68 34 13 44 b4 fb f3 f4`（10 B） | `B.W <inj_sync>` + `00 bf`×3 | 同步字函数分流：ID=99 → `r4=KEY`，否则重放原厂公式 |
| 2 | `0x0802EE44` | `0x0000EE44` | `10 b5 ff f7`（4 B） | `B.W <inj_tx4>` | 4 字节帧发送入口 → 双发包装 |
| 3 | `0x0802EEAC` | `0x0000EEAC` | `10 b5 ff f7`（4 B） | `B.W <inj_tx2>` | 2 字节短命令入口 → 双发包装 |
| 4 | `0x081CA000` | `0x001CA000` | `FF` × N（原厂 0x400 全 `FF`） | 注入代码（其余保持 `FF`） | 空闲闪存注入区 |

> 说明：`B.W` 指令的机器码 = 24 位有符号相对偏移编码，**具体字节取决于 `inj_*` 在注入区中的
> 位置以及模块的布局**，因此本表用符号 `<inj_sync>` / `<inj_tx4>` / `<inj_tx2>` 表示；
> 实际字节由 `keypatch/devices/x3pros.py` 的 `apply()` 生成，`verify()` 用同一套编码重新比对。
> 三个入口合计 **18 字节（10 + 4 + 4）**被改写，输出固件长度保持 1,931,868 B 不变。

### 5.5 差异统计与安全约束

- **输出长度 == 输入长度 == 1,931,868 B**（原位改写，不改变 G3 传输的文件长度）；
- 打补丁前逐项校验：文件长度、sha256、每处补丁点的**原始字节**；任一不符立即
  `raise ValueError`（防刷错版本）；
- 与原厂的差异**只允许**落在「3 个入口改写字节 + 注入区实际写入的代码字节」内，
  `verify()` 用差异集合比对断言「差异之外 0 处」；
- 注入区只用预留的 0x400 前段，其余保持 `FF`；
- 密钥为 16 位值（`0x0000..0xFFFF`，另排除 `0x0000/0xFFFF/0xAAAA/0x5555/0x8000` 等保留值，
  见 `keypatch/common.py`）；换密钥只需重新生成，补丁结构不变。

### 5.6 时序与已知风险（设计层面）

- 密钥模式下**每个帧多一次整帧发射**（第二帧）：TX2 帧内约 500µs 延时、TX4 帧内约 600µs，
  2 字节短命令（原厂每个连发 3 次）也会每帧多一次发射 → **发送时序整体略变长**。
  若实机出现 HSS/连拍/高速同步异常，优先怀疑这里；
- 模块内提供 `DUAL_SEND` 开关：关掉后退回「密钥模式 = 只发 KEY 单帧」（最保守，时序变化最小，
  但普通灯不再被兼容引闪）；
- 「第二帧发完 → 写回 `SYNC=KEY`」这一次写**不在射频忙区间内**；补丁未关中断，
  理论上存在极小的竞态窗口（几十 µs 量级），属**已知残余风险**，首次实机测试请一并观察。

---

## 6. 验证（离线）

### 6.1 复现命令

```powershell
# 仓库根
tools\python\python.exe keypatch\keypatch.py list
tools\python\python.exe keypatch\keypatch.py info --device x3pros

# 打补丁 + 离线验证（先输出到 %TEMP%，不要覆盖正式成品）
tools\python\python.exe keypatch\keypatch.py patch --device x3pros --key 0xD798 --verify --out %TEMP%\kp_x3pros

# 同时生成 G3 容器备用件
tools\python\python.exe keypatch\keypatch.py patch --device x3pros --key 0xD798 --verify --container --out %TEMP%\kp_x3pros

# 模块级（等价于 keypatch.py verify；需 x3pros 已在 keypatch/devices/__init__.py 的 DEVICE_IDS 注册）
tools\python\python.exe -c "import sys; sys.path.insert(0,'keypatch'); from devices import x3pros; import common; raw=common.read_file('devices/x3pros/firmware/X3pro_S_v1.11_app.bin'); patched,log=x3pros.apply(raw,0xD798); [print(('PASS' if ok else 'FAIL'),msg) for ok,msg in x3pros.verify(patched,0xD798)]"

# 全设备总自检（可选）
tools\python\python.exe keypatch\selftest.py --key 0xD798
```

预期产物：

| 文件 | 说明 |
|---|---|
| `X3pro_S_v1.11_app_keyD798.bin` | 长度 1,931,868 B；sha256 随密钥变化，以实际生成为准 |
| `X3pro_S_v1.11_app_keyD798_container.bin` | G3 容器备用件（若 G3 报「固件与设备不匹配」时使用） |

### 6.2 离线验证覆盖的分层

| 层 | 覆盖内容 |
|---|---|
| 静态-镜像身份 | 长度 1,931,868 B；sha256 与原厂一致（补丁前） |
| 静态-补丁点 | 同步字函数 10 B 改写、TX4/TX2 各 4 B `B.W`、注入区代码与剩余 `FF` |
| 静态-差异范围 | 与原厂差异集合 == 期望补丁点集合；差异之外 0 处 |
| Unicorn-同步字 | 补丁版：ID=99 → `SYNC=KEY`；ID=0/1/37/98 → 原厂 `50024/(ID+1)`；SCAL/SIDLE 收尾保留；原厂镜像在同一 harness 上自证公式 |
| Unicorn-双发 | 补丁版：TX4/TX2 在 ID=99 时 `SYNC=KEY → FIFO → SYNC=0x01F4 → FIFO → SYNC=KEY`；ID=0/5 时只发一帧 |
| Unicorn-原厂自证 | 原厂镜像的 TX4/TX2 单发序列与同步字公式逐项一致 |

> 具体的 `verify()` 项数与 PASS 数量**以实际运行的 `--verify` 输出为准**，本文档不预先固化数字
> （项数可能随模块实现调整）。判定口径见
> [`docs/02_验证与实机清单.md`](docs/02_验证与实机清单.md)。

### 6.3 做不到的部分（如实记录）

- **整机模拟器未建立**：X3 Pro S 没有像 AD100/AD200 那样的整机 Unicorn 模拟机，
  本轮只做「调用级」验证（直接执行同步字函数/发送入口/注入代码）。启动链、主循环、
  外设交互、看门狗都没有回归覆盖；
- **无实机验证**：Unicorn 只能证明「补丁后的机器码按预期写 SYNC、按预期双发」，
  真正的空口同步过滤、双发时序对 HSS/连拍的影响、G3 是否接受产物，都必须真机验证。

---

## 7. 风险与未验证项

| 项 | 状态 | 说明 |
|---|---|---|
| **实机验证** | ❌ **未做** | 见 [`docs/02_验证与实机清单.md`](docs/02_验证与实机清单.md) §4 安全阶梯 |
| 整机模拟器 | ⚠️ 未建立 | 只有 Unicorn 调用级；启动链/主循环未回归 |
| **双发时序变化** | ⚠️ 已知取舍 | 密钥模式下每帧多一次整帧发射（TX2 帧内约 500µs、TX4 帧内约 600µs 延时）；2 字节短命令连发时更明显。HSS/连拍异常优先怀疑此处；可用 `DUAL_SEND=False` 退回单发 KEY |
| **两次发射之间改同步字** | ⚠️ 已知残余风险 | 未关中断，恢复 KEY 的写不在射频忙区间内，理论上有几十 µs 竞态窗口 |
| **ID=99 语义被占用** | ⚠️ 已知取舍 | 密钥模式下 `ID=99` 不再表示普通 ID；普通模式 0..98 与原厂一致 |
| **普通灯兼容** | ⚠️ 依赖第二帧 | 只有「同频道 + ID=99」的原厂灯会被第二帧引闪；其它 ID 的普通灯不响应 |
| 注入区独立性 | ⚠️ 静态扫描结论 | 本目录扫描 0 引用；换固件版本必须重扫（`docs/01` §9/§10） |
| 热靴子镜像 | ✅ 不触碰 | file `0x1D4000–0x1D7A5C` 是独立链接的热靴 MCU 固件，补丁不改、引用不落进去 |
| 版本绑定 | ✅ 有防护 | 长度 + sha256 + 3 个入口原始字节逐字节校验，版本不符直接拒绝 |
| G3 容器 | ⚠️ 待实机 | 模块提供 `X3pro_S_v1.11_app_key{key:04X}_container.bin` 备用；裸 `.bin` 优先，报「不匹配」时再换容器 |
| ID/设置持久化 | ⚠️ 待确认 | `ID` 在 RAM 设置结构里；原厂 save/load 链本轮未逆向，重启后 ID 是否保持需实机确认 |
| 16 位同步字强度 | ⚠️ 非密码学 | 挡的是正常原厂设备误触发；SDR 抓包理论上可克隆 |
| Flash/RAM 容量、显示链路 | ⚠️ 待确认 | 本轮未做完整逆向 |
| 「无实机验证的结论都不算数」 | ℹ️ 判定口径 | 静态 + Unicorn 只能证明代码按预期执行，空口过滤必须靠真机 CC2500 硬件验证 |

---

## 8. 文件地图与相关文档

```text
devices/x3pros/
├─ README.md                        ← 本文（逆向档案 + ID=99 开关 + 双发设计 + 补丁点总表）
├─ firmware/
│  ├─ X3pro_S_v1.11_app.bin         ← 原厂固件（本机，*.bin 被 .gitignore 忽略，不入库）
│  └─ README.md                     ← 固件来源/版本/字节数/sha256/放置路径
└─ docs/
   ├─ 01_静态证据与补丁点.md          ← 基址判定、反汇编、补丁点、注入区选址与复核脚本
   └─ 02_验证与实机清单.md            ← 离线验证项、实机安全阶梯、待确认 checklist
```

相关：

- 静态证据与补丁点：[`docs/01_静态证据与补丁点.md`](docs/01_静态证据与补丁点.md)
- 验证分层与实机安全阶梯：[`docs/02_验证与实机清单.md`](docs/02_验证与实机清单.md)
- 原厂固件登记：[`firmware/README.md`](firmware/README.md)
- 补丁实现：[`keypatch/devices/x3pros.py`](../../keypatch/devices/x3pros.py)（`DEVICE` / `apply()` / `verify()`）
- 设备模块接口：[`keypatch/INTERFACE.md`](../../keypatch/INTERFACE.md)
- 刷机/回滚/救砖：[`keypatch/README.md`](../../keypatch/README.md)
- 全仓库设备速查：[`docs/设备清单.md`](../../docs/设备清单.md)
- 新设备接入方法：[`docs/新增设备指南.md`](../../docs/新增设备指南.md)
- 同族参考：C 版模块 [`keypatch/devices/x3pro.py`](../../keypatch/devices/x3pro.py)、
  老平台引闪器 [`devices/x3c/README.md`](../x3c/README.md) / [`devices/x3s/README.md`](../x3s/README.md)

---

## 9. 免责声明

- 本文档与补丁只用于**你本人合法拥有**的神牛设备的功能改造/互操作研究；
- 固件版权归厂商所有，**不要**分发原厂固件或补丁产物；
- 刷机有风险：刷坏/刷错版本可能使设备无法开机。请先确认官方 G3 能识别设备、
  手上有同版本原厂固件可回滚，并严格按 [`docs/02_验证与实机清单.md`](docs/02_验证与实机清单.md)
  的安全阶梯操作，风险自负；
- 16 位同步字不是密码学认证；它挡的是正常原厂设备的误触发，不构成对专业抓包/克隆的防护；
- 完整免责声明见仓库根 [`DISCLAIMER.md`](../../DISCLAIMER.md)。
