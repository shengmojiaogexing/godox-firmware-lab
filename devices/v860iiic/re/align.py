# -*- coding: utf-8 -*-
"""把 V1C 固件与 V860IIIC 固件做局部块对齐，得到 V1C 地址 -> V860 地址的映射。

用法:
  tools\python\python.exe devices\v860iiic\re\align.py [--list addr1,addr2,...]

原理: 把 V1C 按 8 字节块建哈希表（记录所有出现位置），对 V860 每个偏移做
8 字节块匹配，然后用贪心扩展 + 单调约束得到若干 (v1c_off, v860_off, length) 区段；
任意 V1C 地址落在区段内即线性映射；多处匹配时选择与邻居一致（单调）的那个。
"""
import sys
from collections import defaultdict

V1C = r"devices/v1c/firmware/V1C_V2.0.bin"
V860 = r"devices/v860iiic/firmware/V860IIIC_V1.3.bin"
BASE = 0x08003000
BLOCK = 8


def load(p):
    return open(p, "rb").read()


def block_table(data, block):
    tab = defaultdict(list)
    for i in range(0, len(data) - block + 1, block):
        tab[bytes(data[i:i + block])].append(i)
    return tab


def segments(v1c, v860):
    tab = block_table(v1c, BLOCK)
    # 对每个 v860 块位置找匹配的 v1c 块；保留唯一匹配以减少噪声
    anchors = []
    for i in range(0, len(v860) - BLOCK + 1, BLOCK):
        ls = tab.get(bytes(v860[i:i + BLOCK]))
        if ls and len(ls) == 1:
            anchors.append((ls[0], i))
    # 单调链：按 v1c_off 排序后做 LIS（v860_off 递增），得到同源骨架
    import bisect
    anchors.sort()
    # LIS on v860 offsets preserving v1c order (else just monotone v860)
    tails = []
    tail_idx = []
    prev = [-1] * len(anchors)
    for idx, (a, b) in enumerate(anchors):
        pos = bisect.bisect_left(tails, b)
        if pos == len(tails):
            tails.append(b)
            tail_idx.append(idx)
        else:
            tails[pos] = b
            tail_idx[pos] = idx
        prev[idx] = tail_idx[pos - 1] if pos > 0 else -1
    chain = []
    cur = tail_idx[-1] if tail_idx else -1
    while cur >= 0:
        chain.append(anchors[cur])
        cur = prev[cur]
    chain.reverse()
    return chain


def main():
    v1c = load(V1C)
    v860 = load(V860)
    chain = segments(v1c, v860)
    # 合并成区段并扩展匹配长度
    segs = []
    for a, b in chain:
        if segs and a == segs[-1][0] + segs[-1][2] and b == segs[-1][1] + segs[-1][2]:
            segs[-1][2] += BLOCK
        else:
            segs.append([a, b, BLOCK])
    print("segments:", len(segs))
    for i, (a, b, n) in enumerate(segs):
        print("  %3d V1C 0x%08X..0x%08X  V860 0x%08X..0x%08X  len=0x%X  shift=%+d"
              % (i, BASE + a, BASE + a + n - 1, BASE + b, BASE + b + n - 1, n, b - a))
    if "--list" in sys.argv:
        addrs = sys.argv[sys.argv.index("--list") + 1].split(",")
        for s in addrs:
            va = int(s, 0)
            off = va - BASE
            hit = None
            for a, b, n in segs:
                if a <= off < a + n:
                    hit = BASE + b + (off - a)
                    break
            print("MAP 0x%08X -> %s" % (va, "0x%08X" % hit if hit else "NOT FOUND"))

if __name__ == "__main__":
    main()
