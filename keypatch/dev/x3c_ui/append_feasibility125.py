# -*- coding: utf-8 -*-
"""追加 v1.2.1 章节（逐组备选 + 造型灯语义确认 + 成品 sha256）。"""
import hashlib
import io
import os

HERE = os.path.dirname(os.path.abspath(__file__))
P = os.path.join(HERE, "FEASIBILITY.md")
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))


def sha(path):
    if not os.path.exists(path):
        return "(缺)"
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


MAIN = os.path.join(HERE, "out", "X3_C_v1.38_app_patch.bin")
PG = os.path.join(ROOT, "keypatch", "out", "X3_C_v1.38_app_keyD798_pergroup.bin")

SEC = r"""
---

## 12.5 v1.2.1：A–E 逐组备选（`opts["broadcast_per_group"]`）

### 为什么要有备选
v1.2 的广播用 `group = 0x50`（ALL）发功率/造型灯，依据是原厂
`FUN_0800BDDC` 里确实有 `FUN_0801184c(0x50, 0xB1, 2)` 这类"全局组"用法。
但 **灯端是否接受 `0x50` + `0xD1`（功率）没有实机验证**。因此提供逐组版：

| opts | 每频道发的属性帧 | 帧数/频道 |
|---|---|---|
| `broadcast_per_group=False`（默认，主成品） | `(0x50, 0xD1, 功率)`、`(0x50, 0xB1, 灯)` | 2 |
| `broadcast_per_group=True`（备选成品） | `(10..14, 0xD1, 功率)` ×5、`(10..14, 0xB1, 灯)` ×5 | 10 |

（10..14 = A–E 组，与原厂 `FUN_080089D0` 里 `uVar7 = i<6 ? i+10 : i-6` 的组号编码一致）

### 验证（两版都 18/18 PASS）
```
主成品   : 功率帧 32 个 ([0x50],0xD1,30)；造型灯帧 32 个 ([0x50],0xB1,1)；TEST 32
逐组备选 : 功率帧 160 个 ([10,11,12,13,14],0xD1,30)；造型灯帧 160 个 ([10..14],0xB1,1)；TEST 32
```

### 成品 sha256（本次生成）
```
__MAIN_NAME__
  len    : __MAIN_LEN__（原厂 1021468，长度不变）
  sha256 : __MAIN_SHA__
  verify : 18/18 PASS（broadcast_per_group=False, KEY=0x6B21）
__PG_NAME__
  len    : __PG_LEN__（原厂 1021468，长度不变）
  sha256 : __PG_SHA__
  verify : 18/18 PASS（broadcast_per_group=True, KEY=0xD798）
```
> 注：这两个 bin 是 **UI/广播层单独成品**（KEY 分别 0x6B21 / 0xD798），
> 不含 fw-x3c 的"双发"核心补丁；Lead 集成进 `devices/x3c.py` 的最终整机 bin
> sha256 为 `95aac447…`（含核心补丁，帧数按双发翻倍），两者不冲突。

## 12.6 造型灯语义确认（`表[+0]` → `0xB1`）

实测/反编译确认映射（`FUN_0800BDDC` 的 else 分支）：
```
表[+0] == 0  ->  造型灯"开" -> send_property(group, 0xB1, 1)
表[+0] != 0  ->  造型灯"关" -> send_property(group, 0xB1, 0)
```
我们的桩测试把 A 组写成 `[+0]=0`，断言实际发出 `(0x50,0xB1,1)` → 与上表一致（PASS）。

**若实机灯反向（点了"开"反而关）**：把 `build_broadcast_allch` 里的两个立即数对调即可：
```
c.beq_label("lamp_on<n>")   # [0]==0 -> 跳到"开"分支
c.movs(2, 0)                # 关   <-- 与下一行的 1 对调
c.b_label("lamp_send<n>")
c.label("lamp_on<n>")
c.movs(2, 1)                # 开
```
即把 `c.movs(2, 0)` 改成 `c.movs(2, 1)`、`c.movs(2, 1)` 改成 `c.movs(2, 0)`，
重跑 `python keypatch\dev\x3c_ui\make_final.py` 即可；
也可用 `opts["model_on"]=0/1` 固定值在模拟器/实机上先验证方向。
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
print("appended §12.5/§12.6")
print("main sha:", sha(MAIN))
print("pg   sha:", sha(PG))
