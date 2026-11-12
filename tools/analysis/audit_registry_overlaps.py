#!/usr/bin/env python3
"""Guard the two registry defects that only the WHOLE BUILD can see.

`test_single_match.py` checks one function in isolation, so it is blind to
what happens when ~800 compiled functions are patched into one image.  Two
defects hide there.  Both shipped undetected until 2026-08-29, when together
they were the entire reason `build_level3_c.py` produced a different SHA1
from retail (5 functions mismatched, every one of them BYTE-PERFECT on its
own).

FATAL -- WINDOW DISAGREEMENT
    A file whose `@SymbolStart` / `@SymbolSize` names a different window than
    its registry `address` / `size`.  `test_single_match` honours the header
    (through config/main_symbol_boundaries.json) and reports BYTE-PERFECT;
    the whole-build patcher uses the REGISTRY and writes the body at the
    wrong address, corrupting it and whatever follows.  **Fix the registry
    entry, not the header** -- the registry address must be the address the C
    actually covers.

WORKLIST -- OVERLAP
    Two registered C entries covering the same bytes.  The usual shape is a
    cross-prologue head symbol registered alongside the real function: the
    head entry's C emits the hoisted `lui`/load pair AND the body, the host
    entry's C emits the body alone.  Whichever is patched second wins.  While
    the two agree byte-for-byte this is latent rather than broken, which is
    why it is reported and not fatal -- but it is duplicated work and one
    edit to either side turns it into a silent corruption.  Prefer keeping
    the entry that covers MORE bytes (the head) and de-registering the
    shorter duplicate, carrying the better name and the header notes over.

Exit status is non-zero only for the FATAL class.
"""
import json, os, re, sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))


def main():
    reg = json.load(open(os.path.join(ROOT, 'config/level3_functions.json')))['functions']
    entries = []
    for e in reg:
        src = str(e.get('source', ''))
        if not src.endswith('.c'):
            continue
        entries.append((int(e['address'], 16), e.get('size') or 0, e['name'], src))
    entries.sort()

    fatal = 0
    for a, n, name, src in entries:
        p = os.path.join(ROOT, src)
        if not os.path.exists(p):
            print(f"FATAL  MISSING  {name} {src}")
            fatal += 1
            continue
        t = open(p, errors='replace').read()
        ms = re.search(r'@SymbolStart:\s*(0x[0-9A-Fa-f]+)', t)
        mz = re.search(r'@SymbolSize:\s*(0x[0-9A-Fa-f]+|\d+)', t)
        if ms and int(ms.group(1), 16) != a:
            print(f"FATAL  WINDOW  {name}: registry 0x{a:08X} but @SymbolStart "
                  f"{ms.group(1)} -- the whole build patches at the REGISTRY address")
            fatal += 1
        if mz and int(mz.group(1), 0) != n:
            print(f"FATAL  WINDOW  {name}: registry size {n} but @SymbolSize {mz.group(1)}")
            fatal += 1

    same_end, partial = [], []
    for i, (a, n, name, src) in enumerate(entries):
        if not n:
            continue
        for b, m, name2, src2 in entries[i + 1:]:
            if b >= a + n:
                break
            (same_end if a + n == b + m else partial).append((name, a, n, name2, b, m))

    for name, a, n, name2, b, m in same_end:
        print(f"WORKLIST  DUPLICATE  {name} 0x{a:08X}+{n} and {name2} 0x{b:08X}+{m}"
              f" end together -- same body registered twice")
    for name, a, n, name2, b, m in partial:
        print(f"WORKLIST  PARTIAL    {name} 0x{a:08X}+{n} runs into {name2} 0x{b:08X}+{m}")

    print(f"\n{len(entries)} registered C entries: {fatal} FATAL, "
          f"{len(same_end)} duplicate + {len(partial)} partial overlaps on the worklist")
    return 1 if fatal else 0


if __name__ == '__main__':
    sys.exit(main())
