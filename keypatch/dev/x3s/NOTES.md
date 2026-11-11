# keypatch/dev/x3s/ —— Godox X3-S V1.25 私有密钥补丁过程材料

> 交付物：`keypatch/devices/x3s.py`（设备补丁模块）+ `devices/x3s/`（设备资料）。
> 静态证据正文见 [`../../../devices/x3s/docs/01_静态证据与复现.md`](../../../devices/x3s/docs/01_静态证据与复现.md)。

## 1. 结论速览

| 项目 | 值 |
|---|---|
| 原厂固件 | `devices/x3s/firmware/X3_S_v1.25_app.bin`（1,012,268 B，sha256 `3cd40ef3…`） |
| 镜像基址 | `0x08004800`（AT32F435 平台，向量表 SP/Reset 与 X3-C 相同） |
| **默认模式 keybtn** | **双发 + 机内开关**（长按拨轮/M/C ≈1.5s 切密钥） |
| 补丁点 | `0x0801184C`/`0x080117E8`（send_short/send_property 入口 B.W）+ `0x080EE000`（344 B 注入）+ `0x0800CBD0`（按键扫描入口 B.W）+ `0x080EE400`（186 B 注入） |
| 差异 | **539 字节**（入口 12 + 注入 530，含未变字节），长度不变 1,012,268 B |
| 成品 | `keypatch/out/X3_S_v1.25_app_keyD798.bin`（sha256 `c2627294853221b5…`） |
| 容器备用件 | `keypatch/out/X3S_v1.25_app_keyD798_container.bin`（1,012,329 B） |
| v1 fixed 历史版 | `keypatch/out/X3_S_v1.25_app_keyD798_fixed.bin`（5 字节差异，sha256 `ab9a18ab…`） |
| 状态 | 模块 verify 17 项 PASS + 整机模拟器端到端 ALL PASS + 启动回归 ALL PASS；**无实机验证** |

## 2. 与 X3-C 的地址对应（同一套代码，分段平移）

| 函数 | X3-C | X3-S | 平移 |
|---|---|---|---|
| `set_sync` | `0x08014864` | `0x0801474C` | -0x118 |
| `set_channel` | `0x08014814` | `0x080146FC` | -0x118 |
| `WriteReg` / `Strobe` / `delay` | `0x08023540`/`0x08023520`/`0x080232B6` | `0x08023428`/`0x08023408`/`0x0802319E` | -0x118 |
| 按键扫描 | `0x0800CC30` | `0x0800CBD0` | -0x60 |
| 读键 | `0x08022EB4` | `0x08022D9C` | -0x118 |
| `send_short` / `send_property` | `0x080118B0` / `0x0801184C` | `0x0801184C` / `0x080117E8` | -0x64 |
| 低层发送 2B / 4B | `0x0801712C` / `0x0801708C` | `0x08017014` / `0x08016F74` | -0x118 |
| CC2500 初始化表 | `0x080232BC` | `0x080231A4` | -0x118 |

`set_sync` 函数头 32 字节与 X3-C 对应函数**逐字节相同**；RAM 布局相同
（ID=`0x20000055`、状态结构=`0x20000030`、UI 状态=`0x200003E7`、组帧门控=`0x20000146`
两边同址同值）。

## 3. 默认模式（keybtn）：双发 + 机内开关

### 3.1 核心双发（`0x080EE000`，344 B）

```
inj_short(cmd):                        # send_short 入口包装
    if KEY_ON != 0 or (ID == 99 and ID_OFF == 0):
        if cmd in {0x09,0x1A,0x21,0x29}: double(cmd)
        else: single(cmd)              # 休眠/心跳等保持原厂单发
    else: single(cmd)
double(cmd):
    WriteReg(4,KEY>>8); WriteReg(5,KEY&0xFF); tramp_short(cmd)
    WriteReg(4,sync>>8); WriteReg(5,sync&0xFF); tramp_short(cmd)   # sync=50024/(ID+1) 现算
tramp_short: 原序言(push {r4,lr}; ldr r4,=0x40020414) -> B.W 0x08011850
```

`send_property` 同构、所有属性帧双发。第二帧同步字用 M4 硬件 `UDIV` 现算，不调原厂
`set_sync`（避免 SCAL + 1ms 延时）。

### 3.2 机内开关（`0x080EE400`，186 B）

* 按键扫描入口 `0x0800CBD0` 的 `2d e9 f0 5f` 改成 `B.W 0x080EE400`；
* 注入：执行原序言 → `btn_update`（每扫描一次）→ `B.W 0x0800CBD4` 回原函数体；
* `btn_update`：GPIOA MODER 校验（PA9 输入模式）→ 读键 `FUN_08022D9C(GPIOA,0x200)`
  → 去抖 3 次 → u16 计数（`0x20001096`）→ 到 30 次 `KEY_ON ^= 1` → >600 看门狗清零
  → 松开清零；
* **不改**原厂其它键长按阈值（`0x0800CCB4` cmp #0x28 / `0x0800CD16` cmp #0x1E 保持原样），
  PA9 本身原厂无长按动作。

### 3.3 为什么 KEY_ON=0x20001080 安全

* X3-S 全镜像无字面量引用 `0x20001080`（手势计数 `0x20001096..99` 同样无引用）；
* 用 X3-C 的 Unicorn 整机模型加载 **X3-S 原厂镜像**，对 `0x20001080..0x2000109F` 做
  UC_HOOK_MEM_WRITE 写追踪：启动阶段只有 BSS 清零（PC=`0x08004A88`，32 次 u8 写），
  30 万条指令内无任何非 0 写；
* RAM 布局与 X3-C 相同（同址同值 RAM 字面量），X3-C 侧已由 40M 指令模拟器证实。

## 4. v1「固定 KEY」模式（历史，`apply_fixed`）

| 地址 | 原厂 | 补丁后 |
|---|---|---|
| `0x0801474E` | `4c f2 68 31`（movw r1,#0xC368=50024） | `movw r1,#KEY` |
| `0x08014752` | `40 1c`（adds r0,r0,#1） | `01 20`（movs r0,#1） |

差异 5 字节；独立 Unicorn 验证见 `unicorn_sync_test.py`（ALL PASS）。

## 5. 验证记录

### 5.1 模块 verify（`keypatch.py patch --device x3s --verify`，17 项 PASS）

```text
[PASS] 输出长度 1012268 与原厂 1012268 一致
[PASS] send_short 入口 B.W -> 0x080EE000
[PASS] send_property 入口 B.W -> 0x080EE0A0
[PASS] 按键扫描入口 0x0800CBD0 B.W -> 0x080EE400（长按切 KEY）
[PASS] 核心注入区 0x080EE000（344 B）与生成一致
[PASS] 按键注入区 0x080EE400（186 B）与生成一致
[PASS] 差异字节 539 个 = 预期 539 个（仅入口跳转 + 注入区）
[PASS] 原厂其它键长按阈值保持原样（cmp #0x28 / #0x1E）
[PASS] 原厂 send_short 自证：单发一次
[PASS] KEY 关：send_short 原厂单发
[PASS] KEY 开：send_short(cmd=0x09) 双发（SYNC=0xD798 -> 发 -> SYNC=0x2091 -> 发）
[PASS] KEY 开：白名单外 cmd=0x11 保持原厂单发
[PASS] KEY 开：send_property 双发
[PASS] 兼容 ID=99：KEY 关时也双发（第二帧 0x01F4）
[PASS] 长按 32 次扫描（去抖 3 + 阈值 30）：第 31 次前=0，第 32 次切到 1，继续按住保持 1
[PASS] 松开清零后再次长按 32 次 -> KEY_ON 切回 0
[PASS] 短按（<5 次扫描后松开）不改变 KEY_ON
```

### 5.2 整机模拟器端到端（`keybtn_runtime.py`，ALL PASS）

X3-C 的 Unicorn 整机模型直接加载 X3-S 镜像（同平台/同 RAM 布局）：

```text
[PASS] 0x20001080..0x2000109F 启动只有 BSS 清零写（PC=0x08004A88）
[PASS] 补丁镜像启动到 main（PC=0x08033D08，108,416 条指令）
[PASS] 长按 32 次扫描：第 31 次前 KEY_ON=0，第 32 次切到 1，继续按住保持 1
[PASS] 松开清零后再次长按 32 次 -> KEY_ON 切回 0
[PASS] 短按（<5 次扫描后松开）不改变 KEY_ON
[PASS] KEY 关：send_short(cmd=0x09) 原厂单发
[PASS] KEY 开：send_short(cmd=0x09) 双发（KEY 帧 + ID=5 普通帧 0x2091）
[PASS] KEY 开：白名单外 cmd=0x11 保持原厂单发
[PASS] KEY 开：send_property(0x50,0xD1,0x40) 双发
```

### 5.3 启动回归（`boot_regression.py`，ALL PASS）

原厂 vs 补丁各跑 1,200,000 条指令：停止原因、指令数、最终 PC/SP/LR、最近 8192 条 PC
轨迹逐条一致。

### 5.4 静态证据

`analyze.py` / `analyze.log`：镜像信息、同步字函数反汇编（原厂/补丁）、SYNC 写入点扫描、
初始化表与启动链、MOVW 编码、地址映射、注入代码反汇编、入口 B.W 解码。

## 6. 风险 / 未确认

1. **无实机验证**。刷写前用官方 G3 工具确认能识别设备，保留原厂 bin 回滚。
2. **键位映射沿用 X3-C 实机结论**（PA9 = 拨轮/M/C）；X3-S 按键扫描代码与 X3-C 逐字节
   相同，若实机手感/键位不符，改 `BTN0_PORT/BTN0_MASK` 重生成即可。
3. **长按阈值** 30 次扫描 ≈1.5s（与 X3-C 相同，主循环扫描周期未在 X3-S 实机标定）；
   手感不对改 `BTN_KEYBTN5_TICKS1` 一个常量。
4. **KEY_ON 是 RAM 版**：关机/换电池后复位为 0（普通模式），需要重新长按开启。
   若要掉电保存，可参照 X3-C 的「设置区空字节」方案另做（需先定位 X3-S 的设置区 RAM 副本）。
5. **双发白名单**沿用 X3-C（`0x09/0x1A/0x21/0x29` + property）。静态扫描 X3-S 的
   `send_short` 调用点见 `keypatch/dev/x3s/keybtn_build.log` 与 `analyze.log`；
   若 X3-S（Sony 热靴）的触发命令不在白名单里，表现会是「密钥模式开了但引闪不了」，
   此时把该命令补进 `keypatch/devices/x3s.py::build_injection` 的白名单再重生成。
6. **UI 磁贴版未做**：X3-C 的快捷面板 KEY/BCAST 磁贴补丁依赖 X3-C 专属 UI 页面数据
   （X3-S 的页面/坐标/字符串不同，且 X3-C UI 版本身曾是高风险方案），故 X3-S 采用
   X3-C 同样提供、且写入其刷机指南的「物理按键长按」方案作为机内开关。
7. 硬件同步字不是密码学认证：SDR 抓包理论上可克隆。

## 7. 交付清单

| 文件 | 内容 |
|---|---|
| `keypatch/devices/x3s.py` | 设备模块：`apply`（默认 keybtn）/ `apply_fixed` / `verify`（自动识别） |
| `keypatch/out/X3_S_v1.25_app_keyD798.bin` | 默认成品（双发 + 长按开关） |
| `keypatch/out/X3S_v1.25_app_keyD798_container.bin` | G3 容器备用件 |
| `keypatch/out/X3_S_v1.25_app_keyD798_fixed.bin` | v1 固定 KEY 历史版 |
| `devices/x3s/README.md`、`firmware/README.md`、`docs/01_静态证据与复现.md` | 设备资料 |
| `keypatch/dev/x3s/analyze.py` + `.log` | 静态证据 |
| `keypatch/dev/x3s/unicorn_sync_test.py` + `.log` | v1 独立 Unicorn 交叉验证 |
| `keypatch/dev/x3s/keybtn_build.py` + `.log` | keybtn/fixed 构建 + 模块 verify |
| `keypatch/dev/x3s/keybtn_runtime.py` + `.log` | 整机模拟器端到端 |
| `keypatch/dev/x3s/boot_regression.py` + `.log` | 原厂 vs 补丁启动回归 |
| `keypatch/dev/x3s/NOTES.md` | 本文件 |
