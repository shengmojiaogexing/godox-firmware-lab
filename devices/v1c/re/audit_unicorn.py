#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""V1C_V2.0.bin 补丁点 —— Unicorn 调用级独立验证（fw-auditor）。

做法：
  * 按 Lead 设计「ID==0 -> KEY；ID=1..99 -> 50024//(ID+1)；写 SYNC1/SYNC0 后
    delay(20us)，去掉原厂 SCAL(0x33)+delay(1000)」手工构造 56 字节补丁
    （复用 audit_static.build_patch_blob 的同一份编码器）。
  * 在 Unicorn 里执行 0x080121BC|1，r0=ID：
      - WriteReg(0x080156CE)/strobe(0x08015790)/delay(0x08015440) 入口写成 `bx lr`
        返回桩，用 code hook 记录调用参数；
      - __aeabi_uidiv(0x080030F0) 保留真实代码执行（自包含除法循环），
        确保 patch 的 50024//(ID+1) 是真算出来的，而不是桩返回。
  * 原厂镜像自证：5 个 ID 都必须得到 50024//(ID+1)，且调用序列含
    delay(20) -> strobe(0x33) -> delay(1000)。
  * 补丁镜像：KEY=0xD798 / 0xA5C3 各测一次；ID=0 -> KEY，
    ID=1/5/37/99 -> 50024//(ID+1)，调用序列恰为
    [WriteReg(4,hi), WriteReg(5,lo), delay(20)]（无 SCAL、无 1ms）。

用法:
    tools\\python\\python.exe devices\\v1c\\re\\audit_unicorn.py
退出码: 0 = 全部 PASS；1 = 存在 FAIL。
"""
from __future__ import annotations

import hashlib
import os
import re
import struct
import sys

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB

sys.dont_write_bytecode = True        # 只读审计：不生成 __pycache__
_HERE = os.path.dirname(os.path.abspath(__file__))
if _HERE not in sys.path:
    sys.path.insert(0, _HERE)

import audit_static as st  # noqa: E402  同一份固件常量/补丁编码器

try:
    import unicorn
    from unicorn import (Uc, UC_ARCH_ARM, UC_MODE_MCLASS, UC_HOOK_CODE,
                         UC_PROT_ALL, UcError)
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3, UC_ARM_REG_SP, UC_ARM_REG_LR,
                                   UC_ARM_REG_PC)
except Exception as exc:  # noqa: BLE001
    print("FATAL: 无法导入 unicorn：%r" % (exc,))
    print("（本仓库 Python 位于 tools\\python\\python.exe，已装 unicorn 2.1.4）")
    sys.exit(2)

# ---- 测试参数 --------------------------------------------------------------
IDS = (0, 1, 5, 37, 99)
KEY_VALUES = (0xD798, 0xA5C3)

FLASH_BASE = 0x08000000
FLASH_SIZE = 0x20000
RAM_BASE = 0x20000000
RAM_SIZE = 0x4000
STACK_TOP = 0x20001800
SENTINEL = 0x20000400          # 返回落点：放 `b .`
MAX_INSNS = 20000

B_X_LR = bytes.fromhex("7047")  # bx lr
B_DOT = bytes.fromhex("fee7")   # b .

RESULTS: list[tuple[bool, str]] = []


def check(cond: bool, msg: str) -> bool:
    RESULTS.append((bool(cond), msg))
    print("[%s] %s" % ("PASS" if cond else "FAIL", msg))
    return bool(cond)


def header(title: str) -> None:
    print()
    print("=" * 78)
    print(title)
    print("=" * 78)


def fmt_calls(calls) -> str:
    parts = []
    for c in calls:
        if c[0] == "wreg":
            parts.append("WriteReg(%d,0x%02X)" % (c[1], c[2]))
        elif c[0] == "cmd":
            parts.append("strobe(0x%02X)" % (c[1] or 0))
        else:
            parts.append("delay(%d)" % (c[1] or 0))
    return " -> ".join(parts) if parts else "(无)"


def run_sync_fn(image: bytes, ident: int) -> dict:
    """在 Unicorn 中执行 image 的同步字函数 0x080121BC，r0=ID。

    返回 {"calls": [...], "error": str|None, "returned": bool, "insn": int}
    calls 元素: ("wreg", reg, val) / ("cmd", cmd, None) / ("delay", n, None)
    """
    if len(image) > FLASH_SIZE - (st.BASE - FLASH_BASE):
        raise ValueError("镜像过大")

    uc = Uc(UC_ARCH_ARM, UC_MODE_MCLASS)
    uc.mem_map(FLASH_BASE, FLASH_SIZE, UC_PROT_ALL)
    uc.mem_map(RAM_BASE, RAM_SIZE, UC_PROT_ALL)
    uc.mem_write(st.BASE, image)

    # 返回落点 + 三个辅助函数入口桩（uidiv 保留真实代码！）
    uc.mem_write(SENTINEL, B_DOT)
    for addr in (st.WRITE_REG, st.STROBE, st.DELAY):
        uc.mem_write(addr, B_X_LR)

    calls: list[tuple] = []
    state = {"error": None, "insn": 0, "returned": False}

    def hook_code(_uc, address, _size, _user):
        state["insn"] += 1
        if address == SENTINEL:
            state["returned"] = True
            _uc.emu_stop()
            return
        if address == st.WRITE_REG:
            calls.append(("wreg", _uc.reg_read(UC_ARM_REG_R0),
                          _uc.reg_read(UC_ARM_REG_R1)))
        elif address == st.STROBE:
            calls.append(("cmd", _uc.reg_read(UC_ARM_REG_R0), None))
        elif address == st.DELAY:
            calls.append(("delay", _uc.reg_read(UC_ARM_REG_R0), None))

    uc.hook_add(UC_HOOK_CODE, hook_code)
    uc.reg_write(UC_ARM_REG_SP, STACK_TOP)
    uc.reg_write(UC_ARM_REG_LR, SENTINEL | 1)   # Thumb 返回落点
    uc.reg_write(UC_ARM_REG_R0, ident & 0xFF)
    for reg, val in ((UC_ARM_REG_R1, 0x1234), (UC_ARM_REG_R2, 0x5678),
                     (UC_ARM_REG_R3, 0x9ABC)):
        uc.reg_write(reg, val)

    try:
        uc.emu_start(st.SYNC_FN | 1, 0, 0, MAX_INSNS)
    except UcError as exc:  # noqa: BLE001
        state["error"] = "%s (pc=0x%08X, lr=0x%08X, sp=0x%08X)" % (
            exc, uc.reg_read(UC_ARM_REG_PC), uc.reg_read(UC_ARM_REG_LR),
            uc.reg_read(UC_ARM_REG_SP))
    if state["error"] is None and not state["returned"]:
        state["error"] = ("未在 %d 条指令内返回（执行 %d 条，pc=0x%08X），疑似跑飞"
                          % (MAX_INSNS, state["insn"], uc.reg_read(UC_ARM_REG_PC)))
    return {"calls": calls, "error": state["error"],
            "returned": state["returned"], "insn": state["insn"]}


def sync_from_calls(calls) -> int | None:
    """从调用序列前两条 WriteReg(4, hi)/WriteReg(5, lo) 还原 16 位同步字。"""
    if len(calls) < 2 or calls[0][0] != "wreg" or calls[1][0] != "wreg":
        return None
    if calls[0][1] != st.REG_SYNC1 or calls[1][1] != st.REG_SYNC0:
        return None
    return ((calls[0][2] & 0xFF) << 8) | (calls[1][2] & 0xFF)


def _bl_target(ins) -> int | None:
    """从 capstone 操作数 '#0x...' 解析 BL 目标（偶数地址）。"""
    m = re.search(r"#(0x[0-9a-fA-F]+)", ins.op_str)
    return int(m.group(1), 16) & ~1 if m else None


# ---------------------------------------------------------------------------
# 原厂镜像自证
# ---------------------------------------------------------------------------
def check_original(image: bytes) -> None:
    header("1. 原厂镜像自证（同一 harness 必须还原原厂行为 + SCAL/1ms）")
    for ident in IDS:
        res = run_sync_fn(image, ident)
        if res["error"]:
            check(False, "ID=%-2d 原厂 Unicorn 执行失败: %s" % (ident, res["error"]))
            continue
        calls = res["calls"]
        expect = st.ID_NUMERATOR // (ident + 1)
        hi, lo = expect >> 8, expect & 0xFF
        want = [("wreg", st.REG_SYNC1, hi), ("wreg", st.REG_SYNC0, lo),
                ("delay", 20, None), ("cmd", 0x33, None), ("delay", 1000, None)]
        sync = sync_from_calls(calls)
        ok = calls == want and sync == expect
        extra = ""
        if not ok:
            extra = "  期望同步字 0x%04X / 序列 %s" % (expect, fmt_calls(want))
        check(ok, "ID=%-2d 原厂: sync=0x%04X (50024//%d) | %s%s"
              % (ident, expect, ident + 1, fmt_calls(calls), extra))
        if ident == 0:
            check(sync == 0xC368,
                  "ID=0 原厂 sync = 0x%04X = 50024（不是 KEY；说明 ID=0->KEY 是补丁才有的行为）"
                  % (sync if sync is not None else -1))


# ---------------------------------------------------------------------------
# 补丁镜像验证
# ---------------------------------------------------------------------------
def check_patched(image: bytes) -> None:
    header("2. 补丁镜像：ID==0 -> KEY；ID=1/5/37/99 -> 50024//(ID+1)（无 SCAL/1ms）")
    for key in KEY_VALUES:
        header("KEY = 0x%04X（手工构造 56 字节补丁）" % key)
        patched = st.apply_patch(image, key)
        blob = st.build_patch_blob(key)

        check(len(patched) == len(image),
              "补丁后镜像长度 %d 字节不变" % len(patched))
        off = st._off(st.SYNC_FN)
        check(patched[off:off + st.SYNC_FN_SIZE] == blob,
              "0x%08X..0x%08X 补丁区与 build_patch_blob() 完全一致"
              % (st.SYNC_FN, st.SYNC_FN_END - 1))
        diffs = [i for i in range(len(image)) if patched[i] != image[i]]
        region = set(range(off, off + st.SYNC_FN_SIZE))
        check(set(diffs) <= region,
              "与原厂差异 %d 字节，全部落在 0x%05X..0x%05X（区外 %d 处）"
              % (len(diffs), off, off + st.SYNC_FN_SIZE - 1,
                 len([d for d in diffs if d not in region])))
        check(len(diffs) == 48,
              "差异字节数 = %d（48B 代码 + KEY 字面量改变；50024 与原厂同值）" % len(diffs))

        for ident in IDS:
            res = run_sync_fn(patched, ident)
            if res["error"]:
                check(False, "KEY=0x%04X ID=%-2d 补丁 Unicorn 执行失败: %s"
                      % (key, ident, res["error"]))
                continue
            calls = res["calls"]
            if ident == 0:
                expect = key
                mode = "密钥模式（ID==0 -> KEY）"
            else:
                expect = st.ID_NUMERATOR // (ident + 1)
                mode = "原厂公式"
            hi, lo = expect >> 8, expect & 0xFF
            want = [("wreg", st.REG_SYNC1, hi), ("wreg", st.REG_SYNC0, lo),
                    ("delay", 20, None)]
            sync = sync_from_calls(calls)
            ok = (calls == want and sync == expect)
            extra = ""
            if not ok:
                extra = "  期望 sync=0x%04X / 序列 %s" % (expect, fmt_calls(want))
            check(ok, "ID=%-2d %s: sync=0x%04X -> SYNC1=0x%02X SYNC0=0x%02X | %s%s"
                  % (ident, mode, expect, hi, lo, fmt_calls(calls), extra))
            # 补丁必须没有 SCAL / 1ms：序列长度恰为 3，且无 cmd、无 delay(1000)
            check(len(calls) == 3 and
                  not any(c[0] == "cmd" for c in calls) and
                  not any(c[0] == "delay" and c[1] == 1000 for c in calls),
                  "ID=%-2d 补丁序列恰 3 次辅助调用（WriteReg x2 + delay(20)），"
                  "无 SCAL(0x33)、无 delay(1000)" % ident)
            if ident == 0:
                check(sync == key and sync != st.ID_NUMERATOR,
                      "ID=0 补丁 sync=0x%04X == KEY，且 != 原厂值 0x%04X"
                      % (sync, st.ID_NUMERATOR))
            else:
                check(sync != key,
                      "ID=%-2d 补丁 sync=0x%04X != KEY 0x%04X（普通模式未误入密钥分支）"
                      % (ident, sync, key))

        # 同一 ID 下两个 KEY 必须产生不同同步字（KEY 字面量确实生效）
        s1 = sync_from_calls(run_sync_fn(patched, 0)["calls"])
        patched2 = st.apply_patch(image, KEY_VALUES[1] if key == KEY_VALUES[0] else KEY_VALUES[0])
        s2 = sync_from_calls(run_sync_fn(patched2, 0)["calls"])
        check(s1 == key and s2 is not None and s2 != s1,
              "ID=0 在两个 KEY 下同步字分别为 0x%04X / 0x%04X（KEY 字面量生效）"
              % (s1 if s1 is not None else -1, s2 if s2 is not None else -1))

    # 原厂/补丁同 ID 对照：ID=5 原厂 0x2091，补丁 ID=5 仍 0x2091、ID=0 变 KEY
    orig5 = sync_from_calls(run_sync_fn(image, 5)["calls"])
    for key in KEY_VALUES:
        p = st.apply_patch(image, key)
        pat5 = sync_from_calls(run_sync_fn(p, 5)["calls"])
        pat0 = sync_from_calls(run_sync_fn(p, 0)["calls"])
        check(orig5 == 0x2091 and pat5 == 0x2091 and pat0 == key,
              "KEY=0x%04X 对照：ID=5 原厂=0x%04X 补丁=0x%04X（一致）；"
              "ID=0 补丁=0x%04X（KEY）"
              % (key, orig5 if orig5 is not None else -1,
                 pat5 if pat5 is not None else -1,
                 pat0 if pat0 is not None else -1))


def check_current_module(image: bytes) -> None:
    """只读对照：若 keypatch/devices/v1c.py（Lead 的实现）已存在，则用同一 Unicorn
    harness 直接跑它生成的真实补丁镜像（不修改该文件）。

    注意：本审计不要求实现字节与 audit_static.build_patch_blob 相同（本报告 §9 的布局
    只是一种等价实现）；只要求：56B 边界/字面量位置正确，且行为语义与设计一致。
    """
    header("3. 与 keypatch/devices/v1c.py 现行实现的只读对照（若存在）")
    mod_path = os.path.join(st.ROOT, "keypatch", "devices", "v1c.py")
    if not os.path.exists(mod_path):
        print("[SKIP] 未找到 %s（补丁模块尚未落地，跳过本项）" % mod_path)
        return
    mod_sha = hashlib.sha256(open(mod_path, "rb").read()).hexdigest()
    print("模块 : %s" % mod_path)
    print("sha256: %s" % mod_sha)
    import importlib.util
    spec = importlib.util.spec_from_file_location("v1c_patch_module_audit", mod_path)
    mod = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = mod
    prev_dont_write = sys.dont_write_bytecode
    sys.dont_write_bytecode = True   # 只读审计：不在 keypatch/devices/ 下生成 .pyc
    try:
        spec.loader.exec_module(mod)
    except Exception as exc:  # noqa: BLE001
        check(False, "导入 keypatch/devices/v1c.py 失败（可能仍在编辑中）：%r" % (exc,))
        return
    finally:
        sys.dont_write_bytecode = prev_dont_write

    # 常量一致性（这些是打补丁前必须严格校验的地址/字节）
    check(getattr(mod, "BASE", None) == st.BASE,
          "模块 BASE = 0x%08X（本审计 0x%08X）"
          % (getattr(mod, "BASE", -1), st.BASE))
    check(getattr(mod, "SYNC_FN", None) == st.SYNC_FN and
          getattr(mod, "SYNC_FN_END", None) == st.SYNC_FN_END,
          "模块函数区间 0x%08X..0x%08X（本审计 0x%08X..0x%08X）"
          % (getattr(mod, "SYNC_FN", -1), getattr(mod, "SYNC_FN_END", -1),
             st.SYNC_FN, st.SYNC_FN_END))
    check(getattr(mod, "ORIG_FUNC", None) == st.ORIG_FUNC,
          "模块 ORIG_FUNC 与原厂 56 字节一致（apply 前逐字节校验）")
    check(getattr(mod, "DIV_FUNC", None) == st.UIDIV and
          getattr(mod, "WRITE_REG", None) == st.WRITE_REG and
          getattr(mod, "WRITE_CMD", None) == st.STROBE and
          getattr(mod, "DELAY_US", None) == st.DELAY,
          "模块辅助函数地址 uidiv/WriteReg/strobe/delay = "
          "0x%08X/0x%08X/0x%08X/0x%08X"
          % (st.UIDIV, st.WRITE_REG, st.STROBE, st.DELAY))

    for key in KEY_VALUES:
        print()
        print("---- 模块实现 KEY=0x%04X ----" % key)
        blob = mod.build_sync_function(key)
        patched, _log = mod.apply(image, key)
        check(len(blob) == st.SYNC_FN_SIZE and len(patched) == len(image),
              "模块输出：函数体 %d B、镜像 %d B（长度不变）"
              % (len(blob), len(patched)))
        check(blob[0x30:0x34] == struct.pack("<I", key) and
              blob[0x34:0x38] == struct.pack("<I", st.ID_NUMERATOR),
              "模块字面量：KEY@+0x30 = 0x%04X，50024@+0x34 = %d（原厂同址）"
              % (key, st.ID_NUMERATOR))
        off = st._off(st.SYNC_FN)
        diffs = [i for i in range(len(image)) if patched[i] != image[i]]
        region = set(range(off, off + st.SYNC_FN_SIZE))
        check(patched[off:off + st.SYNC_FN_SIZE] == blob and set(diffs) <= region,
              "模块补丁区与 build_sync_function() 一致；与原厂差异 %d 字节全在区内"
              % len(diffs))
        # 代码/字面量不重叠：顺序解码到 +0x30
        md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
        md.skipdata = True
        code_ins = []
        for ins in md.disasm(blob, st.SYNC_FN):
            if ins.address >= st.SYNC_FN + 0x30:
                break
            code_ins.append(ins)
        code_end = code_ins[-1].address + code_ins[-1].size if code_ins else 0
        check(code_end <= st.SYNC_FN + 0x30,
              "模块实现：代码+对齐填充结束于 0x%08X（KEY 字面量在 +0x30，不重叠）"
              % code_end)
        bl_targets = [_bl_target(ins) for ins in code_ins
                      if ins.mnemonic in ("bl", "blx")]
        check(bl_targets == [st.UIDIV, st.WRITE_REG, st.WRITE_REG, st.DELAY],
              "模块 BL 目标 = %s（uidiv/WriteReg x2/delay）"
              % [("0x%08X" % t) for t in bl_targets])
        # 行为：直接跑模块生成的镜像
        for ident in IDS:
            res = run_sync_fn(patched, ident)
            if res["error"]:
                check(False, "模块 KEY=0x%04X ID=%d 执行失败: %s"
                      % (key, ident, res["error"]))
                continue
            calls = res["calls"]
            expect = key if ident == 0 else st.ID_NUMERATOR // (ident + 1)
            hi, lo = expect >> 8, expect & 0xFF
            want = [("wreg", st.REG_SYNC1, hi), ("wreg", st.REG_SYNC0, lo),
                    ("delay", 20, None)]
            sync = sync_from_calls(calls)
            check(calls == want and sync == expect,
                  "模块 KEY=0x%04X ID=%-2d: sync=0x%04X | %s"
                  % (key, ident, expect, fmt_calls(calls)))
            # 语义等价性：模块实现 vs 本审计独立构造的补丁（允许字节不同）
            ours = st.apply_patch(image, key)
            res2 = run_sync_fn(ours, ident)
            check(res2["error"] is None and res2["calls"] == calls,
                  "模块实现与本审计独立构造在该用例行为一致（仅布局可不同）")


def main() -> int:
    try:
        sys.stdout.reconfigure(encoding="utf-8", errors="replace")
    except Exception:  # noqa: BLE001
        pass
    print("V1C_V2.0.bin 补丁点 Unicorn 调用级独立验证（audit_unicorn.py）")
    print("固件 :", st.FW_PATH)
    print("Python:", sys.version.split()[0])
    print("Unicorn:", getattr(unicorn, "__version__", "?"))

    if not os.path.exists(st.FW_PATH):
        print("FATAL: 固件不存在:", st.FW_PATH)
        return 2
    image = st.load_image()
    got = hashlib.sha256(image).hexdigest()
    print("镜像 : %d 字节 sha256=%s" % (len(image), got))
    if got != st.IMAGE_SHA256 or len(image) != st.IMAGE_SIZE:
        print("FATAL: 固件身份不匹配（期望 %d 字节 / %s）"
              % (st.IMAGE_SIZE, st.IMAGE_SHA256))
        return 2

    print("harness: 执行 0x%08X|1；r0=ID；桩替换 WriteReg(0x%08X)/strobe(0x%08X)/"
          "delay(0x%08X)；__aeabi_uidiv(0x%08X) 保留真实代码"
          % (st.SYNC_FN, st.WRITE_REG, st.STROBE, st.DELAY, st.UIDIV))

    check_original(image)
    check_patched(image)
    check_current_module(image)

    npass = sum(1 for ok, _ in RESULTS if ok)
    nfail = len(RESULTS) - npass
    header("汇总")
    print("检查项: %d   PASS: %d   FAIL: %d" % (len(RESULTS), npass, nfail))
    if nfail:
        print("\n失败项：")
        for ok, msg in RESULTS:
            if not ok:
                print("  [FAIL]", msg)
    print("\n结论:", "PASS（Unicorn 调用级验证全部通过）" if nfail == 0
          else "FAIL（存在不符项）")
    return 0 if nfail == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
