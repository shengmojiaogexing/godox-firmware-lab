# devices/x3pros/firmware/ —— Godox X3 Pro S 原厂固件（本机，不入库）

本目录只存放**从官方渠道获得、未改动的原厂固件**；`*.bin` 已被 `.gitignore` 忽略，
不会进入 git。补丁产物请输出到 `keypatch/out/` 或 `studio/builds/`，不要放这里。

## 固件登记

| 项目 | 值 |
|---|---|
| 设备 | 神牛（Godox）**X3 Pro S** 无线引闪器（Sony 热靴版，2.4 GHz CC2500 兼容） |
| 固件版本 | **v1.11** |
| 文件名 | `X3pro_S_v1.11_app.bin` |
| 字节数 | **1,931,868 B（0x1D7A5C）** |
| sha256 | `4b3465cab7975749dd3d109aa1b6f60ee065402869aa88d7b007325def90e103` |
| 来源 | 用户提供的官方升级包固件（只读副本；请以官方 G3 升级软件可识别的官方包为准） |
| 获取日期 | 2026-11-11 |
| 镜像基址 | `0x08020000`（`file_offset = vaddr - 0x08020000`） |
| 镜像范围 | `0x08020000–0x081F7A5C`（1,931,868 B） |
| 备注 | 纯 app 镜像；**file 0x1D4000–0x1D7A5C（14,940 B）是独立链接的 Sony 热靴 MCU 子镜像**（补丁不动它）。`0x08000000–0x0801FFFF` 不在此文件内（未在本文档分析）。Flash/RAM 容量未实测。 |

## 校验

```powershell
# Windows PowerShell / 仓库根
(Get-FileHash devices\x3pros\firmware\X3pro_S_v1.11_app.bin -Algorithm SHA256).Hash.ToLower()
tools\python\python.exe keypatch\keypatch.py list
```

应输出 `sha256 OK`。若为 `MISMATCH` 或文件缺失，说明版本不对/放错目录，**不要**刷入，
也不要打补丁（`apply()` 会按长度 + sha256 + 补丁点原始字节拒绝）。

## 不要入库

- 固件版权归权利人所有；`.gitignore` 已忽略 `*.bin`/`*.hex`/`*.zip`，本目录的固件**不会**进入 git；
- 仓库只记录**文件名 + 字节数 + sha256**（本文件），别人拿到正版固件后可自行校验；
- 不要用 `git add -f` 强推固件；studio「上传原厂固件」也保存到 `devices/<设备>/firmware/`，
  并会按 `keypatch/devices/x3pros.py` 里的 sha256 校验。

## 注意

- 只放原厂、未改动的固件；保留一份用于**回滚**（官方 G3 升级软件可刷回）；
- **热靴子镜像边界**：file `0x1D4000` 起 14,940 B 是 Sony 热靴 MCU 固件 blob（独立链接在
  `0x08000000`，有自己的向量表/Reset/SystemInit）。补丁的注入区在它之前（`0x081CA000` 起
  0x400 B），**任何新补丁都不要写进 `0x1D4000` 之后**；
- 换固件版本时必须同步更新 `keypatch/devices/x3pros.py` 的 `sha256` / `image` / 地址常量与
  补丁点原始字节，并重跑 `keypatch.py patch --device x3pros --key <KEY> --verify`；
- 补丁产物的 sha256 随密钥变化，以实际生成为准；本文件只登记**原厂**固件。
