# -*- coding: utf-8 -*-
"""追加 §15：照抄原厂磁贴完整属性序列 + 每个 setter 的作用猜测 + 新 sha。"""
import hashlib
import io
import os

HERE = os.path.dirname(os.path.abspath(__file__))
P = os.path.join(HERE, "FEASIBILITY.md")
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
MAIN = os.path.join(HERE, "out", "X3_C_v1.38_app_patch.bin")
PG = os.path.join(ROOT, "keypatch", "out", "X3_C_v1.38_app_keyD798_pergroup.bin")


def sha(path):
    if not os.path.exists(path):
        return "(缺)"
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


SEC = r"""
---

## 15. v1.3：磁贴创建照抄原厂完整属性序列（降低未初始化字段导致的崩溃风险）

### 15.1 背景
v1.2.3 里 `ui_quickpanel_ext` 只做了"`FUN_08006220` 建控件 + `FUN_0802D97A` 设文本"，
漏掉了原厂每个磁贴创建后必做的一整串属性初始化。若控件绘制/命中时读未初始化字段，
仍有崩溃风险。本版**逐条照抄原厂"声音"磁贴（0x08007B02-0x08007BD2）的完整序列**。

### 15.2 原厂序列（反汇编逐条核对）与作用猜测
| # | 调用 | 参数 | 作用猜测 |
|---|---|---|---|
| 1 | `FUN_08006220` | `(parent, 1, x, y, w, h)` | 建子控件（type=1 = 磁贴/按钮类） |
| 2 | `FUN_08027D90` | `(0x27,0x27)` | 由 (fg,bg) 生成调色板/配色对象 |
| 3 | `FUN_0803157C` | `(tile, color, 0)` | 设背景色（索引 0） |
| 4 | `FUN_08031594` | `(tile, 0xFF, 0)` | 设前景/文本色（0xFF = 白，索引 0） |
| 5 | `FUN_0803161C` | `(tile, 0xFF, 4)` | 设某个颜色/描边（索引 4） |
| 6 | `FUN_08027FE0` | `()` | 取"动画/主题值"（随主题变化的数值） |
| 7 | `FUN_08031604` | `(tile, value, 4)` | 把动画值写入属性索引 4 |
| 8 | `FUN_08031630` | `(tile, 2, 4)` | 形状/样式枚举 = 2（索引 4）——猜测 2=圆形/圆角磁贴 |
| 9 | `FUN_08031626` | `(tile, 4, 4)` | 圆角半径/边宽 = 4（原厂 r1=r2=4） |
| 10 | `FUN_080315B6` | `(tile, 0, 0)` | 标志位（索引 0）=0（如禁用/选中位清 0） |
| 11 | `FUN_080315C0` | `(tile, 2, 0)` | 类型/对齐枚举 = 2（索引 0） |
| 12 | `FUN_080315B6` | `(tile, 0, 4)` | 标志位（索引 4）=0 |
| 13 | `FUN_080315C0` | `(tile, 2, 4)` | 类型/对齐枚举 = 2（索引 4） |
| 14 | `FUN_08027FE0` | `()` | 再取动画/主题值 |
| 15 | `FUN_0803159E` | `(tile, value, 4)` | 写入属性索引 4（与 7 同族） |
| 16 | `FUN_08031696` | `(tile, 0x6C, 0)` | 尺寸/内半径 = 0x6C(108)（= 磁贴边长） |
| 17 | `FUN_0802553E` | `(tile, 9, 0, 0, font=0x0806B090, color=FUN_08027FE0(), fmt=0x08007E20)` | 建图标文本子控件；fmt 指向私用区字符 U+E900（图标字形） |
| 18 | `FUN_0802D97A` | `(tile, "KEY"/"BCAST", 0)` | 设磁贴文本（本补丁改为 ASCII 标签） |

> 说明：属性 setter 的"索引"（第三参数 0/4）在原厂里成对出现（0 与 4 各设一遍），
> 猜测是"正常态/按下态"两套属性或"背景/前景"两套，具体语义未逐个实验确认（不影响"全部初始化"这一目的）。
> `0x08027FE0` 返回值随主题/动画变化，照抄"每次使用前重新调用"的行为。

### 15.3 我们改了什么
- 只改：磁贴坐标 `(40,288,128,44)` / `(176,288,128,44)`，以及第 18 步文本指针 → `KEY` / `BCAST`（0x080F0500 / 0x080F0510，ASCII，确保字库有字形）；
- 其余**逐条照抄**（含图标：暂时与原厂"声音"磁贴同一图标字形 U+E900，保证字形一定存在；
  每片不同图标需要位图/字库扩展，后续再做）。
- 寄存器约定保持 v1.2.3 修正：parent 用 **r5**，`0x200004C7` 绝不触碰（桩测试有 canary 断言）。

### 15.4 验证（19/19 PASS，新增断言）
```
[PASS] ui_quickpanel_ext 共 39 个 bl；2×建磁贴、2×文本、2×图标(0x0802553E)，
       且原厂属性 setter 全部出现=True
[PASS] ui_quickpanel_ext 桩执行: parent(r5)=0x20042100 正确=True；
       磁贴 2 个 [(0x20042100,1,40,288),(0x20042100,1,176,288)]；
       属性 setter 序列与原厂(0x08007B16-0x08007BD2)一致=True（14 个/片 × 2 片）；
       关键参数=True（3157C 颜色 / 31630(2,4) / 31626(4,4) / 315B6/315C0 四组 /
       31696(0x6C,0) / 图标 font=0x0806B090 fmt=0x08007E20 / 文本指针）；
       返回颜色 0x00332211（与原函数一致）；r6(0x200004C7) RAM 未被写=True
```

### 15.5 新成品 sha256（v1.3，均 19/19 PASS）
```
__MAIN_NAME__
  len    : __MAIN_LEN__（原厂 1021468，长度不变；注入+挂载点合计差异 1348 字节）
  sha256 : __MAIN_SHA__
  verify : 19/19 PASS（ALL 组，KEY=0x6B21）
__PG_NAME__
  len    : __PG_LEN__（原厂 1021468，长度不变）
  sha256 : __PG_SHA__
  verify : 19/19 PASS（A–E 逐组，KEY=0xD798）
```
作废：v1.2.3 的 `17242d89…` / `7eaf5282…`（缺完整属性序列）。
注入区布局（本版）：磁贴构建 0x080F0800..0x080F09EC（492B）、轮询 0x080F1000、
页 1 跳板 0x080F1100 —— 互不重叠，且都在 0x080F0000..0x080F6FFF 空闲区内。
"""

body = SEC
body = body.replace("__MAIN_NAME__", os.path.basename(MAIN))
body = body.replace("__MAIN_LEN__", str(os.path.getsize(MAIN) if os.path.exists(MAIN) else 0))
body = body.replace("__MAIN_SHA__", sha(MAIN))
body = body.replace("__PG_NAME__", os.path.basename(PG))
body = body.replace("__PG_LEN__", str(os.path.getsize(PG) if os.path.exists(PG) else 0))
body = body.replace("__PG_SHA__", sha(PG))

with io.open(P, "a", encoding="utf-8") as f:
    f.write(body)
print("appended §15")
print("main:", sha(MAIN))
print("pg  :", sha(PG))
