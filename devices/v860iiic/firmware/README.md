# devices/v860iiic/firmware/ —— 原厂固件存放点（本机，不入库）

把**从官方渠道获得的、未改动的原厂固件**放在本目录。

## 固件身份

| 项目 | 值 |
|---|---|
| 设备 | Godox V860III-C（神牛 V860III-C 闪光灯） |
| 固件版本 | **V1.3** |
| 文件名 | `V860IIIC_V1.3.bin` |
| 字节数 | **87,612 B** |
| sha256 | `8530a7e85ce1098d64d72d7ea4a28f3fdfb489e3d39da49be1ffa166f6ba270d` |
| 镜像基址 | `0x08003000`（app 镜像占 `0x08003000–0x0801863B`） |
| 来源 | 官方 G3 升级包 `V860IIIC_V1.3.zip`（解包后为裸 `V860IIIC_V1.3.bin`） |
| 备注 | 前 12 KB（`0x08000000–0x08002FFF`）是厂商 bootloader，**不在**本 `.bin` 内；文件是整段 app 镜像 |

> 上表同时是 `keypatch/devices/v860iiic.py` 里 `IMAGE_SIZE` / `DEVICE["sha256"]` 的登记值：
> 长度或 sha256 任一不符，`apply()` 会直接拒绝打补丁（防刷错版本）。

## 为什么不在仓库里

- 固件版权归权利人所有；`.gitignore` 已忽略 `*.bin` / `*.hex` / `*.zip` 等，
  所以本目录里的固件**不会进入 git**，只保留在你本机；
- 仓库只记录**文件名 + 字节数 + sha256**，别人拿到正版固件后可以自行校验；
- studio 的「上传原厂固件」也会保存到 `devices/v860iiic/firmware/`，并以 sha256 校验。

## 放进来之后

1. 计算 sha256 并核对上表：

   ```powershell
   # Windows PowerShell（仓库根）
   (Get-FileHash devices\v860iiic\firmware\V860IIIC_V1.3.bin -Algorithm SHA256).Hash.ToLower()
   ```

   或：

   ```powershell
   tools\python\python.exe -c "import hashlib,sys;print(hashlib.sha256(open(sys.argv[1],'rb').read()).hexdigest())" devices\v860iiic\firmware\V860IIIC_V1.3.bin
   ```

2. 校验工具能否识别（v860iiic 行应输出 `sha256 OK`）：

   ```powershell
   tools\python\python.exe keypatch\keypatch.py list
   ```

3. 生成补丁前先跑一次模块自检（输出到 `%TEMP%`，不要覆盖正式成品）：

   ```powershell
   tools\python\python.exe keypatch\keypatch.py patch --device v860iiic --key 0x6B21 --verify --out %TEMP%\kp_v860iiic
   ```

## 注意

- **只放原厂、未改动的固件**；补丁产物请输出到 `keypatch/out/` 或 `studio/builds/`；
- 不要提交 `*.bin`（`.gitignore` 已挡）；不要用 `git add -f` 强推；
- 保留一份原厂固件用于**回滚**（G3 升级软件可刷回）；
- 固件版本一旦升级（例如 V1.4），本目录要换成新文件、更新上表与
  [`../../../keypatch/devices/v860iiic.py`](../../../keypatch/devices/v860iiic.py) 的
  `IMAGE_SIZE` / `DEVICE["sha256"]`，并重新定位全部补丁点（方法见
  [`../docs/01_静态证据与复现.md`](../docs/01_静态证据与复现.md)）。
