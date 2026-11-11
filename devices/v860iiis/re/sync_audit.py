# -*- coding: utf-8 -*-
"""V860IIIS_V1.1.bin —— 平台/内核判定 + 同步字函数独立审计 + Unicorn 调用级验证。

对象：devices/v860iiis/firmware/V860IIIS_V1.1.bin（79300 B，只读，绝不修改）
产物：本脚本可重复运行；退出码 0 = 全部检查 PASS；同时把完整输出写入
      devices/v860iiis/re/log_sync_audit.txt（UTF-8）。

用法（在 D:\\神牛 下）：
    tools\\python\\python.exe devices\\v860iiis\\re\\sync_audit.py

约束：本脚本不 import 任何设备/补丁模块（Unicorn harness 完全内嵌、独立）；
      只读取固件，不写固件。
"""

import collections
import hashlib
import io
import os
import struct
import sys

# ---------------------------------------------------------------------------
# 0. 基础常量（全部来自本次独立复核，见 SYNC_AUDIT.md）
# ---------------------------------------------------------------------------
HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(HERE)))
IMAGE_PATH = os.path.join(ROOT, "devices", "v860iiis", "firmware", "V860IIIS_V1.1.bin")
LOG_PATH = os.path.join(HERE, "log_sync_audit.txt")

BASE = 0x08003000                 # 镜像基址（由 __main 桩反推，§1）
IMAGE_SIZE = 79300                # 0x135C4
IMAGE_SHA256 = ("bdc8f3547808d121f15a6c58b9363c5792c957c2dce2a16b203276ab078d0b20")
END = BASE + IMAGE_SIZE           # 0x080165C4

SP_INIT = 0x20001548              # 向量[0] 初值 SP
RESET_VEC = 0x080030D5            # 向量[1] Reset（Thumb）

SYNC_FN = 0x080115B8              # 同步字函数入口
SYNC_FN_END = 0x080115F0          # 下一函数入口（本函数占 56B：0x080115B8..0x080115EF）
SYNC_FN_SIZE = SYNC_FN_END - SYNC_FN
NEXT_FN = 0x080115F0              # 0x080115F0 是真实函数入口（BL @0x0800CD80）
NEXT_NEXT_FN = 0x0801162C         # 再下一个函数入口（BL @0x08005B18 / 0x0800C664）

NUMERATOR = 50024                 # 0xC368
NUMERATOR_VA = 0x080115EC         # +0x34
ORIG_FUNC_HEX = ("0c4a10b5411c1046f1f796fd0446010a042002f0acf9e1b2052002f0a8f9"
                 "142002f05ef8332002f003fa7d20c00002f057f810bd68c30000")
ORIG_FUNC = bytes.fromhex(ORIG_FUNC_HEX)

UDIV = 0x080030F0                 # __aeabi_uidiv（软件除法）
WRITE_REG = 0x08013926            # WriteReg(reg=r0, val=r1)
STROBE = 0x080139E8               # strobe(cmd=r0)
DELAY_US = 0x08013698             # delay_us(n=r0)
WRITE_BUF = 0x08013A00            # 底层写：r0=起始寄存器/命令, r1=buf, r2=len
CS_LOW = 0x08013A2C
SPI_XFER = 0x08013970
CS_HIGH = 0x08013A4C
READ_CMD = 0x080139BC             # 底层读（r0=0x80|reg 等）

INIT_TABLE = 0x080136C8           # CC2500 配置初始化表函数
INIT_SYNC1_SITE = 0x080136F6      # WriteReg(4, 0xC3)
INIT_SYNC0_SITE = 0x080136FE      # WriteReg(5, 0x68)
RF_INIT_FN = 0x08005DD0           # RF 初始化函数（先 init 表、后同步字）
RF_INIT_TABLE_CALL = 0x08005DE6   # bl INIT_TABLE
RF_INIT_SYNC_CALL = 0x08005E36    # bl SYNC_FN

SETTINGS_BASE = 0x20001058        # C.Fn 设置区基址
ID_OFFSET = 6                     # settings[6] = 远程 ID

SYNC1_REG = 4
SYNC0_REG = 5

CALL_SITES = [0x08005A78, 0x08005E36, 0x0800C5C2, 0x0800D0BA, 0x0800D0E6]
# 每个调用点所在函数入口（由"最大已知函数入口 <= 调用点"独立求得）
CALL_SITE_FN = {
    0x08005A78: 0x080059E4,
    0x08005E36: 0x08005DD0,
    0x0800C5C2: 0x0800C528,
    0x0800D0BA: 0x0800CD7C,
    0x0800D0E6: 0x0800CD7C,
}
# 每个调用点之前"取 settings[6]"的指令与其 settings 基址装载指令
CALL_SITE_ID_LOAD = {
    0x08005A78: (0x08005A76, "r0", 0x08005A26),   # ldrb r0,[r0,#6] ; ldr r0,=0x20001058
    0x08005E36: (0x08005E34, "r0", 0x08005E32),
    0x0800C5C2: (0x0800C5C0, "r0", 0x0800C56A),
    0x0800D0BA: (0x0800D0B8, "r4", 0x0800D094),
    0x0800D0E6: (0x0800D0E4, "r0", 0x0800D0E2),
}
# 触发路径（静态调用链），脚本用 BL 扫描独立复核每条边
TRIGGER_PATHS = {
    0x08005A78: [("主循环", 0x08013CE4), ("状态轮询", 0x080058E4), ("改值处理", 0x080059E4)],
    0x08005E36: [("整机初始化", 0x08012364), ("RF 初始化", 0x08005DD0)],
    0x0800C5C2: [("主循环", 0x08013CE4), ("状态轮询", 0x080058E4), ("改值处理", 0x0800C528)],
    0x0800D0BA: [("主循环", 0x08013CE4), ("事件分派", 0x0800CD7C)],
    0x0800D0E6: [("主循环", 0x08013CE4), ("事件分派", 0x0800CD7C)],
}
MAIN_LOOP = 0x08013CE4
POLL_FN = 0x080058E4
EVENT_FN = 0x0800CD7C
INIT123_FN = 0x08012364

# 期望的"32 位非 BL 解码"（全部是数据/常量表被误判；逐条在脚本里给上下文）
NONBL4_EXPECTED = {
    0x08004124: "ldc p15, c3, [r2, #0x1f0]",
    0x080092C8: "and r8, r0, #1",
    0x0800AE28: "stc2l p11, c1, [r1, #0x304]!",
    0x0800E4C4: "stc p0, c14, [r0, #-0]",
    0x08010DC8: "and r8, r0, #1",
    0x08014088: "cdp2 p2, #0, c8, c4, c2, #4",
    0x08014092: "cdp2 p0, #4, c0, c0, c0, #0",
    0x080140A6: "cdp2 p0, #9, c0, c2, c0, #0",
    0x080141A6: "cdp2 p0, #9, c0, c2, c0, #0",
}
# 注：以审计脚本实际扫描结果为准（下断言时按真实集合判定），此表仅用于报告说明。

# 代码洞候选（Lead 初勘给出，须逐一验证）
CAVE_CANDIDATES = [0x08014060, 0x08014B6E, 0x080162B9]

# Unicorn 常量
UC_FLASH_BASE, UC_FLASH_SIZE = 0x08003000, 0x14000   # 0x08003000..0x08017000
UC_RAM_BASE, UC_RAM_SIZE = 0x20000000, 0x4000        # 0x20000000..0x20004000
UC_SP = 0x20001548                                   # 用原厂向量[0] 初值作栈顶
UC_RET_SENTINEL = 0x20001FF0                         # LR 哨兵落点（放 `b .`）
UC_LR = UC_RET_SENTINEL | 1
UC_R4_SENTINEL = 0xDEADBEEF
UC_IDS = (0, 1, 5, 37, 99)
UC_MAX_INSNS = 20000

# ---------------------------------------------------------------------------
# 1. 输出/断言框架
# ---------------------------------------------------------------------------
class Reporter:
    def __init__(self):
        self.lines = []
        self.results = []

    def line(self, text=""):
        self.lines.append(text)
        try:
            print(text)
        except UnicodeEncodeError:                    # pragma: no cover
            print(text.encode("utf-8", "replace").decode("utf-8", "replace"))

    def header(self, title):
        self.line()
        self.line("=" * 78)
        self.line(title)
        self.line("=" * 78)

    def check(self, cond, msg):
        ok = bool(cond)
        self.results.append(ok)
        self.line("[%s] %s" % ("PASS" if ok else "FAIL", msg))
        return ok

    def info(self, msg):
        self.line("       " + msg)

    def save(self, path):
        with io.open(path, "w", encoding="utf-8", newline="\n") as fh:
            fh.write("\n".join(self.lines) + "\n")

    @property
    def passed(self):
        return sum(1 for x in self.results if x)

    @property
    def failed(self):
        return sum(1 for x in self.results if not x)


R = Reporter()


def hx(v):
    return "0x%08X" % v


# ---------------------------------------------------------------------------
# 2. 反汇编基础设施（capstone，Thumb-1/Thumb-2 自动；数据用 skipdata）
# ---------------------------------------------------------------------------
try:
    from capstone import (Cs, CS_ARCH_ARM, CS_MODE_THUMB,
                          CS_MODE_LITTLE_ENDIAN)
except Exception as exc:                                  # pragma: no cover
    print("FATAL: 无法导入 capstone：%r" % (exc,))
    sys.exit(2)


def _new_md(skipdata=False, detail=True):
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_LITTLE_ENDIAN)
    md.detail = detail
    md.skipdata = skipdata
    return md


MD = _new_md(skipdata=False)
MD_SKIP = _new_md(skipdata=True)

DATA = b""
N = 0


def decode(addr):
    """在 addr（偶数、镜像内）处解码单条指令；无法解码返回 None。"""
    off = addr - BASE
    if off < 0 or off + 2 > N:
        return None
    for ins in MD.disasm(DATA[off:off + 4], addr, count=1):
        return ins
    return None


def dis(addr, length):
    """带 skipdata 的线性反汇编（仅用于显示，数据会显示为 .byte）。"""
    off = addr - BASE
    if off < 0:
        return []
    return list(MD_SKIP.disasm(DATA[off:off + length], addr))


def lit_ldr_info(ins):
    """16 位 `ldr rt,[pc,#imm]` → (reg_name, literal_addr, value)；否则 None。"""
    if ins.mnemonic != "ldr" or ins.size != 2:
        return None
    hw = struct.unpack_from("<H", DATA, ins.address - BASE)[0]
    if (hw & 0xF800) != 0x4800:
        return None
    rt = (hw >> 8) & 0x7
    imm8 = hw & 0xFF
    tgt = ((ins.address + 4) & ~3) + imm8 * 4
    if not (BASE <= tgt < END - 3):
        return None
    return "r%d" % rt, tgt, struct.unpack_from("<I", DATA, tgt - BASE)[0]


def scan_all_insns():
    """对镜像每个半字偏移做一次保守解码（数据也会被解码，故引用检查偏保守）。"""
    out = []
    for off in range(0, N - 2, 2):
        a = BASE + off
        ins = decode(a)
        if ins is not None:
            out.append(ins)
    return out


def build_call_index(all_ins):
    """BL（32 位 imm）调用点索引：target -> [call sites]。"""
    calls = collections.defaultdict(list)
    for ins in all_ins:
        if ins.mnemonic == "bl" and ins.size == 4 and ins.operands \
                and ins.operands[0].type == 2:
            calls[ins.operands[0].imm].append(ins.address)
    return {k: sorted(set(v)) for k, v in calls.items()}


def build_branch_refs(all_ins):
    """所有直接分支/调用 (site, target) 列表（b/bcc/cbz/cbnz/bl），偏保守。"""
    refs = []
    for ins in all_ins:
        if not ins.operands or ins.operands[0].type != 2:
            continue
        m = ins.mnemonic
        if m in ("bl", "b", "cbz", "cbnz") or (m.startswith("b") and m != "bkpt"):
            refs.append((ins.address, ins.operands[0].imm))
    return refs


def build_ptr_words():
    """全镜像 4 字节窗口（每 2 字节起点）→ 出现偏移列表；只保留指向镜像内的值。"""
    words = collections.defaultdict(list)
    for off in range(0, N - 3, 2):
        v = struct.unpack_from("<I", DATA, off)[0]
        if BASE <= v < END:
            words[v].append(BASE + off)
        elif v & 1 and BASE <= (v & ~1) < END:
            words[v].append(BASE + off)
    return words


def build_ldr_refs(all_ins):
    """所有 16 位 ldr(literal) 指令 → (site, reg, literal_addr, value)。"""
    out = []
    for ins in all_ins:
        info = lit_ldr_info(ins)
        if info is None:
            continue
        rt, tgt, val = info
        out.append((ins.address, rt, tgt, val))
    return out


def word_at(addr):
    return struct.unpack_from("<I", DATA, addr - BASE)[0]


def fmt_ins(ins):
    return "0x%08X  %-8s %-30s ; %s" % (
        ins.address, ins.mnemonic, ins.op_str,
        DATA[ins.address - BASE:ins.address - BASE + ins.size].hex(" "))


# ---------------------------------------------------------------------------
# 3. 主流程
# ---------------------------------------------------------------------------
def main():
    global DATA, N

    R.header("V860IIIS_V1.1.bin 平台/同步字函数独立审计（只读固件）")
    if not os.path.exists(IMAGE_PATH):
        R.line("FATAL: 找不到固件 %s" % IMAGE_PATH)
        R.save(LOG_PATH)
        return 2
    with open(IMAGE_PATH, "rb") as fh:
        DATA = fh.read()
    N = len(DATA)

    R.line("镜像 : %s" % IMAGE_PATH)
    R.line("大小 : %d (0x%X)" % (N, N))

    all_ins = scan_all_insns()
    calls = build_call_index(all_ins)
    branch_refs = build_branch_refs(all_ins)
    ldr_refs = build_ldr_refs(all_ins)
    ptr_words = build_ptr_words()

    vectors = []
    for i in range(16):
        w = struct.unpack_from("<I", DATA, i * 4)[0]
        vectors.append(w)

    # ---------------- §1 固件身份 / 基址 / 向量表 ----------------
    R.header("1. 固件身份、镜像基址与向量表")

    R.check(N == IMAGE_SIZE, "镜像大小 %d == 0x%X（79300）" % (N, IMAGE_SIZE))
    sha = hashlib.sha256(DATA).hexdigest()
    R.check(sha == IMAGE_SHA256, "sha256 == %s" % sha)

    sp = vectors[0]
    reset = vectors[1]
    R.check(0x20000000 <= sp < 0x20002000 and sp % 8 == 0,
            "向量[0] 初值 SP = %s（RAM 内、8 字节对齐）" % hx(sp))
    R.check(sp == SP_INIT, "向量[0] == %s（与 __main 桩内字面量一致）" % hx(SP_INIT))
    R.check(reset == RESET_VEC and (reset & 1) == 1,
            "向量[1] Reset = %s（Thumb 位=1）" % hx(reset))

    vec_bad = []
    for i in range(2, 16):
        w = vectors[i]
        if w == 0:
            continue
        if (w & 1) == 0 or not (BASE <= (w & ~1) < END):
            vec_bad.append((i, w))
    R.check(not vec_bad, "向量[2..15] 非零项全部落在镜像内且为 Thumb 地址：坏项=%r" % (vec_bad,))
    R.info("向量[2..15] = %s" % " ".join("v%d=%s" % (i, hx(vectors[i])) for i in range(2, 16)))
    R.check(END == 0x080165C4, "镜像末端 = %s（= base + 0x135C4）" % hx(END))

    # Reset 处理函数（base+0xD4）与 __main 桩（base+0xC0）自洽反推 base
    reset_code = dis(BASE + 0xD4, 0x0C)
    reset_ok = (
        len(reset_code) >= 4
        and reset_code[0].mnemonic == "ldr" and reset_code[1].mnemonic in ("blx", "bl")
        and reset_code[2].mnemonic == "ldr" and reset_code[3].mnemonic == "bx"
    )
    R.check(reset_ok, "Reset 桩 base+0xD4：ldr r0,[pc]; blx r0; ldr r0,[pc]; bx r0")
    if reset_ok:
        r_lit0 = lit_ldr_info(reset_code[0])[1]
        r_lit1 = lit_ldr_info(reset_code[2])[1]
        r_target0 = word_at(r_lit0)
        r_target1 = word_at(r_lit1)
        R.check((r_target0 & 1) == 1 and BASE <= (r_target0 & ~1) < END,
                "Reset 桩第一个字面量 %s = %s（SystemInit，Thumb）" % (hx(r_lit0), hx(r_target0)))
        R.check(r_target1 == (BASE + 0xC0) | 1,
                "Reset 桩第二个字面量 %s = %s（__main = base+0xC0|1）"
                % (hx(r_lit1), hx(r_target1)))

    main_stub = dis(BASE + 0xC0, 0x10)
    stub_bytes = DATA[0xC0:0xCC]
    R.check(stub_bytes == bytes.fromhex("0348854600f07cf900480047"),
            "__main 桩 base+0xC0 字节 = %s" % stub_bytes.hex(" "))
    if main_stub and main_stub[0].mnemonic == "ldr":
        m_lit = lit_ldr_info(main_stub[0])[1]
        m_val = word_at(m_lit)
        R.check(m_val == sp,
                "__main 桩 ldr 字面量 %s = %s == 向量[0] SP（base 反推：0x%08X-0xC0=%s）"
                % (hx(m_lit), hx(m_val), BASE + 0xC0, hx(BASE)))
    R.info("=> 结论：镜像基址 base = %s，镜像范围 [%s, %s)" % (hx(BASE), hx(BASE), hx(END)))

    # ---------------- §2 内核判定 ----------------
    R.header("2. 内核判定：Cortex-M0 / ARMv6-M（Thumb-1）？")

    # 2.1 干净函数体统计（最可信的口径）：
    #     函数入口 = 向量表 + 全镜像 BL 目标 + veneer 目标；从入口线性解码到首个
    #     返回/无条件分支为止（跳过数据），统计 4 字节指令。
    veneer_targets = []
    for a in range(BASE, END - 4, 2):
        i1 = decode(a)
        if i1 is None:
            continue
        info = lit_ldr_info(i1)
        if info is None:
            continue
        i2 = decode(a + i1.size)
        if i2 is None or i2.mnemonic not in ("bx", "blx") or not i2.operands \
                or i2.operands[0].type != 1:
            continue
        if MD.reg_name(i2.operands[0].reg) != info[0]:
            continue
        t = info[2] & ~1
        if BASE <= t < END:
            veneer_targets.append((a, i2.mnemonic, t))
    vec_targets = [w & ~1 for w in vectors[1:] if w and BASE <= (w & ~1) < END]
    entry_set = set(calls.keys()) | set(vec_targets) | set(t[2] for t in veneer_targets)
    entry_set = {e for e in entry_set if BASE <= e < END and (e & 1) == 0}
    R.info("函数入口集合：向量 %d + veneer %d + BL 目标 → 去重 %d 个"
           % (len(vec_targets), len(veneer_targets), len(entry_set)))
    R.info("veneer（ldr rX,[pc]; bx/blx rX）：%s"
           % ", ".join("0x%08X→0x%08X" % (a, t) for a, m, t in veneer_targets))

    def linear_body(entry, max_bytes=0x800):
        insns = []
        a = entry
        while a < min(END, entry + max_bytes):
            ins = decode(a)
            if ins is None:
                break
            insns.append(ins)
            if ins.mnemonic == "bx" or ins.mnemonic == "b" \
                    or (ins.mnemonic == "pop" and "pc" in ins.op_str):
                break
            a += ins.size
        return insns

    body_stats = {"insn": 0, "bl": 0, "four": 0, "four_nonbl": [],
                  "undecodable": 0}
    body_ranges = []
    for e in sorted(entry_set):
        body = linear_body(e)
        if not body:
            body_stats["undecodable"] += 1
            continue
        body_ranges.append((e, body[-1].address + body[-1].size))
        for ins in body:
            body_stats["insn"] += 1
            if ins.size == 4:
                body_stats["four"] += 1
                if ins.mnemonic.startswith("bl"):
                    body_stats["bl"] += 1
                else:
                    body_stats["four_nonbl"].append((ins.address, ins.mnemonic))
    R.check(body_stats["insn"] > 8000,
            "干净函数体（入口→首个返回/分支）共 %d 条指令，覆盖 %d 个函数"
            % (body_stats["insn"], len(body_ranges)))
    R.check(body_stats["four_nonbl"] == [],
            "干净函数体中 32 位非 BL 指令 = %d（期望 0；4 字节共 %d 条，BL %d 条）"
            % (len(body_stats["four_nonbl"]), body_stats["four"], body_stats["bl"]))
    if body_stats["four"]:
        R.info("函数体 4 字节指令中 BL 占比 = %.1f%%（%d/%d）"
               % (100.0 * body_stats["bl"] / body_stats["four"],
                  body_stats["bl"], body_stats["four"]))

    # 2.2 全镜像逐偏移线性反汇编（skipdata）：数据会被误判，需要给可信度
    all_lin = list(MD_SKIP.disasm(DATA, BASE))
    tot = len(all_lin)
    four = [i for i in all_lin if i.size == 4]
    four_bl = [i for i in four if i.mnemonic.startswith("bl")]
    four_nonbl = [i for i in four if not i.mnemonic.startswith("bl")]
    skipped = sum(1 for i in all_lin if i.mnemonic == ".byte")
    in_body = sum(1 for i in four_nonbl
                  if any(lo <= i.address < hi for lo, hi in body_ranges))
    R.info("全镜像线性反汇编：指令 %d 条（skipdata .byte %d 条），4 字节 %d 条"
           % (tot, skipped, len(four)))
    R.check(not in_body,
            "全镜像 %d 条 32 位非 BL 解码中，落在任何真实函数体内的 = %d 条"
            "（=> 全部是数据/常量池被误判，不可执行）"
            % (len(four_nonbl), in_body))
    sig = ("movw", "movt", "ldrd", "strd", "ubfx", "sbfx", "bfi", "bfc",
           "clz", "sdiv", "udiv", "mla", "mls", "rbit")
    sig_hits = [i for i in all_lin
                if any(i.mnemonic == s or i.mnemonic.startswith(s + ".")
                       for s in sig)]
    sig_in_body = [i for i in sig_hits
                   if any(lo <= i.address < hi for lo, hi in body_ranges)]
    R.info("M4 特征指令（%s）全镜像命中 %d 条：%s"
           % ("/".join(sig), len(sig_hits),
              ", ".join("%s(%s)" % (hx(i.address), i.mnemonic) for i in sig_hits)))
    R.check(sig_in_body == [],
            "真实函数体内 M4 特征 32 位指令 = %d（期望 0）" % len(sig_in_body))
    for i in sig_hits:
        R.info("  %s %-8s %s  上下文: %s" % (
            hx(i.address), i.mnemonic, i.op_str,
            DATA[max(0, i.address - BASE - 8):i.address - BASE + 12].hex(" ")))
    # 4 字节非 BL 分类：协处理器/NEON/数据
    COPROC = ("ldc", "ldc2", "ldc2l", "ldcl", "stc", "stc2", "stc2l",
              "mcr", "mcr2", "mrc", "mrc2", "cdp", "cdp2")
    hw2_only = [i for i in four_nonbl
                if any(i.mnemonic.startswith(p) for p in COPROC)
                or i.mnemonic.startswith("v")]
    R.info("其中 %d/%d 条为协处理器/NEON 编码（任何 Cortex-M 都不可能执行）；"
           "其余 %d 条均为常量表/字面量池误判（见上/见下）"
           % (len(hw2_only), len(four_nonbl), len(four_nonbl) - len(hw2_only)))

    # 2.3 软件除法 __aeabi_uidiv 存在（M0/M3 无硬件除法；M4 会直接编出 udiv）
    udiv_body = []
    a = UDIV
    while a < END:
        ins = decode(a)
        if ins is None:
            break
        udiv_body.append(ins)
        if ins.mnemonic == "bx" or (ins.mnemonic == "pop" and "pc" in ins.op_str):
            break
        a += ins.size
    udiv_has_bl = any(i.mnemonic in ("bl", "blx") for i in udiv_body)
    udiv_has_loop = any(i.operands and i.operands[0].type == 2
                        and i.operands[0].imm <= i.address for i in udiv_body
                        if i.mnemonic.startswith("b"))
    R.check(len(udiv_body) > 10 and not udiv_has_bl and udiv_has_loop,
            "0x%08X 处是自包含软件除法（%d 条指令、无 BL、含回边循环）"
            % (UDIV, len(udiv_body)))
    R.check(0x080115C0 in calls.get(UDIV, []),
            "同步字函数在 0x080115C0 调用该软件除法（__aeabi_uidiv）")
    R.info("若目标为 Cortex-M4，编译器会用硬件 UDIV 指令而不是 __aeabi_uidiv；"
           "本镜像真实代码中无 udiv/sdiv 指令，且 4 字节非 BL=0 => ARMv6-M(M0)。")
    R.info("=> 内核判定：Cortex-M0 / Thumb-1（ARMv6-M）。Unicorn 用 UC_ARCH_ARM + "
           "UC_MODE_MCLASS 即可（Unicorn 无独立 M0 模式；M4/Thumb-2 模式不适用）。")

    # ---------------- §3 同步字函数 ----------------
    R.header("3. 同步字函数 0x080115B8..0x080115EF（56 B）逐条复核")

    orig = DATA[SYNC_FN - BASE:SYNC_FN - BASE + SYNC_FN_SIZE]
    R.check(orig == ORIG_FUNC, "56 字节与给定原厂值逐字节一致: %s" % orig.hex(" "))
    R.line("      原始 56 字节：")
    for row in range(0, 56, 16):
        R.line("        +0x%02X: %s" % (row, orig[row:row + 16].hex(" ")))

    seq = []
    a = SYNC_FN
    while a < SYNC_FN + SYNC_FN_SIZE:
        ins = decode(a)
        if ins is None:
            break
        if ins.address >= NUMERATOR_VA:
            break
        seq.append(ins)
        a += ins.size
    R.check(len(seq) == 20 and seq[-1].address == 0x080115EA,
            "顺序解码 20 条指令、代码止于 0x080115EA（共 %d 条，末条 %s %s）"
            % (len(seq), seq[-1].mnemonic, seq[-1].op_str))
    R.line("      逐条反汇编（代码 52 B + +0x34 处字面量 4 B）：")
    for ins in seq:
        R.line("        " + fmt_ins(ins))
    R.line("        %s  %-8s %-30s ; %s" % (
        hx(NUMERATOR_VA), "<数据>", "= 50024 (0xC368)",
        DATA[NUMERATOR_VA - BASE:NUMERATOR_VA - BASE + 4].hex(" ")))

    lit_info = lit_ldr_info(seq[0])
    R.check(lit_info is not None and lit_info[1] == NUMERATOR_VA
            and lit_info[2] == NUMERATOR,
            "第 1 条 ldr 的字面量落在 +0x34 = %s，值 = %d (0x%04X)"
            % (hx(NUMERATOR_VA), NUMERATOR, NUMERATOR))
    R.check(seq[1].mnemonic == "push" and seq[1].op_str == "{r4, lr}",
            "第 2 条：push {r4, lr}（保存 LR/寄存器）")
    R.check(seq[2].mnemonic == "adds" and seq[2].op_str == "r1, r0, #1",
            "第 3 条：adds r1, r0, #1（r1 = ID + 1）")
    R.check(seq[3].mnemonic == "mov" and seq[3].op_str == "r0, r2",
            "第 4 条：mov r0, r2（r0 = 50024）")
    bl_seq = [i for i in seq if i.mnemonic == "bl"]
    R.check([i.operands[0].imm for i in bl_seq] == [UDIV, WRITE_REG, WRITE_REG,
                                                    DELAY_US, STROBE, DELAY_US],
            "6 个 BL 目标依次 = uidiv / WriteReg / WriteReg / delay_us / strobe / delay_us")
    R.check(seq[4].mnemonic == "bl" and seq[4].operands[0].imm == UDIV,
            "第 5 条：bl __aeabi_uidiv（sync = 50024 / (ID+1)）")
    R.check(seq[5].mnemonic == "mov" and seq[5].op_str == "r4, r0",
            "第 6 条：mov r4, r0（保存 sync 到 r4）")
    R.check(seq[6].mnemonic == "lsrs" and seq[6].op_str == "r1, r0, #8",
            "第 7 条：lsrs r1, r0, #8（r1 = SYNC1 = sync>>8）")
    R.check(seq[7].mnemonic == "movs" and seq[7].op_str == "r0, #4",
            "第 8 条：movs r0, #4（寄存器号 SYNC1=4）")
    R.check(seq[8].mnemonic == "bl" and seq[8].operands[0].imm == WRITE_REG,
            "第 9 条：bl WriteReg → WriteReg(4, sync>>8)")
    R.check(seq[9].mnemonic == "uxtb" and seq[9].op_str == "r1, r4",
            "第 10 条：uxtb r1, r4（r1 = SYNC0 = sync & 0xFF）")
    R.check(seq[10].mnemonic == "movs" and seq[10].op_str == "r0, #5",
            "第 11 条：movs r0, #5（寄存器号 SYNC0=5）")
    R.check(seq[11].mnemonic == "bl" and seq[11].operands[0].imm == WRITE_REG,
            "第 12 条：bl WriteReg → WriteReg(5, sync & 0xFF)")
    R.check(seq[12].mnemonic == "movs" and seq[12].op_str == "r0, #0x14"
            and seq[13].mnemonic == "bl" and seq[13].operands[0].imm == DELAY_US,
            "第 13-14 条：delay_us(0x14=20)（写 SYNC 后建立时间）")
    R.check(seq[14].mnemonic == "movs" and seq[14].op_str == "r0, #0x33"
            and seq[15].mnemonic == "bl" and seq[15].operands[0].imm == STROBE,
            "第 15-16 条：strobe(0x33)（SCAL 频率合成器校准）")
    R.check(seq[16].mnemonic == "movs" and seq[16].op_str == "r0, #0x7d"
            and seq[17].mnemonic == "lsls" and seq[17].op_str == "r0, r0, #3"
            and seq[18].mnemonic == "bl" and seq[18].operands[0].imm == DELAY_US,
            "第 17-19 条：delay_us(0x7d<<3 = 125*8 = 1000)")
    R.check(seq[19].mnemonic == "pop" and seq[19].op_str == "{r4, pc}",
            "第 20 条：pop {r4, pc}（函数返回）")

    # 50824 唯一性 / LDR 引用
    pat = struct.pack("<I", NUMERATOR)
    hits = []
    i = DATA.find(pat)
    while i >= 0:
        hits.append(BASE + i)
        i = DATA.find(pat, i + 1)
    R.check(hits == [NUMERATOR_VA],
            "50024(0xC368) 四字节模式全镜像只出现 1 次：%s" % [hx(x) for x in hits])
    ldr_to_lit = [x for x in ldr_refs if x[2] == NUMERATOR_VA]
    R.check([x[0] for x in ldr_to_lit] == [SYNC_FN],
            "引用该字面量的 ldr(literal) 指令恰为 0x%08X 一条：%s"
            % (SYNC_FN, [hx(x[0]) for x in ldr_to_lit]))

    # 边界：0x080115F0 是下一函数入口，0x080115F4 不是
    R.check(0x0800CD80 in calls.get(NEXT_FN, []),
            "0x%08X 是真实函数入口：被 0x0800CD80 的 BL 调用（BL→0x%08X）"
            % (NEXT_FN, NEXT_FN))
    next_seq = dis(NEXT_FN, 0x40)
    next_body_end = None
    for ins in next_seq:
        if ins.mnemonic == "bx" or (ins.mnemonic == "pop" and "pc" in ins.op_str):
            next_body_end = ins.address + ins.size
            break
    R.check(next_body_end == 0x0801161A,
            "0x%08X 起函数体线性解码合法，止于 0x08011618 的 bx lr" % NEXT_FN)
    R.check(0x0801161A - 0x080115F0 > 0 and
            word_at(0x0801161C) == 0x2000045E,
            "0x0801161C 起为下一函数的字面量池（%s=0x2000045E 等 RAM 指针）"
            % hx(0x0801161C))
    R.check(0x0800CD80 in calls.get(NEXT_FN, []) and not calls.get(0x080115F4),
            "0x080115F4 不是任何 BL/向量目标（Lead 初勘的 0x080115F4 为笔误，"
            "应为 0x080115F0；与 V1C 一样是 +0x38 边界）")
    refs_into = [(s, t) for s, t in branch_refs
                 if SYNC_FN <= t < SYNC_FN_END]
    R.check(refs_into == [(s, SYNC_FN) for s in sorted(CALL_SITES)],
            "同步字函数范围内只有入口 0x%08X 被 5 个 BL 指向，"
            "内部无任何分支/调用目标：%s"
            % (SYNC_FN, [(hx(s), hx(t)) for s, t in refs_into]))
    ptr_into = [(v, o) for v, offs in ptr_words.items()
                if SYNC_FN <= v < SYNC_FN_END for o in offs]
    R.check(not ptr_into, "没有字面量池指针指向同步字函数范围：%r" % (ptr_into,))

    # ---------------- §4 辅助函数语义 ----------------
    R.header("4. 辅助函数语义复核（udiv / WriteReg / strobe / delay_us）")

    body = []
    a = UDIV
    while a < END:
        ins = decode(a)
        if ins is None:
            break
        body.append(ins)
        if ins.mnemonic == "bx" or (ins.mnemonic == "pop" and "pc" in ins.op_str):
            break
        a += ins.size
    R.check(len(body) == 22
            and not any(i.mnemonic in ("bl", "blx") for i in body),
            "__aeabi_uidiv @0x%08X：%d 条自包含指令、无 BL，r0=商/r1=余"
            % (UDIV, len(body)))
    R.line("      uidiv 体（前 6 条）：")
    for ins in body[:6]:
        R.line("        " + fmt_ins(ins))

    wr_body = dis(WRITE_REG, 0x10)
    wr_ok = (len(wr_body) >= 6
             and wr_body[0].mnemonic == "push"
             and wr_body[1].mnemonic == "movs" and wr_body[1].op_str == "r2, #1"
             and wr_body[2].mnemonic == "add"
             and wr_body[3].mnemonic == "bl"
             and wr_body[3].operands[0].imm == WRITE_BUF
             and wr_body[4].mnemonic == "pop")
    R.check(wr_ok,
            "WriteReg @0x%08X：push {r0,r1,r4,lr}; movs r2,#1; add r1,sp,#4; "
            "bl 0x%08X（写 1 字节数据）" % (WRITE_REG, WRITE_BUF))
    R.line("      WriteReg 体：")
    for ins in wr_body[:6]:
        R.line("        " + fmt_ins(ins))

    st_body = dis(STROBE, 0x18)
    st_ok = (len(st_body) >= 9
             and st_body[0].mnemonic == "push"
             and st_body[1].mnemonic == "mov" and st_body[1].op_str == "r4, r0"
             and st_body[2].mnemonic == "bl" and st_body[2].operands[0].imm == CS_LOW
             and st_body[3].mnemonic == "mov" and st_body[3].op_str == "r0, r4"
             and st_body[4].mnemonic == "bl" and st_body[4].operands[0].imm == SPI_XFER
             and st_body[6].mnemonic == "bl" and st_body[6].operands[0].imm == CS_HIGH
             and st_body[8].mnemonic == "pop" and "pc" in st_body[8].op_str)
    R.check(st_ok,
            "strobe @0x%08X：push {r4,lr}; mov r4,r0; bl CS_LOW; mov r0,r4; "
            "bl SPI_XFER; ...; bl CS_HIGH; pop {r4,pc}（把命令字节发给 RF）" % STROBE)
    R.line("      strobe 体：")
    for ins in st_body[:9]:
        R.line("        " + fmt_ins(ins))

    dl_body = dis(DELAY_US, 0x30)
    nop_cnt = sum(1 for i in dl_body if i.mnemonic == "nop")
    dl_ok = (len(dl_body) >= 12
             and dl_body[0].mnemonic == "subs" and dl_body[0].op_str == "r0, r0, #1"
             and dl_body[1].mnemonic == "uxth"
             and nop_cnt >= 10
             and any(i.mnemonic == "blo" for i in dl_body)
             and dl_body[-1].mnemonic == "bx")
    R.check(dl_ok,
            "delay_us @0x%08X：subs r0,#1; uxth; %d 个 nop 内层循环; bx lr（软件延时）"
            % (DELAY_US, nop_cnt))
    for name, addr in (("uidiv", UDIV), ("WriteReg", WRITE_REG),
                       ("strobe", STROBE), ("delay_us", DELAY_US)):
        R.check(bool(calls.get(addr)),
                "%s @0x%08X 是真实函数（全镜像 BL 调用点数 %d）"
                % (name, addr, len(calls.get(addr, []))))

    # ---------------- §5 5 个调用点 ----------------
    R.header("5. 同步字函数的 5 个调用点与 settings[6] 入参")

    sync_calls = calls.get(SYNC_FN, [])
    R.check(sync_calls == CALL_SITES,
            "全镜像 BL→0x%08X 的调用点恰为 5 个：%s"
            % (SYNC_FN, [hx(x) for x in sync_calls]))

    # ---- 函数级 CFG：用于证明"基址装载→ID 装载"所有路径上基址寄存器未被改写 ----
    def fn_span(fn_entry, entry_candidates):
        later = [e for e in entry_candidates if e > fn_entry]
        return fn_entry, (later[0] if later else END)

    def build_cfg(lo, hi):
        insns = {}
        a = lo
        while a < hi:
            ins = decode(a)
            if ins is None:
                a += 2
                continue
            insns[a] = ins
            a += ins.size
        succ = {}
        for addr, ins in insns.items():
            nxt = addr + ins.size
            m = ins.mnemonic
            ops = ins.operands
            tgt = ops[0].imm if ops and ops[0].type == 2 else None
            if m in ("bx",) or (m == "pop" and "pc" in ins.op_str):
                s = []
            elif m == "b":
                s = [tgt] if tgt in insns else []
            elif m in ("bl", "blx"):
                s = [nxt] if nxt in insns else []
            elif m in ("cbz", "cbnz") or (m.startswith("b") and m != "bkpt"):
                s = ([tgt] if tgt in insns else []) + \
                    ([nxt] if nxt in insns else [])
            else:
                s = [nxt] if nxt in insns else []
            succ[addr] = s
        return insns, succ

    def path_register_writes(lo, hi, start, target, reg_name):
        """返回同时"从 start 可达"且"能到达 target"的指令里对 reg_name 的写点。"""
        insns, succ = build_cfg(lo, hi)
        fwd, stack = set(), [start]
        while stack:
            x = stack.pop()
            if x in fwd or x not in insns:
                continue
            fwd.add(x)
            stack.extend(succ.get(x, []))
        rev = collections.defaultdict(list)
        for u, vs in succ.items():
            for v in vs:
                rev[v].append(u)
        bwd, stack = set(), [target]
        while stack:
            x = stack.pop()
            if x in bwd or x not in insns:
                continue
            bwd.add(x)
            stack.extend(rev.get(x, []))
        path = fwd & bwd
        writes = []
        for x in sorted(path):
            if x in (start, target):
                continue
            ins = insns[x]
            try:
                _, wregs = ins.regs_access()
            except Exception:                            # pragma: no cover
                wregs = []
            if any(MD.reg_name(r) == reg_name for r in wregs):
                writes.append(ins)
        return writes, len(path)

    for site in CALL_SITES:
        R.line("")
        R.line("  -- 调用点 0x%08X --" % site)
        fn_entry = CALL_SITE_FN[site]
        # 所在函数入口：'最大已知入口 <= site'
        entry_candidates = sorted(entry_set)
        derived_fn = max([e for e in entry_candidates if e <= site], default=None)
        R.check(derived_fn == fn_entry,
                "所在函数入口 = 0x%08X（= 最大的已知 BL 目标入口 ≤ 调用点）"
                % fn_entry)
        lo, hi = fn_span(fn_entry, entry_candidates)
        R.info("所在函数区间（入口~下一个已知入口）：[0x%08X, 0x%08X)" % (lo, hi))
        # 取 ID 的指令
        id_load_addr, base_reg, base_load_addr = CALL_SITE_ID_LOAD[site]
        id_ins = decode(id_load_addr)
        R.check(id_ins is not None and id_ins.mnemonic == "ldrb"
                and id_ins.op_str == "r0, [%s, #6]" % base_reg,
                "调用前一条 = ldrb r0, [%s, #6]（0x%08X）=> r0 = settings[6]"
                % (base_reg, id_load_addr))
        # settings 基址装载
        base_ins = decode(base_load_addr)
        info = lit_ldr_info(base_ins) if base_ins else None
        R.check(info is not None and info[0] == base_reg
                and info[2] == SETTINGS_BASE,
                "0x%08X：ldr %s, =0x%08X（settings 基址）"
                % (base_load_addr, base_reg, SETTINGS_BASE))
        # CFG 数据流：基址装载→ID 装载的所有路径上，基址寄存器不得被改写
        bad, path_len = path_register_writes(lo, hi, base_load_addr + 2,
                                             id_load_addr, base_reg)
        R.check(not bad,
                "CFG 数据流：0x%08X(装载) → 0x%08X(ID) 的 %d 条路径指令上，"
                "%s 无改写（实际改写 %d 处）"
                % (base_load_addr, id_load_addr, path_len, base_reg, len(bad)))
        if bad:
            for ins in bad:
                R.info("    改写点: " + fmt_ins(ins))
        R.info("静态数据流：r0 = *(uint8_t *)(0x%08X + 6) = *(uint8_t *)0x%08X"
               % (SETTINGS_BASE, SETTINGS_BASE + ID_OFFSET))
        # 触发路径：逐条边用"调用点是否位于上游函数区间内"复核
        chain = TRIGGER_PATHS[site]
        edge_ok = True
        for idx in range(len(chain) - 1):
            up = chain[idx][1]
            down = chain[idx + 1][1]
            ulo, uhi = fn_span(up, entry_candidates)
            if not any(ulo <= s < uhi for s in calls.get(down, [])):
                edge_ok = False
        R.check(edge_ok, "触发路径每条调用边均由 BL 调用点复核："
                + " ← ".join("%s(0x%08X)" % (n, a) for n, a in chain))
        if site in (0x08005A78, 0x0800C5C2):
            # 这两个改值路径前面有 cmp #6（只对 ID 项改值才重设同步字）
            gate = None
            for off in range(0x30, 0, -2):
                ins = decode(site - off)
                if ins is not None and ins.mnemonic == "cmp" \
                        and ins.op_str.endswith("#6"):
                    gate = (ins.address, ins.op_str)
                    break
            R.check(gate is not None,
                    "改值门条件存在：0x%08X `cmp %s`（仅 C.Fn 项==6/ID 时重设同步字）"
                    % (gate[0], gate[1]) if gate else "未找到 cmp #6 门条件")
        else:
            R.info("该路径无 cmp #6 门条件（无条件重设：RF 初始化/设置恢复/重初始化）")

    # ---------------- §6 WriteReg 寄存器 4/5 写点 ----------------
    R.header("6. 运行期写 CC2500 寄存器 4/5 的全部路径")

    wr_calls = calls.get(WRITE_REG, [])
    R.check(len(wr_calls) == 60,
            "全镜像 BL→WriteReg(0x%08X) 调用点 = %d 个（含初始化表 47 个）"
            % (WRITE_REG, len(wr_calls)))

    def resolve_reg_value(site, reg_name, window=0x20):
        """在 site 之前静态解析某寄存器的立即数；返回 (kind, value, addr)。"""
        insns = []
        a = site - window
        while a < site:
            ins = decode(a)
            if ins is None:
                a += 2
                continue
            if a + ins.size > site:
                break
            insns.append(ins)
            a += ins.size
        for ins in reversed(insns):
            try:
                _, wregs = ins.regs_access()
            except Exception:                            # pragma: no cover
                wregs = []
            if any(MD.reg_name(x) == reg_name for x in wregs):
                if ins.mnemonic in ("movs",) and ins.operands[0].type == 1 \
                        and ins.operands[1].type == 2:
                    return ("imm", ins.operands[1].imm, ins.address)
                if ins.mnemonic == "mov" and ins.operands[0].type == 1 \
                        and ins.operands[1].type == 1:
                    src = MD.reg_name(ins.operands[1].reg)
                    sub = resolve_reg_value(ins.address, src, window=0x10)
                    return (sub[0], sub[1], sub[2])
                return ("dynamic", None, ins.address)
        return ("unknown", None, None)

    reg_hits = []
    unresolved = []
    for site in wr_calls:
        kind, val, at = resolve_reg_value(site, "r0")
        if kind == "imm":
            reg_hits.append((site, val, at))
        else:
            unresolved.append((site, kind, at))
    R.check(not unresolved,
            "60 个 WriteReg 调用点的 r0（寄存器号）全部可静态解析：%r" % (unresolved,))
    reg45 = [(s, v) for s, v, at in reg_hits if v in (SYNC1_REG, SYNC0_REG)]
    R.check(sorted(s for s, v in reg45) == sorted([0x080115CA, 0x080115D2,
                                                    INIT_SYNC1_SITE, INIT_SYNC0_SITE]),
            "写 reg4/reg5 的调用点恰为 4 个：%s"
            % ", ".join("%s(reg=%d)" % (hx(s), v) for s, v in sorted(reg45)))
    R.check(any(s == 0x080115CA for s, v in reg45) and
            any(s == 0x080115D2 for s, v in reg45),
            "同步字函数内写 reg4/5：0x080115CA(reg4=SYNC1)、0x080115D2(reg5=SYNC0)")
    R.check(any(s == INIT_SYNC1_SITE for s, v in reg45) and
            any(s == INIT_SYNC0_SITE for s, v in reg45),
            "初始化表内写 reg4/5：0x%08X(reg4)、0x%08X(reg5)"
            % (INIT_SYNC1_SITE, INIT_SYNC0_SITE))
    # 初始化表缺省值 0xC3/0x68
    kind_c3, val_c3, _ = resolve_reg_value(INIT_SYNC1_SITE, "r1")
    kind_c6, val_c6, _ = resolve_reg_value(INIT_SYNC0_SITE, "r1")
    R.check(kind_c3 == "imm" and val_c3 == 0xC3,
            "初始化表 WriteReg(4, 0x%02X) —— 缺省 SYNC1=0xC3" % (val_c3 or 0))
    R.check(kind_c6 == "imm" and val_c6 == 0x68,
            "初始化表 WriteReg(5, 0x%02X) —— 缺省 SYNC0=0x68" % (val_c6 or 0))
    # 初始化表函数与调用关系
    init_body = dis(INIT_TABLE, 0x10)
    R.check(init_body and init_body[0].mnemonic == "push",
            "CC2500 初始化表函数入口 0x%08X（push {r3,r4,r5,lr}）" % INIT_TABLE)
    R.check(calls.get(INIT_TABLE) == [RF_INIT_TABLE_CALL],
            "初始化表函数只被 RF 初始化函数在 0x%08X 调用一次" % RF_INIT_TABLE_CALL)
    R.check(RF_INIT_TABLE_CALL < RF_INIT_SYNC_CALL,
            "RF 初始化函数内顺序：0x%08X 先写初始化表（0xC3/0x68），"
            "0x%08X 后写同步字（覆盖）" % (RF_INIT_TABLE_CALL, RF_INIT_SYNC_CALL))
    # 初始化表覆盖寄存器集合 0..0x2E
    init_sites = [s for s in wr_calls if INIT_TABLE <= s < 0x0801384C]
    init_regs = []
    for s in init_sites:
        kind, val, at = resolve_reg_value(s, "r0")
        init_regs.append(val)
    R.check(len(init_sites) == 47 and sorted(init_regs) == list(range(0x2F)),
            "初始化表共 47 次 WriteReg，寄存器号恰为 0x00..0x2E 各一次")
    # 旁路检查：底层写函数 0x08013A00 的直接调用点
    wb_calls = calls.get(WRITE_BUF, [])
    R.info("底层写 0x%08X 的直接调用点：%s"
           % (WRITE_BUF, [hx(x) for x in wb_calls]))
    wb_ok = True
    wb_detail = []
    for s in wb_calls:
        kind, val, at = resolve_reg_value(s, "r0", window=0x10)
        wb_detail.append((s, kind, val))
        if kind == "imm" and val in (SYNC1_REG, SYNC0_REG):
            wb_ok = False
    R.check(wb_ok,
            "底层写 0x%08X 的直接调用点里没有 reg4/reg5：%r"
            % (WRITE_BUF, [(hx(s), k, v) for s, k, v in wb_detail]))
    # 0x08013920（写 0x7F 命令）与 WriteReg 内部点
    R.check(0x08013920 in wb_calls and 0x0801392C in wb_calls,
            "底层写调用点包含 WriteReg 内部 0x0801392C 与 0x08013920(0x7F)")
    R.info("=> 结论：运行期只有【同步字函数 0x080115B8】与【初始化表 0x%08X】写 "
           "SYNC1/SYNC0；上电/重初始化时先写 0xC3/0x68，随后必被同步字函数覆盖；"
           "未发现第三处运行期写入。" % INIT_TABLE)

    # ---------------- §7 代码洞 ----------------
    R.header("7. 注入代码洞：全镜像 0x00/0xFF 连续区间 + 引用检查")

    def find_runs(byte, min_len=16):
        runs = []
        i = 0
        while i < N:
            if DATA[i] == byte:
                j = i
                while j < N and DATA[j] == byte:
                    j += 1
                if j - i >= min_len:
                    runs.append((BASE + i, j - i))
                i = j
            else:
                i += 1
        return runs

    runs = [("00", va, ln) for va, ln in find_runs(0x00)]
    runs += [("FF", va, ln) for va, ln in find_runs(0xFF)]
    R.info("全镜像 0x00/0xFF 连续 ≥16 字节区间共 %d 个" % len(runs))
    R.info("0xFF（擦除态）区间：%d 个 => 镜像内无任何擦除态空闲区"
           % len(find_runs(0xFF)))
    R.check(len(find_runs(0xFF)) == 0,
            "镜像内没有 ≥16B 的 0xFF 擦除态区间（无 V1C/III-C 式大死区）")

    # 引用检查数据结构
    def refs_for_range(lo, hi):
        br = [(s, t) for s, t in branch_refs if lo <= t < hi]
        ldr_t = [(s, r, t) for s, r, t, v in ldr_refs if lo <= t < hi]
        ptr = []
        for v, offs in ptr_words.items():
            base_v = v & ~1
            if lo <= base_v < hi:
                for o in offs:
                    ptr.append((o, v))
        vec = [(i, w) for i, w in enumerate(vectors) if w and lo <= (w & ~1) < hi]
        in_body = [(s, e) for s, e in body_ranges if not (e <= lo or s >= hi)]
        return br, ldr_t, ptr, vec, in_body

    # 被引用数据指针（用于判断洞是否位于数据表内部）
    data_ptr_vals = sorted({v for v in ptr_words if 0x08014000 <= (v & ~1) < END})

    def owner_span(addr):
        prev = None
        nxt = None
        for v in data_ptr_vals:
            if v <= addr:
                prev = v
            elif nxt is None:
                nxt = v
        return prev, nxt

    cave_rows = []
    for kind, va, ln in runs:
        br, ldr_t, ptr, vec, in_body = refs_for_range(va, va + ln)
        prev, nxt = owner_span(va)
        span = ""
        if prev is not None and nxt is not None and nxt - prev <= 0x400:
            span = "位于被引用数据块 [0x%08X,0x%08X) 内部" % (prev, nxt)
        safe = (not br and not ldr_t and not ptr and not vec and not in_body)
        cave_rows.append(dict(kind=kind, va=va, ln=ln, br=br, ldr=ldr_t,
                              ptr=ptr, vec=vec, in_body=in_body, span=span,
                              safe=safe))
        R.line("  洞 %s 0x%08X 长度 %3d B：分支/调用指向=%d 个，字面量读取目标=%d 个，"
               "指针值=%d 个，向量=%d 个，真实函数体重叠=%d 个%s"
               % (kind, va, ln, len(br), len(ldr_t), len(ptr), len(vec),
                  len(in_body), ("；" + span) if span else ""))

    # 逐一验证 Lead 的候选（预期结论：都不满足"静态可证安全"）
    R.line("")
    R.line("  Lead 初勘候选逐一验证：")
    # 0x08014060
    def zero_run_at(addr):
        """addr 处（含）向前后量出的连续 0x00 长度。"""
        off = addr - BASE
        if not (0 <= off < N) or DATA[off] != 0:
            return 0
        lo = off
        while lo > 0 and DATA[lo - 1] == 0:
            lo -= 1
        hi = off
        while hi < N and DATA[hi] == 0:
            hi += 1
        return hi - lo
    runlen_14060 = zero_run_at(0x08014060)
    ref_14060 = [o for v, offs in ptr_words.items() if (v & ~1) == 0x08014060
                 for o in offs]
    R.check(runlen_14060 < 16 and len(ref_14060) >= 1,
            "候选 0x08014060：所在连续 0x00 仅 %d B（<16B，不满足洞条件）；"
            "且被指针 0x08014060 引用 %d 处 => 不安全（数据表指针目标）"
            % (runlen_14060, len(ref_14060)))
    # 0x08014B6E
    run_14b6e = [c for c in cave_rows if c["va"] == 0x08014B6E]
    c14 = run_14b6e[0] if run_14b6e else None
    R.check(c14 is not None
            and not (c14["br"] or c14["ldr"] or c14["ptr"] or c14["vec"]
                     or c14["in_body"]),
            "候选 0x08014B6E：连续 %d B 全 0、无任何直接引用；但位于图形/查表数据区"
            "%s => 不能证明未被表索引读取，不安全"
            % (c14["ln"] if c14 else 0,
               ("（" + c14["span"] + "）") if c14 and c14["span"] else ""))
    # 0x080162B9
    run_162b9 = [c for c in cave_rows if c["va"] == 0x080162B9]
    c16 = run_162b9[0] if run_162b9 else None
    if c16 is None:
        R.check(False, "未找到 0x080162B9 的零区")
    else:
        LO16, HI16 = c16["va"], c16["va"] + c16["ln"]
        ldr_in16 = [(s, r, t) for s, r, t, v in ldr_refs if LO16 <= t < HI16]
        ldr_in_code = [x for x in ldr_in16
                       if any(bl <= x[0] < bh for bl, bh in body_ranges)]
        ptr_in16 = [(o, v) for v, offs in ptr_words.items()
                    for o in offs if LO16 <= (v & ~1) < HI16]
        R.check(not ldr_in_code,
                "候选 0x080162B9：连续 %d B 全 0；无任何分支/调用目标、无真实函数体重叠；"
                "%d 个 ldr 伪命中全部位于数据区（非真实代码）"
                % (c16["ln"], len(ldr_in16)))
        for s, r, t in ldr_in16:
            in_code = any(bl <= s < bh for bl, bh in body_ranges)
            R.info("    ldr 伪命中 0x%08X -> 0x%08X（值=0x%08X，site 在真实代码中=%s）"
                   % (s, t, word_at(t), in_code))
        R.check(len(ptr_in16) == 1 and (ptr_in16[0][1] & ~1) == c16["va"] + c16["ln"] - 1,
                "候选 0x080162B9：存在 1 个字面量指针 0x%08X 指向本区间最后 1 字节"
                "（= 紧邻的下一个被引用表首字节），整段改写会破坏该表 => 不安全"
                % (ptr_in16[0][1] if ptr_in16 else 0))
        for o, v in ptr_in16:
            R.info("    指针存储于 0x%08X，值 0x%08X" % (o, v))
        R.info("  => 该零区是数据表（亮度/曲线 LUT）尾部；末尾字节 0x%08X 被真实指针"
               "引用，不能整段覆盖。扣除该字节后即使取入口 0x080162BA 也只有 "
               "%d B，且仍属于数据表内部、无法静态证明不被表索引读到。"
               % (c16["va"] + c16["ln"] - 1, c16["ln"] - 2))
        R.info("  => 结论：0x080162B9 属「有条件可用/默认不用」，不是静态可证安全洞。")
    # 向量保留槽位
    R.line("")
    R.info("向量表保留槽位 0x08003010..0x0800302B（槽 4..10，ARMv6-M 的 Reserved "
           "异常，硬件永不取用）也满足 ≥16B 全 0 且零引用：")
    R.check(any(va == 0x08003010 for _, va, _ in runs),
            "0x08003010 处存在 28B 连续 0x00（向量槽 4..10）")
    R.check(any(va == 0x08003010 and ln == 28 for _, va, ln in runs),
            "该区间长度 28B，位于向量表 0x08003010..0x0800302B")
    R.info("  => 这是全镜像唯一「确定不会被运行期读取」的零区（硬件只在异常发生时读"
           "对应向量槽，Reserved 异常不存在）；缺点是只有 28B 且改的是向量表。")
    R.info("  => 但本任务（同步字审计）不需要注入代码：同步字函数 56B 原位即可改写，"
           "因此本报告结论为「代码洞：无静态可证安全区，本补丁不需要注入」。")

    R.line("")
    R.info("安全性排序（详见 SYNC_AUDIT.md §7）：")
    R.info("  1) 0x08003010（28B，向量保留槽，零引用、确定不读取；仅 28B，改向量表）")
    R.info("  2) 0x080162BA（最多 110B，LUT 尾部零区，末尾字节被指针引用，默认不用）")
    R.info("  3) 0x08014B6E / 其余 16~41B 零区（图形/查表数据内部，明确不安全）")
    R.info("  4) 0x08014060（仅 8B 且被指针引用，不满足条件）")
    R.check(not any(c["safe"] and not (0x08003000 <= c["va"] < 0x08003040)
                    and not c["span"] for c in cave_rows),
            "除向量保留槽位外，没有「零引用 + 不在任何被引用数据块内」的 ≥16B 代码洞"
            "（=> 本镜像没有 V1C 0x08017944 / V860III-C 0x080179C2 那种大死区）")

    # ---------------- §8 Unicorn 调用级验证 ----------------
    R.header("8. Unicorn 调用级 harness（独立脚本、stub udiv/WriteReg/strobe/delay）")
    rc = run_unicorn_checks()
    if rc:
        R.check(False, "Unicorn 部分存在 FAIL 项（见上）")

    # ---------------- 汇总 ----------------
    R.header("汇总")
    R.line("检查项：%d PASS / %d FAIL / 共 %d"
           % (R.passed, R.failed, len(R.results)))
    R.save(LOG_PATH)
    print()
    print("日志已写入：%s" % LOG_PATH)
    return 0 if R.failed == 0 else 1


# ---------------------------------------------------------------------------
# 9. Unicorn harness（内嵌，不 import 任何设备模块）
# ---------------------------------------------------------------------------
def run_unicorn_checks():
    """执行原厂镜像同步字函数，断言公式/写点/顺序。返回 FAIL 计数。"""
    try:
        from unicorn import (Uc, UC_ARCH_ARM, UC_MODE_MCLASS, UC_HOOK_CODE,
                             UC_PROT_ALL, UcError)
        from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1,
                                       UC_ARM_REG_R4, UC_ARM_REG_SP,
                                       UC_ARM_REG_LR, UC_ARM_REG_PC)
    except Exception as exc:                              # pragma: no cover
        R.check(False, "无法导入 unicorn：%r" % (exc,))
        return 1

    fails_before = R.failed
    R.line("内存布局：flash 0x%08X..0x%08X（0x%X，镜像 %d B 位于 0x%08X）；"
           "RAM 0x%08X..0x%08X（0x%X）"
           % (UC_FLASH_BASE, UC_FLASH_BASE + UC_FLASH_SIZE, UC_FLASH_SIZE, N, BASE,
              UC_RAM_BASE, UC_RAM_BASE + UC_RAM_SIZE, UC_RAM_SIZE))
    R.line("SP = 0x%08X（原厂向量[0]）；LR 哨兵 = 0x%08X（落点 0x%08X 放 `b .`）；"
           "R4 哨兵 = 0x%08X" % (UC_SP, UC_LR, UC_RET_SENTINEL, UC_R4_SENTINEL))
    R.line("桩：uidiv(0x%08X)/WriteReg(0x%08X)/strobe(0x%08X)/delay(0x%08X)；"
           "uidiv 用宿主整数除法并回填 r0，其余 `bx lr`"
           % (UDIV, WRITE_REG, STROBE, DELAY_US))

    def run_once(image, ident, stub_udiv):
        uc = Uc(UC_ARCH_ARM, UC_MODE_MCLASS)
        uc.mem_map(UC_FLASH_BASE, UC_FLASH_SIZE, UC_PROT_ALL)
        uc.mem_map(UC_RAM_BASE, UC_RAM_SIZE, UC_PROT_ALL)
        code = bytearray(image)
        for a in (WRITE_REG, STROBE, DELAY_US):
            code[a - BASE:a - BASE + 2] = b"\x70\x47"       # bx lr
        uc.mem_write(UC_FLASH_BASE, bytes(code))
        uc.mem_write(UC_RET_SENTINEL, b"\xfe\xe7")          # b .
        uc.reg_write(UC_ARM_REG_SP, UC_SP)
        uc.reg_write(UC_ARM_REG_LR, UC_LR)
        uc.reg_write(UC_ARM_REG_R4, UC_R4_SENTINEL)
        uc.reg_write(UC_ARM_REG_R0, ident & 0xFF)
        events = []
        udiv = []

        def hook(uc_, address, size, user):
            if address == UC_RET_SENTINEL:
                uc_.emu_stop()
                return
            if address == UDIV:
                r0v = uc_.reg_read(UC_ARM_REG_R0)
                r1v = uc_.reg_read(UC_ARM_REG_R1)
                udiv.append((r0v, r1v))
                if stub_udiv:
                    uc_.reg_write(UC_ARM_REG_R0, (r0v // r1v) if r1v else 0)
                    uc_.reg_write(UC_ARM_REG_PC, uc_.reg_read(UC_ARM_REG_LR))
                return
            if address == WRITE_REG:
                events.append(("WriteReg", uc_.reg_read(UC_ARM_REG_R0),
                               uc_.reg_read(UC_ARM_REG_R1)))
            elif address == STROBE:
                events.append(("strobe", uc_.reg_read(UC_ARM_REG_R0), None))
            elif address == DELAY_US:
                events.append(("delay", uc_.reg_read(UC_ARM_REG_R0), None))

        uc.hook_add(UC_HOOK_CODE, hook)
        state = {"error": None, "returned": False}
        try:
            uc.emu_start(SYNC_FN | 1, 0, 0, UC_MAX_INSNS)
        except UcError as exc:                            # pragma: no cover
            state["error"] = "%s (pc=0x%08X sp=0x%08X lr=0x%08X)" % (
                exc, uc.reg_read(UC_ARM_REG_PC), uc.reg_read(UC_ARM_REG_SP),
                uc.reg_read(UC_ARM_REG_LR))
        pc = uc.reg_read(UC_ARM_REG_PC)
        return dict(uc=uc, events=events, udiv=udiv, error=state["error"],
                    pc=pc, sp=uc.reg_read(UC_ARM_REG_SP),
                    r4=uc.reg_read(UC_ARM_REG_R4))

    R.line("")
    R.line("ID  期望 sync      实际 WriteReg 序列 / delay / strobe")
    R.line("-" * 78)
    for ident in UC_IDS:
        sync = NUMERATOR // (ident + 1)
        expect_seq = [("WriteReg", SYNC1_REG, sync >> 8),
                      ("WriteReg", SYNC0_REG, sync & 0xFF),
                      ("delay", 20, None),
                      ("strobe", 0x33, None),
                      ("delay", 1000, None)]
        res = run_once(DATA, ident, True)
        ev = res["events"]
        R.line("ID=%-3d 0x%04X        %s"
               % (ident, sync,
                  " -> ".join("WriteReg(%d,0x%02X)" % (a, b) if t == "WriteReg"
                              else "%s(%d)" % (t, a) for t, a, b in ev)))
        R.check(res["error"] is None and res["pc"] == UC_RET_SENTINEL,
                "ID=%-2d 执行到 LR 哨兵正常返回（pc=0x%08X）" % (ident, res["pc"]))
        R.check(res["sp"] == UC_SP and res["r4"] == UC_R4_SENTINEL,
                "ID=%-2d 栈平衡（SP=0x%08X）且 r4 由 pop 恢复（0x%08X）"
                % (ident, res["sp"], res["r4"]))
        R.check(res["udiv"] == [(NUMERATOR, ident + 1)],
                "ID=%-2d uidiv 入参 = (50024, %d) => sync=50024//%d=0x%04X"
                % (ident, ident + 1, ident + 1, sync))
        R.check(ev == expect_seq,
                "ID=%-2d 调用序列/参数完全匹配：WriteReg(4,0x%02X) -> WriteReg(5,0x%02X) "
                "-> delay(20) -> strobe(0x33) -> delay(1000)"
                % (ident, sync >> 8, sync & 0xFF))
        # 用真实 uidiv 交叉核对（不 stub）
        real = run_once(DATA, ident, False)
        R.check(real["events"] == ev,
                "ID=%-2d 保留真实 __aeabi_uidiv 时序列与 stub 版一致（除法结果可信）"
                % ident)

    # 单独强调公式与 SYNC1/SYNC0 拆分
    for ident in UC_IDS:
        sync = NUMERATOR // (ident + 1)
        res = run_once(DATA, ident, True)
        w = [e for e in res["events"] if e[0] == "WriteReg"]
        ok = (len(w) == 2 and w[0] == ("WriteReg", 4, sync >> 8)
              and w[1] == ("WriteReg", 5, sync & 0xFF))
        R.check(ok,
                "断言 sync=50024//(ID+1)、SYNC1=sync>>8、SYNC0=sync&0xFF 成立"
                "（ID=%d, sync=0x%04X）" % (ident, sync))
    R.line("")
    R.info("说明：uidiv 用宿主除法等价替换（记录入参 r0/r1 后回填 r0），"
           "WriteReg/strobe/delay 用 `bx lr` 桩 + code hook 记录参数；"
           "另跑一遍保留真实软件除法的用例，二者序列逐项一致。")
    R.info("Unicorn 模式：UC_ARCH_ARM + UC_MODE_MCLASS（同 V1C 审计）；"
           "镜像为纯 Thumb-1+BL，与 MCLASS 兼容。")
    return R.failed - fails_before


if __name__ == "__main__":
    try:
        sys.stdout.reconfigure(encoding="utf-8", errors="replace")
    except Exception:                                     # pragma: no cover
        pass
    sys.exit(main())
