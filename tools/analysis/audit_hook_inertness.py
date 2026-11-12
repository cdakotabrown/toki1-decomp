#!/usr/bin/env python3
"""Find @CompileEnv hooks that are SILENT NO-OPS.

A hook that is wired into the recomp but never changes any object is worse
than a missing one: every "refuted with HOOK" note written against it reads
as evidence that the mechanism was tested, when in fact nothing happened.
CLAUDE.md already records this failure mode for the SKIP_PASSES tokens
("did nothing and reported nothing, a silent no-op that mis-attributed at
least one blocker"); this tool makes it checkable.

Method: compile a random sample of real overlay .c files with and without
each hook and compare object md5s.  A hook that changes 0 of N objects at
every value tried is reported INERT.  Values are tried in order, and the
first value that changes anything wins -- which is how the `=1` vs `=all`
trap shows up (IDO52_AS1_NO_COPY_PROP is inert at 1 and live at all).

Limits, stated plainly: INERT here means "inert on this sample at these
values".  A hook that only fires on a shape absent from the sample will be
reported INERT; raise --sample or pass --files to test a targeted set.  A
hook reported LIVE is definitely live.

    python3 tools/analysis/audit_hook_inertness.py
    python3 tools/analysis/audit_hook_inertness.py --sample 60
    python3 tools/analysis/audit_hook_inertness.py --only IDO52_AS1_NO_FILL
"""
import argparse, glob, hashlib, os, random, re, subprocess, sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
CC = os.path.join(ROOT, 'tools/ido-static-recomp/build/5.2/out/cc')
AS1 = os.path.join(ROOT, 'tools/ido-static-recomp/build/5.2/as1.c')
UOPT = os.path.join(ROOT, 'build/5.2/uopt.c')
VALUES = ['1', 'all', '0', '0,1', '2']


def discover():
    names = set()
    for p in (AS1, UOPT):
        if os.path.exists(p):
            names |= set(re.findall(rb'IDO52_[A-Z0-9_]+', open(p, 'rb').read()))
    out = sorted(n.decode() for n in names)
    # trace-only knobs never change codegen by design
    return [n for n in out if not n.endswith(('_TRACE', '_DEBUG', '_DUMP'))]


def build(path, env, tmp):
    e = dict(os.environ); e.update(env)
    r = subprocess.run([CC, '-O2', '-G0', '-mips1', '-EL', '-non_shared', '-c',
                        '-I', os.path.join(ROOT, 'include'), path, '-o', tmp],
                       env=e, capture_output=True, cwd=ROOT)
    if r.returncode:
        return None
    return hashlib.md5(open(tmp, 'rb').read()).hexdigest()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--sample', type=int, default=25)
    ap.add_argument('--only')
    ap.add_argument('--seed', type=int, default=7)
    a = ap.parse_args()

    tmp = '/tmp/hookinert.o'
    pool = sorted(glob.glob(os.path.join(ROOT, 'src/overlay/*/*.c')))
    random.seed(a.seed)
    files = random.sample(pool, min(a.sample, len(pool)))

    base = {}
    for f in files:
        h = build(f, {}, tmp)
        if h:
            base[f] = h
    if not base:
        print('no compilable sample'); return 1
    print('%d baseline objects\n' % len(base))

    hooks = [a.only] if a.only else discover()
    inert = []
    for h in hooks:
        # ⚠️ A random sample can miss the shape a hook fires on entirely --
        # IDO52_AS1_KEEP_ANDI showed 0/25 on the random sample and is
        # demonstrably LIVE on the two files that actually ship it.  So always
        # test the files whose own @CompileEnv names this hook, first.
        targeted = {}
        for f in pool:
            try:
                t = open(f, errors='replace').read()
            except OSError:
                continue
            if re.search(r'@CompileEnv:[^\n]*' + re.escape(h) + r'\b', t):
                hh = build(f, {}, tmp)
                if hh:
                    targeted[f] = hh
        probe = dict(base); probe.update(targeted)
        hit = None
        for v in VALUES:
            n = 0
            for f, b in probe.items():
                got = build(f, {h: v}, tmp)
                if got and got != b:
                    n += 1
            if n:
                hit = (v, n); break
        if hit:
            print('  LIVE   %-36s =%-4s %2d/%d%s' % (h, hit[0], hit[1], len(probe),
                  '  (via its own users)' if targeted else ''))
        else:
            inert.append(h)
            print('  INERT  %-36s (no value in %s changed any of %d)%s' % (h, VALUES, len(probe),
                  '' if targeted else '  -- NO file ships it, so untargeted'))
    print('\n%d hooks checked, %d INERT on this sample' % (len(hooks), len(inert)))
    if inert:
        print('⚠️  A "refuted with HOOK" note citing any of these tested NOTHING.')
    return 0


if __name__ == '__main__':
    sys.exit(main())
