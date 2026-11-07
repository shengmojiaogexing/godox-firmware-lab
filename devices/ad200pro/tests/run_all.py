#!/usr/bin/env python3
"""汇总运行 tests/ 下所有 test_*.py。

用法：
    python tests/run_all.py            # 逐个跑并汇总
    python tests/run_all.py test_boot  # 只跑匹配名字的

退出码：0 = 全部通过，1 = 有失败，2 = 一个测试都没找到。
"""
import os
import subprocess
import sys

try:
    sys.stdout.reconfigure(encoding="utf-8")
    sys.stderr.reconfigure(encoding="utf-8")
except Exception:  # noqa: BLE001
    pass

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
TIMEOUT = 600          # 单个测试上限（秒）


def discover(patterns):
    files = sorted(f for f in os.listdir(HERE)
                   if f.startswith("test_") and f.endswith(".py"))
    if patterns:
        files = [f for f in files if any(p in f for p in patterns)]
    return files


def main(argv=None):
    argv = list(sys.argv[1:] if argv is None else argv)
    files = discover(argv)
    if not files:
        print("没有找到测试文件（tests/test_*.py）")
        return 2
    print("运行 %d 个测试文件（python=%s）" % (len(files), sys.executable))
    print("=" * 70)
    results = []
    for name in files:
        path = os.path.join(HERE, name)
        print("\n>>> %s" % name)
        try:
            p = subprocess.run([sys.executable, path], cwd=ROOT,
                               capture_output=True, text=True,
                               encoding="utf-8", errors="replace",
                               timeout=TIMEOUT)
            out = (p.stdout or "") + (p.stderr or "")
            ok = p.returncode == 0
            results.append((name, ok, p.returncode, out))
        except subprocess.TimeoutExpired as e:
            out = ((e.stdout or b"").decode("utf-8", "replace")
                   if isinstance(e.stdout, bytes) else (e.stdout or ""))
            results.append((name, False, "timeout", out))
        except Exception as e:                       # noqa: BLE001
            results.append((name, False, "error", "%s: %s" % (type(e).__name__, e)))

    print("\n" + "=" * 70)
    print("汇总：")
    failed = 0
    for name, ok, code, out in results:
        print("  [%s] %-24s (exit=%s)" % ("PASS" if ok else "FAIL", name, code))
        if not ok:
            failed += 1
    for name, ok, code, out in results:
        if not ok:
            print("\n---- %s 输出（尾部 60 行）----" % name)
            tail = out.strip().splitlines()[-60:]
            print("\n".join(tail))
    print("\n%d/%d 通过" % (len(results) - failed, len(results)))
    return 0 if failed == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
