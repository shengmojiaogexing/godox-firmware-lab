# X3-S（Godox X3 S 引闪器）

Godox **X3-S** TTL 引闪器（Sony 热靴版，与 **X3-C** 同属 X3 平台）的固件逆向档案与
「私有密钥」补丁说明。

> ⚠️ 固件二进制不随仓库分发（`.gitignore` 已忽略 `*.bin`）；请从官方渠道获取后放入
> `devices/x3s/firmware/`。仅对本人合法拥有的设备做互操作研究，见根目录 `DISCLAIMER.md`。

## 机型与固件

| 项目 | 值 |
|---|---|
| 设备 | Godox X3-S 无线引闪器（TTL，Sony） |
| 固件文件 | `devices/x3s/firmware/X3_S_v1.25_app.bin` |
| 版本 | v1.25 |
| 文件大小 | 1,012,268 字节 |
| SHA-256 | `3cd40ef3f91369d3147c538cf6745f9308d82e14ad94c9bad9c8f129e0753e4d` |
| MCU | Artery **AT32F435/437** 平台（与 X3-C 同族：向量表 SP=`0x200477C0`、Reset=`0x08004AF9`） |
| App 链接基址 | `0x08004800`（`file_offset = vaddr - 0x08004800`；前 18 KB 是厂商 bootloader，不在 bin 里） |
| 射频 | TI CC2500 兼容 2.4 GHz GFSK；同步字寄存器 `SYNC1/SYNC0 = CC2500 reg 4/5` |

## 补丁能力（默认 keybtn 版：机内开关 + 双发）

与 X3-C 的 keybtn 方案同款：

1. **机内开关（长按拨轮/M/C ≈1.5s）**：切换密钥模式，**不用重新刷固件**；
2. **密钥模式双发**：每个要发的帧发两次 —— 先 `SYNC=私有 KEY`（密钥灯收到），
   再 `SYNC=50024/(当前ID+1)`（同 CH/同 ID 的原厂普通灯也收到）；
3. **普通模式**：完全原厂行为（单发，不写同步字）；
4. 开关状态 `KEY_ON` 是 **RAM 版**：关机/换电池后复位为 0（普通模式），需重新长按开启。

> 可选历史版本 **v1「固定 KEY」**：同步字恒等于私有 KEY（无开关、无双发），
> 只作为对照/保守方案保留（`keypatch/out/X3_S_v1.25_app_key<KEY>_fixed.bin`）。

### 补丁点（全部原位，文件长度不变 1,012,268 B，共 539 字节差异）

| # | 地址 | 内容 | 说明 |
|---|---|---|---|
| 1 | `0x0801184C` | `send_short` 入口 4 B → `B.W 0x080EE000` | 每帧组帧前经过双发包装 |
| 2 | `0x080117E8` | `send_property` 入口 4 B → `B.W 0x080EE0A0` | 同上（属性帧） |
| 3 | `0x080EE000` | 344 B 注入：双发包装 + 蹦床 | 空闲闪存（全 0xFF、无引用） |
| 4 | `0x0800CBD0` | 按键扫描入口 4 B → `B.W 0x080EE400` | 每主循环扫描一次 |
| 5 | `0x080EE400` | 186 B 注入：长按检测 → `KEY_ON ^= 1` | 空闲闪存 |

实现要点：

* 双发包装按「蹦床」方式重建被覆盖的 4 字节序言（`push` + `ldr r4,=0x40020414`）后再
  `B.W` 回原函数 +4 —— 原厂组帧/发送逻辑 **100% 保留**；
* 双发白名单与 X3-C 一致（`send_short` 的 `0x09/0x1A/0x21/0x29` + 全部 `send_property`），
  休眠/心跳等帧保持原厂单发，避免密钥帧让灯「滴滴」响；
* 第二帧同步字用 Cortex-M4 硬件 `UDIV` 现算，发完同步字停在普通值（= 原厂同 ID 状态）；
* 按键注入只检测 PA9（拨轮/M/C）长按；**不修改**原厂其它键的长按阈值（`cmp #0x28` /
  `cmp #0x1E` 保持原样），短按/其它键逻辑完全不动；
* 兼容旧触发条件：ID=99 且 ID-OFF 标志=0 时也按密钥模式双发（与 X3-C v3 相同）。

### 与 X3-C 的地址对应（同一套代码，分段平移）

| 函数 | X3-C | X3-S | 平移 |
|---|---|---|---|
| `set_sync` | `0x08014864` | `0x0801474C` | -0x118 |
| `set_channel` | `0x08014814` | `0x080146FC` | -0x118 |
| CC2500 `WriteReg` | `0x08023540` | `0x08023428` | -0x118 |
| `Strobe` / `delay` | `0x08023520` / `0x080232B6` | `0x08023408` / `0x0802319E` | -0x118 |
| 按键扫描 | `0x0800CC30` | `0x0800CBD0` | -0x60 |
| 读键 `FUN_08022EB4` | `0x08022EB4` | `0x08022D9C` | -0x118 |
| `send_short` | `0x080118B0` | `0x0801184C` | -0x64 |
| `send_property` | `0x0801184C` | `0x080117E8` | -0x64 |
| 低层发送（2B / 4B） | `0x0801712C` / `0x0801708C` | `0x08017014` / `0x08016F74` | -0x118 |
| CC2500 初始化表 | `0x080232BC` | `0x080231A4` | -0x118 |

`set_sync` 函数头 32 字节与 X3-C 对应函数**逐字节相同**；RAM 布局也相同
（ID=`0x20000055`、状态结构=`0x20000030`、界面状态=`0x200003E7` 等两边同址）。
`KEY_ON=0x20001080` 在 X3-S 全镜像无字面量引用；用 X3-C 的 Unicorn 整机模型加载
X3-S 原厂镜像实测：启动 BSS 清零只在 PC=0x08004A88 写一次 0，之后 30 万条指令内无其它写，
**是安全 RAM 字节**。

- 补丁模块：[`keypatch/devices/x3s.py`](../../keypatch/devices/x3s.py)（`apply` 默认 keybtn；
  `apply_fixed` 为 v1；`verify` 自动识别模式）
- 静态证据/复现：[`docs/01_静态证据与复现.md`](docs/01_静态证据与复现.md)
- 过程材料与验证日志：[`keypatch/dev/x3s/NOTES.md`](../../keypatch/dev/x3s/NOTES.md)

## 生成固件

```powershell
# 在仓库根目录执行；密钥必须与灯（AD100 / AD200 / V1C / V860III 等）一致
tools\python\python.exe keypatch\keypatch.py patch --device x3s --key 0xD798 --verify --container

# 或者用设备自己的构建脚本（同时产出 keybtn 默认版 + v1 fixed 历史版 + 容器备用件）
tools\python\python.exe keypatch\dev\x3s\keybtn_build.py
```

产物：

| 文件 | 内容 |
|---|---|
| `keypatch/out/X3_S_v1.25_app_keyD798.bin` | **默认成品**：双发 + 长按拨轮/M/C 切换密钥 |
| `keypatch/out/X3S_v1.25_app_keyD798_container.bin` | G3 容器备用件（内嵌文件名含 `X3S`） |
| `keypatch/out/X3_S_v1.25_app_keyD798_fixed.bin` | v1 固定 KEY（历史/保守方案，无开关） |

## 使用方式

| 场景 | 操作 |
|---|---|
| 开密钥 | **长按拨轮/M/C ≈1.5s** → 进入密钥模式（`KEY_ON=1`，无屏幕提示） |
| 关密钥 | 再长按 ≈1.5s → 回普通模式（`KEY_ON=0`） |
| 验证是否开启 | 密钥灯闪 = 密钥模式；不闪 = 普通模式（也可引闪同 CH 的普通灯对照） |
| 重启后 | 回普通模式（RAM 开关不持久），需要再长按一次 |

**前提**：灯端也要刷同一把 KEY 的补丁并打开密钥开关（AD100「KEY」/ AD200「F8」/
V1C、V860III「LIGHT=ON」）。

## 验证（可复现，全部 PASS）

```powershell
tools\python\python.exe keypatch\dev\x3s\keybtn_build.py      # 构建 + 模块 verify（17 项 PASS）
tools\python\python.exe keypatch\dev\x3s\keybtn_runtime.py    # 整机模拟器端到端（BSS/按键/双发）
tools\python\python.exe keypatch\dev\x3s\boot_regression.py   # 原厂 vs 补丁启动回归（120 万条指令一致）
tools\python\python.exe keypatch\dev\x3s\analyze.py           # 静态地址映射/反汇编证据
```

* **模块 verify 17 项 PASS**：入口跳转、注入区、差异范围、原厂长按阈值未改、原厂自证、
  KEY 关单发 / KEY 开双发 / 白名单外单发 / property 双发 / ID=99 兼容 / 长按切换 / 松开切回 /
  短按不切换；
* **整机模拟器（X3-C Unicorn 模型加载 X3-S）**：启动到 main（108,416 条指令）、
  真实按键扫描 + 注入手势、真实组帧函数 + 双发，全部 PASS；
* **启动回归**：原厂 vs 补丁各跑 120 万条指令，最终寄存器与最近 8192 条 PC 轨迹逐条一致；
* **未做**：整机 UI 渲染验证、实机刷写验证。

## 风险 / 未确认

1. **无实机验证**：刷写前请用官方 G3 工具确认能识别设备，保留原厂 bin 回滚。
2. **键位映射**沿用 X3-C 实机结论（PA9 = 拨轮/M/C；X3-S 按键扫描代码与 X3-C 逐字节相同）；
   若实机键位不符，改 `keypatch/devices/x3s.py` 的 `BTN0_PORT/BTN0_MASK` 重生成即可。
3. **长按阈值** 30 次扫描 ≈1.5s（与 X3-C 相同，主循环节奏未在 X3-S 实机标定）；
   手感不对改 `BTN_KEYBTN5_TICKS1` 一个常量。
4. **KEY_ON 是 RAM 版**：重启回普通模式；后续可像 X3-C v2 那样做「设置区字节持久化」。
5. **双发白名单**沿用 X3-C（0x09/0x1A/0x21/0x29 + property）；若 X3-S 的 Sony 热靴
   触发命令不同，可能需要在白名单里补命令（静态证据见 `keypatch/dev/x3s/NOTES.md`）。
6. 硬件同步字不是密码学认证：SDR 抓包理论上可克隆。

## 复现命令

```powershell
tools\python\python.exe keypatch\keypatch.py list                    # 设备/固件 sha256 状态
tools\python\python.exe keypatch\keypatch.py info --device x3s       # 设备元数据
tools\python\python.exe keypatch\dev\x3s\analyze.py                  # 静态证据 -> analyze.log
tools\python\python.exe keypatch\dev\x3s\unicorn_sync_test.py        # v1 fixed 语义独立交叉验证
tools\python\python.exe keypatch\dev\x3s\keybtn_build.py             # keybtn/fixed 构建 + verify
tools\python\python.exe keypatch\dev\x3s\keybtn_runtime.py           # 整机模拟器端到端
tools\python\python.exe keypatch\dev\x3s\boot_regression.py          # 启动回归
```

## 目录说明

| 路径 | 内容 |
|---|---|
| `firmware/` | 原厂固件（`*.bin` 被 gitignore，仅本地存在） |
| `docs/` | 逆向证据与复现说明 |
| `../../keypatch/devices/x3s.py` | 补丁模块（apply / verify） |
| `../../keypatch/dev/x3s/` | 过程材料：静态证据、构建、整机运行时、启动回归、NOTES |
