# devices/v860iiis/firmware/ —— Godox V860III-S 原厂固件（本机，不入库）

本目录只存放**从官方渠道获得、未改动的原厂固件**；`*.bin` 已被 `.gitignore` 忽略，
不会进入 git。补丁产物请输出到 `keypatch/out/` 或 `studio/builds/`。

## 固件登记

| 项目 | 值 |
|---|---|
| 设备 | Godox V860III-S（索尼口 TTL 机顶闪光灯，2.4 GHz） |
| 固件版本 | V1.1 |
| 文件名 | `V860IIIS_V1.1.bin` |
| 字节数 | 79,300 B（0x135C4） |
| sha256 | `bdc8f3547808d121f15a6c58b9363c5792c957c2dce2a16b203276ab078d0b20` |
| 来源 | 用户提供的官方升级包固件（只读副本；请以官方 G3 升级软件可识别的官方包为准） |
| 获取日期 | 2026-11-11 |
| 备注 | 纯 app 镜像，不含 bootloader（基址 `0x08003000`，前 12 KB 为 bootloader）；整片 flash 容量未实测 |

## 校验

```powershell
(Get-FileHash devices\v860iiis\firmware\V860IIIS_V1.1.bin -Algorithm SHA256).Hash.ToLower()
tools\python\python.exe keypatch\keypatch.py list
```

应输出 `sha256 OK`。若为 `MISMATCH`，说明版本不对，**不要**刷入。

## 注意

- 只放原厂、未改动的固件；保留一份用于回滚（官方 G3 升级软件可刷回）；
- studio「上传原厂固件」也保存到本目录，并按 `keypatch/devices/v860iiis.py` 里的 sha256 校验；
- 换固件版本时必须同步更新 `keypatch/devices/v860iiis.py` 的 `sha256` / `image` 与补丁点。
