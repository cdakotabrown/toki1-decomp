#!/usr/bin/env python3
"""audit_unregistered_matched.py - audit the files in src/main/matched/ that are
NOT registered in config/level3_functions.json.

    python3 tools/analysis/audit_unregistered_matched.py [--json OUT]

`audit_main_fixups.py` walks the REGISTRY, so anything sitting in the
byte-perfect directory without a registry row is invisible to it.  That is a
blind spot: those files are counted as matched_c by audit_main_functions.py
(which classifies by directory), they are presented as finished, and nothing
checks them.  The first run (2026-08-28) found 137 such files, of which 31 were
not clean -- 21 with words copied from the target, 21 not byte-perfect, 9
overrunning their window -- plus 32 whose address could not be resolved at all.

Each file is reported with the reason it is not clean, and with whether its
address is ALREADY claimed by a registered file.  That last column matters:
a duplicate at a claimed address is redundant work, not missing work, and
promoting one is how a second definition of the same symbol gets into the
build (see the four retired on 2026-08-28).
"""
import json, os, glob, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

def main():
    reg_json = json.load(open(os.path.join(ROOT, 'config/level3_functions.json')))['functions']
    reg_src = {os.path.abspath(os.path.join(ROOT, f['source'])) for f in reg_json}
    by_addr = {f['address'].upper(): f['name'] for f in reg_json}
    files = [p for p in sorted(glob.glob(os.path.join(ROOT, 'src/main/matched/*.c')))
             if os.path.abspath(p) not in reg_src]
    print(f"{len(files)} files in src/main/matched/ with no registry row")
    rows, flagged = [], 0
    for p in files:
        fn = os.path.basename(p)[:-2]
        head = open(p).read(3000)
        m = re.search(r'Address:\s*(0x[0-9A-Fa-f]+)', head)
        claimed = by_addr.get(m.group(1).upper()) if m else None
        r = subprocess.run([sys.executable, 'tools/analysis/test_single_match.py', fn],
                           cwd=ROOT, capture_output=True, text=True, timeout=900)
        o = r.stdout + r.stderr
        e = re.search(r'Exact:\s+(\d+)/(\d+)', o)
        f = re.search(r'Fixup:\s+(\d+) word', o)
        fx = int(f.group(1)) if f else 0
        ov, bp = 'OVERRUN' in o, 'BYTE-PERFECT MATCH' in o
        why = []
        if e is None: why.append('unresolved')
        else:
            if fx: why.append(f'{fx} copied word(s)')
            if ov: why.append('window overrun')
            if not bp: why.append('not byte-perfect')
        row = {'name': fn, 'exact': e.group(0) if e else None, 'fixup': fx,
               'overrun': ov, 'byte_perfect': bp, 'claimed_by': claimed, 'why': why}
        rows.append(row)
        if why:
            flagged += 1
            dup = f"  [address already registered as {claimed}]" if claimed else ""
            print(f"  !  {fn:26s} {e.group(0) if e else 'unresolved':22s} {', '.join(why)}{dup}")
    print(f"\n{flagged} of {len(files)} flagged")
    if '--json' in sys.argv:
        out = sys.argv[sys.argv.index('--json') + 1]
        json.dump(rows, open(out, 'w'), indent=1)
        print("wrote", out)

if __name__ == '__main__':
    main()
