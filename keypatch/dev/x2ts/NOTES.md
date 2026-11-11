# Godox X2T-S V1.4 补丁点与验证记录（Lead）

- 角色：Lead（Team 会话最终实现与交叉验证）
- 日期：2026-11-11
- 对象：`devices/x2ts/firmware/X2T-S_V1.4.bin`
  （58,628 B，sha256 `478476447ff8d55496180facf578662dcbf9a1af0cf21f79df048b7df0a76efc`，只读）
- 权威实现：`keypatch/devices/x2ts.py`（`DEVICE` / `apply()` / `verify()`）
- 设备档案：`devices/x2ts/docs/README.md`；静态侦察脚本：`devices/x2ts/re/`

## 0. 结论摘要

| 项 | 结论 |
|---|---|
| 真实链接基址 | **0x08003000**（Reset_Handler 0x080031AC；SystemInit 0x0800CF60；尾部初始化表 0x080114A8；镜像尾 0x08011504） |
| 同步字函数 | **0x0800C8F8..0x0800C92B（52 B）**，下一函数 0x0800C92C；原厂 `movw r1,#0xC368; adds r0,r0,#1; udiv r4,r1,r0` |
| 密钥开关 | **C.Fn 第 3 项 BEEP**：`C.Fn[2] = RAM 0x200203E2`（C.Fn 数组基址 0x200203E0，最大值表 0x20020072[2]=1）；`1=ON -> 密钥模式`，`0=OFF -> 原厂 50024/(ID+1)` |
| 补丁形态 | 52 B 整段改写 + `b.w 0x0800C8DC` 保留原厂 delay(20)+SCAL(0x33)+delay(1000) 尾部 + 4 个 C.Fn 加减 hook 注入未用中断向量槽 |
| 输出 | `X2T-S_V1.4_key<KEY>.bin`，等长 58,628 B；差异 121 字节；`verify` 30 项 PASS、`keypatch.py list` sha256 OK、`selftest.py` ALL PASS |
| 实机 | 未验证（无硬件） |

## 1. 启动链与 RAM 布局（Unicorn 实证）

`0x080031D0` 解释初始化表 `0x080114A8`（2 条 16 B record）：

| record | 源 | 目标 | 长度 | 执行函数 | 结论 |
|---|---|---|---|---|---|
| rec0 | 0x080114C8 | 0x20020000 | 0x188 | 0x080031F4（RLE 解压） | `.data`（含 C.Fn 最大值表初值 0x20020072 等） |
| rec1 | 0x08011504（忽略，仅占位） | 0x20020188 | 0x1EB0 | 0x0800DC20（memset 0） | `.bss` 清零，SP=0x20022038 |

关键 RAM：

- C.Fn 值数组 `0x200203E0`（项 n = base+n）；项 5=CH、6=ID、0xA=DIST 由菜单 apply 函数
  `0x0800C790` 特殊处理；BEEP 等其它项由加减处理函数直接写数组。
- C.Fn 最大值表 `0x20020072`（.data 解压初值，17 项）：
  `01 01 01 09 01 1F 63 01 63 02 01 05 01 03 02 06 01`
  → 与 X2T 手册 C.Fn 顺序 SHUTTER / BLUE.T. / **BEEP** / ZOOM / SCAN / CH / ID / PC SYNC /
  DELAY / SHOOT / DIST 对应（BEEP 索引 2，max=1）。
- ID 副本：`0x200203E6`（生效 ID，同步字入参）/ `0x2002003E`（菜单编辑缓存）/
  `0x2002002E`（设置字节）/ `0x20020123`（影子缓存）。

## 2. 补丁点

### 2.1 同步字函数（52 B 整段改写）

`0x0800C8F8..0x0800C92B`，替换为 `build_sync_function(key)`：

```
push {r4,lr}; ldr r3,=0x200203E2; ldrb r3,[r3]; cmp r3,#1; beq .key
adds r0,r0,#1; movw r1,#0xC368; udiv r4,r1,r0; b .write
.key: movw r4,#KEY
.write: lsrs r1,r4,#8; movs r0,#4; bl WriteReg(0x0800E842)
        uxtb r1,r4; movs r0,#5; bl WriteReg
        b.w 0x0800C8DC      ; 原厂频道设置函数尾部：delay(20) + SCAL(0x33) + pop{r4,lr} + delay(1000)
nop; .word 0x200203E2
```

- 保留原厂 SCAL/延时 → 射频时序与原厂一致（不同于 V1C 的“无 SCAL”变体）。
- 辅助：WriteReg `0x0800E842`、strobe `0x0800E932`、delay `0x0800E5AA`。

### 2.2 即时生效 hook（4 个，注入未用中断向量槽）

未使用的中断向量槽原值均为默认陷阱 `0x080031C7`（对应 IRQ 从未使能），属安全空闲槽：

| hook | 地址/长度 | 调用点（4 B -> BL） | 被搬走的原指令 |
|---|---|---|---|
| HOOK_INC | 0x0800306C / 16 B | 0x08009232（递增普通通路） | `strb r4,[r1,r2]; ldrb r5,[r3,r2]` |
| HOOK_DEC | 0x0800307C / 14 B | 0x08005C8E（递减普通通路） | `subs r3,r3,#1; strb r3,[r0,r1]` |
| HOOK_WRAP_I | 0x0800308C / 16 B | 0x0800923A（递增回绕通路） | `cmp r2,#0xF; beq 0x0800928E` |
| HOOK_WRAP_D | 0x080030A0 / 14 B | 0x08005CD4（递减回绕通路） | `ldrb r2,[r2,r1]; strb r2,[r0,r1]` |

hook 统一逻辑：执行被搬走指令（必要时先写回新值/0/最大值）→
`ldrb r0,[C.Fn_base,#6]; bl sync`（补丁版同步字函数内部再读 BEEP 开关）→ 恢复现场返回。
因此菜单里切 BEEP **立即**切换密钥模式；加/减与两端回绕 4 条通路全部覆盖。

> 备注：注入点是向量表槽（数据/异常入口），**不是**代码洞；不新增代码洞、不改 .data/.bss。
> 隐藏项 15 的“回绕到最大值”特殊分支被 WRAP_I hook 覆盖为常规回绕（该项属固件内部预留项，
> 手册 11 项之外，正常使用不可见）。

## 3. 验证结果（2026-11-11）

```
tools\python\python.exe keypatch\keypatch.py patch --device x2ts --key 0xD798 --verify --container
→ 30 项 PASS（静态 21 项 + Unicorn 9 组：同步字 ID=0/5/37/99 × flag=0/1、原厂自证、4 个 hook 行为）
→ X2T-S_V1.4_keyD798.bin（58,628 B，sha256 10756bfa…）
→ X2TS_V1.4_keyD798_container.bin（58,685 B，payload+footer 57 B）

tools\python\python.exe keypatch\selftest.py --key 0xD798
→ ALL PASS（13 台设备；x2ts 段：差异 121 字节、30 项 PASS）

node studio\tests\test_ui.mjs --url http://127.0.0.1:18781/
→ studio 可视化配置端到端（设备卡片/生成/下载）
```

复现脚本：

- `devices/x2ts/re/recon16_decompress.py`：启动解压 .data（得到最大值表初值）；
- `devices/x2ts/re/sync_xref.py` / `sync_ram.py` / `sync_holes.py`：调用点/ID 变量/空洞审计（re-sync 交付）；
- `keypatch/dev/x2ts/sync_patch_preview.py`：52 B 补丁原型与 3 种开关语义的 Unicorn 验证；
- `keypatch/dev/x2ts/test_module.py`：模块 `apply()+verify()` 全量自测。
