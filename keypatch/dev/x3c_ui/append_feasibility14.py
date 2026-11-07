# -*- coding: utf-8 -*-
"""追加 §14：致命 bug 复盘（r5/r6 误用导致实机反复重启）。"""
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

## 14. 实机事故复盘（v1.2.3 修正）：`ui_quickpanel_ext` 用错父窗口寄存器

### 14.1 现象
刷入 v1.2 UI 版后，X3-C **反复重启 / 自动关机**（看门狗复位）。

### 14.2 根因（Lead 实机定位 + 本任务复核）
`ui_quickpanel_ext` 在 `FUN_080079E8` 尾部（挂载点 `0x08008236`）执行，原实现假设
"进入时 r6 = 父窗口"，于是把 **r6 当 parent** 传给 `FUN_08006220`。实际该点寄存器是：

| 寄存器 | 真实内容 | 证据 |
|---|---|---|
| **r5** | 快捷面板**根窗口句柄**（正是磁贴的父窗口） | `0x08007A02 bl 0x08010ACC(...)` → `0x08007A06 mov r5,r0`；到 `0x08008236` 之前 r5 未被改写 |
| **r6** | **`0x200004C7`——一个 RAM 字节指针，不是控件** | `0x08007F38 ldr r6,[pc,#0x324]`，字面量 `0x08008260` 处值 = `0x200004C7` |
| r4 | 底部指示条控件（调用者随后继续使用，必须保持） | `0x08008234 mov r4,r0` → `0x08008240 mov r0,r4` |

→ 把 `0x200004C7` 当控件结构写（`FUN_08006220` 会给"父窗口"挂子控件、改字段），
等于**用随机数据覆盖 RAM**，破坏 UI 状态机 → 看门狗反复复位。属我们的补丁 bug，与固件无关。

### 14.3 修正（3 条指令，字节级）
| 地址 | 原（错误） | 新（正确） | 含义 |
|---|---|---|---|
| `0x080F0806` | `05 46` (mov r5,r0) | **`06 46` (mov r6,r0)** | 颜色暂存改 r6（r5 留给父窗口） |
| `0x080F0812` | `30 46` (mov r0,r6) | **`28 46` (mov r0,r5)** | 磁贴#1 parent = r5 |
| `0x080F083A` | `30 46` (mov r0,r6) | **`28 46` (mov r0,r5)** | 磁贴#2 parent = r5 |
| `0x080F0858` | `28 46` (mov r0,r5) | **`30 46` (mov r0,r6)** | 返回颜色从 r6 取 |

（与 Lead 用 4 字节临时补丁验证过的方案一致。）

### 14.4 验证（新增护栏，19/19 PASS）
桩测试现在**模拟真实寄存器上下文**：`r5 = 假根窗口 0x20042100`、`r6 = 0x200004C7`，
并在 `0x200004C4` 起放 32 字节 canary，断言：
```
[PASS] ui_quickpanel_ext 桩执行: parent(r5)=0x20042100；
       建磁贴 2 个 [(0x20042100,1,40,288,128,44), (0x20042100,1,176,288,128,44)]；
       设标签 2 次（0x080F0500 / 0x080F0510）；返回颜色 0x00332211（与原函数一致）；
       r6(0x200004C7) RAM 未被写=True      <-- 新增护栏
```
如果以后再把 r6 当 parent，`parent` 断言 + canary 断言会同时 FAIL。

### 14.5 新成品 sha256（v1.2.3，均 19/19 PASS）
```
__MAIN_NAME__
  len    : __MAIN_LEN__（原厂 1021468，长度不变）
  sha256 : __MAIN_SHA__
  verify : 19/19 PASS（ALL 组，KEY=0x6B21，差异 1001 字节）
__PG_NAME__
  len    : __PG_LEN__（原厂 1021468，长度不变）
  sha256 : __PG_SHA__
  verify : 19/19 PASS（A–E 逐组，KEY=0xD798）
```
作废：v1.2.2 的 `4264f13d…`（主）/`bb342290…`（逐组）都有本 bug，**不可再用于实机**。

### 14.6 建议的安全阶梯（给用户）
1. 原厂 v1.38（`devices/x3c/firmware/X3_C_v1.38_app.bin`）—— 确认设备恢复正常；
2. 核心-only（仅 fw-x3c 的私有密钥补丁，无 UI）—— 确认引闪功能正常；
3. **本版 UI（v1.2.3）** —— 再上 UI 层；首刷后重点观察：能否稳定进入主界面（不再重启）、
   快捷面板是否出现两个磁贴、点击是否触发。
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
print("appended §14")
print("main sha:", sha(MAIN))
print("pg   sha:", sha(PG))
