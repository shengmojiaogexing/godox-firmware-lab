# AD300 Pro 原厂固件（本机文件，不入库）

| 项目 | 值 |
|---|---|
| 文件名 | `AD300pro_V1.30.bin` |
| 大小 | 44,220 字节 |
| sha256 | `e3be6a63581adb2b3d3225286706ee14d185f8a8ddd18c50ea50069a98765932` |
| 来源 | 官方渠道（神牛 G3 升级包 / 官网下载），仅用于本人合法拥有设备的互操作研究 |
| 用途 | `keypatch/devices/ad300.py` 的原厂基准；文件本身被 `.gitignore` 忽略，**不要提交/分发** |

## 放置位置

```text
devices/ad300pro/firmware/AD300pro_V1.30.bin
```

studio「上传原厂固件」也会保存到这里，并按 `DEVICE["sha256"]` 校验。

## 自查

```powershell
(Get-FileHash devices\ad300pro\firmware\AD300pro_V1.30.bin -Algorithm SHA256).Hash.ToLower()
tools\python\python.exe keypatch\keypatch.py info --device ad300
```

> 换版本（V1.2 / V1.4 …）时**不能**直接复用本补丁：`apply()` 会逐字节校验
> 同步字函数与各补丁点原始字节，版本不符会拒绝打补丁。需要重新定位补丁点并更新
> `keypatch/devices/ad300.py`（流程见 `docs/新增设备指南.md`）。
