# -*- coding: utf-8 -*-
"""AD200 Pro V1.60 —— C.Fn 交互路径 Unicorn 调用级验证（cfn-re / task-1）。

验证内容：
  A. 真实按键路径：flash_power_task 0x08004500 轮询 GPIOA IDR bit8(PA8)/bit9(PA9)，
     顺序注入 PA9→both 调用 inc 0x08008504；PA8→both 调用 dec 0x080045AC。
  B. inc/dec 处理函数内部行为（原厂 vs 补丁）：
     - C.Fn 项导航：inc UI+0x14+1（>7 回绕 0）；dec -1（0 回绕 7）=> F8 可达
     - 值编辑（UI+0x13!=0）：F8(settings[7]) 只 0/1（上限表 0x0800E314[7]=1）
     - 原厂：仅 item==6(ID) 改值调用 0x0800B9AC；F8 改值不调用（=> 切换不立即生效）
     - 补丁 A（Lead 方案，0x080045EC/0x0800854C 各改 4B 为 2x nop）：
       任何 item 改值都调用同步字，且调用时 settings[7] 已更新
     - 补丁 B（本报告建议，更小：只把 2 处 `bne` 条件码 d1->d3，共 2 字节）：
       仅 item>=6（ID/F8）改值调用同步字；F1..F6 行为与原厂完全一致
  C. F8 渲染：cfn_render 0x08003FD0 对 item7、settings[7]=0/1 生成不同段码位图
  D. 补丁字节 / 差异范围 / sha256。

运行（脚本内部写 UTF-8 日志 verify_handlers.log）：
    devices\ad200pro\\python\\python.exe verify_handlers.py
"""
from __future__ import annotations

import hashlib
import os
import sys
from contextlib import redirect_stdout

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(HERE))))
BASE = 0x08003000
BIN = os.path.join(ROOT, "devices", "x3c", "firmware", "AD200pro_V1.60.bin")
LOG = os.path.join(HERE, "verify_handlers.log")

UI = 0x20000150
CFN_OPEN = UI + 0x12        # 0x20000162 C.Fn 打开标志
CFN_STATE = UI + 0x13       # 0x20000163 编辑态（1=值编辑，0=项导航）
CFN_ITEM = UI + 0x14        # 0x20000164 当前项 0..7
SETTINGS = 0x20000418       # settings[0..7]
SEG_BUF = 0x20000408        # 段码位图 16B
SAVE_COUNTDOWN = 0x20000218  # 0x0800B810 strh #200（保存倒计时）

FLASH_BASE = 0x08000000
FLASH_SIZE = 0x20000
RAM_BASE = 0x20000000
RAM_SIZE = 0x2000
STACK_TOP = RAM_BASE + 0x1800
RET_STUB = RAM_BASE + 0x1F00

FUNC_DEC = 0x080045AC
FUNC_INC = 0x08008504
FUNC_KEYTASK = 0x08004500
FUNC_SYNC = 0x0800B9AC
FUNC_CFN_RENDER = 0x08003FD0
FUNC_GPIO_READ = 0x08006E94

HELPERS = {
    0x0800CAA2: "WriteReg(reg,val)",
    0x0800C828: "delay_us",
    0x0800CB64: "strobe",
}

# 补丁 A（Lead 已做）：cmp rX,#6 + bne -> nop;nop
NOP2 = b"\x00\xbf"
PATCH_NOP_SITES = [
    (0x080045EC, "dec 0x080045AC：cmp r2,#6 / bne 4B -> 00 bf 00 bf"),
    (0x0800854C, "inc 0x08008504：cmp r0,#6 / bne 4B -> 00 bf 00 bf"),
]
# 补丁 B（本报告建议）：只把 bne(0xd1) 改成 blo(0xd3)，1 字节/处，
#   条件变为 "item >= 6 才重新同步"（ID 与 F8），F1..F6 完全不动
PATCH_BLO_SITES = [
    (0x080045EF, "dec：0xd1(bne) -> 0xd3(blo)  => item>=6 才同步"),
    (0x0800854F, "inc：0xd1(bne) -> 0xd3(blo)  => item>=6 才同步"),
]

results = []


def check(ok, msg):
    results.append((bool(ok), msg))
    print(("[PASS] " if ok else "[FAIL] ") + msg)
    return ok


_orig_cache = None


def orig_bytes():
    global _orig_cache
    if _orig_cache is None:
        _orig_cache = open(BIN, "rb").read()
    return _orig_cache


def load_image(mode: str) -> bytes:
    """mode: orig / nop(=Lead 补丁 A) / blo(=建议补丁 B)"""
    out = bytearray(orig_bytes())
    if mode == "orig":
        return bytes(out)
    if mode == "nop":
        for addr, _why in PATCH_NOP_SITES:
            off = addr - BASE
            out[off:off + 4] = NOP2 + NOP2
        return bytes(out)
    if mode == "blo":
        for addr, _why in PATCH_BLO_SITES:
            off = addr - BASE
            assert out[off] == 0xD1, "0x%08X 原字节不是 0xD1" % addr
            out[off] = 0xD3
        return bytes(out)
    raise ValueError(mode)


def new_machine(image: bytes):
    from unicorn import (Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS,
                         UC_HOOK_CODE, UC_HOOK_MEM_WRITE)
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_SP,
                                   UC_ARM_REG_LR, UC_ARM_REG_PC)

    uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
    uc.mem_map(FLASH_BASE, FLASH_SIZE)
    uc.mem_write(BASE, image)
    uc.mem_map(RAM_BASE, RAM_SIZE)
    uc.mem_write(RET_STUB, b"\xfe\xe7")

    st = {"events": [], "gpio_idr": 0xFFFF}

    def code_hook(uc_, address, size, ud):
        a = address & ~1
        if a == RET_STUB:
            uc_.emu_stop()
            return
        if a == FUNC_SYNC:
            # 记录调用：入参 = r0（settings[6]=ID），同时快照 settings[6]/[7]
            st["events"].append(("sync", uc_.reg_read(UC_ARM_REG_R0) & 0xFF,
                                 uc_.mem_read(SETTINGS + 6, 1)[0],
                                 uc_.mem_read(SETTINGS + 7, 1)[0]))
            return   # 继续执行真实同步字函数（内部辅助函数被下面的 hook 跳过）
        if a in (FUNC_INC, FUNC_DEC):
            st["events"].append(("handler", a))
        if a == FUNC_KEYTASK:
            st["events"].append(("keytask",))
        if a == FUNC_GPIO_READ:
            port = uc_.reg_read(UC_ARM_REG_R0)
            mask = uc_.reg_read(UC_ARM_REG_R1) & 0xFFFF
            val = 1 if (st["gpio_idr"] & mask) else 0 if port == 0x48000000 else 1
            st["events"].append(("gpio", port, mask, val))
            uc_.reg_write(UC_ARM_REG_R0, val)
            uc_.reg_write(UC_ARM_REG_PC, uc_.reg_read(UC_ARM_REG_LR))
            return
        if a in HELPERS:
            if HELPERS[a] == "WriteReg(reg,val)":
                st["events"].append(("reg", uc_.reg_read(UC_ARM_REG_R0) & 0xFF,
                                     uc_.reg_read(UC_ARM_REG_R1) & 0xFF))
            elif HELPERS[a] == "delay_us":
                st["events"].append(("delay", uc_.reg_read(UC_ARM_REG_R0)))
            else:
                st["events"].append(("strobe", uc_.reg_read(UC_ARM_REG_R0) & 0xFF))
            uc_.reg_write(UC_ARM_REG_PC, uc_.reg_read(UC_ARM_REG_LR))
            return
        # 与本次验证无关的硬件/显示/存储函数：记录并跳过
        if a in (0x080038F0, 0x0800BA40, 0x08009D58, 0x0800B7F4, 0x0800B820):
            st["events"].append(("stub", a))
            uc_.reg_write(UC_ARM_REG_PC, uc_.reg_read(UC_ARM_REG_LR))
            return

    def mem_write_hook(uc_, access, address, size, value, ud):
        if SETTINGS <= address < SETTINGS + 8:
            st["events"].append(("mem", address - SETTINGS, value & 0xFF))

    uc.hook_add(UC_HOOK_CODE, code_hook)
    uc.hook_add(UC_HOOK_MEM_WRITE, mem_write_hook)
    return uc, st


def set_state(uc, cfn_open=1, cfn_state=0, cfn_item=0, settings=None):
    for i in range(0x50):
        uc.mem_write(UI + i, b"\x00")
    uc.mem_write(SETTINGS, bytes(8))
    uc.mem_write(SEG_BUF, bytes(16))
    uc.mem_write(SAVE_COUNTDOWN, bytes(2))
    uc.mem_write(CFN_OPEN, bytes([cfn_open & 0xFF]))
    uc.mem_write(CFN_STATE, bytes([cfn_state & 0xFF]))
    uc.mem_write(CFN_ITEM, bytes([cfn_item & 0xFF]))
    if settings:
        uc.mem_write(SETTINGS, bytes(settings))


def call_func(uc, addr, timeout_count=400000):
    from unicorn.arm_const import UC_ARM_REG_SP, UC_ARM_REG_LR, UC_ARM_REG_PC
    uc.reg_write(UC_ARM_REG_SP, STACK_TOP)
    uc.reg_write(UC_ARM_REG_LR, RET_STUB | 1)
    uc.emu_start(addr | 1, 0, count=timeout_count)
    pc = uc.reg_read(UC_ARM_REG_PC) & ~1
    if pc != RET_STUB:
        raise RuntimeError("函数 0x%08X 未正常返回（PC=0x%08X）" % (addr, pc))


def run_handler(mode, handler, cfn_state, cfn_item, settings, cfn_open=1,
                gpio_idr=0xFFFF):
    uc, st = new_machine(load_image(mode))
    set_state(uc, cfn_open=cfn_open, cfn_state=cfn_state, cfn_item=cfn_item,
              settings=settings)
    st["gpio_idr"] = gpio_idr
    call_func(uc, handler)
    return {
        "events": st["events"],
        "settings": list(uc.mem_read(SETTINGS, 8)),
        "cfn": {
            "open": uc.mem_read(CFN_OPEN, 1)[0],
            "state": uc.mem_read(CFN_STATE, 1)[0],
            "item": uc.mem_read(CFN_ITEM, 1)[0],
        },
        "save_timer": int.from_bytes(uc.mem_read(SAVE_COUNTDOWN, 2), "little"),
    }


def syncs(r):
    return [e for e in r["events"] if e[0] == "sync"]


def sync_events_after_settings_write(r, idx):
    """settings[idx] 的写事件是否早于第一次 sync 调用。"""
    ev = r["events"]
    mem = [i for i, e in enumerate(ev) if e[0] == "mem" and e[1] == idx]
    sy = [i for i, e in enumerate(ev) if e[0] == "sync"]
    return bool(mem) and bool(sy) and mem[0] < sy[0]


# ---------------------------------------------------------------- D. 补丁字节
def test_patch_bytes():
    print("\n=== D. 补丁字节 / 差异范围 / sha256 ===")
    orig = orig_bytes()
    check(len(orig) == 46380, "原厂镜像长度 46380 字节")
    for addr, why in PATCH_NOP_SITES:
        off = addr - BASE
        print("    补丁A 0x%08X: %s -> %s   ; %s"
              % (addr, orig[off:off + 4].hex(" "),
                 load_image("nop")[off:off + 4].hex(" "), why))
    for addr, why in PATCH_BLO_SITES:
        off = addr - BASE
        print("    补丁B 0x%08X: %02x -> %02x           ; %s"
              % (addr, orig[off], load_image("blo")[off], why))

    a = load_image("nop")
    diffs = [i for i in range(len(orig)) if orig[i] != a[i]]
    want = {addr - BASE + k for addr, _ in PATCH_NOP_SITES for k in range(4)}
    check(set(diffs) == want and len(a) == len(orig),
          "补丁A：长度不变，差异仅 8 字节 offsets=%s" % [hex(d) for d in diffs])
    b = load_image("blo")
    diffs_b = [i for i in range(len(orig)) if orig[i] != b[i]]
    want_b = {addr - BASE for addr, _ in PATCH_BLO_SITES}
    check(set(diffs_b) == want_b and len(b) == len(orig),
          "补丁B：长度不变，差异仅 2 字节 offsets=%s" % [hex(d) for d in diffs_b])
    print("    补丁A sha256 = %s" % hashlib.sha256(a).hexdigest())
    print("    补丁B sha256 = %s" % hashlib.sha256(b).hexdigest())


# ---------------------------------------------------------------- A. 按键路径
def test_key_path():
    print("\n=== A. 真实按键路径：GPIOA PA8/PA9 -> flash_power_task 0x08004500 -> inc/dec ===")
    uc, st = new_machine(load_image("orig"))
    set_state(uc)

    def keytask(idr):
        st["gpio_idr"] = idr
        call_func(uc, FUNC_KEYTASK)
        return [(e[0], e[1]) for e in st["events"] if e[0] == "handler"]

    # INC：PA9(bit9) 先按 -> both。flash_power_task 每轮读 PA8(mask0x100)/PA9(mask0x200)
    st["events"].clear()
    keytask(0xFFFF & ~0x200)           # 仅 PA9 低（按下）
    seq_inc = keytask(0xFFFF & ~0x300)  # PA8+PA9 均低
    names = ["inc" if a == FUNC_INC else "dec" for _k, a in seq_inc]
    print("    PA9->both 调用: %s" % names)
    check(names == ["inc"], "PA9(bit9)->both 触发 inc 0x08008504")

    # DEC：PA8(bit8) 先按 -> both
    set_state(uc)
    st["events"].clear()
    keytask(0xFFFF & ~0x100)
    seq_dec = keytask(0xFFFF & ~0x300)
    names = ["inc" if a == FUNC_INC else "dec" for _k, a in seq_dec]
    print("    PA8->both 调用: %s" % names)
    check(names == ["dec"], "PA8(bit8)->both 触发 dec 0x080045AC")

    # 无按键：不触发
    set_state(uc)
    st["events"].clear()
    keytask(0xFFFF)
    keytask(0xFFFF)
    check(not [e for e in st["events"] if e[0] == "handler"],
          "PA8/PA9 未按下时不调用 inc/dec（0x20000170/71 状态机）")


# ---------------------------------------------------------------- B. 处理函数
def test_nav_and_values():
    print("\n=== B1. C.Fn 项导航 / F8 取值（原厂，证明 F8 可达且值只有 0/1）===")
    r = run_handler("orig", FUNC_INC, cfn_state=0, cfn_item=7, settings=[0] * 8)
    check(r["cfn"]["item"] == 0, "inc 导航 F8(item7)+1 -> 回绕 item0（实际 %d）"
          % r["cfn"]["item"])
    r = run_handler("orig", FUNC_DEC, cfn_state=0, cfn_item=0, settings=[0] * 8)
    check(r["cfn"]["item"] == 7, "dec 导航 F1(item0)-1 -> 回绕 item7（F8 可达）")
    r = run_handler("orig", FUNC_INC, cfn_state=0, cfn_item=6, settings=[0] * 8)
    check(r["cfn"]["item"] == 7, "inc 导航 item6(F7 ID) -> item7(F8)")
    for handler, v0, v1, msg in [
            (FUNC_INC, 0, 1, "F8 inc: 0 -> 1"),
            (FUNC_INC, 1, 0, "F8 inc: 1 -> 0（超上限回绕）"),
            (FUNC_DEC, 1, 0, "F8 dec: 1 -> 0"),
            (FUNC_DEC, 0, 1, "F8 dec: 0 -> 回绕上限 1")]:
        s = [0] * 8
        s[7] = v0
        r = run_handler("orig", handler, cfn_state=1, cfn_item=7, settings=s)
        check(r["settings"][7] == v1, "%s（实际 %d）" % (msg, r["settings"][7]))
    r = run_handler("orig", FUNC_INC, cfn_state=1, cfn_item=7, settings=[0] * 8)
    check(r["settings"][7] == 1, "F8 编辑态 inc 写入 settings[7]=1（0x0800041F）")


def test_immediate_effect_orig():
    print("\n=== B2. 原厂：只有 item6(ID) 改值才调用同步字 0x0800B9AC ===")
    for item in range(8):
        s = [0] * 8
        s[item] = 1
        if item == 6:
            s[item] = 1
        r = run_handler("orig", FUNC_INC, cfn_state=1, cfn_item=item, settings=s)
        if item == 6:
            check(len(syncs(r)) == 1, "原厂 item6(ID) 改值 -> 调用 0x0800B9AC")
        else:
            check(not syncs(r), "原厂 item%d 改值 -> 不调用同步字" % item)
    s = [0] * 8
    s[6] = 37
    r = run_handler("orig", FUNC_INC, cfn_state=1, cfn_item=7, settings=s)
    check(not syncs(r) and r["settings"][7] == 1,
          "原厂 F8 0->1：settings[7] 已变 1，但同步字函数未被调用（=> 必须补丁）")


def test_patch_behaviour(mode, label):
    print("\n=== %s：补丁 %s 的即时生效行为 ===" % (label, mode))
    # F8 0->1 与 1->0
    for v0, v1 in ((0, 1), (1, 0)):
        s = [0] * 8
        s[6] = 37
        s[7] = v0
        r = run_handler(mode, FUNC_INC if v0 == 0 else FUNC_DEC,
                        cfn_state=1, cfn_item=7, settings=s)
        sy = syncs(r)
        check(r["settings"][7] == v1 and len(sy) == 1 and sy[0][1] == 37 and sy[0][3] == v1,
              "F8 %d->%d：调用 0x0800B9AC 入参 settings[6]=37，调用时 settings[7]=%d"
              % (v0, v1, v1))
        check(sync_events_after_settings_write(r, 7),
              "F8 %d->%d：先写 settings[7] 再调用同步字（事件顺序 mem->sync）" % (v0, v1))
    # item6(ID) 行为保持
    s = [0] * 8
    s[6] = 20
    r = run_handler(mode, FUNC_INC, cfn_state=1, cfn_item=6, settings=s)
    sy = syncs(r)
    check(len(sy) == 1 and sy[0][1] == 21,
          "item6(ID) 20->21 仍调用同步字（入参 21）")
    # 导航/值编辑结果与补丁前一致
    diffs = []
    for item in range(8):
        for handler in (FUNC_INC, FUNC_DEC):
            s = [0] * 8
            s[item] = 1
            a = run_handler("orig", handler, cfn_state=1, cfn_item=item, settings=s)
            b = run_handler(mode, handler, cfn_state=1, cfn_item=item, settings=s)
            if a["settings"] != b["settings"] or a["cfn"] != b["cfn"]:
                diffs.append((item, hex(handler)))
    check(not diffs, "补丁未改变 item/值编辑结果（item0..7 x inc/dec 对比一致）%s"
          % ("" if not diffs else "；差异 %s" % diffs))
    # 保存倒计时
    r = run_handler(mode, FUNC_INC, cfn_state=1, cfn_item=7, settings=[0] * 8)
    check(r["save_timer"] == 0xC8,
          "处理函数调用 0x0800B810：0x20000218(u16)=%d（期望 200=0xC8 保存倒计时）"
          % r["save_timer"])


def test_other_items(mode, label):
    print("\n=== %s：补丁 %s 对其它 item 的同步字调用范围 ===" % (label, mode))
    synced_items = []
    for item in range(8):
        s = [0] * 8
        s[6] = 11
        if item != 6:
            s[item] = 1
        r = run_handler(mode, FUNC_INC, cfn_state=1, cfn_item=item, settings=s)
        sy = syncs(r)
        if sy:
            expect = 11 if item != 6 else 12
            ok_arg = sy[0][1] == expect
            synced_items.append((item, ok_arg))
    items = [i for i, _ok in synced_items]
    args_ok = all(ok for _i, ok in synced_items)
    print("    会触发同步字的 item: %s" % items)
    if mode == "nop":
        check(items == list(range(8)) and args_ok,
              "补丁A：item0..7 全部改值都重新同步（入参=settings[6]）")
    elif mode == "blo":
        check(items == [6, 7] and args_ok,
              "补丁B：仅 item6(ID)/item7(F8) 改值重新同步（F1..F6 不再多调）")
    a = run_handler("orig", FUNC_INC, cfn_state=1, cfn_item=3, settings=[0] * 8)
    b = run_handler(mode, FUNC_INC, cfn_state=1, cfn_item=3, settings=[0] * 8)
    if mode == "nop":
        check(bool(syncs(b)) and not syncs(a),
              "补丁A：F4(item3) 改值也会调用同步字（行为变化范围较大）")
    else:
        check(not syncs(a) and not syncs(b),
              "补丁B：F4(item3) 改值与原厂一样不调用同步字（行为变化最小）")


# ---------------------------------------------------------------- C. 渲染
def test_render():
    print("\n=== C. F8 渲染：cfn_render 0x08003FD0（item7, settings[7]=0/1）===")

    def render(item, s7):
        uc, _st = new_machine(load_image("orig"))
        set_state(uc, cfn_open=1, cfn_state=0, cfn_item=item,
                  settings=[0] * 7 + [s7])
        call_func(uc, FUNC_CFN_RENDER)
        return bytes(uc.mem_read(SEG_BUF, 16))

    b_off = render(7, 0)
    b_on = render(7, 1)
    print("    F8 OFF (settings[7]=0): %s" % b_off.hex(" "))
    print("    F8 ON  (settings[7]=1): %s" % b_on.hex(" "))
    check(b_off != b_on, "F8 OFF/ON 两种值渲染出不同 16 字节段位图")
    diffs = [i for i in range(16) if b_off[i] != b_on[i]]
    print("    OFF/ON 差异字节位置: %s" % [i for i in diffs])
    check(all(i < 8 for i in diffs) and b_off[8:10] == b_on[8:10],
          "F8 差异只在值字段（字节 0..7），\"F8\"编号字段（字节 8..9）两值一致")
    check(render(0, 0) != b_off, "F1(item0) 与 F8(item7) 位图不同（F 编号随 item 变化）")
    print("    各页段位图（settings 全 0）：")
    for i in range(8):
        print("      item%d(F%d): %s" % (i, i + 1, render(i, 0).hex(" ")))
    return b_off, b_on


# ---------------------------------------------------------------- E. 端到端
def test_end_to_end():
    print("\n=== E. 端到端按键路径：MENU 进入 C.Fn -> 导航到 F8 -> SET 编辑 -> + 改值 -> MENU 退出 ===")
    MENU = 0x08004270     # matrix 0x100 -> 开关 UI+0x12
    SET = 0x0800B90C      # matrix 0x020 -> 在 C.Fn 内切换 UI+0x13
    uc, st = new_machine(load_image("nop"))
    set_state(uc, cfn_open=0, cfn_state=0, cfn_item=0, settings=[0, 0, 0, 0, 0, 0, 5, 0])

    def step(addr):
        st["events"].append(("--", addr))
        call_func(uc, addr)
        return (uc.mem_read(CFN_OPEN, 1)[0], uc.mem_read(CFN_STATE, 1)[0],
                uc.mem_read(CFN_ITEM, 1)[0])

    o, s, i = step(MENU)
    check(o == 1 and s == 0 and i == 0, "MENU(矩阵0x100) 按下：UI+0x12=1 打开 C.Fn")
    o, s, i = step(FUNC_DEC)
    check(i == 7, "C.Fn 内 -（dec）从 F1 回绕导航到 F8（item7）")
    o, s, i = step(SET)
    check(o == 1 and s == 1, "SET(矩阵0x020)：UI+0x13=1 进入 F8 值编辑")
    st["events"].clear()
    o, s, i = step(FUNC_INC)
    sy = [e for e in st["events"] if e[0] == "sync"]
    check(list(uc.mem_read(SETTINGS, 8))[7] == 1 and len(sy) == 1
          and sy[0][1] == 5 and sy[0][3] == 1,
          "编辑态 +（inc）：settings[7] 0->1 且立即调用同步字(入参 ID=5)")
    o, s, i = step(SET)
    check(s == 0, "再按 SET：UI+0x13=0 退出值编辑（回到项导航）")
    o, s, i = step(MENU)
    check(o == 0, "MENU 再按：UI+0x12=0 退出 C.Fn")

    # 同一路径跑原厂镜像：F8 改值后同步字不会被调用（对照）
    uc2, st2 = new_machine(load_image("orig"))
    set_state(uc2, cfn_open=0, cfn_state=0, cfn_item=0,
              settings=[0, 0, 0, 0, 0, 0, 5, 0])
    st2["events"].clear()
    call_func(uc2, MENU)
    call_func(uc2, FUNC_DEC)      # 0 -> 7
    call_func(uc2, SET)           # 编辑
    call_func(uc2, FUNC_INC)      # 0 -> 1
    sy2 = [e for e in st2["events"] if e[0] == "sync"]
    check(list(uc2.mem_read(SETTINGS, 8))[7] == 1 and not sy2,
          "对照原厂：同一操作 settings[7]=1 但未调用同步字（证明补丁必要）")


def main():
    print("AD200 Pro V1.60 C.Fn 交互路径 Unicorn 调用级验证（cfn-re / task-1）")
    print("镜像 %s  %d 字节 sha256=%s"
          % (BIN, len(orig_bytes()), hashlib.sha256(orig_bytes()).hexdigest()))
    test_patch_bytes()
    test_key_path()
    test_nav_and_values()
    test_immediate_effect_orig()
    test_other_items("nop", "补丁A(Lead)")
    test_patch_behaviour("nop", "补丁A(Lead)")
    test_other_items("blo", "补丁B(建议)")
    test_patch_behaviour("blo", "补丁B(建议)")
    test_end_to_end()
    test_render()
    bad = [m for ok, m in results if not ok]
    print("\n===== 汇总：%d/%d PASS =====" % (len(results) - len(bad), len(results)))
    for m in bad:
        print("  FAIL: " + m)
    return 1 if bad else 0


class _Tee:
    def __init__(self, path, stream):
        self.f = open(path, "w", encoding="utf-8")
        self.s = stream

    def write(self, text):
        self.s.write(text)
        self.f.write(text)

    def flush(self):
        self.s.flush()
        self.f.flush()


if __name__ == "__main__":
    try:
        with open(os.devnull, "w"):
            pass
        tee = _Tee(LOG, sys.stdout)
        with redirect_stdout(tee):
            rc = main()
        print("[log] %s" % LOG)
        sys.exit(rc)
    except Exception as exc:  # noqa: BLE001
        import traceback
        traceback.print_exc()
        print("FAIL:", exc)
        sys.exit(1)
