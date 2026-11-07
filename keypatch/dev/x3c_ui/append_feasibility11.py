# -*- coding: utf-8 -*-
"""把 v1.1（点击动作接线）章节追加到 FEASIBILITY.md。"""
import io
import os

HERE = os.path.dirname(os.path.abspath(__file__))
P = os.path.join(HERE, "FEASIBILITY.md")

SEC = r"""
---

## 11. v1.1 追加：磁贴点击 → 动作接线（完成）

> 目标：让快捷面板新增的 KEY / BCAST 磁贴"点了有反应"。
> 结果：**注入 + 桩验证全 PASS（17/17）**，仍属"无实机验证"。

### 11.1 按下状态读取（与固件等价）
- 按下判定 = `(*(u16*)(widget + 0x28) & 8) != 0`，与固件 `0x0802FCB6(widget, mask)`
  （`ldrh r0,[r0,#0x28]; ands r0,r1`）等价；`0x0800BA4C` 里用 `mask=8` 判断控件激活。
- 边沿检测：last-state 字节 **0x2000108C（KEY）/ 0x2000108D（BCAST）**，
  按住只触发一次，松开后清零可再次触发。
- 注：上一版试过 `0x080303B8`（Lead 更正：那是"控件树成员检查"，不是按下状态），已弃用。

### 11.2 注入布局（新增 2 段）
| 地址 | 大小 | 内容 |
|---|---:|---|
| 0x080F0900 | ~120B | `ui_tiles_poll()`：句柄非空 → 读 `[h+0x28]&8` → 边沿触发 `ui_key_toggle` / `ui_broadcast_allch` |
| 0x080F0A00 | 16B | 页 1 入口跳板：`push {r0-r3,lr}; bl 0x080F0900; pop {r0-r3,lr}; <原首指令 4B>; b.w 0x0800FE7C` |
| **0x0800FE78** | 4 | `b.w 0x080F0A00`（页 1 处理器入口，UI tick 每 100ms 调 0x0800FE78） |

句柄 RAM：`0x20001084` = KEY 磁贴、`0x20001088` = BCAST 磁贴、`0x2000108C/8D` = 上次状态。

### 11.3 为什么"每 tick 调用"成立
`FUN_08014B28`（100ms 软定时器回调）页 1 分支里有
`0x08014C18..0x08014C22: cmp r1,#1; bne ret; bl 0x0800FE78; b ret`
→ **页 1 每 tick 都会调 0x0800FE78**。因此把 poll 挂在它的入口即可获得 100ms 轮询，
且不改动 UI tick 本身的寄存器上下文（跳板保存 r0-r3/lr）。

### 11.4 关于"原首指令"的一处修正
Lead 给的是 `2de9f04f`（push.w {r4-r11,lr}），但**本镜像 0x0800FE78 的实际首指令是
`2de9fc41`（push.w {r2,r3,r4,r5,r6,r7,r8,lr}）**。为避免猜错，实现里由
`UI_PAGE1_ORIG_INS = "2de9fc41"` 常量 + `apply_ui` 的原始字节校验保证一致；
跳板把这 4 字节**原样搬入**再 `b.w 0x0800FE7C`。

### 11.5 反汇编（补丁后，`out/menu_evidence.txt` §八）
```
页 1 入口 0x0800FE78:  b.w #0x80f0a00
跳板 0x080F0A00: push {r0, r1, r2, r3, lr}
                 bl   #0x80f0900
                 pop  {r0, r1, r2, r3, pc}
                 push.w {r2, r3, r4, r5, r6, r7, r8, lr}   ; 原首指令
                 b.w  #0x800fe7c                             ; 原第二指令处
轮询 0x080F0900: push {r4, r5, r6, lr}
                 movs r6, #8
                 ldr  r4, =0x20001084 ; ldr r0,[r4] ; cmp r0,#0 ; beq end1
                 ldrh r1,[r0,#0x28]  ; ands r1,r6  ; cmp r1,#0 ; beq np1
                 ldr  r5, =0x2000108C ; ldrb r2,[r5] ; cmp r2,#0 ; bne end1
                 movs r2,#1 ; strb r2,[r5] ; bl 0x080F0000 (ui_key_toggle) ; b end1
                 np1: ldr r5,=0x2000108C ; movs r2,#0 ; strb r2,[r5]
                 end1: ... BCAST 同构（0x20001088 / 0x2000108D -> bl 0x080F0020）
```

### 11.6 桩验证（Unicorn）
```
轮询桩执行: 按住 KEY 3 tick -> KEY_ON=1（边沿只触发一次）
            松开再按        -> KEY_ON=0
            按住 BCAST 3 tick -> 广播调用 1 次
```

### 11.7 风险（无实机）
| 风险 | 概率 | 说明 / 缓解 |
|---|---|---|
| `widget+0x28` bit8 不是"按下" | 中 | 若点不动：改 mask 常量（1/2/4/0x10）重生成即可（poll 里 `movs r6,#8` 一处） |
| 磁贴按下状态需要"消费"后才复位 | 低中 | 我们的边沿检测不依赖复位（last-state 自己维护）；若 bit 一直为 1 只会触发一次 |
| 页 1 处理器不是每 tick 调用 | 低 | 静态已证 0x08014C1E；若实机不成立，退化到挂 UI tick（方案见上一条 Lead 指示） |
| 跳板破坏页 1 寄存器 | 低 | 只保存/恢复 r0-r3+lr；原首指令原样重放；不碰 r4-r8 |
| 仍无显示 | 中 | 显示/页面问题见 §10.5 R2，与本次接线无关 |

### 11.8 产物更新
- `out/X3_C_v1.38_app_patch.bin` —— **v1.1（含点击接线）**，972 字节差异，17/17 验证 PASS
- `out/X3_C_v1.38_app_patch_v1.0.bin` —— v1 旧版（仅视觉接线，留作对比）
- `out/menu_evidence.txt` —— 已含 §八：页 1 入口/跳板/轮询反汇编 + 句柄 RAM + 972 差异字节
"""

with io.open(P, "a", encoding="utf-8") as f:
    f.write(SEC)
print("appended", len(SEC), "chars")
