# devices/_template/ —— 新设备脚手架（V1C / V862C 从这里开始）

本目录是**复制用模板**，不会被任何脚本自动加载。新增一台设备时，把这里的结构复制成
两台：一台是"设备资料目录"，另一台是"补丁模块"。

---

## 1. 复制成设备资料目录

在仓库根运行（把 `<id>` / `<设备名>` 换成真实值，例如 `v1c` / `V1C`）：

```powershell
# 1) 设备资料目录：devices/<id>/
New-Item -ItemType Directory -Force devices\<id>\firmware, devices\<id>\docs
Copy-Item devices\_template\README.md       devices\<id>\README.md
Copy-Item devices\_template\firmware\README.md devices\<id>\firmware\README.md
Copy-Item devices\_template\docs\README.md  devices\<id>\docs\README.md
```

然后把原厂固件放进 `devices/<id>/firmware/`（文件被 `.gitignore` 忽略，仅本机保留），
并把版本、来源、sha256 填进 `devices/<id>/firmware/README.md`。

## 2. 复制成补丁模块

```powershell
Copy-Item devices\_template\device.py keypatch\devices\<id>.py
```

然后逐项替换 `keypatch/devices/<id>.py` 里的 `<id>` / `<设备名>` / `<型号>` / `<版本>` /
固件文件名 / `sha256` / `base` / 补丁点地址，并实现真正可用的 `apply()` 与 `verify()`。
**不要**只改 DEVICE 就完事——`apply()` 里必须是经过验证的原位字节补丁。

## 3. 注册设备

1. 把 `<id>` 追加到 `keypatch/devices/__init__.py` 的 `DEVICE_IDS`；
2. `DEVICE` 里建议保留 `kind`（`"trigger"` / `"flash"`）、`model`、`firmware_version`，
   studio 会直接用它们渲染设备卡片；
3. `image` 用**仓库相对路径**，指向 `devices/<id>/firmware/<原厂固件>.bin`；
4. `out_name` 建议沿用原厂文件名 + `_key{key:04X}`，例如
   `V1C_V1.0_key{key:04X}.bin`（容器备用件会在此基础上再加 `_container`）。

## 4. 验证

```powershell
# 立刻能列到设备、看到固件 sha256 是否匹配
tools\python\python.exe keypatch\keypatch.py list
tools\python\python.exe keypatch\keypatch.py info --device <id>

# 打一个测试密钥 + 离线验证（先输出到临时目录，别覆盖正式成品）
tools\python\python.exe keypatch\keypatch.py patch --device <id> --key 0xD798 --verify --out %TEMP%\kp_<id>
# 需要 G3 容器备用件时
tools\python\python.exe keypatch\keypatch.py patch --device <id> --key 0xD798 --container --out %TEMP%\kp_<id>

# 全设备总自检（会跑三台已支持设备；新设备 join 后也一起跑）
tools\python\python.exe keypatch\selftest.py --key 0xD798
```

studio 端不需要改代码：`studio/server.py` 调 `keypatch/devices.list_devices()`，
新设备会自动出现在 `/api/devices` 与网页卡片里。若固件缺失或 sha256 不匹配，
卡片会提示「上传原厂固件」，上传成功后自动可用（见 `studio/API.md`）。

## 5. 还要写什么

- `devices/<id>/README.md`：设备项目索引（机器型号、固件版本、怎么跑模拟器/调试器）；
- `devices/<id>/docs/`：逆向报告（启动链、内存映射、射频/显示、同步字函数推导）；
- `keypatch/dev/<id>/NOTES.md`：补丁点、推导过程、Unicorn 验证证据（过程材料）；
- `docs/设备清单.md`：补一行（设备/类型/固件/sha256/基址/Flash/RAM/射频/显示/补丁入口/状态）。

完整步骤与常见坑见 [`docs/新增设备指南.md`](../../docs/新增设备指南.md)。
