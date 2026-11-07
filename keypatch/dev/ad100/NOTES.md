# AD100 Pro V1.2 私有密钥补丁（v4「C.Fn 开关」版）— 推导 / 实现 / 验证

> 设备：Godox AD100 Pro，固件 `devices/ad100pro/firmware/AD100pro_V1.2.bin`
> （50248 字节，sha256 `3a3d3b917615bb20339f6c355ebe66d8d94e8199509c1e820007542c1ebf4e7c`，基址 0x08003000，文件偏移 = 地址 − 0x08003000）。
> 交付模块：`keypatch/devices/ad100.py`；本版成品：`keypatch/out/AD100pro_V1.2_keyD798.bin`
> （50248 字节，sha256 `0b254518d49d511191d7f668c5ac67c324664b399380a1d9b5d2beee4f1af5ce`）。
> 本目录脚本：`re_check.py`（静态复核）、`disasm_region.py`（前后反汇编）、`check_out.py`（输出核对）、
> `emu_regress.py`（整机回归）。日志见 §8。

## 1. 结论

把原厂「ID → CC2500 16 位硬件同步字」函数 **0x0800B758 整段原位改写**（56 字节
= 44 字节代码 + 3 个 32 位字面量），改为读取灯端 C.Fn 新增的「密钥开关」标志字节：

```
flag = *(uint8_t *)0x20000486;      // C.Fn 预留项 6：0=普通、1=密钥
if (flag) sync = KEY;               // 密钥模式
else      sync = 50024/(id + 1);    // 原厂普通模式
WriteReg(4, sync >> 8);             // SYNC1
WriteReg(5, sync & 0xFF);           // SYNC0
return;                             // 只改同步字：无 SCAL、无任何延时
```

* 补丁后长度不变（50248），差异字节全部落在 `0x08758..0x0878B`（48 个变化字节，
  其余 8 字节恰与原厂相同），**不越过下一函数 0x0800B790**；紧邻的下一函数首 8 字节未动。
* Unicorn 调用级测试：flag=1 时 ID=0/5/99 均得 `WriteReg(4,KEY>>8)->WriteReg(5,KEY&0xFF)`
  （KEY=0xD798 → `D7 98`）；flag=0 时 ID=0/1/5/37/99 回到原厂公式（`C368/61B4/2091/0524/01F4`）；
  原厂镜像自证仍 PASS（含 SCAL+1ms 原序列）。
* 整机模拟器：复位→200 万条指令与原厂**逐条 MMIO trace 一致**（91621 条、0 fault、LCD 一致）；
  真 `RF_Init(0x08004AF0)` 经虚拟 CC2500 实测：补丁 flag=1 → SYNC=0xD798，flag=0 → 与原厂完全一致；
  运行期直接改 flag 再调用 0x0800B758 也立即生效。
* 与 ui-lights 的 UI 补丁（0x08003F0C 菜单函数 + 少量导航立即数）地址不重叠（见 §7）。

## 2. 关键前提：标志字节 RAM 0x20000486（已复核）

ui-lights 确认「C.Fn 预留项 6」= RAM **0x20000486**（即设置区 RAM_20000480[6]，
0=普通、1=密钥；随 flash 参数记录 `0x0801F000 + slot*100`、记录内偏移 30 自动加载/保存）。
本目录 `re_check.py` §8 的独立静态复核：

* 校验表 **0x0800DA2E** 前 16 字节 = `01 02 03 96 01 63 01 01 01 01 07 06 05 04 04 03`，
  其中 **[6] = 1** → 该设置项允许取值 0..1（即 0/1），符合「开关」语义。
* 全镜像共 **8 处** 访问 `RAM_20000480[6]`（地址 0x20000486）：
  * 读（均为 `cmp rX,#1` 判断）：0x08006130、0x0800678C、0x0800AF98、0x0800B0A0（两处函数）；
  * 写：0x08004BB0、0x08005502、0x0800B8D8（有写 0 的路径，见 §8 风险 3）。
* 原厂同步字函数 0x0800B758 区原 56 字节里不含该地址引用（本补丁新增的只有字面量）。

## 3. 补丁点推导（独立复核，不盲信既有结论）

脚本：`keypatch/dev/ad100/re_check.py`（capstone 5.0.7，`CS_MODE_THUMB|CS_MODE_MCLASS`，
按 `devices/ad100pro/out/functions.json` 的函数边界反汇编全镜像）。日志 `re_check.log`。

### 3.1 函数边界与原始代码

`0x0800B758` 被 4 处调用：`0x08004880`（改 ID 后重设同步字）、`0x08004AF0`（RF_Init）、
`0x08008A5C`、`0x08008D00`（按键/菜单路径）。capstone 反汇编（20 条指令，end=0x0800B78C）：

```
0x0800B758  0c 4a      ldr  r2, [pc, #0x30]      ; 字面量在 0x0800B78C = 0x0000C368 = 50024
0x0800B75A  10 b5      push {r4, lr}
0x0800B75C  41 1c      adds r1, r0, #1           ; r1 = ID+1
0x0800B75E  10 46      mov  r0, r2               ; r0 = 50024
0x0800B760  f7 f7 c6 fc bl  #0x80030f0           ; __aeabi_uidiv
0x0800B764  04 46      mov  r4, r0
0x0800B766  01 0a      lsrs r1, r0, #8           ; SYNC1 = sync>>8
0x0800B768  04 20      movs r0, #4               ; reg 4 = SYNC1
0x0800B76A  01 f0 e2 fa bl  #0x800cd32           ; CC2500_WriteReg
0x0800B76E  e1 b2      uxtb r1, r4               ; SYNC0 = sync&0xFF
0x0800B770  05 20      movs r0, #5               ; reg 5 = SYNC0
0x0800B772  01 f0 de fa bl  #0x800cd32
0x0800B776  14 20      movs r0, #0x14            ; delay_us(20)
0x0800B778  01 f0 9e f9 bl  #0x800cab8
0x0800B77C  33 20      movs r0, #0x33            ; SCAL
0x0800B77E  01 f0 39 fb bl  #0x0800cdf4
0x0800B782  7d 20      movs r0, #0x7d
0x0800B784  c0 00      lsls r0, r0, #3            ; 1000
0x0800B786  01 f0 97 f9 bl  #0x0800cab8           ; delay_ms(1)
0x0800B78A  10 bd      pop  {r4, pc}
0x0800B78C  68 c3 00 00   .word 0x0000C368
```

* 函数代码 0x0800B758..0x0800B78B（52 字节）+ 0x0800B78C..0x0800B78F 字面量（4 字节）
  = **56 字节**；`functions.json`/反汇编确认下一函数从 **0x0800B790** 开始
  （`06 48 41 8e 00 29 08 d0 ...`）。整段替换不需要代码注入、不改长度。

### 3.2 字面量唯一性与「运行期不会二次覆盖 SYNC」

* 4 字节字面量 `68 c3 00 00`（0x0000C368）**全镜像只出现 1 次**，位于 0x0800B78C，
  唯一引用是 `0x0800B758` 的 `ldr r2,[pc,#0x30]`。
* 全镜像所有 `bl 0x0800CD32 (CC2500_WriteReg)` 调用点中，只有 **4 处** 写 reg4/reg5：
  1. `0x0800CB16/0x0800CB1E`（`CC2500_Init` 的静态初值 SYNC1=0xC3、SYNC0=0x68）；
  2. `0x0800B76A/0x0800B772`（本函数，即补丁目标）。
* `CC2500_Init(0x0800CAE8)` 只被 `RF_Init(0x08004AF0)` 调用；`RF_Init` 内部先
  `CC2500_Init`、后 `0x0800B758(id)`（0x08004B56 处调用）。所以开机时初始化表写的
  0xC3/0x68 会立刻被本函数覆盖；运行期没有其它路径重置同步字。
* 三个「改 ID」调用点也都走本函数，因此改一次补丁即可覆盖所有同步字设置路径。

### 3.3 相关地址

| 符号 | 地址 | 用途 |
|---|---|---|
| `rf_set_id(id)` | 0x0800B758 | 本补丁整段改写（56 字节） |
| 下一函数 | 0x0800B790 | 补丁边界，不得越界 |
| C.Fn 密钥开关标志 | 0x20000486 | RAM_20000480[6]，0=普通/1=密钥 |
| `__aeabi_uidiv` | 0x080030F0 | 软件除法（纯寄存器代码，Unicorn 可直接执行） |
| `CC2500_WriteReg(reg,val)` | 0x0800CD32 | r0=reg, r1=val |
| `CC2500_WriteCmd(cmd)` | 0x0800CDF4 | v4 不再调用（原用于 SCAL=0x33） |
| `delay_us(n)` | 0x0800CAB8 | v4 不再调用 |
| 校验表 | 0x0800DA2E | [6]=1 → 该 C.Fn 项允许 0/1 |

## 4. v4 函数布局（原位 56 字节）

```
+00  10 b5        push {r4, lr}
+02  0b 4b        ldr  r3, =FLAG(0x20000486)   ; 字面量 +0x30
+04  0b 4a        ldr  r2, =50024              ; 字面量 +0x34（原厂字面量地址不变）
+06  1b 78        ldrb r3, [r3]                ; flag
+08  41 1c        adds r1, r0, #1              ; 普通模式除数 = ID+1
+0A  00 2b        cmp  r3, #0
+0C  01 d0        beq  +0x12 (go)              ; flag==0 -> 原厂公式
+0E  07 4a        ldr  r2, =KEY                 ; 字面量 +0x2C，密钥模式被除数
+10  01 21        movs r1, #1                  ; 密钥模式除数 = 1
+12  10 46        mov  r0, r2                  ; go: 被除数
+14  f7 f7 c0 fc  bl   0x080030F0              ; __aeabi_uidiv -> r0=sync
+18  04 46        mov  r4, r0
+1A  01 0a        lsrs r1, r0, #8              ; SYNC1
+1C  04 20        movs r0, #4
+1E  01 f0 dc fa  bl   0x0800CD32              ; WriteReg(4, sync>>8)
+22  e1 b2        uxtb r1, r4
+24  05 20        movs r0, #5
+26  01 f0 d8 fa  bl   0x0800CD32              ; WriteReg(5, sync&0xFF)
+2A  10 bd        pop  {r4, pc}
+2C  98 d7 00 00  .word KEY
+30  86 04 00 20  .word 0x20000486 (FLAG)
+34  68 c3 00 00  .word 50024
```

编码由模块内极简 Thumb-1 汇编器 `_Asm` 生成（参考 lead 已验证原型
`keypatch/dev/lead/toggle_proto.py` 的编码器），LDR (literal) 字面量按
`Align(addr+4,4)+imm8*4` 现场计算，BL 偏移现场编码；生成后断言长度=44+12=56、
结束=0x0800B790、三个字面量落位正确。分支只用一条 `beq`（密钥分支顺序接在
普通分支之后），因此 44 字节刚好放下。

### 4.1 为什么可以去掉 SCAL(0x33)、delay(1ms) 和 20us 建立延时

* SCAL 是给 **频率合成器重新校准**用的；本补丁只改 SYNC1/SYNC0（纯数字包同步配置），
  不改 FREQ2/1/0、不改信道，故不需要 SCAL。`CC2500_Init` 里已有一次 SCAL。
* v4 连 `delay_us(20)` 也去掉：同步字寄存器是数字配置，写入后立即生效；原 del20
  主要是配套 SCAL 的等待。这样省下 4 字节，正好容纳「读 flag + 条件选择被除数」
  的额外代码与第 3 个字面量（44B 代码 + 12B 字面量 = 56B）。
* 风险：若某批次 CC2500 兼容芯片要求写 SYNC 后延时/SCAL（数据手册无此要求），
  极端情况下可能出现首包丢失——真机验证时优先检查「上电后第一次收包」。

## 5. 补丁前后反汇编对比

完整日志 `disasm_patch_D798.log`。关键差异：

* 补丁前：`ldr r2,=50024; adds r1,r0,#1; udiv; ...; delay(20us); SCAL(0x33); delay(1ms)`。
* 补丁后：`ldr r3,=0x20000486; ldrb r3,[r3]; ldr r2,=50024; adds r1,r0,#1; cmp r3,#0;
  beq go; ldr r2,=KEY; movs r1,#1; go: udiv; ...`，**无 WriteCmd、无任何 delay**。
* 字面量：`+0x2C = KEY`（小端）、`+0x30 = 0x20000486`、`+0x34 = 50024`（原厂字面量地址不变）。

## 6. 验证（可复现命令与真实输出）

所有命令在 `` 下执行，Python 均为 `tools\python\python.exe`
（capstone 5.0.7 / unicorn 2.1.4）。**最终成品 KEY 用 0xD798**。

### 6.1 打补丁 + 离线验证（验收命令）

```powershell
tools\python\python.exe keypatch\keypatch.py patch --device ad100 --key 0xD798 --verify
```

真实输出（完整日志 `patch_D798.log`，exit=0，全 PASS）：

```
== Godox AD100 Pro (ad100)
   原厂固件 : devices\ad100pro\firmware\AD100pro_V1.2.bin  (50248 字节, sha256 3a3d3b917615bb20…)
   私有密钥 : 0xD798  ->  SYNC1=0xD7 SYNC0=0x98
   补丁点   :
  [1] off+0x08758  0x0800B758  <原厂 56 字节> -> <v4 函数 56 字节>
      整段改写同步字函数 0x0800B758..0x0800B78F（56 字节）：C.Fn 密钥开关
      RAM[0x20000486]==1 -> SYNC=KEY 0xD798；==0 -> 50024/(ID+1)；只写 SYNC1/0（无 SCAL、无延时）
   输出     : keypatch\out\AD100pro_V1.2_keyD798.bin  (50248 字节, sha256 0b254518d49d5111…)
   [PASS] 固件长度 50248 字节（应为 50248，原位补丁）
   [PASS] 0x0800B758..0x0800B78F 补丁区 56 字节与期望函数体完全一致
   [PASS] 函数体结束于 0x0800B790 == 下一函数起点 0x0800B790（不越界）
   [PASS] 字面量: KEY=0xD798 @0x0800B784, FLAG=0x20000486 @0x0800B788, 50024 @0x0800B78C
   [PASS] 与原厂差异 48 字节，offset=[0x08758, 0x08759…0x0878B]（应恰为同集合）
   [PASS] 补丁区之外无任何字节变化（差异 0 处）
   [PASS] Unicorn 2.1.4 调用级测试：从 0x0800B759 执行，RAM[0x20000486]=flag，桩替换 WriteReg/WriteCmd/delay_us
   [PASS] ID=0  flag=1 密钥模式(KEY): sync=0xD798 -> SYNC1=0xD7 SYNC0=0x98（仅 2 次写寄存器）| WriteReg(4,0xD7) -> WriteReg(5,0x98)
   [PASS] ID=5  flag=1 密钥模式(KEY): sync=0xD798 -> SYNC1=0xD7 SYNC0=0x98
   [PASS] ID=99 flag=1 密钥模式(KEY): sync=0xD798 -> SYNC1=0xD7 SYNC0=0x98
   [PASS] ID=0  flag=0 原厂公式: sync=0xC368 -> SYNC1=0xC3 SYNC0=0x68
   [PASS] ID=1  flag=0 原厂公式: sync=0x61B4 -> SYNC1=0x61 SYNC0=0xB4
   [PASS] ID=5  flag=0 原厂公式: sync=0x2091 -> SYNC1=0x20 SYNC0=0x91
   [PASS] ID=37 flag=0 原厂公式: sync=0x0524 -> SYNC1=0x05 SYNC0=0x24
   [PASS] ID=99 flag=0 原厂公式: sync=0x01F4 -> SYNC1=0x01 SYNC0=0xF4
   [PASS] 同一 ID=5 下 flag=1/0 同步字分别为 0xD798 / 0x2091（应 0xD798 / 0x2091）
   [PASS] ID=0/1/5/37/99 原厂(自证): … -> delay(20) -> WriteCmd(0x33) -> delay(1000)
```

Unicorn 调用级测试实现（`devices/ad100.py` 的 `_uc_run_sync_fn`）：
镜像加载到 0x08003000，RAM 0x20000000+ 映射并放栈；先写 `RAM[0x20000486]=flag`；
从 `0x0800B758|1` 执行、r0=ID；把 `0x0800CD32/0x0800CDF4/0x0800CAB8` 入口改成
`bx lr` 返回桩，代码钩子记录 `(kind, r0, r1)`；函数 `pop {pc}` 落到 RAM 里的 `b .`
哨兵并 `emu_stop()`。同一 harness 把补丁区还原成原厂 56 字节再跑一遍（原厂自证）。
注：unicorn 2.1.4 在 MCLASS 下 `emu_start(count=0)` 实测会卡住，故显式给指令上限。

### 6.2 输出 bin 独立核对

```powershell
tools\python\python.exe keypatch\dev\ad100\check_out.py
```

真实输出（`check_out_D798.log`，exit=0）：

```
原厂 : devices\ad100pro\firmware\AD100pro_V1.2.bin  50248 字节  sha256 3a3d3b917615bb20339f6c355ebe66d8d94e8199509c1e820007542c1ebf4e7c
补丁 : keypatch\out\AD100pro_V1.2_keyD798.bin  50248 字节  sha256 0b254518d49d511191d7f668c5ac67c324664b399380a1d9b5d2beee4f1af5ce
[PASS] 长度一致: 50248 == 50248
差异字节: 48 个，范围 0x08758..0x0878B
[PASS] 全部差异落在补丁区 0x08758..0x0878F（56 字节整函数改写）
[PASS] 补丁区与期望函数体一致（KEY=0xD798）
[PASS] 下一函数 0x0800B790 起点字节未被改动: 06 48 41 8e 00 29 08 d0
[PASS] 原厂固件 sha256 与 DEVICE 记录一致（未被改动）
结论: PASS
```

48 个差异字节之外，补丁区里 `0x08772`（`01 0a`）、`0x0877D-0x0877F`（`01 f0`）、
`0x0878C-0x0878F`（`50024` 字面量）等 8 字节恰与原厂相同，所以差异集合不是整段 56 字节。

### 6.3 静态复核

```powershell
tools\python\python.exe keypatch\dev\ad100\re_check.py keypatch\dev\ad100\re_check.log
```

见 §2/§3；`re_check.log` 含：函数反汇编、56 字节原始内容、0xC368 唯一性、
0x0800B758 的 4 个调用点、全镜像 57 个 WriteReg 调用点的寄存器号（仅 4 处写 SYNC）、
CC2500_Init 静态 SYNC=C3/68、向量表无异常入口、**标志字节校验表[6]=1 与 8 处访问点**、
核心补丁区与 UI 补丁目标 0x08003F0C 不重叠。

### 6.4 整机启动 + 射频回归（devices/ad100pro/emu）

```powershell
tools\python\python.exe keypatch\dev\ad100\emu_regress.py keypatch\dev\ad100\emu_regress.log
```

真实输出（`emu_regress.log`，exit=0）：

```
== 1. 复位 -> 2000000 条指令：原厂 vs 补丁 逐项比较 ==
  [PASS] 停止 PC 一致: 0x0800CA3E == 0x0800CA3E（状态 ok/ok）
  [PASS] 运行状态一致且正常: ok/ok
  [PASS] 指令计数一致: 2000015 == 2000015
  [PASS] 无故障: 原厂 0 / 补丁 0
  [PASS] 16 个寄存器完全一致
  [PASS] LCD 帧数一致: 3 == 3
  [PASS] LCD 帧缓冲内容一致（nonzero 原厂 161 / 补丁 161）
  [PASS] MMIO 外设访问条数一致: 91621 == 91621
  [PASS] MMIO 外设访问逐条一致

== 2a. 整机 RF bring-up（跳过启动动画 -> 真 RF_Init 0x08004AF0）: 原厂 ==
  ID=0/1/5/37/99 flag=0 -> 0xC368 / 0x61B4 / 0x2091 / 0x0524 / 0x01F4（=50024/(ID+1)）
  ID=0/5/99     flag=1 -> 同原厂值（原厂不读 flag）

== 2b. 整机 RF bring-up: 补丁 ==
  ID=0/1/5/37/99 flag=0 -> 0xC368 / 0x61B4 / 0x2091 / 0x0524 / 0x01F4（与原厂一致）
  ID=0/5/99     flag=1 -> SYNC1=0xD7 SYNC0=0x98 sync=0xD798（密钥模式）
  CC2500 state=RX、freq2=0x5E、pktlen=4、patable 一致、faults=0
  [PASS] 8 组 (ID,flag) 原厂/补丁对比全部符合预期

== 3. 运行期切换 flag（模拟器内直接调用 0x0800B758） ==
  [PASS] ID=5  flag=1 -> sync=0xD798；flag=0 -> sync=0x2091
  [PASS] ID=99 flag=1 -> sync=0xD798；flag=0 -> sync=0x01F4
  结论: 补丁函数每次调用都读 RAM[0x20000486]

== 4. LCD 画面 PNG（CLI 2,000,000 条指令）一致性 ==
  [PASS] 原厂/补丁画面 sha256 相同 = 8b7edf032d29957f911381d339491e229e9182fc4319f90c32658e93cfbc57e4
== 结论: PASS ==
```

补充：200 万条指令的 CLI 直跑（`emu_orig_run.log` / `emu_patch_run.log`）结果一致：

```
run status: ok, executed=2000015, total=2000015, pc=0x0800CA3E, ...  no faults
final PC : 0x0800CA3E  SP=0x20000C98  LR=0x0800C25D
LCD      : 128x64 1bpp, FB@0x200004CC nonzero=161, 3172 bytes streamed, frames=3
screen   : emu_orig_screen.png / emu_patch_screen.png（1484 字节，sha256 一致，lit_pixels=698）
```

说明：AD100 完整启动路径在模拟器里 >50M 条指令仍停留在启动动画/显示刷新循环
（见 `devices/ad100pro/emu/README.md` §9），因此 200 万条指令处只走到 RF 初始化前的
20ms 延时；**射频初始化通过 `Emulator.skip_to_main_loop → rf_init()` 直接驱动真实
`RF_Init(0x08004AF0)`**（内部 CC2500 软件 SPI 位翻转 + 虚拟 CC2500 模型），
这是补丁函数在整机环境里真实执行的部分。§3 的「运行期切换」进一步验证了
每次调用都会重新读取标志字节。

CLI 复现命令（输出写本目录，不覆盖项目既有 out/ 产物）：

```powershell
tools\python\python.exe devices\ad100pro\emu\main.py --firmware devices\ad100pro\firmware\AD100pro_V1.2.bin `
  --run 2000000 --regs --dump keypatch\dev\ad100\emu_orig_trace.log --screen keypatch\dev\ad100\emu_orig_screen.png
tools\python\python.exe devices\ad100pro\emu\main.py --firmware keypatch\out\AD100pro_V1.2_keyD798.bin `
  --run 2000000 --regs --dump keypatch\dev\ad100\emu_patch_trace.log --screen keypatch\dev\ad100\emu_patch_screen.png
```

## 7. 与 ui-lights 的 UI 补丁（方案 A）的关系

* 本补丁只改 **0x0800B758..0x0800B78F**（56 字节，同步字函数）。
* ui-lights 的 UI 补丁计划插到 **0x08003F0C 菜单渲染函数**并改少量导航立即数；
  两者地址相距很远（0x0800B758 vs 0x08003F0C），**不重叠**，可以各自独立打补丁后合并。
* 合并方式：两个模块的 `apply()` 都只做原位字节替换、互不覆盖对方的偏移，
  合并时按顺序 apply 即可；`verify()` 各自只检查自己的补丁区。合并后建议再跑一次
  整体 Unicorn/模拟器回归。
* **重要（即时生效问题）**：固件只在以下路径调用 0x0800B758 重设同步字：
  `RF_Init(0x08004AF0)`（开机/BF 初始化链）、`0x08004880`（**仅在改 ID 项时**，
  见下）、`0x08008A5C`、`0x08008D00`。设置变更处理函数 0x08004880 里只有
  `cmp r0,#5`（ID 项）才重新调用 0x0800B758（0x080048F8–0x080048FE），
  **改 C.Fn 第 6 项本人不会触发同步字重设**。若要求「菜单里一开/一关立即生效」，
  ui-lights 的 UI 补丁需要在写标志后调用一次 `0x0800B758(RAM[0x20000485])`
  （或在改 ID/重新初始化时自然生效）。否则需要重新上电/改一次 ID 才生效——请 ui-lights 确认其实现路径。本模块对此不做假设，只保证函数每次被调用时都读标志。

## 8. 风险与未决

1. **未在真机验证**：本补丁只做了静态复核 + Unicorn + 整机模拟器验证。真机需用 G3
   刷入后实测（灯 ID 任意、C.Fn 密钥开关 1/0，配合已打补丁的引闪器）。
2. **去掉 SCAL / 全部延时**：见 §4.1。CC2500 手册中 SYNC1/SYNC0 只是数字包同步配置，
   不参与频率校准；`CC2500_Init` 已做 SCAL。真机若出现「上电首包丢失」，优先怀疑此点。
3. **标志字节是既有 C.Fn 配置项，已有代码写它**：全镜像有 3 处 `strb [RAM_20000480,#6]`
   （0x08004BB0、0x08005502、0x0800B8D8，其中后者在 UI 任务 0x08008D00 的调用链里写 0）
   以及 5 处 `ldrb`+`cmp #1`。也就是说该字节原本就参与若干 UI/运行逻辑；ui-lights 的
   UI 方案需要确认这些既有写 0 路径不会在运行中把密钥开关清掉，以及开启该 C.Fn 项后
   既有读点行为是否可接受。
4. **即时生效**：见 §7 末尾；取决于 UI 补丁是否在改动标志后重新调用 0x0800B758。
5. **16 位同步字不是密码学认证**：SDR 抓包理论上可克隆 KEY；只挡正常原厂设备。
6. **KEY 选择**：`common.validate_key()` 拒绝 100 个公开值、0x0000/0xFFFF/0xAAAA/0x5555/0x8000。
   换 KEY 需重新给所有设备打补丁并刷机；灯/引闪器必须同一把 KEY。
7. **版本历史**：v1（固定 KEY，改 2 处 4 字节）、v2（ID==0 当开关）、v4（本版，C.Fn 标志）
   都基于同一 56 字节区域；本版取代前两版，`keypatch/out/` 里的最终 AD100 产物是
   `AD100pro_V1.2_keyD798.bin`。

## 9. 产物与日志清单

| 文件 | 说明 |
|---|---|
| `keypatch/devices/ad100.py` | v4 补丁模块（DEVICE/apply/verify；Thumb 汇编器 + Unicorn harness） |
| `keypatch/out/AD100pro_V1.2_keyD798.bin` | 最终固件（50248 字节，sha256 `0b254518d49d5111…`） |
| `keypatch/dev/ad100/re_check.py` / `re_check.log` | 静态复核（反汇编/字面量/SYNC 写点/调用点/标志字节） |
| `keypatch/dev/ad100/disasm_region.py` / `disasm_patch_D798.log` | 补丁前后 56 字节反汇编与差异偏移 |
| `keypatch/dev/ad100/check_out.py` / `check_out_D798.log` | 输出 bin 长度/差异/原厂未改动核对 |
| `keypatch/dev/ad100/emu_regress.py` / `emu_regress.log` | 整机启动 + RF 初始化 + 运行期切换回归 |
| `keypatch/dev/ad100/patch_D798.log` | `keypatch.py patch --verify` 完整输出 |
| `keypatch/dev/ad100/emu_orig_run.log` / `emu_patch_run.log` | 两镜像 200 万条指令 CLI 输出 |
| `keypatch/dev/ad100/emu_orig_screen.png` / `emu_patch_screen.png` | 200 万条指令 LCD 画面（1484 字节，sha256 一致） |
| `keypatch/dev/ad100/make_id0.py` / `build_id0.log` | 生成/验证 id0 临时可切换版（§11.1） |
| `keypatch/dev/ad100/emu_regress_variant.py` / `emu_regress_id0.log` | 任意变体镜像的整机回归（§11.1） |
| `keypatch/dev/ad100/build_ui_merged.py` / `build_ui_merged.log` | 合并/验证 UI 版（§11.2） |
| `keypatch/dev/ad100/emu_patch_id0_run.log` / `emu_patch_id0_screen.png` | id0 版 200 万条指令 CLI 输出/画面 |

> 200 万条指令的完整 MMIO trace（各 91621 条、约 6.7MB）未入库，可用 §6.4 的 CLI
> 命令加 `--dump` 重新生成；逐条一致性已由 `emu_regress.py` 在内存中比较
> （91621 == 91621 且首个差异为 None）。

## 10. 一键复现

```powershell
$py = "tools\python\python.exe"
& $py keypatch\keypatch.py patch --device ad100 --key 0xD798 --verify
& $py keypatch\dev\ad100\re_check.py keypatch\dev\ad100\re_check.log
& $py keypatch\dev\ad100\disasm_region.py keypatch\dev\ad100\disasm_patch_D798.log
& $py keypatch\dev\ad100\check_out.py
& $py keypatch\dev\ad100\emu_regress.py keypatch\dev\ad100\emu_regress.log
```

## 11. 三个发布变体（v4 核心 / id0 临时可切换 / UI 合并）

AD100 现在有 3 份同长度（50248 B）的原位补丁 bin，**都来自同一 56 字节函数区域**，
语义不同、互不覆盖：

| 成品文件（`keypatch/out/`） | 语义（密钥模式怎么开） | sha256 |
|---|---|---|
| `AD100pro_V1.2_keyD798.bin` | **v4 核心**：C.Fn 标志 RAM[0x20000486]==1。当前无 UI 项，flag 恒 0 → 只能普通模式；等 UI 合并版 | `0b254518d49d511191d7f668c5ac67c324664b399380a1d9b5d2beee4f1af5ce` |
| `AD100pro_V1.2_keyD798_id0.bin` | **临时可切换版（零 UI）**：**ID 菜单设 0 = 密钥模式**；ID=1..99 = 原厂普通模式。**这是当前交付给用户用的版本** | `9d4754e7a5d6ac82d585625a5f26316cb2d0b7a8e6a011dbe765ca109845d546` |
| `AD100pro_V1.2_keyD798_ui.bin` | **最终 UI 版**：v4 核心 + ui-lights 的 C.Fn 第 7 项「KEY」菜单（ON/OFF，持久化）；密钥模式由菜单开关控制 | `b446084f9b1d1d87b4e9127b8ecda6537a08f11a42c289d91804831457c5ad31` |

三个 bin 均为 50248 字节、可走 G3 官方升级（替换 app.bin）。

### 11.1 id0 临时可切换版（`make_id0.py`）

语义：`ID == 0 -> SYNC = KEY`；`ID != 0 -> SYNC = 50024/(ID+1)`（零 UI 改动）。

函数布局（56 字节，42B 代码 + 对齐/填充 + 字面量 KEY@0x0800B784、50024@0x0800B78C）：

```
0x0800B758  0a 4a      ldr  r2, [pc, #0x28]   ; =KEY
0x0800B75A  10 b5      push {r4, lr}
0x0800B75C  00 28      cmp  r0, #0            ; ID == 0 ?
0x0800B75E  02 d0      beq  +0x0E (keymode)
0x0800B760  0a 4a      ldr  r2, [pc, #0x28]   ; =50024
0x0800B762  41 1c      adds r1, r0, #1        ; r1 = ID+1
0x0800B764  00 e0      b    +0x10 (go)
0x0800B766  01 21      movs r1, #1           ; keymode: 除数=1
0x0800B768  10 46      mov  r0, r2           ; go: 被除数
0x0800B76A  f7 f7 c1 fc bl  0x080030F0         ; __aeabi_uidiv
0x0800B76E  04 46      mov  r4, r0
0x0800B770  01 0a      lsrs r1, r0, #8       ; SYNC1
0x0800B772  04 20      movs r0, #4
0x0800B774  01 f0 dd fa bl  0x0800CD32         ; WriteReg(4, sync>>8)
0x0800B778  e1 b2      uxtb r1, r4
0x0800B77A  05 20      movs r0, #5
0x0800B77C  01 f0 d9 fa bl  0x0800CD32         ; WriteReg(5, sync&0xFF)
0x0800B780  10 bd      pop  {r4, pc}
0x0800B782  00 bf      nop
0x0800B784  98 d7 00 00 .word KEY
0x0800B788  00 bf 00 bf nop;nop
0x0800B78C  68 c3 00 00 .word 50024
```

生成与验证（`build_id0.log`，exit=0 全 PASS）：

```powershell
tools\python\python.exe keypatch\dev\ad100\make_id0.py 0xD798 keypatch\dev\ad100\build_id0.log
```

* 输出 `AD100pro_V1.2_keyD798_id0.bin`：50248 B；与原厂差异 48 字节且全在补丁区（区外 0 处）；
* Unicorn（ID=0/1/5/37/99）：ID=0 → `WriteReg(4,0xD7)->WriteReg(5,0x98)`；
  ID=1/5/37/99 → `61B4/2091/0524/01F4`；原厂镜像自证 5 个 ID 全 PASS（含 SCAL 序列）。

整机回归（`emu_regress_variant.py --mode id0`，`emu_regress_id0.log`，exit=0 全 PASS）：

```powershell
tools\python\python.exe keypatch\dev\ad100\emu_regress_variant.py `
  --image keypatch\out\AD100pro_V1.2_keyD798_id0.bin --mode id0 `
  --screen keypatch\dev\ad100\emu_patch_id0_screen.png --log keypatch\dev\ad100\emu_regress_id0.log
```

* 复位→200 万条指令与原厂逐项一致（PC 0x0800CA3E、16 寄存器、91621 条 MMIO trace、0 fault、LCD/PNG 一致）；
* 真 RF_Init + 虚拟 CC2500：ID=0 → 0xD798；ID=1/5/37/99 与原厂一致；state=RX、freq2=0x5E、pktlen=4、0 fault；
* 运行期调用 0x0800B758：ID=0 → 0xD798，ID=5/99 → 原厂值。

**使用说明（给用户）**：刷入后，在灯的 ID 菜单把 ID 设为 **0（OFF）** 即进入密钥模式；
ID 设为 1–99 为普通模式（与原厂灯一致）。与 X3-C（ID=99 密钥模式）、AD200（ID=OFF）配套。
注意：ID=0 在补丁灯上表示密钥模式，不再是"ID=0 的普通模式"。

### 11.2 UI 合并版（`build_ui_merged.py`）

把 v4 核心（本模块 `apply()`，读 RAM[0x20000486]）与 ui-lights 的
`keypatch/dev/lights_ui/ui_patch_ad100.py`（C.Fn 第 7 项「KEY」，ON/OFF 绘制 +
导航上限 6 + 字形函数收尾，写入标志字节 0x20000486）合并：

```
merged = ad100.apply(ui_patch_ad100.apply_ui(原厂)[0], KEY)
       == ui_patch_ad100.apply_ui(ad100.apply(原厂, KEY)[0])   # 两种顺序逐字节相同
```

* 两个补丁区互不重叠：核心 0x0800B758..0x0800B78F（48 个变化字节）+ UI
  0x08003F10(120B)/0x08003FA2/0x0800490C/0x08008A8A/0x08004F88（112 个变化字节），
  合并差异 = 160 字节 = 48 ∪ 112，无交集、无越界。
* 核心 Unicorn 在合并镜像上仍 PASS（flag=1 → 0xD798；flag=0 原厂公式；原厂自证）。
* 整机 UI 功能（模拟器，`build_ui_merged.log`）全 PASS：
  C.Fn sel=3 列表区与原厂一致（只改页头）；sel=6 能渲染 KEY 项；KEY OFF/ON 画面不同；
  `+/-` 选择项 wrap `[1..6,0,1]` / `[6,5,4,3,2,1,0,6,5]`；选中 KEY 时 +/- 切换
  flag `0→1→0→1`；**菜单开关 flag 后调用 0x0800B758 同步字立即变化（UI→核心端到端）**；
  设置记录偏移 30 / 写 flash / 加载恢复链路全 PASS。
* 整机启动回归：复位→200 万条指令与原厂逐项一致；RF bring-up flag=1 → 0xD798；
  运行期调用 flag 切换 PASS。

复现：

```powershell
tools\python\python.exe keypatch\dev\ad100\build_ui_merged.py 0xD798 keypatch\dev\ad100\build_ui_merged.log
```

**使用说明（给用户）**：刷入后 MENU → C.Fn → 第 7 项「KEY」→ ON = 密钥模式、OFF = 普通模式；
设置掉电保存（记录偏移 30）。ID 菜单恢复原厂含义（0..99 仍是 ID；密钥模式不再依赖 ID）。

### 11.3 已知问题 / 与发布文档的衔接

* `keypatch/selftest.py --key 0xD798` 目前对 ad100/ad200 报
  `ID=0 sync=0xC368 期望 0xD798` FAIL：该自检仍按 v2「ID==0 → 密钥」语义检查，
  而 v4 模块按 C.Fn 标志（flag=0 时 ID=0 就是原厂 0xC368）。需要 Lead 把自检改成
  按各设备 v4 `verify()` 或读取对应 flag（不在本目录写权限内，未修改）。
* `keypatch/out/AD100pro_V1.2_keyD798_container.bin` 是早期（v4 之前的）容器，
  若最终发布要容器兜底，需用 v4/id0/ui 重新打包（打包脚本在 `keypatch/dev/lead/`）。
* UI 版与 id0 版互斥（刷哪个用哪个语义）；两者与 X3-C/AD200 的 KEY 相同（0xD798）。

