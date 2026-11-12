#!/usr/bin/env python3
"""Report @PostProc: strip_arg_homes users -- instruction DELETION that the
main banned-PostProc detector does not see.

`tools/ido/strip_arg_homes.py` removes instructions at given .text offsets and
compacts the remaining code (its own docstring: "Remove instructions at
specified .text offsets and compact"). IDO 5.2 -O2 emits `sw aX,off(sp)`
argument-home stores that some retail functions do not contain, so the
directive deletes them to make the instruction count match.

That is fabrication by subtraction, and CLAUDE.md's own audit test is explicit:
"if deleting all PostProc produces the wrong instruction count, the function is
unmatchable from C -- add it to config/c_unmatchable.txt with a blocker
category."

WHY THE EXISTING DETECTOR MISSES IT
-----------------------------------
`banned_postproc._deletes_or_inserts_insns()` DOES name this case:

    if re.search(r"strip_arg_homes", text) and "--shrink" in text:
        return True

but that function is only applied to `custom_script` SOURCE TEXT. A first-class
`@PostProc: strip_arg_homes` directive line is matched against
`_BANNED_PATTERNS` / `BANNED_DIRECTIVES` instead, and `strip_arg_homes` is in
neither (9 names, none of them this). So `check_banned_postproc()` returns
clean for every user.

This script is REPORT-ONLY on purpose. Arming the main detector would make
`build_level3_c.py` abort on registered functions, so remediation has to come
first. Exit status is always 0.
"""
import json, re, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

def main() -> int:
    reg = json.loads((ROOT / "config/level3_functions.json").read_text())
    registered = {f["source"]: f for f in reg["functions"]}
    rows = []
    for p in sorted(ROOT.glob("src/main/**/*.c")):
        text = p.read_text(errors="replace")
        for line in text.splitlines():
            if "@PostProc:" in line and "strip_arg_homes" in line:
                rel = str(p.relative_to(ROOT))
                offs = re.findall(r"0x[0-9a-fA-F]+", line)
                rows.append((rel, len(offs), "--shrink" in line, registered.get(rel)))
    if not rows:
        print("no strip_arg_homes users")
        return 0
    print(f"{len(rows)} file(s) use @PostProc: strip_arg_homes "
          f"(instruction DELETION; see this script's docstring)\n")
    for rel, n, shrink, entry in rows:
        tag = "REGISTERED " + entry["match_status"] if entry else "not registered"
        print(f"  {'SHRINK' if shrink else '      '}  deletes {n} insn(s)  "
              f"[{tag}]  {rel}")
    bad = [r for r in rows if r[3]]
    print(f"\n{len(bad)} of these are REGISTERED and counted as byte-perfect C.")
    print("Per CLAUDE.md's audit test they are unmatchable from C and belong in")
    print("config/c_unmatchable.txt as honest asm_only, not in the registry.")
    return 0

if __name__ == "__main__":
    sys.exit(main())
