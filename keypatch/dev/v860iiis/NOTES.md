# Godox V860III-S V1.1 —— 私有密钥补丁规格（C.Fn `LIGHT` 开关版）

- 设备：Godox V860III-S（索尼口 TTL 机顶闪光灯，2.4 GHz）
- 固件：`devices/v860iiis/firmware/V860IIIS_V1.1.bin`（79,300 B，
  sha256 `bdc8f3547808d121f15a6c58b9363c5792c957c2dce2a16b203276ab078d0b20`）
- 模块：`keypatch/devices/v860iiis.py`
- 补丁形态：**原位补丁，输出长度不变**；共 6 处补丁点
- 用户语义：**C.Fn 第 3 页 `LIGHT`（12sec/OFF/ON）复用为密钥开关**
  - `LIGHT = ON(2)` → CC2500 同步字 = 私有 KEY（只有打了补丁的引闪器能引闪）
  - `LIGHT = 12sec(0，出厂默认) / OFF(1)` → 原厂 `50024/(ID+1)`
- 镜像事实：基址 `0x08003000`，末端 `0x080165C4`；初始 SP=`0x20001548`、Reset=`0x080030D5`；
  settings 基址 `0x20001058`（item n = +n；item6=ID、item8=LIGHT）；
  C.Fn 最大值表 `0x08016102`；Unicorn（M0）验证，无整机模拟器、**未实机**。

## 1. 补丁点总表

| # | 地址 | 原字节 | 新字节 | 语义 |
|---|---|---|---|---|
| 1 | `0x080115B8`..`0x080115EF` | 见 §2 | 见 §2 | 同步字函数 56 B 整段改写：`flag=RAM[0x20001060]`，`flag==2 → sync=KEY`，否则原厂公式；只写 SYNC1/SYNC0（去掉原厂 SCAL+延时） |
| 2 | `0x08005A74` | `02 d1` | `00 bf` | C.Fn 递减 handler `0x080059E4` 内：`bne` 跳过 sync → `nop`，除 item9 外任意项改值都立即重设同步字 |
| 3 | `0x0800C5BE` | `f5 d1` | `00 bf` | C.Fn 递增 handler `0x0800C528` 内：同上 |
| 4 | `0x08003956` | `00 7a` | `00 20` | 造型灯任务 `0x08003950`：`settings[8]` 读者固定 0（LIGHT=ON 不再点亮造型灯） |
| 5 | `0x0800397E` | `01 7a` | `00 21` | 同上，第二个读者固定 0 |
| 6 | `0x0800D0A2` | `26 72` | `00 bf` | RF 设置复位 handler `0x0800CD7C` 的复位分支不再覆盖 `settings[8]`，保护密钥开关不被无线命令静默清 0 |

差异总计 **58 字节**（56 B 函数体里 49 B 实际变化 + 2×2 B gate + 2×2 B 读者 + 2 B 写者），
其余字节与原厂完全一致。`apply()` 前对 6 处原始字节逐一校验，不匹配立即 `ValueError`。

## 2. 同步字函数改写（`0x080115B8`，56 B）

原厂（`0x080115B8`，下一函数 `0x080115F0`）：

```
0c4a      ldr  r2, =50024
10b5      push {r4, lr}
411c      adds r1, r0, #1        ; 除数 = ID+1
1046      mov  r0, r2
f1f796fd  bl   __aeabi_uidiv     ; 0x080030F0
0446      mov  r4, r0
010a      lsrs r1, r0, #8        ; SYNC1
0420      movs r0, #4
02f0acf9  bl   CC2500_WriteReg   ; 0x08013926
e1b2      uxtb r1, r4            ; SYNC0
0520      movs r0, #5
02f0a8f9  bl   CC2500_WriteReg
1420      movs r0, #20
02f05ef8  bl   delay_us          ; 0x08013698
3320      movs r0, #0x33
02f003fa  bl   strobe            ; 0x080139E8 (SCAL)
7d20c000  movs r0, #1000
02f057f8  bl   delay_us
10bd      pop  {r4, pc}
68c30000  .word 50024           ; +0x34（与原厂同址同值保留）
```

补丁后（56 B，44 B 代码 + 3 个字面量）：

```
10b5      push {r4, lr}
0b4b      ldr  r3, =0x20001060  ; C.Fn LIGHT 开关字节（字面量 @+0x30）
0b4a      ldr  r2, =50024       ; 字面量 @+0x34（原厂同址同值）
1b78      ldrb r3, [r3]
411c      adds r1, r0, #1
022b      cmp  r3, #2           ; LIGHT=ON(2)
01d1      bne  go               ; 否则原厂公式
074a      ldr  r2, =KEY         ; 字面量 @+0x2C
0121      movs r1, #1           ; KEY/1 = KEY
1046  go: mov  r0, r2
f1f790fd  bl   __aeabi_uidiv
0446      mov  r4, r0
010a      lsrs r1, r0, #8
0420      movs r0, #4
02f0a6f9  bl   CC2500_WriteReg
e1b2      uxtb r1, r4
0520      movs r0, #5
02f0a2f9  bl   CC2500_WriteReg
10bd      pop  {r4, pc}
<KEY:4> <FLAG=0x20001060:4> <50024:4>
```

去掉了原厂的 `delay(20)+SCAL(0x33)+delay(1000)`：只改同步字、不改频率寄存器，
不需要重新校准；代价与风险见 `devices/v860iiis/README.md` §6.6（同 V1C 结论）。

## 3. 为什么用 C.Fn、为什么选 `LIGHT`

- **菜单项是现成的**：本机 C.Fn 共 10 项可见（`m/ft, AF, STBY, RX STBY, SCAN, CH, ID,
  BEEP, LIGHT, LCD`），无隐藏/预留项；不新增菜单/字库，UI 零改动（见 `CFN_MENU.md`）。
- **`LIGHT` 值域 3 档（12sec/OFF/ON）**，借用后建议只使用 `ON` 作为“开”、
  `12sec/OFF` 都算“关”；出厂默认 0（12sec）→ 默认即普通模式，最安全。
- 代价：原厂「造型灯模式」功能被复用（造型灯输出固定为 12sec 档语义，见 §补丁点 4/5）。
- 若后续想换成 `BEEP` 等 2 档项也可行（settings 字节仍是 `0x20001058+7`），
  但需要把同步字函数里的 `cmp r3,#2` 改成 `cmp r3,#1`，并把 `KEY_FLAG_ON_VALUE` 改 1。

## 4. 改值即时生效（为什么只改 2 个字节）

原厂只有 ID(item6) 改值时才重设同步字；两条改值 handler 在 `cmp r1,#6` 后用 `bne`
跳过 sync 调用。把这 2 字节 `bne` 改成 `nop` 后：除 item9(LCD，原厂在 gate 前已被
`cmp #9` 分支排除、走专用函数 `0x080128F0`) 外，任意 C.Fn 项改值/导航都会落入既有的
`ldrb r0,[r0,#6]; bl 0x080115B8`，于是 LIGHT 开关一改、同步字立即切换。

不需要代码洞注入（本镜像没有像 V1C 那样的大片 safe cave；尾部 0x080162BA 的零区属于
数据段 padding，不如 2 字节 NOP 稳）。行为验证见 `devices/v860iiis/re/cfn_settings.py`：
原厂 item 非 6 时 0 次 sync；补丁后 item 0..8/10/11 各 1 次、r0 == settings[6]，
item9 仍 0 次；SP/r3–r7 原样恢复，非改动路径零 sync。

## 5. 持久化 / 读者 / 写者（为什么开关不会被原厂逻辑改回）

- **持久化**：保存 builder `0x080056D8` 把 `settings[0..13]` 打包进 72 B 记录（s8 在
  record+0x36），save task `0x0800D9C8 → 0x08012F68 → 0x08005EF0` 写到 flash
  `0x0801F000` 起 20 槽（每槽 0x64 B，头字节 0x5C）；load task `0x0800EEB0` 校验后
  展开回 `settings[0..13]`（含 s8），并按最大值表钳位（s8 max=2，密钥值 2 合法）。
  → `LIGHT=ON` 掉电保存，重启后仍是密钥模式。
- **读者**：造型灯任务 `0x08003950` 读 `settings[8]` 驱动造型灯 GPIO（`0x48001000`
  bit13/14 三态）；补丁点 4/5 把它固定 0（= 12sec 默认，不主动点亮）。
- **写者**：RF 消息 handler `0x0800CD7C` 的“设置复位”分支把 `settings[0..9]` 写成默认
  值（s8=0），会静默关掉密钥开关；补丁点 6 NOP 掉对 s8 的写。注意 handler 另一分支
  `0x0800D0DA` 直接调 `0x08005E54` 做**恢复出厂**（会清 s8）——这是用户主动恢复出厂，
  保留原语义、不清除。

## 6. 复现与验证

```powershell
# 仓库根
tools\python\python.exe keypatch\keypatch.py list
tools\python\python.exe keypatch\keypatch.py patch --device v860iiis --key 0xD798 --verify --out %TEMP%\kp_v860iiis
tools\python\python.exe keypatch\selftest.py --key 0xD798
tools\python\python.exe devices\v860iiis\re\cfn_settings.py
tools\python\python.exe devices\v860iiis\re\sync_audit.py
tools\python\python.exe devices\v860iiis\re\cfn_menu.py
```

验收：`patch --verify` 全部 PASS（静态 11 项 + Unicorn 调用级 19 项 + 说明 1 项 + C.Fn handler 级 42 项 = **73 项**）；
`selftest` 中 **v860iiis 段全部 PASS（含 Lead `call_harness` 8/8 交叉验证）**；输出 bin 与原厂等长（79,300 B），
差异只在上述 6 处（58 B）。注意：`selftest.py` 是**全设备**总检，项目里其它设备由别的并行工作线维护，
其状态可能变化，引用时请以 v860iiis 段与 `patch --verify` 的实际输出为准（不要笼统写「selftest ALL PASS」）。

## 7. 已知风险 / 未做项

1. **未实机验证**：CC2500 空口同步、C.Fn 菜单改值、造型灯复用、掉电保存都只在
   Unicorn/静态层验证过；实机请按 `devices/v860iiis/docs/02_验证与实机清单.md` 的安全阶梯做。
2. 去掉 `SCAL+延时` 是相对原厂唯一的射频时序差异；若实机首包异常优先怀疑此处。
3. 复用 `LIGHT` 后原厂造型灯模式功能不再可用（固定 12sec，不主动点亮）。
4. 换密钥需重新生成并刷入；灯与引闪器必须同一把密钥。
5. **恢复出厂会关闭密钥开关**：`0x08005E54` 出厂默认把 `settings[8]` 写 0（=12sec，普通模式），
   属合理语义；之后需手动把 C.Fn `LIGHT` 再设为 `ON`。
6. 本机型 C.Fn 菜单只有 **10 项可编辑（0..9）**；最大值表/持久化记录仍为 12 字节
   （item10/11 无菜单入口、无功能读者），不要按「12 个可编辑项」描述。
7. NOP gate 后**光标移动**（非值编辑）在落到 item6 以外的路径上也会调用一次同步字函数；
   本补丁的同步字函数只写 SYNC1/SYNC0、无 SCAL/无延时，代价是两次寄存器写，可忽略。
