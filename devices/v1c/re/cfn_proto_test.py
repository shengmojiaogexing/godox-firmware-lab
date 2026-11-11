# -*- coding: utf-8 -*-
"""cfn_proto_test：用 Unicorn 验证 C.Fn 版补丁原型（同步字函数 flag 语义 + 注入 hook 行为）。"""
import sys
import os

from unicorn import (Uc, UcError, UC_ARCH_ARM, UC_MODE_MCLASS, UC_HOOK_CODE,
                     UC_PROT_ALL)
from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                               UC_ARM_REG_R3, UC_ARM_REG_SP, UC_ARM_REG_LR,
                               UC_ARM_REG_PC)

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import cfn_proto as P

FW = r'devices/v1c/firmware/V1C_V2.0.bin'
ROM, ROM_SIZE = 0x08000000, 0x20000
RAM, RAM_SIZE = 0x20000000, 0x4000
STACK = 0x20001400
SENT = 0x20000400
KEY = 0xD798


def make_uc(image):
    uc = Uc(UC_ARCH_ARM, UC_MODE_MCLASS)
    uc.mem_map(ROM, ROM_SIZE, UC_PROT_ALL)
    uc.mem_map(RAM, RAM_SIZE, UC_PROT_ALL)
    uc.mem_write(P.BASE, image)
    uc.mem_write(SENT, bytes.fromhex('fee7'))      # b .
    for addr in (P.WRITE_REG, P.DELAY_US, 0x08015790):
        uc.mem_write(addr, bytes.fromhex('7047'))  # bx lr
    return uc


def run_sync(image, ident, flag, flag_idx):
    uc = make_uc(image)
    uc.mem_write(P.SETTINGS + flag_idx, bytes([flag]))
    calls = []

    def hook(_uc, address, _size, _user):
        if address == SENT:
            _uc.emu_stop()
            return
        if address == P.WRITE_REG:
            calls.append(('wreg', _uc.reg_read(UC_ARM_REG_R0), _uc.reg_read(UC_ARM_REG_R1)))
        elif address == P.DELAY_US:
            calls.append(('delay', _uc.reg_read(UC_ARM_REG_R0), None))

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.reg_write(UC_ARM_REG_SP, STACK)
    uc.reg_write(UC_ARM_REG_LR, SENT | 1)
    uc.reg_write(UC_ARM_REG_R0, ident)
    uc.emu_start(P.SYNC_FN | 1, 0, 0, 20000)
    return calls


def run_inj(image, item, flag_idx):
    uc = make_uc(image)
    uc.mem_write(P.SETTINGS + 6, bytes([37]))    # settings[6] = ID = 37
    sync_calls = []

    def hook(_uc, address, _size, _user):
        if address == SENT:
            _uc.emu_stop()
            return
        if address == P.SYNC_FN:
            sync_calls.append(('sync', _uc.reg_read(UC_ARM_REG_R0)))
            _uc.reg_write(UC_ARM_REG_PC, _uc.reg_read(UC_ARM_REG_LR))

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.reg_write(UC_ARM_REG_SP, STACK)
    uc.reg_write(UC_ARM_REG_LR, SENT | 1)
    uc.reg_write(UC_ARM_REG_R0, P.SETTINGS)     # 与 handler 内一致：settings 基址
    uc.reg_write(UC_ARM_REG_R1, item)           # 当前 C.Fn 项
    uc.reg_write(UC_ARM_REG_R2, 0x1234)
    uc.emu_start(P.INJ | 1, 0, 0, 2000)
    return sync_calls, uc.reg_read(UC_ARM_REG_R0), uc.reg_read(UC_ARM_REG_R1)


def main():
    d = open(FW, 'rb').read()
    ok = True
    # 原厂自证
    for ident in (0, 1, 5, 37, 99):
        calls = run_sync(d, ident, 0, 7)
        want = [('wreg', 4, (50024 // (ident + 1)) >> 8), ('wreg', 5, 50024 // (ident + 1) & 0xFF),
                ('delay', 20, None)]
        # 原厂函数有 SCAL/1ms，先只比较前 3 个
        got = calls[:3]
        p = got == want
        ok = ok and p
        print('[%s] 原厂 ID=%-2d 前3调用 %s (期望 %s) 全部=%s' % (
            'PASS' if p else 'FAIL', ident, got, want,
            [(k, a, b) for k, a, b in calls]))
    for flag_idx in (7, 8, 10, 11):
        img = P.apply(d, KEY, flag_idx)
        for flag in (0, 1):
            for ident in (0, 1, 5, 37, 99):
                calls = run_sync(img, ident, flag, flag_idx)
                want_sync = KEY if flag else 50024 // (ident + 1)
                want = [('wreg', 4, want_sync >> 8), ('wreg', 5, want_sync & 0xFF)]
                p = calls == want
                ok = ok and p
                if not p:
                    print('[FAIL] flag_idx=%d flag=%d ID=%d calls=%s want=%s'
                          % (flag_idx, flag, ident, calls, want))
        # 注入 hook 行为
        for item, expect in ((6, 1), (flag_idx, 1), (5, 0), (11 if flag_idx != 11 else 10, 0)):
            sc, r0, r1 = run_inj(img, item, flag_idx)
            p = (len(sc) == expect and (not sc or sc[0][1] == 37) and
                 r0 == P.SETTINGS and r1 == item)
            ok = ok and p
            print('[%s] flag_idx=%d 注入 item=%d -> sync调用=%s r0=0x%08X r1=%d'
                  % ('PASS' if p else 'FAIL', flag_idx, item, sc, r0, r1))
        print('[PASS] flag_idx=%d 同步字函数 flag=0/1 × ID 0/1/5/37/99 全部符合预期' % flag_idx
              if ok else '[FAIL] flag_idx=%d 有失败项' % flag_idx)
    print('RESULT:', 'PASS' if ok else 'FAIL')
    return 0 if ok else 1


if __name__ == '__main__':
    sys.exit(main())
