#!/usr/bin/env python3
"""Compare the Ghidra function list against the capstone-based list from the Lead.

Inputs
  devices/ad100pro/out/functions.json     (capstone, Lead's analysis)
  devices/ad100pro/out/ghidra/functions.tsv (Ghidra export)

Output
  devices/ad100pro/out/ghidra/compare_lead.md
"""
import json
import os
import sys
from collections import Counter

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))  # -> devices/ad100pro
CAP = os.path.join(ROOT, "out", "functions.json")
GH = os.path.join(ROOT, "out", "ghidra", "functions.tsv")
OUT = os.path.join(ROOT, "out", "ghidra", "compare_lead.md")


def load_cap():
    with open(CAP, "r", encoding="utf-8") as f:
        data = json.load(f)
    starts = []
    for f in data:
        starts.append(int(f["addr"], 16))
    return starts


def load_gh():
    starts, sizes, names = [], {}, {}
    with open(GH, "r", encoding="utf-8") as f:
        for line in f:
            line = line.rstrip("\n")
            if not line:
                continue
            parts = line.split("\t")
            if len(parts) < 3:
                continue
            a = int(parts[0], 16)
            starts.append(a)
            sizes[a] = int(parts[1])
            names[a] = parts[2]
    return starts, sizes, names


def main():
    cap = load_cap()
    gh, sizes, names = load_gh()
    cap_set = set(cap)
    gh_set = set(gh)
    both = cap_set & gh_set
    only_gh = sorted(gh_set - cap_set)
    only_cap = sorted(cap_set - gh_set)

    # near matches: Ghidra start within +-31 bytes of a capstone start
    gh_sorted = sorted(gh_set)
    import bisect

    def near(a, other):
        i = bisect.bisect_left(other, a - 32)
        j = bisect.bisect_right(other, a + 32)
        return other[i:j]

    near_pairs = []
    for a in only_gh:
        ns = near(a, sorted(cap_set))
        if ns:
            near_pairs.append((a, ns[0], a - ns[0]))

    lines = []
    lines.append("# Ghidra vs capstone 函数清单对照\n")
    lines.append(f"- capstone (out/functions.json): **{len(cap)}** 个函数起始地址")
    lines.append(f"- Ghidra (out/ghidra/functions.tsv): **{len(gh)}** 个函数")
    lines.append(f"- 完全相同的起始地址: **{len(both)}**")
    lines.append(f"- 仅 Ghidra 发现: **{len(only_gh)}**")
    lines.append(f"- 仅 capstone 发现: **{len(only_cap)}**")
    lines.append("")
    lines.append("## 仅 Ghidra 发现（前 40 个）")
    lines.append("")
    lines.append("| Ghidra 地址 | 名称 | 大小 | 附近 capstone 函数 | 差 |")
    lines.append("|---|---|---|---|---|")
    near_map = {a: (b, d) for a, b, d in near_pairs}
    for a in only_gh[:40]:
        b, d = near_map.get(a, ("", ""))
        lines.append(f"| 0x{a:08X} | {names.get(a,'')} | {sizes.get(a,'')} | "
                     f"{'0x%08X' % b if b != '' else '(无)'} | {d} |")
    lines.append("")
    lines.append("## 仅 capstone 发现（前 40 个）")
    lines.append("")
    lines.append("| capstone 地址 | 附近的 Ghidra 函数 | 差 |")
    lines.append("|---|---|---|")
    for a in only_cap[:40]:
        ns = near(a, gh_sorted)
        if ns:
            lines.append(f"| 0x{a:08X} | 0x{ns[0]:08X} | {ns[0]-a} |")
        else:
            lines.append(f"| 0x{a:08X} | (无) | |")
    lines.append("")
    # region histogram of only_cap
    hist = Counter((a >> 8) << 8 for a in only_cap)
    lines.append("## 仅 capstone 函数按 256 字节页分布（前 20 页）")
    lines.append("")
    lines.append("| 页 | 数量 |")
    lines.append("|---|---|")
    for page, n in sorted(hist.items(), key=lambda kv: -kv[1])[:20]:
        lines.append(f"| 0x{page:08X} | {n} |")
    lines.append("")
    # near totals
    lines.append(f"- 仅 Ghidra 中能在 capstone 起始地址 ±31 内找到近邻的数量: {len(near_pairs)}")
    lines.append("")

    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w", encoding="utf-8") as f:
        f.write("\n".join(lines))

    print(f"cap={len(cap)} ghidra={len(gh)} both={len(both)} only_gh={len(only_gh)} only_cap={len(only_cap)}")
    print("wrote", OUT)


if __name__ == "__main__":
    main()
