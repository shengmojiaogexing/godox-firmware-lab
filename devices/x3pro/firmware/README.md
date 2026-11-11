# devices/x3pro/firmware/

放 **Godox X3 Pro（C 版引闪器）** 的原厂固件（本目录的 `*.bin` 被 `.gitignore` 忽略，只在本机存在）。

| 设备 | 版本 | 文件名 | 大小 | SHA-256 |
|---|---|---|---|---|
| Godox X3 Pro（引闪器，C 版） | V1.22 | `X3pro_C_v1.22_app.bin` | 1,941,020 B | `2e2b8a3b5ff70e62c4acddc406b055ea2625e4833a213e81863f96c0da5a911b` |

- **来源（占位，待补全）**：官方 G3 升级软件下载的 X3 Pro 固件包中解出的 app 镜像（裸 bin，无容器）；
  具体下载包名/日期请在获取后补在此处。本仓库**不分发**厂商固件。
- **整片 app、不含 bootloader**：本文件从 App 基址 `0x08020000` 起，前 128 KB
  （`0x08000000–0x0801FFFF`）是厂商 bootloader，**不在**此 `.bin` 内。镜像范围
  `0x08020000–0x081F9E1C`（文件偏移 = vaddr − `0x08020000`）。
- **尾部含热靴子镜像**：文件末尾 24,092 B（vaddr `0x081F4000` 起，文件偏移 `0x1D4000`）是热靴
  子镜像，与 `tools/hotshoe_blob.bin` 逐字节相同（该 blob sha256
  `f42c0ce0aecfa23729415a39d61d86914afa8e1c865bf35e09af56841485f294`）。
  补丁**不改动**该区。
- **版本绑定**：`keypatch/devices/x3pro.py` 会校验文件长度 1,941,020 B + 上面的 sha256，
  版本/文件不对会直接拒绝打补丁。换固件版本必须重新做静态定位（见 `../docs/01_静态证据与复现.md`）。
- 校验：

  ```powershell
  (Get-FileHash devices\x3pro\firmware\X3pro_C_v1.22_app.bin -Algorithm SHA256).Hash.ToLower()
  ```

- 打补丁后长度不变（原位补丁），可直接用官方 G3 升级软件刷写；换密钥需重新生成。
  产物默认在 `keypatch/out/`（`X3pro_C_v1.22_app_key<KEY>.bin`），已 gitignore。
- **当前补丁语义（key=0xD798）**：物理按键**长按 PA6（GPIOA mask `0x40`）≈1.5 秒**切换密钥模式
  （`KEY_ON` 为 RAM 版，重启回普通模式；原厂 PA6 长按关机被复用，**两处** compare
  `0x0802830A`/`0x080284F2` 均禁用，电源键长按关机不受影响）；
  `KEY_ON=1` 时每帧 = KEY 帧 + 当前 ID 原厂同步字帧双发，`KEY_ON=0` 时原厂单发。
  成品 `keypatch/out/X3pro_C_v1.22_app_keyD798.bin`，1,941,020 B，sha256
  `11f58c71e83192fdf6dabd15d541602f391c9db6991bcbf49daf8db4156e4ae6`（换密钥/版本会变）。
