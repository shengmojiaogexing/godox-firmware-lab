# -*- coding: utf-8 -*-
"""生成 A2 fallback bin + 追加 §19（句柄表证据 / 长按语义 / sha）。"""
import hashlib
import io
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, HERE)
import ui_patch as U  # noqa: E402

OUTL = os.path.join(HERE, "out")
PATH_A2 = os.path.join(OUTL, "X3_C_v1.38_app_ui_A2_fallback.bin")
PATH_A2PG = os.path.join(ROOT, "keypatch", "out", "X3_C_v1.38_app_keyD798_A2.bin")


def sha(b):
    return hashlib.sha256(b).hexdigest()


def main() -> int:
    orig = open(U.IMAGE, "rb").read()
    res_lines = []
    for path, opts, label in (
            (PATH_A2, {"enable_menu_patch": True, "entry_mode": "fallback"},
             "A2 fallback（复用锁磁贴/设置页签长按；不建控件）"),
            (PATH_A2PG, {"enable_menu_patch": True, "entry_mode": "fallback",
                         "broadcast_per_group": True, "key": 0xD798},
             "A2 fallback + A–E 逐组广播（KEY=0xD798）")):
        patched, log = U.apply_ui(orig, opts)
        res = U.verify_ui(patched, opts)
        ok = all(o for o, _ in res)
        with open(path, "wb") as f:
            f.write(patched)
        diffs = sum(1 for a, b in zip(orig, patched) if a != b)
        print("%s\n  len=%d diff=%d sha256=%s verify=%s (%d/%d)"
              % (label, len(patched), diffs, sha(patched),
                 "PASS" if ok else "FAIL", sum(1 for o, _ in res if o), len(res)))
        res_lines.append((label, path, diffs, sha(patched), ok,
                          sum(1 for o, _ in res if o), len(res), res))

    # ---- 追加 §19 ----
    lines = []
    for label, path, diffs, h, ok, npass, ntot, res in res_lines:
        lines.append("| %s | `%s` | %d | `%s` | %d/%d |" % (
            label, os.path.relpath(path, ROOT), diffs, h, npass, ntot))
    SEC = r"""
---

## 19. A2 Fallback（不新建任何控件）：复用锁磁贴 / 设置页签长按

### 19.1 句柄表 0x20042740 的静态证据（0x080079E8 反汇编）
```
080081fe  ldr r0,[pc,#0xb8]        ; r0 = *(0x080082B8) = 0x20042740（句柄表）
08008200  str r5,[r0]              ; [0] = r5  = 根窗口（FUN_08010ACC 返回）
08008202  str.w sb,[r0,#8]         ; [2] = sb  = 滑出面板
08008208  str r1,[r0,#4]           ; [1] = 上滑层（[sp,#0x24]）
0800820c  str r1,[r0,#0xc]         ; [3] = 声音磁贴（[sp,#0x20]）
08008210  str r1,[r0,#0x10]        ; [4] = 造型灯磁贴（[sp,#0x1c]）
08008212  str.w r8,[r0,#0x14]      ; [5] = r8 = **锁磁贴**（0x08007D56 `mov r8,r0`）
08008218  str r1,[r0,#0x18]        ; [6] = 底部页签1（[sp,#0x18]）
0800821a  str.w sl,[r0,#0x1c]      ; [7] = sl = **底部页签2 "设置"**（0x0800808E `mov sl,r0`）
```
→ 索引 **[5]=锁磁贴、[7]=设置页签** 与 v1.3 的推断一致，有指令级证据。

### 19.2 A2 的行为（完全不建控件、不写 LCD、不改原厂数据）
| 手势 | 动作 |
|---|---|
| 按住**锁磁贴**连续 ≥5 tick（≈500ms） | `ui_key_toggle()`（KEY_ON 切换），触发一次后锁存到松开 |
| 按住**设置页签**连续 ≥5 tick | `ui_broadcast_allch()`（32 频道广播），同样只触发一次 |
| 松开任一句柄 | 计数/触发标志清零，可再次触发 |
| 句柄为 0（页面未建/已销毁） | 直接跳过 |
| 双击（预留） | `ui_broadcast_allid()`（接口/状态位已预留，A2 未启用） |

按下判定：`(*(u16*)(handle + 0x28) & 8) != 0`（等价原厂 `0x0802FCB6(h,8)`）。
状态字节：0x20001084/85（锁：计数/已触发）、0x20001086/87（页签：计数/已触发）；
`0x20001080 = KEY_ON`、**`0x2000108C/8D`** 仍留给 v1.5 磁贴方案（本档不用）。

### 19.3 注入布局（A2）
| 地址 | 大小 | 内容 |
|---|---:|---|
| 0x080F0000 | 28B | `ui_key_toggle()` |
| 0x080F0020 | 56B | `ui_broadcast_allch()` |
| 0x080F0100 | 88B | `ui_broadcast_allid()` |
| 0x080F0200 | 480B | 24×24 字模（A2 暂不绘制，B2 用） |
| 0x080F0400 | 45B | UTF-8 标签 |
| **0x080F1000** | 172B | **`ui_fallback_poll()`**：读句柄表 + 长按计数/触发 |
| **0x080F1100** | 12B | **UI tick wrapper**：`push {r4,lr}; bl poll; bl 0x0800FE78; pop {r4,pc}` |
| **0x08014C1E** | 4B | 原厂代码唯一改动：`bl 0x0800FE78` → `bl wrapper` |

verify 断言：`0x0800FE78 == 2de9fc41`；tick 调用点 = bl wrapper；wrapper 反汇编正确；
`0x0800FF50` 尾部保持原厂（A2 不建控件）；poll 含句柄表字面量、`ldrh r1,[r5,#0x28]`、
`cmp r2,#5`、动作调用恰为 [ui_key_toggle, ui_broadcast_allch]、**0 次 0x08006220**；
桩测试：按住锁 7 tick 触发 1 次、松开再按累计 2 次、按住页签 7 tick 广播 1 次。

### 19.4 成品
| 档 | 文件 | 差异 | sha256 | 验证 |
|---|---|---|---|---|
%s

### 19.5 待办 / 风险
* A2 无视觉反馈（用户靠"行为差异"判断：长按锁=密钥模式切换；长按设置页签=灯全闪）；
* B2 才加自绘覆盖（LCD 0x2A/0x2B/0x2C + 24×24 点阵），设计见 §18.3；
* 风险：若原厂"锁磁贴/页签"的按下位不是 bit3（0x08），改 `FALLBACK_LONG_TICKS` 旁的
  `movs r2,#8` 一处即可（备选 1/2/4/0x10）。
""" % ("\n".join(lines))

    P = os.path.join(HERE, "FEASIBILITY.md")
    with io.open(P, "a", encoding="utf-8") as f:
        f.write(SEC)
    print("appended §19")
    return 0


if __name__ == "__main__":
    sys.exit(main())
