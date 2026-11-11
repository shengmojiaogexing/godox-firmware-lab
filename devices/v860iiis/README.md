# 神牛 V860III-S 固件逆向档案（V1.1 · 私有密钥补丁 · C.Fn `LIGHT` 开关版）

> 目标固件：`V860IIIS_V1.1.bin`（**79,300 字节**，sha256 `bdc8f354…`）
> 目标设备：神牛（Godox）V860III-S 索尼口 TTL 机顶闪光灯（Cortex-M0 / Thumb-1，2.4 GHz CC2500 兼容射频）
> 本目录内容：V860III-S 的静态逆向证据、「ID → CC2500 同步字」补丁点分析、
> **C.Fn 第 3 页第 1 行 `LIGHT` 项开关**语义、6 处补丁布局、73 项离线验证与实机待办。
>
> 本目录位于仓库 `devices/v860iiis/`；仓库根的便携 Python 为 `tools\python\python.exe`
> （含 capstone 5.0.7 / unicorn 2.1.4）。
>
> ⚠️ **状态：静态 + Unicorn 调用级/handler 级验证通过（`verify()` 73 项 PASS / 0 FAIL；
> 独立专项 `cfn_settings` 193 PASS / `cfn_menu` 158 PASS / `sync_audit` 132 PASS）；
> 无整机模拟器、无实机验证。**
> 只研究你本人合法拥有的设备；刷机前务必确认官方 G3 能识别设备、手上有同版本原厂固件可回滚。

---

## 0. 一句话结论

V860III-S 的「ID → 同步字」函数是 **`0x080115B8`**（56 字节，到下一函数 `0x080115F0` 为止），
它是**全镜像唯一**在运行期写 CC2500 `SYNC1/SYNC0`（寄存器 4/5）的地方
（另一处是 CC2500 初始化表写的缺省值 `0xC3/0x68`，会在初始化收尾时被它覆盖，见 §4）。

**密钥开关用 C.Fn 菜单**：复用 **C.Fn 第 3 页（page2）第 1 行 `LIGHT`（原厂造型灯模式，
`settings[8]` = RAM `0x20001060`，三档：`0=12sec`、`1=OFF`、`2=ON`）** ——

- **`LIGHT = ON`（值 2）** → 同步字 = 私有 KEY（密钥模式）；
- **`LIGHT = 12sec`（值 0，出厂默认）或 `OFF`（值 1）** → 同步字 = `50024/(ID+1)`
  （原厂普通模式，ID 菜单恢复原厂含义）。

补丁共 **6 处**：①同步字函数 56 B 整段改写；②③两个 C.Fn 改值 gate 的 `bne` 改 `nop`
（`0x08005A74` / `0x0800C5BE`，让改 LIGHT 立即生效，**不需要代码洞**）；
④⑤造型灯任务的两个 `settings[8]` 读者固定 0（`0x08003956` / `0x0800397E`）；
⑥RF 设置复位分支不再覆盖 `settings[8]`（`0x0800D0A2`）。

| 项目 | 结论 |
|---|---|
| 固件 | `devices/v860iiis/firmware/V860IIIS_V1.1.bin`，79,300 B（0x135C4），sha256 `bdc8f3547808d121f15a6c58b9363c5792c957c2dce2a16b203276ab078d0b20` |
| 镜像基址 | **`0x08003000`**（前 12 KB 是 bootloader，与 AD100 Pro / AD200 Pro / V1C / V860III-C 相同） |
| 镜像范围 | 文件偏移 0 → 虚拟地址 `0x08003000`；末尾虚拟地址 `0x080165C3`（末端 `0x080165C4`） |
| 向量表 | 初始 SP = **`0x20001548`**；Reset = **`0x080030D5`** → 处理函数 `0x080030D4` |
| 补丁形态 | ①同步字函数 `0x080115B8..0x080115EF` 56 B 整段改写（44 B 代码 + KEY@`+0x2C` + FLAG@`+0x30` + 50024@`+0x34`）；②～⑥ 见 §6，全部原位改写，**文件长度不变** |
| 开关语义 | **C.Fn item 8 `LIGHT` = ON（值 2）→ 密钥模式；12sec/OFF（0/1）→ 原厂公式** |
| 输出命名 | `V860IIIS_V1.1_key{key:04X}.bin`；G3 容器备用件 `V860IIIS_V1.1_key{key:04X}_container.bin` |
| 补丁引擎 | [`keypatch/devices/v860iiis.py`](../../keypatch/devices/v860iiis.py)（`apply()` / `verify()`） |
| 验证状态 | `verify()` **73 项 PASS / 0 FAIL**；Lead `call_harness.py` 交叉验证 **8/8 PASS**；专项报告 `cfn_settings` **193 PASS**、`cfn_menu` **158 PASS**、`sync_audit` **132 PASS / 0 FAIL**；整机模拟器**未建立**；实机**未验证** |
| 产物（key=`0xD798`） | `V860IIIS_V1.1_keyD798.bin` 79,300 B，sha256 `9ef11e8116e07dda30bd08ea407498f373ad3d953effb32258e3104c2ba5753d`；容器 79,361 B，sha256 `208f001d92d8ac543a8656905a7aac2da10a3454ccdfc12b03dcf4e56bc40b05` |

> 与同族设备的差别（一句话）：V1C / V860III-C 的「改值即时生效」用 **10 B `BL` 注入 hook + 全 0 死区**；
> 本机型没有经引用审计确认的大片死区，改用 **2 字节 `bne→nop`**（两处共 4 B）达到同样效果，
> 因此**补丁点数从 7 处降为 6 处、无需代码洞**（理由与证据见 §5.5 / §6.5）。

---

## 1. 固件与镜像事实

| 项 | 结论 | 证据 |
|---|---|---|
| 文件 | `firmware/V860IIIS_V1.1.bin`，79,300 B（0x135C4） | `Get-FileHash` / `keypatch.py info` |
| sha256 | `bdc8f3547808d121f15a6c58b9363c5792c957c2dce2a16b203276ab078d0b20` | 同上；`re/cfn_settings.py` §1 断言 PASS |
| 来源 | 用户提供的官方升级包固件（只读副本） | [`firmware/README.md`](firmware/README.md) |
| 镜像基址 | `0x08003000` | 向量表非零项全部落在 `[0x08003000, 0x080165C4)`；`re/log_recon1.txt` §1 |
| 前 12 KB | `0x08000000–0x08002FFF` 是厂商 bootloader，**不在此 `.bin` 内** | 与 AD100/AD200/V1C/V860III-C 同构 |
| 向量表初值 | 初始 SP = **`0x20001548`**；Reset = `0x080030D5` → 处理函数 `0x080030D4` | 镜像前 8 字节（小端）；`re/log_recon1.txt` §1 |
| app 镜像占用 | `0x08003000–0x080165C3`（79,300 B，约 77.4 KB） | 文件长度 + 基址 |
| Flash | **待确认**（app 已占约 77.4 KB，容量未实测；同族灯为 128 KB，但本机未验证） | — |
| RAM | **待确认**（本轮只静态确认初始 SP=`0x20001548` 落在 8 KB 级布局内 / 未实测） | 向量表初值 |
| 显示 | **168×96 单色**：帧缓冲 `0x200007AC`（12 条带 × 0xA8 = 2,016 B） | `re/log_cfn_menu.txt` §0/§8（真实渲染重建） |
| 版本字符串 | 顶栏渲染为 **`Ver1.1`**（8×16 字形自证） | `re/log_cfn_menu.txt` §1 |
| 射频 | **CC2500 兼容**（初始化表写寄存器 0x00–0x2E、软 SPI 写寄存器、`SCAL(0x33)`、同步字公式与 V1C 同源） | §4 |
| C.Fn 设置区 | 基址 **`0x20001058`**（item n = `settings[n]`）；ID = `settings[6]` = `0x2000105E`；**LIGHT = `settings[8]` = `0x20001060`** | §5；`re/log_cfn_settings.txt` §2 |
| 内核 | Cortex-M0 / Thumb-1（ARMv6-M）：干净函数体共 11,428 条指令 / 447 个函数，32 位非 `BL` 指令 **0** 条；全镜像 227 条 32 位非 `BL` 解码 100% 落在函数体外（数据/常量池）；M4 特征指令在真实函数体内 **0** 条 | `re/log_sync_audit.txt` §2；[`../../keypatch/dev/v860iiis/SYNC_AUDIT.md`](../../keypatch/dev/v860iiis/SYNC_AUDIT.md) §0 第 2 条 |

> 显示/Flash/RAM 的「待确认」不是保守措辞：本轮主要做了**与密钥开关有关**的静态分析
> （同步字函数、CC2500 写点、C.Fn 子系统/读者/持久化），没有做整机启动链、显示控制器、
> Flash 参数区的完整逆向。
>
> 另：镜像初始 SP 是 `0x20001548`；模块 Unicorn harness 自己把栈顶设为 `0x20001480`
> （`CFN_HARNESS_SP`）/ 单次调用栈顶 `0x20001400`（`UC_STACK`）并映射 `0x20000000` 起 16 KB RAM
> —— 三者不要混淆。
>
> 早期初勘脚本 `re/recon1.py` 输出过一条 `!! 出现 32 位指令 … 可能是 M3/M4` 的提示：那是脚本把
> **`bl`（Thumb 的 32 位 BL 编码）** 误计为「M0 不支持的 32 位指令」。`bl` 是 Cortex-M0 的合法指令，
> 本轮所有 Unicorn（`UC_MODE_MCLASS`）执行均正常，结论仍是 **Cortex-M0 / Thumb-1**。

---

## 2. 核心补丁点：同步字函数 `0x080115B8`

### 2.1 原厂反汇编（56 字节）

`0x080115B8` 起、到下一函数 `0x080115F0` 为止，共 **56 字节**：
其中代码 `0x080115B8..0x080115EB`（52 B）+ 字面量 `0x080115EC..0x080115EF`（4 B，`0x0000C368`）。

```asm
0x080115B8  0c 4a        ldr   r2, [pc, #0x30]   ; r2 = 字面量 @0x080115EC = 0x0000C368 = 50024
0x080115BA  10 b5        push  {r4, lr}
0x080115BC  41 1c        adds  r1, r0, #1          ; r1 = ID + 1
0x080115BE  10 46        mov   r0, r2             ; r0 = 50024
0x080115C0  f1 f7 96 fd  bl    0x080030F0         ; __aeabi_uidiv(50024, ID+1)
0x080115C4  04 46        mov   r4, r0             ; r4 = 16 位同步字
0x080115C6  01 0a        lsrs  r1, r0, #8          ; SYNC1 = sync >> 8
0x080115C8  04 20        movs  r0, #4
0x080115CA  02 f0 ac f9  bl    0x08013926         ; CC2500_WriteReg(4, SYNC1)
0x080115CE  e1 b2        uxtb  r1, r4             ; SYNC0 = sync & 0xFF
0x080115D0  05 20        movs  r0, #5
0x080115D2  02 f0 a8 f9  bl    0x08013926         ; CC2500_WriteReg(5, SYNC0)
0x080115D6  14 20        movs  r0, #0x14           ; 20
0x080115D8  02 f0 5e f8  bl    0x08013698         ; delay_us(20)      ← 建立时间
0x080115DC  33 20        movs  r0, #0x33           ; SCAL
0x080115DE  02 f0 03 fa  bl    0x080139E8         ; strobe(SCAL)
0x080115E2  7d 20        movs  r0, #0x7d
0x080115E4  c0 00        lsls  r0, r0, #3          ; r0 = 0x3E8 = 1000
0x080115E6  02 f0 57 f8  bl    0x08013698         ; delay_us(1000)
0x080115EA  10 bd        pop   {r4, pc}
0x080115EC  68 c3 00 00                          ; 字面量 50024（0x0000C368）
0x080115F0  ...                    ; 下一函数（不是补丁区）
```

原厂 56 字节（`apply()` 逐字节校验用的基准）：

```text
0c 4a 10 b5 41 1c 10 46 f1 f7 96 fd 04 46 01 0a
04 20 02 f0 ac f9 e1 b2 05 20 02 f0 a8 f9 14 20
02 f0 5e f8 33 20 02 f0 03 fa 7d 20 c0 00 02 f0
57 f8 10 bd 68 c3 00 00
```

### 2.2 函数边界与安全约束

- **函数前**：`0x08011500` 处有指向 `0x20001058`（settings 基址）的字面量引用，
  说明这是 C.Fn/设置侧代码的邻居；本补丁**不碰**函数之前/之后的任何字节。
- **函数后**：`0x080115F0` 是**真实函数起点**——RF 设置 handler `0x0800CD7C` 在
  `0x0800CD80` 处 `bl 0x080115F0`。补丁区**必须止步于此**：
  `keypatch/devices/v860iiis.py` 有 `assert SYNC_FN_SIZE == 56` 与
  「函数体结束地址 == 下一函数起点」的运行时检查（verify 第 3 项）。
- **字面量 50024 全镜像唯一**：小端 `68 c3 00 00` 只命中 `文件偏移 0x0E5EC / VA 0x080115EC`
  （`re/log_recon1.txt` §3；`re/cfn_settings.py` §3 断言 PASS）。补丁把 KEY/FLAG 两个字面量
  放进 `+0x2C/+0x30`，**原 `+0x34` 的 50024 保持同址同值**（普通模式公式仍依赖它）。
- `apply()` 打补丁前对 6 处原始字节逐一校验，任何一处不符立即 `raise ValueError`
  （防固件版本不对时刷错）。

### 2.3 补丁后的函数（key = `0xD798`）

布局（相对 `0x080115B8`；44 B 代码 + 3 个字面量）：

```asm
0x080115B8  10 b5        push  {r4, lr}
0x080115BA  0b 4b        ldr   r3, [pc, #0x2c]   ; r3 = 字面量 @+0x30 = 0x20001060（C.Fn LIGHT）
0x080115BC  0b 4a        ldr   r2, [pc, #0x2c]   ; r2 = 字面量 @+0x34 = 50024（原厂同址同值）
0x080115BE  1b 78        ldrb  r3, [r3]           ; r3 = LIGHT 值（0/1/2）
0x080115C0  41 1c        adds  r1, r0, #1          ; 普通模式除数 = ID + 1
0x080115C2  02 2b        cmp   r3, #2              ; LIGHT == ON(2)？
0x080115C4  01 d1        bne   0x080115CA          ; 否（12sec/OFF）→ 原厂公式
0x080115C6  07 4a        ldr   r2, [pc, #0x1c]   ; 密钥：r2 = 字面量 @+0x2C = KEY
0x080115C8  01 21        movs  r1, #1              ; 密钥模式除数 = 1（KEY/1 = KEY）
0x080115CA  10 46        mov   r0, r2
0x080115CC  f1 f7 90 fd  bl    0x080030F0         ; __aeabi_uidiv（统一走除法）
0x080115D0  04 46        mov   r4, r0
0x080115D2  01 0a        lsrs  r1, r0, #8          ; SYNC1 = sync >> 8
0x080115D4  04 20        movs  r0, #4
0x080115D6  02 f0 a6 f9  bl    0x08013926         ; CC2500_WriteReg(4, SYNC1)
0x080115DA  e1 b2        uxtb  r1, r4             ; SYNC0 = sync & 0xFF
0x080115DC  05 20        movs  r0, #5
0x080115DE  02 f0 a2 f9  bl    0x08013926         ; CC2500_WriteReg(5, SYNC0)
0x080115E2  10 bd        pop   {r4, pc}           ; 返回（无 SCAL、无延时）
0x080115E4  98 d7 00 00                          ; +0x2C .word KEY（0xD798）
0x080115E8  60 10 00 20                          ; +0x30 .word FLAG（0x20001060）
0x080115EC  68 c3 00 00                          ; +0x34 .word 50024（原厂字面量，原址原值）
```

补丁后 56 字节（`keypatch.py info` 实际输出）：

```text
10 b5 0b 4b 0b 4a 1b 78 41 1c 02 2b 01 d1 07 4a
01 21 10 46 f1 f7 90 fd 04 46 01 0a 04 20 02 f0
a6 f9 e1 b2 05 20 02 f0 a2 f9 10 bd 98 d7 00 00
60 10 00 20 68 c3 00 00
```

语义等价 C 代码：

```c
void rf_set_id(uint8_t id) {                  /* 0x080115B8 */
    flag = *(uint8_t *)0x20001060;            /* settings[8] = C.Fn LIGHT */
    if (flag == 2) sync = KEY;                /* LIGHT=ON -> 私有 KEY */
    else           sync = 50024 / (id + 1);   /* 12sec/OFF -> 原厂公式 */
    WriteReg(4, sync >> 8);                   /* SYNC1 */
    WriteReg(5, sync & 0xFF);                 /* SYNC0 */
    return;                                   /* 只写同步字：无 SCAL、无延时 */
}
```

---

## 3. 5 个调用点（入参都是 `settings[+6]` = 当前 ID）

全镜像对 `0x080115B8` 的 `bl` 调用共 **5 处**（`re/cfn_settings.py` §3 断言集合相等；
`re/log_recon1.txt` 与本文档的探针脚本均可复现）。每处在 `bl` 前一条指令都是
`ldrb r0, [rX, #6]`，即 `r0 = settings[6] = RAM[0x2000105E]`（当前 ID）：

| 调用点 | 所在函数 | 取参指令 | settings 基址来源 |
|---|---|---|---|
| `0x08005A78` | C.Fn 递减 handler `0x080059E4` | `0x08005A76 ldrb r0,[r0,#6]` | `0x08005A26 ldr r0,[pc,#0x330]` → 字面量 `@0x08005D58` = `0x20001058` |
| `0x08005E36` | RF 初始化路径（PATABLE/SCAL 收尾之后） | `0x08005E34 ldrb r0,[r0,#6]` | `0x08005E32 ldr r0,[pc,#0x1c]` → 字面量 `@0x08005E50` = `0x20001058` |
| `0x0800C5C2` | C.Fn 递增 handler `0x0800C528` | `0x0800C5C0 ldrb r0,[r0,#6]` | `0x0800C56A ldr r0,[pc,#0x36c]` → 字面量 `@0x0800C8D8` = `0x20001058` |
| `0x0800D0BA` | RF 设置复位分支（handler `0x0800CD7C` 内） | `0x0800D0B8 ldrb r0,[r4,#6]` | `0x0800D094 ldr r4,[pc,#0xc4]` → 字面量 `@0x0800D15C` = `0x20001058` |
| `0x0800D0E6` | 「恢复出厂」分支（`bl 0x08005E54` 之后） | `0x0800D0E4 ldrb r0,[r0,#6]` | `0x0800D0E2 ldr r0,[pc,#0x78]` → 字面量 `@0x0800D15C` = `0x20001058` |

即 5 个调用点执行前 `r0 = RAM[0x2000105E]`。补丁函数只接收 `r0`，**不需要硬编码这个 RAM 地址**；
LIGHT 开关标志地址 `0x20001060` 作为字面量写在函数体内（§2.3）。

补充静态线索：

- **原厂只有 item==6（ID）改值才重设同步字**：两条改值 handler 在 `cmp r1,#6` 后用 `bne`
  跳过 `ldrb r0,[r0,#6]; bl 0x080115B8`（`0x08005A72/74`、`0x0800C5BC/BE`）。
  **本补丁已把两个 `bne` 改成 `nop`**，于是除 item9(LCD) 外任意 C.Fn 项改值/导航都会立即重设同步字
  （见 §5.5）；Unicorn handler 级实测的证据在 `re/log_cfn_settings.txt` §9。
- ID 菜单保持原厂语义：C.Fn item6 仍是「ID / 范围 0..99 / 0=OFF」；密钥开关在 LIGHT 项。
- `settings[8]`（LIGHT）由原厂 save/load 随 72 B 记录持久化（§5.6）。

> 结论：5 个入口共享同一个「同步字函数」，**不存在**第二个绕开它的同步字设置点；
> 因此整函数改写（而不是逐个调用点打补丁）是正确且最省的选择。

---

## 4. CC2500 写点与初始化覆盖关系

### 4.1 辅助函数

| 符号 | 地址 | 说明 |
|---|---|---|
| `__aeabi_uidiv` | `0x080030F0` | 自包含软件除法（22 条指令、无 `BL`、含回边循环）；`r0/r1` 入、`r0` 商（入口字节 `30 b5 0b 46 …`） |
| `CC2500_WriteReg` | `0x08013926` | 写寄存器：`r0`=寄存器地址、`r1`=值（内部 `0x0801392C bl 0x08013A00` 写 1 字节；入口字节 `13 b5 01 22 01 a9 00 f0`） |
| `strobe` | `0x080139E8` | 发命令选通（`r0`=命令字节，如 `0x33`=SCAL；入口字节 `10 b5 04 46 00 f0`） |
| `delay_us` | `0x08013698` | 软件延时（`subs r0,#1; uxth;` 内层 14×`nop` 循环；原厂同步字函数里 `20` 与 `1000` 都走它） |
| CC2500 初始化表 | `0x080136C8` | 一次写入寄存器 `0x00..0x2E` 各一次（共 47 次 `WriteReg`，含 `reg4/reg5` 缺省 `0xC3/0x68`），见 §4.2 |

### 4.2 初始化表写的 SYNC1/SYNC0 会被覆盖

初始化表函数（入口 `0x080136C8`）在 `0x080136F2..0x080136FE` 写入：

```asm
0x080136F2  c3 21        movs  r1, #0xC3
0x080136F4  04 20        movs  r0, #4        ; SYNC1 = 0xC3
0x080136F6  00 f0 16 f9  bl    0x08013926
0x080136FA  68 21        movs  r1, #0x68
0x080136FC  05 20        movs  r0, #5        ; SYNC0 = 0x68
0x080136FE  00 f0 12 f9  bl    0x08013926
```

`0xC3/0x68` 正是 `0xC368 = 50024` 的高/低字节，也就是**原厂 ID=0 时的同步字**。
这组值只是「初始缺省」。初始化表函数只被 RF 初始化函数在 `0x08005DE6` 调用一次，
随后同一函数在 `0x08005E36` 按当前 ID（及 LIGHT 开关）覆盖它：

```text
RF 初始化函数 0x08005DD0（本片段 0x08005DE6..0x08005E3A）
  ├─ bl 0x080136C8              ; 0x08005DE6  初始化表：写 reg0x00..0x2E（reg4=0xC3、reg5=0x68）
  ├─ WriteReg(0x3E, …)          ; 0x08005E1E/0x08005E20  PATABLE（功率表）
  ├─ strobe(0x33)               ; 0x08005E24/0x08005E26  SCAL
  ├─ delay_us(1000)             ; 0x08005E2A..0x08005E2E
  └─ bl 0x080115B8              ; 0x08005E36  按当前 ID / LIGHT 覆盖 SYNC1/SYNC0
```

触发链（`re/log_sync_audit.txt` §5）：整机初始化 `0x08012364` → RF 初始化 `0x08005DD0`
→ 无条件重设同步字（该路径没有 `cmp #6` gate）。

### 4.3 全镜像扫描：只有两处写 SYNC1/SYNC0

对全镜像的 `CC2500_WriteReg(0x08013926)` 调用点做「`r0` 立即数回溯」扫描，共 **60 个调用点**
（其中初始化表占 47 个），写寄存器 **4/5** 的只有：

| 调用点 | 寄存器 | 场景 |
|---|---|---|
| `0x080136F6` / `0x080136FE` | 4 / 5 | CC2500 初始化表（缺省 `0xC3/0x68`，随后被覆盖） |
| `0x080115CA` / `0x080115D2` | 4 / 5 | 同步字函数 `0x080115B8`（**唯一运行期写入**） |

其它 `WriteReg` 调用点**都不写 4/5**（写的是初始化配置、频率、PATABLE 等，寄存器号各不同；
底层写函数 `0x08013A00` 的直接调用点里也没有 reg4/5）。

> 结论：运行期唯一的同步字写入口就是 `0x080115B8`；初始化表的缺省值会被它覆盖。
> 所以**只改这一个函数**即可覆盖所有「设置同步字」路径，不需要 hook 初始化路径。
> 证据：`re/log_sync_audit.txt` §6（60 个调用点寄存器号全部可静态解析，写 4/5 恰为 4 个）与
> `re/log_cfn_settings.txt`。

---

## 5. 用户开关语义：C.Fn `LIGHT` = 密钥开关

### 5.1 为什么用 C.Fn，为什么选 `LIGHT`

用户要求「改用 C.Fn」——用设备自带菜单项当密钥开关，不改 UI。V860III-S 的 C.Fn 是
**3 页 × 4 行、本机型共 10 项、不显示 F 编号**（见 §5.2/§5.7）。

**为什么最终选 `LIGHT`（item 8）**：

| 理由 | 说明 |
|---|---|
| **语义直观** | LIGHT 有三档：`12sec`(0) / `OFF`(1) / `ON`(2)。**把 `ON`（值 2）当密钥模式 = 用户看到 ON 即开密钥** |
| 出厂默认即普通模式 | 出厂默认 `settings[8]=0`（12sec）→ 新刷/恢复出厂 = **普通模式**，符合预期 |
| 功能读者集中、可屏蔽 | 造型灯任务 `0x08003950` 只有 2 个 `settings[8]` 功能读者（`0x08003956`/`0x0800397E`），各 2 字节改成常数 0 即固定回原厂 12sec 档 |
| 非 0/1 值可控 | `flag == 2` 才进密钥模式；0/1 都走原厂公式，避免把「12sec/OFF」误当开关 |
| 持久化现成 | `settings[8]` 随原厂 72 B 设置记录写 flash（record+`0x36`），掉电保持 |
| 不碰 RF/闪光关键路径 | 只涉及造型灯 GPIO（`0x48001000` 位 13/14，`re/log_cfn_settings.txt` §6），不碰射频/闪光输出 |

> 备选项（例如 `BEEP` item7 两档）：settings 字节仍是 `0x20001058+7`，但需要把同步字函数里
> `cmp r3,#2` 改成 `cmp r3,#1` 并把 `KEY_FLAG_ON_VALUE` 改 1——本轮**未做**，仅记录为可选方案。

### 5.2 C.Fn 子系统与 10 项总表

证据来源：[`../../keypatch/dev/v860iiis/CFN_SETTINGS.md`](../../keypatch/dev/v860iiis/CFN_SETTINGS.md)
+ [`re/log_cfn_settings.txt`](re/log_cfn_settings.txt)、
[`../../keypatch/dev/v860iiis/CFN_MENU.md`](../../keypatch/dev/v860iiis/CFN_MENU.md)
+ [`re/log_cfn_menu.txt`](re/log_cfn_menu.txt)。

- 设置区基址 **`0x20001058`**：item n = `settings[n]`（`re/cfn_settings.py` §2 断言：
  该 u32 字面量全镜像 **30 处**、被 **42 条 `LDR` 指令**引用）；
- 最大值/回绕表 **`0x08016102`**，12 字节：`01 01 01 01 01 1F 63 01 02 06 01 01`
  → item0..11 的 max = `1,1,1,1,1,31,99,1,2,6,1,1`；**3 个 LDR 引用点**（`0x08005A6A` 递减、
  `0x0800C586` 递增、`0x0800F086` 设置加载后的钳位）；钳位循环 `0x0800F08C..0x0800F09C`
  对 12 项逐项 `settings[i] > max[i] → settings[i]=0`（`cmp r0,#0xC` 边界；LIGHT max=2，
  密钥值 2 不会被清）；
- 持久化/加载按 **12 项**处理（`settings[0..13]` 打包进 72 B 记录），但**本机型 UI 只显示 10 项**：
  item10/11（平台表的 `FEB ACL` / `TX`）**没有菜单入口**，`+/-` 只在 item0..9 间循环
  （`re/log_cfn_menu.txt` §0/§4/§6；`re/log_cfn_settings.txt` §4 INFO）；
- 固件内没有可直接读的 C.Fn 项名字符串（UI 用 8×16 字形表 `0x080143D6`，17 B/字形），
  **项名由真实执行渲染函数重建**（`re/cfn_menu.py` + `re/log_cfn_menu.txt`）。

| item | 显示名 | 右栏取值 | max | 出厂默认 | 与密钥开关的关系 / 备注 |
|---|---|---|---|---|---|
| 0 | `m/ft` | m / ft | 1 | 0（m） | 纯显示 |
| 1 | `AF` | ON / OFF | 1 | 1（OFF） | 辅助对焦灯 |
| 2 | `STBY` | 月亮图标 + ON/OFF | 1 | 0（ON） | 待机 |
| 3 | `RX STBY` | 60min / 30min | 1 | 0 | 接收待机 |
| 4 | `SCAN` | OFF / START（SET 在 `settings[4]==1` 时启动扫描） | 1 | 0 | 动作项，**改值会被扫描流程清回 0**，不宜复用（与 V1C 同结论） |
| 5 | `CH` | 两位十进制 = `settings[5]+1`（01..32） | 31 | 0x14 | 信道 |
| 6 | `ID` | OFF（ID=0）/ 两位十进制 01..99 | 99 | 0 | **保留原厂含义**（普通模式的 ID） |
| 7 | `BEEP` | 喇叭图标 + OFF / ON | 1 | 1（ON） | 备选开关项（需改函数里的比较值） |
| **8** | **`LIGHT`** | **12sec / OFF / ON**（箭头行 = `2*settings[8]+4`） | **2** | **0（12sec）** | **★ 选为密钥开关**；补丁把造型灯任务的两个功能读者固定为 0 |
| 9 | `LCD` | 竖排 -3..+3 刻度（表 `0x08015A48`） | 6 | 3 | 亮度/对比；改值走专用函数 `0x080128F0`，**不触发同步字**（补丁前后一致） |
| 10 | *(无菜单)* | — | 1 | 0 | 平台表遗留（原 `FEB ACL`），本机型 UI 不可达 |
| 11 | *(无菜单)* | — | 1 | 1 | 平台表遗留（原 `TX`），本机型 UI 不可达 |

> 项名/取值/默认值来源：`re/log_cfn_menu.txt` §9（渲染真实执行 + 帧缓冲重建）与
> `re/log_cfn_menu.txt` §7.1（出厂默认 `[m/ft=0, AF=1, STBY=0, RX STBY=0, SCAN=0, CH=0x14, ID=0, BEEP=1, LIGHT=0, LCD=3]`）。

### 5.3 开关语义（现行）

```c
flag = *(uint8_t *)0x20001060;     // settings[8] = C.Fn LIGHT 值（0=12sec / 1=OFF / 2=ON）
if (flag == 2) sync = KEY;         // 密钥模式（LIGHT = ON）
else           sync = 50024 / (id + 1); // 原厂普通模式（12sec / OFF）
WriteReg(4, sync >> 8);            // SYNC1
WriteReg(5, sync & 0xFF);          // SYNC0
// 只写同步字：无 SCAL、无延时
```

| C.Fn 第 3 页 LIGHT | settings[8] | 同步字 | 模式 | ID 菜单 |
|---|---|---|---|---|
| **ON（值 2）** | `2` | **私有 KEY**（如 `0xD798`） | **密钥模式**：只接受这把钥匙发来的帧 | 原厂含义 |
| **12sec（值 0，出厂默认）** | `0` | `50024 / (ID+1)` | 普通模式：与未打补丁的原厂一致 | 原厂含义 |
| OFF（值 1） | `1` | `50024 / (ID+1)` | 普通模式（同上） | 原厂含义 |

要点：

- **出厂默认 `settings[8]=0`（12sec）** → 新刷 / 恢复出厂后默认是**普通模式**；
- 用户要开密钥：进 **C.Fn → 第 3 页（page2）→ 第 1 行 `LIGHT` → 设为 `ON`**；
  关密钥：改回 `12sec`（或 OFF）；
- **UI 只显示名称、不显示 F1..F12 编号**，操作说明用「第 3 页第 1 行 LIGHT」；
- **原厂 LIGHT 功能代价**：原厂 LIGHT 控制造型灯模式（12sec/OFF/ON）；复用后
  **造型灯输出固定为原厂 12sec 档**（补丁把造型灯任务的两个读者固定为 0），
  用户不能再通过 C.Fn 的 LIGHT 改造型灯模式。菜单里 LIGHT 仍显示三档，但只有「ON=密钥开关」生效。

### 5.4 读者屏蔽与开关保护（为什么密钥开关不会被原厂逻辑改回）

`settings[8]` 在原厂固件里的**静态读者共 5 处、写者共 3 处**（`re/log_cfn_settings.txt` §6 断言集合）：

| 位置 | 原厂字节 | 补丁后 | 作用 |
|---|---|---|---|
| `0x08003956`（造型灯任务 `0x08003950`，读者 1） | `00 7a` = `ldrb r0,[r0,#8]` | `00 20` = `movs r0,#0` | 造型灯任务看到 LIGHT=0（12sec），**密钥开关不影响造型灯输出** |
| `0x0800397E`（造型灯任务，读者 2） | `01 7a` = `ldrb r1,[r0,#8]` | `00 21` = `movs r1,#0` | 同上（第二个读者） |
| `0x0800D0A2`（RF 设置复位分支） | `26 72` = `strb r6,[r4,#8]` | `00 bf` = `nop` | **不再用默认值覆盖 `settings[8]`** → 防止密钥开关被无线/远程「设置复位」静默清 0 |

**保留不屏蔽的读者/写者**（有意为之）：

| 位置 | 类型 | 说明 |
|---|---|---|
| `0x08004A8A` / `0x08004A9A` | 读者（菜单显示 ×2） | C.Fn 画面画 LIGHT 的 `12sec/OFF/ON` 与箭头，用户能看到/操作开关 |
| `0x08005816` | 读者（save builder） | `ldrb r4,[r2,#8]`：把 LIGHT 值打包进 flash 记录（必须保留） |
| `0x08005EB0` | 写者（出厂默认） | `01 72` = `strb r1,[r0,#8]`，`r1=0` → 恢复出厂 = 12sec = 普通模式（语义正确） |
| `0x08013C48` | 写者（load 展开） | `13 72` = `strb r3,[r2,#8]`：上电从 flash 记录恢复 LIGHT 值（必须保留） |

**残留风险（需实机验证）**：除上述 3 个写者外本轮未再发现 `settings[8]` 的静态写点
（`re/log_cfn_settings.txt` §6 断言「全部静态写者」集合恰好为这 3 处）；另有 load task 末尾的
**动态钳位** `0x0800F094`（`strb r4,[r2,r0]`，`settings[i] > max[i]` 时清 0，12 项循环）——
LIGHT max=2，密钥值 2 不会被清。但「设置同步/远程设置」在运行期的**动态**影响
（经 `settings` 数组整体复制、其它 RAM 别名等）无法用静态扫描完全排除，**必须实机确认**。

NOP `0x0800D0A2` 的安全性（`CFN_SETTINGS.md` §10.1）：`strb` 不改变条件标志位；该分支后续
`0x0800D0A4..0x0800D0AC` 只写 settings[4..9]（不再写 s8）、调用 LCD 应用函数 `0x080128F0`、
读 settings[6] 调 sync，随后 `b 0x0800D10C` 读 `0x200004D7` 后 `pop`，**不再读 settings[8]**；
settings[8] 保留原值，不会「未初始化」。恢复出厂（`0x0800D0DA` → `0x08005E54`）仍会把
LIGHT 清 0（合理语义，未 patch）。

### 5.5 改值即时生效：2 字节 `bne → nop`

原厂只有「改 ID（item==6）」才立即调用同步字函数；换成 LIGHT 后，**改 LIGHT 也必须立即生效**。
本补丁的做法（**无需代码洞**）：

| 位置 | 原厂 | 补丁后 | 说明 |
|---|---|---|---|
| `0x08005A74`（C.Fn 递减 handler `0x080059E4` 内） | `02 d1` = `bne 0x08005A7C`（跳过 sync） | `00 bf` = `nop` | gate 失效 → 继续执行 `0x08005A76 ldrb r0,[r0,#6]` + `0x08005A78 bl sync`，然后 `pop` |
| `0x0800C5BE`（C.Fn 递增 handler `0x0800C528` 内） | `f5 d1` = `bne 0x0800C5AC`（跳过 sync） | `00 bf` = `nop` | gate 失效 → 继续 `0x0800C5C0 ldrb r0,[r0,#6]` + `0x0800C5C2 bl sync`，然后 `pop` |

效果：**除 item9(LCD) 外的任意 C.Fn 项改值**都会立即重设同步字。item9(LCD) 在原厂就
`cmp #9; beq` 转到专用函数 `0x080128F0`（`0x08005A7E` / `0x0800C5C8`），不经过 gate，
补丁后仍**不触发**同步字（与所改值无关，符合预期）。

gate 进入现场（`CFN_SETTINGS.md` §3）：两个 handler 都**没有函数入参**——
`r0 = settings 基址 0x20001058`（分别由 `0x08005A26` / `0x0800C56A` 载入），
`r1 = 当前项索引`（从 RAM `0x200004D6` 读取），由键事件分发函数 `0x080058E4`
（事件码 `0x78` → 递减、`0xB4` → 递增）调用；编辑态相关 RAM：
`0x200004D4`（C.Fn 画面）、`0x200004D5`（值编辑；0=光标移动）、`0x20000536`（忙标志，
=1 时提前返回）、`0x20000563`（=3 时走 `0x0800EAE8` 分支）。
因为 gate 是 `bne`（不是 `beq`），改成 `nop` 后自然 fall-through 到既有的
`ldrb r0,[r0,#6]; bl 0x080115B8`，**item==6 的原行为完全保留**。

Unicorn handler 级实测（`re/log_cfn_settings.txt` §9，原厂 vs 方案(a) `nop` vs 方案(b) `BL`-hook）：

- 原厂：只有 **item6(ID)** 改值会 sync，其余 0 次；
- 补丁（方案 a）：**item 0..8、10、11** 的 `dec/inc`（含回绕）各 **1 次** sync，且 `r0 == settings[6]`；
  item9 恒 0 次；`SP` 与 `r3..r7` 原样恢复；忙标志路径（`[0x20000536]==1`）0 次 sync、settings 不变；
- 光标移动（未编辑态 `+/-` 换项）也会触发 sync：**递减原索引 1..9、递增原索引 0..7 各 1 次**；
  「回绕到 item9」的路径 0 次（原厂只在落到 item6 时触发 1 次）。属已知副作用
  （`CFN_SETTINGS.md` §10.3）：因为改写后的同步字函数只做两次寄存器写、无 SCAL/延时，
  菜单导航的额外开销可忽略；
- 方案(a) 与方案(b)（`BL` 注入 + 16 B hook）在全部 item/取值上**行为等价**；
  本补丁取 **(a)**：只要 4 字节、零新增代码、零死区依赖。

> 为什么不加 10 B 注入 hook（V1C/V860III-C 的做法）：见 §6.5。

### 5.6 持久化

链路（地址均已静态核对，`re/log_cfn_settings.txt` §7 断言 PASS）：

```text
改 C.Fn 值
  → 保存倒计时 [0x20000516] = 200（0x080109C8，`01 49 c8 20 08 80 70 47`）
  → tick 0x080109A4 递减，到 0 置 [0x20000518] = 1（`03 49 01 20 08 70`）
  → 保存任务 0x0800D9C8（读 0x20000518 / 0x2000055A 守卫）
      → save builder 0x080056D8：把 settings[0..0xD] 打包进 72 B 记录（目标缓冲 0x20000FD4）
           · settings[8] 读取点 0x08005816（`14 7a` = ldrb r4,[r2,#8]）
           · s6..s9 组写 record+0x34，**settings[8] 落在 record+0x36**
      → 比对/提交 0x08012F68（`0x12` words = 72 B，与上一份记录比对，返回 1=有变化）
  → flash 写函数 0x08010D94 → 参数区基址 0x0801F000，20 槽（0x14）× 0x64 B，记录头字节 0x5C
  → 槽计数 0x20000428，≥ 0x14 回绕（覆盖最旧槽）

上电
  → load 任务 0x0800EEB0：从 0x0801F000 起按 20 槽逆序扫描，
     校验记录头 == 0x5C（0x20000F8C）后
  → 展开函数 0x08013B08：把记录写回 RAM 变量，settings[8] 写点 0x08013C48（`13 72`）
     并按最大值表 clamp（LIGHT max=2，密钥值 2 合法，不会被清）
  → 无有效记录 → 出厂默认 0x08005E54（其中 `01 72` 写 settings[8]=0，即 12sec/普通模式）
```

→ **`LIGHT=ON` 掉电保存，重启后仍是密钥模式**；恢复出厂 / 无效记录回落到普通模式。
补丁不需要额外做存储。

### 5.7 菜单/UI（未改，仅复用）

- 补丁**不新增**任何 UI/字库/菜单项，也不改显示代码；
- C.Fn 画面由渲染函数 **`0x0800431C`**（`0x0800431C..0x08004D03`，2,536 B）绘制，
  唯一调用点 **`0x0801219E`**（画面分派任务 `0x08012124` 内，条件 `[0x200004D4]==1`）；
  页/槽位 `page = idx>>2`、`slot = idx&3`（`0x080043C2`/`0x080043C4`）；
- UI 变量：画面 `0x200004D4` / 编辑态 `0x200004D5` / 选中项 `0x200004D6` / 另见 `0x200004D7`；
- 帧缓冲 `0x200007AC`（168×96 单色，12 条带 × `0xA8` = 2,016 B）；8×16 字形表 `0x080143D6`
  （17 B/字形）、`DRAW16=0x080069C0`；8×8 表 `0x08014088`/`0x0801437C`、`DRAW8=0x08006908`；
  LCD 刷新 `0x08012880`（每次 3 条带）；
- **本机型 C.Fn 只有 10 项**（`idx 0..9`）：`m/ft, AF, STBY, RX STBY, SCAN, CH, ID, BEEP, LIGHT, LCD`；
  **第 3 页 = `LIGHT`(第 1 行)、`LCD`(第 2 行)**，`FEB ACL`/`TX` 两项在本机型 UI 中**不存在**；
- LIGHT 右栏三档 `12sec / OFF / ON`（箭头行 = `2*settings[8]+4`），编辑态用 `0x08004D50` 反显选中档；
- 按键（**键名是按功能与同代 UI 推断，物理丝印需实机确认**）：`MENU` 类键（键码 8）→
  `0x08004D04` 切换 C.Fn 画面；`+/-`（`0x080058E4`）→ 未编辑时移动项（0..9 循环）、
  编辑态改值（按 `0x08016102` 回绕）；`SET`（`0x08011454`）→ 切换编辑态；
  返回键（键码 1）→ `0x0800CCB0` 关闭；
- 因此复用 LIGHT 项**零 UI 改动**即可被用户看到/操作；**ID 菜单保持原厂含义**。
- 证据：[`re/cfn_menu.py`](re/cfn_menu.py) + [`re/log_cfn_menu.txt`](re/log_cfn_menu.txt)
  （158 PASS / 0 FAIL；报告 [`../../keypatch/dev/v860iiis/CFN_MENU.md`](../../keypatch/dev/v860iiis/CFN_MENU.md)）。

---

## 6. 补丁布局（6 处原位补丁，文件长度不变 = 79,300 B）

### 6.1 同步字函数 `0x080115B8`（56 B）

| 偏移 | 长度 | 内容 | 说明 |
|---|---|---|---|
| `+0x00` | 2 | `push {r4, lr}` | 保护现场 |
| `+0x02` | 2 | `ldr r3, =FLAG` | 字面量在 `+0x30`（`0x20001060`） |
| `+0x04` | 2 | `ldr r2, =50024` | 字面量在 `+0x34`（原厂地址、原值不变） |
| `+0x06` | 2 | `ldrb r3, [r3]` | r3 = LIGHT 开关值（0/1/2） |
| `+0x08` | 2 | `adds r1, r0, #1` | 普通模式除数 = ID+1 |
| `+0x0A` | 2 | `cmp r3, #2` | LIGHT == ON(2)？ |
| `+0x0C` | 2 | `bne go` | 否（12sec/OFF）→ 原厂公式（跳到 `+0x12`） |
| `+0x0E` | 2 | `ldr r2, =KEY` | 密钥模式：字面量在 `+0x2C` |
| `+0x10` | 2 | `movs r1, #1` | 密钥模式除数 = 1（复用同一段除法） |
| `+0x12` | 2 | `go: mov r0, r2` | 被除数 |
| `+0x14` | 4 | `bl __aeabi_uidiv` | 统一走除法：KEY/1 或 50024/(ID+1) |
| `+0x18` | 2 | `mov r4, r0` | r4 = sync |
| `+0x1A` | 2 | `lsrs r1, r0, #8` | SYNC1 = sync >> 8 |
| `+0x1C` | 2 | `movs r0, #4` | — |
| `+0x1E` | 4 | `bl CC2500_WriteReg` | 写 SYNC1 |
| `+0x22` | 2 | `uxtb r1, r4` | SYNC0 = sync & 0xFF |
| `+0x24` | 2 | `movs r0, #5` | — |
| `+0x26` | 4 | `bl CC2500_WriteReg` | 写 SYNC0 |
| `+0x2A` | 2 | `pop {r4, pc}` | 返回（**无 SCAL、无延时**） |
| `+0x2C` | 4 | `.word KEY` | 私有密钥 |
| `+0x30` | 4 | `.word FLAG`（`0x20001060`） | C.Fn LIGHT 开关字节 |
| `+0x34` | 4 | `.word 50024` | **原厂字面量，保持同址同值** |

### 6.2 两处 C.Fn 改值 gate（`bne → nop`，共 4 B）

| 位置 | 原厂 2 B | 补丁后 2 B | 说明 |
|---|---|---|---|
| `0x08005A74`（C.Fn 递减处理 `0x080059E4` 内） | `02 d1`（`bne 0x08005A7C`） | `00 bf`（`nop`） | 任意项改值后落入 `ldrb r0,[r0,#6]; bl 0x080115B8` |
| `0x0800C5BE`（C.Fn 递增处理 `0x0800C528` 内） | `f5 d1`（`bne 0x0800C5AC`） | `00 bf`（`nop`） | 同上 |

### 6.3 造型灯读者屏蔽 / 开关保护（共 5 B）

| 位置 | 原厂 | 补丁后 | 说明 |
|---|---|---|---|
| `0x08003956` | `00 7a`（`ldrb r0,[r0,#8]`） | `00 20`（`movs r0,#0`） | 造型灯任务读者固定 0 → 输出保持原厂 12sec 档 |
| `0x0800397E` | `01 7a`（`ldrb r1,[r0,#8]`） | `00 21`（`movs r1,#0`） | 同上（第二个读者） |
| `0x0800D0A2` | `26 72`（`strb r6,[r4,#8]`） | `00 bf`（`nop`） | RF 设置复位不再覆盖 `settings[8]`，保护密钥开关 |

### 6.4 差异统计与安全约束

- 与原厂的总差异 = **58 字节**，**全部**落在上述 6 处补丁点内（`verify()` 用集合比对断言）：
  同步字函数区 **49 B**（56 B 区内 7 B 与原厂相同：`+0x25..+0x27` 的 `20 02 f0` 片段，
  以及 `+0x34..+0x37` 的 50024 字面量）+ 两个 gate 4 B + 两个读者 3 B（`0x08003956` 只变高字节）
  + 一个保护点 2 B；
- 补丁点原始字节全部逐字节校验，固件版本不符直接 `raise ValueError`（防刷错）；
- 输出长度 == 输入长度 == 79,300 B（原位补丁）；
- 50024 字面量保持原址原值（普通模式公式仍依赖它）。

### 6.5 为什么不用代码洞（与 V1C / V860III-C 的差异）

V1C / V860III-C 的「改值即时生效」需要在全 0 死区里放 10 B 注入 hook，再把两个 gate 改成 `BL`。
本机型**不需要**：

- 本补丁把两个 gate 的 `bne` 直接改成 `nop`（共 4 B），行为已验证与注入方案等价
  （`re/log_cfn_settings.txt` §5/§9：方案(a) NOP vs 方案(b) BL-hook 全部 item/取值一致）；
- 本镜像**没有可证死区**（`re/log_sync_audit.txt` §7 与 [`../../keypatch/dev/v860iiis/SYNC_AUDIT.md`](../../keypatch/dev/v860iiis/SYNC_AUDIT.md) §0 第 6 条 +
  `re/log_cfn_settings.txt` §5 INFO）：
  全镜像 **没有任何 ≥16 B 的 `0xFF` 擦除态区间**；≥16 B 的全 0 区间共 27 个，其中绝大多数
  位于被引用的数据块（图形/查表数据）内部；唯一「零引用且不在被引用数据块内」的是
  **向量表保留槽 `0x08003010`（28 B，ARMv6-M Reserved 异常，硬件永不取用）**，
  但它改的是向量表且只有 28 B；`0x080162B9` 起的 112 B 零区虽零引用，却位于数据段尾部
  `0x08016000` 之后的 LUT 数据表内，**只能算「条件可用」**（审计脚本自身也把它标为需动态确认，
  且 `0x08016328` 被真实代码作为数据指针引用）；
- 因此 2 字节 NOP 比注入更小、更安全（不新增可执行代码、不依赖死区判断、不改向量表）。

> 若未来需要更强行为（例如让「光标移动」也精确重设同步字），仍可评估方案(b) `BL` hook；
> 其编码与 16 B hook 已在 `re/log_cfn_settings.txt` §5 给出并通过等价性验证，但**本轮未采用**。

### 6.6 为什么去掉 `SCAL(0x33)+delay(1000)`（以及 20 µs）

本补丁只改同步字、不改频率寄存器，CC2500 手册上改同步字不需要重新校准；56 B 空间
被 LIGHT 开关判断占用，**连原厂的 `delay(20µs)` 也不再保留**（只做两次寄存器写），
这是相对原厂**唯一的射频行为差异**。

> **若实机出现「偶发首包丢失 / 配对不稳」，优先怀疑这里**（回退方案：用保留 strobe/延时的
> 变体，或改用注入方案让原厂收尾继续执行）。

---

## 7. 验证记录（离线，2026-11-11）

### 7.1 复现命令

```powershell
# 仓库根
tools\python\python.exe keypatch\keypatch.py info --device v860iiis
tools\python\python.exe keypatch\keypatch.py patch --device v860iiis --key 0xD798 --verify --out %TEMP%\kp_v860iiis
tools\python\python.exe keypatch\keypatch.py patch --device v860iiis --key 0xD798 --verify --container --out %TEMP%\kp_v860iiis_c

# 模块级（等价于 keypatch.py verify）
tools\python\python.exe -c "import sys; sys.path.insert(0,'keypatch'); from devices import v860iiis; import common; raw=common.read_file('devices/v860iiis/firmware/V860IIIS_V1.1.bin'); patched,log=v860iiis.apply(raw,0xD798); [print(('PASS' if ok else 'FAIL'),msg) for ok,msg in v860iiis.verify(patched,0xD798)]"

# 总自检（全设备；输出到 %TEMP%，不覆盖 keypatch/out）
tools\python\python.exe keypatch\selftest.py --key 0xD798 --out %TEMP%\kp_selftest_v860

# 逆向专项脚本
tools\python\python.exe devices\v860iiis\re\cfn_settings.py
tools\python\python.exe devices\v860iiis\re\cfn_menu.py
tools\python\python.exe devices\v860iiis\re\sync_audit.py
```

### 7.2 模块 `verify()`：73 项全部 PASS（实测 73 PASS / 0 FAIL）

| # | 层 | 断言（`keypatch/devices/v860iiis.py`） |
|---|---|---|
| 1 | 静态 | 固件长度 79,300 B（原位补丁） |
| 2 | 静态 | 同步字函数 56 B 与期望函数体逐字节一致 |
| 3 | 静态 | 函数体结束 == `0x080115F0`（不越界） |
| 4 | 静态 | 字面量 KEY@`0x080115E4` / FLAG(`0x20001060`)@`0x080115E8` / 50024@`0x080115EC` |
| 5–6 | 静态 | 两个改值 gate `0x08005A74` = `00 bf`、`0x0800C5BE` = `00 bf` |
| 7–9 | 静态 | 三个读者/保护点 `0x08003956` = `00 20`、`0x0800397E` = `00 21`、`0x0800D0A2` = `00 bf` |
| 10 | 静态 | 与原厂差异集合 == 期望补丁点集合（58 B） |
| 11 | 静态 | 差异之外无任何字节变化（0 处） |
| 12 | Unicorn | 调用级测试说明（Cortex-M0、`RAM[0x20001060]=flag`） |
| 13–15 | Unicorn | **flag=2（LIGHT=ON）**：ID=0/5/99 → `WriteReg(4,KEY>>8)`、`WriteReg(5,KEY&0xFF)` |
| 16–20 | Unicorn | **flag=0（12sec）**：ID=0/1/5/37/99 → 原厂 `50024/(ID+1)` |
| 21–25 | Unicorn | **flag=1（OFF）**：ID=0/1/5/37/99 → 原厂 `50024/(ID+1)` |
| 26–30 | Unicorn | **原厂自证**：原厂 56 B 在 ID=0/1/5/37/99 → 原厂公式 + `delay(20)+SCAL(0x33)+delay(1000)` |
| 31 | Unicorn | C.Fn handler 级测试说明（`dec=0x080059E4` / `inc=0x0800C528` / 索引 RAM `0x200004D6`） |
| 32–53 | Unicorn | **handler 级（改值）**：item 0..8、10、11 的 `dec`/`inc`（含回绕）→ 各 1 次 sync、`r0==settings[6]`、新值正确、`SP` 与 `r3..r7` 恢复 |
| 54–59 | Unicorn | **handler 级（非回绕路径）**：item 5/6/8 的 `dec(max)→max-1`、`inc(0)→1` |
| 60–61 | Unicorn | **handler 级（item9 LCD）**：`dec`/`inc` → 0 次 sync |
| 62–73 | Unicorn | **原厂 handler 自证**：原厂 handler 在 item 0..11 上只有 item6 会 sync（1 次），其余 0 次 |

关键输出（key = `0xD798`，节选）：

```text
[PASS] 固件长度 79300 字节（应为 79300，原位补丁）
[PASS] 0x080115B8..0x080115EF 补丁区 56 字节与期望函数体完全一致
[PASS] 函数体结束于 0x080115F0 == 下一函数起点 0x080115F0（不越界）
[PASS] 字面量: KEY=0xD798 @0x080115E4, FLAG=0x20001060 @0x080115E8, 50024 @0x080115EC
[PASS] 即时生效 gate 0x08005A74 = 00 bf / 0x0800C5BE = 00 bf
[PASS] 读者屏蔽 0x08003956 = 00 20 / 0x0800397E = 00 21 / 0x0800D0A2 = 00 bf
[PASS] 与原厂差异 58 字节，offset=[0x00957, 0x0097E, 0x0097F, 0x02A74, …, 0x0E5EB]（应恰为 …）
[PASS] 差异之外无任何字节变化（0 处）
[PASS] Unicorn 2.1.4 调用级测试：同步字 0x080115B9，RAM[0x20001060]=flag
[PASS] ID=0  flag=2 密钥模式(LIGHT=ON): sync=0xD798 -> SYNC1=0xD7 SYNC0=0x98 | WriteReg(4,0xD7) -> WriteReg(5,0x98)
[PASS] ID=99 flag=0 原厂公式: sync=0x01F4 -> SYNC1=0x01 SYNC0=0xF4 | WriteReg(4,0x01) -> WriteReg(5,0xF4)
[PASS] C.Fn handler item=8  dec sync 已触发: sync(r0=50)，sync 次数=1(期望 1)，settings[8]=2(期望 2)，返回=是 SP=0x20001480 r3-r7=OK
[PASS] C.Fn handler item=9  dec sync 未触发: sync 次数=0(期望 0)，返回=是 SP=0x20001480 r3-r7=OK
[PASS] ID=0/1/5/37/99 原厂(自证): WriteReg(4,hi) -> WriteReg(5,lo) -> delay(20) -> WriteCmd(0x33) -> delay(1000)
```

### 7.3 本轮产物（key = `0xD798`，静态生成、**未实机**）

| 文件 | 大小 | sha256 |
|---|---|---|
| `V860IIIS_V1.1_keyD798.bin` | 79,300 B | `9ef11e8116e07dda30bd08ea407498f373ad3d953effb32258e3104c2ba5753d` |
| `V860IIIS_V1.1_keyD798_container.bin` | 79,361 B（payload 79,300 + footer 61） | `208f001d92d8ac543a8656905a7aac2da10a3454ccdfc12b03dcf4e56bc40b05` |

> sha256 会随密钥变化；换密钥必须重新生成。容器备用件是 G3 对 V2.0/A1 设备要求的格式兜底
> （文件名含 `V860IIIS`，G3 才认），正常情况下直接用裸 `.bin` 即可。

### 7.4 Lead 独立交叉验证（`keypatch/dev/lead/call_harness.py`）：8/8 PASS

`call_harness.py` 的 `v860iiis` 条目（`base=0x08003000`、`entry=0x080115B8`、
`write_reg=0x08013926`、`strobe=0x080139E8`、`delay=0x08013698`、`sp=0x20001400`、`arch="m0"`）
在 `selftest.py` 中逐 ID 对比「开关 ON / 开关 OFF」，本机实测 **8/8 PASS**：

```text
[PASS] flag=0x02(ON): ID=0  sync=0xD798 期望 0xD798
[PASS] flag=0x02(ON): ID=1  sync=0xD798 期望 0xD798
[PASS] flag=0x02(ON): ID=37 sync=0xD798 期望 0xD798
[PASS] flag=0x02(ON): ID=99 sync=0xD798 期望 0xD798
[PASS] flag=0: ID=0  sync=0xC368 期望 0xC368
[PASS] flag=0: ID=1  sync=0x61B4 期望 0x61B4
[PASS] flag=0: ID=37 sync=0x0524 期望 0x0524
[PASS] flag=0: ID=99 sync=0x01F4 期望 0x01F4
```

> ⚠️ 全设备 `selftest.py` 本轮**整体结论是 FAIL（1 台设备）**，失败项是 **`x3pro`** 的
> 双发/帧语义断言（属于另一条工作线，与本设备无关）；**v860iiis 段全部 PASS**
> （长度不变、补丁点 58 B、区外 0 处差异、73 项 verify、8 项 Lead 交叉验证）。
> 不要引用为「selftest ALL PASS」。

### 7.5 独立 / 专项逆向报告（另一条工作线交叉复核，2026-11-11）

| 产物 | 内容 | 结果 |
|---|---|---|
| [`re/cfn_settings.py`](re/cfn_settings.py) + [`re/log_cfn_settings.txt`](re/log_cfn_settings.txt)，报告 [`../../keypatch/dev/v860iiis/CFN_SETTINGS.md`](../../keypatch/dev/v860iiis/CFN_SETTINGS.md) | **C.Fn 子系统专项**：镜像身份、settings 基址/字面量/42 条 LDR 引用、12 项最大值表与 clamp、5 个调用点、两个 gate 与 handler 现场、`settings[8]` 读者/写者全集、持久化链路、自动保存倒计时、**Unicorn 三态（原厂 / NOP / BL-hook）handler 级对照**、备选代码洞评估 | **193 PASS / 0 FAIL**（退出码 0） |
| [`re/cfn_menu.py`](re/cfn_menu.py) + [`re/log_cfn_menu.txt`](re/log_cfn_menu.txt)，报告 [`../../keypatch/dev/v860iiis/CFN_MENU.md`](../../keypatch/dev/v860iiis/CFN_MENU.md) | **C.Fn 菜单/UI 逆向**：字形表与索引→字符映射、渲染函数 `0x0800431C`/唯一调用点、10 项显示名与取值、LIGHT 三档、按键路径、200 tick 自动保存、168×96 帧缓冲重建、8 张整帧 ASCII | **158 PASS / 0 FAIL**（退出码 0，日志 2,321 行） |
| [`re/sync_audit.py`](re/sync_audit.py) + [`re/log_sync_audit.txt`](re/log_sync_audit.txt)，报告 [`../../keypatch/dev/v860iiis/SYNC_AUDIT.md`](../../keypatch/dev/v860iiis/SYNC_AUDIT.md) | **平台/同步字函数独立审计**：镜像基址反推（Reset 桩 + `__main` 桩）、内核判定（ARMv6-M）、56 B 逐条复核、50024 唯一性与 5 个调用点（CFG 数据流）、60 个 `WriteReg` 寄存器号解析、初始化表 47 次写 0x00..0x2E、零区/代码洞引用检查、Unicorn 调用级（5 个 ID） | **132 PASS / 0 FAIL**（退出码 0；Unicorn 另 30 项断言 PASS） |
| [`re/recon1.py`](re/recon1.py) + [`re/log_recon1.txt`](re/log_recon1.txt) | 初勘：向量表、架构初判、50024 字面量唯一性、LDR 引用定位 | 见 §1/§2 引用（含 1 条 `bl` 误判提示，已在 §1 更正） |
| [`../../keypatch/dev/v860iiis/NOTES.md`](../../keypatch/dev/v860iiis/NOTES.md) | 补丁规格（6 处补丁点、开关语义、持久化、复现命令、已知风险 7 条） | 与本文档一致；`re/log_cfn_settings.txt` §9 末尾另核对「9/9 个关键地址出现」 |

> `sync_audit.py` 在首轮运行中曾报 6 项 FAIL（均为脚本自身的判定口径问题：uidiv/strobe/delay_us
> 的函数体判定、「r0 未被改写」把另一分支的 `0x08005A60 movs r0,#9` 计入、`0x080162B9` 零区
> 按严格死区口径记 FAIL）；脚本修订后**修订版 132 项全 PASS**，见
> [`SYNC_AUDIT.md`](../../keypatch/dev/v860iiis/SYNC_AUDIT.md) §0。其独立结论与本文档一致，
> 并额外确认 **Lead 初勘的「下一函数 0x080115F4」是笔误，应为 `0x080115F0`**（`0x080115F4`
> 无任何 `BL`/向量目标），本文档已按 `0x080115F0` 书写。

### 7.6 做不到的部分（如实记录）

- **整机模拟器未建立**：V860III-S 没有像 AD100/AD200 那样的整机 Unicorn 模拟机，本轮只做到
  「调用级 + C.Fn handler 级」验证（直接执行同步字函数 / 两个改值 handler），
  启动链、主循环、外设交互、显示刷新都没有回归覆盖；
- **无实机验证**：Unicorn 只能证明「补丁后的机器码按预期写 SYNC1/SYNC0、handler 按预期调用同步字函数」，
  真正的空口同步过滤、C.Fn 菜单显示/持久化、LIGHT 语义、造型灯行为都必须真机验证（见 §8.1）；
- **未做**：Flash/RAM 容量实测、显示控制器逆向、bootloader 行为（本轮不涉及、也不应该动）；
- **未验证 CRC/签名启动校验**：本镜像未见明显的应用 CRC 校验代码，但未穷举；
  若 bootloader 校验固件完整性，任何原位补丁都会失败（与 V1C/AD200 项目同一假设：不校验，
  `CFN_SETTINGS.md` §10.6）。

---

## 8. 待办与风险

### 8.1 实机安全阶梯（建议顺序，风险自负）

1. **原厂基线**：先确认官方 G3 能识别设备、能从官方渠道重下同版本 `V860IIIS_V1.1.bin`
   用于回滚（sha256 必须一致）；
2. **刷补丁版**：刷入 `V860IIIS_V1.1_key<KEY>.bin` 后确认能开机、菜单正常、版本号 V1.1；
3. **LIGHT 菜单确认**：进 C.Fn → 第 3 页第 1 行 `LIGHT`，确认可在 `12sec/OFF/ON` 三档间切换；
   新刷/恢复出厂后应为 **12sec（`settings[8]=0`）**；
4. **普通模式**：LIGHT=12sec（或 OFF），ID=1..99，与原厂引闪器/灯同 CH/ID，确认能正常引闪；
5. **密钥模式**：LIGHT=ON + 已打补丁的引闪器（同一把密钥）→ 能引闪；
6. **隔离验证**：把引闪器切回普通模式 → LIGHT=ON 的灯应**不被引闪**（密钥生效）；
7. **即时生效**：不动其它设置，仅来回切 LIGHT ON↔12sec，确认同步字立即切换；
8. **造型灯（LIGHT 代价）**：确认造型灯输出保持原厂 12sec 档行为（补丁把造型灯任务读者固定为 0），
   且 C.Fn 里改 LIGHT 不再改变造型灯模式；
9. **持久化**：重启设备后 LIGHT 仍为 ON；恢复出厂后回到 12sec（普通模式）；
10. **无线设置同步覆盖检查**：用 G3/引闪器做一次「设置同步/远程设置」，确认 LIGHT 不被清回 0/1
    （补丁已 NOP `0x0800D0A2` 的写点，但运行期动态复制路径需实测）；
11. **ID 行为回归**：改 ID 来回切几次，确认普通模式下每次同步字都跟着变（2 字节 NOP 不破坏原厂 ID 行为）；
12. 任意一步异常 → 立刻刷回 `devices/v860iiis/firmware/V860IIIS_V1.1.bin`
    （详见 [`keypatch/README.md`](../../keypatch/README.md)）。

### 8.2 未确认项 / 已知风险

| 项 | 状态 | 说明 |
|---|---|---|
| 实机验证 | ❌ 未做 | 见 8.1；无整机模拟器 |
| **LIGHT 原厂功能代价** | ⚠️ 已知取舍 | 原厂 LIGHT 三档控制造型灯模式；复用后**造型灯输出固定原厂 12sec 档**（读者被 mask），C.Fn 里 LIGHT 只剩「ON=密钥开关」语义 |
| **无线设置同步覆盖** | ⚠️ 静态已保护、待实机 | 已 NOP `0x0800D0A2`（RF 设置复位写 `settings[8]` 处）；静态写者只有 3 处且其余 2 处语义正确，但运行期动态复制需实测 |
| **恢复出厂会关密钥** | ℹ️ 预期行为 | 出厂默认 `0x08005E54` 把 `settings[8]` 写 0 → 回到普通模式；与「恢复出厂」语义一致，未 patch |
| **升级语义** | ⚠️ 需告知用户 | 原厂 LIGHT 值 `2 = ON` 与补丁「密钥模式」重合：若设备里已保存 LIGHT=ON，刷入补丁后会**自动进入密钥模式**（普通/未打补丁引闪器无法引闪）；出厂默认 `12sec`(0) 为普通模式 |
| **改值即时同步范围** | ℹ️ 说明 | 两个 gate 的 NOP 覆盖 item0..8、10、11 的改值；item9(LCD) 走专用处理，不触发；**光标移动**也会触发（「回绕到 item9」的路径除外），属可接受副作用 |
| LIGHT 菜单显示/导航 | ⚠️ 未实机 | 渲染/帧缓冲证据见 §5.7；物理按键丝印无法从固件恢复，「MENU/SET/+/-」是按功能与同代 UI 推断 |
| 显示/UI（屏幕/触控） | ❌ 未逆向 | 补丁不改 UI；只确认了 168×96 单色帧缓冲与渲染函数，显示控制器本身未逆向 |
| Flash / RAM 容量 | ⚠️ 待确认 | 只确认了向量表与 SP；app 占约 77.4 KB |
| 删 `SCAL+1ms`（及 20 µs） | ⚠️ 理论安全 | 只改同步字不改频率；实机首包异常优先怀疑此处（§6.6） |
| 版本绑定 | ✅ 有防护 | 长度 79,300 B + 6 处补丁点原始字节 + sha256 三重校验，版本不符直接拒绝打补丁 |
| 启动完整性校验 | ⚠️ 未穷举 | 未见明显应用 CRC/签名校验代码，但未穷举；若 bootloader 校验则原位补丁无法启动（`CFN_SETTINGS.md` §10.6） |
| G3 容器 | ✅ 已备 | `--container` 生成 `*_container.bin`（文件名含 `V860IIIS`，G3 才认） |
| 16 位同步字强度 | ⚠️ 非密码学 | 挡的是正常原厂设备误触发；SDR 抓包理论上可克隆 |
| 与 V1C/V860III-C 的差异 | ℹ️ 已记录 | 本机用 2 字节 NOP 而非 10 B 注入 hook（§6.5）；C.Fn 只有 10 项（V1C 12 项） |

### 8.3 可能需要 Lead 决策 / 后续补做项

- [ ] 把本设备状态同步进 [`docs/设备清单.md`](../../docs/设备清单.md)、
      [`docs/新增设备指南.md`](../../docs/新增设备指南.md) §8（新增 V860III-S 小节）——**顶层文档由 Lead 维护**；
- [ ] 实机验证后回填 §7.3/§8.2 的状态，并把产物 sha256 记入 `keypatch/out/` 台账；
- [ ] `selftest.py` 目前因 **x3pro** 失败而整体 FAIL，需对应工作线修复后才能恢复「全设备 ALL PASS」口径；
- [ ] 若想进一步收紧「光标移动也会 sync」的副作用（原厂只在落到 item6 时 sync），
      可评估方案(b) `BL` hook + 在 hook 内加「item==6 或 item==8」白名单判断的变体（需另行设计/验证）；
- [ ] 若换固件版本：必须重跑 `re/cfn_settings.py`/`re/cfn_menu.py` 与
      [`docs/01_静态证据与复现.md`](docs/01_静态证据与复现.md) 的复核清单，再更新
      `keypatch/devices/v860iiis.py` 的 `sha256`/`image`/补丁点。

---

## 9. 文件地图与相关文档

```text
devices/v860iiis/
├─ README.md                       ← 本文（逆向档案 + C.Fn LIGHT 开关 + 补丁布局 + 验证记录）
├─ firmware/
│  ├─ README.md                    ← 固件登记（大小/sha256/来源/校验）
│  └─ V860IIIS_V1.1.bin            ← 原厂固件（本机，*.bin 被 .gitignore 忽略）
├─ re/                             ← 静态分析 / C.Fn 分析 / 菜单逆向 / 独立审计脚本与真实日志
│  ├─ recon1.py                    ← 初勘：向量表 / 架构 / 50024 唯一性 / LDR 引用
│  ├─ log_recon1.txt               ← recon1 的真实输出（UTF-16LE）
│  ├─ cfn_settings.py              ← C.Fn 子系统全量分析（193 项断言，退出码 0）
│  ├─ log_cfn_settings.txt         ← 上述脚本真实输出
│  ├─ cfn_menu.py                  ← C.Fn 菜单渲染/按键 Unicorn 行为验证（158 项断言，退出码 0）
│  ├─ log_cfn_menu.txt             ← 上述脚本真实输出（含 168×96 帧缓冲 ASCII 重建）
│  ├─ sync_audit.py                ← 平台/同步字函数独立审计（132 项，退出码 0）
│  └─ log_sync_audit.txt           ← 上述脚本真实输出（132 PASS / 0 FAIL）
└─ docs/
   ├─ 01_静态证据与复现.md          ← 定位过程、扫描脚本、复核清单
   └─ 02_验证与实机清单.md          ← verify() 分层说明、实机安全阶梯与回滚
```

补丁/审计材料（`keypatch/dev/v860iiis/`）：

| 文件 | 内容 |
|---|---|
| [`NOTES.md`](../../keypatch/dev/v860iiis/NOTES.md) | 补丁规格：6 处补丁点、开关语义、即时生效原理、持久化/读者/写者、复现与验收、已知风险 7 条 |
| [`CFN_SETTINGS.md`](../../keypatch/dev/v860iiis/CFN_SETTINGS.md) | **C.Fn 设置机制专项**：settings 布局/最大值表、gate 与 handler 现场、`settings[8]` 读者/写者全集、持久化与倒计时、Unicorn 三态验证、方案(a)/(b) 对比、风险与未决问题 |
| [`CFN_MENU.md`](../../keypatch/dev/v860iiis/CFN_MENU.md) | **C.Fn 菜单/UI 逆向报告**：字形表、渲染函数与页/槽位、10 项清单与 FEB ACL/TX 反证、按键路径、画面草图、风险 |
| [`SYNC_AUDIT.md`](../../keypatch/dev/v860iiis/SYNC_AUDIT.md) | **平台/同步字函数独立审计报告**：7 条待验证结论逐条 PASS、基址反推、内核判定、56 B 复核、5 个调用点 CFG 数据流、写 reg4/5 唯一性、代码洞负结论、Unicorn 调用级；并记录「下一函数应为 `0x080115F0`」的更正 |

相关：

- 定位过程与复核清单：[`docs/01_静态证据与复现.md`](docs/01_静态证据与复现.md)
- 验证分层与实机安全阶梯：[`docs/02_验证与实机清单.md`](docs/02_验证与实机清单.md)
- 补丁实现：[`keypatch/devices/v860iiis.py`](../../keypatch/devices/v860iiis.py)（`DEVICE` / `apply()` / `verify()`）
- 设备模块接口：[`keypatch/INTERFACE.md`](../../keypatch/INTERFACE.md)
- 刷机/回滚/救砖：[`keypatch/README.md`](../../keypatch/README.md)
- 全仓库设备速查：[`docs/设备清单.md`](../../docs/设备清单.md)
- 新设备接入方法：[`docs/新增设备指南.md`](../../docs/新增设备指南.md)
- 同族设备档案：[`../v1c/README.md`](../v1c/README.md)、[`../v860iiic/README.md`](../v860iiic/README.md)

---

## 10. 版本演进

| 阶段 | 内容 | 补丁形态 | 验证 | 状态 |
|---|---|---|---|---|
| 初勘（2026-11-11 01:54） | `re/recon1.py`：向量表、base、架构初判、50024 唯一性、LDR 引用定位到 `0x080115B8` | — | 见 `re/log_recon1.txt` | ✅ 已完成（其中「32 位指令→M3/M4」为 `bl` 误判，见 §1） |
| 补丁 v1（2026-11-11，现行） | **C.Fn 第 3 页 `LIGHT`（item8）开关**：ON(2)=密钥；12sec(0)/OFF(1)=原厂公式；6 处原位补丁 | 56 B 同步字函数 + 2×2 B gate NOP + 2 读者屏蔽 + 1 写者保护（**无注入、无代码洞**） | `verify()` **73 项 PASS**；Lead harness **8/8 PASS**；`cfn_settings` 193 PASS；`cfn_menu` 158 PASS；`sync_audit` 132 PASS | ✅ 现行（**无实机验证**） |
| 备选方案(b)（仅评估） | 两个 gate 改 `BL` 到 `0x080162BA` 的 16 B hook（类似 V1C/V860III-C） | 4 B gate + 16 B hook，差异 23 B | Unicorn 与方案(a) 行为**完全等价**（`re/log_cfn_settings.txt` §5/§9） | ❌ 未采用（2 字节 NOP 更小、无死区依赖） |

> v1 与 V1C v5-LIGHT 的差异：开关项同为 `LIGHT`、语义同为「ON=密钥」，但
> ①本机型 C.Fn **只有 10 项**（无 FEB ACL/TX）；②即时生效用 **2 字节 NOP** 而非 10 B 注入 hook；
> ③地址整体不同（同步字函数 `0x080115B8`、settings `0x20001058`、最大值表 `0x08016102`、
> 渲染函数 `0x0800431C`）；④固件身份与 sha256 完全不同（79,300 B vs 88,032 B）。
