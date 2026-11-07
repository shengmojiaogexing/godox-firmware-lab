#!/usr/bin/env python3
"""Boot self-test / trace: run the firmware from reset and report where it goes.

Usage: python -m sim.boot_trace [max_insns]
"""
import sys
import collections
from .machine import Machine, StopRun

MAIN_LO, MAIN_HI = 0x0800CE40, 0x0800CEB0


def main():
    max_insns = int(sys.argv[1]) if len(sys.argv) > 1 else 2_000_000
    m = Machine()
    print('reset: sp=0x%08x pc=0x%08x (%s)' % (
        m.fw.reset_sp, m.fw.reset_pc, m.fw.name(m.fw.reset_pc)))
    hist = collections.Counter()
    main_hits = 0
    last_pc = None
    reason = None
    step = 20000
    done = 0
    while done < max_insns:
        r = m.run(max_insns=step, stop_on_stall=False)
        done += step
        st = m.stats()
        pc = st['pc']
        hist[pc >> 8] += 1
        print('  t=%6.1fms insns=%8d pc=0x%08x %-28s state=%s %s' % (
            st['uptime_ms'], st['insns'], pc, st['pc_name'] or '', m.state,
            m.stop_reason or ''))
        if MAIN_LO <= pc < MAIN_HI:
            main_hits += 1
        if r not in ('insn_limit',):
            reason = r
            break
        if main_hits >= 3:
            reason = 'main_loop'
            break
        if main_hits == 0 and pc == last_pc:
            pass
        last_pc = pc
    print('\nresult: reason=%s main_hits=%d insns=%d uptime=%.1fms exc_active=%s' % (
        reason, main_hits, m.insn_count, m.insn_count / m.systick_insn_per_ms, m.exc_active))
    print('top pc pages:', [(('0x%08x' % (p << 8)), n) for p, n in hist.most_common(8)])
    print('sysTick pending:', m.systick_pending,
          'irq pending:', m.bus.peripheral_state()['irq_pending'],
          'irq enabled:', m.bus.peripheral_state()['irq_enabled'])
    print('trace tail:')
    for e in m.tracer.last(15):
        print('  ', e['type'], e['name'], 'pc=0x%08x' % e['pc'] if e['pc'] else '')
    return 0 if reason == 'main_loop' else 1


if __name__ == '__main__':
    sys.exit(main())
