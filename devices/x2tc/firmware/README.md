# firmware/ —— Godox X2T-C 原厂固件（本机，gitignore）

| 项 | 值 |
|---|---|
| 官方文件名 | `X2TC_Z02_V1.01.bin` |
| 适用设备 | Godox **X2T-C**（Canon 版 2.4 GHz 引闪器，X2T 系列） |
| 版本 | V1.01 |
| 文件大小 | 79,748 B |
| sha256 | `d185e4101ad61422f2ac2bc9ee7ade1e6c245c6002361dfd50ed5f9e7b90210b` |
| 格式 | **G3 V2.0 容器**：payload 79,704 B + 16 B MD5 + 内嵌文件名 `X2TC_Z02_V1.01.bin`(18 B) + u16 + 8 B 魔数 `AA55669977882233` |
| 来源 | 官方渠道（G3 升级软件 / 官网固件包），本机只读副本 |
| 回滚 | G3 → 打开文件 → 选本文件 → 更新 |

- 本文件是 **原厂未修改镜像**，`keypatch` / studio 的所有补丁产物都输出到
  `keypatch/out/` 或 `studio/builds/`，不会覆盖这里。
- 该固件是 `keypatch` 里第一台「原厂文件本身即 G3 V2.0 容器」的设备；
  补丁产物同样保持容器格式（长度不变、MD5 同步刷新），详见
  [`../README.md`](../README.md) 与 `keypatch/devices/x2tc.py`。
- 官方 G3 升级软件对该容器只做「文件名含设备 ID」的型号匹配与每 1024 B 字节和校验
  （见 `keypatch/dev/g3/FINDINGS.md`），清单见 `docs/设备清单.md`。
