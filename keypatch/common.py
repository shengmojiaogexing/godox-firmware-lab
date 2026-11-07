# -*- coding: utf-8 -*-
"""keypatch 公共库：密钥校验/派生、固件读写、补丁日志。

私有密钥原理
------------
神牛 2.4G 链路使用 TI CC2500 兼容收发芯片。固件把 "ID" 编码成 16 位硬件同步字
(SYNC1/SYNC0)：

    sync = 50024 // (ID + 1)        # 原厂固件公式, ID = 0..99

因此原厂引闪器能发出的同步字只有 100 个公开取值。本工具把固件里的这条公式替换成
"同步字 = 固定 16 位私有密钥"，灯和引闪器都用同一把密钥，于是：

* 你的引闪器（已打补丁）发出的帧带私有同步字，你的灯（已打补丁）能收到并引闪；
* 其他人的任意原厂神牛引闪器只能发那 100 个公开同步字，灯不会解出同步字，
  硬件层直接丢弃 -> 无法引闪。

注意：这是 16 位硬件同步字级别的"密钥"，能挡住正常原厂设备，但不是密码学级别的
认证（用 SDR 抓包理论上可克隆）。不要把它当作防专业攻击手段。
"""
from __future__ import annotations

import hashlib
import os
import struct

# 原厂固件 ID 编码公式里的分子
ID_NUMERATOR = 50024
# 原厂 ID 允许范围 0..99 -> 100 个公开同步字
PUBLIC_IDS = range(0, 100)


def public_sync_values() -> set[int]:
    """原厂设备能够产生的全部同步字（100 个）。私有密钥必须避开这些值。"""
    return {ID_NUMERATOR // (i + 1) for i in PUBLIC_IDS}


# 一些额外要避开的"危险"同步字：
#   0x0000 / 0xFFFF：部分芯片复位值或特殊图案，容易造成误同步/无法同步
#   0xAAAA / 0x5555：前导码/训练图案，可能导致同步检测异常
EXTRA_FORBIDDEN = {0x0000, 0xFFFF, 0xAAAA, 0x5555, 0x8000}


class PatchError(RuntimeError):
    pass


def validate_key(key: int) -> int:
    """校验私有密钥（16 位同步字）。返回规范化后的 int，非法则抛 PatchError。"""
    if not isinstance(key, int):
        raise PatchError("key 必须是整数")
    if not (0 <= key <= 0xFFFF):
        raise PatchError("key 必须是 16 位值 (0x0000..0xFFFF)")
    if key in EXTRA_FORBIDDEN:
        raise PatchError("key 0x%04X 是保留/危险同步字（0x0000/0xFFFF/0xAAAA/0x5555/0x8000）" % key)
    if key in public_sync_values():
        raise PatchError(
            "key 0x%04X 与原厂 ID 同步字冲突（原厂设备能产生该值，会破坏隔离性）；"
            "请换一个密钥（可运行 keypatch.py keygen）" % key)
    return key


def derive_key(passphrase: str, salt: str = "godox-private-sync-v1") -> int:
    """由口令确定性地派生一个合规的 16 位密钥（SHA-256 前 2 字节，冲突则重试）。"""
    if not passphrase:
        raise PatchError("passphrase 不能为空")
    counter = 0
    while True:
        h = hashlib.sha256(("%s|%s|%d" % (salt, passphrase, counter)).encode("utf-8")).digest()
        key = struct.unpack(">H", h[:2])[0]
        try:
            return validate_key(key)
        except PatchError:
            counter += 1
            if counter > 10000:
                raise PatchError("无法派生合规密钥")


def random_key() -> int:
    """生成随机合规密钥（用 os.urandom，避开公开同步字）。"""
    while True:
        key = struct.unpack(">H", os.urandom(2))[0]
        try:
            return validate_key(key)
        except PatchError:
            continue


def sha256_hex(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def read_file(path: str) -> bytes:
    with open(path, "rb") as f:
        return f.read()


def write_file(path: str, data: bytes) -> None:
    os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
    with open(path, "wb") as f:
        f.write(data)


def key_from_text(text: str) -> int:
    """解析命令行里的密钥：0xABCD / ABCD / 十进制 / 口令（用 passphrase: 前缀或自动判定）。"""
    t = text.strip()
    try:
        if t.lower().startswith("0x"):
            return validate_key(int(t, 16))
        if all(c in "0123456789abcdefABCDEF" for c in t) and 1 <= len(t) <= 4:
            return validate_key(int(t, 16))
        return validate_key(int(t, 10))
    except ValueError:
        pass
    # 不是十六进制数字 -> 当作口令派生的种子
    return derive_key(t)


def find_original(path_candidates: list[str]) -> str:
    """在若干候选中找存在的原厂固件路径。"""
    for p in path_candidates:
        if os.path.isfile(p):
            return p
    raise PatchError("找不到原厂固件，尝试过: %r" % (path_candidates,))
