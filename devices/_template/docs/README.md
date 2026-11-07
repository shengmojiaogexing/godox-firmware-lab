# devices/_template/docs/ —— 设备逆向文档目录

复制到 `devices/<id>/docs/` 后，把该设备的逆向结论按下面建议整理。不强制文件名，
但建议保持和已支持设备（`devices/ad100pro/docs/`、`devices/ad200pro/docs/`）相近的结构，
方便横向比对与后续接手。

## 建议内容

| 文件（建议） | 写什么 |
|---|---|
| `00_overview.md` | 机型、固件版本、sha256、镜像基址、Flash/RAM/时钟、启动链 |
| `01_memory_map.md` | 向量表、RAM 变量、出厂参数区、外设基址 |
| `02_rf_cc2500.md` | 射频芯片、软 SPI 引脚、信道/频率、同步字公式与函数地址 |
| `03_display.md` | 显示控制器/分辨率/接口/字库，刷新函数与帧缓冲 |
| `04_sync_fn.md` | **「ID → 同步字」函数的定位过程**：常量 50024 交叉引用、调用点、原始字节 |
| `05_patch_plan.md` | 打算怎么原位改写（字节布局、长度预算、风险、回滚） |
| `images/`（可选） | 模拟器截图、示波器/波形、UI 截图等证据图 |

## 硬要求

- 每条结论尽量给出**证据地址**（函数地址 / 文件偏移 / 反汇编片段），不要只写"应该是"；
- 无法唯一确定的地方明确标 **待确认 / 推测**，并写清验证方法；
- 不写本机绝对路径（用仓库相对路径，如 `devices/<id>/firmware/<固件>.bin`）；
- 文档 UTF-8（无 BOM）；
- 不把原厂固件、反汇编大文件、厂商 PDF/升级包放进 `docs/`（大件和 `*.bin` 都被 gitignore）。

## 关联位置

- 补丁模块：`keypatch/devices/<id>.py`（接口见 `keypatch/INTERFACE.md`）
- 补丁过程材料：`keypatch/dev/<id>/NOTES.md`
- 设备总览：`docs/设备清单.md`
- 接入步骤：`docs/新增设备指南.md`
