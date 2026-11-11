# Godox V1C v5-LIGHT（C.Fn LIGHT 密钥开关）—— 独立复核报告

- 任务：task-6 最终版（覆盖先前 v5-STBY 中间版）/ 独立固件审计员 **fw-auditor**
- 对象：`keypatch/devices/v1c.py` 当前 **v5-LIGHT** 实现（FLAG_IDX=8、
  `KEY_FLAG_ADDR=0x2000108E`、`KEY_FLAG_ON_VALUE=2`）生成的：
  - `keypatch/out/V1C_V2.0_keyD798.bin`（88032 B，sha256
    `86d563c8c3bc5816844b79a2c6303a39910c7bf349d757d47c8ce01bd79409cd`）
  - `keypatch/out/V1C_V2.0_keyD798_container.bin`（88088 B，sha256
    `e1634a089cf87d0fc71668be02447139b3b856bed12c3ef3c2ea63db4d5c3e5d`）
- 原厂固件：`devices/v1c/firmware/V1C_V2.0.bin`，88032 B，sha256
  `4435dda84903cb97cfaf34f7eeb15ecb6168771d60b57da1c346e6a9ca36edfb`（只读）
- 方法：**独立重编码**（不调用模块 build/verify）+ Capstone 静态核验 + 自建 Unicorn
  harness + 独立运行期 settings[8] 读者/写者扫描 + CLI/selftest 真实复现
- 约束：未修改 `keypatch/devices/v1c.py`、未改固件；写范围仅 `devices/v1c/re/`、
  `keypatch/dev/v1c/`。

## 0. 结论

**PASS（独立复核 102/102 PASS）**。v5-LIGHT 与设计一致，达到可交付（静态 + 调用级）标准：

- 56B 同步字函数：`flag==2`（LIGHT=ON）→ KEY；`flag=0/1` → `50024//(ID+1)`；
  布局 KEY@+0x2C、FLAG@+0x30=0x2000108E、50024@+0x34；无 SCAL/延时。
- 注入 10B @0x08017944；两个 gate @0x0800625C/0x0800D46E 改 `BL 注入`；
- 两处 LIGHT 功能读者 mask：0x080036DA/0x08003702 → `movs rN,#0`；
- 无线解包对 settings[8] 的写点 0x0800A6FA → NOP；
- `flag=0/1/2 × ID=0/1/5/37/99` 行为、injection hook、原厂自证全部通过；
- CLI `patch --verify` 36 PASS / 0 FAIL；`selftest --key 0xD798` 180 PASS / 0 FAIL，
  **ALL PASS**；
- 重新生成后产物 sha256 仍为上面的最终值。

**独立发现的注意点（不导致 FAIL）：**

1. **`0x0800E0A4`（0x0800DCDC 内）仍会在“恢复 C.Fn 默认”输入组合下写 settings[8]=0**。
   它不是无线同步/远程路径，而是输入扫描任务的默认重置块（条件：输入 bitmask
   r2==0x300 + `[0x200004F0]==1`）；清 settings[8] 与“恢复默认”语义一致，
   保留不 patch 合理；若产品要求密钥开关不被该组合清除，需另加 `a5 72→00 bf`。
2. **“任意 C.Fn 项改值都调 sync”对 item9(LCD) 不成立**：原厂 dec/inc handler 在到达
   gate 前就 `cmp #9; beq 0x0800626C/0x0800D47C`（转 0x08014EF8），不经过 hook。
   LIGHT=item8，不受影响；但模块文档措辞需限界。
3. **v5 按 AD100 v4 布局去掉了 delay(20us)**（原厂有 delay20+SCAL+1ms；v5 只写
   SYNC1/0 后 pop）。这是相对原厂唯一的射频时序差异，实机验证时重点看“首包”。
4. **升级语义**：原厂 LIGHT 值 0=12sec、1=OFF、2=ON。若已有用户把 LIGHT 保存为
   2（ON），升级 v5 后该值会被解释为“密钥模式”，普通引闪器将无法引闪；发布说明
   需明确“LIGHT=ON 即密钥模式”。默认 LIGHT=0（12sec）→ 普通模式。

## 1. 交付物与复现

| 文件 | 说明 |
|---|---|
| `devices/v1c/re/audit_cfn.py` | 本次独立复核脚本（v5-LIGHT；102 项检查；退出码 0；不复用模块 verify） |
| `devices/v1c/re/log_audit_cfn.txt` | 真实运行输出：独立复核 102 PASS/0 FAIL + 附 A CLI patch（36 PASS）+ 附 B selftest（180 PASS，ALL PASS） |
| `keypatch/dev/v1c/AUDIT_CFN.md` | 本报告 |

复现：

```powershell
cd D:\神牛
tools\python\python.exe devices\v1c\re\audit_cfn.py
tools\python\python.exe keypatch\keypatch.py patch --device v1c --key 0xD798 --verify
tools\python\python.exe keypatch\selftest.py --key 0xD798
```

（独立脚本不写盘；CLI 命令会重新生成 `keypatch/out` 同名产物，重生成后 sha256
仍为上面最终值。）

## 2. 静态复核（PASS）

### 2.1 独立重编码 vs CLI 产物

`audit_cfn.py` 按设计**自己重新编码**全部补丁：
- 同步字函数 56B：44B 代码（`cmp #2; bne`）+ KEY@+0x2C + FLAG(0x2000108E)@+0x30
  + 50024@+0x34；
- 注入 10B：`push {r0-r4,lr}; ldrb r0,[r0,#6]; bl 0x080121BC; pop {r0-r4,pc}`；
- 两个 gate：`BL 0x08017944`；
- 三处 mask：0x080036DA `00 7a→00 20`、0x08003702 `01 7a→00 21`、
  0x0800A6FA `08 72→00 bf`。

**独立编码结果与 CLI 产物逐字节相同**；与原厂差异 **71 字节**，分组恰为：

```
0x006DB..0x006DB (mask#1 高字节 1B)
0x00702..0x00703 (mask#2 2B)
0x0325C..0x0325F (gate dec 4B)
0x076FA..0x076FB (mask#3 2B)
0x0A46E..0x0A471 (gate inc 4B)
0x0F1BC..0x0F1E0 (sync 区 37B)
0x0F1E4..0x0F1E4 (1B)
0x0F1E6..0x0F1EF (sync 区 10B)
0x14944..0x1494D (注入 10B)
```

即除上述补丁点外无任何字节变化。

### 2.2 同步字函数 0x080121BC（56B，LIGHT flag 版）

补丁后反汇编（完整见 log）：

```
0x080121BC  10 b5        push {r4, lr}
0x080121BE  0b 4b        ldr  r3, [pc, #0x2C]   ; = FLAG 0x2000108E
0x080121C0  0b 4a        ldr  r2, [pc, #0x2C]   ; = 50024
0x080121C2  1b 78        ldrb r3, [r3]          ; flag = settings[8]
0x080121C4  41 1c        adds r1, r0, #1
0x080121C6  02 2b        cmp  r3, #2            ; LIGHT=ON
0x080121C8  01 d1        bne  +0x10             ; 非 ON -> 原厂公式
0x080121CA  07 4a        ldr  r2, [pc, #0x1C]   ; = KEY
0x080121CC  01 21        movs r1, #1
0x080121CE  10 46        mov  r0, r2
0x080121D0  f0 f7 8e ff  bl   __aeabi_uidiv
0x080121D4  04 46        mov  r4, r0
0x080121D6  01 0a        lsrs r1, r0, #8        ; SYNC1
0x080121D8  04 20        movs r0, #4
0x080121DA  03 f0 78 fa  bl   WriteReg
0x080121DE  e1 b2        uxtb r1, r4            ; SYNC0
0x080121E0  05 20        movs r0, #5
0x080121E2  03 f0 74 fa  bl   WriteReg
0x080121E6  10 bd        pop  {r4, pc}
0x080121E8  98 d7 00 00  .word KEY = 0xD798
0x080121EC  8e 10 00 20  .word FLAG = 0x2000108E
0x080121F0  68 c3 00 00  .word 50024（与原厂同址同值）
```

核验：56B 与独立构造完全一致；KEY/FLAG/50024 落位正确；BL 目标
`uidiv/WriteReg/WriteReg`；无 SCAL/delay；结束于 0x080121F4。

### 2.3 注入区 0x08017944（10B）

- 原厂前 10B 全 0；补丁后与独立编码一致：
  `push {r0-r4,lr}; ldrb r0,[r0,#6]; bl 0x080121BC; pop {r0-r4,pc}`；
- BL 目标 = 同步字函数；原厂无 BL/分支/ldr 字面量/全镜像 u32 绝对地址指向该 10B；
  整个 0x08017944..0x08017A2D (234B) 全 0 死区也无引用 → 注入安全。

### 2.4 两个 gate（0x0800625C / 0x0800D46E）

| 地址 | 原厂 4B | 补丁 4B | 解码 |
|---|---|---|---|
| 0x0800625C（dec） | `06 29 08 d0` = `cmp r1,#6; beq 0x08006272` | `11 f0 72 fb` | `bl 0x08017944` |
| 0x0800D46E（inc） | `06 29 07 d0` = `cmp r1,#6; beq 0x0800D482` | `0a f0 69 fa` | `bl 0x08017944` |

原厂字节/补丁字节正确、无引用落入 4B 内；hook 返回后继续 0x08006260 / 0x0800D472
（原 handler 后续流程）；item11 特殊分支保留。

### 2.5 读者屏蔽 / 标志保护

| 地址 | 原厂 | 补丁 | 语义核验 |
|---|---|---|---|
| 0x080036DA | `00 7a` = `ldrb r0,[r0,#8]` | `00 20` = `movs r0,#0` | LIGHT 造型灯任务（0x080036D4）读者1 |
| 0x08003702 | `01 7a` = `ldrb r1,[r0,#8]` | `00 21` = `movs r1,#0` | LIGHT 造型灯任务读者2 |
| 0x0800A6FA | `08 72` = `strb r0,[r1,#8]` | `00 bf` = NOP | 0x0800A6DC 无线紧凑结构解包中写 settings[8] 的唯一指令 |

- 原厂字节/补丁字节均正确；无引用落入 2B 内；
- 补丁后反汇编分别为 `movs r0,#0`、`movs r1,#0`、`nop`；
- NOP 0x0800A6FA 只删除一个 `strb`，无 flags/寄存器副作用；同函数其余 settings 写点
  （0x0800A6E2/EA/EE/F2/F6/FA 等）保持不变。

### 2.6 独立扫描：settings[8]=0x2000108E 的全部访问

用独立实现的前向污点 + CFG 可达性 + **跨函数参数传播**扫描全镜像：

**原厂固定写点（4 个）：**

| 写点 | 函数 | 性质 | 补丁后 |
|---|---|---|---|
| 0x0800672C | 0x080066D0 | 恢复出厂默认（settings[8]=0） | 保留（默认 LIGHT=12sec） |
| 0x0800A6FA | 0x0800A6DC | 无线设置同步解包（c[0x16] -> settings[8]） | **已 NOP** |
| 0x0800E0A4 | 0x0800DCDC | 输入组合触发的「恢复 C.Fn 默认」块 | 保留（见 §3） |
| 0x080159F0 | 0x080158B0 | flash 记录 load/expand（恢复持久化值） | 保留（持久化） |

**原厂读者（6 个）：**

| 读者 | 函数 | 类型 | 处理 |
|---|---|---|---|
| 0x080036DA / 0x08003702 | 0x080036D4 | **LIGHT 造型灯任务（仅两个功能读者）** | **mask -> movs 常数 0** |
| 0x08005076 / 0x08005086 | 0x08004914 | 菜单显示 | 保留（显示 LIGHT 状态） |
| 0x0800602E | 0x08005EF0 | save 记录 | 保留（持久化） |
| 0x0800A6CC | 0x0800A6AC | 紧凑结构打包（无线同步） | 保留（不写 settings[8]） |

补丁后：写点仅少 0x0800A6FA；读者集合除两处 mask 外不变，并新增同步函数
0x080121C2 读 flag（符合设计）。**未发现其它运行期写/读 settings[8] 的遗漏**
（扫描含 word/half 覆盖与跨 BL 参数传播；静态盲区为函数指针/全局指针间接传递，
本镜像中未发现此类 settings[8] 访问）。

### 2.7 （重要）0x0800E0A4 不是远程路径，是输入组合的默认重置块

0x0800DCDC 是输入/按键扫描任务（由设置应用 dispatcher 0x08015B52 每轮调用），
0x0800E05E..0x0800E0CC 块条件：

```
输入 bitmask r2 == 0x300（两个 GPIO 输入位）
且 [0x200004F0] == 1
```

命中后把 settings[0..0xB] 全部写回出厂默认值（含 settings[8]=0），随后
`bl 0x08014EF8` + `bl 0x080121BC` 重设同步字。这是「恢复 C.Fn 默认值」的输入组合路径，
不是无线/远程设置同步；清 settings[8] 与恢复默认语义一致，**保留不 patch 合理**。
若产品要求密钥开关不被该组合清除，可把 `0x0800E0A4 a5 72 → 00 bf`，但会改变
恢复默认语义（恢复默认后密钥仍开），需产品决定。

## 3. Unicorn 独立 harness（不复用模块 verify）

自建 harness：flash/RAM 映射；WriteReg/strobe/delay 入口写 `bx lr` 桩并 hook 记录；
`__aeabi_uidiv` 真实执行；返回哨兵 `b .`；gate 测试在 continuation 地址快照 r0..r4；
LIGHT 任务额外桩化 GPIO 读写函数（0x0800B870/0x0800B882）并记录调用参数。

### 3.1 原厂自证（PASS）

ID=0/1/5/37/99 原厂均为：
`WriteReg(4,hi) -> WriteReg(5,lo) -> delay(20) -> strobe(0x33) -> delay(1000)`，
sync = 50024//(ID+1)（0xC368/0x61B4/0x2091/0x0524/0x01F4）。

### 3.2 补丁 sync：flag=0/1/2 × ID=0/1/5/37/99（15 组，PASS）

- flag=2（LIGHT=ON）：任意 ID 都得 KEY 0xD798（SYNC1=0xD7/SYNC0=0x98）；
- flag=0/1（12sec/OFF）：得原厂公式值（同上 5 个）；
- 序列恰为 `[WriteReg(4,hi), WriteReg(5,lo)]`，**无 delay(20)/SCAL/1ms**。

### 3.3 注入 hook 经两个 gate（PASS）

对 gate 0x0800625C（dec）与 0x0800D46E（inc），item=0,1,2,3,4,5,6,7,8,10,11：
- hook 均调用 sync 一次，参数为 settings[6]=37；flag=2 → 0xD798；
- continuation（0x08006260/0x0800D472）快照的 **r0..r4 与进入 gate 前完全一致**；
- 均能正常返回哨兵。
- item=8 flag=0/1 两个 gate 再测：sync=0x2091（普通模式），r0..r4 恢复。

### 3.4 mask 语义（PASS）

- **LIGHT 两个读者**：原厂 settings[8]=2 时，0x080036DA 后 r0=2、0x08003702 后 r1=2；
  补丁后两处分别为 0 → 验证 mask 生效。
- **mask 后造型灯行为=原厂 12sec 默认**：补丁后 settings[8]=2 的 GPIO 调用序列
  **逐项等于原厂 settings[8]=0（12sec 默认）的序列**（GPIO stub 记录一致）。
- **无线解包保护**：原厂 0x0800A6DC 会覆盖 settings[8]（测试值 0x66）；
  补丁后 settings[8] 保持哨兵 0xAA（NOP 生效）。

## 4. CLI / selftest 复现（PASS）

### 4.1 `keypatch.py patch --device v1c --key 0xD798 --verify`

真实运行（退出码 0，**36 PASS / 0 FAIL**）：
- 补丁点 6 类输出（sync/注入/两 gate/两读者 mask/解包 NOP）全部 PASS；
- 与原厂差异 71 字节，区外 0；
- 模块 Unicorn 自检 flag=0/1/2 × ID、注入 hook、原厂自证全部 PASS；
- 输出 `keypatch/out/V1C_V2.0_keyD798.bin`，sha256
  `86d563c8c3bc5816844b79a2c6303a39910c7bf349d757d47c8ce01bd79409cd`。

### 4.2 `keypatch/selftest.py --key 0xD798`

真实运行（退出码 0，**180 PASS / 0 FAIL，ALL PASS**）；v1c 段覆盖长度/差异/补丁区/
注入/gate/mask/NOP/原厂自证/flag 行为，最后 `总自检结果: ALL PASS`。

### 4.3 最终产物 sha256（Lead 给定值与独立计算一致）

| 文件 | 大小 | sha256 |
|---|---|---|
| `keypatch/out/V1C_V2.0_keyD798.bin` | 88032 | `86d563c8c3bc5816844b79a2c6303a39910c7bf349d757d47c8ce01bd79409cd` |
| `keypatch/out/V1C_V2.0_keyD798_container.bin` | 88088 | `e1634a089cf87d0fc71668be02447139b3b856bed12c3ef3c2ea63db4d5c3e5d` |

container = 补丁镜像(88032B) + 56B 尾部；CLI 重跑后两个 sha256 均不变。

## 5. 残余风险 / 未验证项

1. **无实机、无整机模拟器**：以上为静态 + 单函数级 Unicorn + CLI 自检；未验证
   - LIGHT 菜单显示（12sec/OFF/ON）、改值/持久化/掉电重启保持；
   - 真机把 LIGHT 设为 ON 后同步字立即变化、与配对引闪器实际引闪；
   - 无线设置同步路径在真机上的实际行为。
2. **0x0800E0A4 恢复默认组合**：如果输入 bitmask 0x300 + [0x200004F0]==1 的
   “恢复 C.Fn 默认”组合在正常使用中可能被触发，F3(LIGHT) 会被清 0（密钥关闭）。
   当前判断为恢复默认语义，保留不 patch 合理；若产品要求密钥持久，需另加 NOP。
3. **v5 去掉 delay(20us)**：原厂写 SYNC 后有 20us 建立 + SCAL + 1ms；v5 直接 pop
   （AD100 v4 布局）。理论可行，但这是相对原厂唯一射频时序差异；真机优先测首包。
4. **升级语义**：原厂 LIGHT 值 2=ON；已有用户若保存 LIGHT=2，升级后自动进入密钥模式，
   普通引闪器无法引闪。发布说明需写明「LIGHT=ON = 密钥模式」，或考虑迁移提示。
   默认 LIGHT=0 → 普通模式；恢复出厂 → LIGHT=0（普通）。
5. **item9(LCD) 改值不触发 sync**：与模块“任意 C.Fn 项”措辞不符；不影响 LIGHT 开关。
6. **mask 后原厂 LIGHT 功能被固定为默认（12sec）**：用户不再能通过 C.Fn 改变造型灯
   模式；菜单里 LIGHT 值被复用为密钥开关（ON=密钥）。
7. **0x0800A6FA NOP 的副作用**：无线设置同步不再更新 LIGHT；若引闪器/上位机有
   C.Fn 同步功能，LIGHT 不会被外部改写（保护目的），但可能与外部 UI 显示不一致。
8. **container 尾部 56B**：本报告只核对 sha 与“镜像+56B 尾部”结构；尾部字段语义
   （CRC/密钥元数据）未深挖；G3 工具校验严格时需按既有设备格式复核。
9. **版本锁定**：本报告只对 V1C V2.0（sha256 4435dd…edfb）与当前 v5-LIGHT 产物有效。

## 6. 建议

- 可交付（静态 + 调用级 PASS）；发布前必须做真机/整机模拟验证：
  LIGHT ON/OFF/12sec 显示与持久化、改 LIGHT 后首包、无线设置同步、
  已有 LIGHT=2 用户的升级迁移。
- 建议在 `devices/v1c/README.md` 用户说明/风险节写明：
  「LIGHT=ON（值 2）= 密钥模式；LIGHT=OFF/12sec=普通」、
  「item9 改值不触发同步」「恢复默认组合会把 LIGHT 清 0」「去掉了 20us 延时」。
- 若产品决定保护 0x0800E0A4，请另开补丁并重跑本审计脚本（写点清单会包含它）。

## 7. 文件清单

```
devices/v1c/re/audit_cfn.py            独立复核脚本（v5-LIGHT；102 项检查；退出码 0）
devices/v1c/re/log_audit_cfn.txt       真实输出（102 PASS + CLI 36 PASS + selftest 180 PASS）
keypatch/dev/v1c/AUDIT_CFN.md          本报告（v5-LIGHT）
```
