# -*- coding: utf-8 -*-
"""keypatch 设备模块注册表：devices/<id>.py 的加载与元数据导出（CLI / studio 共用）。

设计目标
--------
1. **向后兼容**：`DEVICE_IDS` / `load()` / `available()` 语义与旧版一致（命令行仍在用）。
2. **给 studio 后端一个稳定契约**：`list_devices()` 直接返回可 JSON 序列化的设备元数据
   （id/name/kind/model/firmware_version/image/base/sha256/out_name/notes/available 以及
   firmware{present,path_abs,size,sha256_actual,matches}），studio 不需要再自己解析模块。
3. **只依赖标准库**：本模块与 devices/*.py 在导入阶段只 import 标准库
   （importlib / os / hashlib 等）；capstone/unicorn 只在 verify() 内部延迟导入。
   所以 studio 后端即使在"裸 Python"环境里也能列出设备。
4. **绝不抛异常**：模块加载、DEVICE 解析、读固件失败都不会打断列表；失败项跳过并追加到
   模块级 `ERRORS`，可用 `list_load_errors()` 查看。

新增一个设备
------------
在 keypatch/devices/ 下新建 `<id>.py`（接口见 ../INTERFACE.md），把 id 追加到 `DEVICE_IDS`，
并建议在 DEVICE 里补 `kind`（"trigger" | "flash"）、`model`、`firmware_version` 三个字段。
"""
from __future__ import annotations

import hashlib
import importlib
import os

# 已注册的设备 id（顺序 = CLI / studio 里的展示顺序；保持与旧版一致）。
DEVICE_IDS = ["ad100", "ad200", "x3c", "x3pro", "x3pros", "x3s", "v1c", "v1s", "v860iiic", "v860iiis", "ad300", "x2tc", "x2ts"]

_HERE = os.path.dirname(os.path.abspath(__file__))
KEYPATCH_DIR = os.path.dirname(_HERE)          # <repo>/keypatch
ROOT = os.path.dirname(KEYPATCH_DIR)           # <repo>

# 最近一次加载/列举过程中出现的错误（list[dict]：id / stage / error）。
# 注意：这是模块级状态，`get()` 与 `list_devices()` 都可能追加；`list_devices()` 会先清空。
ERRORS: list[dict] = []

# 一个稳定的"设备元数据契约"版本号，studio/文档可引用，方便以后加字段。
SCHEMA_VERSION = 1


def _record_error(device_id: str, stage: str, exc) -> dict:
    item = {
        "id": device_id,
        "stage": stage,                        # import | metadata | firmware
        "error": "%s: %s" % (type(exc).__name__, exc),
    }
    ERRORS.append(item)
    return item


def _import_device(device_id: str):
    """导入设备模块，返回 (module, error_str)。任何异常都转成字符串，不向外抛。"""
    try:
        return importlib.import_module(__name__ + "." + device_id), None
    except Exception as exc:  # noqa: BLE001 —— 加载失败也要能列其它设备
        return None, "%s: %s" % (type(exc).__name__, exc)


def get(device_id: str):
    """返回设备模块；未注册或加载失败返回 None（不抛异常，失败记入 ERRORS）。"""
    if device_id not in DEVICE_IDS:
        return None
    mod, err = _import_device(device_id)
    if mod is None:
        _record_error(device_id, "import", err)
    return mod


def load(device_id: str):
    """兼容旧接口：返回设备模块。

    - 未注册的 id 仍抛 KeyError（旧版行为，CLI 依赖）；
    - 已注册但模块导入失败返回 None（并把错误记入 ERRORS）。
    """
    if device_id not in DEVICE_IDS:
        raise KeyError("未知设备 %r，可用: %s" % (device_id, ", ".join(DEVICE_IDS)))
    return get(device_id)


def available() -> list[str]:
    """已实现（模块可加载）的设备 id 列表；保持旧语义，供 CLI patch-all 使用。"""
    out = []
    for device_id in DEVICE_IDS:
        try:
            if get(device_id) is not None:
                out.append(device_id)
        except Exception:  # noqa: BLE001 —— available() 不应因单个设备失败而抛
            pass
    return out


def list_load_errors() -> list[dict]:
    """返回最近一次 `list_devices()` 期间记录的加载失败项（拷贝，防止外部改内部状态）。"""
    return [dict(e) for e in ERRORS]


def _sha256_file(path: str) -> str:
    """流式计算文件 sha256（避免一次把 1 MB 固件读进内存，也为将来大镜像留余量）。"""
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def _firmware_info(info: dict) -> dict:
    """探测 DEVICE["image"] 指向的原厂固件，返回 studio 需要的 firmware 子结构。

    键：present / path / path_abs / size / sha256_actual / matches（+ 可选 error）。
    - matches 为 None 表示模块没声明预期 sha256，无法判断；
    - 任何读文件异常都收敛成 present=False / error，不向外抛。
    """
    image = info.get("image")
    if not image:
        return {"present": False, "path": None, "path_abs": None, "size": None,
                "sha256_actual": None, "matches": None,
                "error": "DEVICE 缺少 image 字段"}
    path_abs = image if os.path.isabs(image) else os.path.join(ROOT, image)
    path_abs = os.path.normpath(path_abs)
    out = {
        "present": False,
        "path": image,                    # 仓库相对路径（studio 上传/展示用）
        "path_abs": path_abs,             # 本机绝对路径（调试用）
        "size": None,
        "sha256_actual": None,
        "matches": None,
    }
    try:
        if not os.path.isfile(path_abs):
            out["error"] = "原厂固件不存在"
            return out
        out["present"] = True
        out["size"] = os.path.getsize(path_abs)
        out["sha256_actual"] = _sha256_file(path_abs)
        want = info.get("sha256")
        if want:
            out["matches"] = (out["sha256_actual"].lower() == str(want).lower())
    except Exception as exc:  # noqa: BLE001
        out["present"] = False
        out["error"] = "%s: %s" % (type(exc).__name__, exc)
    return out


def list_devices() -> list[dict]:
    """返回全部已注册设备的元数据列表（studio 后端 / CLI 直接可用，不抛异常）。

    每个元素：
      id / name / kind / model / firmware_version / image / base / base_hex /
      sha256 / original_sha256 / out_name / notes / available /
      firmware{present,path,path_abs,size,sha256_actual,matches[,error]} /
      features{verify,container}

    约定：
    - `base` 是 int（补丁引擎用），`base_hex` 是 "0x08004800" 字符串（studio JSON API 用）；
    - `sha256` 与 `original_sha256` 同值（后者是 studio API 的字段名）；
    - `available` = 模块加载成功 + 原厂固件存在 + sha256 匹配（没声明 sha256 时只要存在）；
    - `kind` / `model` / `firmware_version` 原样透传 DEVICE 字段，缺省为 None，由 studio 兜底；
    - 加载失败的设备不出现，错误见 `list_load_errors()`。
    """
    del ERRORS[:]
    out: list[dict] = []
    for device_id in DEVICE_IDS:
        mod = get(device_id)
        if mod is None:
            continue
        try:
            raw = getattr(mod, "DEVICE", None)
            if not isinstance(raw, dict):
                raise TypeError("DEVICE 不是 dict")
            info = dict(raw)
        except Exception as exc:  # noqa: BLE001
            _record_error(device_id, "metadata", exc)
            continue
        try:
            firmware = _firmware_info(info)
            base = info.get("base", 0)
            base_int = int(base) if isinstance(base, int) else 0
            sha256 = info.get("sha256")
            present = bool(firmware.get("present"))
            matches = firmware.get("matches")
            entry = {
                "id": info.get("id", device_id),
                "name": info.get("name") or device_id,
                "kind": info.get("kind"),                     # trigger | flash
                "model": info.get("model"),
                "firmware_version": info.get("firmware_version"),
                "image": info.get("image"),
                "base": base_int,
                "base_hex": "0x%08X" % base_int,
                "sha256": sha256,
                "original_sha256": sha256,
                "out_name": info.get("out_name"),
                "notes": info.get("notes") or "",
                # 现在就能生成补丁：模块在 + 固件在 + sha256 对得上（未声明 sha256 时只要求存在）
                "available": bool(present and matches is not False),
                "firmware": firmware,
                # 原厂文件本身即 G3 V2.0 容器（如 X2T-C）：apply() 已就地重算 MD5 封好，
                # CLI/studio 不应再套一层容器。
                "container_builtin": bool(info.get("container_builtin")),
                "features": {
                    "verify": callable(getattr(mod, "verify", None)),
                    "container": (not bool(info.get("container_builtin"))),
                },
            }
        except Exception as exc:  # noqa: BLE001
            _record_error(device_id, "metadata", exc)
            continue
        out.append(entry)
    return out
