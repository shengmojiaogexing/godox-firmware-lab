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
| **Godox AD100 Pro** | 闪光灯 flash | `AD100pro_V1.2.bin` | `3a3d3b91` | `0x08003000` | [`devices/ad100pro/`](ad100pro/) | ✅ 已支持 |
| **Godox AD200 Pro** | 闪光灯 flash | `AD200pro_V1.60.bin` | `fc0f77f1` | `0x08003000` | [`devices/ad200pro/`](ad200pro/) | ✅ 已支持 |
| **Godox V1C** | 闪光灯 flash | 待定 | 待定 | 待定 | 计划中 | 🧩 待接入 |
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
| AD100 Pro | [`devices/ad100pro/README.md`](ad100pro/README.md)（STM32F0 + OLED + 模拟器） | `devices/ad100pro/docs/`（启动链、射频协议、OLED、CC2500 模拟规范） |
| AD200 Pro | [`devices/ad200pro/README.md`](ad200pro/README.md)（Cortex-M0 + 模拟调试机） | `devices/ad200pro/docs/`（内存映射、子系统、引脚、模拟器规范） |
| V1C / V862C | [`devices/_template/`](_template/)（脚手架） | [`devices/_template/README.md`](_template/README.md)、[`docs/新增设备指南.md`](../docs/新增设备指南.md) |

> X3-C 的目录索引见 [`devices/x3c/README.md`](x3c/README.md)；更细的 MCU/显示/射频结论见
> [`devices/x3c/re/HARDWARE.md`](x3c/re/HARDWARE.md) 与 [`devices/x3c/emu/README.md`](x3c/emu/README.md)。

---

## 4. 新增一台设备（V1C / V862C 从这里开始）

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
