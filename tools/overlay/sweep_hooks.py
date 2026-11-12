#!/usr/bin/env python3
"""sweep_hooks.py -- shared-queue @CompileEnv hook sweeper for overlay functions.

Sweeps a hook's parameter space over every open function and reports the
settings that improve the name-derived resolver WITHOUT buying the improvement
with a count deficit.

Why this exists
---------------
The 2026-08-23 session ran six of these by hand as throwaway scripts and hit the
same three problems every time:

1. **Fixed round-robin slices strand workers.**  `worker i` took every i-th
   function, so one 588-word function with a 500-setting space left seven of
   eight workers idle for twenty minutes.  This hands work out from a SHARED
   QUEUE instead: a worker claims the next item atomically (`O_CREAT|O_EXCL` on
   a claim file) and moves on when it is done.

2. **The accept criterion was wrong at first and had to be retrofitted.**  The
   resolver's `len t == len c` is the SLICED WINDOW: it can be equal while the
   compiled body is longer or shorter.  Four settings shipped before
   `audit_raw_count` was added as a gate, and five had to be reverted.  Both
   gates are mandatory here and cannot be skipped.

3. **A crash left the tree dirty.**  Every path restores the original file,
   including the exception path.

Accept criterion (all must hold, and `--apply` will not write without them):
  * the resolver improves on the baseline -- OR the baseline was count-unequal
    and the candidate makes it count-EXACT, which is a win in its own right
  * `*** BYTE-PERFECT ***` or a strictly higher resolved-word count
  * `len t == len c`
  * `audit_raw_count` PASS (or its own "known FALSE FAIL")

Note it does NOT gate on `semcheck` or `audit_paint_isomorphism`: a hook changes
codegen, not paints, so those must be compared against the file's own baseline
rather than tested for emptiness -- several open functions carry PRE-EXISTING
warnings and a naive `'clean' in output` test falsely rejects them.  Run them
when you apply.

    python3 tools/overlay/sweep_hooks.py FAMILY [--workers N] [--status S,...]
                                         [--out FILE] [--resume]

Families (each skips any file that already names the hook):
    precolor   Hook 18 IDO52_PRECOLOR_WEB     -- every COLOURED bitpos x 25 regs
    tempreg    Hook 36 IDO52_UGEN_TEMP_REG    -- every temp request x t0..t9
    webcolor   Hook 37 IDO52_FORCE_WEB_COLOR  -- every mapped web x 17 regs
    as1        the as1 fill/pull index spaces, singly
    as1pair    the same, as a 2-D matrix (LA_PULL x XBB/FALLTHRU)
    passes     Hook 15 IDO52_SKIP_PASSES, all 15 non-empty token subsets
    bools      the boolean / small-value settings

Measured yields on the 2026-08-23 open set, for picking what to run:
    tempreg 65%, precolor 58%, webcolor 33%, as1 25%, passes 17%, as1pair 8%.
⚠️ `precolor` and `webcolor` are NOT interchangeable: Hook 37 refuses any colour
in the web's own forbidden bitvector, which is exactly what Hook 18 overrides.
"""
import argparse
import errno
import itertools
import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
CC = ROOT / "tools/ido-static-recomp/build/5.2/out/cc"
SRC = ROOT / "src/overlay"
STATUS = ROOT / "config/overlay_match_status.json"

REGS = ['v0', 'v1', 'a0', 'a1', 'a2', 'a3', 't0', 't1', 't2', 't3', 't4', 't5',
        't6', 't7', 't8', 't9', 's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7', 'fp']
TREGS = ['t%d' % i for i in range(10)]
PASS_TOKENS = ['codemotion', 'eliminduct', 'opt_saved_regs', 'loopunroll']

ENV_RE = re.compile(r'(?m)^\s*(?:/\*|\*)\s*@CompileEnv:\s*(.*?)\s*\*/')


def trace(path, var, pattern, tag):
    env = dict(os.environ)
    env[var] = '1'
    with tempfile.NamedTemporaryFile(suffix='.o', delete=False) as t:
        obj = t.name
    try:
        r = subprocess.run([str(CC), '-O2', '-G0', '-mips1', '-EL', '-non_shared',
                            '-I', 'include', '-c', str(path), '-o', obj],
                           capture_output=True, text=True, env=env, cwd=str(ROOT), timeout=300)
        return sorted({int(x) for x in re.findall(pattern, r.stdout + r.stderr)})
    finally:
        try:
            os.unlink(obj)
        except OSError:
            pass


def settings_for(family, path):
    if family == 'precolor':
        bps = trace(path, 'IDO52_GC_COLOR_TRACE', r'bitpos=(\d+)[^\n]*?color=', 'gc')[:14]
        return ['IDO52_PRECOLOR_WEB=%s:%d' % (r, b) for b in bps for r in REGS]
    if family == 'tempreg':
        n = len(trace(path, 'IDO52_UGEN_TEMP_REG_TRACE', r'UGEN-TREG36\] occ=(\d+)', 'tr'))
        return ['IDO52_UGEN_TEMP_REG=%d:%s' % (o, r)
                for o in range(1, min(n, 24) + 1) for r in TREGS]
    if family == 'webcolor':
        ws = trace(path, 'IDO52_DUMP_WEBS', r'^\s*\[\s*(\d+)\] reg=', 'w')[:8]
        return ['IDO52_FORCE_WEB_COLOR=%d:%s' % (w, r) for w in ws for r in REGS]
    if family == 'passes':
        return ['IDO52_SKIP_PASSES=' + ','.join(c)
                for n in range(1, 5) for c in itertools.combinations(PASS_TOKENS, n)]
    A = (['IDO52_AS1_NO_LA_PULL=%d' % i for i in range(12)] + ['IDO52_AS1_NO_LA_PULL=all']
         + ['IDO52_AS1_FORCE_LA_PULL=%d' % i for i in range(12)] + ['IDO52_AS1_FORCE_LA_PULL=all'])
    B = (['IDO52_AS1_XBB_VOLATILE_OK=%d' % i for i in range(8)] + ['IDO52_AS1_XBB_VOLATILE_OK=all']
         + ['IDO52_AS1_XBB_SKIP_CAND=%d' % i for i in range(10)]
         + ['IDO52_AS1_NO_FALLTHRU_FILL=1', 'IDO52_AS1_XBB_BUDGET=0', 'IDO52_AS1_XBB_BUDGET=1'])
    if family == 'as1':
        return A + B
    if family == 'as1pair':
        return ['%s %s' % (a, b) for a in A for b in B]
    if family == 'bools':
        return ['IDO52_NO_LICM=1', 'IDO52_NO_CSE=1', 'IDO52_NO_DEADSTORE_GLOBALS=1',
                'IDO52_NO_STRENGTH_REDUCE=1', 'IDO52_NO_LOOP_TEST_REPLACEMENT=1',
                'IDO52_NO_FINDINDUCT_STRIDE=1', 'IDO52_AS1_NO_PASS1_PROP=1',
                'IDO52_AS1_KEEP_ANDI=1', 'IDO52_CSE_MAX_REUSE=1', 'IDO52_CSE_MAX_REUSE=2',
                'IDO52_LDA_NOINREG_CLASSES=1', 'IDO52_LDA_NOINREG_CLASSES=4',
                'IDO52_LDA_NOINREG_CLASSES=1,4'] + \
               ['IDO52_MAX_SREGS=%d' % i for i in range(5)]
    raise SystemExit('unknown family %r' % family)


def resolved(ov, fn):
    """(words, count_equal) or (-1, False).  Count-unequal is never a candidate."""
    r = subprocess.run([sys.executable, str(ROOT / 'tools/overlay/resolve_overlay_relocs.py'),
                        ov, fn], capture_output=True, text=True, cwd=str(ROOT), timeout=900)
    m = re.search(r'link: (\d+)/(\d+) words exact \(len t=(\d+) c=(\d+)\)', r.stdout)
    if not m:
        return -1, False
    return int(m.group(1)), m.group(3) == m.group(4)


def raw_ok(ov, fn):
    r = subprocess.run([sys.executable, str(ROOT / 'tools/overlay/audit_raw_count.py'),
                        ov, fn], capture_output=True, text=True, cwd=str(ROOT), timeout=900).stdout
    return 'PASS' in r or 'known FALSE FAIL' in r


def targets(statuses):
    st = json.load(open(STATUS))
    out = []
    for ov, fns in st.items():
        for fn, v in fns.items():
            if not (isinstance(v, dict) and v.get('status') in statuses):
                continue
            p = SRC / ov / f'{fn}.c'
            if p.exists():
                out.append((ov, fn, p))
    return sorted(out)


def claim(claimdir, key):
    """Atomically take ownership of one work item.  True = it is ours."""
    try:
        fd = os.open(os.path.join(claimdir, key.replace('/', '__')),
                     os.O_CREAT | os.O_EXCL | os.O_WRONLY)
        os.close(fd)
        return True
    except OSError as e:
        if e.errno == errno.EEXIST:
            return False
        raise


VAR_RE = {
    'precolor': r'IDO52_PRECOLOR_WEB=([0-9a-z:,]+)',
    'tempreg': r'IDO52_UGEN_TEMP_REG=([0-9a-z:,]+)',
    'webcolor': r'IDO52_FORCE_WEB_COLOR=([0-9a-z:,]+)',
}


def worker(family, statuses, claimdir, outpath, second=False):
    out = open(outpath, 'a')
    for ov, fn, p in targets(statuses):
        key = f'{ov}/{fn}'
        if not claim(claimdir, key):
            continue
        orig = p.read_text()
        try:
            base, eq = resolved(ov, fn)
            if base < 0:
                continue
            # A count_mismatch baseline is NOT skipped: a setting that makes the
            # body count-EXACT is itself a win even at a lower resolved score
            # (TACO/func_801406E4 did exactly that on 2026-08-23).  What is
            # never accepted is a CANDIDATE that is count-unequal.
            if second:
                # A SECOND value for a hook this file already carries.  The
                # first-round sweeps found second pins/forces nearly as
                # productive as first ones (48 of 101 for tempreg), so this is
                # its own pass rather than a re-run.
                if family not in VAR_RE:
                    raise SystemExit('--second not supported for %r' % family)
                cur = re.search(VAR_RE[family], orig)
                if not cur:
                    continue
                have = {x.split(':')[0] for x in cur.group(1).split(',')}
                var = VAR_RE[family].split('=')[0]
                sets = []
                for e in settings_for(family, p):
                    val = e.split('=', 1)[1]
                    if val.split(':')[0] in have or val.split(':')[1] in have:
                        continue
                    sets.append('%s=%s,%s' % (var, cur.group(1), val))
            else:
                sets = [e for e in settings_for(family, p)
                        if e.split('=')[0].split()[0] not in orig]
            if not sets:
                continue
            m = ENV_RE.search(orig)
            best = (base, '')
            for e in sets:
                if second:
                    var = e.split('=', 1)[0]
                    old = re.search(re.escape(var) + r'=[0-9a-z:,]+', orig).group(0)
                    s = orig.replace(old, e, 1)
                elif m:
                    s = orig.replace(m.group(0),
                                     m.group(0).replace(m.group(1), m.group(1) + ' ' + e), 1)
                else:
                    s = '/* @CompileEnv: %s */\n' % e + orig
                p.write_text(s)
                try:
                    sc, ok = resolved(ov, fn)
                except Exception:
                    sc, ok = -1, False
                if ok and (sc > best[0] or not eq) and raw_ok(ov, fn):
                    if not eq:
                        eq = True          # first count-exact candidate wins the seat
                        best = (sc, e)
                    elif sc > best[0]:
                        best = (sc, e)
            if best[1]:
                line = f'{key} {base} -> {best[0]} via {best[1]}'
                print(line, flush=True)
                out.write(line + '\n')
                out.flush()
        except Exception as ex:                                  # noqa: BLE001
            print(f'{key} ERR {ex!r}', flush=True)
        finally:
            p.write_text(orig)          # ALWAYS restore, including on crash


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('family')
    ap.add_argument('--workers', type=int, default=8)
    ap.add_argument('--status', default='semi_matched,count_mismatch')
    ap.add_argument('--out', default='/tmp/sweep_results.txt')
    ap.add_argument('--claimdir', default=None)
    ap.add_argument('--resume', action='store_true')
    ap.add_argument('--second', action='store_true',
                    help='sweep a SECOND value for a hook the file already names, '
                         'appended comma-separated to the existing one (a second '
                         'PRECOLOR pin / temp-reg force / web colour).  Without '
                         'this, files that already name the hook are skipped.')
    ap.add_argument('--worker', action='store_true', help=argparse.SUPPRESS)
    args = ap.parse_args()

    statuses = set(args.status.split(','))
    claimdir = args.claimdir or ('/tmp/sweep_claims_' + args.family)
    if args.worker:
        worker(args.family, statuses, claimdir, args.out, args.second)
        return
    if os.path.isdir(claimdir) and not args.resume:
        for f in os.listdir(claimdir):
            os.unlink(os.path.join(claimdir, f))
    os.makedirs(claimdir, exist_ok=True)
    n = len(targets(statuses))
    print(f'{args.family}: {n} targets, {args.workers} workers, '
          f'shared queue in {claimdir}')
    procs = [subprocess.Popen([sys.executable, __file__, args.family, '--worker'] +
                              (['--second'] if args.second else []) + [
                               '--out', args.out, '--claimdir', claimdir,
                               '--status', args.status], cwd=str(ROOT))
             for _ in range(args.workers)]
    for pr in procs:
        pr.wait()
    print('done; results in', args.out)


if __name__ == '__main__':
    main()
