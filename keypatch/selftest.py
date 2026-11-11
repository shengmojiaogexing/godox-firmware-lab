# -*- coding: utf-8 -*-
"""keypatch 总自检：生成三份成品固件 + 全部离线验证 + Lead 独立交叉验证。

用法（在 <repo> 根目录下）::

    tools\\python\\python.exe keypatch\\selftest.py
    tools\\python\\python.exe keypatch\\selftest.py --key 6B21
    tools\\python\\python.exe keypatch\\selftest.py --out %TEMP%\\kp_test

检查项：
1. 每台设备：原厂 sha256 / 长度校验、apply 成功、verify() 全 PASS、输出长度不变；
2. 差异字节数报告（应与各设备补丁点一致，且不会新增长度）；
3. Lead 独立 Unicorn 交叉验证（灯：ID=0→KEY、ID=1/37/99→原厂公式）。引闪器的
   双发语义由 devices/x3c.py 的 verify() 覆盖。
"""
from __future__ import annotations

import argparse
import hashlib
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
if HERE not in sys.path:
    sys.path.insert(0, HERE)

import common  # noqa: E402
import devices as devs  # noqa: E402


def sha(b: bytes) -> str:
    return hashlib.sha256(b).hexdigest()


def resolve(p: str) -> str:
    return p if os.path.isabs(p) else os.path.join(ROOT, p)


def lead_cross_check(device_id: str, patched: bytes, key: int) -> list[tuple[bool, str]]:
    """Lead 的独立 Unicorn 调用级验证（不复用设备模块的 verify）。

    注意：不同设备/版本的"密钥开关"语义不同：
      * ad100 v4：读 RAM 0x20000486（C.Fn 预留槽），flag=1 -> KEY，flag=0 -> 原厂公式；
      * ad200 v2：灯 ID=0 -> KEY，ID>=1 -> 原厂公式；
      * x3c v2/v3：双发语义，由 devices/x3c.py 的 verify 覆盖，这里跳过。
  * x3pros：ID=99 双发语义，由 devices/x3pros.py 的 verify 覆盖，这里跳过。
    """
    sys.path.insert(0, os.path.join(HERE, "dev", "lead"))
    try:
        from call_harness import DEVICES, run
    except Exception as e:  # noqa: BLE001
        return [(False, "Lead harness 不可用: %s" % e)]
    if device_id not in DEVICES or device_id in ("x3c", "x3pros"):
        # X3-C / X3 Pro S 的密钥模式是双发语义，由设备模块 verify 覆盖；这里跳过
        return [(True, "（%s 的 Lead 交叉验证由设备模块 verify 覆盖）" % device_id)]

    out: list[tuple[bool, str]] = []
    # 灯的「密钥开关」语义：设备模块可用 KEY_FLAG_ADDR 声明自己的开关字节；
    # 未声明时按 v2 语义（ID==0 -> KEY）验证。
    flag_addrs = {"ad100": 0x20000486, "ad200": 0x2000041F}
    try:
        flag_addr = getattr(devs.load(device_id), "KEY_FLAG_ADDR", None)
    except Exception:  # noqa: BLE001
        flag_addr = None
    if isinstance(flag_addr, int) and 0x20000000 <= flag_addr < 0x20010000:
        flag_addrs[device_id] = flag_addr
        try:
            on_value = int(getattr(devs.load(device_id), "KEY_FLAG_ON_VALUE", 1))
        except Exception:  # noqa: BLE001
            on_value = 1
    else:
        on_value = 1
    if device_id in flag_addrs:
        flag_addr = flag_addrs[device_id]
        # flag=ON_VALUE 时任意 ID 都应写 KEY
        res = run(device_id, patched, ids=(0, 1, 37, 99), key=key,
                  pre_ram={flag_addr: bytes([on_value & 0xFF])})
        for ident, r in res.items():
            got = r[0] if isinstance(r[0], int) else -1
            out.append((got == key, "flag=0x%02X(ON): ID=%-2d sync=0x%04X 期望 0x%04X"
                        % (on_value, ident, got, key)))
        # flag=0 时回到原厂公式（含 ID=0 -> 50024）
        res = run(device_id, patched, ids=(0, 1, 37, 99), key=key,
                  pre_ram={flag_addr: b"\x00"})
        for ident, r in res.items():
            want = common.ID_NUMERATOR // (ident + 1)
            got = r[0] if isinstance(r[0], int) else -1
            out.append((got == want, "flag=0: ID=%-2d sync=0x%04X 期望 0x%04X"
                        % (ident, got, want)))
        return out

    res = run(device_id, patched, ids=(0, 1, 37, 99), key=key)
    for ident, r in res.items():
        want = key if ident == 0 else common.ID_NUMERATOR // (ident + 1)
        got = r[0] if isinstance(r[0], int) else -1
        out.append((got == want, "ID=%-2d sync=0x%04X 期望 0x%04X" % (ident, got, want)))
    return out


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description="keypatch 总自检")
    ap.add_argument("--key", default="6B21", help="测试用密钥（默认 6B21）")
    ap.add_argument("--out", default=None,
                    help="输出目录（默认临时目录，避免覆盖 keypatch/out 里的正式成品）")
    args = ap.parse_args(argv)

    key = common.key_from_text(args.key)
    if args.out:
        out_dir = resolve(args.out)
    else:
        import tempfile
        out_dir = tempfile.mkdtemp(prefix="keypatch_selftest_")
    os.makedirs(out_dir, exist_ok=True)

    print("=" * 80)
    print("keypatch 总自检   密钥 = 0x%04X  输出目录 = %s" % (key, out_dir))
    print("=" * 80)

    fail = 0
    for device_id in devs.DEVICE_IDS:
        try:
            mod = devs.load(device_id)
        except Exception as e:  # noqa: BLE001
            mod, err = None, str(e)
        else:
            err = None
        print("\n### %s" % device_id)
        if mod is None:
            print("  [SKIP] 模块未实现 %s" % (err or ""))
            continue

        info = mod.DEVICE
        src_path = resolve(info["image"])
        orig = common.read_file(src_path)
        if sha(orig) != info["sha256"]:
            print("  [FAIL] 原厂固件 sha256 不匹配")
            fail += 1
            continue
        patched, log = mod.apply(orig, key)
        out_name = info.get("out_name", "%s_key{key:04X}.bin" % device_id).format(key=key)
        dst = os.path.join(out_dir, out_name)
        common.write_file(dst, patched)

        checks: list[tuple[bool, str]] = [
            (len(patched) == len(orig),
             "长度不变 %d 字节" % len(patched)),
        ]
        diffs = [i for i in range(len(orig)) if orig[i] != patched[i]]
        checks.append((len(diffs) > 0, "差异字节 %d 个（补丁点）" % len(diffs)))
        checks.append((sha(patched) != sha(orig), "输出 sha256 与原厂不同"))

        ver = getattr(mod, "verify", None)
        if ver:
            try:
                checks += list(ver(patched, key))
            except Exception as e:  # noqa: BLE001
                checks.append((False, "verify() 抛异常: %s" % e))
        else:
            checks.append((False, "模块未提供 verify()"))

        checks += lead_cross_check(device_id, patched, key)

        bad = [c for c in checks if not c[0]]
        for ok, msg in checks:
            print("  [%s] %s" % ("PASS" if ok else "FAIL", msg))
        print("  输出: %s" % dst)
        print("  sha256: %s" % sha(patched))
        print("  补丁点:")
        for p in log:
            print("    0x%08X  %-20s -> %-20s %s" %
                  (p.get("addr", 0), p.get("before", "?"), p.get("after", "?"),
                   p.get("desc", "")))
        if bad:
            fail += 1
            print("  >>> %s 失败 %d 项" % (device_id, len(bad)))

    print("\n" + "=" * 80)
    if fail:
        print("总自检结论: FAIL（%d 台设备有问题）" % fail)
        return 1
    print("总自检结论: ALL PASS")
    print("提示：成品 bin 在 %s，用官方 G3 升级软件刷入；刷机步骤见 keypatch/README.md" % out_dir)
    return 0


if __name__ == "__main__":
    sys.exit(main())
