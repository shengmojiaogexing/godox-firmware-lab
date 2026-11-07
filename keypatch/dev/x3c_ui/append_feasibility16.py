# -*- coding: utf-8 -*-
"""追加 §16：v1.4 改挂载点 + 三档安全阶梯（实机 A/B/C）。"""
import hashlib
import io
import os

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
P = os.path.join(HERE, "FEASIBILITY.md")
OUTL = os.path.join(HERE, "out")


def sha(p):
    h = hashlib.sha256()
    with open(p, "rb") as f:
        for c in iter(lambda: f.read(1 << 20), b""):
            h.update(c)
    return h.hexdigest()


FILES = [
    ("A 诊断档（不建控件）", os.path.join(OUTL, "X3_C_v1.38_app_ui_A_noTiles.bin"),
     "只装页 1 尾部跳板 + 轮询；不调用 FUN_08006220"),
    ("B 最小档（建磁贴+文本）", os.path.join(OUTL, "X3_C_v1.38_app_ui_B_minimal.bin"),
     "create + FUN_0802D97A，无任何属性 setter/图标"),
    ("C 完整档（原厂属性序列）", os.path.join(OUTL, "X3_C_v1.38_app_ui_C_full.bin"),
     "create + 原厂 13 个 setter + 文本（图标子控件仍关闭）"),
    ("逐组广播备选", os.path.join(ROOT, "keypatch", "out",
                                 "X3_C_v1.38_app_keyD798_pergroup.bin"),
     "A–E 逐组功率/造型灯（KEY=0xD798），与 C 同挂载方式"),
]

lines = []
for name, path, desc in FILES:
    lines.append("| %s | `%s` | %d | `%s` |" % (
        name, os.path.relpath(path, ROOT), os.path.getsize(path) if os.path.exists(path) else 0,
        sha(path) if os.path.exists(path) else "-"))

SEC = """
---

## 16. v1.4：磁贴创建点迁移 + 实机三档诊断（当前进行中）

### 16.1 第三次实机失败的根因假设（结构性，而非参数）
上一版把"建磁贴"挂在 `FUN_080079E8`（页面构建函数）内部的 `0x08008236`
（原 `bl 0x08027FE0`）。复核后确认该点有两个不可靠之处：
1. **函数边界可疑**：Ghidra 的 `FUN_080079E8` 只到 `0x0800821E`，`0x08008236`
   落在它的尾部/共享尾声区，该区 r5/r6 用途不稳定（r6=0x200004C7 RAM 指针即症状）；
2. **时机不对**：此处页面仍在构建中（尾部的底部指示条刚建），此时往窗口树插入新控件，
   会踩到 WM 尚未完成的布局/链表 —— 表现就是 RAM 被破坏、看门狗复位。

### 16.2 改法
把创建点整体迁到 **页 1 处理器 `FUN_0800FE78` 的尾部 `0x0800FF50`**：
- 原 6 字节 `ldr r1,[pc,#0x24]; movs r0,#6; strb r0,[r1]`（写 `*0x200004C2 = 6` 页面就绪标记）
  改 为 `bl 0x080F1200` + `nop`；
- 注入函数先**原样复现**那 3 条原指令，再建磁贴；
- 父窗口用 **r6**（已核实 `0x0800FEBA bl FUN_08010ACC(0,9,...)` → `0x0800FEBE mov r6,r0` = 根窗口）；
- 新增 **NULL 保护**：父窗口==0 跳过；`FUN_08006220` 返回 0 跳过（不会对 NULL 设属性）；
- 图标子控件 `FUN_0802553E`（7 参数/3 栈参数，风险最高）**默认关闭**（`opts["tile_icon"]`）。

### 16.3 三档安全阶梯（实机按 A→B→C 刷，用"哪档开始重启"定位）
| 档 | 文件 | 大小 | sha256 |
|---|---|---|---|
%s

判读规则：
- **A 也重启** → 问题在"页 1 尾部跳板/轮询"本身（不关控件创建）→ 下一步去掉轮询、只留跳板；
- **A 正常、B 重启** → 问题在"新增控件"（父窗口或时机）→ 下一步父窗口改用内容面板句柄、只建 1 个磁贴二分；
- **B 正常、C 重启** → 问题在某个属性 setter → 逐个 setter 二分（出 C1/C2/C3 子档）；
- **三档都正常** → v1.4 可用，再评估图标与逐组广播。

### 16.4 回滚
刷回 `devices/x3c/firmware/X3_C_v1.38_app.bin`（原厂）或核心-only 包即可；本批补丁均为原位、长度不变。
""" % ("\n".join(lines))

with io.open(P, "a", encoding="utf-8") as f:
    f.write(SEC)
print("appended §16")
