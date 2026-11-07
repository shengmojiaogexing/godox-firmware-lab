"""g3_patch_check.py -- reproduce G3 V2.0's flash-time framing/checksums for a raw .bin.

This is a *simulator of the G3 side* (no device contact). It demonstrates that:
  1. G3 never sends a whole-image size/CRC/version/signature for the legacy (raw .bin) path;
  2. the only content-derived value is the per-1024-byte 32-bit sum, which G3 computes
     from the file it sends (FUN_004082e0 case 8, cmd 0x04 payload);
  3. patching bytes in place only changes the sums of the affected blocks, and G3 will
     recompute them from the patched file, so a device checking those sums stays happy.

Reported evidence addresses (Godox V2.0.exe, image base 0x400000):
  - 0x407610 FUN_00407610  packet builder: AA 55 cmd payload sum_be32 in a 65-byte HID report
  - 0x4082e0 SendDataLooper  state machine incl. per-block sum (case 8) at 0x4086a0/0x408891
  - 0x408523 legacy check: if byte[this+0x9a]==0 (no A1 handshake reply) skip container parse
  - 0x407990 container parser (MD5 + magic) - only enforced for V2.0 (A1) devices

Usage:
  python g3_patch_check.py                    # default AD100pro sample
  python g3_patch_check.py <bin> [<bin2> ...]
Optional: --patch OFFSET LEN  (e.g. --patch 0x1234 6) shows before/after sums.
"""
import argparse
import sys

BLOCK = 0x400
MAGIC = bytes.fromhex('aa55669977882233')


def block_sums(data: bytes):
    """G3 legacy per-block checksum: sum of up to 1024 bytes, out-of-range bytes = 0xFF."""
    n = len(data)
    out = []
    blocks = (n + BLOCK - 1) // BLOCK
    for i in range(blocks):
        s = 0
        for j in range(BLOCK):
            off = i * BLOCK + j
            s = (s + (data[off] if off < n else 0xFF)) & 0xFFFFFFFF
        idx = i
        cmd = 4
        if blocks > 0xFF:
            cmd = 4 | ((idx >> 4) & 0xF0)
        payload = bytes([idx & 0xFF]) + s.to_bytes(4, 'big')
        out.append((i, cmd, payload, s))
    return out


def frame(cmd: int, payload: bytes = b'') -> bytes:
    buf = bytearray(65)
    buf[0] = 0x00            # HID report ID
    buf[1] = 0xAA
    buf[2] = 0x55
    buf[3] = cmd
    buf[4:4 + len(payload)] = payload
    s = sum(buf[1:0x3D]) & 0xFFFFFFFF
    buf[0x3D:0x41] = s.to_bytes(4, 'big')
    return bytes(buf)


def v20_block_frame(idx: int, s: int) -> bytes:
    return frame(8, idx.to_bytes(4, 'big') + s.to_bytes(4, 'big'))


def is_container(data: bytes):
    """Recognise the V2.0 container: payload || MD5(payload) || name || u16 L || magic."""
    if len(data) < 10 or data[-8:] != MAGIC:
        return None
    L = int.from_bytes(data[-10:-8], 'little')
    if L < 0 or len(data) < L + 26:
        return None
    name = data[-10 - L:-10]
    digest = data[-10 - L - 16:-10 - L]
    payload = data[:-10 - L - 16]
    import hashlib
    ok = hashlib.md5(payload).digest() == digest
    return dict(name=name, L=L, md5_ok=ok, payload_len=len(payload), container_len=len(data))


def report(path, patch=None):
    data = open(path, 'rb').read()
    print('=' * 78)
    print(path, len(data), 'bytes')
    c = is_container(data)
    if c:
        print('  CONTAINER (.bin for a V2.0/A1 device):', c)
        print('  -> G3 would flash only the first %d payload bytes.' % c['payload_len'])
        data = data[:c['payload_len']]
    else:
        print('  RAW app image (legacy path): no container magic -> G3 flashes whole file')
        print('  container parse at 0x407990 returns 0; at 0x408523 the legacy branch'
              ' (byte[+0x9a]==0) ignores that failure')
    sums = block_sums(data)
    print('  blocks:', len(sums), '(1 KiB each)')
    print('  first block sum: 0x%08x  payload %s' % (sums[0][3], sums[0][2].hex()))
    print('  last  block sum: 0x%08x  payload %s' % (sums[-1][3], sums[-1][2].hex()))
    # what G3 actually transmits that depends on content:
    print('  whole-image values sent to the device:')
    print('    cmd 0x00 payload: none (frame %s)' % frame(0).hex())
    print('    cmd 0x02 payload: none (frame %s)' % frame(2).hex())
    print('    size/CRC/version/signature fields: NONE')
    if patch:
        off, ln = patch
        if off + ln > len(data):
            print('  patch out of range'); return
        patched = bytearray(data)
        for i in range(ln):
            patched[off + i] ^= 0xFF
        sums2 = block_sums(bytes(patched))
        changed = [i for i, (a, b) in enumerate(zip(sums, sums2)) if a[3] != b[3]]
        print('  in-place patch of %d bytes at 0x%x (length unchanged):' % (ln, off))
        print('    blocks whose transmitted sum changes: %s' % changed)
        print('    -> G3 recomputes these sums from the patched file before sending,')
        print('       so the device-side per-block check cannot be the rejection source.')


if __name__ == '__main__':
    ap = argparse.ArgumentParser()
    ap.add_argument('bins', nargs='*', default=[r'devices\ad100pro\firmware\AD100pro_V1.2.bin'])
    ap.add_argument('--patch', nargs=2, type=lambda s: int(s, 0))
    a = ap.parse_args()
    for p in a.bins:
        report(p, tuple(a.patch) if a.patch else None)
