# -*- coding: utf-8 -*-
import os, struct, sys
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))  # devices/ad100pro
path = os.path.join(ROOT, 'firmware', 'AD100pro_V1.2.bin')
data = open(path, 'rb').read()
print('file size', len(data))
print('first 16 bytes', data[:16].hex())
# disasm.txt says flash_08003000 maps to file offset 0
base = 0x08003000
def rd(addr, n):
    off = addr - base
    return data[off:off+n]
def words(addr, cnt):
    b = rd(addr, cnt*4)
    return struct.unpack('<%dI' % cnt, b)
print('\n--- first 0x30 words at 0x08003000 ---')
ws = words(0x08003000, 0x30)
names = ['SP','Reset','NMI','HardFault'] + ['rsv%d'%i for i in range(4)] + ['SVCall','rsv8','rsv9','PendSV','SysTick']
for i, w in enumerate(ws):
    nm = names[i] if i < len(names) else 'IRQ%d' % (i-16)
    print('%02d 0x%08X  %s' % (i, w, nm))
print('\n--- init_array table 0x0800F3EC .. 0x0800F40C ---')
ws = words(0x0800F3EC, 8)
for i, w in enumerate(ws):
    print('%02d 0x%08X' % (i, w))
