# -*- coding: utf-8 -*-
"""追加 §17：致命 bug（跳板 POP 回返回地址）+ v1.5 调用点 wrapper 修法 + 新 sha。"""
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
    ("主成品 / C 完整档", os.path.join(OUTL, "X3_C_v1.38_app_patch.bin")),
    ("A 诊断档（不建控件）", os.path.join(OUTL, "X3_C_v1.38_app_ui_A_noTiles.bin")),
    ("B 最小档（建磁贴+文本）", os.path.join(OUTL, "X3_C_v1.38_app_ui_B_minimal.bin")),
    ("逐组广播备选", os.path.join(ROOT, "keypatch", "out",
                                 "X3_C_v1.38_app_keyD798_pergroup.bin")),
]
rows = "\n".join("| %s | `%s` | %d | `%s` |" % (
    n, os.path.relpath(p, ROOT),
    os.path.getsize(p) if os.path.exists(p) else 0, sha(p)) for n, p in FILES)

SEC = """
---

## 17. 致命 bug 复盘 #2（v1.5 修正）：页 1 跳板的 `pop {...,pc}` 直接返回

### 17.1 现象与根因（Lead 定位，本任务复核确认）
v1.2.3/v1.3/v1.4 的实机现象（反复重启/自动关机）与**属性序列、挂载点都无关**，
真正原因是页 1 钩子写错：

```
旧跳板（0x080F1100）:
  push {r0,r1,r2,r3,lr}
  bl   ui_tiles_poll
  pop  {r0,r1,r2,r3,pc}   <-- Thumb POP 的 bit8 是 PC（不是 LR！）
                              = 把"进入时的 LR"弹进 PC → 直接返回调用者
  push.w {...}            <-- 死代码，永不执行
  b.w  0x0800FE7C         <-- 死代码
```
效果：`0x0800FE78`（页 1 处理器）**一次都没执行** → 主界面/USB 充电分支的 UI 状态
没有初始化 → 看门狗反复复位。**这解释了 A/B/C 都会"假失败"。**

> 另：Thumb 的 `POP` 只能恢复 `PC`（bit8），**不存在 `pop {... ,lr}`**；
> 想保存/恢复 LR 必须走"调用点 wrapper + 尾调用"或栈上显式 ldr。

### 17.2 正解（已实现）：调用点 wrapper
```
0x0800FE78 保持原厂 2de9fc41（不动它）
0x08014C1E:  bl 0x0800FE78   ->   bl 0x080F1100 (wrapper)

wrapper @0x080F1100（12 字节）:
  push {r4, lr}          ; 保存 r4(callee-saved) + 返回 UI tick 的地址
  bl   ui_tiles_poll     ; 每 100ms 轮询
  bl   0x0800FE78        ; 原厂页 1 处理器完整执行（它内部保存/恢复 LR）
  pop  {r4, pc}          ; 返回 UI tick
```
verify 断言（全部 PASS）：`0x0800FE78 == 2de9fc41`、`0x08014C1E == bl wrapper`、
wrapper 反汇编为 `push {r4,lr}; bl poll; bl 0x0800FE78; pop {r4,pc}`。

### 17.3 当前成品（v1.5，均 20/20 verify PASS）
| 档 | 文件 | 大小 | sha256 |
|---|---|---|---|
%s

（长度均 1,021,468 不变；清单 `out/v15_sha256.txt`）

### 17.4 诊断顺序
A（钩子修好 + 轮询，不建控件）→ B（+建磁贴/文本）→ C（+原厂完整属性序列）。
- A 重启 → 钩子/轮询仍有问题；
- A 正常、B 重启 → "新增控件"问题（父窗口/时机）；
- B 正常、C 重启 → 某个 setter；我出 C1/C2/C3 细分。

### 17.5 与 Lead `_ui_fix3.bin` 的差异
`_ui_fix3` = **原挂载点（页面构建函数内 0x08008236）+ 修好的钩子**；
本目录 C 档 = **新挂载点（页 1 尾部 0x0800FF50）+ 修好的钩子**。
若 fix3 可进界面而 C 不行，则把挂载点切回 0x08008236（`opts` 一档即可，5 分钟）。
"""

with io.open(P, "a", encoding="utf-8") as f:
    f.write(SEC % rows)
print("appended §17")
