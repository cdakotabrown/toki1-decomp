#!/usr/bin/env python3
"""sweep_split_admit.py -- greedy sweep of uopt Hook 53 split-admission decisions
(IDO52_SPLIT_ADMIT / IDO52_SPLIT_REJECT), optionally together with Hook 37
colour forces, for a MAIN-BINARY function.

    python3 tools/analysis/sweep_split_admit.py FUNC 0xADDR SIZE src.c
        [--rounds N] [--jobs N] [--no-colour] [--no-split] [--reject P:B,...]

Each round compiles once with IDO52_SPLIT_TRACE=1 / IDO52_GC_COLOR_TRACE=1,
turns every f_addadjacents decision into a candidate (REJECT for a natural
admit, ADMIT for a block only the heuristic rejects) and every coloured or
spilled web into Hook 37 candidates (`--no-colour` disables those), scores all
of them in parallel and keeps the best.  Score = dump_main_diff's normalised
mnemonic/register mismatches + |length difference| -- a SHAPE metric; confirm
with test_single_match (Exact AND Body == target).  Bitposes of split
remainders are fresh per compile, so re-run after any source/env change.
Prints the directive; it never edits the file.  See UOPT-HOOKS-REFERENCE
Hook 53.
"""
import argparse, importlib.util, os, re, subprocess, sys, tempfile, json
ROOT = '.'

WORKER = r'''
import sys, importlib.util, json, os
ROOT = '.'
FN, ADDR, SIZE, SRC = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3]), sys.argv[4]
jobs = json.loads(sys.argv[5])
sys.argv = ['dmd']
spec = importlib.util.spec_from_file_location('dmd', ROOT + '/tools/analysis/dump_main_diff.py')
m = importlib.util.module_from_spec(spec); spec.loader.exec_module(m)
m.ADDR_S = '0x%08X' % ADDR
T = m.target(ADDR, SIZE)
for key, env in jobs:
    try:
        o = m.build(SRC, FN, extra_env=env)
        c = m.words(o, FN)
    except SystemExit:
        c = None
    if not c:
        print(json.dumps([key, -1, -1, 0])); continue
    n = min(len(T), len(c))
    bad = [i for i in range(n) if m.norm(T[i][2]) != m.norm(c[i][2])]
    print(json.dumps([key, len(bad) + abs(len(T) - len(c)), len(c), n - len(bad)]), flush=True)
'''


def src_env(src):
    txt = open(src).read()
    m = re.search(r'@CompileEnv:(.*)', txt)
    env = {}
    if m:
        for tok in m.group(1).replace('*/', '').split():
            if '=' in tok:
                k, v = tok.split('=', 1)
                env[k] = v
    flags = []
    for mm in re.finditer(r'@CompileFlags:\s*(.+)', txt):
        flags += [x for x in mm.group(1).replace('*/', '').split() if x.startswith('-')]
    return env, flags


def events(src, admit, reject, usr_lib, cols=()):
    env0, flags = src_env(src)
    env = dict(os.environ)
    env.update(env0)
    env['IDO52_SPLIT_TRACE'] = '1'
    if admit: env['IDO52_SPLIT_ADMIT'] = ','.join(admit)
    if reject: env['IDO52_SPLIT_REJECT'] = ','.join(reject)
    env['USR_LIB'] = usr_lib
    env['IDO52_GC_COLOR_TRACE'] = '1'
    env.update({k: v for k, v in mkenv(admit, reject, usr_lib, cols).items() if v})
    with tempfile.NamedTemporaryFile(suffix='.o', delete=False) as t:
        obj = t.name
    cmd = [os.path.join(usr_lib, 'cc'), '-O2', '-G0', '-mips1', '-EL', '-non_shared',
           *flags, '-Iinclude', '-c', src, '-o', obj]
    r = subprocess.run(cmd, cwd=ROOT, env=env, capture_output=True, text=True)
    os.unlink(obj)
    ev = []
    for line in (r.stdout + r.stderr).splitlines():
        mm = re.search(r'admit\? piece=(\d+) rest=\d+ bb=(\d+) .* heurOFF=(\d) natural=(\w+)', line)
        if mm:
            p, b, hoff, nat = mm.groups()
            if nat == 'ADMIT':
                ev.append(('R', '%s:%s' % (p, b)))
            elif hoff == '1':
                ev.append(('A', '%s:%s' % (p, b)))
        mm = re.search(r'\[GC-COLOR\] phase=\d bitpos=(\d+) lr=\S+ (class|SPILLED)', line)
        if mm:
            ev.append(('C', mm.group(1)))
    return sorted(set(ev))


BASECOL = ''
def mkenv(admit, reject, usr_lib, cols=()):
    e = {'USR_LIB': usr_lib, 'IDO52_SPLIT_ADMIT': ','.join(admit),
         'IDO52_SPLIT_REJECT': ','.join(reject)}
    c = ','.join([x for x in [BASECOL] + list(cols) if x])
    if c:
        e['IDO52_FORCE_WEB_COLOR'] = c
    return e


def run(worker, a, jobs):
    chunks = [jobs[i::a.jobs] for i in range(a.jobs)]
    procs = [subprocess.Popen([sys.executable, worker, a.func, a.addr, str(a.size), a.src,
                               json.dumps(c)], cwd=ROOT, stdout=subprocess.PIPE, text=True)
             for c in chunks if c]
    out = []
    for p in procs:
        out += [json.loads(l) for l in p.communicate()[0].splitlines() if l.startswith('[')]
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('func'); ap.add_argument('addr'); ap.add_argument('size', type=int)
    ap.add_argument('src')
    ap.add_argument('--rounds', type=int, default=6)
    ap.add_argument('--jobs', type=int, default=6)
    ap.add_argument('--usr-lib', default=os.path.join(ROOT, 'tools/ido-static-recomp/build/5.2/out'))
    ap.add_argument('--admit', default='')
    ap.add_argument('--reject', default='')
    ap.add_argument('--cols', default='')
    ap.add_argument('--regs', default='v0,v1,a0,a1,a2,a3,t0,t1,t2,t3,t4,t5')
    ap.add_argument('--no-split', action='store_true')
    ap.add_argument('--no-colour', action='store_true', help='split candidates only')
    a = ap.parse_args()
    fenv = src_env(a.src)[0]
    # start from the file's own Hook 53 directives; --admit/--reject add to them
    admit = [x for x in (fenv.get('IDO52_SPLIT_ADMIT', '') + ',' + a.admit).split(',') if x]
    reject = [x for x in (fenv.get('IDO52_SPLIT_REJECT', '') + ',' + a.reject).split(',') if x]
    cols = [x for x in a.cols.split(',') if x]
    global BASECOL
    BASECOL = src_env(a.src)[0].get('IDO52_FORCE_WEB_COLOR', '')
    with tempfile.NamedTemporaryFile('w', suffix='.py', delete=False) as t:
        t.write(WORKER); worker = t.name
    base = run(worker, a, [['base', mkenv(admit, reject, a.usr_lib, cols)]])[0]
    print('BASE score=%d len=%d exact-ish=%d' % tuple(base[1:]), flush=True)
    best = base[1]
    for rnd in range(a.rounds):
        ev = events(a.src, admit, reject, a.usr_lib, cols)
        jobs = []
        for kind, key in ev:
            if a.no_split and kind in 'AR':
                continue
            if kind == 'A' and key not in admit:
                jobs.append([kind + key, mkenv(admit + [key], reject, a.usr_lib, cols)])
            if kind == 'R' and key not in reject:
                jobs.append([kind + key, mkenv(admit, reject + [key], a.usr_lib, cols)])
            if kind == 'C' and not a.no_colour:
                for r in a.regs.split(','):
                    c = '%s:%s' % (key, r)
                    if c not in cols:
                        jobs.append(['C' + c, mkenv(admit, reject, a.usr_lib, cols + [c])])
        res = [r for r in run(worker, a, jobs) if r[1] >= 0]
        res.sort(key=lambda r: r[1])
        print('round %d: %d candidates; top: %s' % (rnd + 1, len(jobs), res[:6]), flush=True)
        if not res or res[0][1] >= best:
            break
        best = res[0][1]
        k = res[0][0]
        {'A': admit, 'R': reject, 'C': cols}[k[0]].append(k[1:])
        print('  take %s -> score %d  ADMIT=%s REJECT=%s COLS=%s' % (k, best, ','.join(admit),
                                                              ','.join(reject), ','.join(cols)), flush=True)
    print('FINAL score=%d IDO52_SPLIT_ADMIT=%s IDO52_SPLIT_REJECT=%s COLS=%s' % (
        best, ','.join(admit), ','.join(reject), ','.join(cols)))


if __name__ == '__main__':
    main()
