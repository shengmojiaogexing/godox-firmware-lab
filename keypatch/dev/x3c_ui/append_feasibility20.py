# -*- coding: utf-8 -*-
"""追加 §20：挂载点更正到 page 0（主界面快捷面板）+ page-0 版成品 sha。"""
import hashlib
import io
import os

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
P = os.path.join(HERE, "FEASIBILITY.md")
OUTL = os.path.join(HERE, "out")


def sha(p):
    if not os.path.exists(p):
        return "(缺)"
    h = hashlib.sha256()
    with open(p, "rb") as f:
        for c in iter(lambda: f.read(1 << 20), b""):
            h.update(c)
    return h.hexdigest()


FILES = [
    ("A2-p0（先测；不建控件）", os.path.join(OUTL, "X3_C_v1.38_app_ui_A2p0_fallback.bin")),
    ("C-p0 / 主成品（+磁贴）", os.path.join(OUTL, "X3_C_v1.38_app_patch.bin")),
    ("C-p0 + A–E 逐组（KEY=0xD798）",
     os.path.join(ROOT, "keypatch", "out", "X3_C_v1.38_app_keyD798_pergroup.bin")),
    ("A2-p0 + 逐组（KEY=0xD798）",
     os.path.join(ROOT, "keypatch", "out", "X3_C_v1.38_app_keyD798_A2p0.bin")),
]
rows = "\n".join("| %s | `%s` | %d | `%s` |" % (
    n, os.path.relpath(p, ROOT), os.path.getsize(p) if os.path.exists(p) else 0, sha(p))
    for n, p in FILES)

SEC = """
---

## 20. 挂载点更正：主界面快捷面板是 **page 0**（不是 page 1）

### 20.1 证据（UI tick / 冷启动 / 页面构建）
* UI tick `0x08014B28`：
  `0x08014C0E ldrb r1,[r2]; cmp #2 …; cbz r1 → 0x08014C54`；
  **page 0 分支：`0x08014C54 movs r0,#0` → `0x08014C56 bl 0x080345B4`**（本目录 hook 点）；
  page 1 分支：`0x08014C18 cbz r1; 0x08014C1A cmp r1,#1; 0x08014C1E bl 0x0800FE78`。
* main 冷启动块 `0x08034140-0x08034150` 同样 `movs r0,#0; bl 0x080345B4`（=主界面）。
* `FUN_080345B4`（= page 0 处理器，size 1104）内部：
  `0x080349A2 mov r0,sb` → **`0x080349A4 bl 0x080079E8`**（建快捷面板）→ 之后是收尾。
* 快捷面板 builder `FUN_080079E8` 结束时把控件句柄写入 **0x20042740**（见 §19.1），
  其中 **[0] = 根窗口**（`0x08008200 str r5,[r0]`，r5 来自 `0x08007A02 bl FUN_08010ACC` →
  `0x08007A06 mov r5,r0`）。
* 结论：之前 v1.5 把 wrapper/建磁贴挂在 page 1（0x08014C1E / 0x0800FF50）→ 主界面根本不经过
  那段代码，所以"磁贴不出现"；而 A/B/C 的启动行为与 page1 钩子无关。

### 20.2 page-0 版改动（A2-p0 / C-p0）
| 档 | 改动 |
|---|---|
| A2-p0 | `0x08014C1E` 恢复原厂 `fbf72bf9`；`0x08014C56: bl 0x080345B4 → bl wrapper0(0x080F1100)`；wrapper0 = `push{r4,lr}; mov r4,r0; bl poll; mov r0,r4; bl 0x080345B4; pop{r4,pc}`；poll = fallback_poll（0x20042740 句柄表，[5] 锁/[7] 设置页签长按 ≥5 tick → key_toggle / broadcast_allch） |
| C-p0 | A2-p0 + `0x080349A4: bl 0x080079E8 → bl wrapper1(0x080F1200)`；wrapper1 = `push{r4,lr}; bl builder; bl tiles(0x080F1300); pop{r4,pc}`；tiles 父窗口 = `*(u32*)0x20042740[0]`（判 0 跳过）、创建失败跳过、2 个磁贴 (40,288,128,44)/(176,288,128,44)、12 个原厂 setter + 文本、无图标子控件 |

注入区：poll 0x080F1000、wrapper0 0x080F1100、wrapper1 0x080F1200、tiles 0x080F1300（< 0x080F6FFF）。
**原厂代码改动仅 2~4 字节**（page0 调用点，C-p0 再加 builder 调用点）。

### 20.3 成品 sha256（均 PASS）
| 档 | 文件 | 大小 | sha256 |
|---|---|---|---|
%s

### 20.4 与旧档的关系
§17 的 v1.5 A/B/C（page 1 挂载）**作废**：主界面不经过 page 1，磁贴/轮询不生效；
但其"调用点 wrapper"写法与"0x0800FE78 保持原厂"的教训保留（本页方案沿用同一 wrapper 思路）。
"""

with io.open(P, "a", encoding="utf-8") as f:
    f.write(SEC % rows)
print("appended §20")
