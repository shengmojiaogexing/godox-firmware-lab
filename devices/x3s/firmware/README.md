# devices/x3s/firmware/

放 **Godox X3-S** 的原厂固件（本目录被 `.gitignore` 忽略 `*.bin`，只在本机存在）。

| 文件名 | 版本 | 大小 | SHA-256 |
|---|---|---|---|
| `X3_S_v1.25_app.bin` | V1.25 | 1,012,268 B | `3cd40ef3f91369d3147c538cf6745f9308d82e14ad94c9bad9c8f129e0753e4d` |

- 来源：官方 G3 升级软件下载的 X3-S 固件包中解出的 app 镜像（裸 bin，无容器）。
- 校验：
  ```powershell
  (Get-FileHash devices\x3s\firmware\X3_S_v1.25_app.bin -Algorithm SHA256).Hash.ToLower()
  ```
- 打补丁后长度不变（原位补丁），可直接用官方 G3 升级软件刷写；换密钥需重新生成。
