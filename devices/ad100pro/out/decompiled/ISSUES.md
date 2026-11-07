# out/functions.json 函数边界问题清单（ISSUES）

> 本文不修改 Lead 的 `out/functions.json` / `out/disasm.txt`，只记录伪反编译过程中
> 发现的边界/数据问题，供后续修正分析脚本。自动统计见 `out/decompiled/issues_auto.md`。
> 所有结论可用 `firmware/AD100pro_V1.2.bin` + `out/disasm.txt` 复核。

## 0. 汇总

| # | 问题 | 影响面 |
|---|---|---|
| 1 | 启动区 0x080030C0–0x080030EE 被拆成 6 个"函数"，且把常量池当指令 | 影响启动链理解 |
| 2 | 0x0800DA78 声明 770 条指令，实际绝大多数是字模/数据表 | 1190 字节数据被当代码 |
| 3 | 806 个"函数"起点落在其它函数范围内（重叠），98 个 size≤2、760 个 size≤4 | 函数总数 1199 明显虚高，真实代码函数约 431 个（disasm 有 header 的） |
| 4 | 71 个条目 end==addr（范围内 0 条指令） | 无法生成有意义的伪 C |
| 5 | 95 个条目 functions.json 的 insns 与 [addr,end) 内已解码指令数不符，个别 insns 远超 size/2 | 统计不可信 |
| 6 | 61 个函数 basic block 落在自身 [addr,end) 之外 | 函数范围不可靠 |
| 7 | disasm.txt 只有 431 个函数 header；84 个 header 的 insns 与实际打印指令数不符，60 个 section 打印了超出 end 的指令 | 按 section 读函数会漏/串 |
| 8 | periph_map 把 GPIO 的 IDR/BSRR/BRR 误标成 EXTI 寄存器 | 0x0800741C / 0x08007426 等 GPIO API 注释错 |

---

## 1. 启动区被切碎 + 常量池被当指令（最影响阅读）

`out/functions.json` 中：

| 条目 | end | size | insns | 实际情况 |
|---|---|---|---|---|
| 0x080030C0 | 0x080030C2 | 2 | 1 | 启动桩第 1 条（ldr SP 常量） |
| 0x080030C2 | 0x080030CC | 10 | 4 | 启动桩第 2–5 条（mov sp, bl init_array, ldr main, bx） |
| 0x080030C8 | 0x080030C8 | 0 | 0 | 启动桩内部地址，被重复登记为函数 |
| 0x080030D4 | 0x080030D8 | 4 | 2 | Reset_Handler 前半（ldr SystemInit; blx） |
| 0x080030D8 | 0x080030DC | 4 | 2 | Reset_Handler 后半（ldr __start; bx） |
| 0x080030E6 | 0x080030E8 | 2 | 1 | `b .` 死循环填充（0x080030DC–0x080030E6 共 6 条 `FE E7`），不是函数 |
| 0x080030EC | 0x080030F0 | 4 | 2 | **常量 0x080030C1**（被 0x080030D8 的 `ldr r0,[pc,#0x10]` 使用），不是指令 |

实证（`out/disasm.txt`）：

```
080030D4  0448   ldr r0,[pc,#0x10]   ; =func_0800B964
080030D6  8047   blx r0
080030D8  0448   ldr r0,[pc,#0x10]   ; =func_080030C0
080030DA  0047   bx r0
080030EC  c130   adds r0,#0xc1      <-- 实际是 .word 0x080030C1 的低半字
080030EE  0008   lsrs r0,r0,#0x20   <-- 实际是 .word 0x080030C1 的高半字
```

正确边界应为：
- `Reset_Handler` = 0x080030D4–0x080030DC（向量[1]=0x080030D5）；
- `__start` = 0x080030C0–0x080030D4（含常量池 0x080030CC/0x080030D0）；
- 0x080030DC–0x080030E6 = 死循环填充；0x080030E8/0x080030EC 两个 word 是 Reset 的常量池；
- 0x080030E6 不应出现在函数表里。

**原因推测**：`scan_gaps`/`sweep_gap_starts` 把 gap 里的 2 字节 `FE E7` 和常量池当成函数起点；
`add_entry` 对已有函数的内部地址也登记了 entry。

## 2. 0x0800DA78（声明 770 条指令）实际是数据表

- `functions.json`：addr=0x0800DA78, end=0x0800DF1E, size=1190, insns=770, blocks=[0x0800D726, 0x0800DA78, 0x0800DAAA, 0x0800DB16, 0x0800DCC2]。
- 但 `out/gaps.txt` 在 0x0800DB50–0x0800DB9E、0x0800DBC0–0x0800DC50、0x0800DCC0… 列出一大片"未认领"区域；
  这些字节是 `04 fe 92 92 fe …`、`05 fe 10 28 44 82 …` 这类**点阵字模/字库**（与 0x0800D164–0x0800D724 的字模区同族）。
- 起始字节 `14 5a 5a 5a 5a 5a 5a 50 46 46 3c 32 28 28 ac 03 …` 用 Thumb 解出
  `ldrh/str/mov r8/adds/cmp/lsls` 无合理控制流；`functions.meta.json` 对该地址只记 **25 条指令**；
  反查全固件没有任何 `bl #0x0800DA78`（无调用者）。
- 结论：**该条目不是函数**，是 LCD 字模/查询表数据；真正的代码在 0x0800D164 之前结束
  （0x0800D098 一带仍是代码，0x0800D164 起为数据）。0x0800DA78 的 5 个 block 起点同样是
  从数据里错误解释出的"跳转目标"。

## 3. 函数表虚高：806 个起点嵌在别的函数里

- 1199 个条目中 **806 个** 的 addr 严格落在别的条目 `(addr,end)` 区间内；
- 相邻条目地址区间重叠 201 处；
- `size<=2` 98 个、`size<=4` 760 个 —— 绝大多数是 `b`/`bx` 跳转目标、尾调用桩、
  或把第二条指令当成新函数起点，不是独立函数；
- 与 `disasm.txt` 对照：只有 **431** 个函数有 header（即线性打印时真正遇到的函数起点）。
  `393` 个条目既不嵌套也不重复，数量级与 431 一致，**可信的真实函数数约 400 个**。
- 例：0x080030C8、0x08003E56、0x08005A38、0x08009CE2 等都是 end==addr 或 1 条指令的碎片。

## 4. 71 个"空函数"条目

`end == addr`（size=0）或范围内没有任何已解码指令的条目 71 个；这些文件只能生成
"no decoded instructions" 占位，不能当函数使用。

## 5. insns 统计与 size 矛盾

如 `0x080032BC`：size=12 字节，但 `insns=165`（12 字节能放最多 6 条 Thumb 指令），
说明 `insns` 字段在 `run_func()` 里跨函数累计/重复计数。共 95 个条目不一致。

## 6. basic block 越界

61 个 basic block 落在所属函数 `[addr,end)` 之外（如 0x0800DA78 的 blocks 含
0x0800D726 < 起点），因为 `explore()` 会把跳转/尾调用到达的远端 block 记到当前函数名下。

## 7. disasm.txt 的函数 section 不等于函数区间

打印器按全局线性地址流走，遇到函数起点才插入 header，因此：

- 431 个 header 对 1199 个条目（768 个条目没有独立 section）；
- 84 个 header 的 `insns=N` 与 section 内实际打印指令数不符；
- 60 个 section 打印的指令地址 `>= end`（例如 header func_080030C8 end=0x080030C8
  之后却打印了 0x080030C8/0x080030CA 两条指令）；
- 按 header 切函数切不准，建议后续用"函数 blocks + 已知 entry"重建，而不是按 section。

## 8. periph_map 误标 GPIO 寄存器

- `0x08007426`：`str r1,[r0,#0x18]` / `str r1,[r0,#0x28]` 被标成 `EXTI.+0x18/+0x28`，
  但 128 个调用点传入的 r0 都是 GPIO 基址（0x48000400 等），这两个偏移在 STM32F0 上是
  **GPIOx_BSRR (+0x18) 置位 / GPIOx_BRR (+0x28) 清零**。
- `0x0800741C`：`ldr r0,[r0,#0x10]` 被标成 `EXTI.SWIER`，实际是 **GPIOx_IDR (+0x10)**。
- 原因：periph 标注只看 `ldr rX,[pc,#imm]` 字面量，不跟踪寄存器实参的数据流。

---

## 建议修复优先级（供 Lead/分析脚本参考）

1. **先用真实调用图重建函数起点**：只保留有 `bl/blx` 入边、向量表入口、或 `push`+返回的起点；
   删除 end==addr 与"孤立 1 条指令"条目（预计从 1199 降到 ~400–450）。
2. 修正启动区边界（合并 0x080030C0/C2/C8；合并 0x080030D4/D8；删除 0x080030E6/EC）。
3. 把 0x0800D164–0x0800DF1E 标记为数据（字模/字库），从函数表移除 0x0800DA78 及相关 block。
4. `insns` 改为按函数去重计数（用 `[addr,end)` ∩ `insns` 即可，和本仓库伪 C 一致）。
5. periph 标注增加"寄存器实参数据流"或至少人工维护已知 GPIO/EXTI 基址调用约规。
6. disasm.txt 打印改为按函数 blocks 输出（或不要依赖 section 边界）。
