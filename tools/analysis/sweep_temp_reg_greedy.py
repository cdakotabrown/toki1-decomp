#!/usr/bin/env python3
"""sweep_temp_reg_greedy.py -- GREEDY SEEDED Hook 36 sweep for a main-binary function.

    python3 tools/analysis/sweep_temp_reg_greedy.py <FUNC> <ADDR> <SIZE_BYTES> <src.c>
           [--rounds N] [--jobs N] [--seed '52:t9,48:t5'] [--apply]

Hook 36 is `IDO52_UGEN_TEMP_REG=<occ>:<treg>[,...]`, which forces ugen's integer
temp-register free-list pop at the given 1-BASED occurrence.  The occurrences are
whatever `IDO52_UGEN_TEMP_REG_TRACE=1` prints for this translation unit; this
tool counts them itself, so you do not have to.

WHY A GREEDY SEEDED SWEEP AND NOT A GRID
----------------------------------------
A single force is usually worth almost nothing, because ugen's free list is a
ROTATION: forcing one pop shifts every later pop, so the useful prescription is
a SET and the members only pay off together.  `tools/analysis/sweep_temp_reg.py`
tries one occurrence at a time and is a documented source of FALSE NEGATIVES for
exactly this reason.  This tool instead:

    round 1: try every (occ, reg) alone            -> keep the best
    round 2: try every (occ, reg) ON TOP of that   -> keep the best
    ...until a round buys nothing.

Measured on `main/spot_light` (2026-09-21): the single best force was worth
6 words; the greedy set of ten took the residual 49 -> 12, restored a delay-slot
fill that the rotation had been blocking (dropping the harness Fixup to 0), and
the function then closed byte-perfect.  A flat sweep of the same 68x10 grid found
only the 6.

⭐ AND WHEN GREEDY STALLS AT ROUND 1, USE `--staged`.
Greedy optimises the TOTAL, which is the wrong objective for a per-block temp
ROTATION: the force that fixes block 1 breaks the already-exact words of block 2,
so every single force scores worse and greedy converges having found nothing.
`--staged` instead pushes the FIRST mismatch forward and refuses outright any
candidate that reintroduces a mismatch inside the prefix already made exact, so
each block freezes once solved.  Measured on `make_color_bar16` (2026-09-21):
greedy over 318 requests x 10 registers reports no improvement in round 1 and
stalls at 49, while the staged search finds the five-force set that closes it
byte-perfect.  A residual that reads as a per-block rotation should get
`--staged` first.

⚠️ RE-SWEEP AFTER EVERY SOURCE CHANGE.  The occurrence numbering is a property of
the emitted stream, so editing the C renumbers it.  On that same function a
re-sweep after one source fix found a force (`45:t3`) that did not exist before
and was worth the last two words.

SCORING
-------
The score is the number of words whose MNEMONIC OR REGISTERS differ from the
target (`dump_main_diff`'s normalisation: immediates and displacements masked,
because at an unlinked relocation site they are 0 by construction), plus the
absolute length difference.  Lower is better.

⚠️ This is a REGISTER/SHAPE metric, not a win condition.  `test_single_match`
remains authoritative, and CLAUDE.md's rule still applies: `Exact: X/N` uses
N = min(target, compiled), so confirm `Body == target` before believing a gain.
A force that SHORTENS the body can lower this score while making the function
worse -- the length penalty is here to make that visible, not to excuse it.

`--apply` writes the winning set into the file's `@CompileEnv:` line (creating
one if absent); without it the tool only prints the directive.
"""
import argparse
import importlib.util
import os
import re
import subprocess
import sys
import tempfile

ROOT = '.'
REGS = ['t%d' % i for i in range(10)]


def load_dmd(addr):
    """Import dump_main_diff as a library (it guards its own CLI under __main__)."""
    saved = sys.argv
    sys.argv = ['dmd']
    spec = importlib.util.spec_from_file_location(
        'dmd', os.path.join(ROOT, 'tools/analysis/dump_main_diff.py'))
    m = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(m)
    sys.argv = saved
    m.ADDR_S = '0x%08X' % addr
    return m


def count_occurrences(src):
    """How many integer temp-register requests this TU makes.

    Read from the hook's own trace rather than guessed, because a blind 0..N
    grid both misses sites and wastes compiles -- the same 1-based/0-based trap
    CLAUDE.md records for Hook 22.
    """
    txt = open(src).read()
    flags = []
    for m in re.finditer(r'(?m)^\s*(?:/\*|\*)?\s*@CompileFlags:\s*(.+)', txt):
        flags += [x for x in m.group(1).replace('*/', '').split() if x.startswith('-')]
    env = dict(os.environ)
    env['IDO52_UGEN_TEMP_REG_TRACE'] = '1'
    with tempfile.NamedTemporaryFile(suffix='.o', delete=False) as t:
        obj = t.name
    cmd = [os.path.join(ROOT, 'tools/ido-static-recomp/build/5.2/out/cc'),
           '-O2', *flags, '-G0', '-mips1', '-EL', '-non_shared',
           '-c', '-I', 'include', src, '-o', obj]
    r = subprocess.run(cmd, cwd=ROOT, env=env, capture_output=True, text=True)
    os.unlink(obj)
    return len(re.findall(r'\[UGEN-TREG36\] occ=\d+ -> r', r.stdout + r.stderr))


WORKER = r'''
import sys, importlib.util
ROOT = '.'
FN, ADDR, SIZE, SRC = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3]), sys.argv[4]
if SIZE % 4:
    # SIZE is BYTES.  A word count (e.g. 671) is silently a quarter-window
    # compare (2026-09-25: 671 measured 167 words and reported a fit).
    sys.exit("SIZE must be the window size in BYTES (a multiple of 4); got %d" % SIZE)
LO, HI = int(sys.argv[5]), int(sys.argv[6])
BASE = sys.argv[7] if len(sys.argv) > 7 else ''
sys.argv = ['dmd']
spec = importlib.util.spec_from_file_location('dmd', ROOT + '/tools/analysis/dump_main_diff.py')
m = importlib.util.module_from_spec(spec); spec.loader.exec_module(m)
m.ADDR_S = '0x%08X' % ADDR
T = m.target(ADDR, SIZE)
def score(env):
    """(total mismatches incl. the length penalty, index of the FIRST one)."""
    try:
        o = m.build(SRC, FN, extra_env=env)
    except SystemExit:
        return (-1, -1)
    c = m.words(o, FN)
    if not c:
        return (-1, -1)
    n = min(len(T), len(c))
    bad = [i for i in range(n) if m.norm(T[i][2]) != m.norm(c[i][2])]
    return (len(bad) + abs(len(T) - len(c)), bad[0] if bad else n)
if LO == 0:
    print('BASE %d %d' % score({'IDO52_UGEN_TEMP_REG': BASE} if BASE else {}), flush=True)
    sys.exit()
for occ in range(LO, HI + 1):
    for r in range(10):
        e = {'IDO52_UGEN_TEMP_REG': (BASE + ',' if BASE else '') + '%d:t%d' % (occ, r)}
        print('%d t%d %d %d' % ((occ, r) + score(e)), flush=True)
'''


def run_chunks(worker, fn, addr, size, src, nocc, base, jobs):
    """Score the whole grid.  One PROCESS per chunk, never threads.

    dump_main_diff's scratch directory is keyed on the PID (it used to be a
    fixed /tmp name, and two concurrent runs then read each other's objects),
    so separate processes are what makes this safe to parallelise.
    """
    procs, out = [], []
    step = max(1, (nocc + jobs - 1) // jobs)
    for lo in range(1, nocc + 1, step):
        hi = min(lo + step - 1, nocc)
        procs.append(subprocess.Popen(
            [sys.executable, worker, fn, '0x%X' % addr, str(size), src, str(lo), str(hi), base],
            cwd=ROOT, stdout=subprocess.PIPE, text=True))
    for p in procs:
        out += p.communicate()[0].splitlines()
    best = []
    for line in out:
        f = line.split()
        if len(f) == 4 and f[2].lstrip('-').isdigit() and int(f[2]) >= 0:
            best.append((int(f[2]), int(f[0]), f[1], int(f[3])))
    best.sort()
    return best


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('func')
    ap.add_argument('addr')
    ap.add_argument('size', type=int)
    ap.add_argument('src')
    ap.add_argument('--rounds', type=int, default=12)
    ap.add_argument('--jobs', type=int, default=12)
    ap.add_argument('--seed', default='')
    ap.add_argument('--staged', action='store_true',
                    help='prefix-locked search: push the FIRST mismatch forward and never '
                         'break an already-exact prefix.  Use when the residual reads as a '
                         'per-block register ROTATION, where plain greedy stalls at round 1.')
    ap.add_argument('--apply', action='store_true')
    a = ap.parse_args()
    addr = int(a.addr, 16)

    nocc = count_occurrences(a.src)
    if not nocc:
        print('0 temp-register requests -- Hook 36 has no site to act on here.')
        return
    print('%d temp-register requests' % nocc)

    with tempfile.NamedTemporaryFile('w', suffix='.py', delete=False) as t:
        t.write(WORKER)
        worker = t.name

    m = load_dmd(addr)
    T = m.target(addr, a.size)

    def score(base):
        """(total mismatches incl. the length penalty, index of the FIRST one)."""
        env = {'IDO52_UGEN_TEMP_REG': base} if base else {}
        o = m.build(a.src, a.func, extra_env=env)
        c = m.words(o, a.func)
        n = min(len(T), len(c))
        bad = [i for i in range(n) if m.norm(T[i][2]) != m.norm(c[i][2])]
        return (len(bad) + abs(len(T) - len(c)), bad[0] if bad else n)

    base = a.seed
    cur, first = score(base)
    print('baseline score %d (first mismatch at word %d)%s'
          % (cur, first, ' [seeded]' if base else ''))

    if a.staged:
        # PREFIX-LOCKED search.  Plain greedy cannot find a per-block temp
        # ROTATION: the force that fixes block 1 breaks the already-exact
        # words of block 2, so every single force scores worse on the total
        # and greedy converges immediately at round 1.  Measured on
        # make_color_bar16, where greedy stalls at 49 and this finds the
        # five-force set that closes it byte-perfect.
        #
        # So do not optimise the total.  Push the FIRST mismatch forward, and
        # refuse outright any candidate that reintroduces a mismatch inside
        # the prefix already made exact.  Each block freezes once solved.
        lock = first
        for rnd in range(1, a.rounds + 1):
            rows = run_chunks(worker, a.func, addr, a.size, a.src, nocc, base, a.jobs)
            cand = [r for r in rows if r[3] >= lock]
            if not cand:
                print('round %d: every candidate breaks the locked prefix -- stop' % rnd)
                break
            cand.sort(key=lambda r: (-r[3], r[0]))
            total, occ, reg, f = cand[0]
            if f <= lock and total >= cur:
                print('round %d: no candidate advances the prefix -- converged' % rnd)
                break
            base = (base + ',' if base else '') + '%d:%s' % (occ, reg)
            print('round %d: +%d:%s  first %d -> %d, total %d -> %d'
                  % (rnd, occ, reg, lock, f, cur, total))
            lock, cur = f, total
    else:
        converged = False
        for rnd in range(1, a.rounds + 1):
            best = run_chunks(worker, a.func, addr, a.size, a.src, nocc, base, a.jobs)
            if not best or best[0][0] >= cur:
                print('round %d: no improvement -- converged' % rnd)
                converged = True
                break
            s, occ, reg, _f = best[0]
            base = (base + ',' if base else '') + '%d:%s' % (occ, reg)
            print('round %d: +%d:%s  %d -> %d' % (rnd, occ, reg, cur, s))
            cur = s
        if not converged:
            # Silently stopping on the round limit LOOKS like a result and is
            # not one.  touzyou_basyo_check stopped at 84 on a 6-round cap; re-seeding
            # from those forces and re-running added three more and reached 55.
            print('⚠️  STILL IMPROVING at the round limit -- this is NOT a '
                  'converged result.\n'
                  '    Re-run seeded with the set below and a higher --rounds.')
    os.unlink(worker)

    if not base:
        print('nothing found')
        return
    print('\nscore %d with:\n * @CompileEnv: IDO52_UGEN_TEMP_REG=%s' % (cur, base))

    if a.apply:
        txt = open(a.src).read()
        if re.search(r'(?m)^\s*(?:/\*|\*)?\s*@CompileEnv:.*IDO52_UGEN_TEMP_REG=', txt):
            txt = re.sub(r'(IDO52_UGEN_TEMP_REG=)[^\s*]+', r'\g<1>' + base, txt, count=1)
        else:
            # The new line must be spelled the way its NEIGHBOUR is.  A header
            # block wants ` * @...`; a standalone directive line wants its own
            # `/* ... */`.  Emitting the block form next to a standalone line
            # writes a bare ` * @CompileEnv:` into the code and the file no
            # longer compiles -- hit on SD_CDLevelAvg.
            m = re.search(r'(?m)^(\s*)(/\*)?(\s*)\*?\s*@CompileFlags:', txt)
            if not m:
                print('no @CompileFlags line to anchor to; not written')
                return
            line = '/* @CompileEnv: IDO52_UGEN_TEMP_REG=%s */' % base if m.group(2) \
                else ' * @CompileEnv: IDO52_UGEN_TEMP_REG=%s' % base
            txt = txt[:m.start()] + line + '\n' + txt[m.start():]
        open(a.src, 'w').write(txt)
        print('written to %s' % a.src)


if __name__ == '__main__':
    main()
