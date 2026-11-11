# -*- coding: utf-8 -*-
"""keypatch.py —— 神牛固件"私有密钥(16 位同步字)"补丁工具。

用法示例（在 <repo> 根目录下运行）::

    # 列出支持的设备 / 原厂固件是否就位
    tools\\python\\python.exe keypatch\\keypatch.py list

    # 生成一把随机密钥（或由口令派生）
    tools\\python\\python.exe keypatch\\keypatch.py keygen
    tools\\python\\python.exe keypatch\\keypatch.py keygen --passphrase "我的口令"

    # 给单个设备打补丁并做离线验证
    tools\\python\\python.exe keypatch\\keypatch.py patch --device ad100 --key 6B21 --verify

    # 三台设备打同一把密钥（灯 + 引闪器必须一致！）
    tools\\python\\python.exe keypatch\\keypatch.py patch-all --key 6B21 --verify

    # 额外生成 G3 V2.0 容器备用件（文件名在 out_name 后加 _container：
    #   AD100pro_V1.2_key6B21_container.bin 等；只有 V2.0/A1 设备才需要）
    tools\\python\\python.exe keypatch\\keypatch.py patch-all --key 6B21 --container

    # 校验一个已经打好的 bin
    tools\\python\\python.exe keypatch\\keypatch.py verify --device ad200 ^
        --image keypatch\\out\\AD200pro_V1.60_key6B21.bin --key 6B21

    # 查看补丁清单
    tools\\python\\python.exe keypatch\\keypatch.py info --device x3c

打补丁后的 bin 与原厂 bin 长度完全一致（原位补丁），可直接用官方 G3 升级软件刷入。
原厂固件不会被修改。
"""
from __future__ import annotations

import argparse
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
if HERE not in sys.path:
    sys.path.insert(0, HERE)

import common  # noqa: E402
import container as container_lib  # noqa: E402
import devices as devs  # noqa: E402


def _rel(p: str) -> str:
    try:
        return os.path.relpath(p, ROOT)
    except ValueError:
        return p


def _resolve(p: str) -> str:
    return p if os.path.isabs(p) else os.path.join(ROOT, p)


def _load(device_id: str):
    mod = devs.load(device_id)
    if mod is None:
        raise common.PatchError("设备 %r 的补丁模块还没实现（keypatch/devices/%s.py 不存在）"
                                % (device_id, device_id))
    return mod


def _read_original(mod) -> bytes:
    path = _resolve(mod.DEVICE["image"])
    if not os.path.isfile(path):
        raise common.PatchError("找不到原厂固件: %s" % path)
    data = common.read_file(path)
    want = mod.DEVICE.get("sha256")
    got = common.sha256_hex(data)
    if want and got != want:
        raise common.PatchError("原厂固件 sha256 不匹配，拒绝打补丁\n  期望 %s\n  实际 %s" % (want, got))
    return data


def _print_patch_log(log: list[dict]) -> None:
    for i, p in enumerate(log, 1):
        addr = p.get("addr")
        print("  [%d] %s  0x%08X  %-22s -> %-22s  %s" % (
            i, "off+0x%05X" % p["offset"] if "offset" in p else "",
            addr if isinstance(addr, int) else 0,
            p.get("before", ""), p.get("after", ""), p.get("desc", "")))


def cmd_list(args) -> int:
    print("已实现补丁的设备：")
    any_dev = False
    for d in devs.DEVICE_IDS:
        try:
            mod = devs.load(d)
        except Exception as e:  # noqa: BLE001
            print("  %-6s  加载失败: %s" % (d, e)); continue
        if mod is None:
            print("  %-6s  (未实现)" % d); continue
        any_dev = True
        info = mod.DEVICE
        path = _resolve(info["image"])
        ok = os.path.isfile(path)
        sha_ok = ""
        if ok:
            got = common.sha256_hex(common.read_file(path))
            sha_ok = " sha256 OK" if got == info.get("sha256") else " sha256 MISMATCH!"
        print("  %-6s  %-24s  %s%s" % (d, info.get("name", "?"), _rel(path), sha_ok))
    if not any_dev:
        print("  （无）")
    return 0


def cmd_keygen(args) -> int:
    if args.passphrase:
        key = common.derive_key(args.passphrase)
        print("口令         : %s" % args.passphrase)
    else:
        key = common.random_key()
        print("（随机生成，请自行保存）")
    print("私有密钥     : 0x%04X  (%d)" % (key, key))
    print("十进制       : %d" % key)
    print("十六进制字节 : SYNC1=0x%02X SYNC0=0x%02X" % (key >> 8, key & 0xFF))
    print("提示：灯和引闪器必须使用同一把密钥；换密钥需重新给所有设备打补丁并刷机。")
    return 0


def _container_name(out_name: str) -> str:
    """在 out_name 基础上加 `_container` 后缀（保留扩展名）。

    AD100pro_V1.2_keyD798.bin -> AD100pro_V1.2_keyD798_container.bin
    """
    stem, ext = os.path.splitext(out_name)
    return "%s_container%s" % (stem, ext or ".bin")


def _patch_one(device_id: str, key_text: str, out_dir: str,
               do_verify: bool, quiet: bool = False,
               do_container: bool = False) -> tuple[bool, str]:
    mod = _load(device_id)
    info = mod.DEVICE
    key = common.key_from_text(key_text)
    src = _read_original(mod)
    patched, log = mod.apply(src, key)
    if len(patched) != len(src):
        raise common.PatchError("补丁改变了固件长度（%d -> %d），G3 升级可能拒绝"
                                % (len(src), len(patched)))
    name = info.get("out_name", "%s_key{key:04X}.bin" % device_id).format(key=key)
    dst = os.path.join(out_dir, name)
    common.write_file(dst, patched)
    if not quiet:
        print("\n== %s (%s)" % (info.get("name", device_id), device_id))
        print("   原厂固件 : %s  (%d 字节, sha256 %s)" % (_rel(_resolve(info["image"])),
                                                       len(src), common.sha256_hex(src)[:16] + "…"))
        print("   私有密钥 : 0x%04X  ->  SYNC1=0x%02X SYNC0=0x%02X"
              % (key, key >> 8, key & 0xFF))
        print("   补丁点   :")
        _print_patch_log(log)
        print("   输出     : %s  (%d 字节, sha256 %s)"
              % (_rel(dst), len(patched), common.sha256_hex(patched)[:16] + "…"))
    if do_container:
        if info.get("container_builtin"):
            # 该设备的原厂文件本身就是 G3 V2.0 容器（如 X2T-C）：apply() 已就地重算 MD5，
            # 再包一层会把容器 footer 当 payload 刷进 Flash，所以明确跳过。
            if not quiet:
                print("   容器备用 : 跳过（%s 的固件本身已是 G3 V2.0 容器，"
                      "apply() 已重算 MD5，无需二次打包）" % device_id)
        else:
            # G3 V2.0 容器备用件（仅少数 V2.0/A1 设备需要；内嵌文件名必须含设备 ID，
            # 如 AD100Pro / AD200Pro / X3C，由设备模块的 container_name 模板给出）
            cname = info.get("container_name") or _container_name(name)
            cname = cname.format(key=key)
            cdst = os.path.join(out_dir, cname)
            blob = container_lib.build(patched, cname)
            common.write_file(cdst, blob)
            if not quiet:
                print("   容器备用 : %s  (%d 字节, payload %d + footer %d)"
                      % (_rel(cdst), len(blob), len(patched), len(blob) - len(patched)))
    if do_verify:
        ok_all = True
        ver = getattr(mod, "verify", None)
        if ver is None:
            # 退化：静态检查补丁后字节
            results = []
            for p in log:
                off = p["offset"]; want = p.get("after_bytes")
                if want is not None:
                    results.append((patched[off:off + len(want)] == want,
                                    "offset 0x%05X = %s" % (off, want.hex(" "))))
            ok_all = all(r[0] for r in results) if results else False
            results = results or [(False, "模块未提供 verify()，也没有可静态检查的 after_bytes")]
        else:
            results = ver(patched, key)
            ok_all = all(r[0] for r in (results or []))
        for ok, msg in results:
            print("   [%s] %s" % ("PASS" if ok else "FAIL", msg))
        if not ok_all:
            raise common.PatchError("%s 离线验证失败" % device_id)
    return True, dst


def cmd_patch(args) -> int:
    out_dir = _resolve(args.out)
    os.makedirs(out_dir, exist_ok=True)
    _patch_one(args.device, args.key, out_dir, args.verify,
               do_container=args.container)
    return 0


def cmd_patch_all(args) -> int:
    out_dir = _resolve(args.out)
    os.makedirs(out_dir, exist_ok=True)
    key = common.key_from_text(args.key)
    print("=== 给全部已实现设备打同一把密钥 0x%04X ===" % key)
    made = []
    for d in devs.available():
        ok, dst = _patch_one(d, "0x%04X" % key, out_dir, args.verify, quiet=False,
                             do_container=args.container)
        made.append(dst)
    print("\n全部完成（%d 个）:" % len(made))
    for m in made:
        print("  " + _rel(m))
    print("\n注意：三份固件必须使用同一把密钥；刷机时灯和引闪器都要刷。")
    return 0


def cmd_verify(args) -> int:
    mod = _load(args.device)
    key = common.key_from_text(args.key)
    path = _resolve(args.image)
    data = common.read_file(path)
    orig = _read_original(mod)
    print("既有固件 : %s" % _rel(path))
    print("密钥     : 0x%04X" % key)
    # 1) 与原厂比较：差异必须只出现在补丁点内
    diffs = [i for i in range(min(len(data), len(orig))) if data[i] != orig[i]]
    print("差异字节 : %d 个, offset=%s" % (len(diffs), [hex(x) for x in diffs[:16]]))
    ver = getattr(mod, "verify", None)
    if ver is None:
        print("（模块未提供 verify()）")
        return 1
    results = ver(data, key)
    ok_all = all(r[0] for r in (results or []))
    for ok, msg in results:
        print("  [%s] %s" % ("PASS" if ok else "FAIL", msg))
    print("\n结论:", "PASS" if ok_all else "FAIL")
    return 0 if ok_all else 1


def cmd_info(args) -> int:
    mod = _load(args.device)
    info = dict(mod.DEVICE)
    print("设备     : %s (%s)" % (info.get("name"), info.get("id")))
    print("原厂固件 : %s" % info.get("image"))
    print("基址     : 0x%08X" % info.get("base", 0))
    print("sha256   : %s" % info.get("sha256"))
    if info.get("notes"):
        print("说明     : %s" % info["notes"])
    return 0


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(
        description="神牛固件私有密钥(CC2500 16 位同步字)补丁工具",
        formatter_class=argparse.RawDescriptionHelpFormatter, epilog=__doc__)
    sub = ap.add_subparsers(dest="cmd", required=True)

    p = sub.add_parser("list", help="列出支持的设备与原厂固件状态")
    p.set_defaults(fn=cmd_list)

    p = sub.add_parser("keygen", help="生成/派生私有密钥")
    p.add_argument("--passphrase", help="由口令派生（同一口令每次得到同一密钥）")
    p.set_defaults(fn=cmd_keygen)

    p = sub.add_parser("patch", help="给单个设备打补丁")
    p.add_argument("--device", required=True)
    p.add_argument("--key", required=True, help="16 位密钥：6B21 / 0x6B21 / 十进制 / 口令")
    p.add_argument("--out", default="keypatch/out")
    p.add_argument("--verify", action="store_true", help="打完后做离线验证")
    p.add_argument("--container", action="store_true",
                   help="额外生成 G3 V2.0 容器备用件（*_container.bin，仅 V2.0/A1 设备需要）")
    p.set_defaults(fn=cmd_patch)

    p = sub.add_parser("patch-all", help="给全部设备打同一把密钥")
    p.add_argument("--key", required=True)
    p.add_argument("--out", default="keypatch/out")
    p.add_argument("--verify", action="store_true")
    p.add_argument("--container", action="store_true",
                   help="额外生成 G3 V2.0 容器备用件（*_container.bin）")
    p.set_defaults(fn=cmd_patch_all)

    p = sub.add_parser("verify", help="验证一个已打好的 bin")
    p.add_argument("--device", required=True)
    p.add_argument("--image", required=True)
    p.add_argument("--key", required=True)
    p.set_defaults(fn=cmd_verify)

    p = sub.add_parser("info", help="显示设备信息")
    p.add_argument("--device", required=True)
    p.set_defaults(fn=cmd_info)

    args = ap.parse_args(argv)
    try:
        return args.fn(args)
    except common.PatchError as e:
        print("错误: %s" % e, file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
