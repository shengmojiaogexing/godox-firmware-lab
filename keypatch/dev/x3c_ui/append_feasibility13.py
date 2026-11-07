# -*- coding: utf-8 -*-
"""追加 §13：更正"底部第三页签"的说法（布局补丁改为 opt-in，默认关闭）。"""
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

## 13. 更正（v1.2.2）：**底部页签仍然是 2 个**，布局补丁改为 opt-in 默认关闭

### 13.1 更正内容
§10.1 的第 9/10/11 号补丁点（`0x08007E7C`、`0x0800807A`、`0x08008084`，
把两个页签从 `w=0xA8` 改窄、第二个从 `x=0xBB` 左移到 `0x81`）**已于 v1.2.2 改为默认不应用**。

原因（Lead 复核 + 本任务复核一致）：`FUN_080079E8` 里**只有两处**底部页签创建调用
```
0x08007E8C: FUN_08006220(panel, 1, 0x0B, 0x155, 0xA8, 0x5C)   ; 频闪 / Multi
0x0800808A: FUN_08006220(panel, 1, 0xBB, 0x155, 0xA8, 0x5C)   ; 设置 / Setting
```
**不存在第三个页签**。之前把那两个改窄 + 左移，只会在右下角留一块约 129px 的空缺，
页签变窄却不会出现第三个按钮 —— 属视觉瑕疵，默认关闭。

新增 opt（默认 False，留给"以后真做第三页签"时做视觉实验）：
```python
apply_ui(data, {"enable_menu_patch": True, "three_tab_layout": False})   # 默认
apply_ui(data, {"enable_menu_patch": True, "three_tab_layout": True})    # 实验（仅改宽度/位移）
```

### 13.2 现在的实际 UI 改动（默认版）
- **底部页签保持原厂 2 个、原尺寸原位置**（0x08007E7C/0x0800807A/0x08008084 与原厂逐字节一致，verify 有断言）；
- 新增的 `KEY` / `BCAST` 两个磁贴位于**锁磁贴下方独立一行**：
  `(40, 288, 128, 44)` 与 `(176, 288, 128, 44)`；
- 不重叠核算：锁磁贴 y=172..280；新磁贴 y=288..332；底部页签 y=341..433 → **三段互不重叠**
  （新磁贴底 332 < 页签顶 341，间隙 9px）。若实机发现仍重叠，再考虑真正创建第三个页签
  （需要新增一次 `FUN_08006220` 创建 + 回调/页面分发接线，工作量更大，本次不做）。

### 13.3 成品 sha256（v1.2.2，均 19/19 verify PASS）
```
__MAIN_NAME__
  len    : __MAIN_LEN__（原厂 1021468，长度不变）
  sha256 : __MAIN_SHA__
  verify : 19/19 PASS（ALL 组；无布局补丁，差异 1001 字节）
__PG_NAME__
  len    : __PG_LEN__（原厂 1021468，长度不变）
  sha256 : __PG_SHA__
  verify : 19/19 PASS（A–E 逐组；无布局补丁）
```
（上一版含布局补丁的 sha 已作废：主成品 `4882003a…`、逐组 `07708f3e…`。）
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
print("appended §13")
print("main sha:", sha(MAIN))
print("pg   sha:", sha(PG))
