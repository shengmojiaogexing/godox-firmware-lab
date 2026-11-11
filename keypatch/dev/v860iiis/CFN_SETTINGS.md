# Godox V860III-S（V860IIIS_V1.1.bin）C.Fn 设置机制 / 密钥开关 hook / settings[8] 读者屏蔽

> task-2 / **cfn-hook**；写范围 `devices/v860iiis/re/`、`keypatch/dev/v860iiis/CFN_SETTINGS.md`。
> 对象：`devices/v860iiis/firmware/V860IIIS_V1.1.bin`（**79300 B**，sha256 `bdc8f3547808d121f15a6c58b9363c5792c957c2dce2a16b203276ab078d0b20`，
> 基址 **0x08003000**，镜像末端 0x080165C4；**全程只读，未改动固件**）。
> 对应 V1C V2.0 的同平台实现 `keypatch/devices/v1c.py` v5「C.Fn LIGHT 开关」方案。
> 结论先行：**本镜像不需要代码洞**。两个 C.Fn 改值 handler 各有一处
> `cmp r1,#6; bne skip; ldrb r0,[r0,#6]; bl 0x080115B8` gate，把 **bne 改成 nop（2 B × 2 处）**
> 就使「除 item9(LCD) 外的任意 C.Fn 项改值/导航」立即落入既有同步字调用——方案 (a) 已被 Lead 采纳。
> 配套 3 处保护：LIGHT(settings[8]) 造型灯任务的两处读者屏蔽、RF 设置复位分支写 settings[8] 的 NOP。
> 全部结论由 Capstone 静态数据流 + Unicorn 2.1.4 真实执行 handler 验证（193 PASS / 0 FAIL，见 `log_cfn_settings.txt`），
> **未做实机验证**。

## 0. 一句话结论

| 问题 | 结论 |
|---|---|
| settings 基址 / 项布局 | **0x20001058**（30 处 u32 字面量、42 条 LDR 引用）；item n = base+n；item6=ID=0x2000105E；item8=LIGHT=0x20001060 |
| 最大值/回绕表 | **0x08016102** 12 字节 `01 01 01 01 01 1F 63 01 02 06 01 01`（与 V1C 完全相同）；3 处 LDR 引用（0x08005A6A / 0x0800C586 / 0x0800F086） |
| 可编辑项范围 | 菜单索引 0..9（10 项；wrap 代码 `cmp #9`/回 0）。item10/11 只在 settings/记录里，**无任何功能读者**（仅持久化）；item6=ID、item8=LIGHT 均在可达范围 |
| 同步字函数 | **0x080115B8..0x080115EF（56 B，下一函数 0x080115F0）**；字面量 50024 @0x080115EC；5 个 BL 调用点全部以 settings[6] 为入参 |
| 改值 handler | 递减 **0x080059E4**（键事件分发 0x080058E4 事件 0x78）、递增 **0x0800C528**（事件 0xB4）；无函数入参，gate 现场 r0=settings 基址、r1=项索引（来自 RAM **0x200004D6**） |
| gate 角色 | `cmp r1,#6` → 只有 **item6(ID)** 改值才调同步字函数；其它项被 bne 跳过 |
| 首选开关项 | **item8 = LIGHT**（max=2，0=12sec / 1=OFF / 2=ON；出厂默认 0=普通模式）；仅 2 个功能读者（同一造型灯任务 0x08003950），屏蔽成本 2×2 B |
| 立即生效补丁（★） | **0x08005A74 `02 d1`→`00 bf`**、**0x0800C5BE `f5 d1`→`00 bf`**（方案 a，2 B×2，无代码洞） |
| 读者屏蔽 | **0x08003956 `00 7a`→`00 20`**、**0x0800397E `01 7a`→`00 21`**（造型灯任务固定按 settings[8]=0 运行，LIGHT=2 不再点亮造型灯） |
| 写者保护 | **0x0800D0A2 `26 72`→`00 bf`**（RF 设置复位分支不再把 settings[8] 清 0；该 strb 不改标志位、后续无 settings[8] 读取，安全） |
| 持久化 | settings[8] 随 **72 B(0x48) 记录**写入 flash：save builder **0x080056D8** 把 s8 放 **record+0x36**；save task **0x0800D9C8** → **0x08010D94(slot)** → 0x080093A0 写 **0x0801F000+slot*0x64**（20 槽，头字节 **0x5C**）；上电 load task **0x0800EEB0** 校验头后由 **0x08013B08** 展开回 settings[0..0xD] |
| 出厂默认 | 0x08005E54 写 settings[8]=**0**（→普通模式）；load task 无有效记录时也调它 |
| 自动保存 | handler 入口 0x080109C8 置倒计时 0x20000516=0xC8；0x080109A4 递减，归零置 0x20000518=1；save task 消费后落盘（只影响“何时写”，不影响 settings[8] 内容） |
| 代码洞 | 无需；方案 (b) BL 注入的 16 B hook 语义已实测与 NOP 等价，但本镜像**无可证死区**（唯一大零区 0x080162B9..0x08016328 在数据段尾部，不能证明无人用） |

## 1. 方法与可复现

| 文件 | 作用 |
|---|---|
| `devices/v860iiis/re/cfn_settings.py` | 本任务分析/验证脚本：静态扫描 + Capstone 前向污点 + 三态 Unicorn handler 执行；退出码 0 = 全部断言 PASS |
| `devices/v860iiis/re/log_cfn_settings.txt` | 真实运行日志（220 行，PASS 193 / FAIL 0） |
| `keypatch/dev/v860iiis/CFN_SETTINGS.md` | 本报告 |

```powershell
cd D:\神牛
$env:PYTHONIOENCODING='utf-8'
tools\python\python.exe devices\v860iiis\re\cfn_settings.py    # 退出码 0
```

脚本逐项断言：固件身份、settings/最大值表字面量位置与 LDR 引用集合、两个 handler 的
gate 原字节、settings[8] 全部静态读者/写者（污点扫描结果与预期集合逐地址比对）、持久化链路
（save builder/commit/flash 写/load task/展开函数/默认函数/clamp）字节、自动保存倒计时、
两套补丁方案的差异范围、备选代码洞引用检查，以及 **Unicorn 三态（原厂 / NOP / BL-hook）
handler 级行为**。对固件只读。

## 2. settings 区、项布局与最大值表

- settings 基址 **0x20001058**：全镜像 30 处 u32 字面量、42 条 `ldr Rt,[pc,#imm]` 引用（V1C 为 51 处，
  本镜像代码更小）。item n = 0x20001058 + n。
- 最大值/回绕表 **0x08016102** = `01 01 01 01 01 1F 63 01 02 06 01 01`，12 项
  max = 1,1,1,1,1,31,99,1,2,6,1,1；三处引用：递减 handler 0x08005A6A、递增 handler 0x0800C586、
  上限 clamp 0x0800F086（循环 `cmp r0,#0xC`，即 12 项）。
- 项语义（与 V1C 平台同构，且本镜像行为可交叉验证）：
  item0 m/ft、item1 AF、item2 STBY、item3 RX STBY、item4 SCAN、item5 CH(max31)、
  **item6 ID(max99，改值即调用 0x080115B8 写同步字)**、item7 BEEP(max1)、
  **item8 LIGHT(max2，造型灯模式 0=12sec/1=OFF/2=ON)**、item9 LCD(max6，专用函数 0x080128F0)、
  item10/11（本镜像未启用，见下）。
- **本镜像独有发现（与 V1C 的差异）**：菜单可编辑索引只有 **0..9**——递减 handler 在索引 0 回绕到 9
  （0x08005A60 `movs r0,#9`），递增 handler 在 >9 时回 0（0x0800C5A6..0x0800C5AA）。
  同时静态污点扫描显示 **settings[10]/[11] 除 save builder 读取与 load 展开写入外，无任何功能读者**
  （settings[12]/[13] 同）。即：12 字节上限表/14 字节记录是平台结构，**实际 C.Fn 菜单 10 项 0..9**；
  这不影响 LIGHT(8)/ID(6)，但报告与 keypatch 文档不应宣称本机有 12 个可编辑项。
- 每项全部静态访问点（`devices/v860iiis/re/log_cfn_settings.txt` §6 有断言；完整清单）：

```
item0  R 0x08004440 0x08004450 0x080057EE 0x08006D12 0x0800D100 0x08013F98
       W 0x08005EA6 0x0800D098 0x0800D0F4 0x0800D108 0x08013C20
item1  R 0x08003640 0x080044BA 0x080044CA 0x080057F2      W 0x08005EA8 0x0800D09A 0x08013C24
item2  R 0x0800455E 0x0800456E 0x080057FA 0x08011948      W 0x08005EAA 0x0800D09C 0x08013C2C
item3  R 0x0800464C 0x0800465C 0x080057FE 0x08011994      W 0x08005EAC 0x0800D09E 0x08013C30
item4  R 0x08004734 0x08004744 0x08005802 0x0801148A      W 0x08004D2E 0x08005EB4 0x0800D0A6 0x0800EB0A 0x08013C34
item5  R 0x08004860 0x08004874 0x08005806 0x0800593A 0x0800EA52  W 0x08005EBA 0x0800D0AC 0x08013C38
item6  R 0x0800474E 0x080048C6 0x080048DE 0x0800580E 0x08005E34 0x0800D0B8 0x0800D0E4
       W 0x08005EB6 0x0800D0A8 0x08013C40
item7  R 0x08003D6A 0x080049AC 0x080049BC 0x08005812 0x08007AB4 0x08007DE4 0x080080A8
         0x080084E4 0x080085A2 0x0800F7D8 0x0800F900
       W 0x08005EAE 0x0800D0A0 0x0800F5D8 0x08013C44
item8  R 0x08003956 0x0800397E 0x08004A8A 0x08004A9A 0x08005816      ← 本补丁对象
       W 0x08005EB0 0x0800D0A2 0x08013C48
item9  R 0x08004AD2 0x08004AFE 0x08004B26 0x0800581A 0x080128F6 0x0801294A
       W 0x08005EB2 0x0800D0A4 0x08013C4C
item10 R 0x08005822（save）                     W 0x08013C54（load）
item11 R 0x08005826（save）                     W 0x08013C58（load）
item12 R 0x0800582A（save）                     W 0x08013C5C（load）
item13 R 0x0800582E（save）                     W 0x08013C60（load）
动态索引 [settings, reg]：
       R 0x08005A42(W 0x08005A4A)  ← 递减 handler
       R 0x0800C588(W 0x0800C58E, W 0x0800C59A)  ← 递增 handler
       R 0x0800F08C(W 0x0800F094)  ← settings 上限 clamp
```

### 2.1 为什么选 item8(LIGHT) 而不是 BEEP 等其它项

| 候选 | max/默认 | 功能读者 | 结论 |
|---|---|---|---|
| **item8 LIGHT** | 2 / **0** | **2**（同一造型灯任务 0x08003950，GPIO 0x48001000） | **首选**：默认 0=普通模式；值 2=ON 语义直观；造型灯不参与引闪/射频；2 处 2 B movs 即可完全屏蔽 |
| item7 BEEP | 1 / **1** | 1 个应用 + 10+ 处 UI/按键/显示读者 | 默认 1（出场即“密钥模式”需反相），读者多、屏蔽成本高，不选 |
| item2 STBY | 1 / 0 | 1（待机计时） | V1C 曾评估备选；但本镜像菜单 10 项、STBY 与电源行为相关，LIGHT 更局部且与 V1C v5 交付语义一致 |
| item4 SCAN | 1 / 0 | 扫描/设置应用路径会写回 settings[4]=0 | 会被自动清，不可用 |
| item6 ID / item5 CH / item9 LCD | 99 / 31 / 6 | — | 非 0/1 语义或与开关冲突，不选 |

结论：**沿用 V1C v5 的 item8=LIGHT（值 2=ON=密钥模式，0/1=原厂普通模式）**，语义与 set 一致。

## 3. C.Fn 改值 handler 与 gate（进入参数语义）

- 键事件分发函数 **0x080058E4**：事件码 0xB4 → `bl 0x0800C528`（递增，0x08005928），
  事件码 0x78 → `bl 0x080059E4`（递减，0x0800592E）。两个 handler 都无函数入参。
- handler 运行现场（gate 处）：
  - **r0 = settings 基址 0x20001058**：递减 handler 在 0x08005A26 `ldr r0,[pc,#0x330]`
    （字面量池 0x08005D58），递增 handler 在 0x0800C56A `ldr r0,[pc,#0x36c]`（池 0x0800C8D8）。
  - **r1 = 当前 C.Fn 项索引**：从 RAM **0x200004D6** 读取（递减 0x08005A38 载入指针、0x08005A70
    `ldrb r1,[r2]`；递增 0x0800C57C 载入指针、0x0800C5BA `ldrb r1,[r1]`），**不是函数入参**。
  - 编辑路径的 RAM 标志：`0x200004D4==1`（C.Fn 编辑态）、`0x200004D5==1`（值编辑；=0 为光标移动）、
    `0x20000536==0`（忙标志=1 时 handler 提前返回）、`0x20000563!=3`。
- gate 逻辑（两个 handler 相同形态）：

```
   递减 0x08005A6A: ldr  r3,=max_table(0x08016102)   ; 回绕时取 max
   0x08005A70: ldrb r1,[r2]        ; r1 = 当前项索引(0x200004D6)
   0x08005A72: cmp  r1,#6          ; 06 29
   0x08005A74: bne  0x08005A7C     ; 02 d1  <-- 非 item6 直接返回（不调 sync）
   0x08005A76: ldrb r0,[r0,#6]     ; 80 79  r0 = settings[6] = ID
   0x08005A78: bl   0x080115B8     ; 同步字函数
   0x08005A7C: pop  {r3-r7,pc}

   递增 0x0800C5BC: cmp  r1,#6      ; 06 29
   0x0800C5BE: bne  0x0800C5AC     ; f5 d1  <-- 非 item6 直接返回
   0x0800C5C0: ldrb r0,[r0,#6]     ; 80 79
   0x0800C5C2: bl   0x080115B8
```

- **item9(LCD) 原厂就被排除在 gate 之前**（与 V1C 一致）：递减在 0x08005A4E/0x08005A66
  `cmp r1,#9; beq 0x08005A7E` → `bl 0x080128F0`（LCD 专用函数）；递增在 0x0800C596/0x0800C5B6
  `cmp r2,#9; beq 0x0800C5C8`。因此方案 (a) 的 NOP **不会**把 item9 纳入 sync，行为与原厂一致。
- 回绕路径同样在改值后到达 gate（递减 0x08005A66→0x08005A70；递增 0x0800C596→0x0800C5AE/0x0800C5BA），
  所以 NOP 后“回绕到 0/max”的改值也会立即重设同步字。
- 光标移动分支（0x200004D5==0）修改的是索引而不是 settings，最后也会经过 gate；**原厂在“新索引==6”时
  本来就会 sync**（Unicorn 实测：递减原索引 7、递增原索引 5 时原厂 sync 1 次）。NOP 只是把触发范围
  扩到所有非 9 项——这是已知副作用（见 §9 风险），对“只写 SYNC1/SYNC0、无 SCAL/延时”的改写版
  同步字函数开销可忽略。

## 4. 补丁点表格（★正式方案 a：2 字节 NOP，无需代码洞）

| # | 绝对地址 | 原字节 | 建议新字节 | 语义 | Unicorn 验证结论 | 风险 |
|---|---|---|---|---|---|---|
| **1** | **0x08005A74** | `02 d1`（bne 0x08005A7C） | `00 bf`（nop） | 递减 handler：任意非 item9 项改值/回绕/光标移动都落入既有 `ldrb r0,[r0,#6]; bl 0x080115B8` | 原厂：item6 sync 1 次、其它 0 次；NOP 后：item0..8/10/11 sync 1 次且 r0==settings[6]，item9 仍 0 次并调 0x080128F0；SP/r3-r7 恢复；非改动路径（0x20000536=1）0 次 sync、settings 不变 | 极低；item==6 原行为完全保留（fall-through 后仍是同一条 sync 调用）；无代码洞、无字节长度变化 |
| **2** | **0x0800C5BE** | `f5 d1`（bne 0x0800C5AC） | `00 bf`（nop） | 递增 handler：同上 | 同上（item0..8/10/11 sync 1 次，item9 0 次） | 极低；同上 |
| **3** | **0x08003956** | `00 7a`（ldrb r0,[r0,#8]） | `00 20`（movs r0,#0） | 造型灯任务 0x08003950 第一处 settings[8] 读者固定 0 | 静态读数确认；固定 0 等价原厂 12sec 默认、无触发不点灯 | 失去原厂 LIGHT=OFF/ON 造型灯功能（设计如此）；菜单仍显示开关值 |
| **4** | **0x0800397E** | `01 7a`（ldrb r1,[r0,#8]） | `00 21`（movs r1,#0） | 造型灯任务第二处读者固定 0 | 同上；两处都在同一函数（GPIO 0x48001000 bit13/14 三态） | 同上 |
| **5** | **0x0800D0A2** | `26 72`（strb r6,[r4,#8]，r6=0） | `00 bf` | RF 消息 handler 0x0800CD7C 的“设置复位”分支不再覆盖 settings[8]，保护密钥开关不被静默清 0 | 该分支 0x0800D094 起把 settings[0..9] 写成常量（s8=0）；strb 不改标志、后续不再读 settings[8]，NOP 安全（§10.1） | 远程/触发器的“设置复位”命令不再清除 LIGHT 开关（这是保护目的）；“恢复出厂”仍会清（见 §10.2） |

打补丁后与原厂差异：**恰好 4 字节**（两处 2 B），固件长度不变；脚本断言差异偏移集合
= {0x08005A74, 0x08005A75, 0x0800C5BE, 0x0800C5BF}。
同步字函数本体由 Lead/`keypatch/devices/v860iiis.py` 整段改写（0x080115B8 起 56 B，到 0x080115F0），
本任务的 gate 补丁只负责“何时调用它”。

### 4.1 方案 (b)（V1C 式 BL 注入，仅作对比/不推荐）

- 把两个 gate 的 4 字节（`cmp r1,#6; bne`）各换成 `bl <cave>`：
  0x08005A72 → `10 f0 22 fc`（BL 0x080162BA）、0x0800C5BC → `09 f0 7d fe`（BL 0x080162BA）。
- cave 内 16 B hook（已在 Unicorn 中实测执行）：

```
   0x080162BA: 1f b5        push {r0-r4,lr}
   0x080162BC: 80 79        ldrb r0,[r0,#6]        ; r0 = settings[6] = ID
   0x080162BE: fb f7 7b f9  bl   0x080115B8
   0x080162C2: 1f bc        pop  {r0-r4}
   0x080162C4: 01 bc        pop  {r0}              ; 取回 BL 的返回地址(LR)
   0x080162C6: 06 30        adds r0,#6             ; 跳过原 gate 后的 ldrb+bl
   0x080162C8: 00 47        bx   r0                ; -> 0x08005A7C / 0x0800C5C6
```

- Unicorn 实测：**方案 (b) 与方案 (a) 在 item0..11 × 正常/回绕 × 两个 handler 上行为完全等价**
  （sync 次数/入参、settings 结果、栈与寄存器恢复均一致）。
- 但本镜像**没有可证明的死代码洞**：唯一大零区 `0x080162B9..0x08016328`（112 B）位于数据段尾部
  `0x08016000` 之后，线性扫描虽有 3 处“LDR 池指入”但源地址在数据区（属数据误码），
  且 0x08016328 被真实代码作为数据指针引用（u32 @0x08003890）。不能证明该零区无人使用。
  **因此不采用方案 (b)**；NOP 方案更小、更稳、无需空洞。

## 5. settings[8]=LIGHT 读者/写者完整清单与屏蔽语义

### 5.1 功能读者（2 处，同一造型灯任务，必须屏蔽）

| 地址 | 原厂指令/字节 | 屏蔽后 | 说明 |
|---|---|---|---|
| 0x08003956 | `00 7a` ldrb r0,[r0,#8] | `00 20` movs r0,#0 | 函数 **0x08003950** 入口：r0=LIGHT；`cmp r0,#1`（0x0800395E）→ LIGHT=1(OFF) 分支清 GPIO 0x48001000 bit13/14 |
| 0x0800397E | `01 7a` ldrb r1,[r0,#8] | `00 21` movs r1,#0 | 同函数第二处：`cmp r1,#2`（0x08003982）→ LIGHT=2(ON) 分支驱动造型灯 |

该任务由主循环函数 0x08013CE4 内 0x08013DB2 `bl 0x08003950` 每轮调用（与 load task 0x08013D60 同链）。
屏蔽为常数 0 后：任务按出厂默认（12sec 档的熄灭/清位行为）运行，**LIGHT=2 不再点亮造型灯**；
菜单渲染读者（0x08004A8A/0x08004A9A，属于行渲染函数 0x0800485C 区域）保留，用户仍能看到 ON/12sec/OFF 开关状态。

### 5.2 写者（3 处 + clamp，均需理解）

| 地址 | 类别 | 处理 |
|---|---|---|
| 0x08005EB0 `01 72`（strb r1,[r0,#8]，r1=0） | 出厂默认函数 0x08005E54（settings[0..9]，调用者：load task 0x0800EF2E、RF handler 0x0800D0DA） | **保留**：恢复出厂 → LIGHT=0 → 普通模式（预期） |
| 0x0800D0A2 `26 72`（strb r6,[r4,#8]，r6=0） | RF 消息 handler 0x0800CD7C 的“设置复位”分支（0x0800D058→0x0800D094，写 settings[0..9] 常量 + 调 LCD 函数 + sync） | **NOP 保护**（补丁 #5） |
| 0x08013C48 `13 72`（strb r3,[r2,#8]） | flash 记录加载后展开函数 0x08013B08（record+0x36 → settings[8]） | **保留**：持久化恢复密钥开关 |
| 0x0800F094 `14 54`（strb r4,[r2,r0]，动态索引） | load task 末尾 clamp：settings[i] > max[i] 时置 0；i<12 | **保留**：LIGHT max=2，密钥值 2 合法，不会被清 |

结论：**密钥值只会在“恢复出厂/远程设置复位”时被写回 0**；常规上电加载/持久化不会破坏它。

## 6. 持久化证据（覆盖 settings[8]）

- **记录结构**：72 B（0x48）记录，`0x08012F68` 按 **0x12 words** 比对/提交；记录头字节 **0x5C**。
  settings 组布局（save builder 0x080056D8 内）：
  - record+0x2C = [x, x, **s0, s1**]
  - record+0x30 = **s2..s5**
  - record+0x34 = **s6..s9**（**s8 = record+0x36**；读点 0x08005816 `ldrb r4,[r2,#8]`，写点 0x08005820 `str r4,[r0,#0x34]`）
  - record+0x38 = **s10..s13**
  - record+0x3C = 其它配置字
- **save 链路**：save task **0x0800D9C8**（主循环 0x08013DC6 调用）→
  0x0800D9E4 `bl 0x080056D8`（目标缓冲 **0x20000FD4**）→ 0x0800D9E8 `bl 0x08012F68`（比对/提交到 0x20000F8C）
  → 槽计数 0x20000428++（≥0x14 时 0x080092A8 换页并清 0）→ **0x08010D94(slot)**：
  `0x0801F000 + slot*0x64`，内部再 `bl 0x080093A0` 编程（0x08010DB4 `0x64`、0x08010DC8 字面量 0x0801F000）。
- **load 链路**：load task **0x0800EEB0**（主循环 0x08013D60 调用）从 slot 19 倒序扫描 20 槽
  （0x0800EEB8=0x14、0x0800EEC6=0x64、0x0800EECA 字面量 0x0801F000），读入 0x20000F8C；
  校验 `[0x20000F8C]==0x5C`（0x0800EED6）→ 0x0800EEEC `bl 0x08013B08` 展开；
  无效 → 0x0800EF2E `bl 0x08005E54` 出厂默认；末尾 clamp（0x0800F086..0x0800F09C）后再
  `bl 0x080092A8` / `bl 0x08010D94(0)` 回写规整记录。
- **展开函数 0x08013B08**（无 prologue 的叶子函数）：record+0x2C..+0x3F → 工作缓冲 0x20000434 →
  settings[0..0xD]（settings[8] 在 0x08013C48）。settings 基址字面量在 0x08013CD4。

**结论：settings[8]（LIGHT/密钥开关）随 C.Fn 设置一起保存到 flash，上电恢复。**
（0x20000518 自动保存标志只决定“何时写”，不影响记录内容，见 §7。）

## 7. 自动保存倒计时（风险项，非补丁点）

- 两个 handler 入口（递减 0x080059F6、递增 0x0800C53A）都 `bl 0x080109C8`：把
  **0x20000516 = 0xC8（200）**；另有 RF handler 0x0800CF26、0x08010E0C 也会重置它。
- **0x080109A4**（save task 0x0800D9C8 首句调用）每 tick 递减 0x20000516，归零时置
  **0x20000518 = 1**。
- save task 见 0x20000518==1 且 0x2000055A==0 时生成记录并写 flash（§6）。
- 影响：改 C.Fn 后约 200 个 tick 才落盘；期间掉电可能丢失本次改动（与原厂行为一致，非补丁引入）。
  LIGHT 开关值在 settings[8] 里会被正常保存/加载，不受该标志影响。

## 8. Unicorn handler 级验证

### 8.1 最小执行现场（已用 UC_HOOK_MEM_READ 逐地址核实，供模块 verify 内嵌）

- ROM 0x08000000+0x20000（镜像写 0x08003000）；RAM 0x20000000+0x4000。
- 桩函数（入口写 `70 47`=bx lr，hook 记录并 PC=LR）：**仅 6 个**：
  0x08003A48、0x08011790、0x080109C8（保存倒计时）、0x0800EAE8（仅 mode==3 分支）、
  0x080128F0（item9 LCD 专用）、0x08012A9E（递增非编辑分支）；
  **同步字函数 0x080115B8 也写 bx lr**，在 `addr==0x080115B8` 时记录 r0 后 PC=LR。
- RAM 预置：`0x200004D4=1`、`0x200004D5=1`（值编辑；=0 测光标移动）、
  `0x200004D6=item`（0..11）、`0x20000536=0`、`0x20000563=0`、
  settings[0..11]=0 且 `settings[item]=初值`、`settings[6]=ID`（item==6 时即同一字节）。
  **不需要 0x200004F0/F1**：编辑路径实测只读上述 5 个 RAM 字节 + settings + 栈。
- 哨兵 0x20001300 写 `fe e7`（b .），LR=0x20001301；SP=0x20001480（handler 用到 0x20001468..0x2000147B）；
  r3..r7 预置 0x1003..0x1007 用于恢复检查；实测单次 handler 最大 58 条指令（上限给 2000）。
- 判定：撞哨兵返回 && 无 UcError && SP==0x20001480 && r3-r7 恢复 && sync 次数/入参符合
  && settings[item] 符合期望 && 其它 settings 字节不变 && item 索引符合期望。

### 8.2 期望 sync 次数表（值编辑模式，0x200004D5=1）

| item | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| **原厂 sync** | 0 | 0 | 0 | 0 | 0 | 0 | **1** | 0 | 0 | **0** | 0 | 0 |
| **NOP / BL-hook 后 sync** | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | **0** | 1 | 1 |

- sync 入参 = 改值后的 **settings[6]**（item==6 时即新 ID；其余项时 ID 不变）。
- item9(LCD) 两态都 **0 次 sync**，且都调用 0x080128F0；改值后 settings[9]：
  递减值>0→-1、值0→不变（转专用函数）；递增值<6→+1、值=6→不变（转专用函数恢复 max）。
- 其它项改值后 settings[item]：递减 值>0→-1、值0→max[item]；递增 值<max→+1、值=max→0。

### 8.3 三态实测结果（193 项断言全 PASS）

| 场景 | 原厂 | 方案 (a) NOP | 方案 (b) BL-hook |
|---|---|---|---|
| 值编辑 item0..8/10/11（正常/回绕） | sync 0 次 | sync 1 次，r0==settings[6] | 同 NOP（逐项等价） |
| 值编辑 item6（ID） | sync 1 次，r0=新 ID | sync 1 次，r0=新 ID | 同 NOP |
| 值编辑 item9（LCD） | sync 0 次 + 调 0x080128F0 | 0 次 + 调 0x080128F0 | 同 NOP |
| 光标移动（0x200004D5=0） | 递减仅原索引 7 / 递增仅原索引 5 各 1 次；其余 0 | 递减原索引 1..9、递增原索引 0..7 各 1 次；回绕到 9 的路径 0 次 | 同 NOP |
| 忙标志提前返回（0x20000536=1） | 0 次 sync、settings 不变、SP/r3-r7 恢复 | 同左 | 同左 |
| 寄存器/栈 | SP 恢复、r3-r7 恢复 | 同左 | 同左 |
| 与原厂字节差异 | — | 恰好 4 字节（2 处 NOP） | 2 处 BL + 16 B hook = 23 字节变化（1 字节与原字节重合） |

## 9. 与 V1C V2.0 对照（同平台、地址不同）

| 角色 | V1C V2.0 | 本镜像 V860IIIS V1.1 |
|---|---|---|
| settings 基址 | 0x20001086（51 处字面量） | **0x20001058**（30 处） |
| 最大值表 | 0x08018362 | **0x08016102**（字节完全相同） |
| 同步字函数 | 0x080121BC（56 B） | **0x080115B8..0x080115EF（56 B）** |
| 递减 handler / gate | 0x080061CC / 0x0800625C（cmp+beq） | **0x080059E4 / 0x08005A72（cmp+bne）** |
| 递增 handler / gate | 0x0800D3D8 / 0x0800D46E（cmp+beq） | **0x0800C528 / 0x0800C5BC（cmp+bne）** |
| 立即生效方案 | BL 注入（需 10 B 洞 0x08017944） | **bne→NOP 2 B ×2（无需洞；因 gate 是 bne，NOP 后自然 fall-through）** |
| LIGHT 任务 / 读者 | 0x080036D4 / 0x080036DA、0x08003702 | **0x08003950 / 0x08003956、0x0800397E**（字节同为 `00 7a`/`01 7a`） |
| 无线同步写 settings[8] | 0x0800A6FA（紧凑结构解包） | **0x0800D0A2（RF 设置复位分支，直写常量 0）**；本镜像无紧凑结构解包写 s8 |
| save / load | 0x08005EF0 / 0x080158B0 | **0x080056D8 / 0x08013B08**（记录 72 B，槽 0x0801F000+slot*0x64） |
| 出厂默认 | 0x080066D0 | **0x08005E54** |
| 代码洞 | 0x08017944（234 B） | 无可靠洞（见 §4.1） |

## 10. 风险与未决问题

1. **0x0800D0A2 NOP 安全性（Lead 问题 3）**：安全。`strb` 不改变条件标志位；该分支后续
   0x0800D0A4..0x0800D0AC 只写 settings[4..9]（不再写 s8）、调用 0x080128F0（LCD 应用）、
   读 settings[6] 调 sync，随后 b 0x0800D10C 读 0x200004D7 后 pop，**不再读 settings[8]**；
   settings[8] 保留原值（不会“未初始化”）。
   下一个主循环：造型灯任务已被屏蔽、菜单仍显示开关、save builder 按原值持久化——正是预期。
2. **恢复出厂/远程设置复位会清开关**：出厂默认函数 0x08005E54（s8=0）与 RF handler 的
   0x0800D0DA `bl 0x08005E54` 路径仍会把 LIGHT 清 0 → 恢复为普通模式。这是“恢复出厂”的合理语义；
   文档需说明“恢复出厂后需重新把 LIGHT 设为 ON”。
3. **光标移动也会 sync**（方案 a 的已知副作用）：原厂只有在移动落到 item6 时才 sync；NOP 后
   所有非 9 项都会 sync。若 Lead 的同步字函数保留原厂 SCAL strobe + 1 ms 延时，菜单导航每次
   按键会多 ~1ms 延迟；**建议沿用 V1C v5 的“只写 SYNC1/SYNC0、无 SCAL/无延时”改写**。
4. **本镜像菜单 10 项（0..9）**：item10/11 只存在持久化结构中，无功能读者；报告/文档不要写
   “12 个可编辑 C.Fn 项”。LIGHT=item8、ID=item6 均可达，不影响本补丁。
5. **无实机验证**（最高风险）：所有结论为静态 + Unicorn 仿真。需要真机确认：
   LIGHT 菜单三档可改并持久化；改 LIGHT 后同步字立即变化（配合 Lead 改写后的 0x080115B8）；
   造型灯在两处 reader 屏蔽后不再点亮；掉电重启后开关保持；恢复出厂后回普通模式。
6. **未验证 CRC/签名启动校验**：本镜像未见明显应用 CRC 校验代码，但未穷举；若 bootloader 校验
   固件完整性，任何原位补丁都会失败（V1C/AD200 项目同假设：不校验）。
7. **固件版本锁定**：本报告只对 sha256 `bdc8f354…0b20` 有效；换版本必须重跑
   `cfn_settings.py` 并重新确认 gate/读者/写者/记录布局。
8. **同步字函数改写由 Lead 负责**：本报告不写 `keypatch/devices/v860iiis.py`（Lead 写域）；
   已核对 Lead 模块中本报告 5 个补丁点 + SYNC_FN/SETTINGS_BASE/KEY_FLAG_ADDR/CFN_MAX_TABLE_ADDR
   与本文一致（log 末尾 INFO 9/9 命中）。

## 11. 文件清单

```
devices/v860iiis/re/cfn_settings.py          ← 可重复运行（退出码 0 = 全部断言 PASS）
devices/v860iiis/re/log_cfn_settings.txt     ← 真实运行输出（220 行，PASS 193 / FAIL 0）
keypatch/dev/v860iiis/CFN_SETTINGS.md        ← 本报告
```

复现：`tools\python\python.exe devices\v860iiis\re\cfn_settings.py`（Windows PowerShell，先
`$env:PYTHONIOENCODING='utf-8'`）。
