# X3-C 引闪器 v1.38 密钥补丁 NOTES（v3：UI 密钥开关 + 双发兼容）

> 设备：Godox X3-C（X3 C）引闪器，固件 `devices/x3c/firmware/X3_C_v1.38_app.bin`
> （1,021,468 字节，sha256 `45fdd952…f85ef62`，链接基址 0x08004800，
> 文件偏移 = 地址 − 0x08004800；CPU Artery AT32F435，射频 TI CC2500 类）。
>
> 版本演进：
> * **v1（历史）**：同步字恒等于私有 KEY（6 字节原位补丁，见 §7）。
> * **v2（历史）**：用 ID==99 当开关，ID=99 时 KEY 帧 + 500 帧双发（见 §7）。
> * **v3（当前交付）**：用 **UI 里的"密钥开关"状态字节**（KEY_ON_FLAG）当开关，
>   第二帧同步字按**当前 ID 现算** `floor(50024/(ID+1))`；同时兼容 v2 旧开关
>   （ID==99 且 ID-OFF==0）。运行 `keypatch.py patch --device x3c --key 0xD798 --verify`
>   生成 `keypatch/out/X3_C_v1.38_app_keyD798.bin`。

---

## 0. 交付摘要（v3 成品，最终交付引用用）

| 项 | 值 |
|---|---|
| 成品文件 | `keypatch/out/X3_C_v1.38_app_keyD798.bin` |
| 密钥 | 0xD798（SYNC1=0xD7，SYNC0=0x98） |
| 长度 | 1,021,468 字节（与原厂完全相同，原位补丁） |
| 原厂 sha256 | `45fdd9522c432f4be60a470741d3e5c12a7ee629eb2b277aed5187639f85ef62` |
| **成品 sha256** | **`af5bd6dfbb6b19d981848adb80a48141b885c078c339e44506281c3a050e39e7`** |
| 与原厂差异 | **332 字节**（2 个入口跳转共 8B + 注入区 324B，无越界） |
| 补丁区 1 | 0x080118B0..+4（文件 0x0D0B0）：send_short 入口 `b.w 0x080EE000` |
| 补丁区 2 | 0x0801184C..+4（文件 0x0D04C）：send_property 入口 `b.w 0x080EE08C` |
| **注入区** | **0x080EE000..0x080EE144（324 字节；预留到 0x080EE400）**，原厂为全 0xFF 空闲闪存 |
| 注入布局 | INJ_SHORT=0x080EE000，INJ_PROP=0x080EE08C，TRAMP_SHORT=0x080EE128，TRAMP_PROP=0x080EE136 |
| 验收命令 | `tools\python\python.exe keypatch\keypatch.py patch --device x3c --key 0xD798 --verify` → 全 PASS |
| KEY_ON_FLAG | **0x20001080**（ui-x3c 最终定址：全镜像无字面量引用、40M 指令模拟写追踪仅 BSS 清零 1 次；**RAM 版**，重启复位=关，后续由 ui-x3c 做 flash 持久化） |
| 原始证据 | `keypatch_patch_D798.log`（patch 输出）、`bin_check.log`（长度/sha256/差异）、`unicorn_toggle_test.log`、`runtime_toggle_test.log`、`boot_regression.log`、`analyze_toggle.log` |

> **v3k 物理按键长按保底成品（独立文件，不覆盖上面成品）**：
> `keypatch/out/X3_C_v1.38_app_keyD798_keybtn.bin`，
> sha256 `7f15788639bbdddd801708bba5d7fea2f8aa0227a9021b1f3b676d34304d38b8`，
> 差异 738 字节（core 332B + 按键 406B），按键注入区 0x080EE400..0x080EE590（400B）。
> 长按 M/C ≈1.5s 切密钥、长按 ⚡ ≈1.5s 32 频道广播；详见 §10。

> **v3p 键位探针版（2026-11-07，用户实机确认键位用）**：
> `keypatch/out/X3_C_v1.38_app_keyD798_keyprobe.bin`，
> sha256 `847491e6fef1e4de69e58e53a43bfc9533e524cb4b3d83de5cac275fcfebeb7e`，
> 差异 888 字节；长按三个物理键分别闪 **1/2/3** 次（当前 ID 公共同步字，不受 KEY_ON 影响）。
>
> **v3k2 正式版（长按拨轮切密钥 / 长按 ⚡ 广播 / M/C 长按不再绑定）**：
> `keypatch/out/X3_C_v1.38_app_keyD798_keybtn2.bin`，
> sha256 `7a1f147887351583586755f013ccf83f36cfb489b29f71f04954d0985db409b4`，
> 差异 677 字节；详见 §11。

> **最终版 keybtn4（2026-11-07 04:3x，实机探针键位 + 两段式关机）**：
> `keypatch/out/X3_C_v1.38_app_keyD798_keybtn4.bin`，
> sha256 `ffaaad4b9ceda3641cbef2af37316445418169a196b45e6168855de4129a93e7`，差异 721 字节；
> **长按拨轮/M/C(bit0/PA9) 1.5s=密钥开关、继续按住 20s=关机；长按 ⚡(bit1/PA8)=32 频道广播；
> bit2(PB15) 原厂不动**；原厂关机链路/证据见 §12。

> **keybtn7（2026-11-07 最终手势追加，基于 keybtn5）**：
> `keypatch/out/X3_C_v1.38_app_keyD798_keybtn7.bin`，
> sha256 `b53c1ed573f2aabd1aab4294511ce96b06668f5ebbb0161f89c86309d4652d22`（差异 1151 B）；
> **双击 M/C/拨轮=32ch×100ID 公共同步字广播（分块+喂狗）、长按 1.5s=密钥开关、按住 20s=关机、
> ⚡ 完全原厂、无长按广播**；详见 §15。
>
> **X3-C 最终交付（2026-11-07，用户拍板）**：
> `keypatch/out/X3_C_v1.38_app_keyD798_keybtn5.bin`，
> sha256 `12c1736c1cd67ca4a4569e9ecdc0e4fea38ed82e17cc3076e162c1abebb08c25`（差异 547 B）；
> 长按 M/C/拨轮 1.5s=密钥开关、按住 20s=关机、⚡ 长按=原厂电源、无广播、
> 双发白名单修休眠前"滴滴响"；keybtn6（双击 ⚡）已取消、无 bin。详见 §14。
>
> **无广播变体 A/B（2026-11-07 04:4x）**：
> A `X3_C_v1.38_app_keyD798_keybtn4_nobc_a.bin` sha256
> `7e4aa2018e05b6e0c44f78e69397c5dcf6ccabf6359fb87688ee42e55f7bbccb`（差异 508 B；⚡ 长按恢复原厂关机）；
> B `X3_C_v1.38_app_keyD798_keybtn4_nobc_b.bin` sha256
> `269ffc0f12b6609fb6608112c63b5f7b12a441f7c127978daa04e6517d3165b4`（差异 509 B；⚡ 长按无任何动作）。
> 两版仅差 1 字节（文件 offset 0x08514 = 0x0800CD14：A=0x28 原厂 / B=0xFF 禁用）；详见 §13。

---

## 1. v3 用户可见语义

密钥模式判定（注入代码）：

```
keymode = (KEY_ON_FLAG != 0)  or  (当前ID == 99 and ID-OFF标志 == 0)
```

| 条件 | 行为 |
|---|---|
| 密钥模式 | 每个要发的帧发两次：① 写 SYNC=KEY → 发送；② 现算并写 SYNC=floor(50024/(当前ID+1)) → 再发送 |
| 非密钥模式 | 原厂行为：只发一次，同步字仍由原厂 0x08014864 维护 |

* 第二帧同步字用例：ID=5 → 50024/6 = 8338 = **0x2091**；ID=99 → 50024/100 = **0x01F4**；
  ID=0 → 50024/1 = 0xC368。
* 双发后同步字停留在 `floor(50024/(ID+1))`，等于"原厂同 ID 状态"，收包与后续单发/信道切换不受影响。
* `KEY_ON_FLAG` = **0x20001080**（`keypatch/devices/x3c.py` 的 `KEY_ON_FLAG_RAM`），
  ui-x3c 最终定址。证据：全镜像无任何字面量引用；40M 指令模拟器写追踪只有 BSS
  清零 1 次写、之后从未被写。注意这是 **RAM 版**：重启复位 = 关；flash 持久化由
  ui-x3c 后续完成（引闪器端不负责持久化）。
  （历史：0x200004D0 不能用 —— Lead 核实是固件自用标志：0x0803420A 读 /
  0x0800C2AE 写 / 0x0800E4EA 读，写入会改变固件行为。）

## 2. RAM 关键变量（已定位并交叉确认）

| 变量 | RAM 地址 | 证据 |
|---|---|---|
| 当前 ID（0..99） | **0x20000055**（1 字节） | main 主循环 literal pool `0x08034330 = 0x20000055`，作为 param_3 传给 `FUN_080118E8`（再调 `0x08014864(param_3)` 设置同步字）；`0x08011814` 处 literal 亦指向它 |
| 当前信道（1-based） | **0x20000054**（1 字节） | main literal pool `0x0803432C = 0x20000054`，main 里 `-1` 后传给 `FUN_080118E8` → `0x08014814(ch)` |
| ID-OFF/特殊状态标志 | **0x20000143**（1 字节） | `FUN_080118E8` 状态结构基址 `DAT_080119CC = 0x2000013E` 的 `[5]`；非 0 时改走 `FUN_0801616C`（ID OFF 路径） |
| **UI 密钥开关（KEY_ON）** | **0x20001080**（1 字节） | ui-x3c 最终定址：全镜像无字面量引用、40M 指令模拟写追踪仅 BSS 清零；语义：非 0 = 开密钥；RAM 版重启复位，flash 持久化由 ui-x3c 后续做 |
| 发送门控 | **0x20000146**（1 字节） | `send_short`/`send_property` 开头 `ldrb`，非 0 则跳过发送 |
| TX 缓冲 | **0x20000152** | `send_short` 写 `[D5,cmd]`；`send_property` 写 `[A9,group,key,value]` |

## 3. 发射路径（已复核）

| 地址 | 作用 |
|---|---|
| 0x080118B0 `send_short(cmd)` | 写 `[D5,cmd]` → `bl 0x0801712C` |
| 0x0801184C `send_property(group,key,value)` | 写 `[A9,g,k,v]` → `bl 0x0801708C` |
| 0x0801712C / 0x0801708C | 低层 FIFO 发送：burst 写(0x08023536) → STX(0x35) → delay → SFTX(0x3B) → SIDLE(0x36)，全程 `cpsid i` |
| 0x08023540 | `CC2500_WriteReg(reg=r0, val=r1)` |
| 0x08014814 / 0x08014864 | 设置信道 / 按 ID 设置同步字（v3 都不改） |

关键复核：**全固件只有一处 BL 调用 0x0801712C（0x080118CC）、一处调用 0x0801708C
（0x0801186C）** —— 2.4G 帧发送全部经过这两个组帧函数，包一层入口即可全覆盖。

## 4. 代码洞复核

| 区域 | 结论 |
|---|---|
| **0x080EE000..0x080F6FFF**（约 36 KB 全 0xFF） | **选用**。0x080EE000..0x080EE400 内 4 字节字面量引用=0、BL/BLX 分支目标=0 |
| 0x080ECF20 | 不用：被 0x080ECD34 处数据字面量引用（疑似资源结束指针） |
| 0x080F7000 | 不用：被 0x080110B0、0x08020068 引用（`FUN_0801fffc` flash 写测试 / `FUN_08010c8c`） |
| 0x080F7800 | 不用：被 5 处引用；是 `FUN_08021778`/`FUN_080219e4` 的 **0x8A 字节步长校准表**基址（到 0x080F7F8C） |
| 0x080F8000 起 | **热靴子固件镜像**，绝对不碰 |
| 0x0807DCA8 / 0x08094D6F / 0x080A7B5C 等 0x00 区 | 位于字库/图片资源区，无法排除资源引用，不用 |

与 Lead / ui-x3c 约定：本补丁注入区 **0x080EE000**（实际用 0x080EE000..0x080EE144，
预留到 0x080EE400）；ui-x3c 广播注入从 **0x080F0000** 起，互不重叠。

## 5. 注入实现

### 5.1 补丁点总览

| # | 地址（文件偏移） | 原厂字节 | 补丁后 | 说明 |
|---|---|---|---|---|
| 1 | 0x080118B0（0x0D0B0） | `10 b5 0a 4c` | `dc f0 a6 bb` | `b.w 0x080EE000`（send_short 入口） |
| 2 | 0x0801184C（0x0D04C） | `70 b5 0b 4c` | `dc f0 1e bc` | `b.w 0x080EE08C`（send_property 入口） |
| 3 | 0x080EE000..+0x144（0x0E9800） | 324 字节 `FF` | 注入代码 | 双发包装 + 蹦床 |

文件长度不变（1,021,468 B）；与原厂差异 **332 字节** = 8（入口）+ 324（注入区，全部由
0xFF 变成代码），无越界差异。

### 5.2 注入区布局（0x080EE000 起，共 0x144 = 324 字节）

| 地址 | 内容 | 大小 |
|---|---|---|
| 0x080EE000 | `INJ_SHORT`（send_short 包装），内部分支标签 `short_keymode`=0x080EE02E | 140 B |
| 0x080EE08C | `INJ_PROP`（send_property 包装），`prop_keymode`=0x080EE0BE | 156 B |
| 0x080EE128 | `TRAMP_SHORT`：重建 send_short 序言 + `b.w 0x080118B4` | 14 B |
| 0x080EE136 | `TRAMP_PROP`：重建 send_property 序言 + `b.w 0x08011850` | 14 B |

INJ_SHORT 逻辑（KEY=0xD798 时，反汇编见 `analyze_toggle.log` §4）：

```
push {r4,lr} ; r4 = cmd
r3 = *(u8*)0x20001080 ; if r3 != 0 -> keymode          ; UI 密钥开关
r0 = *(u8*)0x20000055 ; if r0 != 99 -> single           ; 兼容旧开关
r1 = *(u8*)0x20000143 ; if r1 != 0  -> single
keymode:
WriteReg(4, 0xD7) ; WriteReg(5, 0x98)                  ; SYNC=KEY
r0 = r4 ; bl TRAMP_SHORT                                ; 第 1 帧
r0 = *(u8*)0x20000055 ; r0 += 1 ; r1 = 0xC368 ; udiv r2,r1,r0
WriteReg(4, r2>>8)                                      ; 高字节
（重新算一次）WriteReg(5, r2&0xFF)                       ; 低字节
r0 = r4 ; bl TRAMP_SHORT                                ; 第 2 帧
pop {r4,pc}
single: r0 = r4 ; bl TRAMP_SHORT ; pop {r4,pc}
```

INJ_PROP 同理，先保存 r4/r5/r6 = group/key/value，两次调用前恢复 r0/r1/r2。

### 5.3 第二帧同步字：M4 硬件 UDIV 现算

```
movw r0,#0x0055 ; movt r0,#0x2000 ; ldrb r0,[r0]   ; ID
adds r0, r0, #1                                     ; ID+1
movw r1, #0xC368                                    ; 50024
udiv r2, r1, r0                                     ; sync = 50024/(ID+1)
lsrs r1, r2, #8  ... WriteReg(4,r1)                 ; SYNC1
uxtb r1, r2      ... WriteReg(5,r1)                 ; SYNC0
```

`udiv` 编码 `b1 fb f0 f2` 与原厂 0x0801486C 的 `udiv r4,r1,r0`（`b1 fb f0 f4`）同型。
**不调用原厂 0x08014864**（它带 SCAL + 1ms 延时，会让每次双发慢约 2ms）。
低字节是重新 udiv 一次得到的（第一次 `WriteReg` 会破坏 r0–r3，避免额外寄存器/栈操作；
代价是每条帧多约 10 条指令，可忽略）。

### 5.4 蹦床（trampoline）—— 保留 100% 原厂逻辑

原 send 函数前 4 字节 = `push {r4,lr}`（property 为 `push {r4,r5,r6,lr}`）+
`ldr r4,[pc,#…]`（PC 相对载入 GPIO 基址 0x40020414，不能原样搬家）。蹦床重建：

```
TRAMP_SHORT: push {r4,lr} ; movw r4,#0x0414 ; movt r4,#0x4002 ; b.w 0x080118B4
TRAMP_PROP : push {r4,r5,r6,lr} ; movw r4,#0x0414 ; movt r4,#0x4002 ; b.w 0x08011850
```

* 包装里 `bl TRAMP_*` → LR = 包装内返回地址；蹦床 `push` 的 LR 与原函数结尾
  `pop {…,pc}` 严格配对，所以原函数照常返回包装内。
* 蹦床重建被覆盖的序言后跳进原函数 +4，原函数体（GPIO 置位、门控、组帧、低层
  发送、GPIO 清零）一行未改。

### 5.5 寄存器 / 栈安全

* 包装只使用 callee-saved r4（property 另用 r5/r6），push/pop 成对；r7–r11 不碰。
* 栈对齐：`push {r4,lr}`=8B、`push {r4,r5,r6,lr}`=16B，BL 时 SP 仍 8 字节对齐。
* 不写 0x20000146 门控、不改 TX 缓冲内容；两帧内容相同，只换硬件同步字。

### 5.6 自定义 Thumb 编码器（`keypatch/devices/x3c.py` 内 `_Asm`）

用到的指令：MOVS imm8、MOV reg T1、CMP imm8、LDRB imm、ADD imm T1、LSR imm、
UXTB、PUSH/POP、MOVW/MOVT、**UDIV**、B.W(T4)、BL(T1)、BNE(T1)。关键：

* `MOVW rd,#imm16`：hw1 = 0xF240|(i<<10)|imm4，hw2 = (imm3<<12)|(Rd<<8)|imm8
* `MOVT`：hw1 = 0xF2C0|(i<<10)|imm4（hw2 同上）
* `UDIV rd,rn,rm`：hw1 = 0xFBB0|Rn，hw2 = 0xF0F0|(Rd<<8)|Rm
* `BL`：hw1 = 0xF000|S<<10|imm10，hw2 = 0xD000|J1<<13|J2<<11|imm11，offset=target−(addr+4)
* `B.W`：同上但 hw2 = 0x9000|J1<<13|J2<<11|imm11
* `BNE`：hw = 0xD100|((offset/2)&0xFF)，offset=target−(addr+4)，范围 −256..254
* 模拟 `bl` 返回时 PC 必须写 **LR 原值（bit0=1，Thumb）**，不能 `LR & ~1`。

## 6. 可复现命令与真实输出

在 `` 下执行，Python 为 `tools\python\python.exe`（capstone 5.0.7 / unicorn 2.1.4）。

### 6.1 打补丁 + 离线验证（验收命令，全 PASS）

```
tools\python\python.exe keypatch\keypatch.py patch --device x3c --key 0xD798 --verify
```

真实输出（全文 `keypatch_patch_D798.log`）：

```
== Godox X3-C (X3 C trigger) (x3c)
   原厂固件 : devices\x3c\firmware\X3_C_v1.38_app.bin  (1021468 字节, sha256 45fdd9522c432f4b…)
   私有密钥 : 0xD798  ->  SYNC1=0xD7 SYNC0=0x98
   补丁点   :
  [1] off+0x0D0B0  0x080118B0  10 b5 0a 4c            -> dc f0 a6 bb             send_short(0x080118B0) 入口: B.W -> 注入包装
  [2] off+0x0D04C  0x0801184C  70 b5 0b 4c            -> dc f0 1e bc             send_property(0x0801184C) 入口: B.W -> 注入包装
  [3] off+0xE9800  0x080EE000  ff ff ff ff …(324 B)   -> 10 b5 04 46 …(324 B)    空闲闪存 0x080EE000 注入：密钥模式双发（KEY 帧 + 当前ID普通同步字帧）
   输出     : keypatch\out\X3_C_v1.38_app_keyD798.bin  (1021468 字节, sha256 af5bd6dfbb6b19d9…)
   [PASS] 固件长度 1021468 字节（应为 1021468，原位补丁+注入）
   [PASS] 注入区 0x080EE000 起 324 字节（双发包装，共 324/1024 字节）
   [PASS] 注入区剩余 700 字节保持擦除态 0xFF（不越界改写）
   [PASS] 入口 0x080118B0 = dc f0 a6 bb（send_short 入口: B.W -> 注入包装）
   [PASS] 入口 0x0801184C = dc f0 1e bc（send_property 入口: B.W -> 注入包装）
   [PASS] 与原厂差异 332 字节，offset=[…]（应恰为 […]）
   [PASS] Unicorn 2.1.4 调用级测试：桩替换 WriteReg(0x08023540)/低层发送(0x0801712C,0x0801708C)/delay(0x080232B6)
   [PASS] 密钥 KEY=0xD798（SYNC1=0xD7 SYNC0=0x98）；密钥模式第二帧同步字 = floor(50024/(当前ID+1)) 现算
   [PASS] KEY_ON_FLAG = 0x20001080（ui-x3c 已验证：无字面量引用、40M 指令模拟写追踪仅 BSS 清零；RAM 版，重启复位；后续 flash 持久化）
   [PASS] send_short ID=5  KEY_ON=1 补丁版(密钥模式): SYNC=0xD798 -> SEND -> SYNC=50024/6=0x2091(0x20/0x91) -> SEND | WriteReg(4,0xD7) -> WriteReg(5,0x98) -> SEND_SHORT -> WriteReg(4,0x20) -> WriteReg(5,0x91) -> SEND_SHORT
   [PASS] send_short ID=5  KEY_ON=0 补丁版(普通模式, id_off=0): 单发 | SEND_SHORT
   [PASS] send_short ID=99 KEY_ON=0 补丁版(密钥模式): SYNC=0xD798 -> SEND -> SYNC=50024/100=0x01F4(0x01/0xF4) -> SEND | …
   [PASS] send_short ID=99 KEY_ON=1 补丁版(密钥模式): SYNC=0xD798 -> SEND -> SYNC=50024/100=0x01F4(0x01/0xF4) -> SEND | …
   [PASS] send_short ID=99 KEY_ON=0 补丁版(普通模式, id_off=1): 单发 | SEND_SHORT
   [PASS] send_short ID=5  KEY_ON=1 补丁版(密钥模式): SYNC=0xD798 -> SEND -> SYNC=50024/6=0x2091(0x20/0x91) -> SEND | …
   [PASS] send_property ID=5  KEY_ON=1 补丁版(密钥模式): SYNC=0xD798 -> SEND -> SYNC=0x2091(0x20/0x91) -> SEND | …
   [PASS] send_property ID=5  KEY_ON=0 补丁版(普通模式, id_off=0): 单发 | SEND_PROP
   [PASS] send_property ID=99 KEY_ON=0 补丁版(密钥模式): SYNC=0xD798 -> SEND -> SYNC=0x01F4(0x01/0xF4) -> SEND | …
   [PASS] send_short ID=99 KEY_ON=0 原厂(自证): 单发(原厂) | SEND_SHORT
   [PASS] send_short ID=5  KEY_ON=0 原厂(自证): 单发(原厂) | SEND_SHORT
   [PASS] send_property ID=99 KEY_ON=0 原厂(自证): 单发(原厂) | SEND_PROP
```

### 6.2 输出 bin 核对

真实输出（`bin_check.log`）：

```
orig   len=1021468 sha256=45fdd9522c432f4be60a470741d3e5c12a7ee629eb2b277aed5187639f85ef62
patched len=1021468 sha256=af5bd6dfbb6b19d981848adb80a48141b885c078c339e44506281c3a050e39e7
diff bytes=332
diff offsets: ['0xd04c','0xd04d','0xd04e','0xd04f','0xd0b0','0xd0b1','0xd0b2','0xd0b3'] ... ['0xe9940','0xe9941','0xe9942','0xe9943']
entry short: 10 b5 0a 4c -> dc f0 a6 bb
entry prop : 70 b5 0b 4c -> dc f0 1e bc
```

### 6.3 独立 Unicorn 调用级验证（不导入设备模块）

```
tools\python\python.exe keypatch\dev\x3c\unicorn_toggle_test.py
```

真实输出（全文 `unicorn_toggle_test.log`，ALL PASS）：覆盖
`KEY_ON=1,ID=5`（双发，第二帧 0x2091）、`KEY_ON=0,ID=5`（单发）、
`KEY_ON=0,ID=99`（双发，0x01F4）、`KEY_ON=1,ID=99`（双发 0x01F4）、
`KEY_ON=0,ID=99,ID_OFF=1`（单发）、`KEY_ON=1,ID=5,ID_OFF=1`（双发，开关优先）、
`KEY_ON=0,ID=0`（单发）；原厂镜像自证只发一次、不写 SYNC。

### 6.4 整机模拟器端到端（真实固件函数，无桩）

```
tools\python\python.exe keypatch\dev\x3c\runtime_toggle_test.py
```

真实输出（全文 `runtime_toggle_test.log`，ALL PASS）：

```
  [补丁] send_short KEY_ON=1,ID=5  双发(第二帧 0x2091)      写SYNC=WriteReg(4,0xD7) -> WriteReg(5,0x98) -> WriteReg(4,0x20) -> WriteReg(5,0x91) 帧=TX2[d5 09] | TX2[d5 09] PASS
  [补丁] send_short KEY_ON=0,ID=5  单发                  写SYNC=(none)  帧=TX2[d5 09] PASS
  [补丁] send_short KEY_ON=0,ID=99 双发(第二帧 0x01F4)      写SYNC=WriteReg(4,0xD7) -> WriteReg(5,0x98) -> WriteReg(4,0x01) -> WriteReg(5,0xF4) 帧=TX2[d5 09] | TX2[d5 09] PASS
  [补丁] send_short KEY_ON=1,ID=99 双发(第二帧 0x01F4)      写SYNC=WriteReg(4,0xD7) -> WriteReg(5,0x98) -> WriteReg(4,0x01) -> WriteReg(5,0xF4) 帧=TX2[d5 09] | TX2[d5 09] PASS
  [补丁] send_short KEY_ON=0,ID=99,ID-OFF=1 单发         写SYNC=(none)  帧=TX2[d5 09] PASS
  [补丁] send_short KEY_ON=1,ID=5,ID-OFF=1  双发(开关优先)   写SYNC=… 帧=TX2[d5 09] | TX2[d5 09] PASS
  [补丁] send_property KEY_ON=1,ID=5  双发              写SYNC=… 帧=TX4[a9 50 b1 00] | TX4[a9 50 b1 00] PASS
  [补丁] send_property KEY_ON=0,ID=5  单发              写SYNC=(none)  帧=TX4[a9 50 b1 00] PASS
  [补丁] send_property KEY_ON=0,ID=99 双发              写SYNC=… 帧=TX4[a9 50 b1 00] | TX4[a9 50 b1 00] PASS
  [原厂] send_short(ID=99) / send_short(ID=5) / send_property(ID=99) 写SYNC=(none)、单发 PASS
```

说明：先真实启动固件到 main 入口（108,398 条指令），再设置 RAM 的 ID / KEY_ON /
ID-OFF / 门控，直接调用真实 `send_short`/`send_property` + 真实低层发送函数；
钩子记录 CC2500 写寄存器与 TX 缓冲内容（`[D5 09]`、`[A9 50 B1 00]` 与原厂一致）。

### 6.5 整机启动回归

```
tools\python\python.exe keypatch\dev\x3c\boot_regression.py --max-insn 2000000 --deep-insn 2000000
```

真实输出（全文 `boot_regression.log`）：

```
A. run.py 启动对比（--max-insn 2000000 --quiet）
  instructions 原厂 2000000 / 补丁 2000000；stop 均 max_insn
  final_pc 均 0x0801c18a；SP/LR 相同；reached_main YES；hang 分析相同
  [PASS] 两份 boot log 归一化后逐行完全一致（图像路径/时间戳归一化）
B/C. Machine 覆盖统计（原厂 vs 补丁）指令数/PC/SP/LR/fault/关键函数命中次数全部一致
整机启动回归结论: PASS（无启动回归）
```

### 6.6 静态证据脚本

```
tools\python\python.exe keypatch\dev\x3c\analyze_toggle.py     # 全文 analyze_toggle.log
```

包含：RAM 地址证据（含 KEY_ON=0x20001080 说明）、发射路径反汇编、代码洞引用扫描
（0x080ECF20/0x080F7000/0x080F7800 被引用故排除）、注入区 capstone 反汇编、
差异统计、第二帧同步字公式（ID=0/5/37/99 一览）。

## 7. 历史版本（v1 / v2，均已被 v3 取代）

### v1（固定 KEY，6 字节原位补丁）
| 补丁点 | 原厂 | v1 | 说明 |
|---|---|---|---|
| 0x08014866 | `movw r1,#0xC368` = `4c f2 68 31` | `movw r1,#KEY` | 被除数 50024 → KEY |
| 0x0801486A | `adds r0,r0,#1` = `40 1c` | `movs r0,#1` = `01 20` | 除数 → 1 |

MOVW(T3) 推导：imm16=0x6B21 → imm4=6,i=1,imm3=3,imm8=0x21 → hw1=0xF646（`46 f6`），
hw2=0x3121（`21 31`）；原厂 0xC368 → 0xF24C/0x3168 与固件字节一致。v1 复核结论：
0x08014864 的全部 4 个 BL 调用点都经过它；全镜像写 reg4/5 只有 4 处（本函数 2 处 +
初始化表 0x080232BC 静态 0xC3/0x68，启动链 `main→0x08006F58→0x080232BC→0x080118E8→
0x08014864` 会在首次发射前覆盖）。验证日志：`analyze.log`、`unicorn_sync_test.log`
（脚本已改为在内存重建 v1 参考补丁，因为当前 out 已是 v3）。

### v2（ID==99 开关，第二帧固定 500）
- 入口跳转 + 注入结构同 v3；判定条件为 `ID==99 && ID_OFF==0`，第二帧固定
  `SYNC=50024/100=500=0x01F4`。v3 已把判定升级为 KEY_ON 开关并兼容 ID==99，
  第二帧同步字改为按当前 ID 现算。v2 的验证日志保留于 `unicorn_toggle_test.log`
  与 `runtime_toggle_test.log` 的历史修订（脚本已更新到 v3）。

## 8. 风险 / 未确认项（诚实标注）

**已确认**
* 入口/注入/蹦床/寄存器保存：Unicorn 调用级 + 整机真实函数端到端均 PASS。
* v3 判定：KEY_ON=1 双发、KEY_ON=0 普通单发、旧开关 ID=99 双发、ID-OFF 单发、
  KEY_ON 优先于 ID-OFF —— 均有测试覆盖。
* 启动回归：run.py 原厂 vs 补丁 boot log 归一化后逐行一致（2M 指令）。
* 文件长度不变、差异只在 3 个补丁区（332 字节，无越界）。

**不确定 / 风险**
1. **KEY_ON_FLAG=0x20001080 已由 ui-x3c 最终定址**（无字面量引用、40M 指令模拟写追踪
   仅 BSS 清零）。注意：这是 **RAM 版**，设备重启后复位为 0（= 关密钥），需要用户在 UI
   重新打开；flash 持久化由 ui-x3c 后续实现。若将来地址再变，只改 `KEY_ON_FLAG_RAM`
   一个常量并重跑 §6.1/6.3/6.4。
   （0x200004D0 禁用：Lead 核实为固件自用标志 0x0803420A 读 / 0x0800C2AE 写 /
   0x0800E4EA 读。）
2. ~~0x080EE000 属擦除态 0xFF 区，G3 是否写入~~ **已排除**：g3-audit 确认 G3 升级工具
   逐块全部原样发送（包括全 FF 块），注入区 0x080EE000 会被烧写，安全。设备端
   bootloader 行为仍未证实，但按当前证据不影响本补丁；若后续出现异常再考虑改用
   死代码区（如工厂 RF 测试函数 0x080119DC..0x08011AEA，270B，已验证无引用）。
3. **ID-OFF 标志 0x20000143 的语义**：由 `FUN_080118E8` 分支确认（非 0 → 走
   `FUN_0801616C` ID-OFF 路径）；是否还有其它途径置非 0 未穷举，补丁偏保守。
4. **双发是顺序发送**（先 KEY 后普通），两次低层发送各自 STX/SIDLE，耗时约 2 倍；
   真机空口时延未测量。非密钥模式无额外开销。
5. **两次发送之间写 SYNC 未额外关中断**：原厂低层发送内部才 `cpsid i`；若中断在
   "写 SYNC"与"低层发送"之间触发并发送帧，该帧会用新 SYNC。正常固件中断处理不
   调用组帧函数，风险低但未在真机验证。
6. **未在真机刷写验证**：全部结论基于静态分析 + Unicorn/Machine 模拟；刷机有风险，
   请先确认 G3 工具行为并保留原厂 bin 备份。
7. 硬件同步字不是密码学认证：SDR 抓包理论上可克隆；密钥灯必须与引闪器同一把 KEY。

## 9. 交付物

| 文件 | 内容 |
|---|---|
| `keypatch/devices/x3c.py` | v3 设备模块：`DEVICE` / `apply()`（入口跳转 + 0x080EE000 注入，版本校验，长度不变）/ `verify()`（静态 + 差异 + Unicorn 调用级 v3 三组/边界/原厂自证） |
| `keypatch/out/X3_C_v1.38_app_keyD798.bin` | 成品（1,021,468 B，sha256 `af5bd6df…0e39e7`） |
| `keypatch/dev/x3c/analyze_toggle.py` + `analyze_toggle.log` | 静态证据（RAM/发射路径/代码洞/注入反汇编/差异/同步字公式） |
| `keypatch/dev/x3c/unicorn_toggle_test.py` + `.log` | 独立 Unicorn 调用级验证（不导入模块） |
| `keypatch/dev/x3c/runtime_toggle_test.py` + `.log` | 整机模拟器端到端（真实固件函数，抓 SYNC 写 + TX 帧） |
| `keypatch/dev/x3c/boot_regression.py` + `.log` | 原厂 vs 补丁启动回归（run.py 逐行对比） |
| `keypatch/dev/x3c/keypatch_patch_D798.log`、`bin_check.log` | 验收命令输出、bin 长度/哈希/差异 |
| `keypatch/dev/x3c/analyze.py`、`unicorn_sync_test.py`（+ `.log`） | v1（固定 KEY）历史验证脚本 |

---

## 10. 物理按键长按保底方案（v3k，2026-11-07）

> 用户拍板方案 A：不碰触摸/UI，只在原厂按键扫描里加长按动作。
> **长按 M/C ≈1.5s = 切换密钥模式（KEY_ON ^= 1）；长按 ⚡ ≈1.5s = 32 频道广播。**

### 10.1 成品（与 core/UI 成品分开，不覆盖）

| 项 | 值 |
|---|---|
| 成品文件 | `keypatch/out/X3_C_v1.38_app_keyD798_keybtn.bin` |
| 内容 | **core 双发补丁 + 物理按键长按注入**（`apply_keys()`，**不叠加 UI 补丁**；可单独作为保底方案刷机） |
| 长度 / 原厂 sha256 | 1,021,468 B 不变 / `45fdd952…f85ef62` |
| **成品 sha256** | **`7f15788639bbdddd801708bba5d7fea2f8aa0227a9021b1f3b676d34304d38b8`** |
| 与原厂差异 | **738 字节** = core 332B + 按键 406B（入口 4B + 2 个阈值 2B×2 + 注入区 400B） |
| 构建命令 | `tools\python\python.exe keypatch\dev\x3c\keybtn_build.py` |
| 验证命令 | `tools\python\python.exe keypatch\dev\x3c\keybtn_verify.py`（独立断言 + verify_keys）<br>`tools\python\python.exe keypatch\dev\x3c\keybtn_runtime.py`（整机模拟器真实扫描函数） |

### 10.2 三个物理键的定位（GPIO 引脚 + 原厂行为）

`FUN_0800CC30`（按键扫描，main 0x08034322 调用一次/主循环）用
`FUN_08022EB4(port, mask)` 读 3 个键；关键字面量 `0x0800CE04=0x40020000`、
`0x0800CE08=0x40020400`、`0x0800CE0C=0x20000030`：

| 位 | GPIO | mask | 原厂短按 | 原厂长按（阈值） | 是否被本补丁占用 |
|---|---|---|---|---|---|
| bit0 | **PA9** | 0x200 | 调 `FUN_08017018`（发 0xB9/0x30 属性）+ 翻 `0x200003D8`；松开发两次 `0x21` | **无长按动作**（计数器只到 1） | 不占用（拨轮键原样保留） |
| bit1 | **PA8** | 0x100 | 置 `state[0]` bit1；松开翻 `0x200003EB`（原厂 UI 标志） | **>0x28 (40) 次**：state[7]=2、清 `0x200000A0`、`0x08013CDC/0x08035FD0`；若同时按 key2 → About 页（0x08036A14/0x080073F0/0x08013B80） | **占用 → 长按 M/C 切密钥** |
| bit2 | **PB15** | 0x8000 | 无（仅按下/松开状态） | **>0x1E (30) 次**：state[1]\|=4、`0x20000670=0`、state[9]=1 | **占用 → 长按 ⚡ 32 频道广播** |

* **键位与物理丝印的对应关系无法仅从静态确定**（GPIOA8/A9/PB15 没有可读的丝印映射）。
  默认假设 **PA8 = M/C、PB15 = ⚡**（用户拍板手势挂在 M/C / ⚡ 上）。
  **如果实机键位相反**：只改 `devices/x3c.py` 顶部
  `BTN1_PORT/BTN1_MASK`（默认 GPIOA/0x0100）与 `BTN2_PORT/BTN2_MASK`（默认 GPIOB/0x8000）
  两组常量后重跑 `keybtn_build.py` 即可。
* **冲突处理**：key1/key2 的原厂长按动作与用户手势冲突，因此把原厂阈值比较
  `cmp #0x28`/`cmp #0x1E` 原位改成 `cmp #0xFF`（永不触发原厂长按分支），
  短按路径完全不动（已由整机测试验证：key1 短按仍会翻原厂 `0x200003EB`）。
  原厂 key1 长按（隐藏 About 页/显示函数）与 key2 长按（`0x20000670` 清 0、state[9]=1）
  被**有意禁用**；若以后要保留，把 `BTN_DISABLE_ORIGINAL_LONG = False` 重建即可
  （代价：长按 ~0.7–1s 先触发原厂动作，~1.5s 再触发手势）。

### 10.3 注入布局（0x080EE400 起，400 字节）

| 地址 | 内容 |
|---|---|
| 0x080EE400 | 入口跳板：等价原 `push.w {r4..ip,lr}` → `bl btn_update` → `b.w 0x0800CC34`（回原函数体） |
| 0x080EE40C | `btn_update`（每轮扫描一次）：检查 `state[5]==0`；读 key1/key2 电平；长按计数/一次性触发；松开清零 |
| 0x080EE508 | `btn_toggle`：`ldrb r0,[0x20001080]; eor #1; strb` |
| 0x080EE51A | `btn_broadcast`：32 频道循环（set_channel → SYNC=KEY → 功率帧(0xD1,table[4]&0x7F) → 造型灯帧(0xB1) → TEST(0x09)），结束恢复当前信道 + 按当前 ID 恢复原厂同步字 |

补丁点：
1. `0x0800CC30` 入口 4B：`2de9f05f` → `b.w 0x080EE400`；
2. `0x0800CD14`：`28 28` → `ff 28`（禁 key1 原厂长按）；
3. `0x0800CD76`：`1e 28` → `ff 28`（禁 key2 原厂长按）；
4. `0x080EE400` 起 400B 注入（预留 0x800B，到 0x080EEC00，**不碰 0x080F0000 的 ui-x3c 区**）。

手势私有 RAM（与 ui-x3c 的 0x20001084..0x2000108D 错开）：
`0x20001090` key1 长按计数、`0x20001091` key1 已触发、`0x20001092` key2 计数、
`0x20001093` key2 已触发；KEY_ON 仍为 `0x20001080`。均位于 BSS 清零区、全镜像无字面量引用。

长按阈值 `BTN_LONG_TICKS = 60` 次扫描（按原厂长按 40 次≈1s 的节奏估算，约 1.5s；
实际扫描周期未在真机标定，若手感不对改这一个常量即可）。

### 10.4 验证证据（全部可复现，均 PASS）

```
tools\python\python.exe keypatch\dev\x3c\keybtn_build.py
```
真实输出（全文 `keybtn_build.log`）：

```
成品 sha256 : 7f15788639bbdddd801708bba5d7fea2f8aa0227a9021b1f3b676d34304d38b8
差异字节    : 738
[PASS] 按键注入区 0x080EE400 起 400 字节正确（entry=0x080EE400 btn_update=0x080EE40C
       btn_toggle=0x080EE508 btn_broadcast=0x080EE51A size=0x190）
[PASS] 补丁点 0x0800CC30 = e1 f0 e6 bb / 0x0800CD14 = ff 28 / 0x0800CD76 = ff 28
[PASS] 长按 M/C：59 轮=0 → 第60轮=1 → 第61轮=1 → 松开后再长按=0
[PASS] 广播函数：set_channel=33(32+恢复) SYNC写=64 属性帧=64 TEST帧=32 恢复同步字=1
[PASS] 完整扫描：短按不切密钥=0，长按 60 轮后 KEY_ON=1，全程正常返回=True
[PASS] 按键锁 state[5]!=0：70 轮后 KEY_ON=0、广播次数=0
结论: ALL PASS
```

```
tools\python\python.exe keypatch\dev\x3c\keybtn_verify.py   # 独立静态断言 + verify_keys，全 PASS
tools\python\python.exe keypatch\dev\x3c\keybtn_runtime.py   # 整机模拟器（启动到 main 108398 指令）
```

`keybtn_runtime.log` 关键输出：

```
[PASS] 短按后 KEY_ON=0(应0)、原厂短按标志 0x200003EB=1(应1，原厂 release 翻转)、扫描正常返回=True
[PASS] 59 轮=0(应0) → 60 轮=1(应1) → 61 轮=1(应1)
[PASS] set_channel=33(应33) SYNC写=64(应64) 属性帧=64(应64) TEST帧=32(应32) 恢复同步字=1(应1)
结论: ALL PASS
```

整机测试只桩替换 6 个外设/射频接口（读键、set_channel、WriteReg、send_property、
send_short、set_sync），其余（含 `FUN_0800CC30` 原厂逻辑、状态机、BSS、GPIO 写）
全部执行真实固件代码。

### 10.5 风险 / 说明

1. **键位映射是假设**（PA8=M/C、PB15=⚡）：静态无法确证；若实机相反，改两组常量重建即可（§10.2）。
2. **原厂 key1/key2 长按动作被禁用**（用户拍板改成长按手势）：原厂 key1 长按的隐藏
   About 页、key1+key2 组合动作、key2 长按的 `0x20000670`/state[9] 行为不再触发；
   短按完全保持原厂（已测）。要保留则 `BTN_DISABLE_ORIGINAL_LONG=False`。
3. **长按时间按扫描次数估算**（60 次）；若主循环扫描周期与估算不同，实际长按时间可能
   在 ~0.9–2.4s 间，改 `BTN_LONG_TICKS` 一个常量即可。
4. **广播是阻塞式**：32 频道 × (set_channel 含 SIDLE/SCAL 延时 + 2 属性帧 + TEST 帧)
   ≈ 100–200ms，期间主循环被占住（引闪本身也是顺序发，物理上无法真正同时）。
   每帧仍走原厂组帧/发送函数：KEY_ON=1 时 core 补丁会让每帧自动双发（KEY 帧 + 普通帧）。
5. **广播结束恢复**：把信道恢复为 RAM 0x20000054 的当前值、同步字恢复为按当前 ID 的
   原厂值（调 0x08014864），避免后续帧跑到最后一个广播频道/错误同步字。
6. **手势 RAM**（0x20001090–0x20001093）为 BSS 清零区、无字面量引用；若 ui-x3c 后续
   也要用这段 RAM 需协调。
7. **未真机刷写验证**；建议先刷 `_keybtn.bin` 测试长按手势，再决定是否合并 UI 版。
8. 若同时想用 UI 补丁：UI 版 apply() 与按键版是**两个独立 bin**；合并（core+UI+keybtn）
   需要把 `apply_keys()` 的按键注入叠加到 UI 版镜像上（本任务未做，等 Lead 决定）。

### 10.6 本节新增交付物

| 文件 | 内容 |
|---|---|
| `keypatch/out/X3_C_v1.38_app_keyD798_keybtn.bin` | core + 按键长按成品（sha256 `7f157886…4d38b8`） |
| `keypatch/devices/x3c.py` 新增 | `_apply_core_only` / `apply_keys` / `verify_keys` / `build_btn_injection` / `_btn_patched_regions` + 按键测试辅助 |
| `keypatch/dev/x3c/keybtn_build.py` + `keybtn_build.log` | 构建 + verify_keys（Unicorn 桩按键序列） |
| `keypatch/dev/x3c/keybtn_verify.py` + `keybtn_verify.log` | 对成品 bin 的独立静态断言 + verify_keys |
| `keypatch/dev/x3c/keybtn_runtime.py` + `keybtn_runtime.log` | 整机模拟器端到端（启动到 main 后跑真实扫描函数） |

---

## 11. 实机反馈修正：键位探针版 + 正式版 v3k2（2026-11-07 04:0x）

> 实机反馈：刷 `_keybtn.bin` 后"长按 M/C 没用"（无视觉反馈，无法确认是否触发）。
> 因此新增两个**独立成品**（不覆盖任何既有 bin）：
> 1) **探针版** `_keyprobe.bin`：长按三个物理键分别闪 **1 / 2 / 3 次**，用来确认键位；
> 2) **正式版** `_keybtn2.bin`：长按**拨轮**切密钥、长按 **⚡** 广播、M/C 长按不再绑定，
>    并修复了旧版可能不触发的三处问题（见 §11.4）。

### 11.1 探针版 `X3_C_v1.38_app_keyD798_keyprobe.bin`

| 项 | 值 |
|---|---|
| 内容 | core 双发补丁 + 三键长按探针（**不叠加 UI 补丁**） |
| 长度 / 差异 | 1,021,468 B 不变 / **888 字节**（core 332 + 按键 556） |
| **sha256** | **`847491e6fef1e4de69e58e53a43bfc9533e524cb4b3d83de5cac275fcfebeb7e`** |
| 按键注入区 | 0x080EE400..0x080EE62C（556B；布局 entry/btn_update/probe1@0x080EE52A /probe2@0x080EE580 /probe3@0x080EE5D6） |
| 构建 / 验证 | `keybtn_build.py --mode keyprobe` / `keybtn_verify.py` / `keybtn_runtime.py` |

**探针行为**（长按阈值 25 次扫描 ≈ 用户按 1.5~2s）：
* bit0（PA9）长按 → **闪 1 下**；bit1（PA8）长按 → **闪 2 下**；bit2（PB15）长按 → **闪 3 下**；
* 探针帧**绕过 core 的 send_short 双发包装**，直接调低层发送 0x0801712C：
  先 `write_reg(4/5)` 写 **当前 ID 的公共同步字** `floor(50024/(ID+1))`，再发 N 个
  `[D5,09]` TEST 帧 → 用户手里"同 ID 的原厂灯 / 切到普通 ID 的补丁灯"一定能看到 1/2/3 闪，
  **且完全不受 KEY_ON 影响**；
* 短按完全原厂；三个键的原厂长按动作（bit1/bit2）被阈值补丁禁用，避免混入原厂动作。

**使用说明**：刷 `_keyprobe.bin` → 引闪器 ID 设为 5（或任意 1..99）→ 灯设为同一 ID 的普通模式
（补丁灯把 ID 从 OFF 改到 5；或直接用原厂灯）→ 依次长按三个物理键并记下闪几下：
闪 1 = bit0(PA9)，闪 2 = bit1(PA8)，闪 3 = bit2(PB15)。把对应关系告诉 Lead 即可定稿键位。

### 11.2 正式版 `X3_C_v1.38_app_keyD798_keybtn2.bin`

| 项 | 值 |
|---|---|
| 内容 | core 双发补丁 + 长按拨轮切密钥 + 长按 ⚡ 32 频道广播（不叠加 UI 补丁） |
| 长度 / 差异 | 1,021,468 B 不变 / **677 字节**（core 332 + 按键 345，且**不再禁用 bit1 原厂长按**） |
| **sha256** | **`7a1f147887351583586755f013ccf83f36cfb489b29f71f04954d0985db409b4`** |
| 按键注入区 | 0x080EE400..0x080EE554（340B；toggle@0x080EE4CC，broadcast@0x080EE4DE） |
| 构建 / 验证 | `keybtn_build.py --mode keybtn2` / `keybtn_verify.py` / `keybtn_runtime.py` |

键位默认（可按探针结果一行改常量重建）：
* **长按拨轮 = bit0 / PA9（mask 0x200）→ KEY_ON ^= 1**；
* **长按 ⚡ = bit2 / PB15（mask 0x8000）→ 32 频道广播**（同 §10.3 广播实现）；
* **M/C = bit1 / PA8：长按不再绑定手势**，且 `0x0800CD14` 阈值**保持原厂 0x28**
  （即 M/C 原厂长按动作恢复，不再被禁用）；bit2 阈值仍改 0xFF（⚡ 原厂长按被广播替代）。

### 11.3 为什么旧版 M/C 长按可能没触发（三处已修正）

1. **`state[5] != 0` 时整段跳过手势（主因嫌疑）**：旧版 `btn_update` 一进来检查
   `*(u8*)0x20000035`，非 0 就清零计数并返回。该字节实为原厂"按键锁/忙"标志：
   * main 启动时 0x08033EBA 置 1，之后 0x0803400C / 0x080340DE 清 0；
   * 编码器类回调 `FUN_0801FEDC`（无 BL 调用者，走间接调用）也检查它，并在 PB15 按下且
     state[5]==0 时清 ID_OFF 标志 `0x20000143`、重设按键计时器。
   若用户长按时该字节恰好非 0，旧版会**永远跳过**手势。**新版已完全删除该跳过判断**，
   并增加"state[5]!=0 时手势仍触发"的测试（Unicorn + 整机均 PASS）。
2. **长按阈值可能偏大**：旧版 60 次扫描；主循环扫描周期未真机标定（可能 30~80ms），
   若 >25ms 则 60 次 >1.5s，用户按 1.5s 可能不够。新版探针 25 次、正式版 30 次。
3. **键位映射可能不对**：旧版把 M/C 绑在 PA8；若实机 M/C 是 PA9/PB15，则长按 M/C 自然无反应
   （探针版就是用 1/2/3 闪来解决这个问题的）。

补充：`FUN_0801FEDC` 也读 **PB15 mask 0x8000**，说明 PB15 也可能是"拨轮按下"而非 ⚡；
所以正式版默认仍按 Lead 给的假设（拨轮=PA9、⚡=PB15），**以探针结果为准**。

### 11.4 验证输出（全部 PASS）

`keybtn_build.py --mode all`（Unicorn 桩按键序列）：

```
keyprobe sha256=847491e6…ebeb7e 差异 888  [PASS] 探针：bit0 长按->1帧(应1) SYNC=0x2091；bit1->2帧；bit2->3帧；KEY_ON 不变
keyprobe [PASS] 完整扫描短按：三键短按后 KEY_ON=0、bit1 原厂标志 0x200003EB=1、正常返回=True
keybtn2  sha256=7a1f1478…409b4 差异 677  [PASS] 拨轮(bit0)长按 KEY_ON 1→0(应1→0)
         [PASS] ⚡(bit2)长按 set_channel=33 SYNC写=64 属性帧=64 TEST=32 恢复同步=1
         [PASS] M/C(bit1)长按无手势=True
         [PASS] state[5]!=0 时拨轮长按仍切 KEY_ON=1
结论: ALL PASS
```

`keybtn_verify.py`（两个成品独立静态断言 + verify_keys）：ALL PASS。

`keybtn_runtime.py --mode all`（整机模拟器：真实启动到 main 108,398 条指令后反复调用真实
FUN_0800CC30，仅桩 8 个外设/射频/原厂动作接口）：

```
[PASS] 三键短按：KEY_ON=0、bit1 原厂标志 0x200003EB=1、扫描正常返回=True
[PASS] 探针长按（ID=5）：bit0=1帧(应1), bit1=2帧(应2), bit2=3帧(应3)，同步字均=0x2091
[PASS] 正式版：拨轮(bit0)长按 KEY_ON=1→0；⚡(bit2)长按 频道=33 SYNC写=64 属性帧=64
       TEST=32 恢复同步=1；M/C(bit1)长按无手势；state[5]!=0 时拨轮长按仍切 KEY_ON=1
结论: ALL PASS
```

### 11.5 风险

1. **键位仍是假设**：拨轮=PA9、⚡=PB15、M/C=PA8；以探针结果为准。若探针显示不同，
   改 `devices/x3c.py` 的 `BTN_WHEEL_PORT/BTN_WHEEL_MASK`、`BTN_TRIG_PORT/BTN_TRIG_MASK`
   两组常量后 `keybtn_build.py --mode keybtn2` 重建（阈值禁用位会按绑定键自动计算）。
   例：若探针显示拨轮=PB15、⚡=PA9，则改成 wheel=`BTN2_*`、trig=`BTN0_*`（bit0 无原厂长按，
   无需禁用；bit2 自动禁用）。
2. 删掉 `state[5]` 跳过后，原厂"按键锁/忙"期间长按也会触发手势（用户要的就是可靠触发；
   如觉得误触可加回，但要接受旧问题的风险）。
3. 阈值 25/30 次仍是估算；实机若觉得太短/太长，改 `BTN_PROBE_TICKS` / `BTN_KEYBTN2_TICKS`。
4. 探针帧用公共同步字：只会让"同 ID 的普通模式灯/原厂灯"闪；用户自己的补丁灯（ID=OFF）
   不会闪——这是有意的（避免 KEY 同步字干扰判断）。测试时把灯切到普通 ID。
5. 未真机刷写验证新版；两版都与旧 `_keybtn.bin`、`X3_C_v1.38_app_keyD798.bin`、UI 版互不覆盖。

### 11.6 本节新增交付物

| 文件 | 内容 |
|---|---|
| `keypatch/out/X3_C_v1.38_app_keyD798_keyprobe.bin` | 键位探针版（sha256 `847491e6…ebeb7e`） |
| `keypatch/out/X3_C_v1.38_app_keyD798_keybtn2.bin` | 正式版（sha256 `7a1f1478…409b4`） |
| `keypatch/devices/x3c.py` | 按键注入参数化：`apply_keys(data,key,mode)` / `verify_keys(patched,key,mode)`，mode=`keyprobe`/`keybtn2` |
| `keypatch/dev/x3c/keybtn_build.py` + `keybtn_build_keyprobe.log`、`keybtn_build_keybtn2.log` | 两个成品构建 + Unicorn 验证 |
| `keypatch/dev/x3c/keybtn_verify.py` + `keybtn_verify.log` | 两个成品独立静态断言 + verify_keys |
| `keypatch/dev/x3c/keybtn_runtime.py` + `keybtn_runtime.log` | 两个成品整机模拟器端到端 |
| `keypatch/dev/x3c/bin_check_keybtn.log` | 两个成品的长度/sha256/差异汇总 |

---

## 12. 最终版 keybtn4：长按拨轮/M/C 两段式（1.5s 密钥 / 20s 关机）（2026-11-07 04:3x）

> 用户追加需求：保留关机，但改成"拨轮/M/C 按住 20 秒关机"；1.5s 切密钥。
> 先做了原厂关机链路逆向（§12.1），再实现 keybtn4（§12.2）。

### 12.1 原厂关机链路（关键逆向结论）

**原厂按键关机 = 长按 bit1/PA8（探针"闪 2 下"的键 = 用户说的 ⚡），约 40 次扫描（1.5~2s）**：

```
main 0x08034322  bl FUN_0800CC30（按键扫描）
  └─ bit1 长按分支（计数器 > 0x28=40，代码 0x0800CD18 附近）
       ├─ FUN_08013CDC()           # 显示收尾
       └─ FUN_08035FD0()           # 0x08035FD0 原厂长按关机流程
            ├─ state[7]=2；*0x2000066C=0；FUN_08013CDC()
            ├─ 最多 6 轮：FUN_08014634() + FUN_0803BA4C()（等显示/音频收尾）
            ├─ state[6]=1
            └─ FUN_08010C00()      # 0x08010C00 关机本体（不返回）
```

**关机本体 `FUN_08010C00`（0x08010C00，128 B）**：
`FUN_08013B80`（停显示/UI）→ `FUN_080393C4` → `FUN_0801BD04` → `cpsid i` →
`FUN_08036010` → `FUN_0801BAC0(1)`/`FUN_0801BAC0(0)`（RF/电源）→ `FUN_080202BC` →
`cpsie i` → `FUN_08013CDC` → `FUN_08037470` → **循环**：`FUN_0803BA4C` +
`FUN_0801C0FE(1)`，并轮询 **PA8(mask 0x100) 是否仍按住**（按住继续、松开跳出），
同时 state[0x10] 倒计时；最后 `FUN_0801C0FE(0x14)`、`FUN_08036084`、
`FUN_0803604C`、`FUN_0801C118(0x78)` → 软关机/断电序列（不返回）。
即"按住 PA8 不放 → 进入关机流程；松手 → 完成关机"。

**其它关机入口（均未修改，保留）**：
* 菜单电源图块/关机确认流程：`FUN_08010C8C` 内调 `FUN_08010C00`；
* 低电量自动关机：main 每循环 0x0803431E 调 `FUN_08005D64`（ADC/电池），
  条件满足后 `FUN_08010C00`。

**因此原厂"长按关机"在 bit1(PA8/⚡) 上，M/C/拨轮(bit0) 原本没有长按关机。**
新方案把 bit1 长按改为广播（禁用其原厂长按阈值 0x28→0xFF），
并把关机迁到 bit0(PA9) 按住 400 次（≈20s）。

### 12.2 最终成品 keybtn4

| 项 | 值 |
|---|---|
| 文件 | `keypatch/out/X3_C_v1.38_app_keyD798_keybtn4.bin` |
| 内容 | core 双发 + **两段式长按**（拨轮/M/C：30 tick 切 KEY_ON、400 tick 关机；⚡：30 tick 广播；bit2 原厂） |
| 长度 / 差异 | 1,021,468 B 不变 / **721 字节**（core 332 + 按键 389） |
| **sha256** | **`ffaaad4b9ceda3641cbef2af37316445418169a196b45e6168855de4129a93e7`** |
| 按键注入区 | 0x080EE400..0x080EE580（384 B；toggle@0x080EE4EC，broadcast@0x080EE4FE，shutdown@0x080EE574） |
| 补丁点 | 0x0800CC30 入口 b.w；**0x0800CD14 bit1 阈值 0x28→0xFF**（⚡ 原厂长按关机被广播替代）；bit2 0x0800CD76 **保持原厂 0x1E**；0x080EE400 注入 |
| 按键 RAM | bit0 两段计数 u16=0x20001096、stage1 已触发=0x20001098、stage2 已触发=0x20001099；⚡ 计数=0x20001092/93；KEY_ON=0x20001080 |
| 短按 | 三键短按保持原厂（bit1 release 仍翻 0x200003EB，已测） |
| state[5] | 不再跳过（实机根因修复，已测 state[5]!=0 时仍触发） |

**两段式逻辑**（注入 `_emit_two_stage_watch`）：
按住 bit0 后 **u16 计数每轮 +1**：
* 计数到 **30** → `fired1=1`，`KEY_ON ^= 1`（只一次）；
* 继续按住到 **400** → `fired2=1`，调 `FUN_08013CDC` + `FUN_08035FD0`（原厂完整关机流程，不返回）；
* 中途松开 → 计数/fired1/fired2 清零；短按完全原厂；
* 进入关机时 PA9 按住、PA8 未按住 → `FUN_08010C00` 内 PA8 轮询立即退出、正常完成关机。

### 12.3 验证（全部 PASS）

Unicorn（`keybtn_build.py --mode keybtn4` → `keybtn_build_keybtn4.log`）：

```
keybtn4 sha256=ffaaad4b…9a93e7 差异 721
[PASS] 注入区 0x080EE400 起 384 字节；布局 toggle=0x080EE4EC broadcast=0x080EE4FE shutdown=0x080EE574
[PASS] 模式行为 keybtn4：30 tick：KEY_ON=1(应1) 关机=0(应0)；
       400 tick：关机=1(应1) 再按 10 tick=1(应1) KEY_ON 保持=1；
       松开后再按 30 tick：KEY_ON=0(应0) 关机次数仍=1(应1)；
       ⚡(bit1)长按：频道=33 SYNC写=64 属性帧=64 TEST=32 恢复同步=1；
       bit2 长按无动作=True；state[5]!=0 时拨轮长按仍切 KEY_ON=1
[PASS] 短按完全走原厂；与原厂差异 721 字节全部落在补丁区
```

独立验证 `keybtn_verify.py`（keyprobe/keybtn2/keybtn3/keybtn4 四成品）：ALL PASS。

整机模拟器 `keybtn_runtime.py --mode keybtn4`（真实启动到 main 108,398 条指令后反复调真实
FUN_0800CC30，仅桩外设/射频/动作接口；关机桩记录 FUN_08035FD0 调用）：

```
[PASS] 三键短按：KEY_ON=0、bit1 原厂标志 0x200003EB=1、正常返回
[PASS] keybtn4：30 tick KEY_ON=1 关机=0；400 tick 关机=1 再按 10 tick=1；
       松开再按 30 tick KEY_ON=0 关机仍=1；⚡(bit1)广播 33/64/64/32/1；
       bit2 无动作；state[5]!=0 时拨轮长按仍切 KEY_ON=1
结论: ALL PASS
```

整机启动回归 `boot_regression.py --image .../keybtn4.bin --log keybtn4_boot_regression.log`：
`PASS（无启动回归）`。

### 12.4 风险 / 说明

1. 400 tick=20s 基于"30 tick≈1.5s"的扫描周期假设（约 50ms/次）；若实机手感不符，
   改 `BTN_KEYBTN4_TICKS1/2` 两个常量重建。
2. 关机走原厂 `FUN_08013CDC`+`FUN_08035FD0`（与旧按键关机同一条流程），不改菜单关机/
   低电量自动关机；`FUN_08010C00` 内部轮询 PA8，新触发键是 PA9，故不会卡在等待。
3. 关机动作不可逆、且注入层不再返回；Unicorn/整机测试用桩验证"调用一次"。
4. 未真机刷写验证；建议先刷 `_keybtn4.bin` 测：短按原厂 → 长按 1.5s 切密钥（灯闪行为变化可
   用 KEY/普通同步字观察）→ 按住 20s 关机；⚡ 长按广播；bit2 无新行为。
5. `keybtn3`（sha `e3041a5b…8f5ff9`）为两段式之前的过渡版，**最终请用 keybtn4**。

### 12.5 本节新增交付物

| 文件 | 内容 |
|---|---|
| `keypatch/out/X3_C_v1.38_app_keyD798_keybtn4.bin` | 最终版（sha256 `ffaaad4b…9a93e7`） |
| `keypatch/devices/x3c.py` | 新增 mode=`keybtn4`、`_emit_two_stage_watch`、`_emit_shutdown`、`SHUTDOWN_*` 常量、u16 计数器指令 |
| `keypatch/dev/x3c/keybtn_build_keybtn4.log` | 构建 + Unicorn 两段式验证 |
| `keypatch/dev/x3c/keybtn4_boot_regression.log` | keybtn4 整机启动回归 |
| `keypatch/dev/x3c/keybtn_runtime.log` | 整机 runtime（含 keybtn4 两段式） |
| `keypatch/dev/x3c/bin_check_keybtn.log` | 四成品长度/sha256/差异 |
| `keypatch/dev/x3c/boot_regression.py` | 增加 `--image/--log` 参数（默认行为不变） |

---

## 13. 无广播变体 keybtn4_nobc_a / keybtn4_nobc_b（2026-11-07 04:4x）

用户要求"没有广播功能的引闪器固件"，基于 keybtn4 出两个变体，其余完全相同：

| 项 | A 版（恢复 ⚡ 原厂关机） | B 版（⚡ 长按无绑定） |
|---|---|---|
| 文件 | `X3_C_v1.38_app_keyD798_keybtn4_nobc_a.bin` | `X3_C_v1.38_app_keyD798_keybtn4_nobc_b.bin` |
| **sha256** | `7e4aa2018e05b6e0c44f78e69397c5dcf6ccabf6359fb87688ee42e55f7bbccb` | `269ffc0f12b6609fb6608112c63b5f7b12a441f7c127978daa04e6517d3165b4` |
| 与原厂差异 | **508 字节** | **509 字节** |
| 注入区 | 0x080EE400..0x080EE4AC（172 B，**不含 broadcast**） | 同 A |
| bit1(⚡) 原厂长按阈值 0x0800CD14 | **0x28 原厂**（原厂长按关机恢复） | **0xFF**（禁用，且无动作） |
| bit1 长按行为 | **原厂关机**（FUN_0800CC30 原厂长按分支 → FUN_08013CDC + FUN_08035FD0） | **无任何动作** |

**共同行为**（完全一致的 507 字节差异 + 注入代码）：
1. **长按拨轮/M/C（bit0/PA9）30 tick ≈1.5s → 切密钥 KEY_ON（0x20001080 ^= 1，任意 ID 生效、只一次）**；
2. **继续按住到 400 tick ≈20s → 关机**（原厂 `FUN_08013CDC`+`FUN_08035FD0` 完整流程）；
3. **无广播注入**（注入区里没有 broadcast 函数，仅 172 B）；
4. bit2/PB15 保持原厂；三键短按完全原厂；中途松开清零；state[5] 跳过保持删除；
5. 两版**只差 1 字节**：文件 offset **0x08514**（VA 0x0800CD14）A=0x28、B=0xFF，其余逐字节相同。

### 13.1 验证（全部 PASS）

Unicorn（`keybtn_build.py --mode keybtn4_nobc_a/b`，见 `keybtn_build_keybtn4_nobc_a.log`、`_b.log`）：
```
A: 30 tick KEY_ON=1 关机=0；400 tick 关机=1、再按 10 tick 仍=1；松开再按 30 tick KEY_ON=0 关机仍=1；
   ⚡ 长按（50 scan 原厂完整扫描）：原厂关机 FUN_08035FD0 调用=1、无广播帧=True；bit2 无动作；state[5]!=0 仍切
B: 同样两段式；⚡ 长按：无任何动作（关机=0、广播帧变化=False）；bit2 无动作
两版均 [PASS] 无广播注入：layout 无 broadcast
```
`keybtn_verify.py`（6 成品）：ALL PASS；`keybtn_runtime.py --mode all`（6 成品整机）：ALL PASS；
boot 回归：`keybtn4_nobc_a_boot_regression.log` / `keybtn4_nobc_b_boot_regression.log` 均 `PASS（无启动回归）`。

### 13.2 最终手势说明（用户版）

- **长按 M/C/拨轮 1.5 秒 → 密钥开关**（KEY_ON 切换，任意 ID 生效）
- **继续按住 M/C/拨轮 到 20 秒 → 关机**
- **长按 ⚡**：A 版 = **原厂关机**；B 版 = **无任何功能**
- bit2/PB15、所有短按：完全原厂；**两版都没有广播功能**（keybtn4 才有广播）

### 13.3 风险

1. 20s 阈值按"30 tick≈1.5s"（≈50ms/扫描）换算，可改 `BTN_KEYBTN4_TICKS1/2`。
2. A 版恢复原厂 ⚡ 长按关机（约 1.5–2s 触发、松手完成关机）；若不想保留任何 ⚡ 长按，用 B 版。
3. 关机不可逆；测试用桩验证"调用一次"。
4. 未真机刷写验证；推荐先刷 **A 版**（最接近原厂 + 密钥开关），B 版作为"完全不要 ⚡ 长按行为"的选择。

---

## 15. keybtn7：双击 M/C = 32ch×100ID 公共同步字广播（2026-11-07 05:2x）

用户最终手势（基于 keybtn5）：
1) **长按 M/C/拨轮 1.5s（30 tick）→ 切密钥 KEY_ON**（保留，任意 ID）；
2) **双击 M/C/拨轮（两次短按，间隔 <400ms、每次 <300ms）→ 32 频道 × 全部 ID(0..99) 广播引闪**；
3) **按住 M/C/拨轮 20s → 关机**（保留）；
4) **⚡ 完全原厂**（单击/长按电源行为，不做任何新检测）；无长按广播；
5) 保留双发白名单（send_short 仅 cmd=0x09 双发、属性帧双发）。

### 15.1 成品
| 项 | 值 |
|---|---|
| 文件 | `keypatch/out/X3_C_v1.38_app_keyD798_keybtn7.bin` |
| **sha256** | **`b53c1ed573f2aabd1aab4294511ce96b06668f5ebbb0161f89c86309d4652d22`** |
| 长度/差异 | 1,021,468 B 不变 / **1151 字节** |
| 按键注入区 | 0x080EE400..0x080EE72E（814 B；toggle/shutdown/start_broadcast，无 broadcast 长按函数） |
| 广播状态 RAM | 0x200010AA active、0x200010AB saved KEY_ON、0x200010AC ch、0x200010AD id、0x200010A8 pending、0x200010A9 prev、0x200010A0/A4 press_start/first_press；长按计数 0x20001096(u16)、去抖 0x20001098 |
| 双击计时 | tick=0x20000310（u32 1ms；0x2000030C 为 seqlock 字节，读/加 `0x080334D8`/`0x080334EC`） |

### 15.2 实现要点
- **三手势共存（bit0/PA9）**：短按原厂；tick 记录按下/松开沿；两次 <300ms 短按且按下沿间隔 <400ms → 广播；
  长按 30 tick 切 KEY_ON **并清 pending（长按取消双击判定）**；继续 400 tick 关机；>600 tick 看门狗清零；松开清零。
- **广播（public 全 ID）**：保存 KEY_ON → 置 0（保证 send_short 白名单走单发、用我们设的 sync）→
  ch 0..31 × id 0..99：id==0 时 set_channel(ch) → set_sync(id)（原厂 0x08014864，公式 50024/(id+1)）→ send_short(0x09)
  → 结束恢复 KEY_ON 原值。**不改变密钥模式状态**。
- **分块 + 喂狗（防 IWDG 复位）**：
  * 广播由 `bc_run` 状态机续跑，**每次主循环最多发 BTN7_BUDGET=16 帧**，游标在 RAM；
  * 每块开始调用原厂喂狗函数 **0x0803BA4C**（写 IWDG_KR=0xAAAA）；
  * 原厂 main（FUN_08033E20，9 处）与按键扫描（0x0800CD44）本来每轮也调该喂狗函数；
  * IWDG 的 PR/RLR 配置在 app 内未找到直接写点（辅助函数 0x0803BA58.. 无 BL 调用者，疑在 bootloader）；
    因此采用"分块 + 每块喂狗"双保险：单次阻塞 ≤16 帧（几十 ms），远小于任何合理 IWDG 超时。
- 3200 帧预计 **十几秒**（每帧 set_sync 自带 SCAL/延时，chunk≈16 帧/主循环）；期间**屏幕卡住属正常**。

### 15.3 验证（全部 PASS）
Unicorn（`keybtn_build_keybtn7.log`）：
* 双击 M/C：channel=32、set_sync=3200、send(高 层)=3200，**单块最大=16**，set_sync id 序列 0..99×32 正确，KEY_ON=1→1（初始 1）；
* KEY_ON=0 时广播后仍 0；单击不广播；长按 30 tick 切 KEY_ON 且不广播；继续到 400 tick 关机=1；
* 三连击只广播一次；空闲 2000 tick 无动作；双击 ⚡ 不广播；短按三键原厂（0x200003EB 仍翻转）；M/C 两段式保持。
整机 runtime（`keybtn_runtime.py --mode keybtn7`，真实固件扫描函数）：ALL PASS（32/3200、单块≤16、KEY_ON 恢复、
单击不广播、长按切/关机、⚡ 无新行为）。
启动回归（`keybtn7_boot_regression.log`）：PASS（无启动回归）。

### 15.4 说明/风险
1. 双击 M/C 时两次短按**各自会触发原厂短按动作**（M/C 原厂行为），属保留行为。
2. 广播期间屏幕卡住、其它按键手势暂停（active 时只跑广播状态机）；结束自动恢复 KEY_ON。
3. 若实机 IWDG 超时很短（未知 bootloader 配置），分块+每块喂狗已尽量保守；如仍复位可把 BTN7_BUDGET 降到 4~8。
4. 未真机刷写验证；keybtn5 仍保留作为上一版。
5. 旧 bin 均为旧全双发注入；keybtn7 内含白名单（core 注入 328 B 同 keybtn5 版本）。

---

## 16. keybtn8：热靴触发不引闪修复（2026-11-07 05:5x）

用户实机：**手按 ⚡（按键 TEST）能引闪密钥灯，但相机热靴触发不引闪**。

### 16.1 根因（静态证据）
- 相机热靴 → 热靴 MCU → **SPI1 链路** → app 中断入口 **0x08012984**（HARDWARE.md：IRQ 35 = SPI1，RX 字节经 0x08038216/0x080381DA 存入 RAM）；
- 热靴"相机触发"处理代码在 **0x08013262..0x08013688**（ghidra 未单列函数，由 0x08012984 分支进入），其中：
  * `send_property`（0x0801184C）多次：把相机设置/功率等 A9 属性帧发给灯；
  * `send_short` 调用点与命令：
    - **0x080133D8：cmd=0x1A**
    - **0x080135E8：cmd=0x21**
    - **0x080135F4：cmd=0x21**
    - **0x08013642：cmd=0x29**
  * 帧间有 `0x0801C118` 延时（150/200µs 等）。
- 按键 TEST 路径：PA9 短按 → `FUN_08017018` → **send_short(0x09)**（0x08017062）。
- keybtn5/7 白名单只双发 **cmd=0x09**；热靴触发用的 **0x1A/0x21/0x29 都走了"白名单外单发"** → 只发普通同步字帧，密钥模式灯（ID OFF）收不到 → 不引闪。属性帧本来双发，所以灯可能"有反应但不闪"。
- 修复：白名单扩展为 **{0x09, 0x1A, 0x21, 0x29}** 双发；其它命令（休眠/状态：0x40/0x41/0x80/0x81/0x88/0x89 等）仍单发，保持"休眠前不滴滴响"的修复。

### 16.2 成品 keybtn8（= keybtn7 手势 + 热靴白名单）
| 项 | 值 |
|---|---|
| 文件 | `keypatch/out/X3_C_v1.38_app_keyD798_keybtn8.bin` |
| **sha256** | **`6d81308f41ac53f844a7c382a8efc8a3dda5b5bad93b8a68508d987623586dd4`** |
| 长度/差异 | 1,021,468 B 不变 / **1167 字节** |
| 按键注入区 | 0x080EE400..0x080EE72E（814 B，同 keybtn7） |
| 手势 | 长按 M/C 1.5s=密钥开关；双击 M/C=32ch×100ID 公共同步字广播（分块+喂狗）；按住 20s=关机；⚡ 完全原厂；无长按广播 |
| 双发白名单 | send_short: **0x09/0x1A/0x21/0x29 双发**，其它单发；send_property 仍全部双发 |

### 16.3 验证（全部 PASS）
- Unicorn（`keybtn_build_keybtn8.log`）：
  * `send_short(cmd=0x09/0x1A/0x21/0x29)` 密钥模式 → **KEY 帧 + 普通帧双发**（SYNC 0xD798 → 50024/6=0x2091）；
  * `cmd=0x11/0x40` → 单发、不写 SYNC；
  * keybtn7 全部手势/广播/长按/空闲/⚡ 无新行为测试全部 PASS。
- 整机 runtime（`keybtn_runtime.py --mode keybtn8`）：ALL PASS（32/3200、单块≤16、KEY_ON 恢复、长按切/关机、⚡ 无新行为）；短按原厂。
- keybtn8 启动回归（`keybtn8_boot_regression.log`）：PASS（无启动回归）。
- keybtn5/keybtn7 未改动。

### 16.4 "速度慢"分析（待用户确认指向）
1. **双击广播**：3200 帧 × (set_sync 0x08014864 自带 SCAL/延时 + RF 发送) ≈ **十几秒**；
   若要更快可把 ID 步长改为 2 或 5（1600/640 帧）或减少频道数——需用户确认可接受"漏掉部分 ID"。
2. **按键/热靴引闪延迟**：密钥模式每帧双发（KEY + 普通）→ RF 占用时间约 ×2；
   热靴路径本身还会先发多条属性帧（含 150–200µs 延时），双发进一步放大；
   若用户指"热靴引闪慢"，可后续把热靴路径的**属性帧改为单发/仅触发帧双发**（需再评估灯设置同步）。
3. 建议向用户确认：慢是指"双击广播十几秒"还是"每次引闪延迟大"，再定优化方向。

---

## 14. keybtn5：双发白名单修复"休眠前灯滴滴响"（2026-11-07 05:0x）

用户实机：密钥模式约 3 分钟（休眠/待机）时灯"滴滴响、不闪光"；普通模式不响。

### 14.1 根因（高置信）
key 模式下 core 包装对**每一次** `send_short(cmd)` 都双发（私钥帧 + 普通帧）。
休眠/待机前固件会发一条非 TEST 的短命令帧；普通模式下它单发，密钥灯按普通同步收不到/不处理；
密钥模式下我们的包装把这条短命令也用 **KEY 同步字**发了一次，密钥灯收到后作出"滴滴"响应。
→ 修复：**send_short 双发白名单：只有 cmd==0x09（TEST/引闪）双发，其它 cmd 一律原厂单发**
（`send_property` 仍双发，功率/造型灯等设置继续到达密钥灯）。
3 分钟空闲计时器的精确位置仍在定位（候选：main/UI 状态机 FUN_08006F0C 内的 0x40/0x41/0x80/0x81/0x88/0x89 短命令，
时间常数可能为 1800×100ms 或 180s）；但白名单对**所有**非 0x09 短命令生效，因此无论该帧具体是哪个 cmd 都会被覆盖。
若后续发现 3 分钟帧是 **A9 属性帧**，需要在 prop 包装里按 (group,key,val) 再加排除。

### 14.2 成品 keybtn5（无广播 + ⚡ 完全原厂 + 白名单）
`keypatch/out/X3_C_v1.38_app_keyD798_keybtn5.bin`
| 项 | 值 |
|---|---|
| sha256 | **`12c1736c1cd67ca4a4569e9ecdc0e4fea38ed82e17cc3076e162c1abebb08c25`** |
| 与原厂差异 | 547 字节（core 注入含白名单 + 按键注入 198B，无广播） |
| 按键逻辑 | 单 u16 计数器、无 latch：30 tick 切 KEY_ON、400 tick 关机、松开清零、>600 看门狗清零；MODER 非输入（或读不到=0xFFFFFFFF）时按未按下处理；连续 3 次去抖 |
| bit1/⚡ | **完全不动**（0x0800CD14 保持原厂 0x28，原厂长按关机/电源行为恢复） |
| bit2/PB15 | 原厂不动 |
| 短按 | 全部原厂 |

### 14.3 验证
- Unicorn（`keybtn_build_keybtn5.log`）：ALL PASS；含
  * `send_short(cmd=0x09)` 密钥模式 → 双发（KEY 帧 + 普通帧）；
  * `send_short(cmd=0x11)` 密钥模式 → **单发、不写 SYNC**（白名单外）；
  * `send_property` 密钥模式 → 仍双发；
  * 30 tick 切、400 tick 关机一次、外部清零 counter 后再按仍能切、空闲 2000 tick 不误动作、MODER 输出时不计、⚡ 长按走原厂关机路径（无广播）。
- 整机 runtime（`keybtn_runtime.py --mode keybtn5`）：ALL PASS。
- 键位/启动回归（`keybtn5_boot_regression.log`）：PASS（无启动回归）。

### 14.4 最终交付状态 / 遗留说明

**X3-C 最终版 = keybtn5**（用户拍板，2026-11-07）：
`keypatch/out/X3_C_v1.38_app_keyD798_keybtn5.bin`
sha256 **`12c1736c1cd67ca4a4569e9ecdc0e4fea38ed82e17cc3076e162c1abebb08c25`**，差异 547 字节。
手势：**长按 M/C/拨轮 1.5s=密钥开关 / 按住 20s=关机 / ⚡ 长按=原厂电源行为 / 无广播**；
含双发白名单（send_short 仅 cmd=0x09 双发，属性帧双发），修休眠前灯滴滴响。

**已取消**：用户决定不再做"双击 ⚡ 切密钥"的 keybtn6；相关实现未写入固件、未产出 bin，
模块里也没有 keybtn6 模式（`_btn_cfg('keybtn6')` 会报未知模式）。本 Notes 不再保留 keybtn6 待办。

**遗留/已知风险（非阻塞）**：
1. **3 分钟休眠帧的精确 cmd/属性未最终钉死**：已确认 `send_short` 全部调用点 cmd 集合
   （0x09/0x19/0x1A/0x21/0x40/0x41/0x80/0x81/0x88/0x89 等）；用户"普通模式不响、密钥模式响"
   与"非 TEST 短命令被双发"一致，白名单已对所有非 0x09 短命令生效。
   若实机仍响，则说明该帧是 **A9 属性帧**，需再加 (group,key,val) 属性白名单（目前保留属性双发）。
2. **1ms tick 已定位**：`0x20000310` 是 32-bit tick（`0x2000030C` 为 seqlock 字节），
   读/加分别由 `0x080334D8` / `0x080334EC` 完成（供将来做双击/计时功能使用）。
3. **KEY_ON=0x20001080 审计**：全镜像无字面量引用；短时整机空闲追踪未见写入；
   用户已确认"密钥模式并未真正退出"，此前的退出怀疑撤销，故不再迁移到设置 flash。
4. 旧 bin（keyprobe/keybtn2/keybtn3/keybtn4/nobc_a/nobc_b/K1）均为旧"全双发"核心注入，
   **只有 keybtn5 含白名单修复**；`nobc_a` 仅建议作为应急恢复镜像（⚡ 原厂、无广播、无白名单）。
