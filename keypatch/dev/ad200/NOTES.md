# AD200 Pro V1.60 私有密钥「可开关 + 兼容」补丁（v2，KEY = 0x6B21）—— 推导与验证记录

> 设备模块：`keypatch/devices/ad200.py`（DEVICE / apply / verify）
> 成品固件：`keypatch/out/AD200pro_V1.60_key6B21.bin`
> 原厂固件：`devices/ad200pro/firmware/AD200pro_V1.60.bin`（未改动，sha256 `fc0f77f1…`）
> 设计依据：`keypatch/DESIGN_TOGGLE.md`（v2）；原型：`keypatch/dev/lead/toggle_proto.py`
> 环境：Windows / PowerShell；`tools/python/python.exe` = 3.11.9（capstone 5.0.7 / unicorn 2.1.4）；
> 整机模拟器用 `devices/ad200pro/python/python.exe` = 3.12.10（同版本库）。日期：2026-11-07。

> v1（固定密钥）已被本版取代，其验证日志与说明归档在 `keypatch/dev/ad200/history_v1/`。

---

## 0. 结论速览

| 项 | 结果 |
|---|---|
| 用户语义 | 灯 **ID=OFF(0) → SYNC=KEY（密钥模式）**；**ID=1..99 → 50024/(ID+1)（原厂普通模式）** |
| 补丁形态 | **整段原位改写 56 字节**：0x0800B9AC..0x0800B9E4（48B 代码 + KEY/50024 字面量） |
| 输出长度 | **46380 字节（与原厂一致）** |
| 输出 sha256 | `53fc31434559a3bd9ed10b82b92b2425f791b9561aaee3d705612e05044a5d87` |
| 与原厂差异 | **48 字节**，全部落在 `[0x89AC, 0x89E0)`（函数区域）内；0x89E0 的 50024 字面量不变 |
| `keypatch patch --device ad200 --key 6B21 --verify` | **11/11 PASS**（退出码 0） |
| Unicorn 调用级 | ID=0 → `(4,0x6B),(5,0x21)`+`delay(20)`；ID=1/37/99 → 原厂公式；**无 SCAL**；原厂镜像自证保留 SCAL+1ms |
| 与 Lead 原型交叉比对 | 56 字节 blob **逐字节一致** |
| 整机启动回归 | 精确对齐主循环入口 0x0800CE40；再跑 200 万条无 fault/stall；CC2500 仅 SYNC1/SYNC0 不同；strobe 只少一次 SCAL；启动指令数差恒 299（v2 删掉的指令），运行期不扩散 |

---

## 1. 补丁设计（v2）

### 1.1 灯端语义

```
if (id == 0)  sync = KEY;            // 密钥模式：原厂引闪器发不出这个同步字
else          sync = 50024/(id+1);   // 普通模式：与未打补丁的原厂设备完全一致

WriteReg(4, sync >> 8);   // CC2500 SYNC1
WriteReg(5, sync & 0xFF); // CC2500 SYNC0
delay_us(20);
return;
```

用现有 ID 菜单当开关（零 UI 改动）：
- 灯 ID=OFF(0)：密钥模式（配合同样打补丁、ID=99 的引闪器）；
- 灯 ID=1..99：普通模式，可被任何原厂引闪器引闪（ID 相同、频道相同）。

### 1.2 原位布局（0x0800B9AC 起 56 字节，正好到下一函数 0x0800B9E4）

```
+00 ldr  r2,[pc,#0x2C]   ; =KEY          （字面量在 +0x30 = 0x0800B9DC）
+02 push {r4,lr}
+04 cmp  r0,#0
+06 beq  +0x0E           ; ID==0 -> keymode
+08 ldr  r2,[pc,#0x28]   ; =50024        （字面量在 +0x34 = 0x0800B9E0，与原厂同址同值）
+0A adds r1,r0,#1
+0C b    +0x10
+0E movs r1,#1           ; keymode: 除数=1
+10 mov  r0,r2           ; go: 被除数
+12 bl   0x080030F0      ; __aeabi_uidiv
+16 mov  r4,r0
+18 lsrs r1,r0,#8
+1A movs r0,#4
+1C bl   0x0800CAA2      ; WriteReg(4, sync>>8)
+20 uxtb r1,r4
+22 movs r0,#5
+24 bl   0x0800CAA2      ; WriteReg(5, sync&0xFF)
+28 movs r0,#0x14
+2A bl   0x0800C828      ; delay_us(20)
+2E pop  {r4,pc}
+30 .word KEY            ; 0x0800B9DC
+34 .word 50024          ; 0x0800B9E0（保留原厂字面量地址/值）
```

补丁后反汇编（`log_v2_disasm_before_after.txt`）：

```
0x0800B9AC  0b 4a        ldr  r2, [pc, #0x2c]        ; KEY
0x0800B9AE  10 b5        push {r4, lr}
0x0800B9B0  00 28        cmp  r0, #0
0x0800B9B2  02 d0        beq  #0x800b9ba
0x0800B9B4  0a 4a        ldr  r2, [pc, #0x28]        ; 50024
0x0800B9B6  41 1c        adds r1, r0, #1
0x0800B9B8  00 e0        b    #0x800b9bc
0x0800B9BA  01 21        movs r1, #1
0x0800B9BC  10 46        mov  r0, r2
0x0800B9BE  f7 f7 97 fb  bl   #0x80030f0            ; __aeabi_uidiv
0x0800B9C2  04 46        mov  r4, r0
0x0800B9C4  01 0a        lsrs r1, r0, #8
0x0800B9C6  04 20        movs r0, #4
0x0800B9C8  01 f0 6b f8  bl   #0x800caa2            ; WriteReg(4, SYNC1)
0x0800B9CC  e1 b2        uxtb r1, r4
0x0800B9CE  05 20        movs r0, #5
0x0800B9D0  01 f0 67 f8  bl   #0x800caa2            ; WriteReg(5, SYNC0)
0x0800B9D4  14 20        movs r0, #0x14
0x0800B9D6  00 f0 27 ff  bl   #0x800c828            ; delay_us(20)
0x0800B9DA  10 bd        pop  {r4, pc}
0x0800B9DC  21 6b 00 00  .word 0x00006B21           ; KEY
0x0800B9E0  68 c3 00 00  .word 0x0000C368           ; 50024（原厂同址同值）
```

> 编码器与排版复用 Lead 已验证原型 `keypatch/dev/lead/toggle_proto.py`
> （`build_light_patch()` + Thumb/BL 编码器），复制进设备模块以避免运行时依赖
> `dev/lead`；`unicorn_test.py` 会**逐字节比对**本模块输出与 Lead 原型输出（实测一致）。

### 1.3 与原厂函数的差异（整段改写）

| 原厂 | v2 |
|---|---|
| `sync = 50024/(id+1)`（无条件） | `id==0 ? KEY : 50024/(id+1)` |
| 写 SYNC 后 `delay(20us)` → `SCAL(0x33)` → `delay(1ms)` | 写 SYNC 后只 `delay(20us)`，**不 SCAL、不 1ms 延时** |
| 末尾字面量 0x0800B9E0=50024 | 同址同值保留；新增 KEY 字面量 0x0800B9DC |

### 1.4 为什么可以去掉 SCAL + 1ms 延时（风险评估）

原函数在写 SYNC1/SYNC0 之后做了 `SCAL(0x33)`（频率合成器校准）和 1ms 等待。
本次补丁**只改同步字、不改 FREQ2/1/0（0x0D/0x0E/0x0F）或任何频率相关寄存器**：

- CC2500 的 SCAL 用于在频率设置变化后重新校准 VCO/合成器；SYNC 是纯数字匹配字，
  不参与频率合成，写 SYNC 后不需要重新校准；
- 初始化流程 `rf_init` 中 `rf_chip_config` 之后本身就有 `SCAL + delay(1000)` 序列
  （模拟器 strobe 序列可见），频率早已校准好；
- 真正改变信道的路径（`rf_apply_settings` / `rf_write_pair` 写 0x0D/0x0E/0x0F 后）
  自己会走 `SCAL/SIDLE` 序列，不依赖本函数；
- 保留 20µs 建立时间足以让 SYNC 寄存器写入稳定（与原厂首段延时一致）。

**结论：风险低**。但这是相对原厂的唯一行为差异，整机回归里也确认 strobe 序列只少一次
SCAL、其它 46 个 CC2500 寄存器与全部其它 strobe 一致。实机上若遇到异常，可把
SCAL+1ms 加回（56 字节空间会不够，需要挪到别处注入代码——见 §7 风险）。

---

## 2. 静态复核（调用链 / 写入路径 / 边界）

`static_analysis.py` 的输出（`log_v2_static_analysis.txt`）：

```
[2] 调用 0x0800B9AC（同步字设置函数）的全部位置：
  0x080045F2  in FUNC_0x080045AC      <- 设置变化路径（参数 = settings[+6] 即 ID）
  0x080048C6  in FUNC_0x08004860      <- rf_init 末尾（必调）
  0x08008552  in FUNC_0x08008504      <- 设置变化路径（参数 = settings[+6]）
  0x08008AB6  in FUNC_0x08008818      <- ui_key_scan_task（参数 = settings[+6]）
[3] 调用 rf_chip_config 0x0800C858 的位置（唯一调用者）：
  0x08004876  in FUNC_0x08004860
[4] 调用 rf_chip_reset/SRES 0x0800CA2C 的位置：
  0x08004872  in FUNC_0x08004860
[5] 写 CC2500 reg4/reg5 的直接代码：
  0x0800B9BC reg=4  in FUNC_0x0800B9AC   <- 本函数（已整段改写）
  0x0800B9C4 reg=5  in FUNC_0x0800B9AC
  0x0800C884 reg=4  in FUNC_0x0800C858   <- 初始化表写 0xC3
  0x0800C88C reg=5  in FUNC_0x0800C858   <- 初始化表写 0x68
[7] 下一函数起点（disasm.txt）: 0x0800B9E4；原厂 56 字节 == 模块 ORIGINAL_FUNC ✔
```

要点：

1. **全镜像只有两处代码写 SYNC1/SYNC0**：初始化表 `rf_chip_config`（写 0xC3/0x68）与本函数。
   `rf_chip_config`/`SRES` 的唯一调用者是 `rf_init`，而 `rf_init` 在 `rf_chip_config` 之后
   **必定**调用本函数（0x080048C6；自检重试耗尽也顺序执行到），所以初始化值随后被覆盖。
2. 4 个调用点的入参都是 `settings[+6]`（ID）。ID 菜单改动时 `ui_key_scan_task`
   （0x08008AB6）与两条设置变化路径会**立即调用**本函数 → 模式切换实时生效。
3. 56 字节区域正好结束在下一函数 `FUNC_0800b9e4` 之前，越界检查由 apply/verify/静态脚本
   三处把关；0x0800B9E0 的 50024 字面量与原厂同址同值（普通模式公式用）。
4. `apply()` 先校验长度 46380 + 原厂 sha256 + **整段 56 字节原始字节**（`ORIGINAL_FUNC`），
   任一不符直接 `ValueError`，不会半打补丁。

---

## 3. Unicorn 调用级测试（verify 核心）

### 3.1 harness

- 镜像加载到 `0x08003000`（flash 映射 `0x08000000 + 0x20000`），RAM `0x20000000+0x2000`；
- 栈 `SP=0x20000800`；**入口 `0x0800B9AC | 1 = 0x0800B9AD`**（Thumb 位）；
- 桩（Unicorn code hook，记录后改 PC=LR 跳过）：`0x0800CAA2` WriteReg、`0x0800CB64` strobe、
  `0x0800C828` delay；**`bl 0x080030F0`（__aeabi_uidiv）保持真实执行**（覆盖完整数据通路）；
- `r0=ID`，LR=返回哨兵，`pop {r4,pc}` 到哨兵时停止并校验正常返回；
- Unicorn 坑（已规避）：入口必须带 Thumb 位；桩返回时 PC 写 **LR 原值**（bit0=1），
  写 `LR & ~1` 会切 ARM 解码报 `UC_ERR_INSN_INVALID`。

### 3.2 实测（`unicorn_test.py` / `log_v2_unicorn_test.txt`）

原厂镜像（同一 harness 自证）：

```
ID=0   记录=[(4,195),(5,104),('delay',20),('strobe',0x33),('delay',1000)] -> 0xC368
ID=1   记录=[(4, 97),(5,180),('delay',20),('strobe',0x33),('delay',1000)] -> 0x61B4
ID=37  记录=[(4,  5),(5, 36),('delay',20),('strobe',0x33),('delay',1000)] -> 0x0524
ID=99  记录=[(4,  1),(5,244),('delay',20),('strobe',0x33),('delay',1000)] -> 0x01F4
```

补丁镜像：

```
ID=0   密钥模式 记录=[(4,107),(5,33),('delay',20)]  -> SYNC=0x6B21（原公式 0xC368）
ID=1   普通模式 记录=[(4, 97),(5,180),('delay',20)] -> SYNC=0x61B4（= 50024/2）
ID=37  普通模式 记录=[(4,  5),(5, 36),('delay',20)] -> SYNC=0x0524（= 50024/38）
ID=99  普通模式 记录=[(4,  1),(5,244),('delay',20)] -> SYNC=0x01F4（= 50024/100）
```

即：**ID=0 写 KEY，ID=1/37/99 与原厂逐位一致；strobe 为空（无 SCAL）；只有 delay(20)**。
模块 `verify()` 同样断言：56 字节与期望一致、KEY/50024 字面量、差异只在区域、上述 4 个 ID、
以及原厂镜像自检（`50024//(ID+1)` + SCAL + 1ms 延时）—— 共 11 项。

---

## 4. keypatch 命令（验收主命令）

```
tools\python\python.exe keypatch\keypatch.py patch --device ad200 --key 6B21 --verify
```

真实输出（`log_v2_keypatch_6B21.txt`，11 项 PASS，退出码 0）：

```
== Godox AD200 Pro (ad200)
   原厂固件 : devices\ad200pro\firmware\AD200pro_V1.60.bin  (46380 字节, sha256 fc0f77f1eb1c9f9d…)
   私有密钥 : 0x6B21  ->  SYNC1=0x6B SYNC0=0x21
   补丁点   :
  [1] off+0x089AC  0x0800B9AC  0c 4a 10 b5 ... -> 0b 4a 10 b5 ...
      整段改写同步字函数 48B：ID==0 -> KEY，ID=1..99 -> 50024/(ID+1)；去掉 SCAL(0x33)+1ms 延时，保留 delay_us(20)
  [2] off+0x089DC  0x0800B9DC  25 ff 10 bd -> 21 6b 00 00  字面量池：私有密钥 0x6B21（小端）
  [3] off+0x089E0  0x0800B9E0  68 c3 00 00 -> 68 c3 00 00  字面量池：50024（与原厂同址同值，普通模式公式用）
   输出     : keypatch\out\AD200pro_V1.60_key6B21.bin  (46380 字节, sha256 53fc31434559a3bd…)
   [PASS] 长度不变：46380 字节（期望 46380）
   [PASS] 静态：0x0800B9AC 起 56 字节整段改写与期望完全一致（结尾 0x0800B9E4，未越过下一函数 0x0800B9E4）
   [PASS] 静态 0x0800B9DC：KEY 字面量 21 6b 00 00（期望 21 6b 00 00）
   [PASS] 静态 0x0800B9E0：普通模式字面量 68 c3 00 00（期望 68 c3 00 00 = 50024）
   [PASS] 差异仅在被改写函数内：48 字节/56，范围 [0x89AC,0x89E4)
   [PASS] Unicorn: 密钥模式(ID=0) 写寄存器 [(4,107),(5,33)]，期望 [(4,107),(5,33)]；strobe=[]（v2 无 SCAL）；delay=['20us']；原厂公式值 0xC368
   [PASS] Unicorn: 普通模式(ID=1) 写寄存器 [(4,97),(5,180)] ... 原厂公式值 0x61B4
   [PASS] Unicorn: 普通模式(ID=37) 写寄存器 [(4,5),(5,36)] ... 原厂公式值 0x0524
   [PASS] Unicorn: 普通模式(ID=99) 写寄存器 [(4,1),(5,244)] ... 原厂公式值 0x01F4
   [PASS] [unicorn] harness 自检（原厂镜像）：ID=[0,1,37,99] 均得到 50024//(ID+1)，且保留原厂 SCAL(0x33)+1ms 延时
```

成品核对：

```
46380 字节
sha256 53fc31434559a3bd9ed10b82b92b2425f791b9561aaee3d705612e05044a5d87
差异偏移 48 字节，全部在 [0x89AC, 0x89E0)；0x89E0 的 50024 字面量未变
```

换密钥泛化（KEY=0xA5C3，输出到 %TEMP%，未污染 `keypatch/out/`）同样 11/11 PASS，
`reg4=0xA5, reg5=0xC3`（`log_v2_keypatch_A5C3.txt`）。

---

## 5. 整机启动回归（devices/ad200pro 模拟器）

`sim_regress.py`（用 `devices/ad200pro/python/python.exe`，完整输出 `log_v2_sim_regress.txt`）。
为精确比较启动指令数，启动阶段把模拟器 `max_slice_rate=1` 单步执行、在
`0x0800CE40`（主循环入口）精确断点对齐；随后恢复 4000 指令/片跑 200 万条。

```
[原厂] 到主循环: insns=78610 pc=0x0800CE40 main+0x90 reason=breakpoint
[补丁] 到主循环: insns=78311 pc=0x0800CE40 main+0x90 reason=breakpoint
启动指令数差 = 299（v2 删除 SCAL+1ms 延时的预期结果，<1000）
[PASS] 复位 PC/SP 一致；精确主循环入口一致

主循环 200 万条任务/中断入口次数（原厂 vs 补丁）：
  display_task 28/32, ui_key_scan_task 28/33, flash_power_task 28/33,
  rf_poll_settings 28/32, charge_enable_task 28/32, measurement_task 28/32,
  tim14_irq 961/931  —— 同一量级（模拟器时序非周期精确，±25% 内）
[PASS] 两镜像均无 fault/stall，任务与 TIM14 心跳持续执行
主循环阶段各执行 +2,000,000 条；两镜像总指令数差保持 299（运行路径未发散）

启动期 CC2500 reg4/5 写入序列（SYNC1,SYNC0）：
  原厂: ['0xC368', '0xC368']      # 初始化 0xC3/0x68 -> ID0 公式同值
  补丁: ['0xC368', '0x6B21']      # 初始化 0xC3/0x68 -> 本函数覆盖为 KEY
[PASS] 初始化仍写 0xC3/0x68，补丁随后被本函数覆盖

原厂最终 SYNC 0xC368 / 补丁最终 SYNC 0x6B21（ID=0 即密钥模式）
CC2500 寄存器差异: ['0x04','0x05']；其它 46 个寄存器完全一致
strobe 序列：原厂 16 条 / 补丁 15 条，只少一次 SCAL(0x33)，其余完全一致
最终状态: 原厂 state=paused insns=2078610 | 补丁 state=paused insns=2078311
PASS sim_regress (v2)
```

另外原厂基线 `devices/ad200pro/tests/run_all.py` 仍 **2/2 PASS**。
说明：模拟器时序是行为级近似（`docs/usage.md` §10.1：TIM/SysTick 按
"1 ms ≈ 2000 条指令" 折算，非周期精确），因此不要求两镜像 PC 采样逐条相同；
用"精确对齐主循环入口 + 指令数差恒定 + 任务计数同量级 + CC2500 配置一致"证明无回归。

---

## 6. 可复现命令汇总

均在 `` 下执行：

```powershell
# 1) 打补丁 + 离线验证（验收主命令）
tools\python\python.exe keypatch\keypatch.py patch --device ad200 --key 6B21 --verify

# 2) 校验已生成的成品 bin
tools\python\python.exe keypatch\keypatch.py verify --device ad200 `
    --image keypatch\out\AD200pro_V1.60_key6B21.bin --key 6B21

# 3) Unicorn 调用级详版（v2 语义 + Lead 原型逐字节比对 + apply 反例）
tools\python\python.exe keypatch\dev\ad200\unicorn_test.py

# 4) 静态证据（调用点 / 写 SYNC 全部路径 / 56B 边界 / 原厂字节）
tools\python\python.exe keypatch\dev\ad200\static_analysis.py

# 5) 补丁前后反汇编对比
tools\python\python.exe keypatch\dev\ad200\dis_patch_region.py

# 6) 整机启动回归（用 devices/ad200pro 的 python）
cd devices/ad200pro
.\python\python.exe keypatch\dev\ad200\sim_regress.py

# 7) 原厂基线自测（可选）
cd devices/ad200pro; .\python\python.exe tests\run_all.py    # 2/2 PASS
```

---

## 7. 风险 / 不确定 / 限制（如实记录）

1. **未做实机验证**：全程为静态反汇编 + Unicorn 调用级 + 行为级整机模拟器；没有真实
   AD200 Pro / 引闪器 / SDR。刷机后果由使用者承担。
2. **删掉 SCAL(0x33)+1ms 延时是相对原厂唯一的行为差异**：分析认为只改 SYNC、不改频率
   寄存器时不需要重新校准（初始化路径已有 SCAL；信道切换路径自己会校准），保留 20µs
   建立时间。模拟器无法证明 RF 空口行为，若实机出现同步异常，回退方案是恢复
   `SCAL+1ms`（56 字节空间不够，需要把函数挪到空闲 flash 注入；见 DESIGN_TOGGLE §3 思路）。
3. **ID=OFF 的语义变了**：v2 补丁后灯 ID=OFF(0) 不再是"无 ID 的普通模式"，而是密钥模式。
   灯/引闪器必须按 DESIGN_TOGGLE §1 的表配对（推荐：灯 ID=OFF + 引闪器 ID=99）；
   只刷灯不刷引闪器会收不到。
4. **模拟器不模拟空口同步过滤**：`sim/cc2500.py` 只模拟 SPI 寄存器/FIFO/strobe，
   密钥隔离性最终依赖 CC2500 硬件 SYNC 匹配（补丁已把 SYNC1/SYNC0 写对）。
5. **16 位同步字不是密码学认证**：原厂设备只有 100 个公开同步字，普通模式仍可被原厂
   引闪器引闪（这是 v2 的设计目标）；密钥模式能挡住正常原厂设备，但 SDR 抓包理论上可克隆。
6. **固件版本强绑定**：apply 校验长度/sha256/整段 56 字节，非 V1.60 直接拒绝。
7. **整机回归的时序判定**：模拟器时序非周期精确，任务计数允许 ±25%；但已精确对齐主循环
   入口、启动指令数差恒为 299（运行期不扩散）、CC2500 配置逐寄存器一致，足以排除功能回归。
8. **未改动**：原厂 bin（sha256 复核未变）、`common.py`/`keypatch.py`/`INTERFACE.md`/
   `DESIGN_TOGGLE.md`、其它队友目录（含 Godox-S3-Trigger）。本任务只写
   `keypatch/devices/ad200.py` 与 `keypatch/dev/ad200/`。

---

## 8. 文件与日志清单

| 文件 | 说明 |
|---|---|
| `keypatch/devices/ad200.py` | v2 设备模块：DEVICE / apply（56B 整段改写）/ verify（静态+Unicorn） |
| `keypatch/out/AD200pro_V1.60_key6B21.bin` | v2 成品（46380 字节，48 字节差异） |
| `keypatch/dev/ad200/NOTES.md` | 本文档 |
| `keypatch/dev/ad200/unicorn_test.py` | Unicorn 调用级详版（含与 Lead 原型逐字节比对、apply 反例） |
| `keypatch/dev/ad200/static_analysis.py` | 静态证据（调用链/写 SYNC 全部路径/56B 边界） |
| `keypatch/dev/ad200/dis_patch_region.py` | 补丁前后反汇编对比 |
| `keypatch/dev/ad200/sim_regress.py` | 整机启动回归（精确对齐 + CC2500 写入序列/strobe 对比） |
| `log_v2_keypatch_6B21.txt` | 验收主命令真实输出（11 PASS） |
| `log_v2_keypatch_A5C3.txt` | 换密钥 0xA5C3 泛化测试（11 PASS） |
| `log_v2_unicorn_test.txt` | Unicorn 调用级详版输出 |
| `log_v2_static_analysis.txt` | 静态证据输出 |
| `log_v2_disasm_before_after.txt` | 0x0800B9AC..0x0800B9E4 反汇编前后对比 |
| `log_v2_sim_regress.txt` | 整机回归输出 |
| `log_v2_extra_cmds.txt` | `keypatch list/info/verify` 输出 |
| `history_v1/` | v1（固定密钥）旧验证日志与说明（已被 v2 取代，保留对照） |
