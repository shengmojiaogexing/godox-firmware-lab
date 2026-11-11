# keypatch/dev/v1s/NOTES.md —— V1S V1.8 补丁设计 / 证据 / 验证记录

> 设备：Godox **V1S**（V1 的索尼版闪光灯）· 固件 `V1S_V1.8.bin`（78,500 B，sha256 `87a06000…`，base `0x08003000`）
> 模块：[`keypatch/devices/v1s.py`](../../devices/v1s.py) · 设备档案：[`devices/v1s/README.md`](../../../devices/v1s/README.md)
> 独立分析报告：[`devices/v1s/re/SYNC_ANALYSIS.md`](../../../devices/v1s/re/SYNC_ANALYSIS.md)（89 断言）、
> [`devices/v1s/re/CFN_ANALYSIS.md`](../../../devices/v1s/re/CFN_ANALYSIS.md)（144 断言）

状态：**离线验证 38 项 PASS、Lead call_harness PASS、selftest ALL PASS；无整机模拟器、无实机验证。**

---

## 1. 用户语义

复用灯端 **C.Fn 第 3 页 `LIGHT`**（原厂造型灯模式 `12sec`/`OFF`/`ON`）作为私有密钥开关：

| C.Fn LIGHT | `settings[8]` = RAM `0x2000105C` | 同步字 |
|---|---|---|
| `ON` | 2 | **私有 KEY**（16 位；只有刷了同一 KEY 的引闪器能引闪） |
| `12sec`（出厂默认） | 0 | 原厂 `50024/(ID+1)` |
| `OFF` | 1 | 原厂 `50024/(ID+1)` |

LIGHT 值随原厂 C.Fn 记录持久化（save `0x080056F0` / load 展开 `0x0801383C`），
出厂默认 0 ⇒ 刷完补丁不动菜单 = 原厂行为，风险最低。

## 2. 补丁形态（5 处原位补丁，长度不变）

| # | 地址 | 原厂 → 补丁 | 说明 |
|---|---|---|---|
| 1 | `0x080112DC`（56 B 到 `0x08011313`） | 原厂 20 条指令 → 44 B 代码 + 3 个字面量 | 同步字函数整段改写 |
| 2 | `0x08005A8A`（4 B） | `06 29 02 d1` → `00 e0 00 bf` | C.Fn 递减门：`b 0x08005A8E; nop` |
| 3 | `0x0800C3E8`（4 B） | `06 29 f5 d1` → `00 e0 00 bf` | C.Fn 递增门：`b 0x0800C3EC; nop` |
| 4 | `0x08003956`（2 B） | `00 7a` → `00 20` | 造型灯读者 #1 → 常数 0（12sec） |
| 5 | `0x0800397E`（2 B） | `01 7a` → `00 21` | 造型灯读者 #2 → 常数 0（12sec） |

补丁后与原厂差异 **60 字节**，只落在补丁点；输出长度 78,500 B 不变。

### 2.1 同步字函数替换体（44 B 代码 + 3 字面量）

```asm
; 0x080112DC
push {r4, lr}
ldr  r3, =FLAG(0x2000105C)    ; 字面量 +0x30
ldr  r2, =50024               ; 字面量 +0x34（原厂同址同值）
ldrb r3, [r3]                 ; C.Fn LIGHT 值
adds r1, r0, #1               ; 普通模式除数 = ID+1
cmp  r3, #2                   ; LIGHT=ON ?
bne  original
ldr  r2, =KEY                 ; 字面量 +0x2C
movs r1, #1                   ; 密钥模式除数 = 1
original:
mov  r0, r2
bl   __aeabi_uidiv            ; 0x080030F0
mov  r4, r0
lsrs r1, r0, #8               ; SYNC1
movs r0, #4
bl   0x0801365A               ; CC2500_WriteReg
uxtb r1, r4                   ; SYNC0
movs r0, #5
bl   0x0801365A
pop  {r4, pc}
.word KEY          ; +0x2C  = 0x08011308
.word FLAG         ; +0x30  = 0x0801130C
.word 50024        ; +0x34  = 0x08011310（原厂同址同值）
```

不保留原厂的 `delay(20)+SCAL(0x33)+delay(1000)`（理由/风险见 §5）。

### 2.2 改值即时生效：为什么用「4 B 原地分支」而不是 V1C 的 bl 注入

V1S 两个 C.Fn handler 的门是 `cmp r1,#6; bne <跳过同步块>`：
`bne` 的 fall-through **就是** 原厂同步块（`ldrb r0,[r0,#6]; bl 0x080112DC`）。
因此把门改成 `b <同步块>; nop`（`00 e0 00 bf`）即可让除 item9(LCD) 外的所有 C.Fn 改值立即重设同步字：

- 不需要代码洞（本固件**没有**可安全注入的 ≥32 B 全零死区，见 CFN_ANALYSIS §9）；
- 不能用 V1C 式 `bl hook`：fall-through 本来就会再调一次 sync → 重复调用（虽然幂等，但没有必要）；
- gate 进入时 `r0 = settings 基址 0x20001054`、`r1 = 当前项索引（[0x200004D0]）`（CFG 反向数据流已证）。

## 3. 读者屏蔽 / 为什么不做出厂复位保护

| 路径 | 地址 | 处理 |
|---|---|---|
| 造型灯任务 `0x08003950` 读 settings[8] | `0x08003956` / `0x0800397E` | **mask 成 0**（固定 12sec 出厂默认） |
| 菜单显示 `0x0800431C` 读 settings[8] | `0x08004A8A` / `0x08004A9A` | 不改（用户要能看到/切换 LIGHT） |
| save 记录 `0x080056F0` 读 settings[8] | `0x0800582E` | 不改（持久化用户值） |
| 出厂默认 `0x08005E68` 写 settings[8]=0 | `0x08005EC4` | 不改 |
| 按键工厂复位组合写 settings[8]=0 | `0x0800CEAA` | **不改** |
| 上电 load 展开写 settings[8] | `0x0801397C` | 不改（从 flash 记录恢复） |

- V1S **没有** V1C 那种「无线设置同步解包写回 settings[8]」的远程覆盖路径（CFN_ANALYSIS §7），
  所以不需要 V1C 的 `0x0800A6FA` 型防清零补丁；
- **有意保留**「出厂复位/恢复默认 → LIGHT=0 → 普通模式」这条重要恢复路径（忘记密钥时可用），
  与 V1C 语义一致（V1C 的出厂默认同样不清保护）。

## 4. 关键常量

```python
BASE        = 0x08003000
IMAGE_SIZE  = 78500
SHA256      = "87a0600046e1b7c2198a7ac8fac15959bb3a5d582edf44d8e0df38706dcb917a"
SYNC_FN     = 0x080112DC ; SYNC_FN_END = 0x08011314   # 56 B
DIV_FUNC    = 0x080030F0  # __aeabi_uidiv
WRITE_REG   = 0x0801365A  # CC2500_WriteReg(reg, val)
WRITE_CMD   = 0x0801371C  # strobe(0x33=SCAL，补丁版不调用)
DELAY_US    = 0x080133CC  # delay_us（补丁版不调用）
SETTINGS_BASE = 0x20001054 ; ID_IDX = 6 ; FLAG_IDX = 8
KEY_FLAG_ADDR = 0x2000105C ; KEY_FLAG_ON_VALUE = 2
CFN_MAX_TABLE_ADDR = 0x08015DE6  # 12 项 max/回绕表
GATE_DEC = 0x08005A8A ; GATE_INC = 0x0800C3E8
```

## 5. 风险与未验证项

1. **无实机 / 无整机模拟器**（最高风险）；release 前必须真机按 `devices/v1s/docs/02_验证与实机清单.md` 走阶梯。
2. 去掉 `SCAL(0x33)+1ms` 是唯一射频差异（SYNC1/0 是纯数据寄存器；上电路径 init 表/RF init 已各 SCAL 一次）。
   若真机首包丢失优先怀疑此处；回退方案见 `devices/v1s/README.md` §4.1。
3. 造型灯原厂功能被复用（固定 12sec 档），需在用户文档里说明。
4. 固件版本锁定：sha256 + 每处补丁点原始字节双重校验；换版本必须重跑 `devices/v1s/re/` 下两个脚本重新定位。
5. `item10/11` 语义未定性（V1S 中除 save/load 外无功能/显示读者），本补丁未使用。

## 6. 复现 / 验证命令

```powershell
# 仓库根
tools\python\python.exe keypatch\keypatch.py patch --device v1s --key 0xD798 --verify --out %TEMP%\kp_v1s
tools\python\python.exe keypatch\keypatch.py patch --device v1s --key 0xD798 --container --out %TEMP%\kp_v1s
tools\python\python.exe keypatch\dev\lead\call_harness.py        # v1s 段：ALL PASS
tools\python\python.exe keypatch\selftest.py --key 0xD798        # ALL PASS
tools\python\python.exe devices\v1s\re\sync_v1s.py               # 89 PASS
tools\python\python.exe devices\v1s\re\cfn_v1s.py                # 144 PASS
```

示例产物（KEY=0xD798）：`V1S_V1.8_keyD798.bin`，78,500 B，
sha256 `3baa5dc8f76bc3daed5edefeb2d7761041de8c27e6a89b836b7b609f60b037cd`。

## 7. 独立审计交叉核对（2026-11-11）

- 两名独立分析员（sync-analyst / cfn-analyst）分别从同步字路径与 C.Fn 路径独立定位并**逐字节复核**；
  本模块的 5 个补丁点地址、原厂字节、FLAG 地址、gate 语义与两份报告完全一致，未发现冲突。
- 差异处理：cfn 报告建议 gate 改 `00 e0 00 bf`（本模块采用）；出厂复位路径 `0x0800CEAA`
  经确认是**用户主动恢复默认**（非远程覆盖），本模块**不加保护**（保留恢复路径）。
