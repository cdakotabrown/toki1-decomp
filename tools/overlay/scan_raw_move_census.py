#!/usr/bin/env python3
#
# Written 2026-08-14 for the copy-prop volatile sweep.
# The "true class instrument" for surplus-move screening: masked diffs are
# distorted by paints; this census compares RAW (PostProc-stripped) move counts.
"""RAW (PostProc-stripped) mnemonic census: count `move rD,rS` (or/addu with
rt=$zero, rs!=$zero) in the compiled object vs the target .s, per semi
function.  A positive delta = we emit a copy the target does not."""
import json, os, re, subprocess, sys, tempfile, shutil
from concurrent.futures import ThreadPoolExecutor

ROOT = "."
PY = ROOT + "/.venv/bin/python3"
OBJDUMP = "mipsel-linux-gnu-objdump"


def strip_pp(src, dst):
    s = open(src, errors="ignore").read()
    out = [l for l in s.split("\n") if "@PostProc" not in l]
    open(dst, "w").write("\n".join(out))


def count_moves_asm(path):
    n = 0
    tot = 0
    for l in open(path, errors="ignore"):
        if not l.startswith("    "):
            continue
        tot += 1
        p = l.split()
        if p and p[0] == "or" and p[-1] == "$zero" and p[2] != "$zero,":
            n += 1
    return n, tot


def count_moves_obj(path, func):
    p = subprocess.run([OBJDUMP, "-d", path], capture_output=True, text=True)
    n = 0
    tot = 0
    started = False
    for l in p.stdout.split("\n"):
        m = re.match(r"^\s+[0-9a-f]+:\s+([0-9a-f]{8})\s+(\S+)", l)
        if not m:
            continue
        w = int(m.group(1), 16)
        mn = m.group(2)
        tot += 1
        op = w >> 26
        funct = w & 0x3F
        rs = (w >> 21) & 0x1F
        rt = (w >> 16) & 0x1F
        if op == 0 and funct in (0x21, 0x25) and rt == 0 and rs != 0:
            n += 1
    return n, tot


def run(args):
    ov, fn = args
    src = f"{ROOT}/src/overlay/{ov}/{fn}.c"
    asm = f"{ROOT}/asm/overlay/{ov}/{fn}.s"
    if not os.path.exists(asm):
        return None
    tmp = tempfile.mkdtemp()
    try:
        c = os.path.join(tmp, f"{fn}.c")
        strip_pp(src, c)
        keep = os.path.join(tmp, "keep")
        r = subprocess.run([PY, "tools/overlay/test_overlay_match.py", ov, fn,
                            "--src", c, "--keep", keep, "--no-resolve"],
                           cwd=ROOT, capture_output=True, text=True, timeout=900)
        obj = os.path.join(keep, ov, fn, f"{fn}.o")
        if not os.path.exists(obj):
            return (ov, fn, "NOOBJ", 0, 0, 0, 0)
        cm, ct = count_moves_obj(obj, fn)
        tm, tt = count_moves_asm(asm)
        # subtract the 4 leading thunk words objdump sees before the body
        return (ov, fn, "ok", tm, cm, tt, ct)
    except Exception as e:
        return (ov, fn, f"ERR:{e}", 0, 0, 0, 0)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)


def main():
    d = json.load(open(ROOT + "/config/overlay_match_status.json"))
    targets = [(ov, fn) for ov, fns in d.items() for fn, i in fns.items()
               if i.get("status") in ("semi_matched", "count_mismatch")
               and os.path.exists(f"{ROOT}/src/overlay/{ov}/{fn}.c")]
    with ThreadPoolExecutor(max_workers=8) as ex:
        for r in ex.map(run, targets):
            if r is None:
                continue
            ov, fn, st, tm, cm, tt, ct = r
            print(f"{ov}/{fn}\t{st}\ttmove={tm}\tcmove={cm}\tdmove={cm-tm}"
                  f"\ttw={tt}\tcw={ct}", flush=True)


main()
