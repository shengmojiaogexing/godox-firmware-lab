import os, sys, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))  # tools/ (同目录 analyze.py)
import analyze as A
A.run()

def dump(addr, maxins=300):
    f = A.funcs.get(addr)
    if not f:
        print('no func at 0x%08x' % addr); return
    print('\n==== FUNC 0x%08x  %d insns  calls=%s' % (addr, len(f['insns']), [hex(c) for c in sorted(f['calls'])]))
    if f['strings']: print('   strings:', f['strings'])
    print('   mmio:', sorted(set('%s:%s' % (m['rw'], m['name']) for m in f['mmio'])))
    for ins in f['insns'][:maxins]:
        print('%08x  %-8s %s' % (ins.address, ins.bytes.hex(), ins.mnemonic + ' ' + ins.op_str))
        lt = A.lit_target(ins)
        if lt is not None:
            v = A.rd32(lt)
            s = A.ascii_at(v) if v and A.in_img(v) else None
            print('%08x    ; lit 0x%08x = 0x%08x (%s)%s' % (ins.address, lt, v, A.classify(v), ' "%s"' % s if s else ''))

for a in sys.argv[1:]:
    dump(int(a, 0))
