"""make_container.py -- wrap a raw app image into the Godox G3 V2.0 firmware container.

The G3 V2.0 updater only requires this container when the attached device answers the
`AA 55 A0` probe with `AA 55 A1` (internal "V2.0 device" flag, G3 byte [+0x9a]).
Container layout (verified against the official iT32_V1.18.bin download):

    payload                      raw application image
    MD5(payload)                 16 bytes
    filename                    L bytes, UTF-8 (e.g. b"iT32_V1.18.bin")
    L                           u16 little-endian
    AA 55 66 99 77 88 22 33     8-byte magic at end of file

G3 parses it in FUN_00407990 (0x407990):
  - reads last 10 bytes -> L + magic
  - reads 20/16-byte digest at (filesize - L - 26) and MD5-hashes [0 .. filesize-L-26)
  - reads the L-byte name and does name.indexOf(device_id) (0x408534..0x40855e);
    the name must CONTAIN the device ID string reported by the device (e.g. "iT32").
  - payload length = filesize - (L + 26); footer bytes are NOT flashed.

Usage:
  python make_container.py <raw.bin> <out.bin> [--name NAME] [--device-id ID]
  python make_container.py --verify <container.bin>
"""
import argparse
import hashlib
import sys

MAGIC = bytes.fromhex('aa55669977882233')


def build(raw: bytes, name: str) -> bytes:
    n = name.encode('utf-8')
    if len(n) > 0xFFFF:
        raise ValueError('name too long')
    digest = hashlib.md5(raw).digest()
    return raw + digest + n + len(n).to_bytes(2, 'little') + MAGIC


def verify(path: str):
    d = open(path, 'rb').read()
    if d[-8:] != MAGIC:
        print('NOT a G3 V2.0 container (magic missing)')
        return 1
    L = int.from_bytes(d[-10:-8], 'little')
    name = d[-10 - L:-10]
    digest = d[-10 - L - 16:-10 - L]
    payload = d[:-10 - L - 16]
    ok = hashlib.md5(payload).digest() == digest
    print('container ok:', ok)
    print('  total=%d payload=%d L=%d name=%r md5=%s' %
          (len(d), len(payload), L, name, digest.hex()))
    return 0 if ok else 2


if __name__ == '__main__':
    ap = argparse.ArgumentParser()
    ap.add_argument('raw', nargs='?')
    ap.add_argument('out', nargs='?')
    ap.add_argument('--name', default=None, help='embedded file name (must contain device ID)')
    ap.add_argument('--device-id', default=None, help='only used to sanity-check name')
    ap.add_argument('--verify', default=None)
    a = ap.parse_args()
    if a.verify:
        sys.exit(verify(a.verify))
    if not a.raw or not a.out:
        ap.error('need <raw.bin> <out.bin> or --verify <container.bin>')
    raw = open(a.raw, 'rb').read()
    name = a.name or __import__('os').path.basename(a.out)
    if a.device_id and a.device_id not in name:
        print('WARNING: name %r does not contain device id %r -> G3 rejects "not for this device"'
              % (name, a.device_id))
    out = build(raw, name)
    open(a.out, 'wb').write(out)
    print('wrote %s (%d bytes, payload %d)' % (a.out, len(out), len(raw)))
    verify(a.out)
