# devices/x3s/docs/ —— X3-S 逆向资料

| 文件 | 内容 |
|---|---|
| [`01_静态证据与复现.md`](01_静态证据与复现.md) | 地址映射（X3-C→X3-S）、同步字函数、双发/按键注入反汇编、SYNC 写入点扫描、安全 RAM、验证与复现 |

配套过程材料在 `keypatch/dev/x3s/`：

- `analyze.py` / `analyze.log` —— 静态证据（镜像、映射、注入反汇编、入口解码）
- `unicorn_sync_test.py` / `unicorn_sync_test.log` —— v1 固定 KEY 语义的独立 Unicorn 交叉验证
- `keybtn_build.py` / `keybtn_build.log` —— keybtn（默认）/ fixed（v1）构建 + 模块 verify（17 项）
- `keybtn_runtime.py` / `keybtn_runtime.log` —— 整机模拟器端到端（BSS 安全 RAM、真实按键扫描、双发）
- `boot_regression.py` / `boot_regression.log` —— 原厂 vs 补丁启动回归（120 万条指令）
- `NOTES.md` —— 补丁规格、验证记录、交付清单、风险

补丁模块：`keypatch/devices/x3s.py`（`apply` 默认 keybtn；`apply_fixed` 为 v1；`verify` 自动识别模式）。
