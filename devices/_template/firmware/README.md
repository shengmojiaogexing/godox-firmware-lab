# devices/_template/firmware/ —— 原厂固件存放点（本机，不入库）

把**从官方渠道获得的、未改动的原厂固件**放在本目录（复制到 `devices/<id>/firmware/` 后）。

## 为什么不在仓库里

- 固件版权归权利人所有；`.gitignore` 已忽略 `*.bin` / `*.hex` / `*.zip` 等，
  所以本目录里的固件**不会进入 git**，只保留在你本机；
- 仓库只记录**文件名 + sha256**，别人拿到正版固件后可以自行校验；
- studio 的「上传原厂固件」也会保存到 `devices/<设备>/firmware/`，并以 sha256 校验。

## 放进来之后

1. 计算 sha256：

   ```powershell
   # Windows PowerShell
   (Get-FileHash devices\<id>\firmware\<固件>.bin -Algorithm SHA256).Hash.ToLower()
   ```

   或：

   ```powershell
   tools\python\python.exe -c "import hashlib,sys;print(hashlib.sha256(open(sys.argv[1],'rb').read()).hexdigest())" devices\<id>\firmware\<固件>.bin
   ```

2. 校验工具能否识别（应输出 `sha256 OK`）：

   ```powershell
   tools\python\python.exe keypatch\keypatch.py list
   ```

3. 把下面这张表填好（**这个文件会入库**，让别人能核对版本）：

   | 项目 | 值 |
   |---|---|
   | 设备 | <厂商> <型号> |
   | 固件版本 | <如 V1.0> |
   | 文件名 | `<原厂固件>.bin` |
   | 字节数 | <size> |
   | sha256 | `<64 位 hex>` |
   | 来源 | <官方下载页 / 官方升级包文件名> |
   | 获取日期 | <YYYY-MM-DD> |
   | 备注 | <是否整片 flash / 是否含 bootloader / 解压密码等> |

## 注意

- **只放原厂、未改动的固件**；补丁产物请输出到 `keypatch/out/` 或 `studio/builds/`；
- 不要提交 `*.bin`（`.gitignore` 已挡）；不要用 `git add -f` 强推；
- 保留一份原厂固件用于**回滚**（G3 升级软件可刷回）；
- 若同时拿到多个版本，文件名保留版本号（如 `V1C_V1.0.bin`、`V1C_V1.1.bin`），
  并更新 `keypatch/devices/<id>.py` 的 `sha256` / `image` 指向当前支持的那个版本。
