# -*- coding: utf-8 -*-
"""X2T-S V1.4 固件初步侦察：镜像基址、向量表、50024 字面量、同步字函数候选。"""
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
IMG = os.path.join(os.path.dirname(HERE), "firmware", "X2T-S_V1.4.bin")


def main():
    data = open(IMG, "rb").read()
    print("size = %d (0x%X)" % (len(data), len(data)))
    print("md5 = %s" % __import__("hashlib").md5(data).hexdigest())
    print()

    # 1) 向量表解读（按 base 0x08000000）
    base = 0x08000000
    print("== vector table (base 0x%08X) ==" % base)
    for i in range(16):
        v = struct.unpack_from("<I", data, i * 4)[0]
        print("  [%2d] 0x%08X" % (i, v))
    print("  [16..] 0x%08X 0x%08X 0x%08X 0x%08X"
          % struct.unpack_from("<IIII", data, 16 * 4))
    print()

    # 2) 50024 = 0xC368 字面量
    pat = struct.pack("<I", 50024)
    print("== literal 50024 (0xC368) at file offsets ==")
    idx = 0
    while True:
        i = data.find(pat, idx)
        if i < 0:
            break
        print("  offset 0x%05X -> addr 0x%08X" % (i, base + i))
        idx = i + 1
    # 也搜 16 位半字（movw 立即数）
    print("== 50024 low/high halves search ==")
    print("  bytes 68 C3 (LE) at:", [hex(m) for m in _find_all(data, bytes.fromhex("68c3"))][:20])
    print()

    # 3) 文件偏移 0x3000 附近
    print("== first 64 bytes of file ==")
    print(" ", data[:64].hex(" "))
    print("== first 64 bytes at offset 0x3000 ==")
    print(" ", data[0x3000:0x3040].hex(" "))
    print("== first 64 bytes at offset 0x1000 ==")
    print(" ", data[0x1000:0x1040].hex(" "))
    print()

    # 4) 统计函数序言数量（push {...,lr} 2d e9 xx xx / b5 xx）作为代码区参考
    print("== approx count of push{lr} prologues by 4KB page ==")
    for page in range(0, len(data), 0x1000):
        chunk = data[page:page + 0x1000]
        n = 0
        for i in range(0, len(chunk) - 3, 2):
            hw = chunk[i] | (chunk[i + 1] << 8)
            if (hw & 0xFF00) == 0xB500:
                n += 1
            elif hw in (0xE92D,):
                n += 1
        print("  0x%08X-0x%08X: %d" % (base + page, base + page + 0xFFF, n))
    print()

    # 5) 找 __aeabi_uidiv 引用？先找可能的除法函数（SDIV/UDIV 指令 b0 fb ... f0 f?）
    print("== UDIV/SDIV (hw1 & 0xFFF0 == 0xFBB0/FB90) count ==")
    n = 0
    for i in range(0, len(data) - 4, 2):
        hw1 = struct.unpack_from("<H", data, i)[0]
        if (hw1 & 0xFFF0) in (0xFBB0, 0xFB90, 0xFBA0):
            n += 1
    print("  total:", n)
    print()

    # 6) 看 0x0..0x3000 区域的引用目标：哪些值指向 0x08003000 之后
    print("== sample data refs in first 0x3000 ==")
    for i in range(0, 0x3000 - 3, 4):
        v = struct.unpack_from("<I", data, i)[0]
        if 0x0800E000 <= v < 0x08010000:
            print("  off 0x%04X -> 0x%08X" % (i, v))


def _find_all(data, pat):
    out = []
    i = data.find(pat)
    while i >= 0:
        out.append(i)
        i = data.find(pat, i + 1)
    return out


if __name__ == "__main__":
    main()
