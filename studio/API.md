# studio HTTP API 契约

可视化密钥工坊（`studio/server.py`）对前端（`studio/static/`）暴露的本地 HTTP JSON API。

- 仅监听 `127.0.0.1`，无需鉴权（本机工具）。
- 所有响应均为 UTF-8 JSON，除非明确标注为二进制下载。
- 成功：`{"ok": true, ...}`；失败：HTTP 4xx/5xx + `{"ok": false, "error": "<原因>"}`。

## 1. 设备

### `GET /api/health`

```json
{"ok": true, "version": "1.0.0", "python": "3.12.10", "devices": 3}
```

### `GET /api/devices`

```json
{
  "ok": true,
  "devices": [
    {
      "id": "x3c",
      "name": "Godox X3-C",
      "kind": "trigger",            // trigger | flash
      "model": "X3-C",
      "firmware_version": "V1.38",
      "notes": "……",
      "base": "0x08004800",
      "out_name": "X3_C_v1.38_app_key{key:04X}.bin",
      "original_sha256": "45fdd952……",
      "firmware": {
        "present": true,
        "path": "devices/x3c/firmware/X3_C_v1.38_app.bin",
        "size": 1021468,
        "sha256": "45fdd952……",
        "matches": true
      },
      "features": {"verify": true, "container": true}
    }
  ],
  "key_rules": {
    "width": 4,
    "forbidden": ["0x0000", "0xFFFF", "0xAAAA", "0x5555", "0x8000"],
    "public_sync_values": ["0xC35C", "……"],
    "note": "密钥必须是 16 位值，且不能等于原厂 ID 能产生的同步字或保留值。"
  }
}
```

## 2. 密钥

三个接口返回同一种 `key` 对象：

```json
{
  "ok": true,
  "key": {"hex": "0xD798", "decimal": 55192, "sync1": "0xD7", "sync0": "0x98"}
}
```

- `POST /api/key/validate` — body `{"text": "D798" | "0xD798" | "55192"}`
- `POST /api/key/random` — body `{}`
- `POST /api/key/derive` — body `{"passphrase": "我的口令"}`（同一口令总是派生出同一密钥）

校验失败示例：`{"ok": false, "error": "key 0xFFFF 是保留/危险同步字"}`（HTTP 400）。
可附带 `"suggest": "0xD798"` 表示推荐的备用值。

## 3. 原厂固件上传

### `POST /api/firmware/upload?device=x3c`

请求体：原始文件字节（`Content-Type: application/octet-stream`）。
请求头：`X-Filename: X3_C_v1.38_app.bin`（可选，仅用于记录）。

成功：

```json
{"ok": true, "device": "x3c", "path": "devices/x3c/firmware/X3_C_v1.38_app.bin",
 "size": 1021468, "sha256": "45fdd952……", "matches": true}
```

sha256 与原厂不符（HTTP 400）：

```json
{"ok": false, "error": "固件 sha256 与原厂不匹配，已拒绝保存",
 "expected": "45fdd952……", "actual": "……"}
```

## 4. 生成 / 导出

### `POST /api/generate`

```json
{"key": "0xD798", "devices": ["ad100", "x3c"], "verify": true, "container": false}
```

| 字段 | 说明 |
|---|---|
| `key` | 同 validate 的文本（十六进制 / 十进制 / 口令） |
| `devices` | 要生成的设备 id 数组，至少 1 个 |
| `verify` | 是否执行离线验证（Unicorn 调用级 + 静态检查），默认 `true` |
| `container` | 是否额外打包 G3 V2.0 容器格式备用件（`*_container.bin`），默认 `false` |

成功（HTTP 200）：

```json
{
  "ok": true,
  "build": {
    "id": "20261107-214500_D798",
    "created": "2026-11-07T21:45:00+08:00",
    "key": {"hex": "0xD798", "decimal": 55192, "sync1": "0xD7", "sync0": "0x98"},
    "verify": true,
    "container": false,
    "files": [
      {
        "device": "x3c",
        "name": "Godox X3-C",
        "filename": "X3_C_v1.38_app_keyD798.bin",
        "size": 1021468,
        "sha256": "……",
        "original_sha256": "45fdd952……",
        "patch_count": 15,
        "patch_log": [
          {"addr": "0x080118B0", "offset": 73872, "before": "10 b5 0a 4c",
           "after": "dc f0 a6 bb", "desc": "send_short 注入"}
        ],
        "verify_results": [{"ok": true, "msg": "SYNC1=0xD7 SYNC0=0x98"}],
        "warnings": [],
        "download": "/api/builds/20261107-214500_D798/X3_C_v1.38_app_keyD798.bin"
      }
    ],
    "zip": "/api/builds/20261107-214500_D798/all.zip"
  }
}
```

失败（HTTP 400/500）：`{"ok": false, "error": "……", "failed_device": "ad200"}`。
单个设备失败时整次生成失败，但已成功项会放在 `partial` 字段里（用于排查）。

### `GET /api/builds`

最近 20 次生成记录（新→旧）：

```json
{"ok": true, "builds": [
  {"id": "20261107-214500_D798", "created": "……",
   "key": "0xD798", "files": [
     {"device": "x3c", "filename": "X3_C_v1.38_app_keyD798.bin", "size": 1021468, "sha256": "……"}
   ]}
]}
```

### `GET /api/builds/{id}`

返回该次生成的 `manifest.json`。

### `GET /api/builds/{id}/{filename}`

下载单个固件（`Content-Disposition: attachment`）。

### `GET /api/builds/{id}/all.zip`

把该次生成的所有文件打包成 zip 下载。

## 5. 静态资源

| 路径 | 内容 |
|---|---|
| `GET /` | `static/index.html` |
| `GET /static/*` | `static/` 下的静态文件 |
| `GET /favicon.svg` | 图标 |

## 6. 安全约定

- `build id` 只允许 `[0-9A-Za-z_.-]`，文件名只允许 basename，禁止 `..` 穿越。
- 原厂固件只保存在各设备目录的 `firmware/` 下（`.gitignore` 已忽略）。
- 生成物只保存在 `studio/builds/`（`.gitignore` 已忽略）。
- 后端不持久化密钥；密钥只出现在本次生成目录的 `manifest.json` 与文件名中（本机）。
