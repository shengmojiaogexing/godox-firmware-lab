#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""以模块方式运行 sim.* 的入口（适配仓库根的可嵌入版 Python）。

仓库根的 `tools/python/python.exe` 是 Windows embeddable 发行版，它的
`python311._pth` 把 sys.path 固定为自身目录，**不会**自动加入当前工作目录，
所以直接 `python -m sim.server` 会报 `No module named 'sim'`。
本脚本把设备目录（本文件所在目录）加入 sys.path 后再按模块运行，例如：

    # 在 devices/ad200pro 目录下
    ..\\..\\tools\\python\\python.exe run_sim.py sim.server firmware\\AD200pro_V1.60.bin --port 8899
    ..\\..\\tools\\python\\python.exe run_sim.py sim.cli
    ..\\..\\tools\\python\\python.exe run_sim.py sim.boot_trace 4000000

如果你用的是系统 Python（非 embeddable），仍可直接 `python -m sim.server`。
"""
import os
import runpy
import sys

# Windows 控制台默认 cp936；本文件与 sim 输出含中文，尽量切到 UTF-8（失败不影响运行）
try:
    sys.stdout.reconfigure(encoding="utf-8")
    sys.stderr.reconfigure(encoding="utf-8")
except Exception:  # noqa: BLE001
    pass

ROOT = os.path.dirname(os.path.abspath(__file__))   # devices/ad200pro
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return 2
    module = sys.argv[1]
    sys.argv = [module] + sys.argv[2:]
    runpy.run_module(module, run_name="__main__")
    return 0


if __name__ == "__main__":
    sys.exit(main())
