# AD300 Pro V1.30 逆向工作笔记（keypatch/dev/ad300）

> 目标：把「ID → CC2500 同步字」补丁接到 AD300 Pro V1.30，并用 **C.Fn 第 8 项 `F8`**
> 做密钥开关（沿用 AD200 Pro 的开关形态）。实现见
> [`keypatch/devices/ad300.py`](../../devices/ad300.py)，设备档案见
> [`devices/ad300pro/README.md`](../../../devices/ad300pro/README.md)，
> 独立复核见 [`AUDIT.md`](AUDIT.md)。

固件：`devices/ad300pro/firmware/AD300pro_V1.30.bin`，44,220 B，
sha256 `e3be6a63581adb2b3d3225286706ee14d185f8a8ddd18c50ea50069a98765932`。

---

## 0. 复跑脚本

| 脚本 | 作用 |
|---|---|
| `scan1.py` | 镜像头/基址、50024 字面量、全 0/全 FF 死区（结论：无注入死区，必须整函数改写） |
| `scan2.py` | sync 函数周围反汇编 |
| `scan3.py` | 全镜像 BL 解码 → sync 调用点 / 辅助函数引用点 |
| `scan4.py` | 4 个 sync 调用点反汇编 |
| `scan5.py` | C.Fn handler 反汇编 + 字面量池解析（UI base / settings base） |
| `scan6.py` | 最大值表 `0x0800DA64` + settings 基址引用点 |
| `scan7.py`/`scan8.py` | settings 基址所有 LDR 引用 → 符号化追踪 `settings[0..7]` 访问 |
| `scan9.py` | F8(settings[7]) 读者/持久化函数反汇编 |
| `scan10.py` | 交叉校验所有 `ldrb/strb [rY,#7]`；BL → 保存/加载/默认值函数；记录 builder |
| `scan11.py`/`scan12.py` | 可疑 settings[7] 访问点复核（确认 0x0800AEA0 是第 6 个读者） |
| `audit_cfn.py` | 独立复核脚本（审计员产出，可一键复跑全部证据） |

运行：`tools\python\python.exe keypatch\dev\ad300\<脚本>.py`。

---

## 1. 基址与函数定位

- `word0 = 0x200008F8`（初始 SP）、`word1 = 0x080030D5`（Reset，Thumb）
  → **base = 0x08003000**（前 12 KB bootloader 不在 bin 里）。
- `0xC368`（50024，小端 `68 c3 00 00`）在镜像里**只出现一次**：文件偏移 `0x857C`
  → 虚拟地址 `0x0800B57C`。反汇编该处附近：

```text
0x0800B548: ldr  r2, [pc, #0x30]   ; r2 = 50024
0x0800B54A: push {r4, lr}
0x0800B54C: adds r1, r0, #1        ; ID + 1
0x0800B54E: mov  r0, r2
0x0800B550: bl   0x080030F0        ; __aeabi_uidiv
0x0800B554: mov  r4, r0
0x0800B556: lsrs r1, r0, #8
0x0800B558: movs r0, #4
0x0800B55A: bl   0x0800C47A        ; WriteReg(4, SYNC1)
0x0800B55E: uxtb r1, r4
0x0800B560: movs r0, #5
0x0800B562: bl   0x0800C47A        ; WriteReg(5, SYNC0)
0x0800B566: movs r0, #0x14
0x0800B568: bl   0x0800C200        ; delay_us(20)
0x0800B56C: movs r0, #0x33
0x0800B56E: bl   0x0800C53C        ; strobe(SCAL)
0x0800B572: movs r0, #0x7d
0x0800B574: lsls r0, r0, #3        ; 1000
0x0800B576: bl   0x0800C200        ; delay_us(1000)
0x0800B57A: pop  {r4, pc}
0x0800B57C: .word 50024
0x0800B580: （下一函数）
```

结论：**SYNC_FUNC = 0x0800B548，56 B（0x0800B548..0x0800B57F）**；`50024` 字面量在
`+0x34`，与 AD200 Pro / V1C 布局一致；下一函数 `0x0800B580`，整函数改写不会越界。
镜像里没有任何 ≥32 B 的全 0 / 全 FF 死区 → **不用注入方案，直接整函数改写**。

BL 扫描（`scan3.py`）：sync 调用点 4 个 —— `0x0800467A`、`0x080048CE`、`0x08008428`、
`0x08008872`；追调用点上下文，入参都是 `ldrb rX,[settings_base,#6]`（ID）。

## 2. settings 基址与 C.Fn 结构

- C.Fn 递减 handler 入口 `0x080045E8`：首条 `ldr r4,[pc,#0x214]` → **UI base = 0x20000190**；
  `0x08004666: ldr r1,[pc,#0x1ac]` → **settings base = 0x2000048C**；
  编辑值路径 `0x0800465E`：`settings[item]--`（item = `[UI+0x17]`），
  gate `0x08004674: cmp r2,#6; bne return`，通过后 `bl sync(settings[6])`。
- C.Fn 递增 handler 入口 `0x08008388`，同构：gate `0x08008422: cmp r0,#6; bne return`，
  `0x08008410: ldr r4,=0x0800DA64`（最大值表）。
- item 导航回绕：`0x08004680`（dec：item==0 → 7）、`0x08008436`（inc：item+1>7 → 0）
  ⇒ **C.Fn 共 8 项（item 0..7）**。
- 最大值表 `0x0800DA64` 前 8 字节 `01 02 03 96 02 01 63 01`：
  item6 最大 `0x63`=99 → **item6 = ID**；item7 最大 1 → **item7 = F8（OFF/ON）**。
- 默认值初始化 `0x080048EC`，其中 `0x0800491A: ldr r1,=settings` 后写入
  `1,0,2,0,0,0,0,0` → 出厂 `settings[7] = 0`（F8=OFF，普通模式）。

## 3. F8（settings[7] = RAM 0x20000493）的全部访问

符号化追踪（`scan8.py`）漏掉了 `0x0800AEA0`（同一字面量的非最近 ldr 引用），
用 `scan10.py` 交叉扫描所有 `ldrb/strb rX,[rY,#7]` 后逐个复核（`scan11.py`/`scan12.py`）：

| 地址 | 指令 | 分类 | 语义 |
|---|---|---|---|
| `0x08004B08` | `ldrb r1,[r1,#7]` | 读者 | 显示/数值表：F8=1 切备用时长表 |
| `0x08004DC8` | `ldrb r0,[r0,#7]` | 读者 | 显示页：F8=1 清 UI bit4/改页面标志 |
| `0x08005980` | `ldrb r1,[r1,#7]` | 读者 | 测量/待机路径：F8=1 切备用表 |
| `0x08006062` | `ldrb r2,[r2,#7]` | 读者 | level 查表：F8=1 切备用标定表（`[0x2000015C]`） |
| `0x0800ADBC` | `ldrb r3,[r3,#7]` | 读者 | muldiv/缩放：F8=1 跳过功率/时长缩放 |
| `0x0800AEA0` | `ldrb r2,[r2,#7]` | 读者 | RF 远程设置：F8=1 不改设置分叉 |
| `0x0800421C` | `ldrb r0,[r4,#7]` | 读者（保留） | C.Fn 渲染：画 F8 的 ON/OFF 图标，必须保留 |
| `0x080044C4` | `ldrb r2,[r2,#7]` | 读者（保留） | 参数记录 builder：把 `settings[4..7]` 打包进记录 |
| `0x0800492A` | `strb r0,[r1,#7]` | 写者 | 默认值初始化（=0） |
| `0x08004672`/`0x08008418`/`0x08008420` | `strb rX,[r1,rItem]` | 写者 | C.Fn 改值（通用下标，item7 即 F8） |
| `0x0800C8D0` | `strb r0,[r2,#7]` | 写者（保留） | 参数加载：从记录回填 settings |
| `0x0800B6C8` | `strb r1,[r2,#7]` | 写者（**必须 NOP**） | 退出/模式切换路径静默清 0 |

其它 `[rX,#7]` 命中（`0x08003C84`、`0x08003CDC`、`0x0800478C`、`0x08005A48`、
`0x08005A90`、`0x08008532`、`0x0800A0C0`、`0x0800BE74`）经反汇编确认访问的是别的
RAM 结构（UI/显示状态），不是 settings 基址。

读者语义与 AD200 Pro 的 6 处「稳定色温」读者一一对应（显示时长表、显示页、测量换算、
level 查表、muldiv 缩放、RF 远程设置）→ **F8 原厂功能 = 稳定色温模式**。

## 4. 持久化链路

- 参数记录 builder `0x08004410`（纯 RAM）：把 `settings[0..3]` 打包到记录 `+0x18`、
  `settings[4..7]` 打包到 `+0x1C`（**F8 在记录 `+0x1F`，即 +0x1C 字最高字节**）。
- 保存 `0x0800B38C`：`builder → 0x20000460 → 拷贝到 0x20000440 → flash 0x0801F000 + slot*100`。
- 加载 `0x0800C814`：从 `0x20000440`（记录）回填 UI 与 `settings[0..7]`。
- ⇒ F8 随原厂参数记录掉电保存，与 AD200 Pro 的 F8 持久化方式一致。

## 5. 补丁设计

1. **sync 函数 56 B 整函数改写**（44 B 代码 + KEY/FLAG/50024 三字面量，布局同 AD200）：

   ```c
   flag = *(uint8_t*)0x20000493;   // C.Fn F8
   if (flag == 1) sync = KEY; else sync = 50024/(id+1);
   WriteReg(4, sync>>8); WriteReg(5, sync&0xFF);
   ```

2. **改值即时生效**：两个 handler 的 `cmp rX,#6; bne return` 里 bne 条件字节
   `d1` → `d3`（bne → blo）：`item >= 6`（ID=6 / F8=7）改值后立即重设同步字；
   item 0..5 与原厂一致。
3. **屏蔽 6 处稳定色温读者**（`ldrb rX,[base,#7]` → `movs rX,#0`）：F8=OFF 时
   与原厂逐指令一致；F8=ON 时原厂稳定色温功能不生效。
4. **阻断静默清零**（`0x0800B6C8` NOP）：退出路径不再把 F8 清 0。

## 6. 验证结果

- `keypatch.py patch --device ad300 --key 0xD798 --verify`：静态 15 项 + Unicorn 32 项
  **全 PASS**（共 **47 项**；差异恰好 63 字节，全部落在补丁点）。
- `keypatch/selftest.py --key 0xD798`：**ALL PASS**。
- `--container` 产物 44,282 B（payload 44,220 + footer 62），内嵌文件名含 `AD300Pro`。
- 没有整机模拟器（与 V1C 相同），无实机验证。
- 补丁按 `F8 == 1` 严格判断（不是 `!= 0`）；verify 额外覆盖 flag=2 的越界值
  → 走原厂公式，避免任何非 1 值误开密钥。

## 6.1 独立审计（`AUDIT.md`）的补充结论

独立审计员从固件重新推导后 **34 PASS / 1 WARN / 0 FAIL**，确认本笔记的全部地址与
字节；其中几处补充已并入上述结论：

- item7 的访问集合还包括：参数记录 builder 读点 `0x080044C4`（保留）、
  `+/-` handler 的动态下标写（`0x0800466A/0x08004672`、`0x08008412/0x08008418/0x08008420`）、
  以及加载后按 max 表的钳制循环 `0x0800ACE2/0x0800ACEA`（保证 item7 只可能 0/1）。
- max 表 `0x0800DA64` **只有前 8 字节**属于 C.Fn 项 0..7；第 9 字节起是相邻另一张表。
- 初始化函数入口是 `0x0800C230`（唯一调用者 `0x0800487E`），在 `0x0800C25E/0x0800C266`
  写 reg4=0xC3 / reg5=0x68（默认同步字 `0xC368 = 50024`），随后同函数 `0x080048CE`
  才调用同步字函数 → 初始化早于同步字应用。
- `0x0800B6C8` 是 2 字节 `strb`（半字 `0x71D1`），NOP 写作 `0xBF00`（补丁里就是 `00 bf`）。
- 启动时序（静态推断）：flash 参数加载 `0x0800AB6C` 先于 RF 初始化
  `0x0800A6AC → 0x0800B7B0 → 0x08004868`，故开机时 F8 已恢复、首次同步字调用可读到正确 flag。
- `0x0800BB03` 处的 `68 C3` 是跨两条指令编码字节的巧合，**不是** 50024 常量引用；
  32 位 50024 字面量全镜像仍只有 `0x0800B57C` 一处。
- WARN：固件内没有 ASCII 字符串能证明该菜单项叫「F8 / 稳定色温」；这是按
  「6 个读者切换备用校准/功率/显示表」的行为 + 同厂 AD200 Pro 的 C.Fn 布局做的工程命名。

## 7. 风险 / 后续

- 无实机验证；实机首包异常优先怀疑「去掉 SCAL+1ms 延时」。
- F8 原厂「稳定色温模式」被复用（F8=ON 时该功能固定 OFF 行为）。
- 版本绑定：官方升级固件后需重新定位全部补丁点。
- 若要做「不改 F8 的纯静态最小补丁」，可 `apply(full=False)`（只改 sync + gate），
  代价是 F8=ON 时原厂稳定色温逻辑仍会生效（只影响显示/功率换算，不影响同步字）。
