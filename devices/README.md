# devices/ —— 设备目录索引

本目录按**设备**组织：每台神牛（Godox）2.4 GHz 闪光灯 / 引闪器一个子目录，存放它的
逆向资料、模拟调试器、以及**原厂固件存放点**（本机，不入库）。

> ⚠️ 先读仓库根的 [`DISCLAIMER.md`](../DISCLAIMER.md)。固件版权归厂商所有，**不随仓库分发**；
> `devices/*/firmware/*.bin` 已被 `.gitignore` 忽略，只保留在本机。

---

## 1. 设备矩阵

| 设备 | 类型 | 固件文件（放在 `firmware/`） | sha256（前 8 位） | 镜像基址 | 本目录 | 补丁引擎状态 |
|---|---|---|---|---|---|---|
| **Godox X3-C** | 引闪器 trigger | `X3_C_v1.38_app.bin` | `45fdd952` | `0x08004800` | [`devices/x3c/`](x3c/) | ✅ 已支持 |
| **Godox X3-S** | 引闪器 trigger | `X3_S_v1.25_app.bin` | `3cd40ef3` | `0x08004800` | [`devices/x3s/`](x3s/) | ✅ 已支持 |
| **Godox X3 Pro S** | 引闪器 trigger | `X3pro_S_v1.11_app.bin` | `4b3465ca` | `0x08020000` | [`devices/x3pros/`](x3pros/) | ✅ 已支持 |
| **Godox X3 Pro（C 版）** | 引闪器 trigger | `X3pro_C_v1.22_app.bin` | `2e2b8a3b` | `0x08020000` | [`devices/x3pro/`](x3pro/) | ✅ 已支持 |
| **Godox AD100 Pro** | 闪光灯 flash | `AD100pro_V1.2.bin` | `3a3d3b91` | `0x08003000` | [`devices/ad100pro/`](ad100pro/) | ✅ 已支持 |
| **Godox AD200 Pro** | 闪光灯 flash | `AD200pro_V1.60.bin` | `fc0f77f1` | `0x08003000` | [`devices/ad200pro/`](ad200pro/) | ✅ 已支持 |
| **Godox AD300 Pro** | 闪光灯 flash | `AD300pro_V1.30.bin` | `e3be6a63` | `0x08003000` | [`devices/ad300pro/`](ad300pro/) | ✅ 已支持 |
| **Godox V1C** | 闪光灯 flash | `V1C_V2.0.bin` | `4435dda8` | `0x08003000` | [`devices/v1c/`](v1c/) | ✅ 已支持 |
| **Godox V1S** | 闪光灯 flash | `V1S_V1.8.bin` | `87a06000` | `0x08003000` | [`devices/v1s/`](v1s/) | ✅ 已支持 |
| **Godox V860III-C** | 闪光灯 flash | `V860IIIC_V1.3.bin` | `8530a7e8` | `0x08003000` | [`devices/v860iiic/`](v860iiic/) | ✅ 已支持 |
| **Godox V860III-S** | 闪光灯 flash | `V860IIIS_V1.1.bin` | `bdc8f354` | `0x08003000` | [`devices/v860iiis/`](v860iiis/) | ✅ 已支持 |
| **Godox X2T-S** | 引闪器 trigger | `X2T-S_V1.4.bin` | `47847644` | `0x08003000` | [`devices/x2ts/`](x2ts/) | ✅ 已支持 |
| **Godox X2T-C** | 引闪器 trigger | `X2TC_Z02_V1.01.bin`（G3 V2.0 容器） | `d185e410` | `0x08005000` | [`devices/x2tc/`](x2tc/) | ✅ 已支持 |
| **Godox V862C** | 闪光灯 flash | 待定 | 待定 | 待定 | 计划中 | 🧩 待接入 |

完整 sha256、Flash/RAM/射频/显示/补丁入口见 **[`docs/设备清单.md`](../docs/设备清单.md)**。
每台设备怎么接入，见 **[`docs/新增设备指南.md`](../docs/新增设备指南.md)**。

---

## 2. 目录约定（每台设备都一样）

```text
devices/<设备>/
├─ README.md            # 该设备的项目索引（逆向结论、怎么跑模拟器/调试器）
├─ firmware/            # ★ 原厂固件放这里（*.bin 被 .gitignore 忽略，仅本机）
│  └─ README.md         #   记录固件来源、版本、sha256（这个会入库）
├─ docs/                #   逆向报告、内存映射、外设、协议、UI 等文档
├─ re/                  #   反汇编 / 伪 C / Ghidra 导出 / 函数表（可选）
├─ analysis/            #   静态分析流水线产物（可选）
├─ emu/ 或 sim/         #   Unicorn 模拟器 + Web 调试界面（可选）
├─ tools/  tests/       #   分析脚本与自动测试（可选）
└─ webui/  src/  ...    #   该设备项目自带的其它资料
```

几条硬规则：

1. **固件只放 `firmware/`**，不要放仓库根或散落在别处；studio 上传的原厂固件也保存到这里。
2. 脚本/文档一律用**仓库相对路径**或 `__file__` / `$PSScriptRoot` 推导，不写死本机绝对路径（如盘符路径）。
3. 所有文本 UTF-8（无 BOM）；日志、`__pycache__`、大件安装包不要入库。
4. 设备目录里的 **`firmware/*.bin` 永远是原厂、未改动的镜像**；补丁产物由
   `keypatch/`（CLI）或 `studio/`（网页工坊）输出到 `keypatch/out/`、`studio/builds/`。

---

## 3. 各设备项目入口

| 设备 | 先看什么 | 具体文档 |
|---|---|---|
| X3-C | [`devices/x3c/`](x3c/)（引闪器：逆向 + Unicorn 模拟机 + 网页调试器） | [`devices/x3c/re/HARDWARE.md`](x3c/re/HARDWARE.md)（MCU/内存/显示/射频）、[`devices/x3c/re/PROTOCOL.md`](x3c/re/PROTOCOL.md)、[`devices/x3c/emu/README.md`](x3c/emu/README.md) |
| X3 Pro S | [`devices/x3pros/README.md`](x3pros/README.md)（引闪器：ID=99 密钥模式 KEY+ID99 双发，同步字 10 B 改写 + TX4/TX2 入口注入） | `devices/x3pros/docs/`（静态证据与补丁点、验证与实机清单） |
| X3 Pro（C 版） | [`devices/x3pro/README.md`](x3pro/README.md)（引闪器：**长按 PA6 ≈1.5s 切密钥模式**（RAM 版），KEY 帧 + 当前 ID 原厂同步字帧双发；按键扫描/发送入口注入 + 同步字 10 B 改写 + 两处 compare 禁用原厂 PA6 长按关机 + 356 B 注入） | `devices/x3pro/docs/`（静态证据与复现）、`devices/x3pro/firmware/`（原厂固件登记） |
| AD100 Pro | [`devices/ad100pro/README.md`](ad100pro/README.md)（STM32F0 + OLED + 模拟器） | `devices/ad100pro/docs/`（启动链、射频协议、OLED、CC2500 模拟规范） |
| AD200 Pro | [`devices/ad200pro/README.md`](ad200pro/README.md)（Cortex-M0 + 模拟调试机） | `devices/ad200pro/docs/`（内存映射、子系统、引脚、模拟器规范） |
| AD300 Pro | [`devices/ad300pro/README.md`](ad300pro/README.md)（同步字函数 + C.Fn `F8` 开关 + 11 处补丁规格 + 47 项离线验证） | `devices/ad300pro/docs/`、`keypatch/dev/ad300/`（反汇编脚本、NOTES、独立审计） |
| V1C | [`devices/v1c/README.md`](v1c/README.md)（同步字函数 + C.Fn LIGHT 开关 + 7 处补丁规格） | `devices/v1c/docs/`（静态证据与复现、验证与实机清单） |
| V1S | [`devices/v1s/README.md`](v1s/README.md)（同步字函数 + C.Fn LIGHT 开关 + 5 处补丁规格，**无需代码洞注入**） | `devices/v1s/docs/`（静态证据与复现、验证与实机清单）、`devices/v1s/re/`（可复现逆向脚本 + 独立分析报告） |
| V860III-C | [`devices/v860iiic/README.md`](v860iiic/README.md)（同步字函数 + C.Fn LIGHT 开关 + 7 处补丁规格） | `devices/v860iiic/docs/`（静态证据与复现、验证与实机清单） |
| V860III-S | [`devices/v860iiis/README.md`](v860iiis/README.md)（同步字函数 + C.Fn LIGHT 开关 + 6 处补丁规格，无需代码洞注入） | `devices/v860iiis/docs/`（静态证据与复现、验证与实机清单）、`devices/v860iiis/re/`（可复现逆向脚本） |
| X2T-S | [`devices/x2ts/docs/README.md`](x2ts/docs/README.md)（引闪器：C.Fn 私有密钥开关、固件登记、离线验证与刷写提示） | `devices/x2ts/re/`（静态复现脚本）、`devices/x2ts/firmware/`（原厂固件，本机） |
| X2T-C | [`devices/x2tc/README.md`](x2tc/README.md)（引闪器：**G3 容器**、C.Fn `LIGHT` 开关、5 处补丁、Unicorn 验证） | `devices/x2tc/docs/01_静态证据与复现.md`、`devices/x2tc/re/`（侦察/解压脚本） |
| V862C | [`devices/_template/`](_template/)（脚手架） | [`devices/_template/README.md`](_template/README.md)、[`docs/新增设备指南.md`](../docs/新增设备指南.md) |

> X3-C 的目录索引见 [`devices/x3c/README.md`](x3c/README.md)；更细的 MCU/显示/射频结论见
> [`devices/x3c/re/HARDWARE.md`](x3c/re/HARDWARE.md) 与 [`devices/x3c/emu/README.md`](x3c/emu/README.md)。

---

## 4. 新增一台设备（V862C 等新设备从这里开始）

只有 5 步，脚手架已经备好：

1. 从官方渠道拿到原厂固件，放进 `firmware/`，记录 sha256；
2. 复制 [`devices/_template/`](_template/) → `devices/<新设备>/`，按需补 `docs/`；
3. 逆向找到「ID → CC2500 同步字」函数（定位方法见 [`docs/新增设备指南.md`](../docs/新增设备指南.md)）；
4. 在 `keypatch/devices/<id>.py` 里实现原位 `apply()` / `verify()`，并把 `<id>` 加入
   `keypatch/devices/__init__.py` 的 `DEVICE_IDS`；
5. 跑 `tools\python\python.exe keypatch\keypatch.py list` 与 `selftest.py` —— 通过后，
   **studio 会自动出现新设备卡片**（`list_devices()` 会带上 `kind` / `model` / `firmware_version`）。

---

## 5. 与补丁引擎 / 工坊的关系

```text
devices/<设备>/firmware/*.bin        原厂固件（本机，gitignore）
        │
        ├─ keypatch/devices/<id>.py   DEVICE + apply()/verify()（原位补丁）
        │        │
        │        └─ keypatch/keypatch.py patch-all [--verify] [--container]
        │                 → keypatch/out/*.bin（G3 直接刷）
        │
        └─ studio/server.py → keypatch/devices.list_devices() → 网页工坊
                 → studio/builds/<id>/*.bin（含容器备用件、manifest、zip）
```

- 补丁引擎与设备注册 API：[`keypatch/INTERFACE.md`](../keypatch/INTERFACE.md)
- 命令行工作流与刷机/回滚：[`keypatch/README.md`](../keypatch/README.md)
- 网页工坊：[`studio/README.md`](../studio/README.md)（API 契约见 [`studio/API.md`](../studio/API.md)）
