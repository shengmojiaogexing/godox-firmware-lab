import os
import re

path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'machine.py')
p = open(path, encoding='utf-8').read()

# ---- 1) replace the DMA class with a functional transfer engine
start = p.find('class DMA(Peripheral):')
end = p.find('class USB(Peripheral):')
assert start > 0 and end > start
new_dma = '''class DMA(Peripheral):
    """DMA controller (STM32F4/AT32 stream layout) with real transfers.

    Each stream window starts at 0x10 + 0x18*idx and contains:
        +0x00 SxCR, +0x04 SxNDTR, +0x08 SxPAR, +0x0C SxMAR, +0x10 SxFCR
    Writing SxCR with EN=1 performs the transfer immediately through the
    machine's unified memory dispatcher, so a transfer whose peripheral side is
    the XMC LCD window (0x6C000001) feeds the display model.  EN self-clears,
    NDTR becomes 0 and TCIF is raised (flags live in LISR/HISR, 6 bits/stream).
    """

    def __init__(self, machines, name, base, nstreams=8):
        self.nstreams = nstreams
        self.lisr = 0
        self.hisr = 0
        self.streams = [dict(regs={}, ndtr=0) for _ in range(nstreams)]
        self.transfers = []
        self.enabled = True
        super().__init__(machines, name, base, 0x400)

    def reset(self):
        self.lisr = 0
        self.hisr = 0
        for s in self.streams:
            s["regs"] = {}
            s["ndtr"] = 0

    def _stream_index(self, offset):
        if offset < 0x10:
            return None
        rel = offset - 0x10
        return rel // 0x18, rel % 0x18

    def _flag_base(self, idx):
        return idx * 6 if idx < 4 else (idx - 4) * 6

    def _set_flag(self, idx, bit):
        base = self._flag_base(idx)
        if idx < 4:
            self.lisr |= (1 << (base + bit))
        else:
            self.hisr |= (1 << (base + bit))

    # -- register access ----------------------------------------------
    def read(self, offset, size):
        if offset == 0x00:
            return self.lisr
        if offset == 0x04:
            return self.hisr
        if offset == 0x08 or offset == 0x0C:
            return 0
        si = self._stream_index(offset)
        if si is None:
            return 0
        idx, ro = si
        s = self.streams[idx]
        if ro == 0x00:
            return s["regs"].get(0x00, 0) & ~1     # EN reads 0
        if ro == 0x04:
            return s["ndtr"]
        return s["regs"].get(ro, 0)

    def write(self, offset, size, value):
        if offset == 0x08:               # LIFCR
            self.lisr &= ~(value & 0x0FFFFFFF)
            return
        if offset == 0x0C:               # HIFCR
            self.hisr &= ~(value & 0x0FFFFFFF)
            return
        if offset == 0x00 or offset == 0x04:
            return
        si = self._stream_index(offset)
        if si is None:
            return
        idx, ro = si
        s = self.streams[idx]
        if ro == 0x04:
            s["ndtr"] = value & 0xFFFF
        elif ro == 0x00:                 # SxCR
            s["regs"][0x00] = value & ~1
            if (value & 1) and self.enabled:
                self._run_transfer(idx)
        else:
            s["regs"][ro] = value

    # -- transfer engine ---------------------------------------------
    def _run_transfer(self, idx):
        s = self.streams[idx]
        cr = s["regs"].get(0x00, 0)
        ndtr = s["ndtr"] & 0xFFFF
        par = s["regs"].get(0x08, 0)
        mar = s["regs"].get(0x0C, 0)
        dir_ = (cr >> 6) & 3
        pinc = (cr >> 9) & 1
        minc = (cr >> 10) & 1
        psize = 1 << ((cr >> 11) & 3)
        msize = 1 << ((cr >> 13) & 3)
        if ndtr > 0x100000:
            self.log("transfer truncated NDTR=%d" % ndtr)
            ndtr = 0x100000
        if dir_ == 0:            # peripheral -> memory
            src, dst = par, mar
            src_inc, src_sz = pinc, psize
            dst_inc, dst_sz = minc, msize
        elif dir_ == 1:          # memory -> peripheral
            src, dst = mar, par
            src_inc, src_sz = minc, msize
            dst_inc, dst_sz = pinc, psize
        else:                    # memory -> memory (MAR source)
            src, dst = mar, par
            src_inc, src_sz = minc, msize
            dst_inc, dst_sz = pinc, psize
        ok = True
        try:
            for _ in range(ndtr):
                val = self.m.read_mem(src, src_sz)
                self.m.write_mem(dst, dst_sz, val)
                if src_inc:
                    src += src_sz
                if dst_inc:
                    dst += dst_sz
        except Exception as e:
            ok = False
            self.log("DMA transfer error: %r" % (e,))
        s["ndtr"] = 0
        self._set_flag(idx, 5)           # TCIF
        self.transfers.append(dict(vtime=self.m.vtime, stream=idx, dir=dir_,
                                   ndtr=ndtr, par=par, mar=mar,
                                   psize=psize, msize=msize, ok=ok))
        if len(self.transfers) > 2000:
            del self.transfers[:1000]

    def summary(self, n=20):
        lines = ["DMA transfers: %d" % len(self.transfers)]
        for t in self.transfers[-n:]:
            lines.append("  t=%-10d stream=%d dir=%d ndtr=%-6d PAR=%#010x "
                         "MAR=%#010x sz=%d/%d ok=%s"
                         % (t["vtime"], t["stream"], t["dir"], t["ndtr"],
                            t["par"], t["mar"], t["psize"], t["msize"],
                            t["ok"]))
        return "\\n".join(lines)


'''
p = p[:start] + new_dma + p[end:]

# ---- 2) register functional DMA for DMA1/DMA2, drop the RegFile versions
p = p.replace('add(RegFile(self, "DMA1", 0x40026000, 0x400))',
              'add(DMA(self, "DMA1", 0x40026000, 8))')
p = p.replace('add(RegFile(self, "DMA2", 0x40026400, 0x400))',
              'add(DMA(self, "DMA2", 0x40026400, 8))')
p = p.replace('        add(DMA(self, "DMA1_ctrl", 0x40026000, 8))\n', '')
p = p.replace('        # remove duplicate DMA1 RegFile above? keep first match; dedupe below\n', '')

# ---- 3) register XmcDisplay instead of LcdFmc
old_lcd = '''        # ---- FMC-attached LCD controller (observed writes at 0x6C000000)
        try:
            import display as _display
            self.lcd_model = _display.ParallelLcd(width=240, height=240,
                                                  ctrl="unknown",
                                                  base=0x6C000000)
        except Exception:
            self.lcd_model = None
        self.lcd = LcdFmc(self)
        self.lcd.lcd = self.lcd_model
        add(self.lcd)'''
new_lcd = '''        # ---- AT32 XMC-attached display controller (writes at 0x6C000000)
        try:
            from xmc import XmcDisplay
            self.lcd_model = XmcDisplay(self, width=240, height=240)
            add(self.lcd_model)
        except Exception as _e:
            self.log("xmc display model unavailable: %r" % (_e,))
            self.lcd_model = None
        self.lcd = self.lcd_model'''
assert old_lcd in p
p = p.replace(old_lcd, new_lcd)

# ---- 4) wire the latch GPIO listener after gpio dict exists
old_tail = '        self.timers = {p.name: p for p in self.periphs if isinstance(p, Timer)}'
assert old_tail in p
new_tail = old_tail + '''
        if getattr(self, "lcd_model", None) is not None:
            try:
                self.lcd_model.attach_latch_gpio()
            except Exception:
                pass'''
p = p.replace(old_tail, new_tail)

open(path, 'w', encoding='utf-8').write(p)
print("patched machine.py")
