"""Firmware image loader + symbol/metadata helpers for the AD200 Pro emulator."""
import os
import struct
import json

BASE = 0x08003000          # link address of the update image
FLASH_BASE = 0x08000000
FLASH_SIZE = 0x20000       # 128 KiB (device has the bootloader in the first 12 KiB)
RAM_BASE = 0x20000000
RAM_SIZE = 0x2000

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DEFAULT_BIN = os.path.join(ROOT, 'firmware', 'AD200pro_V1.60.bin')
SYMBOLS_JSON = os.path.join(ROOT, 'analysis', 'symbols.json')


class Firmware:
    def __init__(self, path=None):
        self.path = path or DEFAULT_BIN
        self.data = open(self.path, 'rb').read()
        self.base = BASE
        self.size = len(self.data)
        # bootloader region (0x08000000..0x08002FFF) is *not* part of the update file
        self.image = bytearray(b'\xff' * FLASH_SIZE)
        self.image[BASE - FLASH_BASE: BASE - FLASH_BASE + self.size] = self.data
        self.vector_table = struct.unpack_from('<48I', self.data, 0)
        self.reset_sp = self.vector_table[0]
        self.reset_pc = self.vector_table[1] & ~1
        self.symbols = {}          # addr -> name
        self.vector_names = []
        self._load_symbols()

    def _load_symbols(self):
        if not os.path.exists(SYMBOLS_JSON):
            return
        try:
            d = json.load(open(SYMBOLS_JSON, encoding='utf-8'))
        except Exception:
            return
        for k, v in d.get('symbols', {}).items():
            try:
                self.symbols[int(k, 16)] = v
            except ValueError:
                pass
        for v in d.get('vectors', []):
            self.vector_names.append(v.get('name') or '')

    def name(self, addr):
        if addr in self.symbols:
            return self.symbols[addr]
        # nearest symbol below
        best = None
        for a in self.symbols:
            if a <= addr and (best is None or a > best):
                best = a
        if best is not None and addr - best < 0x400:
            return '%s+0x%x' % (self.symbols[best], addr - best)
        return None

    def label(self, addr):
        n = self.name(addr)
        return '%s (0x%08x)' % (n, addr) if n else '0x%08x' % addr

    def word(self, addr):
        off = addr - self.base
        if 0 <= off <= self.size - 4:
            return struct.unpack_from('<I', self.data, off)[0]
        return None
