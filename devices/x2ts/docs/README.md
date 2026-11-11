# 神牛 X2T-S 固件研究档案（V1.4 · 私有密钥补丁 · C.Fn 开关版）

> 目标固件：`X2T-S_V1.4.bin`（**58,628 字节**，sha256 `47847644…`）
> 目标设备：神牛（Godox）**X2T-S** 2.4 GHz 引闪器（trigger；具体 MCU 内核/Flash/RAM 容量待确认）
> 本目录内容：X2T-S 的固件登记、补丁语义（C.Fn 菜单项 = 私有密钥开关）、初步镜像事实、
> 离线验证与刷写提示。完整逆向证据见同目录 `../re/` 与 `keypatch/dev/x2ts/`。
>
> 本目录位于仓库 `devices/x2ts/docs/`；仓库根的便携 Python 为 `tools\python\python.exe`
> （从本目录即 `..\..\..\tools\python\python.exe`，含 capstone 5.0.7 / unicorn 2.1.4）。
>
> ⚠️ **状态：补丁语义已定、逆向与补丁模块正在落地；离线验证与实机验证均以实际产出为准。**
> 只研究你本人合法拥有的设备；刷机前务必确认官方 G3 升级软件能识别设备，并保留同版本原厂固件用于回滚。

---

## 0. 一句话结论

X2T-S 是神牛 2.4 GHz 引闪器：固件把**引闪 ID** 换算成 **CC2500 16 位硬件同步字**
（原厂公式 `sync = 50024 / (ID + 1)`，ID = 0..99，只有 100 个公开值），并写入 CC2500 的
`SYNC1/SYNC0`（寄存器 4/5）。

本项目的私有密钥补丁**复用设备自带的 C.Fn 菜单项**作为「私有密钥开关」，不改 UI：

- **开关 = ON** → 同步字 = **私有 KEY**（16 位，如 `0xD798`）；
- **开关 = OFF / 原厂档** → 同步字 = 原厂公式 `50024 / (ID + 1)`（与未打补丁的原厂一致）。

因此：打补丁的引闪器 + 打补丁的灯（同一把 KEY）→ 正常引闪；别人的原厂引闪器只能产生那
100 个公开同步字，**硬件层就对不上**，无法引闪你的灯。

> 具体复用**哪一个** C.Fn 项、它的 `settings[]` RAM 字节地址、最大值/回绕、持久化链路与
> 「改值立即生效」hook 的最终结论见 §3；权威实现是
> [`keypatch/devices/x2ts.py`](../../../keypatch/devices/x2ts.py) 的 `DEVICE["notes"]` 与
> `apply()` / `verify()`，本文件的描述若与模块不一致，以模块为准。

---

## 1. 固件与镜像事实

| 项 | 结论 | 证据 / 备注 |
|---|---|---|
| 设备 | Godox X2T-S（2.4 GHz 引闪器，索尼口机顶安装） | 用户提供 |
| 固件版本 | **V1.4** | 文件内/官方升级包版本 |
| 文件 | `devices/x2ts/firmware/X2T-S_V1.4.bin` | 本机存放，`*.bin` 被 `.gitignore` 忽略 |
| 大小 | **58,628 字节（0xE504）** | 文件长度 |
| sha256 | `478476447ff8d55496180facf578662dcbf9a1af0cf21f79df048b7df0a76efc` | `Get-FileHash` / `keypatch.py list` |
| 镜像基址 | **`0x08003000`**（前 12 KB 为厂商 bootloader，不在此 `.bin` 内） | 补丁引擎 `DEVICE["base"]`；`file_offset = vaddr - base`；由 Reset_Handler/SystemInit 与尾部指针 `0x08011504` 交叉证实 |
| 镜像范围 | `0x08003000–0x08011503`（文件 58,628 B = 0xE504） | 文件长度 + 基址；尾部字面量 `0x08011504` 恰为镜像结束地址 |
| 向量表 | 初始 SP = `0x20022038`；Reset = `0x080031AD` | 镜像前 8 字节（小端），见 `re/recon1.py` |
| 内核 | 含硬件除法 `UDIV`（Thumb-2），属 Cortex-M3/M4/M7 一级；**具体型号待确认** | `re/recon1.py` 的 UDIV 统计 |
| Flash / RAM | **待确认**（容量未实测） | 初始 SP 指向 RAM 高地址，仅作线索 |
| 射频 | CC2500 兼容（同步字写寄存器 4/5；`WriteReg`/`strobe`/`delay` 与同族设备同构） | §2 |
| 开关位置 | C.Fn 菜单项（复用；具体项/地址由 C.Fn 逆向交付确认） | §3 |

> 「待确认」不是保守措辞：本轮重点是与**密钥开关**相关的静态分析（同步字函数、CC2500 写点、
> C.Fn 子系统、代码洞），整机启动链、显示与 Flash 参数区的完整逆向仍需补做。

---

## 2. 镜像里的「ID → 同步字」链路（初步侦察）

同步字函数初步定位为 **`0x0800C8F8`**（文件偏移 `0x098F8`；到下一函数 `0x0800C92C` 为止，52 B），
原厂形态与 AD100 / AD200 / V1C 同一套写法：

```asm
0x0800C8F8  10 b5        push  {r4, lr}
0x0800C8FA  4c f2 68 31  movw  r1, #0xC368      ; r1 = 50024（原厂分子）
0x0800C8FE  40 1c        adds  r0, r0, #1        ; r0 = ID + 1
0x0800C900  b1 fb f0 f4  udiv  r4, r1, r0        ; r4 = 50024 / (ID + 1)
0x0800C904  21 0a        lsrs  r1, r4, #8        ; SYNC1 = sync >> 8
0x0800C906  04 20        movs  r0, #4
0x0800C908  ...          bl    WriteReg          ; CC2500_WriteReg(4, SYNC1)
0x0800C90C  e1 b2        uxtb  r1, r4            ; SYNC0 = sync & 0xFF
0x0800C90E  05 20        movs  r0, #5
0x0800C910  ...          bl    WriteReg          ; CC2500_WriteReg(5, SYNC0)
0x0800C914  14 20        movs  r0, #0x14         ; delay(20)
0x0800C91A  33 20        movs  r0, #0x33         ; strobe(SCAL)
0x0800C920  bd e8 10 40  pop.w {r4, lr}
0x0800C924  4f f4 7a 70  mov.w r0, #0x3E8        ; delay(1000) 后返回
0x0800C928  ...          b.w   Delay
```

辅助函数（初步确认）：

| 符号 | 地址 | 说明 |
|---|---|---|
| `CC2500_WriteReg` | `0x0800E842` | `r0`=寄存器地址、`r1`=值 |
| `strobe` | `0x0800E932` | `r0`=命令字节（`0x33`=SCAL） |
| `delay` | `0x0800E5AA` | 忙等延时（`r0`=计数值） |

补充静态事实（`re/recon1.py` / `re/recon3.py` / `keypatch/dev/x2ts/`）：

- re-sync 已确认**真实链接基址 `0x08003000`**：向量表在 `0x08003000`（初始 SP=`0x20022038`/
  Reset=`0x080031AD`），镜像尾部字面量 `0x08011504` 恰为 `0x08003000 + 0xE504`（镜像结束地址）；
  `re/recon1.py`、`re/recon3.py` 早期按 `0x08000000` 记录的地址需按 **+0x3000** 换算；
- 全镜像**没有** `50024 = 0x0000C368` 的 4 字节字面量，`68 C3` 小端字节对也不存在 ——
  原厂公式以 `movw r1,#0xC368` 立即数形式存在（全镜像 `movw #0xC368` 只有这一次）；
- 初步扫到的该函数 **BL 调用点**：`0x08005F72`、`0x0800ADC4`、`0x0800C7F8`
  （`0x0800C7F8` 所在函数先比较设置区里的 ID 再调用）；完整调用图与每个调用点的 ID 来源、
  「改 ID/改 C.Fn 后何时重设同步字」的结论由 RF 审计交付，见 `../re/`（`sync_` 前缀）与
  `keypatch/dev/x2ts/`；
- 补丁原型见 `keypatch/dev/x2ts/sync_patch_preview.py`：**52 B 整函数改写**（`movw #KEY` /
  读 C.Fn 标志 / 原厂 `udiv` 公式二选一），并 `b.w` 到同 SDK 频道设置函数的尾部
  **`0x0800C8DC`**（`delay(20)` + `SCAL(0x33)` + `delay(1000)`），保留原厂射频校准收尾。
  **以上为初步结果，以最终审计与补丁模块为准。**

---

## 3. 补丁语义：C.Fn 菜单项 = 私有密钥开关

### 3.1 用户可见语义

**复用项 = C.Fn 第 3 项 `BEEP`（原厂蜂鸣开关，OFF/ON）**——即 C.Fn 值数组
`C.Fn[2] = RAM 0x200203E2`（数组基址 `0x200203E0`，最大值表 `0x20020072[2] = 1`；
该值随原厂 C.Fn 设置一起保存到 flash 参数记录，重启后保留）。

| C.Fn BEEP | 同步字 | 模式 | ID 菜单 |
|---|---|---|---|
| **ON（值 1）** | **私有 KEY**（如 `0xD798`） | **密钥模式**：只有同一把 KEY 的补丁设备能互通 | 原厂含义（普通模式的 ID 仍可用） |
| **OFF（值 0，出厂默认）** | `50024 / (ID + 1)` | 普通模式：与未打补丁的原厂一致 | 原厂含义 |

要点：

- 补丁**不新增**菜单项、不改字库/UI，只是复用现有 C.Fn `BEEP` 项 —— 用户在原厂菜单里就能开/关；
- 密钥模式与普通模式**可以随时切换**，不需要重新刷机；
- 引闪器与灯必须刷**同一把 KEY** 的固件；换密钥 = 重新生成并刷入全部设备；
- 被复用项的**原厂功能代价**：BEEP=ON 时密钥模式生效，同时原厂蜂鸣仍按原厂逻辑工作
  （未屏蔽 BEEP 读者，属已知副作用；不影响射频/引闪）。

### 3.2 补丁实现

补丁形态属于本项目已有的「原位补丁 + 注入」家族（输出长度保持 **58,628 B**，官方 G3 可直接刷写）：

1. **同步字函数 52 B 整函数改写**（`0x0800C8F8..0x0800C92B`）：
   `flag = C.Fn[2]; if (flag == 1) sync = KEY; else sync = 50024/(ID+1);`
   写 `SYNC1/SYNC0` 后 **`b.w 0x0800C8DC`** 跳回同 SDK 频道设置函数的原厂尾部
   （`delay(20)` + `SCAL(0x33)` + `pop {r4,lr}` + `delay(1000)`），**保留完整原厂射频校准时序**；
2. **改 C.Fn 值即时生效 hook（4 个）**：原厂只有 CH(5)/ID(6)/DIST(0xA) 会在菜单里触发 RF 重设，
   BEEP 等项直接写数组、不重设同步字。补丁把 C.Fn 加/减处理函数的普通/回绕两条通路
   （`0x08009232`、`0x08005C8E`、`0x0800923A`、`0x08005CD4`）各换成 `BL hook`，
   hook 执行被搬走的 2 条原指令后调用 `sync(C.Fn[6])`（同步字函数会重新读 BEEP 开关），
   再恢复现场返回 —— 因此切开关**立即生效**，无需重启；
3. **hook 注入位置 = 未使用的中断向量槽**：`0x0800306C`（16 B）、`0x0800307C`（14 B）、
   `0x0800308C`（16 B）、`0x080030A0`（14 B）。这些槽原值均为默认陷阱 `0x080031C7`
   （对应 IRQ 从未使能），不是数据/代码洞，属安全可用的保留槽；
4. 不新增代码洞、不改 .data/.bss、不改尾部启动初始化表；与原厂差异恰为补丁点集合。

> 补丁模块 [`keypatch/devices/x2ts.py`](../../../keypatch/devices/x2ts.py) 的 `apply()` / `verify()`
> 是**唯一权威实现**；逆向证据与复现脚本见 `devices/x2ts/re/`（`recon1..recon17`、`sync_*`）与
> `keypatch/dev/x2ts/`。

---

## 4. 补丁模块与输出

| 项 | 值 |
|---|---|
| 补丁模块 | [`keypatch/devices/x2ts.py`](../../../keypatch/devices/x2ts.py)（`DEVICE` / `apply()` / `verify()`） |
| 设备 id | `x2ts`（注册进 `keypatch/devices/__init__.py` 的 `DEVICE_IDS`） |
| 输出命名 | `X2T-S_V1.4_key{key:04X}.bin`；G3 容器备用件 `..._container.bin`（`--container`） |
| 输入 | 原厂 `devices/x2ts/firmware/X2T-S_V1.4.bin`（sha256 必须匹配，否则拒绝打补丁） |
| 输出 | 与输入**等长**（58,628 B），仅补丁点字节变化 |

---

## 5. 离线验证（可复现）

```powershell
# 仓库根
tools\python\python.exe keypatch\keypatch.py list
tools\python\python.exe keypatch\keypatch.py patch --device x2ts --key 0xD798 --verify --container --out %TEMP%\kp_x2ts

# 模块级（等价于上面 verify 步骤）
tools\python\python.exe -c "import sys; sys.path.insert(0,'keypatch'); from devices import x2ts; import common; raw=common.read_file('devices/x2ts/firmware/X2T-S_V1.4.bin'); patched,log=x2ts.apply(raw,0xD798); [print(('PASS' if ok else 'FAIL'),msg) for ok,msg in x2ts.verify(patched,0xD798)]"
```

`verify()` 预期覆盖（与 AD100 / AD200 / V1C 同风格）：

- **静态**：输入/输出长度不变；补丁点原始字节逐字节校验；与原厂差异集合恰为补丁点集合；
- **Unicorn 调用级**：直接执行补丁后的机器码，断言
  - 开关 = ON → `WriteReg(4, KEY>>8)`、`WriteReg(5, KEY&0xFF)`；
  - 开关 = OFF/原厂档 → 原厂 `50024/(ID+1)` 的 `SYNC1/SYNC0`；
  - 4 个 hook 入口执行后 → C.Fn[2] 按预期写入、同步字立即重算（含 KEY/OFF 两种方向）；
- **原厂自证**：同一 harness 跑原厂镜像 → 原厂公式 + `delay(20)+SCAL(0x33)+delay(1000)`。

当前模块实测：**30 项验证全部 PASS**（`tools\python\python.exe keypatch\keypatch.py patch --device x2ts --key 0xD798 --verify`）。

> `verify()` 只证明「补丁后的机器码按预期写 SYNC1/SYNC0」；真正的空口同步过滤、
> 菜单显示/持久化与实机行为必须真机验证（见 §6）。密钥换成别的值后输出 sha256 会变，需重新生成。

---

## 6. 刷写与实机验证提示（风险自负）

### 6.1 刷写

1. 用官方 **G3 升级软件**：打开文件 → 选生成的 `.bin`（或容器备用件）→ 更新；
2. 刷前确认：模块 `sha256` 与本机固件匹配（`keypatch.py list` 显示 `sha256 OK`）；
3. 保留同版本原厂 `X2T-S_V1.4.bin` 用于回滚；刷写过程中不要断电；
4. 引闪器与所有灯必须刷**同一把 KEY**；否则密钥模式下互相引闪不了。

### 6.2 实机安全阶梯（建议顺序）

1. **原厂基线**：确认官方 G3 能识别 X2T-S、能从官方渠道重下 V1.4 原厂固件；
2. **刷补丁版**：刷入 `X2T-S_V1.4_keyD798.bin`，确认能开机、菜单正常、版本号 V1.4；
3. **C.Fn 开关确认**：进 C.Fn 菜单找到被复用的开关项，确认能在 ON / 原厂档之间切换，
   且新刷/恢复出厂后默认是**原厂档（普通模式）**；
4. **普通模式**：开关 = OFF/原厂档，ID = 1..99，与原厂引闪器/灯同 CH/ID，确认能正常引闪；
5. **密钥模式**：开关 = ON + 已打补丁的灯（同一把 0xD798）→ 确认能引闪；
6. **隔离验证**：把引闪器切回普通模式（或换回原厂引闪器）→ 密钥灯应**不被引闪**；
7. **即时生效**：只来回切 C.Fn 开关，确认同步字立即切换（不需要重启）；
8. **持久化**：重启设备后开关状态仍在；恢复出厂后回到原厂档（普通模式）；
9. 任意一步异常 → 立刻刷回原厂 `X2T-S_V1.4.bin`（详见 [`keypatch/README.md`](../../../keypatch/README.md)）。

### 6.3 已知风险 / 待确认

| 项 | 状态 | 说明 |
|---|---|---|
| 实机验证 | ❌ 未做 | 离线验证通过 ≠ 空口行为正确；必须按 §6.2 实测 |
| C.Fn 项选择 / 原厂功能代价 | ✅ 已定 | 复用 **C.Fn 第 3 项 `BEEP`**（`C.Fn[2] = 0x200203E2`，ON=1）；代价 = BEEP=ON 时原厂蜂鸣仍会响（未屏蔽读者），无射频副作用 |
| 改 C.Fn 即刻生效 hook | ✅ 已实现 | 4 个 hook 注入未用中断向量槽 `0x0800306C/7C/8C/A0`，覆盖加/减的普通+回绕通路 |
| 代码洞 / 注入区 | ✅ 已定 | 不新增代码洞；复用未用中断向量槽（原值均为默认陷阱 `0x080031C7`）；同步字函数 52 B 整段改写（含原厂尾部跳转） |
| 同步字函数 | ✅ 已定 | `0x0800C8F8..0x0800C92B`（52 B，下一函数 `0x0800C92C`）；3 个调用点 `0x08005F72 / 0x0800ADC4 / 0x0800C7F8` |
| 启动链 / .data/.bss | ✅ 已定 | 0x080031D0 解释表 0x080114A8：解压 0x080114C8→0x20020000(.data 0x188 B)、memzero 0x20020188→0x20022038(.bss 0x1EB0 B) |
| Flash / RAM / 显示 | ⏳ 部分确认 | RAM 布局与 C.Fn 数组/最大值表已确认；Flash 容量、显示子系统未做整机逆向 |
| 恢复出厂会关密钥 | ℹ️ 预期行为 | 恢复出厂把 BEEP 清回原厂档 → 普通模式，语义合理 |
| 密钥强度 | ⚠️ 非密码学 | 16 位同步字挡的是「正常原厂设备误触发」，不防专业攻击/SDR 克隆 |

> 补充：正常模式下 BEEP=OFF（出厂默认）即普通模式；把 BEEP 设为 ON 就是密钥模式。
> 菜单里切 BEEP 会立即重设同步字（`verify` 已对 4 个 hook 做 Unicorn 行为验证）。

---

## 7. 文件地图与相关文档

```text
devices/x2ts/
├─ docs/README.md              ← 本文（设备说明 + 补丁语义 + 验证/刷写提示）
├─ firmware/X2T-S_V1.4.bin     ← 原厂固件（本机，*.bin 被 .gitignore 忽略）
└─ re/                         ← 静态复现脚本（recon1.py：镜像/向量表/常量；recon3.py：同步字函数）
```

相关：

- 补丁实现：[`keypatch/devices/x2ts.py`](../../../keypatch/devices/x2ts.py)
- 设备模块接口：[`keypatch/INTERFACE.md`](../../../keypatch/INTERFACE.md)
- 刷机 / 回滚 / 救砖：[`keypatch/README.md`](../../../keypatch/README.md)
- 全仓库设备速查：[`docs/设备清单.md`](../../../docs/设备清单.md)
- 新设备接入方法：[`docs/新增设备指南.md`](../../../docs/新增设备指南.md)
- 同类引闪器参考：[`devices/x3c/`](../../x3c/)（X3-C：双发密钥方案）

---

## 8. 变更记录

| 日期 | 变更 |
|---|---|
| 2026-11-11 | 建立本档案：登记 X2T-S V1.4 固件（58,628 B / sha256 `47847644…` / base **`0x08003000`**）；确认补丁语义 = 复用 C.Fn 菜单项作为私有密钥开关（ON=私有 KEY，OFF/原厂档=`50024/(ID+1)`）；初步定位同步字函数 **`0x0800C8F8`**（文件偏移 `0x098F8`）；接入 studio（设备卡片、文档矩阵、API 测试） |
| 2026-11-11 | **补丁定稿**：复用项 = **C.Fn 第 3 项 `BEEP`**（`C.Fn[2]=0x200203E2`，ON=1）；同步字函数 `0x0800C8F8` 52 B 整段改写（尾部 `b.w 0x0800C8DC` 保留原厂 delay(20)+SCAL+delay(1000)）；4 个 C.Fn 加/减即时生效 hook 注入未用中断向量槽 `0x0800306C/7C/8C/A0`；输出 `keypatch/out/X2T-S_V1.4_keyD798.bin`（等长 58,628 B），`verify` **30 项 PASS**、`keypatch.py list` 显示 sha256 OK |
