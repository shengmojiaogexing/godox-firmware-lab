# -*- coding: utf-8 -*-
"""G3 V2.0 固件容器打包 / 校验库（studio 与命令行共用）。

背景
----
官方 **G3 V2.0** 升级软件对"裸 app.bin"走 legacy 路径（长度不变的原位补丁直接能刷）。
但少数新一代"V2.0（A1）"设备在 `AA 55 A0` 探针后回 `AA 55 A1`（G3 内部标志 [+0x9a]），
G3 会改走容器解析路径；这时需要把裸镜像包成容器格式。

容器格式（已用官方 iT32_V1.18.bin 下载包核对）::

    payload                     裸 app 镜像（要真正刷进 Flash 的内容）
    MD5(payload)                16 字节 MD5
    filename                    L 字节，UTF-8 文件名（必须**包含**设备 ID 字符串，
                                如 "AD100Pro" / "AD200Pro" / "X3C"）
    L                           u16 小端（文件名字节数）
    AA 55 66 99 77 88 22 33     8 字节魔数（文件结尾）

G3 在 FUN_00407990 里解析：
  - 读末尾 10 字节 = L + magic；
  - 在 filesize-L-26 处取 16 字节 MD5，并对 [0, filesize-L-26) 重新做 MD5 比对；
  - 读 L 字节文件名并做 name.indexOf(device_id)；
  - payload 长度 = filesize-(L+26)，尾部 footer 不刷入。

本模块只把 `keypatch/dev/g3/make_container.py` 里验证过的打包/校验逻辑做成可复用库，
方便 `keypatch.py --container` 与 studio 直接 import（只依赖标准库）。

自测::

    tools\\python\\python.exe keypatch\\container.py
"""
from __future__ import annotations

import hashlib

# 8 字节魔数（G3 V2.0 容器结尾标识）
MAGIC = bytes.fromhex("aa55669977882233")
# 末尾固定开销：16 字节 MD5 + u16 L + 8 字节魔数
_FOOTER_MIN = 16 + 2 + len(MAGIC)


def build(raw: bytes, name: str) -> bytes:
    """把裸镜像 `raw` 打包成 G3 V2.0 容器。

    参数：
        raw:  裸 app 镜像字节（原位补丁后的固件）。
        name: 容器内嵌文件名（UTF-8）。**必须包含设备 ID 字符串**，否则 G3 会报
              "选择的固件与设备不匹配"；例如 "AD100Pro_V1.2_keyD798.bin"。
    返回：
        容器字节 = raw + MD5(raw) + name + u16(len(name)) + MAGIC。
    异常：
        raw 不是 bytes / name 为空或超过 0xFFFF 字节时抛 ValueError。
    """
    if not isinstance(raw, (bytes, bytearray)):
        raise ValueError("raw 必须是 bytes")
    raw = bytes(raw)
    if not name:
        raise ValueError("name 不能为空（且必须包含设备 ID 字符串）")
    n = name.encode("utf-8")
    if len(n) > 0xFFFF:
        raise ValueError("name 太长（%d 字节，u16 上限 65535）" % len(n))
    digest = hashlib.md5(raw).digest()
    return raw + digest + n + len(n).to_bytes(2, "little") + MAGIC


def verify(data: bytes) -> dict:
    """校验 G3 V2.0 容器，返回 dict（不抛异常）。

    返回字段：
        ok      (bool)  MD5 与魔数/长度结构是否都通过
        total   (int)   容器总字节数
        payload (int)   payload 字节数（= 会被刷入的内容长度）
        name    (str)   内嵌文件名（UTF-8 解码失败时用 replace 兜底）
        md5     (str)   容器内记录的 MD5（hex）
        error   (str|None) 结构不合法时的原因
    """
    if not isinstance(data, (bytes, bytearray)):
        return {"ok": False, "total": 0, "payload": 0, "name": "",
                "md5": "", "error": "data 必须是 bytes"}
    data = bytes(data)
    total = len(data)
    if total < _FOOTER_MIN + 1:      # 至少 1 字节 payload
        return {"ok": False, "total": total, "payload": 0, "name": "",
                "md5": "", "error": "文件太短，不是 G3 V2.0 容器"}
    if data[-len(MAGIC):] != MAGIC:
        return {"ok": False, "total": total, "payload": 0, "name": "",
                "md5": "", "error": "缺少 8 字节魔数 aa55669977882233（不是 V2.0 容器）"}
    name_len = int.from_bytes(data[-10:-8], "little")
    if name_len == 0 or name_len > total - _FOOTER_MIN:
        return {"ok": False, "total": total, "payload": 0, "name": "",
                "md5": "", "error": "内嵌文件名长度非法：L=%d" % name_len}
    name_bytes = data[-10 - name_len:-10]
    digest = data[-10 - name_len - 16:-10 - name_len]
    payload = data[:-10 - name_len - 16]
    ok = hashlib.md5(payload).digest() == digest
    return {
        "ok": ok,
        "total": total,
        "payload": len(payload),
        "name": name_bytes.decode("utf-8", "replace"),
        "md5": digest.hex(),
        "error": None if ok else "MD5 与 payload 不匹配（文件被截断/改动？）",
    }


def _selftest() -> int:
    """构造 dummy payload -> build -> verify，打印结果并返回退出码。"""
    payload = bytes(range(256)) * 4 + b"Godox container self-test payload"
    name = "AD100Pro_TEST_selftest.bin"
    blob = build(payload, name)
    info = verify(blob)
    print("container.py 自测")
    print("  payload : %d 字节" % len(payload))
    print("  container: %d 字节（+%d 字节 footer）" % (len(blob), len(blob) - len(payload)))
    print("  verify  : %r" % (info,))
    ok = (
        info["ok"]
        and info["total"] == len(blob)
        and info["payload"] == len(payload)
        and info["name"] == name
        and info["md5"] == hashlib.md5(payload).hexdigest()
    )
    # 反例：改坏一个 payload 字节应当校验失败
    broken = bytearray(blob)
    broken[0] ^= 0xFF
    bad = verify(bytes(broken))
    ok = ok and (bad["ok"] is False)
    print("  篡改检测: %s（期望 ok=False）" % ("PASS" if bad["ok"] is False else "FAIL"))
    print("  结论    : %s" % ("PASS" if ok else "FAIL"))
    return 0 if ok else 1


if __name__ == "__main__":
    import sys

    sys.exit(_selftest())
