#!/usr/bin/env python3
"""Measure what an IDO compiler flag actually DOES to a population of functions.

    python3 tools/overlay/screen_flag.py -Wo,-rege,0 \
        --population status=byte_perfect,directive=patch_insn_reg_field --sample 60

────────────────────────────────────────────────────────────────────────────
Why this exists
────────────────────────────────────────────────────────────────────────────
`docs/reference/IDO52-FLAG-ARCHAEOLOGY.md` ranked flag leads by a **directional
bias** table (how often a flag moved a semi's mnemonic score toward the target).
The top-ranked entry by that metric, `-Wc,-nooffsetopt` (16 improved : 8
worsened), was then conclusively refuted: 0 wins, 0 rescues, and it would have
broken 131 already-byte-perfect functions (`NOOFFSETOPT-INVESTIGATION.md`).

**Bias is not yield.** Two columns of this report are decision-grade:

    NEW-BP      functions that were NOT byte-perfect and now ARE
    BROKE-WIN   functions that were an effective win and now are NOT

Everything else (`unchanged` / `improved` / `worsened`) is diagnostic colour —
useful for understanding a flag, never for justifying a campaign. A flag with a
glorious improved:worsened ratio and NEW-BP 0 has produced nothing.

────────────────────────────────────────────────────────────────────────────
Honesty properties
────────────────────────────────────────────────────────────────────────────
* **Read-only.** Never writes a `.c`. When `--strip-directive` is used the
  modified source is a COPY in a temp dir; the tracked file is untouched.
* **Verdict from the resolver.** A `reloc_only` masked verdict can LIE (it masks
  any diff landing on a reloc-typed instruction). Every masked win is settled by
  `resolve_overlay_relocs.resolve_function` — the name-derived link — exactly as
  `test_overlay_match.py` does.
* **Baseline is measured, not looked up.** The `config/overlay_match_status.json`
  status is used only to SELECT the population; the before/after comparison
  recompiles both ways through the same pipeline, so a stale cache entry cannot
  manufacture a fake improvement or a fake regression.
* **Bit-identical output is reported as `unchanged`**, which is the honest answer
  to "does this flag even reach this function".

────────────────────────────────────────────────────────────────────────────
Population selectors  (comma-separated terms, ANDed)
────────────────────────────────────────────────────────────────────────────
    status=byte_perfect | semi_matched | count_mismatch | reloc_only | asm_only
    status=semi                 alias for semi_matched+count_mismatch
    status=win                  byte_perfect+reloc_only (the "effective" set)
    overlay=GEKO                one overlay (repeatable term: overlay=GEKO|DATE)
    directive=patch_insn_reg_field    .c carries that @PostProc directive
    compiler=ido52 | gcc        by @Compiler
    func=GEKO/func_80132244     one function (repeatable with |)
    file=path/to/list.txt       lines of "OVERLAY func_NAME"

`--sample N` takes a seeded random subset (default seed 0, so a screen is
reproducible and two flags can be compared on the SAME functions).
"""
import argparse
import io
import json
import os
import random
import re
import shutil
import sys
import tempfile
from contextlib import redirect_stdout, redirect_stderr
from concurrent.futures import ProcessPoolExecutor
from pathlib import Path
from typing import Dict, List, Optional, Tuple

ROOT = Path(__file__).parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "build"))
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

from ido_flags import glue_option_values, parse_extra_cflags   # noqa: E402
from parse_postproc import directive_bodies                   # noqa: E402

SRC_OVERLAY_DIR = ROOT / "src" / "overlay"
STATUS_FILE = ROOT / "config" / "overlay_match_status.json"

# Verdict tiers. Only the tier and the exact-word score are compared.
TIER_SEMI = 0        # honestly not a match
TIER_EFFECTIVE = 1   # byte-perfect except for symbols no name can resolve
TIER_BP = 2          # byte-perfect
TIER_NAME = {0: 'semi', 1: 'effective', 2: 'byte_perfect'}


# ── population ──────────────────────────────────────────────────────────────

def _status_map() -> Dict[str, Dict[str, dict]]:
    if not STATUS_FILE.exists():
        return {}
    with open(STATUS_FILE) as f:
        return json.load(f)


_STATUS_ALIASES = {
    'semi': {'semi_matched', 'count_mismatch'},
    'win': {'byte_perfect', 'reloc_only'},
}


def select_population(selector: str) -> List[Tuple[str, str]]:
    """Resolve a selector string to a sorted list of (OVERLAY, func)."""
    status = _status_map()
    universe: List[Tuple[str, str]] = []
    for ov, fns in status.items():
        for fn in fns:
            universe.append((ov, fn))
    # Functions with a .c but no JSON entry are still screenable.
    for ovdir in sorted(SRC_OVERLAY_DIR.glob('*')):
        if not ovdir.is_dir():
            continue
        for c in ovdir.glob('*.c'):
            t = (ovdir.name, c.stem)
            if t not in status.get(ovdir.name, {}):
                universe.append(t)
    universe = sorted(set(universe))

    if not selector or selector == 'all':
        return universe

    terms = [t.strip() for t in selector.split(',') if t.strip()]
    sel = universe
    for term in terms:
        if '=' not in term:
            raise SystemExit(f"bad population term {term!r} (need key=value)")
        key, val = term.split('=', 1)
        key = key.strip()
        vals = {v.strip() for v in val.split('|') if v.strip()}

        if key == 'status':
            want = set()
            for v in vals:
                want |= _STATUS_ALIASES.get(v, {v})
            sel = [(ov, fn) for ov, fn in sel
                   if status.get(ov, {}).get(fn, {}).get('status') in want]
        elif key == 'overlay':
            up = {v.upper() for v in vals}
            sel = [(ov, fn) for ov, fn in sel if ov in up]
        elif key == 'func':
            want = set()
            for v in vals:
                ov, _, fn = v.replace(':', '/').partition('/')
                want.add((ov.upper(), fn))
            sel = [t for t in sel if t in want]
        elif key == 'file':
            want = set()
            for v in vals:
                for line in Path(v).read_text().splitlines():
                    line = line.split('#')[0].strip()
                    if not line:
                        continue
                    parts = line.replace('/', ' ').replace(':', ' ').split()
                    if len(parts) >= 2:
                        want.add((parts[0].upper(), parts[1]))
            sel = [t for t in sel if t in want]
        elif key in ('directive', 'compiler'):
            sel = [t for t in sel if _source_matches(t, key, vals)]
        else:
            raise SystemExit(f"unknown population key {key!r}")
    return sel


def _source_matches(t: Tuple[str, str], key: str, vals: set) -> bool:
    src = SRC_OVERLAY_DIR / t[0] / f"{t[1]}.c"
    if not src.exists():
        return False
    content = src.read_text(errors='ignore')
    if key == 'directive':
        bodies = list(directive_bodies(content, '@PostProc:'))
        got = {b.split()[0] for b in bodies if b.split()}
        return bool(got & vals)
    # compiler
    body = next(directive_bodies(content, '@Compiler:'), 'ido52')
    name = body.split()[0].rstrip('*/ ') if body.split() else 'ido52'
    return any(name.startswith(v) for v in vals)


# ── source rewriting (temp copies only — NEVER a tracked .c) ────────────────

_DIRECTIVE_LINE = re.compile(r'^(\s*(?:(?:/\*+|\*+|//)\s*)*)@PostProc:\s*(\S+)')


def stripped_copy(src: Path, dst: Path, drop: List[str]) -> int:
    """Copy `src` to `dst` with the named @PostProc directives NEUTERED.

    A dropped line keeps its text but loses its directive keyword, so the
    header still reads as documentation and every parser skips it. Returns the
    number of directives suppressed. The source file is opened read-only.
    """
    n = 0
    out = []
    for line in src.read_text(errors='ignore').splitlines(keepends=True):
        m = _DIRECTIVE_LINE.match(line)
        if m and m.group(2) in drop:
            line = line.replace('@PostProc:', '@ScreenedOutPostProc:', 1)
            n += 1
        out.append(line)
    dst.write_text(''.join(out))
    return n


# ── verdict ─────────────────────────────────────────────────────────────────

def _verdict(overlay: str, func: str, extra_cflags: List[str],
             src_override: Optional[str]) -> dict:
    """Compile + PostProc + (if needed) resolved link. Returns a verdict dict.

    Silences the pipeline's own chatter: a batch of 500 functions must not
    print 500 audit banners.
    """
    import test_overlay_match as T
    import resolve_overlay_relocs as R

    buf = io.StringIO()
    try:
        with redirect_stdout(buf), redirect_stderr(buf):
            r = T.test_overlay_function(overlay, func, verbose=False,
                                        return_bytes=True,
                                        extra_cflags=extra_cflags,
                                        src_override=src_override)
    except SystemExit as e:                                    # banned-postproc gate
        return {'tier': None, 'error': f'gate: exit {e.code}'}
    except Exception as e:                                     # noqa: BLE001
        return {'tier': None, 'error': f'{type(e).__name__}: {e}'}

    if 'error' in r:
        return {'tier': None, 'error': r['error']}
    if r.get('asm_only'):
        return {'tier': None, 'error': 'asm_only'}

    out = {
        'exact': r['exact_match'],
        'n_target': r['n_target'],
        'n_compiled': r['n_compiled'],
        'bytes': r.get('compiled_bytes', b''),
        'postproc_failed': r.get('postproc_failed'),
        'truncated_surplus': r.get('truncated_surplus'),
    }

    if r['matched']:
        out['tier'] = TIER_BP
        out['detail'] = 'byte_perfect (unmasked)'
        return out
    if not r['reloc_only']:
        out['tier'] = TIER_SEMI
        out['detail'] = 'semi'
        return out

    # Masked reloc_only -> settle with the AUTHORITATIVE name-derived link.
    try:
        with redirect_stdout(buf), redirect_stderr(buf):
            rr = R.resolve_function(overlay, func, extra_cflags=extra_cflags,
                                    src_override=src_override)
    except Exception as e:                                     # noqa: BLE001
        out['tier'] = TIER_SEMI
        out['detail'] = f'resolve failed: {type(e).__name__}'
        return out
    if 'error' in rr:
        out['tier'] = TIER_SEMI
        out['detail'] = f"resolve error: {rr['error']}"
        return out
    if rr.get('matched'):
        out['tier'] = TIER_BP
        out['detail'] = 'byte_perfect (resolved link)'
    elif rr.get('unresolved'):
        out['tier'] = TIER_EFFECTIVE
        out['detail'] = 'effective (.rodata-blocked)'
    else:
        out['tier'] = TIER_SEMI
        out['detail'] = 'semi (masked reloc_only was over-reporting)'
    return out


def _screen_one(job: tuple) -> dict:
    (overlay, func, extra_cflags, base_cflags, drop, workdir) = job
    os.chdir(ROOT)
    src = SRC_OVERLAY_DIR / overlay / f"{func}.c"
    src_override = None
    if drop:
        if not src.exists():
            return {'overlay': overlay, 'func': func, 'verdict': 'error',
                    'note': 'no source'}
        tmp = Path(workdir) / overlay
        tmp.mkdir(parents=True, exist_ok=True)
        copy = tmp / f"{func}.c"
        stripped_copy(src, copy, drop)
        src_override = str(copy)

    base = _verdict(overlay, func, base_cflags, src_override)
    flag = _verdict(overlay, func, extra_cflags, src_override)

    row = {
        'overlay': overlay, 'func': func,
        'base': base.get('detail') or base.get('error'),
        'flag': flag.get('detail') or flag.get('error'),
        'base_exact': base.get('exact'), 'flag_exact': flag.get('exact'),
        'base_tier': base.get('tier'), 'flag_tier': flag.get('tier'),
    }
    row['verdict'] = _classify(base, flag)
    return row


def _classify(base: dict, flag: dict) -> str:
    bt, ft = base.get('tier'), flag.get('tier')
    if bt is None and ft is None:
        return 'skipped'
    if ft is None:
        # The flag broke the compile / PostProc chain outright.
        return 'broke_a_win' if bt is not None and bt >= TIER_EFFECTIVE \
            else 'error_under_flag'
    if bt is None:
        return 'baseline_error'
    if base.get('bytes') == flag.get('bytes'):
        return 'unchanged'
    if ft == TIER_BP and bt < TIER_BP:
        return 'newly_byte_perfect'
    if bt >= TIER_EFFECTIVE and ft < TIER_EFFECTIVE:
        return 'broke_a_win'
    if bt == TIER_BP and ft == TIER_EFFECTIVE:
        return 'downgraded'
    if ft > bt:
        return 'improved'
    if ft < bt:
        return 'worsened'
    be, fe = base.get('exact') or 0, flag.get('exact') or 0
    if fe > be:
        return 'improved'
    if fe < be:
        return 'worsened'
    return 'changed_neutral'


ORDER = ['newly_byte_perfect', 'broke_a_win', 'downgraded', 'improved',
         'worsened', 'changed_neutral', 'unchanged', 'error_under_flag',
         'baseline_error', 'skipped', 'error']


def main():
    # Pull `-W...` pass-through flags straight out of argv: argparse cannot
    # take a positional/optional value that starts with '-', and every flag
    # worth screening does. Anything after a bare `--` is also a flag.
    argv = glue_option_values(sys.argv[1:], '--baseline-cflags')
    argv = glue_option_values(argv, '--flag')
    flags: List[str] = []
    rest: List[str] = []
    seen_ddash = False
    for tok in argv:
        if seen_ddash:
            flags.append(tok)
        elif tok == '--':
            seen_ddash = True
        elif tok.startswith('-W'):
            flags.append(tok)
        else:
            rest.append(tok)

    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--flag', action='append', default=None, metavar='FLAG',
                    help='Flag to screen (repeatable). Bare -W... tokens '
                         'anywhere on the command line are picked up '
                         'automatically, as is anything after a bare `--`.')
    ap.add_argument('--population', default='all', metavar='SELECTOR',
                    help='comma-separated key=value terms (see module docstring)')
    ap.add_argument('--sample', type=int, default=None, metavar='N',
                    help='seeded random subset of N functions')
    ap.add_argument('--seed', type=int, default=0)
    ap.add_argument('--limit', type=int, default=None,
                    help='first N of the (sorted) population')
    ap.add_argument('--baseline-cflags', action='append', default=None,
                    metavar='FLAGS',
                    help='Flags for the BASELINE compile (default: none, i.e. '
                         'the tree as it stands). Set it to compare flag A '
                         'against flag B rather than against stock -- e.g. '
                         '`-Wc,-pic1 --baseline-cflags=-Wc,-pic0`.')
    ap.add_argument('--strip-directive', action='append', default=None,
                    metavar='NAME',
                    help='Suppress this @PostProc directive in a TEMP COPY of '
                         'each source before compiling (repeatable). Use it to '
                         'ask "does this flag make the directive unnecessary?" '
                         'The tracked .c is never modified.')
    ap.add_argument('--jobs', type=int, default=4)
    ap.add_argument('--json', default=None, metavar='OUT.json')
    ap.add_argument('--list', action='store_true',
                    help='print the resolved population and exit')
    ap.add_argument('-v', '--verbose', action='store_true',
                    help='print every non-unchanged row')
    args = ap.parse_args(rest)

    flags += parse_extra_cflags(args.flag)
    base_flags_ = parse_extra_cflags(args.baseline_cflags)
    if not flags and not args.list:
        ap.error('no flag given (e.g. `screen_flag.py -Wo,-rege,0 ...` '
                 'or `--flag=-Olimit,1`)')

    pop = select_population(args.population)
    if args.sample is not None and args.sample < len(pop):
        random.Random(args.seed).shuffle(pop)
        pop = sorted(pop[:args.sample])
    if args.limit:
        pop = pop[:args.limit]

    if args.list:
        for ov, fn in pop:
            print(f"{ov} {fn}")
        print(f"# {len(pop)} functions", file=sys.stderr)
        return

    drop = list(args.strip_directive or [])
    print(f"flag(s):     {' '.join(flags)}")
    print(f"baseline:    {' '.join(base_flags_) or '(stock command line)'}")
    print(f"population:  {args.population}  -> {len(pop)} functions"
          + (f" (sample {args.sample}, seed {args.seed})" if args.sample else ""))
    if drop:
        print(f"suppressed:  @PostProc {', '.join(drop)} (in temp copies only)")
    print()

    workdir = tempfile.mkdtemp(prefix='screen_flag_')
    rows = []
    try:
        jobs = [(ov, fn, flags, base_flags_, drop, workdir) for ov, fn in pop]
        if args.jobs > 1:
            with ProcessPoolExecutor(max_workers=args.jobs) as ex:
                for i, row in enumerate(ex.map(_screen_one, jobs), 1):
                    rows.append(row)
                    _progress(i, len(jobs), rows)
        else:
            for i, job in enumerate(jobs, 1):
                rows.append(_screen_one(job))
                _progress(i, len(jobs), rows)
    finally:
        shutil.rmtree(workdir, ignore_errors=True)
    print()

    counts = {k: 0 for k in ORDER}
    for r in rows:
        counts[r['verdict']] = counts.get(r['verdict'], 0) + 1

    print("=" * 72)
    print(f"  {'NEWLY BYTE-PERFECT':<22} {counts['newly_byte_perfect']:>5}"
          "   <<< the only column that is a WIN")
    print(f"  {'BROKE A WIN':<22} {counts['broke_a_win']:>5}"
          "   <<< the only column that is a COST")
    print("  " + "-" * 68)
    for k in ORDER:
        if k in ('newly_byte_perfect', 'broke_a_win'):
            continue
        if counts.get(k):
            print(f"  {k:<22} {counts[k]:>5}")
    print(f"  {'TOTAL':<22} {len(rows):>5}")
    print("=" * 72)
    if not counts['newly_byte_perfect'] and not counts['broke_a_win']:
        print("VERDICT: no yield and no cost on this population. `improved` /"
              " `worsened`\n         are directional BIAS, not yield — do not"
              " plan a campaign off them\n         (see -Wc,-nooffsetopt,"
              " docs/reference/NOOFFSETOPT-INVESTIGATION.md).")

    for k in ('newly_byte_perfect', 'broke_a_win', 'downgraded',
              'error_under_flag'):
        hits = [r for r in rows if r['verdict'] == k]
        if hits:
            print(f"\n{k}:")
            for r in hits[:40]:
                print(f"  {r['overlay']:<10} {r['func']:<18} "
                      f"{r['base']} -> {r['flag']}")
            if len(hits) > 40:
                print(f"  ... and {len(hits) - 40} more")

    if args.verbose:
        print("\nall changed rows:")
        for r in rows:
            if r['verdict'] != 'unchanged':
                print(f"  {r['verdict']:<20} {r['overlay']:<10} {r['func']:<18}"
                      f" exact {r['base_exact']}->{r['flag_exact']}")

    if args.json:
        with open(args.json, 'w') as f:
            json.dump({'flags': flags, 'baseline_cflags': base_flags_,
                       'population': args.population,
                       'strip_directive': drop, 'counts': counts,
                       'rows': rows}, f, indent=2)
        print(f"\nWrote {args.json}")

    # Exit 0 always: a screen that finds nothing is a successful measurement.


def _progress(i, n, rows):
    nb = sum(1 for r in rows if r['verdict'] == 'newly_byte_perfect')
    bw = sum(1 for r in rows if r['verdict'] == 'broke_a_win')
    print(f"\r  {i}/{n}  new-bp={nb} broke-win={bw}", end='', file=sys.stderr,
          flush=True)


if __name__ == '__main__':
    main()
