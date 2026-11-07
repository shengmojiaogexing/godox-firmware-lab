#!/usr/bin/env python3
"""ghidra_finish.py -- finish the Ghidra zip download with small parallel piece requests.

Reads the existing chunk_*.bin prefix files (36 chunks of 15825353 bytes except last),
downloads the remaining byte ranges as ~1MB pieces (unique temp file per piece, several
mirrors, retries), appends them in order, then assembles the zip and verifies size+sha256.

Usage: tools/python/python.exe ghidra_finish.py
"""
import hashlib
import os
import subprocess
import sys
import threading
from concurrent.futures import ThreadPoolExecutor

BASE = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
DL = os.path.join(BASE, "tools", "ghidra_dl")
OUT = os.path.join(BASE, "tools", "ghidra_12.2_PUBLIC.zip")
SIZE = 569712708
CHUNKS = 36
CHUNK = 15825353
SHA = "2c2cd776db3c1f6a097ba418922378ac3649cadd5e844a6f5f088ae628a7be67"
URL = ("https://github.com/NationalSecurityAgency/ghidra/releases/download/"
       "Ghidra_12.2_build/ghidra_12.2_PUBLIC_20261016.zip")
PROXIES = ["https://gh-proxy.com/", "https://gh.monlor.com/"]
PIECE = 1 << 20


def kill_curls():
    ps = subprocess.run(["powershell", "-NoProfile", "-Command",
                         "Get-CimInstance Win32_Process | Where-Object { $_.Name -eq 'curl.exe' -and "
                         "$_.CommandLine -match 'ghidra_dl' } | ForEach-Object { Stop-Process -Id $_.ProcessId -Force }"],
                        capture_output=True, text=True)
    return ps.returncode


def seed_from_tmp():
    """append any tmp bytes to their chunk bin (they are continuations), delete tmp."""
    n = 0
    for i in range(CHUNKS):
        tmp = os.path.join(DL, "chunk_%03d.tmp" % i)
        if not os.path.exists(tmp):
            continue
        try:
            sz = os.path.getsize(tmp)
        except OSError:
            continue
        if sz <= 0:
            os.remove(tmp)
            continue
        fn = os.path.join(DL, "chunk_%03d.bin" % i)
        src = open(tmp, "rb")
        dst = open(fn, "ab")
        dst.write(src.read())
        src.close()
        dst.close()
        os.remove(tmp)
        n += 1
    return n


def chunk_len(i):
    fn = os.path.join(DL, "chunk_%03d.bin" % i)
    return os.path.getsize(fn) if os.path.exists(fn) else 0


def fetch_piece(idx, start, end, results):
    target = os.path.join(DL, "piece_%05d.bin" % idx)
    need = end - start + 1
    for attempt in range(10):
        proxy = PROXIES[(idx + attempt) % len(PROXIES)]
        if os.path.exists(target):
            os.remove(target)
        subprocess.run(["curl.exe", "-s", "-L", "--max-time", "45",
                        "-r", "%d-%d" % (start, end), "-o", target, proxy + URL],
                       capture_output=True)
        if os.path.exists(target) and os.path.getsize(target) == need:
            results[idx] = True
            return
    results[idx] = False


def main():
    kill_curls()
    time_sleep = __import__("time").sleep
    time_sleep(1)
    print("seeded tmp chunks:", seed_from_tmp())

    missing = []
    for i in range(CHUNKS):
        s = i * CHUNK
        e = min(s + CHUNK, SIZE) - 1
        exp = e - s + 1
        have = chunk_len(i)
        if have > exp:
            print("WARN chunk %d oversized %d > %d (truncating)" % (i, have, exp))
            with open(os.path.join(DL, "chunk_%03d.bin" % i), "r+b") as f:
                f.truncate(exp)
            have = exp
        if have < exp:
            missing.append((i, s + have, e))
    total = sum(e - s + 1 for _, s, e in missing)
    print("chunks complete: %d/%d, missing %.1f MB in %d ranges"
          % (CHUNKS - len(missing), CHUNKS, total / 1e6, len(missing)))
    if not missing:
        print("nothing to fetch")
    else:
        pieces = []
        for (i, s, e) in missing:
            pos = s
            while pos <= e:
                end = min(pos + PIECE - 1, e)
                pieces.append((i, pos, end))
                pos = end + 1
        print("downloading %d pieces (1MB each) with 24 threads..." % len(pieces))
        results = {}
        done = [0]
        lock = threading.Lock()

        def work(args):
            idx, i, s, e = args
            fetch_piece(idx, s, e, results)
            with lock:
                done[0] += 1
                if done[0] % 20 == 0:
                    ok = sum(1 for v in results.values() if v)
                    print("  pieces %d/%d (ok=%d)" % (done[0], len(pieces), ok), flush=True)

        with ThreadPoolExecutor(max_workers=24) as ex:
            list(ex.map(work, [(k, i, s, e) for k, (i, s, e) in enumerate(pieces)]))
        okn = sum(1 for v in results.values() if v)
        print("pieces ok: %d/%d" % (okn, len(pieces)))
        if okn != len(pieces):
            print("SOME PIECES FAILED:", [pieces[k] for k in range(len(pieces)) if not results.get(k)])
            sys.exit(2)
        # append pieces to their chunk in order
        by_chunk = {}
        for k, (i, s, e) in enumerate(pieces):
            by_chunk.setdefault(i, []).append((s, e, k))
        for i in sorted(by_chunk):
            fn = os.path.join(DL, "chunk_%03d.bin" % i)
            with open(fn, "ab") as dst:
                for (s, e, k) in sorted(by_chunk[i]):
                    p = os.path.join(DL, "piece_%05d.bin" % k)
                    dst.write(open(p, "rb").read())
                    os.remove(p)
            print("chunk %d now %d/%d" % (i, os.path.getsize(fn), CHUNK))

    # assemble
    print("assembling", OUT)
    with open(OUT, "wb") as dst:
        for i in range(CHUNKS):
            fn = os.path.join(DL, "chunk_%03d.bin" % i)
            with open(fn, "rb") as src:
                dst.write(src.read())
    size = os.path.getsize(OUT)
    print("size:", size, "expected:", SIZE)
    if size != SIZE:
        print("SIZE MISMATCH")
        sys.exit(3)
    h = hashlib.sha256()
    with open(OUT, "rb") as f:
        while True:
            b = f.read(8 << 20)
            if not b:
                break
            h.update(b)
    print("sha256:", h.hexdigest(), "expected:", SHA)
    if h.hexdigest().lower() != SHA:
        print("SHA256 MISMATCH")
        sys.exit(4)
    print("DOWNLOAD OK")


if __name__ == "__main__":
    main()
