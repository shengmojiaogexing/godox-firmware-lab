# V860III-C V1.3 私有密钥「C.Fn LIGHT 开关」补丁 —— 推导与验证记录

> 设备模块：[`../../../keypatch/devices/v860iiic.py`](../../../keypatch/devices/v860iiic.py)（`DEVICE` / `apply()` / `verify()`）
> 原厂固件：`devices/v860iiic/firmware/V860IIIC_V1.3.bin`（87,612 B，sha256 `8530a7e8…`，未改动）
> 关联文档：[`CFN.md`](CFN.md)（C.Fn 子系统专项）、
> [`../../../devices/v860iiic/README.md`](../../../devices/v860iiic/README.md)（设备总览）
> 环境：Windows / PowerShell；`tools/python/python.exe`（capstone 5.0.7 / unicorn 2.1.4）。日期：2026-11-11。

---

## 0. 结论速览

| 项 | 结果 |
|---|---|
| 设备 | Godox **V860III-C**（闪光灯，Cortex-M0 / Thumb-1，CC2500 兼容射频） |
| 固件 | `V860IIIC_V1.3.bin`，**87,612 B**，sha256 `8530a7e85ce1098d64d72d7ea4a28f3fdfb489e3d39da49be1ffa166f6ba270d`，基址 `0x08003000` |
| 用户语义 | **C.Fn item 8 `LIGHT` = ON（值 2）→ SYNC = 私有 KEY；12sec(0)/OFF(1) → 原厂 `50024/(ID+1)`** |
| 开关字节 | `settings[8]` = RAM **`0x20001092`**（随原厂 C.Fn 设置持久化） |
| 补丁形态 | **7 处原位补丁，输出长度不变（87,612 B）** |
| 与原厂差异 | **71 字节**，全部落在 7 处补丁点内（`verify()` 集合断言） |
| 离线验证 | 模块 `verify()` **35 项 PASS** / 0 FAIL；Lead `call_harness.py` 交叉验证 **PASS**（key=0x6B21） |
| 整机模拟器 | **未建立** |
| 实机验证 | **未做** |

---

## 1. 用户语义（指针版）

```c
/* 0x080122E8 void rf_set_sync(uint8_t id) */
flag = *(uint8_t *)0x20001092;        // C.Fn LIGHT：0=12sec / 1=OFF / 2=ON
if (flag == 2) sync = KEY;            // 密钥模式（只有打了同一把密钥的引闪器能引闪）
else           sync = 50024 / (id + 1); // 原厂普通模式
WriteReg(4, sync >> 8);               // CC2500 SYNC1
WriteReg(5, sync & 0xFF);             // CC2500 SYNC0
return;                               // 只改同步字：无 SCAL、无延时
```

| C.Fn 第 3 页第 1 行 LIGHT | `settings[8]` | 同步字 | 模式 |
|---|---|---|---|
| **ON（值 2）** | `2` | **私有 KEY** | **密钥模式** |
| **12sec（值 0，出厂默认）** | `0` | `50024/(ID+1)` | 原厂普通模式 |
| OFF（值 1） | `1` | `50024/(ID+1)` | 原厂普通模式 |

---

## 2. 补丁点推导过程

### 2.1 先找到同步字函数（同族经验 + 50024 字面量）

V860III-C 与已接入的 V1C 同族（同为 `0x08003000` base 的 Cortex-M0 灯）。定位按
[`../../../docs/新增设备指南.md`](../../../docs/新增设备指南.md) §3.1 的标准做法：

1. 搜小端字面量 `68 c3 00 00`（= 50024 = 0xC368）；
2. 找到引用它的函数；反汇编确认
   `adds r1,r0,#1` → `bl __aeabi_uidiv` → 两次 `WriteReg(4/5)` →
   `delay(20)` → `strobe(0x33)` → `delay(1000)`；
3. 结果：字面量在 `0x0801231C`，函数入口 **`0x080122E8`**，下一函数 **`0x08012320`**，
   函数体 **56 字节**；全镜像只有这一处 `0xC368` 字面量；
4. 全镜像 `WriteReg` 调用点扫描：只有本函数（`0x080122FA`/`0x08012302`）与
   CC2500 初始化表（`0x080155BE`/`0x080155C6`，写缺省 `0xC3/0x68`）写寄存器 4/5；
   初始化表的值随后被 RF 初始化（`0x080066FC`）里的本函数调用覆盖；
5. `BL -> 0x080122E8` 共 5 处（`0x08006324` / `0x08006762` / `0x0800D554` /
   `0x0800E1A8` / `0x0800E23C`），调用前都 `ldrb r0,[rX,#6]`（取 `settings[6]` = ID）。

→ 只改这一个函数即可覆盖全部「设置同步字」路径，不需要 hook 初始化路径。

### 2.2 再找开关字节（C.Fn / settings 子系统）

用 `re/cfn.py` 的 settings 污点扫描（证据：[`../../../devices/v860iiic/re/log_cfn.txt`](../../../devices/v860iiic/re/log_cfn.txt)）：

- settings 基址 = **`0x2000108A`**（全镜像 51 处字面量引用）；
- item 数 = 12（`0..0xB`），最大值表 **`0x080181AA`** =
  `01 01 01 01 01 1f 63 01 02 06 01 01`，与 V1C 12 项表**逐字节同值**；
- C.Fn 渲染函数 `0x080048B0..0x0800538F` 里：
  `0x08004E46 cmp r0,#6`（ID）、`0x08004F92 cmp r0,#8`（LIGHT）、`0x08005054 cmp r0,#9`（LCD）；
- LIGHT 渲染块按 `settings[8]` 选三档字形（`2*值+4` → 0/1/2 对应字形 4/6/8）；
- `settings[8]` = **`0x20001092`**，全部访问点见 [CFN.md](CFN.md) §3。

### 2.3 gate 与代码洞

- 原厂只有「改 ID（item6）」才立即调用同步字函数：
  - 递减处理 `0x0800627C` 内 `0x0800630C`：`06 29 08 d0`（`cmp r1,#6; beq sync`）；
  - 递增处理 `0x0800D4A8` 内 `0x0800D53E`：`06 29 07 d0`；
  - 两处 gate 前数据流解析出 `r0 = 0x2000108A`（settings 基址）；
- 代码洞扫描（全 0 段 + 引用检查）：**`0x080179C2` 起 234 B 连续 0x00**
  （到 `0x08017AAB`），无 BL/分支/LDR 字面量/绝对地址引用 → 用作注入区；
  备用洞 `0x08017C3A`（222 B）。

---

## 3. 补丁规格（7 处原位补丁）

### 3.1 清单

| # | 位置 | 类型 | 原厂 | 补丁后 | 作用 |
|---|---|---|---|---|---|
| ① | `0x080122E8..0x0801231F`（56 B） | 整函数改写 | `ORIG_FUNC` 56 B | 44 B 代码 + KEY/FLAG/50024 | 读 LIGHT 开关，写 SYNC1/SYNC0 |
| ② | `0x080179C2..0x080179CB`（10 B） | 注入代码 | 10 × `0x00` | `1f b5 80 79 fa f7 8f fc 1f bd` | C.Fn 改值后按 ID 重设同步字 |
| ③ | `0x0800630C..0x0800630F` | gate | `06 29 08 d0` | `11 f0 59 fb`（BL 注入） | C.Fn 递减改值 → 重设同步字 |
| ④ | `0x0800D53E..0x0800D541` | gate | `06 29 07 d0` | `0a f0 40 fa`（BL 注入） | C.Fn 递增改值 → 重设同步字 |
| ⑤ | `0x080036AE..0x080036AF` | 读者屏蔽 | `00 7a` | `00 20` | 造型灯任务 LIGHT 读者 1 固定 0 |
| ⑥ | `0x080036D6..0x080036D7` | 读者屏蔽 | `01 7a` | `00 21` | 造型灯任务 LIGHT 读者 2 固定 0 |
| ⑦ | `0x0800A7EA..0x0800A7EB` | 写点保护 | `08 72` | `00 bf`（nop） | 无线设置同步解包不再覆盖 settings[8] |

> 注：③④ 的 `BL` 机器码（`11 f0 59 fb` / `0a f0 40 fa`）随目标地址 `0x080179C2` 固定，
> 与密钥无关；① 的内容随 KEY 变化。

### 3.2 同步字函数布局（相对 `0x080122E8`）

| 偏移 | 长度 | 内容 | 说明 |
|---|---|---|---|
| `+0x00` | 2 | `push {r4, lr}` | 保护现场 |
| `+0x02` | 2 | `ldr r3, =FLAG` | 字面量在 `+0x30`（`0x20001092`） |
| `+0x04` | 2 | `ldr r2, =50024` | 字面量在 `+0x34`（原厂地址、原值不变） |
| `+0x06` | 2 | `ldrb r3, [r3]` | r3 = LIGHT 值（0/1/2） |
| `+0x08` | 2 | `adds r1, r0, #1` | 普通模式除数 = ID+1 |
| `+0x0A` | 2 | `cmp r3, #2` | LIGHT == ON(2)？ |
| `+0x0C` | 2 | `bne go` | 否（12sec/OFF）→ 原厂公式（跳到 `+0x12`） |
| `+0x0E` | 2 | `ldr r2, =KEY` | 密钥模式：字面量在 `+0x2C` |
| `+0x10` | 2 | `movs r1, #1` | 密钥模式除数 = 1 |
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
| `+0x2C` | 4 | `.word KEY` | 私有密钥（`0x08012314`） |
| `+0x30` | 4 | `.word FLAG` = `0x20001092` | C.Fn LIGHT 开关字节（`0x08012318`） |
| `+0x34` | 4 | `.word 50024` | **原厂字面量，同址同值**（`0x0801231C`） |

以 key = `0x6B21` 为例，补丁后 56 字节：

```text
10 b5 0b 4b 0b 4a 1b 78 41 1c 02 2b 01 d1 07 4a
01 21 10 46 f0 f7 f8 fe 04 46 01 0a 04 20 03 f0
72 fa e1 b2 05 20 03 f0 6e fa 10 bd 21 6b 00 00
92 10 00 20 68 c3 00 00
```

补丁后反汇编（节选，字面量行按 capstone 线性解码显示为指令）：

```text
0x080122E8  10 b5        push {r4, lr}
0x080122EA  0b 4b        ldr  r3, [pc, #0x2c]   ; =0x20001092（LIGHT）
0x080122EC  0b 4a        ldr  r2, [pc, #0x2c]   ; =50024
0x080122EE  1b 78        ldrb r3, [r3]
0x080122F0  41 1c        adds r1, r0, #1
0x080122F2  02 2b        cmp  r3, #2
0x080122F4  01 d1        bne  #0x080122FA
0x080122F6  07 4a        ldr  r2, [pc, #0x1c]   ; =KEY
0x080122F8  01 21        movs r1, #1
0x080122FA  10 46        mov  r0, r2
0x080122FC  f0 f7 f8 fe  bl   #0x080030F0       ; __aeabi_uidiv
0x08012300  04 46        mov  r4, r0
0x08012302  01 0a        lsrs r1, r0, #8
0x08012304  04 20        movs r0, #4
0x08012306  03 f0 72 fa  bl   #0x080157EE       ; WriteReg(4, SYNC1)
0x0801230A  e1 b2        uxtb r1, r4
0x0801230C  05 20        movs r0, #5
0x0801230E  03 f0 6e fa  bl   #0x080157EE       ; WriteReg(5, SYNC0)
0x08012312  10 bd        pop  {r4, pc}
```

### 3.3 注入代码

```text
0x080179C2  1f b5        push {r0, r1, r2, r3, r4, lr}
0x080179C4  80 79        ldrb r0, [r0, #6]          ; 入口 r0 = settings 基址(0x2000108A) → r0 = ID
0x080179C6  fa f7 8f fc  bl   0x080122E8            ; 同步字函数（内部读 settings[8] 开关）
0x080179CA  1f bd        pop  {r0, r1, r2, r3, r4, pc}  ; 原样恢复 r0..r4，返回 handler
```

- 注入区原厂 10 字节全 `0x00`，`apply()` 先校验再写；
- 只用前 10 B，洞余量 224 B。

### 3.4 差异统计

| 补丁点 | 变化字节 |
|---|---|
| ① 同步字函数 56 B 区 | **48 B**（其余 8 B 恰与原厂相同，含 `+0x34` 的 50024） |
| ② 注入 10 B | **10 B** |
| ③④ 两个 gate | **4 + 4 = 8 B** |
| ⑤ 读者 1 | **1 B**（只改 `7a → 20`） |
| ⑥ 读者 2 | **2 B**（`01 7a → 00 21`） |
| ⑦ 解包写点 | **2 B**（`08 72 → 00 bf`） |
| **合计** | **71 B**，全部在原厂差异集合内（`verify()` 断言「差异之外 0 处」） |

---

## 4. C.Fn 菜单证据（摘要）

详见 [`CFN.md`](CFN.md)。关键点：

- **item 8 = LIGHT**：渲染函数 `0x080048B0..0x0800538F` 内 `0x08004F92 cmp r0,#8`，
  右栏按 `settings[8]` 选三档字形（`0x08005012 ldrb r0,[r7,#8]` → `lsls #1` → `adds #4` → 绘制）；
- **两个功能读者**在造型灯任务 `0x080036A8..0x0800379F`（`0x080036AE` / `0x080036D6`），
  与 V1C 的 `0x080036D4..0x080037CB` 逐条同构（仅 BL 目标不同）；
- 出厂默认 `settings[8]=0`（`0x080067DC`），12sec = 普通模式；
- 持久化：save `0x08005FA0` 读 `settings[8]`（`0x080060DE`）写记录 `+0x34`；
  load `0x080159D0` 从 `+0x34` 展开回 `settings[8]`（`0x08015B10`）；
- 补丁**不改 UI**：不新增菜单项/字库，只复用原厂 LIGHT 项；造型灯输出固定 12sec 档。

> 本设备未单独做 UI 报告（V860III-C 的显示/触控未逆向）：菜单文本「12sec/OFF/ON」按
> V1C 同构推断，**实机确认项**见 [`../../../devices/v860iiic/docs/02_验证与实机清单.md`](../../../devices/v860iiic/docs/02_验证与实机清单.md)。

---

## 5. Unicorn / 调用级验证记录

### 5.1 模块 `verify()`：35 项

| 层 | 项数 | 内容 |
|---|---|---|
| 静态-函数 | 4 | 长度 / 56 B 函数体 / 边界 / 三个字面量 |
| 静态-补丁点 | 6 | 注入 / 2 gate / 3 mask |
| 静态-差异范围 | 2 | 差异集合 == 71 B 且之外 0 处 |
| Unicorn-补丁版 | 14 | 说明 1 + flag=2（ID 0/5/99）+ flag=0（ID 0/1/5/37/99）+ flag=1（同 5 个） |
| Unicorn-注入 hook | 4 | item=6/8/5/11 → sync 各 1 次、r0=ID、r0..r4 恢复 |
| Unicorn-原厂自证 | 5 | 原厂 56 B：ID 0/1/5/37/99 → 原厂公式 + `delay(20)+SCAL(0x33)+delay(1000)` |

关键输出（key = `0x6B21`，完整 35 行见设备 README §7 与 `docs/02` §1.2）：

```text
[PASS] 字面量: KEY=0x6B21 @0x08012314, FLAG=0x20001092 @0x08012318, 50024=0x0000C368 @0x0801231C
[PASS] 与原厂差异 71 字节 ... [PASS] 差异之外无任何字节变化（0 处）
[PASS] ID=0  flag=2 密钥模式(LIGHT=ON): sync=0x6B21 -> SYNC1=0x6B SYNC0=0x21
[PASS] ID=99 flag=1 原厂公式: sync=0x01F4 -> SYNC1=0x01 SYNC0=0xF4
[PASS] 注入 hook: item=8 -> sync 调用 [37]（期望 1 次, r0=ID=37），返回 r0=0x2000108A r1=8
[PASS] ID=37 原厂(自证): ... -> delay(20) -> WriteCmd(0x33) -> delay(1000)
```

### 5.2 Lead 独立交叉验证 `keypatch/dev/lead/call_harness.py`

独立于模块 `verify()` 的 harness（`v860iiic` 条目：`base=0x08003000`、`entry=0x080122E8`、
`write_reg=0x080157EE`、`strobe=0x080158B0`、`delay=0x08015560`、`sp=0x20001400`、`arch="m0"`），
固定 key=`0x6B21`，逐 ID 对比「原厂 / 开关 ON / 开关 OFF」：

```text
设备 v860iiic  镜像 V860IIIC_V1.3.bin  (87612 B)
  ID=0  原厂 sync=0xC368 (期望 0xC368) | 开关ON sync=0x6B21 (期望 0x6B21) | 开关OFF sync=0xC368 (期望 0xC368) PASS
  ID=1  原厂 sync=0x61B4 | 开关ON sync=0x6B21 | 开关OFF sync=0x61B4 PASS
  ID=37 原厂 sync=0x0524 | 开关ON sync=0x6B21 | 开关OFF sync=0x0524 PASS
  ID=99 原厂 sync=0x01F4 | 开关ON sync=0x6B21 | 开关OFF sync=0x01F4 PASS
```

### 5.3 本轮没有做的验证

- 没有跑 `keypatch/selftest.py` 的全设备汇总（本设备模块级 `verify()` + Lead harness 已单独通过）；
- **没有整机模拟器**：启动链/主循环/外设没有回归；
- **没有实机**：空口同步过滤、C.Fn 菜单显示/持久化、造型灯行为都必须真机验证。

---

## 6. 风险与取舍

| 项 | 状态 | 说明 |
|---|---|---|
| 实机验证 | ❌ 未做 | 见设备 `docs/02` 的安全阶梯 |
| **唯一的射频差异** | ⚠️ 理论安全 | 只改同步字、不改频率寄存器；但原厂的 `delay(20)` + `SCAL(0x33)` + `delay(1000)` **全部去掉**。若实机「偶发首包丢失/配对不稳」，优先怀疑这里（回退方案：保留 strobe/延时的变体） |
| **LIGHT 造型灯功能被复用** | ⚠️ 已知取舍 | 两个读者固定 0 → 造型灯输出固定原厂 12sec 档；C.Fn 里 LIGHT 只剩「ON = 密钥开关」语义 |
| 设置同步覆盖 | ⚠️ 部分保护 | 解包写点 `0x0800A7EA` 已 NOP；另有一处整体写路径（`0x0800E18A`）未屏蔽，需实测 |
| 恢复默认会关密钥 | ℹ️ 预期 | 默认值函数把 settings[8] 置 0 → 回普通模式 |
| 升级语义 | ⚠️ 需告知 | 原厂 `LIGHT=ON` 与补丁「密钥模式」重合：设备里若已保存 LIGHT=ON，刷入补丁后会自动进密钥模式 |
| 16 位同步字强度 | ⚠️ 非密码学 | 挡的是正常原厂设备误触发；SDR 抓包理论上可克隆 |
| 版本绑定 | ✅ 有防护 | 长度 + 7 处补丁点的原始字节（函数 56 B、注入 10 B、2 gate、3 mask）+ 50024 唯一性，任一不符直接拒绝打补丁 |

---

## 7. 复现命令

```powershell
# 仓库根
tools\python\python.exe devices\v860iiic\re\cfn.py            # 7 项断言，退出码 0
tools\python\python.exe devices\v860iiic\re\compare.py        # 对照反汇编
tools\python\python.exe keypatch\keypatch.py patch --device v860iiic --key 0x6B21 --verify --out %TEMP%\kp_v860iiic
tools\python\python.exe keypatch\dev\lead\call_harness.py     # 独立调用级交叉验证
```

> 说明：`devices/v860iiic/re/fwlib.py` 是从 `devices/v1c/re/audit_static.py` 裁剪出的
> **通用分析库**（镜像/地址常量 + `Code` 反汇编/数据流辅助），V1C 专用的补丁点复核主体
> 已删除；本设备的结论只以 `re/cfn.py`、`re/compare.py` 与模块 `verify()` 为准。

---

## 8. 相关文件

| 文件 | 内容 |
|---|---|
| [`CFN.md`](CFN.md) | C.Fn 子系统专项（12 项表、settings 读写点、读者屏蔽、注入 hook） |
| [`../../../devices/v860iiic/README.md`](../../../devices/v860iiic/README.md) | 设备总览、补丁布局、验证记录、实机待办 |
| [`../../../devices/v860iiic/docs/01_静态证据与复现.md`](../../../devices/v860iiic/docs/01_静态证据与复现.md) | 定位过程与可复现脚本 |
| [`../../../devices/v860iiic/docs/02_验证与实机清单.md`](../../../devices/v860iiic/docs/02_验证与实机清单.md) | 35 项分层说明 + 实机安全阶梯 |
