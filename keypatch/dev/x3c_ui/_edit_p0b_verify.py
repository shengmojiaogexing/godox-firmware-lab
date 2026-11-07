# -*- coding: utf-8 -*-
"""适配 page0b：tiles_end label；verify 断言/桩测试加页面门与控件树校验。"""
import io
import os

HERE = os.path.dirname(os.path.abspath(__file__))
P = os.path.join(HERE, "ui_patch.py")
t = io.open(P, encoding="utf-8").read()


def rep(old, new):
    global t
    assert old in t, "NOT FOUND: " + old[:100]
    t = t.replace(old, new, 1)


# 1) tiles_end 标签
rep('''        c.label(skip)
    c.pop(0x170)
    return c.finish()''',
    '''        c.label(skip)
    c.label("tiles_end")
    c.pop(0x170)
    return c.finish()''')

# 2) poll 静态断言：动作调用序列改为 [IS_WINDOW, key_toggle, IS_WINDOW, broadcast]
rep('''    ok = (struct.pack("<I", HANDLE_TABLE) in code
          and "ldrh r1, [r5, #0x28]" in ptext
          and ("cmp r2, #%d" % FALLBACK_LONG_TICKS) in ptext
          and tg == [CAVE + OFF_KEY_TOGGLE, CAVE + OFF_BROADCAST_ALLCH]
          and tg.count(UI_CREATE_WIDGET) == 0)
    out.append((ok, "fallback poll: 句柄表字面量/按下位/长按阈值/动作调用 均正确=%s" % ok))''',
    '''    ok = (struct.pack("<I", HANDLE_TABLE) in code
          and struct.pack("<I", RAM_UI_PAGE) in code
          and "ldrh r1, [r5, #0x28]" in ptext
          and ("cmp r2, #%d" % FALLBACK_LONG_TICKS) in ptext
          and tg == [IS_WINDOW, CAVE + OFF_KEY_TOGGLE, IS_WINDOW,
                     CAVE + OFF_BROADCAST_ALLCH]
          and tg.count(UI_CREATE_WIDGET) == 0)
    out.append((ok, "fallback poll: 句柄表/页面门(0x%08X)/按下位/长按阈值/"
                    "控件树校验(0x%08X)×2/动作调用 均正确=%s"
                % (RAM_UI_PAGE, IS_WINDOW, ok)))''')

# 3) tiles 静态断言：页面门 + 2× show
rep('''        ok = (struct.pack("<I", HANDLE_TABLE) in tcode
              and ttg.count(UI_CREATE_WIDGET) == 2
              and ttg.count(UI_SET_TEXT) == 2
              and ttg.count(0x0802553E) == 0
              and "cmp r5, #0" in ttxt)
        out.append((ok, "tiles0: 父窗口取自 0x%08X（字面量=%s）、2×建磁贴、2×文本、"
                        "0 个图标子控件、父窗口判 0=%s"
                    % (HANDLE_TABLE, struct.pack("<I", HANDLE_TABLE) in tcode,
                       "cmp r5, #0" in ttxt)))''',
    '''        ok = (struct.pack("<I", HANDLE_TABLE) in tcode
              and struct.pack("<I", RAM_UI_PAGE) in tcode
              and ttg.count(UI_CREATE_WIDGET) == 2
              and ttg.count(UI_SET_TEXT) == 2
              and ttg.count(0x0802553E) == 0
              and ttg.count(WIDGET_SHOW) == 2
              and "cmp r5, #0" in ttxt)
        out.append((ok, "tiles0: 页面门(0x%08X)=%s、父窗口取自 0x%08X、2×建磁贴、2×文本、"
                        "2×show(0x%08X)、0 个图标子控件、父窗口判 0=%s"
                    % (RAM_UI_PAGE, struct.pack("<I", RAM_UI_PAGE) in tcode,
                       HANDLE_TABLE, WIDGET_SHOW,
                       ttg.count(WIDGET_SHOW) == 2)))''')

# 4) poll 桩测试：设页面字节 + stub 控件树校验
rep('''    def hook(uc_, address, size, user):
        if address == CAVE + OFF_BROADCAST_ALLCH:''',
    '''    def hook(uc_, address, size, user):
        if address == IS_WINDOW:
            uc_.reg_write(UC_ARM_REG_R0, 1 if uc_.reg_read(UC_ARM_REG_R0) else 0)
            uc_.reg_write(UC_ARM_REG_PC, uc_.reg_read(UC_ARM_REG_LR))
            return
        if address == CAVE + OFF_BROADCAST_ALLCH:''')

rep('''    uc.mem_write(DEFAULT_KEY_FLAG, bytes(1))

    def hook''',
    '''    uc.mem_write(DEFAULT_KEY_FLAG, bytes(1))
    uc.mem_write(RAM_UI_PAGE, bytes(1))          # 页面门：主界面(0)

    def hook''')

# 5) tiles 桩测试：页面字节=0 + stub show
rep('''    def hook(uc_, address, size, user):
        r0, r1, r2 = (uc_.reg_read(UC_ARM_REG_R0), uc_.reg_read(UC_ARM_REG_R1),
                      uc_.reg_read(UC_ARM_REG_R2))
        if address == 0x08027FE0:''',
    '''    shows = []

    def hook(uc_, address, size, user):
        r0, r1, r2 = (uc_.reg_read(UC_ARM_REG_R0), uc_.reg_read(UC_ARM_REG_R1),
                      uc_.reg_read(UC_ARM_REG_R2))
        if address == WIDGET_SHOW:
            shows.append((r0, r1))
            uc_.reg_write(UC_ARM_REG_PC, uc_.reg_read(UC_ARM_REG_LR))
            return
        if address == 0x08027FE0:''')

rep('''    uc.mem_write(HANDLE_TABLE, struct.pack("<I", PARENT))       # [0] = 根窗口''',
    '''    uc.mem_write(HANDLE_TABLE, struct.pack("<I", PARENT))       # [0] = 根窗口
    uc.mem_write(RAM_UI_PAGE, bytes(1))                              # 主界面(0)''')

rep('''    ok = parent_ok and all(labels_ok) and len(texts) == len(TILE_SPECS) and setter_ok
    return ok, ("page0 tiles 桩执行: parent=0x%08X 正确=%s；建磁贴 %d 个；setter %d 次"
                "（期望 %d）；文本指针正确=%s"
                % (PARENT, parent_ok, len(created), len(calls),
                   12 * len(TILE_SPECS), all(labels_ok)))''',
    '''    show_ok = len(shows) == len(TILE_SPECS) and all(s[1] == 1 for s in shows)
    ok = (parent_ok and all(labels_ok) and len(texts) == len(TILE_SPECS)
          and setter_ok and show_ok)
    return ok, ("page0 tiles 桩执行: parent=0x%08X 正确=%s；建磁贴 %d 个；setter %d 次"
                "（期望 %d）；文本指针正确=%s；show(w,1) %d 次（期望 %d）"
                % (PARENT, parent_ok, len(created), len(calls),
                   12 * len(TILE_SPECS), all(labels_ok), len(shows),
                   len(TILE_SPECS)))''')

io.open(P, "w", encoding="utf-8", newline="\n").write(t)
print("page0b verify adapted")
