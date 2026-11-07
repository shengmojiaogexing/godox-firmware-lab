# -*- coding: utf-8 -*-
"""A2-p0b / C-p0b：poll 加控件树校验+页面门；tiles 加页面门+显式 show。"""
import io
import os

HERE = os.path.dirname(os.path.abspath(__file__))
P = os.path.join(HERE, "ui_patch.py")
t = io.open(P, encoding="utf-8").read()


def rep(old, new):
    global t
    assert old in t, "NOT FOUND: " + old[:100]
    t = t.replace(old, new, 1)


# ---------- 1) 重新布点（poll 变大） ----------
rep('WRAPPER0 = 0x080F1100             # page0 处理器 wrapper',
    'WRAPPER0 = 0x080F1200             # page0 处理器 wrapper')
rep('WRAPPER1 = 0x080F1200             # 快捷面板 builder wrapper',
    'WRAPPER1 = 0x080F1300             # 快捷面板 builder wrapper')
rep('TILES0_EXT = 0x080F1300           # page0 磁贴创建注入函数',
    'TILES0_EXT = 0x080F1400           # page0 磁贴创建注入函数')

# ---------- 2) 常量：页面字节 / 控件树校验 / 显示 API ----------
rep('FB_TAB_TRIG = 0x20001087         # "设置"页签：已触发标志',
    '''FB_TAB_TRIG = 0x20001087         # "设置"页签：已触发标志
RAM_UI_PAGE = 0x200003E7         # 当前页面 id（page0=主界面）
IS_WINDOW = 0x080303B8           # (handle)->1=仍在控件树中（原厂用法见 §19.1/§21）
WIDGET_SHOW = 0x0802DA22         # (widget, 1) = 显示控件（builder 里多处用于显示子控件）''')

# ---------- 3) 重写 build_fallback_poll（页面门 + 控件树校验 + 状态清零） ----------
start = t.index("def build_fallback_poll(")
end = t.index("def build_tiles_poll(")
NEW_POLL = '''def build_fallback_poll(va: int = CAVE + OFF_POLL) -> bytes:
    r"""A2-p0b poll：不建控件，只观察原厂控件；**先做控件树校验与页面门**。

    防线（针对实机"短按/长按频闪页签重启"）：
      1) 页面门：`*(u8*)0x200003E7 != 0`（不是主界面）→ 清零状态并直接返回，
         避免页面切换中途动作；
      2) 控件树校验：`0x080303B8(handle) == 0`（句柄已不在当前控件树）→ 跳过并清零状态，
         不再 `ldrh [h,#0x28]`，避免野指针；
      3) 句柄为 0 → 跳过并清零。

    行为：锁磁贴长按 >=5 tick -> ui_key_toggle；设置页签长按 >=5 tick -> ui_broadcast_allch。
    """
    c = Code(va)
    c.push(0x170)                       # push {r4, r5, r6, lr}
    c.ldr_lit(4, HANDLE_TABLE)

    def reset_state(count_va, trig_va):
        c.ldr_lit(1, count_va)
        c.movs(2, 0)
        c.strb_imm(2, 1)
        c.ldr_lit(1, trig_va)
        c.movs(2, 0)
        c.strb_imm(2, 1)

    # ---- 页面门 ----
    c.ldr_lit(1, RAM_UI_PAGE)
    c.ldrb_imm(0, 1)
    c.cmp(0, 0)
    c.beq_label("page_ok")
    reset_state(FB_LOCK_COUNT, FB_LOCK_TRIG)
    reset_state(FB_TAB_COUNT, FB_TAB_TRIG)
    c.b_label("poll_end")
    c.label("page_ok")

    for (idx, count_va, trig_va, action, tag) in (
            (5, FB_LOCK_COUNT, FB_LOCK_TRIG, CAVE + OFF_KEY_TOGGLE, "lock"),
            (7, FB_TAB_COUNT, FB_TAB_TRIG, CAVE + OFF_BROADCAST_ALLCH, "tab")):
        rel, bad, skip = "rel_" + tag, "bad_" + tag, "skip_" + tag
        c.ldr_imm5(5, 4, idx)           # r5 = 句柄表[idx]
        c.cmp(5, 0)
        c.beq_label(bad)
        c.mov(0, 5)                     # 控件树校验：0x080303B8(h)
        c.bl(IS_WINDOW)
        c.cmp(0, 0)
        c.beq_label(bad)
        c.ldrh_imm(1, 5, 0x28)          # 按下位
        c.movs(2, 8)
        c.ands(1, 2)
        c.cmp(1, 0)
        c.beq_label(rel)
        # 按下：已触发则跳过；否则计数
        c.ldr_lit(1, trig_va)
        c.ldrb_imm(2, 1)
        c.cmp(2, 0)
        c.bne_label(skip)
        c.ldr_lit(1, count_va)
        c.ldrb_imm(2, 1)
        c.adds(2, 1)
        c.strb_imm(2, 1)
        c.cmp(2, FALLBACK_LONG_TICKS)
        c.bcc_label(skip)
        c.ldr_lit(1, trig_va)
        c.movs(2, 1)
        c.strb_imm(2, 1)
        c.bl(action)
        c.b_label(skip)
        # 松开：清零
        c.label(rel)
        reset_state(count_va, trig_va)
        c.b_label(skip)
        # 句柄无效/为 0：清零
        c.label(bad)
        reset_state(count_va, trig_va)
        c.label(skip)
    c.label("poll_end")
    c.pop(0x170)
    return c.finish()


'''
t = t[:start] + NEW_POLL + t[end:]

# ---------- 4) tiles：页面门 + 显式显示 ----------
rep('''    c = Code(va)
    c.push(0x170)                      # push {r4, r5, r6, lr}
    c.ldr_lit(1, HANDLE_TABLE)         # r1 = &句柄表
    c.ldr_imm5(5, 1)                   # r5 = 句柄表[0] = 根窗口
    if mode == "none":
        c.pop(0x170)
        return c.finish()''',
    '''    c = Code(va)
    c.push(0x170)                      # push {r4, r5, r6, lr}
    if mode == "none":
        c.pop(0x170)
        return c.finish()
    # ---- 页面门：只在该页为主界面(0)时才建磁贴 ----
    c.ldr_lit(1, RAM_UI_PAGE)
    c.ldrb_imm(0, 1)
    c.cmp(0, 0)
    c.beq_label("pg_ok")
    c.pop(0x170)
    return c.finish()
    c.label("pg_ok")
    c.ldr_lit(1, HANDLE_TABLE)         # r1 = &句柄表
    c.ldr_imm5(5, 1)                   # r5 = 句柄表[0] = 根窗口''')

rep('''        c.ldr_lit(1, RAM_TILE_KEY if label_off == OFF_TEXT_KEY else RAM_TILE_BCAST)
        c.str_imm5(4, 1)
        c.label(skip)
    c.pop(0x170)
    return c.finish()''',
    '''        c.ldr_lit(1, RAM_TILE_KEY if label_off == OFF_TEXT_KEY else RAM_TILE_BCAST)
        c.str_imm5(4, 1)
        # 显式显示（原厂 builder 用 FUN_0802DA22(w,1) 显示子控件，见 FEASIBILITY §21）
        c.mov(0, 4)
        c.movs(1, 1)
        c.bl(WIDGET_SHOW)
        c.label(skip)
    c.pop(0x170)
    return c.finish()''')

io.open(P, "w", encoding="utf-8", newline="\n").write(t)
print("A2-p0b poll + C-p0b tiles updated")
