# studio — 神牛密钥工坊（可视化平台）

浏览器里完成整套「私有密钥固件」流程：**选设备 → 自定义密钥 → 生成 → 下载刷机**。

- 纯标准库 HTTP 服务（`studio/server.py`），只监听 `127.0.0.1`；
- 前端是零构建单页（`studio/static/`），离线可用、无 CDN；
- 复用 `keypatch/` 的补丁引擎与设备模块，设备接入后会自动出现在界面上。

## 启动

```powershell
# 仓库根目录
tools\python\python.exe studio\server.py --port 18780 --open
# 或双击 scripts\start_studio.bat
```

打开 <http://127.0.0.1:18780/>。

| 参数 | 说明 |
|---|---|
| `--port` | 端口，默认 18780 |
| `--host` | 监听地址，默认 127.0.0.1（不建议改） |
| `--open` | 启动后自动打开浏览器 |
| `--builds-dir` | 生成物目录，默认 `studio/builds/`（已 gitignore） |

## 使用流程

1. **准备原厂固件**：放到 `devices/<设备>/firmware/<文件>.bin`，或在界面上上传（会校验 sha256）。
2. **勾选设备**：灯和引闪器一起选，保证刷入同一把密钥。
3. **输入密钥**：4 位十六进制（如 `D798`），可随机生成或由口令派生。
4. **生成并导出**：下载单个 `.bin` 或全部打成一个 zip。
5. **刷机**：用官方 G3 升级软件打开生成的 `.bin` 更新。

> 密钥只写入本次生成目录的 `manifest.json`（本机 / 服务器本地）。换密钥后所有设备都要重刷。

## 界面能力

- 顶部流程条：选择设备 → 设置密钥 → 生成固件，步骤状态实时更新；
- 设备分组：引闪器 / 闪光灯分栏展示，组头显示「已选 x / y」与数量；
- 设备卡片：固件版本、sha256、备注、勾选/全选，缺固件可就地拖拽上传；
- 密钥工坊：实时合法性校验、随机、口令派生、SYNC1/SYNC0 预览与目标设备数；
- 生成选项：离线验证（Unicorn 调用级 + 静态检查）、G3 V2.0 容器备用件；
- 结果：文件名/sha256/下载、补丁点明细、验证 PASS/FAIL、警告；
- 刷入教程：结果区附带通用刷入/验证/回滚步骤，以及按本次生成设备自动生成的「密钥开关」对照表；
- 历史：按日期分组的生成时间线，默认显示最近 6 条，可展开全部（最多 20 条）；
- `?mock=1`：内置示例数据的离线演示模式（无后端也能看界面）。

## 部署到服务器

默认只监听 `127.0.0.1`。需要在局域网 / 内网提供访问时，建议用反向代理（Nginx / Caddy）转发到 `http://127.0.0.1:18780/`，并至少做到：

1. 启用 HTTPS 与访问控制（Basic Auth、SSO 或 IP 白名单）；
2. 限制 `studio/builds/` 的访问并定期清理——生成物里含有自定义密钥；
3. 不要直接把 `--host 0.0.0.0` 暴露到公网；必须暴露时加登录、限流与审计；
4. 前端零外部依赖（无 CDN），静态资源可交给反代缓存，HTML/API 保持 `no-store`。

密钥只写入生成目录的 `manifest.json`，请按敏感数据管理。

## 目录

```text
studio/
├─ server.py       # HTTP 服务 + API
├─ static/         # index.html / app.js / styles.css / favicon.svg
├─ tests/
│  ├─ test_api.py  # API 回归（stdlib unittest）
│  └─ test_ui.mjs  # 无头 Edge + CDP 的 UI 回归
├─ API.md          # 接口契约
├─ builds/         # 生成物（gitignore）
└─ README.md
```

## 测试

```powershell
# API（自动用临时生成目录，不会污染 studio/builds）
tools\python\python.exe studio\tests\test_api.py

# UI（需要先启动一个 studio 服务；脚本会自己拉起 headless Edge）
node studio\tests\test_ui.mjs --url http://127.0.0.1:18780/
```

## 排障

| 现象 | 处理 |
|---|---|
| 提示「原厂固件未就绪」 | 把对应原厂 `.bin` 放入 `devices/<id>/firmware/`，或界面里上传 |
| 上传报 sha256 不匹配 | 版本不对；对照 `docs/设备清单.md` 的 sha256，或先更新设备模块 |
| 验证失败 | 设备模块与固件版本不匹配，先运行 `keypatch.py selftest.py` 排查 |
| 端口被占用 | 换 `--port 18781` |
| 界面空白 | 看终端日志；确认用 `http://127.0.0.1:18780/` 打开而不是直接双击 html |
