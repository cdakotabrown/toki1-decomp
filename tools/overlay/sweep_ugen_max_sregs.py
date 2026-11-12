#!/usr/bin/env python3
"""Sweep IDO52_UGEN_MAX_SREGS values across semi_matched overlay candidates.

For each candidate, this script:
  1. Adds `@CompileEnv: IDO52_UGEN_MAX_SREGS=N` to the C file (preserves existing PostProc)
  2. Runs `tools/overlay/test_overlay_match.py <OVL> <FUNC>`
  3. Records the masked% for each N in {0,1,2,3,4,5,6,7}
  4. If any N achieves RELOC-ONLY (100% masked), keeps that change in the file
  5. Otherwise restores the file to the original state

Usage:
    python3 tools/overlay/sweep_ugen_max_sregs.py --ovl NAME_ENT --func func_80132E88
    python3 tools/overlay/sweep_ugen_max_sregs.py --candidates-file /tmp/cands.txt
    python3 tools/overlay/sweep_ugen_max_sregs.py --auto --min-reloc 70 --max-sregs 3
        (auto: pick all candidates with target s-reg count <= --max-sregs and reloc% >= --min-reloc)
"""
import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent

def read_status():
    return json.loads((ROOT/'config/overlay_match_status.json').read_text())

def count_sregs(asm_path):
    if not asm_path.exists():
        return -1
    try: text = asm_path.read_text(errors='replace')
    except: return -1
    return len(set(re.findall(r'\$(s[0-7]|fp)\b', text)))

def run_match(ovl, fn):
    """Returns (masked%, exact%, is_match)."""
    r = subprocess.run(
        ['python3', 'tools/overlay/test_overlay_match.py', ovl, fn, '--verbose'],
        capture_output=True, text=True, timeout=90, cwd=str(ROOT))
    out = (r.stdout or '') + (r.stderr or '')
    m = re.search(r'Masked:\s+\d+/\d+\s+\(([\d.]+)%\)', out)
    masked = float(m.group(1)) if m else 0.0
    m = re.search(r'Exact:\s+\d+/\d+\s+\(([\d.]+)%\)', out)
    exact = float(m.group(1)) if m else 0.0
    is_match = 'RELOCATION-ONLY' in out or 'BYTE-PERFECT' in out
    return masked, exact, is_match

def try_one(ovl, fn, max_sregs_values):
    """Sweep for one function. Returns (best_N, best_masked) or (None, baseline)."""
    src = ROOT/'src'/'overlay'/ovl/f'{fn}.c'
    if not src.exists():
        return None, 0.0, "src missing"
    original = src.read_text()

    # Baseline (no env injection)
    base_m, base_e, base_match = run_match(ovl, fn)
    if base_match:
        return None, base_m, "already RELOC-ONLY"

    best_N = None
    best_m = base_m
    best_match = False
    results = {}

    try:
        for N in max_sregs_values:
            # Inject @CompileEnv at top
            injected = f'/* @CompileEnv: IDO52_UGEN_MAX_SREGS={N} */\n' + original
            src.write_text(injected)
            m, e, mm = run_match(ovl, fn)
            results[N] = (m, e, mm)
            if mm:
                # Lock it in!
                best_N, best_m, best_match = N, m, True
                # Don't restore — leave the modified file
                return best_N, best_m, results
            elif m > best_m:
                best_N, best_m = N, m
    finally:
        if not best_match:
            src.write_text(original)

    return best_N, best_m, results

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--ovl', help='Overlay name')
    ap.add_argument('--func', help='Function name')
    ap.add_argument('--auto', action='store_true', help='Auto-pick candidates')
    ap.add_argument('--min-reloc', type=float, default=70.0)
    ap.add_argument('--max-sregs', type=int, default=3,
                    help='Filter: candidates with target s-reg count <= this')
    ap.add_argument('--values', default='0,1,2,3,4,5',
                    help='Comma-separated MAX_SREGS values to try')
    ap.add_argument('--candidates-file', help='File with "OVL FUNC" per line')
    args = ap.parse_args()

    sweep_values = [int(x) for x in args.values.split(',')]

    candidates = []
    if args.ovl and args.func:
        candidates.append((args.ovl, args.func))
    elif args.candidates_file:
        for ln in Path(args.candidates_file).read_text().splitlines():
            parts = ln.split()
            if len(parts) >= 2:
                candidates.append((parts[0], parts[1]))
    elif args.auto:
        status = read_status()
        for ovl, funcs in status.items():
            for fn, info in funcs.items():
                if info.get('status') != 'semi_matched': continue
                if info.get('reloc_pct', 0) < args.min_reloc: continue
                asm = ROOT/'asm'/'overlay'/ovl/f'{fn}.s'
                sn = count_sregs(asm)
                if 0 <= sn <= args.max_sregs:
                    candidates.append((ovl, fn))

    if not candidates:
        print("No candidates", file=sys.stderr)
        sys.exit(1)

    print(f"Sweeping {len(candidates)} candidates with MAX_SREGS={sweep_values}")
    wins = []
    improvements = []
    for ovl, fn in candidates:
        sys.stdout.write(f"  {ovl}/{fn} ... ")
        sys.stdout.flush()
        try:
            best_N, best_m, results = try_one(ovl, fn, sweep_values)
            if isinstance(results, str):
                print(results)
                continue
            if isinstance(results, dict):
                # check if any was a true match
                any_match = any(r[2] for r in results.values())
                if any_match:
                    print(f"WIN MAX_SREGS={best_N} masked={best_m:.1f}%")
                    wins.append((ovl, fn, best_N, best_m))
                else:
                    deltas = [(N, r[0]) for N, r in results.items() if r[0] > 0]
                    deltas.sort(key=lambda x: -x[1])
                    if deltas:
                        N0, m0 = deltas[0]
                        if m0 > 50:  # mention only if non-trivial
                            print(f"best={m0:.1f}% (MAX_SREGS={N0})")
                            improvements.append((ovl, fn, N0, m0))
                        else:
                            print("no improvement")
                    else:
                        print("no improvement")
        except subprocess.TimeoutExpired:
            print("TIMEOUT")
        except Exception as e:
            print(f"ERROR: {e}")

    print(f"\n=== Summary ===")
    print(f"Wins (RELOC-ONLY): {len(wins)}")
    for w in wins:
        print(f"  {w[0]}/{w[1]} MAX_SREGS={w[2]} masked={w[3]:.1f}%")
    print(f"Improvements (no win): {len(improvements)}")
    for i in improvements[:20]:
        print(f"  {i[0]}/{i[1]} MAX_SREGS={i[2]} masked={i[3]:.1f}%")

if __name__ == '__main__':
    main()
