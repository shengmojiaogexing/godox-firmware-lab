# devices/v1s/firmware/ —— Godox V1S 原厂固件（本机，不入库）

把**从官方渠道获得的、未改动的原厂固件**放在本目录。`*.bin` 已被 `.gitignore` 忽略，
不会进入 git；仓库只登记文件名 + sha256。

## 登记信息

| 项目 | 值 |
|---|---|
| 设备 | Godox V1S（V1 的索尼 / Sony 版闪光灯） |
| 固件版本 | V1.8 |
| 文件名 | `V1S_V1.8.bin` |
| 字节数 | 78,500 |
| sha256 | `87a0600046e1b7c2198a7ac8fac15959bb3a5d582edf44d8e0df38706dcb917a` |
| 镜像基址 | `0x08003000`（app 镜像占 `0x08003000–0x080162A3`；前 12 KB 是 bootloader，不在本文件内） |
| 来源 | 官方渠道 / 官方升级包（本机留档，未分发） |
| 获取日期 | 2026-11-11 |
| 备注 | Cortex-M0 / Thumb-1；初始 SP=`0x20001540`、Reset=`0x080030D5`；参数区 `0x0801F000`（20 槽 × 0x64 B，备份页 `0x0801F800`） |

## 校验

```powershell
# 仓库根
(Get-FileHash devices\v1s\firmware\V1S_V1.8.bin -Algorithm SHA256).Hash.ToLower()
tools\python\python.exe keypatch\keypatch.py list        # v1s 应显示 sha256 OK
```

## 注意

- **只放原厂、未改动的固件**；补丁产物在 `keypatch/out/` 或 `studio/builds/`；
- 保留原厂固件用于**回滚**（官方 G3 升级软件可刷回）；
- 官方升级固件版本一旦变化，必须重新定位补丁点（见 `../re/`、`../../../keypatch/dev/v1s/NOTES.md`）。
