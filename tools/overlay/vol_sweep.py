#!/usr/bin/env python3
#
# Written 2026-08-14 for the copy-prop volatile sweep.
# Per-global volatile screen: flips one extern volatile at a time (PostProc
# stripped), reports word count + exact%. ~90 compiles across 7 functions on
# first use; every global inert -> the 2026-08-14 class-exhaustion evidence.
"""Per-global `volatile` screen: for each extern global in a candidate's .c,
compile a PostProc-stripped variant with that one global marked volatile and
report emitted word count + exact%.  Screening only -- adoption still needs a
faithfulness argument from the target's access pattern."""
import os, re, subprocess, sys, tempfile, shutil
from concurrent.futures import ThreadPoolExecutor

ROOT = "."
PY = ROOT + "/.venv/bin/python3"
EXT = re.compile(r"^(extern\s+)((?:unsigned\s+|signed\s+)?(?:u8|s8|u16|s16|u32|s32|int|short|char|long|float|[A-Za-z_]\w*)\s*\**\s*)(D_[0-9A-Fa-f]{8}\b)")


def strip_pp(text):
    return "\n".join(l for l in text.split("\n") if "@PostProc" not in l)


def measure(ov, fn, text, tag):
    tmp = tempfile.mkdtemp()
    try:
        c = os.path.join(tmp, f"{fn}.c")
        open(c, "w").write(text)
        keep = os.path.join(tmp, "k")
        r = subprocess.run([PY, "tools/overlay/test_overlay_match.py", ov, fn,
                            "--src", c, "--keep", keep, "--no-resolve"],
                           cwd=ROOT, capture_output=True, text=True, timeout=900)
        out = r.stdout
        m = re.search(r"\(([\d.]+)% exact, (\d+)/(\d+) insns\)", out)
        ex = float(m.group(1)) if m else -1.0
        obj = os.path.join(keep, ov, fn, f"{fn}.o")
        nw = -1
        if os.path.exists(obj):
            p = subprocess.run(["mipsel-linux-gnu-objdump", "-d", obj],
                               capture_output=True, text=True)
            nw = sum(1 for l in p.stdout.split("\n")
                     if re.match(r"^\s+[0-9a-f]+:\s+[0-9a-f]{8}", l)) - 4
        return (tag, ex, nw, "OVERRUN" in out)
    except Exception as e:
        return (tag, -1.0, -1, str(e)[:40])
    finally:
        shutil.rmtree(tmp, ignore_errors=True)


def main():
    ov, fn = sys.argv[1].split("/")
    src = f"{ROOT}/src/overlay/{ov}/{fn}.c"
    base = strip_pp(open(src, errors="ignore").read())
    lines = base.split("\n")
    jobs = [(base, "BASELINE")]
    for i, l in enumerate(lines):
        m = EXT.match(l)
        if not m or "volatile" in l:
            continue
        nl = list(lines)
        nl[i] = m.group(1) + "volatile " + m.group(2) + l[m.end(2):]
        jobs.append(("\n".join(nl), m.group(3)))
    print(f"{ov}/{fn}: {len(jobs)-1} globals", file=sys.stderr)
    with ThreadPoolExecutor(max_workers=8) as ex:
        res = list(ex.map(lambda j: measure(ov, fn, j[0], j[1]), jobs))
    b = res[0]
    print(f"BASELINE  exact={b[1]:5.1f}%  words={b[2]}")
    for tag, e, nw, ov_ in sorted(res[1:], key=lambda r: (-r[1], r[2])):
        mark = ""
        if e > b[1] + 0.5: mark += " EXACT+"
        if nw != b[2]: mark += f" dW={nw-b[2]:+d}"
        print(f"{tag}  exact={e:5.1f}%  words={nw}{mark}")


main()
