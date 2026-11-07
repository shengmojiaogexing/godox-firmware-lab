#!/usr/bin/env python3
"""Salvage-extract a partially corrupt Ghidra zip: extract every member
individually and skip the ones whose compressed data is damaged."""
import os
import sys
import zipfile
import time

HERE = os.path.dirname(os.path.abspath(__file__))            # devices/ad100pro/ghidra
ROOT = os.path.dirname(os.path.dirname(HERE))                # <repo>
ZIP = os.path.join(ROOT, "tools", "ghidra_12.2_PUBLIC.zip")
DEST = os.path.join(ROOT, "tools")
LOG = os.path.join(HERE, "extract_salvage.log")

def main():
    t0 = time.time()
    good = 0
    bad = []
    total = 0
    with open(LOG, "w", encoding="utf-8") as log:
        with zipfile.ZipFile(ZIP) as zf:
            infos = zf.infolist()
            total = len(infos)
            log.write(f"members={total}\n")
            for i, info in enumerate(infos):
                name = info.filename
                try:
                    data = zf.read(info)          # CRC/decompress check happens here
                except Exception as e:
                    bad.append((name, str(e)))
                    log.write(f"BAD {name}: {e}\n")
                    log.flush()
                    continue
                out = os.path.join(DEST, name.replace("/", os.sep))
                try:
                    if name.endswith("/"):
                        os.makedirs(out, exist_ok=True)
                    else:
                        os.makedirs(os.path.dirname(out), exist_ok=True)
                        with open(out, "wb") as fh:
                            fh.write(data)
                    good += 1
                except Exception as e:
                    bad.append((name, "write: " + str(e)))
                    log.write(f"WRITE_FAIL {name}: {e}\n")
                if i % 500 == 0:
                    print(f"{i}/{total} good={good} bad={len(bad)}", flush=True)
        log.write(f"\nDONE good={good} bad={len(bad)} total={total} elapsed={time.time()-t0:.1f}s\n")
        for n, e in bad:
            log.write(f"FAILED {n} :: {e}\n")
    print(f"DONE good={good} bad={len(bad)} total={total} elapsed={time.time()-t0:.1f}s")
    for n, e in bad:
        print("FAILED", n, "::", e)
    return 0 if not bad else 2

if __name__ == "__main__":
    sys.exit(main())
