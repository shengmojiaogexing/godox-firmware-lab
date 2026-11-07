#!/usr/bin/env python3
"""decompile.py -- decompile arbitrary addresses of the X3-C firmware via Ghidra headless.

Usage:
    tools/python/python.exe devices/x3c/tools/decompile.py 0x08004af8 0x0801666c
    ... --program hotshoe 0x080020cc      # second (hot-shoe) image
    ... --program main  0x08033e20        # main app image (default)

It reuses the persistent, already-analyzed Ghidra project (D:\\ghidra_project, project X3C)
and only runs a tiny script (DumpAddr.java), so each call is ~15-30 s.
Output: decompiled C + disassembly of the containing function.
"""
import json
import os
import subprocess
import sys
import tempfile

BASE = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
PROJ_MANIFEST = os.path.join(BASE, "devices", "x3c", "re", "ghidra_exports", "project.json")
DEFAULT = {
    "project": r"D:\ghidra_project",
    "projectName": "X3C",
    "ghidra": r"D:\ghidra_12.2_PUBLIC",
    "jdk": r"D:\jdk25",
    "scripts": r"D:\ghidra_scripts",
    "programs": {
        "main": {"name": "X3_C_v1.38_app.bin", "base": "0x08004800"},
        "hotshoe": {"name": "hotshoe_blob.bin", "base": "0x08002000"},
    },
}


def load_manifest():
    if os.path.exists(PROJ_MANIFEST):
        try:
            with open(PROJ_MANIFEST, "r", encoding="utf-8") as f:
                m = json.load(f)
            cfg = dict(DEFAULT)
            for k in ("project", "projectName", "ghidra", "jdk", "scripts"):
                if m.get(k) and os.path.exists(m[k]) if k in ("project", "ghidra", "jdk", "scripts") else m.get(k):
                    cfg[k] = m[k]
            if m.get("programs"):
                cfg["programs"] = m["programs"]
            return cfg
        except Exception:
            pass
    return DEFAULT


def pick_jdk(preferred):
    cands = [preferred, r"D:\jdk25", os.path.join(BASE, r"tools\jdk\jdk-25.0.5+8"),
             os.path.join(BASE, r"tools\jdk\jdk-21.0.13+9")]
    for c in cands:
        if c and os.path.exists(os.path.join(c, "bin", "java.exe")):
            return c
    return preferred


def main():
    argv = sys.argv[1:]
    program = "main"
    addrs = []
    i = 0
    while i < len(argv):
        a = argv[i]
        if a == "--program":
            i += 1
            program = argv[i]
        elif a in ("-h", "--help"):
            print(__doc__)
            return
        else:
            addrs.append(a)
        i += 1
    if not addrs:
        print(__doc__)
        return
    if program not in ("main", "hotshoe"):
        raise SystemExit("--program must be main or hotshoe")

    addrs = [int(str(a).lower().replace("0x", ""), 16) for a in addrs]
    cfg = load_manifest()
    prog = cfg["programs"][program]
    ah = os.path.join(cfg["ghidra"], "support", "analyzeHeadless.bat")
    if not os.path.exists(ah):
        raise SystemExit("Ghidra not installed: " + ah)
    jdk = pick_jdk(cfg.get("jdk"))
    out = os.path.join(tempfile.gettempdir(), "dsh_decompile_%d.txt" % os.getpid())
    script_args = " ".join(["0x%08x" % a for a in addrs])
    cmd = ("\"" + ah + "\" \"" + cfg["project"] + "\" " + cfg["projectName"]
           + " -process \"" + prog["name"] + "\" -noanalysis -readOnly"
           + " -scriptPath \"" + cfg["scripts"] + "\""
           + " -postScript DumpAddr.java \"" + out + "\" " + script_args)
    env = dict(os.environ)
    env["JAVA_HOME"] = jdk
    env["PATH"] = os.path.join(jdk, "bin") + os.pathsep + env.get("PATH", "")
    print("# program=%s (%s) addrs=%s" % (program, prog["name"], script_args), file=sys.stderr)
    proc = subprocess.run(cmd, shell=True, env=env, capture_output=True, text=True, errors="replace")
    txt = ""
    if os.path.exists(out):
        with open(out, "r", encoding="utf-8", errors="replace") as f:
            txt = f.read()
        try:
            os.remove(out)
        except OSError:
            pass
    if not txt:
        sys.stderr.write(proc.stdout[-3000:] + "\n" + proc.stderr[-3000:] + "\n")
        raise SystemExit("decompile produced no output")
    print(txt)


if __name__ == "__main__":
    main()
