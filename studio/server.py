#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""studio/server.py — 神牛密钥工坊（Godox Key Studio）本地服务。

浏览器里勾选设备、自定义 16 位私有密钥，一键生成并导出可刷入的固件。
纯标准库实现（生成/验证会复用 keypatch 与设备模块，验证步骤依赖 unicorn 时自动降级）。

启动：
    python studio/server.py --port 18780 --open

API 契约见同目录 API.md。
"""
from __future__ import annotations

import argparse
import json
import os
import shutil
import stat
import sys
import threading
import time
import traceback
import webbrowser
import zipfile
from datetime import datetime
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import parse_qs, quote, unquote, urlparse

VERSION = "1.0.0"
MAX_BODY = 32 * 1024 * 1024      # 32 MB 请求体上限
MAX_UPLOAD = 16 * 1024 * 1024    # 16 MB 固件上传上限

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
KEYPATCH_DIR = os.path.join(REPO, "keypatch")
STATIC_DIR = os.path.join(HERE, "static")
DEFAULT_BUILDS_DIR = os.path.join(HERE, "builds")

if KEYPATCH_DIR not in sys.path:
    sys.path.insert(0, KEYPATCH_DIR)

import common  # noqa: E402  (keypatch/common.py)
import devices as dev_registry  # noqa: E402  (keypatch/devices/__init__.py)

try:  # container.py 由 keypatch 提供；缺失时容器功能自动隐藏
    import container as g3_container  # noqa: E402
except Exception:  # noqa: BLE001
    g3_container = None


# --------------------------------------------------------------------------- #
# 基础工具
# --------------------------------------------------------------------------- #
class ApiError(Exception):
    """带 HTTP 状态码的 API 错误。"""

    def __init__(self, message: str, status: int = 400, **extra):
        super().__init__(message)
        self.status = status
        self.extra = extra


def _resolve(path: str) -> str:
    if not path:
        return ""
    return path if os.path.isabs(path) else os.path.normpath(os.path.join(REPO, path))


def _rel(path: str) -> str:
    try:
        return os.path.relpath(path, REPO).replace(os.sep, "/")
    except ValueError:
        return path


def _sha256(data: bytes) -> str:
    return common.sha256_hex(data)


def _key_obj(key: int) -> dict:
    return {
        "hex": "0x%04X" % key,
        "decimal": key,
        "sync1": "0x%02X" % (key >> 8),
        "sync0": "0x%02X" % (key & 0xFF),
    }


def _suggest_key(bad: int | None) -> int:
    if isinstance(bad, int) and 0 <= bad <= 0xFFFF:
        for delta in range(1, 8192):
            cand = (bad + delta) & 0xFFFF
            try:
                return common.validate_key(cand)
            except common.PatchError:
                continue
    return common.random_key()


# --------------------------------------------------------------------------- #
# 设备注册表
# --------------------------------------------------------------------------- #
# 兜底元数据：仅在 keypatch/devices/<id>.py 的 DEVICE 缺少 kind/model/firmware_version 时使用。
# 正常情况下设备元数据优先来自设备模块（devices.list_devices()），这里只是老模块的补齐表。
_FALLBACK_META = {
    "ad100": {"kind": "flash", "model": "AD100 Pro", "firmware_version": "V1.2"},
    "ad200": {"kind": "flash", "model": "AD200 Pro", "firmware_version": "V1.60"},
    "x3c": {"kind": "trigger", "model": "X3-C", "firmware_version": "V1.38"},
    "x3pros": {"kind": "trigger", "model": "X3 Pro S", "firmware_version": "V1.11"},
    "x2ts": {"kind": "trigger", "model": "X2T-S", "firmware_version": "V1.4"},
}


def _device_ids() -> list[str]:
    ids = getattr(dev_registry, "DEVICE_IDS", None)
    if ids is None:
        ids = [d for d in dir(dev_registry) if not d.startswith("_")]  # 兜底
    return list(ids)


def _load_device(device_id: str):
    load = getattr(dev_registry, "get", None)
    if load is not None:
        return load(device_id)
    return dev_registry.load(device_id)


def _device_rows() -> list[dict]:
    rows = []
    for device_id in _device_ids():
        try:
            mod = _load_device(device_id)
        except Exception:  # noqa: BLE001
            mod = None
        if mod is None:
            continue
        info = dict(getattr(mod, "DEVICE", {}) or {})
        fb = _FALLBACK_META.get(device_id, {})
        image = info.get("image", "")
        abs_path = _resolve(image)
        present = bool(abs_path) and os.path.isfile(abs_path)
        actual = None
        size = None
        if present:
            try:
                raw = common.read_file(abs_path)
                actual = _sha256(raw)
                size = len(raw)
            except OSError:
                present = False
        expected = info.get("sha256")
        matches = bool(present and expected and actual == expected)
        base = info.get("base")
        container_builtin = bool(info.get("container_builtin"))
        rows.append({
            "id": device_id,
            "name": info.get("name", device_id),
            "kind": info.get("kind") or fb.get("kind", "flash"),
            "model": info.get("model") or fb.get("model", info.get("name", device_id)),
            "firmware_version": info.get("firmware_version") or fb.get("firmware_version", ""),
            "notes": info.get("notes", ""),
            "base": ("0x%08X" % base) if isinstance(base, int) else str(base or ""),
            "out_name": info.get("out_name", "%s_key{key:04X}.bin" % device_id),
            "original_sha256": expected,
            "firmware": {
                "present": present,
                "path": _rel(abs_path),
                "size": size,
                "sha256": actual,
                "matches": matches,
            },
            # 原厂文件即 G3 V2.0 容器（如 X2T-C）：输出已内嵌容器，不再二次打包
            "container_builtin": container_builtin,
            "features": {
                "verify": hasattr(mod, "verify"),
                "container": (g3_container is not None) and (not container_builtin),
            },
        })
    return rows


def _key_rules() -> dict:
    public = sorted(common.public_sync_values())
    return {
        "width": 4,
        "forbidden": ["0x%04X" % v for v in sorted(common.EXTRA_FORBIDDEN)],
        "public_sync_values": ["0x%04X" % v for v in public],
        "note": "密钥必须是 16 位值（0x0001-0xFFFE），且不能等于原厂 ID 能产生的 100 个同步字或保留值。",
    }


# --------------------------------------------------------------------------- #
# 密钥解析
# --------------------------------------------------------------------------- #
def _parse_key(text, strict: bool = False) -> int:
    raw = (text or "").strip() if isinstance(text, str) else text
    if raw is None or raw == "":
        raise ApiError("密钥不能为空")
    if isinstance(raw, int):
        try:
            return common.validate_key(raw)
        except common.PatchError as exc:
            raise ApiError(str(exc), suggest="0x%04X" % _suggest_key(raw))
    text = str(raw).strip()
    try:
        if text.lower().startswith("0x"):
            value = int(text, 16)
        elif len(text) <= 4 and all(c in "0123456789abcdefABCDEF" for c in text):
            value = int(text, 16)
        elif text.isdigit():
            value = int(text, 10)
        else:
            if strict:
                raise ApiError("密钥必须是 1-4 位十六进制或十进制数字")
            return common.derive_key(text)
    except ValueError:
        raise ApiError("无法解析密钥 %r" % text)
    try:
        return common.validate_key(value)
    except common.PatchError as exc:
        raise ApiError(str(exc), suggest="0x%04X" % _suggest_key(value))


# --------------------------------------------------------------------------- #
# 生成
# --------------------------------------------------------------------------- #
_gen_lock = threading.Lock()


def _new_build_id(key: int, builds_dir: str) -> str:
    stem = "%s_%04X" % (time.strftime("%Y%m%d-%H%M%S"), key)
    candidate = stem
    n = 2
    while os.path.isdir(os.path.join(builds_dir, candidate)):
        candidate = "%s-%d" % (stem, n)
        n += 1
    return candidate


def _normalize_verify(results) -> list[dict]:
    out = []
    for item in results or []:
        if isinstance(item, dict):
            out.append({"ok": bool(item.get("ok")), "msg": str(item.get("msg", ""))})
        elif isinstance(item, (tuple, list)) and len(item) >= 2:
            out.append({"ok": bool(item[0]), "msg": str(item[1])})
        else:
            out.append({"ok": bool(item), "msg": ""})
    return out


def _normalize_patch_log(log) -> list[dict]:
    out = []
    for entry in log or []:
        if not isinstance(entry, dict):
            continue
        addr = entry.get("addr")
        out.append({
            "addr": ("0x%08X" % addr) if isinstance(addr, int) else str(addr or ""),
            "offset": entry.get("offset"),
            "before": str(entry.get("before", "")),
            "after": str(entry.get("after", "")),
            "desc": str(entry.get("desc", "")),
        })
    return out


def _build_zip(build_dir: str, build_id: str) -> str:
    """把生成目录里的固件打包成 all.zip（不含 manifest.json）。"""
    zip_path = os.path.join(build_dir, "all.zip")
    with zipfile.ZipFile(zip_path, "w", zipfile.ZIP_DEFLATED) as zf:
        for name in sorted(os.listdir(build_dir)):
            full = os.path.join(build_dir, name)
            if not os.path.isfile(full):
                continue
            if name in ("manifest.json", "all.zip"):
                continue
            zf.write(full, arcname=name)
    return zip_path


def api_generate(payload: dict, builds_dir: str) -> dict:
    key = _parse_key(payload.get("key"))
    device_ids = payload.get("devices") or []
    if not isinstance(device_ids, list) or not device_ids:
        raise ApiError("请至少选择一台设备")
    do_verify = bool(payload.get("verify", True))
    want_container = bool(payload.get("container", False))
    if want_container and g3_container is None:
        raise ApiError("容器模块不可用（keypatch/container.py 缺失）")

    known = {row["id"]: row for row in _device_rows()}
    unknown = [d for d in device_ids if d not in known]
    if unknown:
        raise ApiError("未知设备: %s" % ", ".join(map(str, unknown)))

    ordered = []
    for d in device_ids:
        if d not in ordered:
            ordered.append(d)

    missing = [d for d in ordered if not known[d]["firmware"]["matches"]]
    if missing:
        detail = ", ".join(
            "%s(%s)" % (d, "缺失" if not known[d]["firmware"]["present"] else "sha256 不匹配")
            for d in missing)
        raise ApiError("原厂固件未就绪: %s —— 请先放入/上传原厂固件" % detail, devices=missing)

    with _gen_lock:
        build_id = _new_build_id(key, builds_dir)
        build_dir = os.path.join(builds_dir, build_id)
        os.makedirs(build_dir, exist_ok=True)
        files: list[dict] = []
        try:
            for device_id in ordered:
                mod = _load_device(device_id)
                info = dict(getattr(mod, "DEVICE", {}) or {})
                src_path = _resolve(info.get("image", ""))
                src = common.read_file(src_path)
                expected = info.get("sha256")
                if expected and _sha256(src) != expected:
                    raise ApiError("%s 的原厂固件 sha256 不匹配，拒绝打补丁" % device_id)
                patched, patch_log = mod.apply(src, key)
                if len(patched) != len(src):
                    raise ApiError("%s 补丁改变了固件长度（%d -> %d）" % (device_id, len(src), len(patched)))
                fname = info.get("out_name", "%s_key{key:04X}.bin" % device_id).format(key=key)
                fname = os.path.basename(fname)
                dst_path = os.path.join(build_dir, fname)
                common.write_file(dst_path, patched)

                extra_files = []
                verify_results = []
                warnings = []
                if want_container and info.get("container_builtin"):
                    # 该设备原厂固件本身就是 G3 V2.0 容器（如 X2T-C）：apply() 已就地
                    # 重算 MD5，再包一层会把容器 footer 当 payload 刷进 Flash，必须跳过。
                    warnings.append("固件本身即 G3 V2.0 容器，已内嵌容器信息（无需再打包容器）")
                elif want_container:
                    cname = info.get("container_name") or (
                        (fname[:-4] if fname.lower().endswith(".bin") else fname) + "_container.bin")
                    cname = os.path.basename(cname.format(key=key))
                    cdata = g3_container.build(patched, cname)
                    common.write_file(os.path.join(build_dir, cname), cdata)
                    extra_files.append(cname)

                if do_verify:
                    verifier = getattr(mod, "verify", None)
                    if verifier is None:
                        warnings.append("设备模块未提供 verify()，跳过离线验证")
                    else:
                        try:
                            verify_results = _normalize_verify(verifier(patched, key))
                        except Exception as exc:  # noqa: BLE001
                            verify_results = [{"ok": False, "msg": "验证执行失败: %s" % exc}]
                        if verify_results and not all(v["ok"] for v in verify_results):
                            raise ApiError("%s 离线验证失败，已中止导出" % device_id,
                                           device=device_id, verify=verify_results)
                    if not verify_results and not warnings:
                        warnings.append("未产生验证结果")

                files.append({
                    "device": device_id,
                    "name": info.get("name", device_id),
                    "filename": fname,
                    "size": len(patched),
                    "sha256": _sha256(patched),
                    "original_sha256": expected,
                    "patch_count": len(patch_log or []),
                    "patch_log": _normalize_patch_log(patch_log),
                    "verify_results": verify_results,
                    "warnings": warnings,
                    "extra_files": extra_files,
                    "download": "/api/builds/%s/%s" % (quote(build_id), quote(fname)),
                })
        except Exception:
            shutil.rmtree(build_dir, ignore_errors=True)
            raise

        created = datetime.now().astimezone().isoformat(timespec="seconds")
        manifest = {
            "id": build_id,
            "created": created,
            "key": _key_obj(key),
            "verify": do_verify,
            "container": want_container,
            "files": files,
            "zip": "/api/builds/%s/all.zip" % quote(build_id),
        }
        common.write_file(os.path.join(build_dir, "manifest.json"),
                          json.dumps(manifest, ensure_ascii=False, indent=2).encode("utf-8"))
        _build_zip(build_dir, build_id)
        manifest["zip"] = "/api/builds/%s/all.zip" % quote(build_id)
        return manifest


# --------------------------------------------------------------------------- #
# 历史与下载
# --------------------------------------------------------------------------- #
def api_builds(builds_dir: str, limit: int = 20) -> list[dict]:
    entries = []
    try:
        names = os.listdir(builds_dir)
    except OSError:
        return []
    for name in names:
        manifest_path = os.path.join(builds_dir, name, "manifest.json")
        if not os.path.isfile(manifest_path):
            continue
        try:
            with open(manifest_path, "r", encoding="utf-8") as fh:
                manifest = json.load(fh)
        except (OSError, ValueError):
            continue
        entries.append({
            "id": manifest.get("id", name),
            "created": manifest.get("created"),
            "key": (manifest.get("key") or {}).get("hex"),
            "verify": manifest.get("verify"),
            "container": manifest.get("container"),
            "files": [
                {"device": f.get("device"), "name": f.get("name"),
                 "filename": f.get("filename"), "size": f.get("size"),
                 "sha256": f.get("sha256"),
                 "download": f.get("download")}
                for f in manifest.get("files", [])
            ],
            "zip": manifest.get("zip"),
        })
        entries[-1]["_mtime"] = os.path.getmtime(manifest_path)
    entries.sort(key=lambda e: e.get("_mtime", 0), reverse=True)
    for entry in entries:
        entry.pop("_mtime", None)
    return entries[:limit]


_SAFE_ID = set("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_.-")


def _safe_build(builds_dir: str, build_id: str) -> str:
    if not build_id or any(ch not in _SAFE_ID for ch in build_id) or ".." in build_id:
        raise ApiError("非法的 build id", status=404)
    build_dir = os.path.join(builds_dir, build_id)
    if not os.path.isdir(build_dir):
        raise ApiError("build 不存在: %s" % build_id, status=404)
    return build_dir


def api_build_file(builds_dir: str, build_id: str, filename: str) -> tuple[str, str]:
    build_dir = _safe_build(builds_dir, build_id)
    filename = os.path.basename(filename)
    if filename in ("all.zip",):
        zip_path = os.path.join(build_dir, "all.zip")
        if not os.path.isfile(zip_path):
            _build_zip(build_dir, build_id)
        return zip_path, "%s.zip" % build_id
    full = os.path.join(build_dir, filename)
    if not os.path.isfile(full) or os.path.dirname(os.path.normpath(full)) != os.path.normpath(build_dir):
        raise ApiError("文件不存在", status=404)
    return full, filename


# --------------------------------------------------------------------------- #
# 固件上传
# --------------------------------------------------------------------------- #
def api_upload(device_id: str, body: bytes, filename: str = "") -> dict:
    if not device_id:
        raise ApiError("缺少 device 参数")
    if not body:
        raise ApiError("上传内容为空")
    if len(body) > MAX_UPLOAD:
        raise ApiError("文件过大（上限 %d MB）" % (MAX_UPLOAD // 1024 // 1024))
    try:
        mod = _load_device(device_id)
    except Exception:  # noqa: BLE001
        mod = None
    if mod is None:
        raise ApiError("未知设备: %s" % device_id)
    info = dict(getattr(mod, "DEVICE", {}) or {})
    expected = info.get("sha256")
    actual = _sha256(body)
    if expected and actual != expected:
        raise ApiError("固件 sha256 与原厂不匹配，已拒绝保存",
                       expected=expected, actual=actual, status=400)
    dst = _resolve(info.get("image", ""))
    if not dst:
        raise ApiError("设备模块未声明 image 路径")
    os.makedirs(os.path.dirname(dst), exist_ok=True)
    tmp = dst + ".uploading"
    try:
        with open(tmp, "wb") as fh:
            fh.write(body)
        # 原厂固件在磁盘上可能是只读属性（-ar---），Windows 下会导致 os.replace 失败
        if os.path.exists(dst):
            try:
                os.chmod(dst, stat.S_IWRITE)
            except OSError:
                pass
        os.replace(tmp, dst)
    except OSError as exc:
        try:
            os.remove(tmp)
        except OSError:
            pass
        raise ApiError("写入固件失败: %s" % exc, status=500)
    return {
        "ok": True,
        "device": device_id,
        "path": _rel(dst),
        "size": len(body),
        "sha256": actual,
        "matches": True,
        "filename": filename or os.path.basename(dst),
    }


def api_health() -> dict:
    try:
        count = len(_device_rows())
    except Exception:  # noqa: BLE001
        count = 0
    return {"ok": True, "version": VERSION, "python": sys.version.split()[0], "devices": count}


# --------------------------------------------------------------------------- #
# HTTP
# --------------------------------------------------------------------------- #
class Handler(BaseHTTPRequestHandler):
    server_version = "GodoxKeyStudio/" + VERSION
    protocol_version = "HTTP/1.1"

    # ---- 输出辅助 ----
    def log_message(self, fmt, *args):  # noqa: D401
        sys.stdout.write("[studio] %s\n" % (fmt % args))
        sys.stdout.flush()

    def _send_json(self, obj, status: int = 200):
        data = json.dumps(obj, ensure_ascii=False).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(data)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(data)

    def _send_error_json(self, exc: ApiError):
        payload = {"ok": False, "error": str(exc)}
        payload.update(exc.extra)
        self._send_json(payload, exc.status)

    def _send_bytes(self, data: bytes, ctype: str, status: int = 200,
                    filename: str | None = None):
        self.send_response(status)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(data)))
        self.send_header("Cache-Control", "no-store")
        if filename:
            from urllib.parse import quote as _quote
            self.send_header("Content-Disposition",
                             'attachment; filename="%s"; filename*=UTF-8\'\'%s'
                             % (filename, _quote(filename)))
        self.end_headers()
        self.wfile.write(data)

    def _send_file(self, path: str, filename: str | None = None):
        ctype, _ = mimetypes_guess(path)
        with open(path, "rb") as fh:
            data = fh.read()
        self._send_bytes(data, ctype, 200, filename)

    # ---- 请求体 ----
    def _read_body(self) -> bytes:
        try:
            length = int(self.headers.get("Content-Length") or 0)
        except ValueError:
            raise ApiError("Content-Length 非法")
        if length > MAX_BODY:
            raise ApiError("请求体过大", status=413)
        return self.rfile.read(length) if length else b""

    def _read_json(self) -> dict:
        raw = self._read_body()
        if not raw:
            return {}
        try:
            obj = json.loads(raw.decode("utf-8"))
        except (ValueError, UnicodeDecodeError):
            raise ApiError("请求体不是合法 JSON")
        if not isinstance(obj, dict):
            raise ApiError("请求体必须是 JSON 对象")
        return obj

    # ---- 路由 ----
    def do_GET(self):  # noqa: N802
        try:
            self._route_get()
        except ApiError as exc:
            self._send_error_json(exc)
        except (BrokenPipeError, ConnectionResetError):
            pass
        except Exception as exc:  # noqa: BLE001
            traceback.print_exc()
            self._send_json({"ok": False, "error": "服务器内部错误: %s" % exc}, 500)

    def do_POST(self):  # noqa: N802
        try:
            self._route_post()
        except ApiError as exc:
            self._send_error_json(exc)
        except (BrokenPipeError, ConnectionResetError):
            pass
        except Exception as exc:  # noqa: BLE001
            traceback.print_exc()
            self._send_json({"ok": False, "error": "服务器内部错误: %s" % exc}, 500)

    def do_HEAD(self):  # noqa: N802
        try:
            self._route_get(head=True)
        except Exception:  # noqa: BLE001
            pass

    def _route_get(self, head: bool = False):
        parsed = urlparse(self.path)
        path = unquote(parsed.path)
        query = parse_qs(parsed.query)
        builds_dir = self.server.builds_dir  # type: ignore[attr-defined]

        if path in ("/", "/index.html"):
            self._send_static("index.html")
            return
        if path == "/favicon.svg":
            self._send_static("favicon.svg", optional=True)
            return
        if path.startswith("/static/"):
            self._send_static(path[len("/static/"):])
            return
        if path == "/api/health":
            self._send_json(api_health())
            return
        if path == "/api/devices":
            self._send_json({
                "ok": True,
                "devices": _device_rows(),
                "key_rules": _key_rules(),
            })
            return
        if path == "/api/builds":
            self._send_json({"ok": True, "builds": api_builds(builds_dir)})
            return
        if path.startswith("/api/builds/"):
            rest = path[len("/api/builds/"):]
            if not rest:
                raise ApiError("缺少 build id", status=404)
            parts = rest.split("/")
            build_id = parts[0]
            if len(parts) == 1:
                build_dir = _safe_build(builds_dir, build_id)
                with open(os.path.join(build_dir, "manifest.json"), "r", encoding="utf-8") as fh:
                    self._send_json(json.load(fh))
                return
            filename = parts[1]
            full, fname = api_build_file(builds_dir, build_id, filename)
            ctype, _ = mimetypes_guess(full)
            with open(full, "rb") as fh:
                data = fh.read()
            self._send_bytes(data, ctype, 200, fname)
            return
        raise ApiError("未知接口: %s" % path, status=404)

    def _route_post(self):
        parsed = urlparse(self.path)
        path = unquote(parsed.path)
        query = parse_qs(parsed.query)
        builds_dir = self.server.builds_dir  # type: ignore[attr-defined]
        dev = (query.get("device") or [""])[0]

        if path == "/api/key/validate":
            body = self._read_json()
            key = _parse_key(body.get("text"), strict=True)
            self._send_json({"ok": True, "key": _key_obj(key)})
            return
        if path == "/api/key/random":
            self._read_body()
            key = common.random_key()
            self._send_json({"ok": True, "key": _key_obj(key)})
            return
        if path == "/api/key/derive":
            body = self._read_json()
            passphrase = (body.get("passphrase") or "").strip()
            if not passphrase:
                raise ApiError("口令不能为空")
            key = common.derive_key(passphrase)
            payload = {"ok": True, "key": _key_obj(key)}
            self._send_json(payload)
            return
        if path == "/api/firmware/upload":
            body = self._read_body()
            filename = self.headers.get("X-Filename", "")
            self._send_json(api_upload(dev, body, filename))
            return
        if path == "/api/generate":
            body = self._read_json()
            build = api_generate(body, builds_dir)
            self._send_json({"ok": True, "build": build})
            return
        raise ApiError("未知接口: %s" % path, status=404)

    def _send_static(self, relpath: str, optional: bool = False):
        relpath = relpath.lstrip("/")
        full = os.path.normpath(os.path.join(STATIC_DIR, relpath))
        if not full.startswith(os.path.normpath(STATIC_DIR) + os.sep) and full != os.path.normpath(STATIC_DIR):
            raise ApiError("非法路径", status=404)
        if not os.path.isfile(full):
            if optional:
                self._send_bytes(b"", "image/svg+xml", 204)
                return
            raise ApiError("资源不存在: %s" % relpath, status=404)
        ctype, _ = mimetypes_guess(full)
        with open(full, "rb") as fh:
            data = fh.read()
        self.send_response(200)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(data)))
        self.send_header("Cache-Control", "no-cache")
        self.end_headers()
        self.wfile.write(data)


def mimetypes_guess(path: str) -> tuple[str, str]:
    import mimetypes
    ctype, enc = mimetypes.guess_type(path)
    if ctype is None:
        ext = os.path.splitext(path)[1].lower()
        ctype = {
            ".bin": "application/octet-stream",
            ".zip": "application/zip",
            ".js": "text/javascript; charset=utf-8",
            ".css": "text/css; charset=utf-8",
            ".svg": "image/svg+xml",
            ".html": "text/html; charset=utf-8",
            ".json": "application/json; charset=utf-8",
            ".png": "image/png",
            ".ico": "image/x-icon",
        }.get(ext, "application/octet-stream")
    return ctype, enc or ""


def make_server(host: str = "127.0.0.1", port: int = 18780,
                builds_dir: str | None = None) -> ThreadingHTTPServer:
    builds_dir = os.path.abspath(builds_dir or DEFAULT_BUILDS_DIR)
    os.makedirs(builds_dir, exist_ok=True)
    httpd = ThreadingHTTPServer((host, port), Handler)
    httpd.daemon_threads = True
    httpd.builds_dir = builds_dir  # type: ignore[attr-defined]
    return httpd


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description="神牛密钥工坊本地服务")
    parser.add_argument("--host", default="127.0.0.1", help="监听地址（默认仅本机）")
    parser.add_argument("--port", type=int, default=18780, help="监听端口")
    parser.add_argument("--open", action="store_true", help="启动后自动打开浏览器")
    parser.add_argument("--builds-dir", default=DEFAULT_BUILDS_DIR, help="生成物目录")
    args = parser.parse_args(argv)

    try:
        httpd = make_server(args.host, args.port, args.builds_dir)
    except OSError as exc:
        print("[studio] 无法监听 %s:%s —— %s" % (args.host, args.port, exc), file=sys.stderr)
        print("[studio] 端口可能被占用，换一个：--port 18781", file=sys.stderr)
        return 1

    host, port = httpd.server_address[0], httpd.server_address[1]
    url = "http://%s:%d/" % (host, port)
    devices = _device_rows()
    ready = sum(1 for d in devices if d["firmware"]["matches"])
    print("=" * 62)
    print("  神牛密钥工坊 / Godox Key Studio  v%s" % VERSION)
    print("=" * 62)
    print("  地址     : %s" % url)
    print("  设备     : %d 台（原厂固件就绪 %d 台）" % (len(devices), ready))
    print("  生成目录 : %s" % httpd.builds_dir)
    print("  停止     : Ctrl+C")
    print("-" * 62)
    for row in devices:
        state = "已就位" if row["firmware"]["matches"] else (
            "缺失" if not row["firmware"]["present"] else "sha256 不匹配")
        print("  - %-6s %-24s %s" % (row["id"], row["name"], state))
    print("=" * 62)

    if args.open:
        threading.Timer(0.8, lambda: webbrowser.open(url)).start()

    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\n[studio] 正在停止 ...")
    finally:
        httpd.server_close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
