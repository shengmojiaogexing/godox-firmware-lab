# keypatch 设备模块接口（给团队 / 后续二开用）

每个待打补丁的固件对应 `keypatch/devices/<id>.py` 一个模块，模块必须提供：

```python
DEVICE = {
    "id": "ad100",                       # 与文件名一致
    "name": "Godox AD100 Pro",
    "kind": "flash",                     # trigger=引闪器 / flash=闪光灯（studio 卡片用）
    "model": "AD100 Pro",                # 型号（studio 显示用，缺省时 studio 用 name 兜底）
    "firmware_version": "V1.2",          # 固件版本（studio 显示用，缺省时留空）
    "image": "devices/ad100pro/firmware/AD100pro_V1.2.bin",  # 相对仓库根
    "base": 0x08003000,                  # 镜像加载/链接基址
    "sha256": "<原厂固件 sha256>",        # 输入校验，防止拿错文件
    "out_name": "AD100pro_V1.2_key{key:04X}.bin", # 输出文件名模板
    "notes": "一句话说明",                # 可选
}

def apply(data: bytes, key: int) -> tuple[bytes, list[dict]]:
    """把原厂镜像 data 打上"同步字=key"补丁，返回 (patched_bytes, patch_log)。

    要求：
    - 必须是原位补丁，输出长度与输入完全一致；
    - 修改前逐字节校验 original 字节，不匹配立即抛 ValueError（防止固件版本不对）；
    - patch_log 每项形如
      {"addr": 0x0800B75C, "offset": 0x875C, "before": "41 1c",
       "after": "01 21", "desc": "..."}
    """

def verify(patched: bytes, key: int) -> list[tuple[bool, str]]:
    """（可选但强烈建议）离线验证补丁后的补丁点，返回 [(ok, 说明), ...]。
    推荐用 Unicorn 直接执行打过补丁的同步字设置函数，把写 CC2500 寄存器的
    辅助函数替换成记录桩，断言 SYNC1=key>>8, SYNC0=key&0xFF。
    如果没有实现，keypatch.py verify 会退化为静态字节检查。
    """
```

补充约定：

- 模块只能依赖标准库 + `keypatch/common.py`；验证脚本可以用项目自带的
  `tools/python/python.exe`（capstone 5.0.7 / unicorn 2.1.4）。
  注意：**模块在导入阶段**不要 import capstone/unicorn（只在 `verify()` 内部延迟导入），
  这样 studio 后端只依赖标准库也能 `import devices` 并列出设备。
- 不要在模块里写死绝对路径；`image` 用相对仓库根的路径。
- 补丁点、推导过程、验证证据写到 `keypatch/dev/<id>/NOTES.md`。
- 完成的成品 bin 统一由 `keypatch.py patch / patch-all` 输出到 `keypatch/out/`；
  加 `--container` 会额外生成 `*_container.bin`（G3 V2.0 容器备用件）。
- **原厂文件本身就是 G3 V2.0 容器的设备**（如 X2T-C）请在 `DEVICE` 里声明
  `"container_builtin": True`：`apply()` 负责拆容器、改 payload、重算 MD5 并封回，
  CLI/studio 会自动跳过额外的容器打包（避免二次套壳），`list_devices()` 里
  `features.container` 也会置为 `False`。

---

## 设备注册 API（studio 后端 / 外部脚本用）

`keypatch/devices/__init__.py` 暴露以下公开函数（**只依赖标准库，且不向外抛异常**）：

### `DEVICE_IDS`

已注册设备 id 的列表（顺序即展示顺序）：`["ad100", "ad200", "x3c", "x3pros", "x3s", "v1c", ...]`。
新增设备时追加 id 并在 `keypatch/devices/` 下新建同名模块。

### `get(device_id) -> module | None`

返回设备模块对象；未注册或导入失败返回 `None`（失败原因追加到 `ERRORS`）。

### `load(device_id) -> module | None`

旧接口，兼容保留：未注册 id 抛 `KeyError`，模块导入失败返回 `None`。

### `available() -> list[str]`

已实现（模块可导入）的设备 id 列表；`keypatch.py patch-all` 使用它。

### `list_devices() -> list[dict]`

返回全部已注册设备的元数据，可直接 JSON 序列化交给 studio：

```python
[
  {
    "id": "x3c",
    "name": "Godox X3-C (X3 C trigger)",
    "kind": "trigger",                       # trigger | flash
    "model": "X3-C",
    "firmware_version": "V1.38",
    "image": "devices/x3c/firmware/X3_C_v1.38_app.bin",
    "base": 134236160,                       # int（补丁引擎用）
    "base_hex": "0x08004800",                # studio JSON API 用这个
    "sha256": "45fdd952…",                    # 期望的原厂 sha256
    "original_sha256": "45fdd952…",           # 同 sha256（studio API 字段名）
    "out_name": "X3_C_v1.38_app_key{key:04X}.bin",
    "notes": "……",
    "available": True,                        # 模块可加载 + 固件就位 + sha256 匹配
    "firmware": {
      "present": True,
      "path": "devices/x3c/firmware/X3_C_v1.38_app.bin",  # 仓库相对路径
      "path_abs": "<repo>\\devices\\x3c\\firmware\\X3_C_v1.38_app.bin",  # 本机绝对路径
      "size": 1021468,
      "sha256_actual": "45fdd952…",           # 实际文件 sha256
      "matches": True                          # 实际 == 期望；未声明 sha256 时为 None
    },
    "features": {"verify": True, "container": True},
  },
  ...
]
```

约定：

- `kind` / `model` / `firmware_version` 原样透传 `DEVICE` 字段，**缺省为 `None`，由 studio 兜底**
  （`kind` 缺省可当 `flash`，`model` 缺省可用 `name`，`firmware_version` 缺省可留空）。
- `firmware.present=False` 或 `matches=False` 时 `available=False`；studio 应提示用户上传原厂固件。
- 加载失败的设备**不会**出现在返回值里，错误可通过 `list_load_errors() -> list[dict]` 读取：

```python
[{"id": "v862c", "stage": "import", "error": "ModuleNotFoundError: No module named 'devices.v862c'"}]
```

stdio 示例：

```python
import sys
sys.path.insert(0, "keypatch")   # 让 `import devices` 生效（与 keypatch.py 相同做法）
import devices
for d in devices.list_devices():
    print(d["id"], d["name"], d["kind"], d["available"], d["firmware"]["present"])
```
