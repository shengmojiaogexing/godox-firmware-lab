#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""studio API 回归测试（stdlib unittest，不依赖 pytest）。

用法：
    tools\\python\\python.exe studio\\tests\\test_api.py -v

测试会：
  * 在随机端口起一个 studio 服务（生成目录用临时目录，不污染 studio/builds）；
  * 校验设备/密钥/生成/下载/上传/历史/静态资源；
  * 用 keypatch.py 命令行对同一把密钥再生成一次，逐字节比对 sha256（强一致性）。
"""
from __future__ import annotations

import io
import json
import os
import shutil
import subprocess
import sys
import tempfile
import threading
import unittest
import urllib.error
import urllib.request
import zipfile

HERE = os.path.dirname(os.path.abspath(__file__))
STUDIO = os.path.dirname(HERE)
REPO = os.path.dirname(STUDIO)
KEYPATCH = os.path.join(REPO, "keypatch")

for path in (STUDIO, KEYPATCH):
    if path not in sys.path:
        sys.path.insert(0, path)

import server as studio_server  # noqa: E402


def request(port, path, method="GET", data=None, headers=None, timeout=60):
    """返回 (status, body_bytes, headers)。"""
    url = "http://127.0.0.1:%d%s" % (port, path)
    req = urllib.request.Request(url, data=data, method=method)
    for key, value in (headers or {}).items():
        req.add_header(key, value)
    try:
        with urllib.request.urlopen(req, timeout=timeout) as resp:
            return resp.status, resp.read(), dict(resp.headers)
    except urllib.error.HTTPError as exc:
        return exc.code, exc.read(), dict(exc.headers)


def jrequest(port, path, method="GET", obj=None, data=None, headers=None, timeout=60):
    body = data
    hdrs = dict(headers or {})
    if obj is not None:
        body = json.dumps(obj).encode("utf-8")
        hdrs["Content-Type"] = "application/json"
    status, raw, hdrs_out = request(port, path, method, body, hdrs, timeout)
    try:
        parsed = json.loads(raw.decode("utf-8"))
    except (ValueError, UnicodeDecodeError):
        parsed = None
    return status, parsed, raw, hdrs_out


class StudioApiTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.builds_dir = tempfile.mkdtemp(prefix="studio-test-builds-")
        cls.httpd = studio_server.make_server("127.0.0.1", 0, cls.builds_dir)
        cls.port = cls.httpd.server_address[1]
        cls.thread = threading.Thread(target=cls.httpd.serve_forever, daemon=True)
        cls.thread.start()
        status, parsed, _, _ = jrequest(cls.port, "/api/devices")
        assert status == 200 and parsed and parsed.get("ok"), "设备接口不可用"
        cls.devices = {d["id"]: d for d in parsed["devices"]}
        cls.ready_ids = [d for d, row in cls.devices.items() if row["firmware"]["matches"]]

    @classmethod
    def tearDownClass(cls):
        cls.httpd.shutdown()
        cls.httpd.server_close()
        shutil.rmtree(cls.builds_dir, ignore_errors=True)

    # ---------------------------------------------------------------- health
    def test_health(self):
        status, parsed, _, _ = jrequest(self.port, "/api/health")
        self.assertEqual(status, 200)
        self.assertTrue(parsed["ok"])
        self.assertGreaterEqual(parsed["devices"], 1)
        self.assertTrue(parsed.get("version"))

    # --------------------------------------------------------------- devices
    def test_devices_registry(self):
        status, parsed, _, _ = jrequest(self.port, "/api/devices")
        self.assertEqual(status, 200)
        ids = {d["id"] for d in parsed["devices"]}
        self.assertIn("x3c", ids)
        self.assertIn("ad100", ids)
        self.assertIn("ad200", ids)
        for row in parsed["devices"]:
            for key in ("id", "name", "kind", "model", "firmware_version",
                        "out_name", "original_sha256", "firmware", "features"):
                self.assertIn(key, row)
            self.assertIn(row["kind"], ("flash", "trigger"))
            self.assertIn("present", row["firmware"])
            self.assertIn("matches", row["firmware"])
        rules = parsed["key_rules"]
        self.assertEqual(rules["width"], 4)
        self.assertEqual(len(rules["public_sync_values"]), 100)

    # ------------------------------------------------------------------- key
    def test_key_validate_ok(self):
        status, parsed, _, _ = jrequest(self.port, "/api/key/validate",
                                        "POST", {"text": "D798"})
        self.assertEqual(status, 200)
        self.assertEqual(parsed["key"]["hex"], "0xD798")
        self.assertEqual(parsed["key"]["sync1"], "0xD7")
        self.assertEqual(parsed["key"]["sync0"], "0x98")

    def test_key_validate_forbidden(self):
        status, parsed, _, _ = jrequest(self.port, "/api/key/validate",
                                        "POST", {"text": "FFFF"})
        self.assertEqual(status, 400)
        self.assertFalse(parsed["ok"])
        self.assertIn("error", parsed)
        self.assertIn("suggest", parsed)

    def test_key_validate_public_sync_conflict(self):
        # 取一个原厂同步字（如 ID=0 -> 50024 = 0xC368），必须被拒绝
        status, parsed, _, _ = jrequest(self.port, "/api/key/validate",
                                        "POST", {"text": "C368"})
        self.assertEqual(status, 400)
        self.assertFalse(parsed["ok"])

    def test_key_random_and_derive(self):
        status, parsed, _, _ = jrequest(self.port, "/api/key/random", "POST", {})
        self.assertEqual(status, 200)
        self.assertTrue(parsed["ok"])
        first = parsed["key"]["hex"]
        status, parsed2, _, _ = jrequest(self.port, "/api/key/derive",
                                        "POST", {"passphrase": "godox-test"})
        self.assertEqual(status, 200)
        status, parsed3, _, _ = jrequest(self.port, "/api/key/derive",
                                        "POST", {"passphrase": "godox-test"})
        self.assertEqual(parsed2["key"]["hex"], parsed3["key"]["hex"])
        self.assertEqual(len(first), 6)

    # -------------------------------------------------------------- generate
    def _require_ready(self):
        if not self.ready_ids:
            self.skipTest("原厂固件未就位，跳过生成相关测试")

    def test_generate_all_no_verify(self):
        self._require_ready()
        status, parsed, _, _ = jrequest(self.port, "/api/generate", "POST",
                                        {"key": "D798", "devices": self.ready_ids,
                                         "verify": False, "container": False},
                                        timeout=120)
        self.assertEqual(status, 200, parsed)
        build = parsed["build"]
        self.assertEqual(build["key"]["hex"], "0xD798")
        self.assertEqual(len(build["files"]), len(self.ready_ids))
        by_dev = {f["device"]: f for f in build["files"]}
        for device_id in self.ready_ids:
            info = self.devices[device_id]
            entry = by_dev[device_id]
            self.assertEqual(entry["size"], info["firmware"]["size"])
            self.assertNotEqual(entry["sha256"], info["original_sha256"])
            self.assertTrue(entry["filename"].startswith(info["out_name"].split("{")[0]))
            self.assertGreaterEqual(entry["patch_count"], 1)
            self.assertTrue(entry["patch_log"])
            # 单文件下载内容与 sha256 一致
            status, raw, hdrs = request(self.port, entry["download"])
            self.assertEqual(status, 200)
            self.assertEqual(studio_server._sha256(raw), entry["sha256"])
            self.assertIn("attachment", hdrs.get("Content-Disposition", ""))
        # zip 打包
        status, raw, _ = request(self.port, build["zip"])
        self.assertEqual(status, 200)
        with zipfile.ZipFile(io.BytesIO(raw)) as zf:
            names = set(zf.namelist())
            for entry in build["files"]:
                self.assertIn(entry["filename"], names)
        # 历史记录
        status, parsed_hist, _, _ = jrequest(self.port, "/api/builds")
        self.assertEqual(status, 200)
        self.assertIn(build["id"], [b["id"] for b in parsed_hist["builds"]])
        # manifest
        status, parsed_manifest, _, _ = jrequest(self.port, "/api/builds/" + build["id"])
        self.assertEqual(status, 200)
        self.assertEqual(parsed_manifest["id"], build["id"])
        self.assertEqual(len(parsed_manifest["files"]), len(self.ready_ids))

    def test_generate_matches_cli(self):
        """同一把密钥：studio 生成的固件必须与 keypatch.py 命令行逐字节一致。"""
        self._require_ready()
        status, parsed, _, _ = jrequest(self.port, "/api/generate", "POST",
                                        {"key": "0x6B21", "devices": self.ready_ids,
                                         "verify": False}, timeout=120)
        self.assertEqual(status, 200, parsed)
        api_files = {f["device"]: f for f in parsed["build"]["files"]}
        tmp = tempfile.mkdtemp(prefix="keypatch-cli-")
        try:
            proc = subprocess.run(
                [sys.executable, os.path.join(KEYPATCH, "keypatch.py"),
                 "patch-all", "--key", "0x6B21", "--out", tmp],
                cwd=REPO, capture_output=True, text=True, timeout=180)
            self.assertEqual(proc.returncode, 0, proc.stdout + proc.stderr)
            for device_id in self.ready_ids:
                fname = api_files[device_id]["filename"]
                cli_path = os.path.join(tmp, fname)
                self.assertTrue(os.path.isfile(cli_path), "CLI 未生成 %s" % fname)
                with open(cli_path, "rb") as fh:
                    cli_bytes = fh.read()
                self.assertEqual(studio_server._sha256(cli_bytes),
                                 api_files[device_id]["sha256"],
                                 "%s 与 CLI 输出不一致" % device_id)
        finally:
            shutil.rmtree(tmp, ignore_errors=True)

    def test_generate_with_verify_x3c(self):
        if "x3c" not in self.ready_ids:
            self.skipTest("X3-C 原厂固件未就位")
        status, parsed, _, _ = jrequest(self.port, "/api/generate", "POST",
                                        {"key": "0x5A3C", "devices": ["x3c"],
                                         "verify": True}, timeout=180)
        self.assertEqual(status, 200, parsed)
        entry = parsed["build"]["files"][0]
        self.assertTrue(entry["verify_results"], "应有验证结果")
        for item in entry["verify_results"]:
            self.assertTrue(item["ok"], item["msg"])

    def test_generate_with_container(self):
        self._require_ready()
        status, parsed, _, _ = jrequest(self.port, "/api/generate", "POST",
                                        {"key": "0x1234", "devices": self.ready_ids[:1],
                                         "verify": False, "container": True},
                                        timeout=120)
        if status == 400 and "容器" in (parsed or {}).get("error", ""):
            self.skipTest("容器模块未就绪")
        self.assertEqual(status, 200, parsed)
        entry = parsed["build"]["files"][0]
        self.assertTrue(entry["extra_files"], "应生成容器备用件")
        status, raw, _ = request(self.port, "/api/builds/%s/%s"
                                 % (parsed["build"]["id"], entry["extra_files"][0]))
        self.assertEqual(status, 200)
        self.assertTrue(raw.endswith(bytes.fromhex("aa55669977882233")))

    def test_generate_rejects_bad_input(self):
        self._require_ready()
        status, parsed, _, _ = jrequest(self.port, "/api/generate", "POST",
                                        {"key": "FFFF", "devices": ["x3c"]})
        self.assertEqual(status, 400)
        status, parsed, _, _ = jrequest(self.port, "/api/generate", "POST",
                                        {"key": "D798", "devices": []})
        self.assertEqual(status, 400)
        status, parsed, _, _ = jrequest(self.port, "/api/generate", "POST",
                                        {"key": "D798", "devices": ["no-such-device"]})
        self.assertEqual(status, 400)
        self.assertIn("未知设备", parsed["error"])

    # ---------------------------------------------------------------- upload
    def test_upload_roundtrip(self):
        self._require_ready()
        for device_id in self.ready_ids:
            info = self.devices[device_id]
            src = os.path.join(REPO, info["firmware"]["path"].replace("/", os.sep))
            with open(src, "rb") as fh:
                original = fh.read()
            status, parsed, _, _ = jrequest(
                self.port, "/api/firmware/upload?device=" + device_id, "POST",
                data=original,
                headers={"Content-Type": "application/octet-stream",
                         "X-Filename": os.path.basename(src)},
                timeout=60)
            self.assertEqual(status, 200, parsed)
            self.assertTrue(parsed["matches"])
            self.assertEqual(parsed["sha256"], info["firmware"]["sha256"])
            # 错误内容必须被拒绝
            status, parsed_bad, _, _ = jrequest(
                self.port, "/api/firmware/upload?device=" + device_id, "POST",
                data=b"not-a-real-firmware", headers={"Content-Type": "application/octet-stream"})
            self.assertEqual(status, 400)
            self.assertIn("sha256", parsed_bad["error"])
            self.assertIn("expected", parsed_bad)

    # --------------------------------------------------------------- static
    def test_static_index(self):
        status, raw, _ = request(self.port, "/")
        self.assertEqual(status, 200)
        text = raw.decode("utf-8", "replace")
        self.assertIn("神牛密钥工坊", text)
        self.assertIn("app.js", text)
        status, raw, _ = request(self.port, "/favicon.svg")
        self.assertIn(status, (200, 204))

    def test_path_traversal_blocked(self):
        status, _, _ = request(self.port, "/static/..%2fserver.py")
        self.assertEqual(status, 404)
        status, _, _ = request(self.port, "/api/builds/..%2f..%2fetc/all.zip")
        self.assertEqual(status, 404)

    def test_unknown_api(self):
        status, parsed, _, _ = jrequest(self.port, "/api/nope")
        self.assertEqual(status, 404)
        self.assertFalse(parsed["ok"])


if __name__ == "__main__":
    unittest.main(verbosity=2)
