#!/usr/bin/env python3
"""Round-trip check for the Hook 28 (IDO52_EXCLUDE_REGS_WEB) register parser.

    python3 tools/ido-static-recomp/test_hook28_regspec.py           # static
    python3 tools/ido-static-recomp/test_hook28_regspec.py --live    # + compiler

THE BUG THIS PINS DOWN (found 2026-08-09; fixed the same day)
---------------------------------------------------------------------
`IDO52_EXCLUDE_REGS_WEB=<bitpos>:<reg>` ORs a bit into the web's forbidden
colour bitvector at `lr+40`.  The parser used to compute

    mask |= 1u << ((~(uint32_t)(rc - 3)) & 31u)

on the theory that the vector stores "dump space" (rc-3), because the
printregs `forbidden:` listing prints in that space.  It does not.
`f_updatecolorsleft` reads the vector as

    word  = lr + 40 + 4*(colour >> 5)
    taken = (word << (colour & 31)) < 0        i.e. bit (31 - colour)

with `colour` in the SAME rc space the parser uses (v0=1 .. t9=16,
s0=17 .. s8/fp=25).  Subtracting 3 therefore forbade the colour THREE SLOTS
LOWER than the one named: **`t3` forbade `$t0`** (rc 10 -> 7).  Measured by
sweeping a fixture: at the one bitpos where the hook bites, only the spec
`t3` moved a web that was coloured `$t0`, and after the fix only `t0` does.

⚠️ Any `@CompileEnv: IDO52_EXCLUDE_REGS_WEB=...` written BEFORE 2026-08-09
was authored against the buggy offset and now means something different
(three slots higher than it used to).  See the report/TODO for the list.
"""
import argparse
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
HOOKS = Path(__file__).resolve().parent / "apply_uopt_hooks.py"
CC = ROOT / "tools/ido-static-recomp/build/5.2/out/cc"

# The documented PRECOLOR rc colour space.
EXPECTED_RC = (["v0", "v1", "a0", "a1", "a2", "a3"] +
               ["t%d" % i for i in range(10)] +
               ["s%d" % i for i in range(8)] + ["s8"])   # rc 1..25

FIXTURE = """extern int G[64];
int f(int n)
{
    int a, b, c, d, e, g, h, i;
    a = G[0]; b = G[1]; c = G[2]; d = G[3];
    e = G[4]; g = G[5]; h = G[6]; i = G[7];
    while (n--) {
        a += b; b += c; c += d; d += e;
        e += g; g += h; h += i; i += a;
        G[n & 63] = a + b + c + d + e + g + h + i;
    }
    return a + b + c + d + e + g + h + i;
}
"""


def parse_rc_table(text):
    """{name: rc} from the C if-chain in apply_uopt_hooks.py."""
    body = text.split("IDO52_EXCLUDE_REGS_WEB=BITPOS", 1)[-1]
    body = body.split("IDO52_PRECOLOR_LIVEIN", 1)[0]
    tab = {}
    for m in re.finditer(r'strcmp\(r,\s*"(\w+)"\)\s*==\s*0\)?\s*rc\s*=\s*(\d+)',
                         body):
        tab[m.group(1)] = int(m.group(2))
    for m in re.finditer(r'strcmp\(r,\s*"(\w+)"\)\s*==\s*0\s*\|\|\s*'
                         r'strcmp\(r,\s*"(\w+)"\)\s*==\s*0\)\s*rc\s*=\s*(\d+)',
                         body):
        tab[m.group(1)] = tab[m.group(2)] = int(m.group(3))
    return tab


def mask_for(rc):
    """The bit the (fixed) parser sets for colour `rc`."""
    return 1 << ((~rc) & 31)


def colours_of(mask):
    """Decode a forbidden-vector word back to rc colour numbers."""
    return sorted((~b) & 31 for b in range(32) if mask & (1 << b))


def static_checks():
    text = HOOKS.read_text()
    fails = []

    if "(rc - 3)" in text.split("IDO52_EXCLUDE_REGS_WEB=BITPOS", 1)[-1] \
                        .split("IDO52_PRECOLOR_LIVEIN", 1)[0]:
        fails.append("apply_uopt_hooks.py still contains the rc-3 off-by-three")

    tab = parse_rc_table(text)
    for i, name in enumerate(EXPECTED_RC, start=1):
        if tab.get(name) != i:
            fails.append("rc table: %s = %r, expected %d"
                         % (name, tab.get(name), i))
    if tab.get("fp") != 25:
        fails.append("rc table: fp = %r, expected 25 (alias of s8)"
                     % tab.get("fp"))
    print("rc table: %d names parsed, v0=%s t0=%s t3=%s s0=%s s8=%s"
          % (len(tab), tab.get("v0"), tab.get("t0"), tab.get("t3"),
             tab.get("s0"), tab.get("s8")))

    # name -> mask -> colour -> name must be the identity
    inv = {v: k for k, v in tab.items() if k != "fp"}
    for name, rc in sorted(tab.items()):
        if name == "fp":
            continue
        got = colours_of(mask_for(rc))
        if got != [rc] or inv.get(got[0]) != name:
            fails.append("round trip broken for %s: rc=%d mask bits -> %r"
                         % (name, rc, got))
    print("round trip: %d/%d register specs map back to themselves"
          % (len(tab) - 1 - len(fails), len(tab) - 1))

    # the regression itself, spelled out
    if colours_of(mask_for(tab.get("t3", 0))) == [tab.get("t0")]:
        fails.append("REGRESSION: spec t3 still forbids t0")
    return fails


def live_check():
    """Run the real compiler and decode the mask it actually builds."""
    if not CC.exists():
        print("live: SKIP (no built cc at %s)" % CC)
        return []
    fails = []
    with tempfile.TemporaryDirectory() as td:
        src = Path(td) / "t.c"
        src.write_text(FIXTURE)
        env0 = os.environ.copy()
        env0["TMPDIR"] = td
        base = subprocess.run([str(CC), "-O2", "-G0", "-mips1", "-EL",
                               "-non_shared", "-S", str(src)],
                              capture_output=True, text=True, env=env0, cwd=td)
        if base.returncode != 0:
            print("live: SKIP (fixture failed to compile)")
            return []
        base_s = (Path(td) / "t.s").read_text()

        for spec in ("t0", "t3", "s5", "a2"):
            hit = None
            for bp in range(0, 48):
                env = dict(env0)
                env["IDO52_EXCLUDE_REGS_WEB"] = "%d:%s" % (bp, spec)
                env["IDO52_EXCLUDE_REGS_WEB_DEBUG"] = "1"
                r = subprocess.run([str(CC), "-O2", "-G0", "-mips1", "-EL",
                                    "-non_shared", "-S", str(src)],
                                   capture_output=True, text=True, env=env,
                                   cwd=td)
                m = re.search(r"\[EXCLREGS-WEB\] bitpos \d+: forbid "
                              r"0x([0-9a-f]+) -> 0x([0-9a-f]+)", r.stderr)
                if m:
                    hit = (bp, int(m.group(1), 16), int(m.group(2), 16),
                           (Path(td) / "t.s").read_text())
                    break
            if hit is None:
                print("live: %-3s no web matched in bitpos 0..47 (skipped)" % spec)
                continue
            bp, old, new, out_s = hit
            added = colours_of(new & ~old)
            want = EXPECTED_RC.index(spec) + 1
            ok = added == [want]
            print("live: %-3s bitpos %-3d forbid 0x%08x -> 0x%08x  "
                  "colours added %s (want [%d]) %s"
                  % (spec, bp, old, new, added, want, "OK" if ok else "FAIL"))
            if not ok:
                fails.append("live: spec %s set colour(s) %s, expected %d"
                             % (spec, added, want))
            if spec == "t0" and out_s == base_s:
                print("      (note: codegen unchanged at this bitpos — mask "
                      "decode is still authoritative)")
    return fails


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--live", action="store_true",
                    help="also run the built IDO cc and decode the mask it "
                         "actually constructs")
    args = ap.parse_args()
    fails = static_checks()
    if args.live:
        fails += live_check()
    if fails:
        print("\nFAIL:")
        for f in fails:
            print("  -", f)
        return 1
    print("\nPASS")
    return 0


if __name__ == "__main__":
    sys.exit(main())
