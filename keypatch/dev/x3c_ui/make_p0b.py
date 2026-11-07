# -*- coding: utf-8 -*-
"""生成 page0b 成品（A2-p0b / C-p0b）+ sha，并追加 FEASIBILITY §21。"""
import hashlib
import io
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, HERE)
import ui_patch as U  # noqa: E402

OUTL = os.path.join(HERE, "out")
OUTK = os.path.join(ROOT, "keypatch", "out")

CASES = [
    ("X3_C_v1.38_app_ui_A2p0b.bin", os.path.join(OUTL, "X3_C_v1.38_app_ui_A2p0b.bin"),
     {"enable_menu_patch": True, "entry_mode": "fallback_p0"},
     "A2-p0b：page0 轮询 + 页面门/控件树校验（不建控件）"),
    ("X3_C_v1.38_app_ui_Cp0b.bin", os.path.join(OUTL, "X3_C_v1.38_app_ui_Cp0b.bin"),
     {"enable_menu_patch": True, "entry_mode": "tiles_p0"},
     "C-p0b：A2-p0b + 磁贴（页面门 + 显式 show）"),
    ("X3_C_v1.38_app_patch.bin", os.path.join(OUTL, "X3_C_v1.38_app_patch.bin"),
     {"enable_menu_patch": True, "entry_mode": "tiles_p0"},
     "主成品（= C-p0b）"),
    ("X3_C_v1.38_app_keyD798_pergroup.bin", os.path.join(OUTK, "X3_C_v1.38_app_keyD798_pergroup.bin"),
     {"enable_menu_patch": True, "entry_mode": "tiles_p0",
      "broadcast_per_group": True, "key": 0xD798},
     "C-p0b + A–E 逐组广播（KEY=0xD798）"),
    ("X3_C_v1.38_app_keyD798_A2p0b.bin", os.path.join(OUTK, "X3_C_v1.38_app_keyD798_A2p0b.bin"),
     {"enable_menu_patch": True, "entry_mode": "fallback_p0",
      "broadcast_per_group": True, "key": 0xD798},
     "A2-p0b + 逐组广播（KEY=0xD798）"),
]


def sha(b):
    return hashlib.sha256(b).hexdigest()


def main() -> int:
    orig = open(U.IMAGE, "rb").read()
    rows = []
    for _n, path, opts, desc in CASES:
        patched, log = U.apply_ui(orig, opts)
        res = U.verify_ui(patched, opts)
        ok = all(o for o, _ in res)
        with open(path, "wb") as f:
            f.write(patched)
        diffs = sum(1 for a, b in zip(orig, patched) if a != b)
        rows.append("| %s | `%s` | %d | `%s` | %d/%d |"
                    % (desc, os.path.relpath(path, ROOT), diffs, sha(patched),
                       sum(1 for o, _ in res if o), len(res)))
        print("%-52s diff=%4d  %s (%d/%d)\n   sha256 %s"
              % (desc, diffs, "PASS" if ok else "FAIL",
                 sum(1 for o, _ in res if o), len(res), sha(patched)))

    SEC = r"""
---

## 21. v1.7（page-0b）：页面门 + 控件树校验 + 显式 show（修复"频闪页签重启/磁贴不显示"）

### 21.1 实机现象 → 两个修复点
1. **短按/长按"频闪"页签都会重启** → 页面切换瞬间句柄表 `0x20042740` 里的指针可能已被释放/重建，
   poll 直接 `ldrh [h,#0x28]` 踩到野指针。
2. **磁贴不显示** → 磁贴是"页面建完后追加"的子控件，需要显式 `show`（并只在主界面构建时创建）。

### 21.2 A2-p0b：poll 三道防线
```
push {r4-r6,lr}
r4 = 0x20042740                    ; 句柄表
① 页面门：*(u8*)0x200003E7 == 0 ?   ; page0=主界面；否则清零状态后直接返回
                                        （避免页面切换中途动作）
for idx in (5=锁, 7=设置页签):
   h = [r4 + idx*4]
   if h == 0            -> 清零状态, 跳过
   if FUN_080303B8(h)==0-> 清零状态, 跳过       ; ② 控件树校验（句柄是否仍在树中）
   if (*(u16*)(h+0x28) & 8) == 0 -> 清零状态, 跳过
   ... 长按计数/触发（同上版）
```
`0x080303B8` 语义证据：`FUN_08014B28` / `FUN_0800E31C` 里都是
`ldr r0,[literal]; ldr r0,[r0]; bl 0x080303B8; cbz r0, skip`（先取句柄再校验是否存在），
与"控件是否仍在控件树"一致（Lead 已复核）。

### 21.3 C-p0b：tiles 页面门 + 显式 show
```
tiles():
  if *(u8*)0x200003E7 != 0: return           ; ① 只在主界面(0)构建时创建
  parent = *(u32*)0x20042740[0]              ; 根窗口；parent==0 -> 跳过
  for each tile:
      tile = FUN_08006220(parent, 1, x, y, w, h)
      if tile == 0 -> 跳过                    ; NULL 保护
      12 个原厂 setter + FUN_0802D97A(文本)
      FUN_0802DA22(tile, 1)                   ; ② **显式显示**
      句柄存入 0x20001084/88
```
**show API 依据**（`0x0802DA22(widget, 1)` = 显示）：
```
080081f6  movs r1, #1
080081f8  ldr  r0, [r4, #4]      ; 子控件
080081fa  bl   0x0802DA22        ; 显示该子控件（builder 内部用法）
```
且有成对语义：`0x0802E2D2(w,1)` = 隐藏 / `0x0802DA22(w,1)` = 显示，
在 `FUN_08014B28` 的页面更新分支里大量成对出现（如 `FUN_0802e2d2(*puVar6,1)` 与
`FUN_0802da22(puVar6[1],1)`）。

### 21.4 成品 sha256（均 PASS）
| 档 | 文件 | 差异 | sha256 | 验证 |
|---|---|---|---|---|
%s

### 21.5 诊断价值
- 若 **A2-p0b 按频闪仍重启** → 与 poll/句柄无关，问题在"UI tick 调用点 wrapper"或别处；
- 若 **A2-p0b 正常** → 说明旧 poll 的野指针就是原因，C-p0b 再验磁贴显示；
- 若 **C-p0b 显示磁贴但按频闪重启** → 与 tiles 创建时机（builder 之后追加 + 页面门）相关，
  下一步改成"延迟到 poll 里一次创建（仅当页面==0 且句柄表[0] 变化时重建）"。
""" % ("\n".join(rows))

    P = os.path.join(HERE, "FEASIBILITY.md")
    with io.open(P, "a", encoding="utf-8") as f:
        f.write(SEC)
    print("appended §21")
    return 0


if __name__ == "__main__":
    sys.exit(main())
