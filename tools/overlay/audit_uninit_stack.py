#!/usr/bin/env python3
"""Flag compiled objects that are SILENTLY BROKEN in one of two ways.

Both were found on 2026-08-29 in files whose scores looked healthy, and
neither is visible to the masked test, to semcheck, or to the resolver.

CHECK 1 -- reads a local stack slot nothing ever writes.

Found 2026-08-29: a semi carrying `@CompileEnv: IDO52_SKIP_PASSES=codemotion`
scored 68/79 while being a MISCOMPILE — with code motion off, uopt dropped a
`(val + 1) & 0xFF` computation entirely and one switch arm then loaded its call
argument from an uninitialised slot (`lw $a0,48($sp)` twice, in a frame that
existed only to hold that phantom spill).  A high resolved-word count is not
evidence of a correct object.

The check: disassemble the compiled object, collect every `s{b,h,w}` write to
`N($sp)` and every `l{b,bu,h,hu,w}` read of `N($sp)`, and report reads of a slot
that is never written anywhere in the function AND lies below the frame size
(so it is a local, not a caller-provided argument home).

Limits, stated plainly: this is a whole-function check, not a dataflow one. It
cannot see a slot written on one path and read on another (that is legal), and
it says nothing about a slot written with the wrong value. It catches the
specific, and quite common, shape above.

CHECK 2 -- reads a register nothing in the function ever defines.
⚠️ This check is a DIAGNOSTIC, not a screen.  A function with a HIDDEN REGISTER
PARAMETER legitimately reads a register its own window never writes -- that is a
documented category here, and on this tree every remaining CHECK-2 hit is one
(EVENT/func_80140D88, RPG_BAT/func_80144B8C, RPG_BAT/func_80151600 are all
byte-perfect and all say so in their headers).  Use CHECK 2 to CONFIRM a
suspected deleted definition on one function, not to sweep a population.
`TT/func_8013B1C4` shipped `@CompileEnv: IDO52_PRECOLOR_WEB=t2:156`, which
DELETED the `li $t2,160` feeding `sh $t2,4($s2)`; the object stored a stale
`sll` result.  A colour hook's resolver gain must be checked against the
disassembly, not the score.

Both checks are whole-function and deliberately conservative: they flag only a
slot or register that is read and NEVER written anywhere in the body.  That
cannot produce a false positive from path-sensitivity, and it will miss a
value that is written on one path and read on another, or written with a wrong
value.  It catches the two shapes above, which is what it is for.

    python3 tools/overlay/audit_uninit_stack.py <OVERLAY> <FUNC>
    python3 tools/overlay/audit_uninit_stack.py --scan-hook codemotion
    python3 tools/overlay/audit_uninit_stack.py --scan-hook PRECOLOR_WEB
"""
import os, re, sys, glob, json, subprocess, argparse, tempfile

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
# NOTE: mips-linux-gnu-objdump prints registers WITHOUT a `$` prefix
# (`sh t2,4(s2)`).  An earlier version of this file required `$` and therefore
# matched nothing at all -- it reported "0 flagged" on 155 functions while
# detecting nothing.  Accept both spellings.
REG = r'\$?(?:zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|k[01]|gp|sp|fp|ra)'
W = re.compile(r'\bs(?:b|h|w)\s+' + REG + r',\s*(-?\d+)\(\$?sp\)')
R = re.compile(r'\bl(?:b|bu|h|hu|w)\s+' + REG + r',\s*(-?\d+)\(\$?sp\)')
FRAME = re.compile(r'addiu\s+\$?sp,\$?sp,-(\d+)')
# A local whose ADDRESS is handed to a callee is legitimately read without any
# visible write in this function -- the callee fills it.  `addiu $rX,$sp,N`
# with rX != sp is that pattern (`f(&local)`), and every offset at or above the
# lowest such N may be callee-written.  Without this, CHECK 1 flags any
# out-parameter or by-address struct: on the first real sweep it produced 24
# hits of which most were `f(&local)` (GEKO/func_801441A0 flagged 42..55($sp)
# while taking $sp+40; TACO/func_8013DE04 flagged 152..174 while taking 144).
TAKEN = re.compile(r'addiu\s+\$?(?!sp\b)(\w+),\s*\$?sp,\s*(-?\d+)')
# ^ 2026-08-29: this line required `$sp` while objdump prints `sp`, so
# `frame` was ALWAYS 0 and CHECK 1's `0 <= o < frame` filter could never
# be true -- the check reported "ok" for every function ever passed to it,
# including RPG_BAT/func_80154624, which reads FIVE never-written slots.
# Same `$`-prefix bug the note above records for W and R; it was fixed in
# two of the three regexes.  A vacuous check is worse than no check: it
# produced a clean sweep that was cited as evidence.
# registers live on entry or defined by the ABI -- never "undefined"
ABI = {'a0', 'a1', 'a2', 'a3', 'sp', 'ra', 'gp', 'fp', 'zero', 'at',
       's8', 'k0', 'k1'}
INSN = re.compile(r'^\s*[0-9a-f]+:\s+[0-9a-f]{8}\s+(\S+)\s*(.*)$')
STORE_OPS = ('sb', 'sh', 'sw', 'swl', 'swr', 'swc', 'beq', 'bne', 'blez',
             'bgtz', 'bltz', 'bgez', 'j', 'jr', 'jal', 'jalr', 'mult',
             'multu', 'div', 'divu', 'mtlo', 'mthi', 'break', 'nop')


def check(ov, fn):
    with tempfile.TemporaryDirectory() as td:
        r = subprocess.run(
            [sys.executable, os.path.join(ROOT, 'tools/overlay/test_overlay_match.py'),
             ov, fn, '--keep', td],
            capture_output=True, text=True, cwd=ROOT)
        objs = sorted(glob.glob(os.path.join(td, '**', '*.o'), recursive=True))
        # prefer the RAW compile (.o), not the post-PostProc .ppN.o:
        # PostProc can only permute/patch, so the raw object is what the
        # compiler actually decided, which is the question here.
        raw = [o for o in objs if re.search(r'/[^/]+\.o$', o)
               and not re.search(r'\.pp\d+\.o$', o)]
        objs = raw or objs
        if not objs:
            return None, 'no object (%s)' % (r.stdout.strip().splitlines()[-1:] or [''])[0][:60]
        dis = subprocess.run(['mips-linux-gnu-objdump', '-d', objs[-1]],
                             capture_output=True, text=True).stdout
    m = FRAME.search(dis)
    frame = int(m.group(1)) if m else 0
    writes = {int(x) for x in W.findall(dis)}
    reads = {int(x) for x in R.findall(dis)}
    taken = [int(n) for _, n in TAKEN.findall(dis)]
    cutoff = min(taken) if taken else frame
    bad = ['%d($sp)' % o for o in sorted(reads - writes)
           if 0 <= o < frame and o < cutoff]

    # CHECK 2: a register read that nothing in the body ever defines
    defined, used = set(ABI), []
    for line in dis.splitlines():
        m = INSN.match(line)
        if not m:
            continue
        op, rest = m.group(1), m.group(2)
        regs = [r.lstrip('$') for r in re.findall(REG, rest)]
        # NOTE: `jal 0 <label>` has NO register operands, so this must be
        # handled BEFORE the empty-regs skip -- otherwise the call never marks
        # $v0 defined and every `x = f();` reads as undefined.
        if op.startswith(('jal', 'jalr')):
            used += regs
            defined.update(('v0', 'v1'))
            continue
        if not regs:
            continue
        if op.startswith(STORE_OPS):
            used += regs
        else:
            used += regs[1:]
            defined.add(regs[0])
    undef = sorted({r for r in used if r not in defined})
    bad += ['%s (never defined)' % r for r in undef]
    return bad, None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('overlay', nargs='?')
    ap.add_argument('func', nargs='?')
    ap.add_argument('--scan-hook', help='scan every .c whose @CompileEnv names this token')
    args = ap.parse_args()

    if args.scan_hook:
        targets = []
        for f in sorted(glob.glob(os.path.join(ROOT, 'src/overlay/*/*.c'))):
            if re.search(r'@CompileEnv:[^\n]*' + re.escape(args.scan_hook),
                         open(f, errors='replace').read()):
                targets.append((f.split(os.sep)[-2], os.path.basename(f)[:-2]))
    elif args.overlay and args.func:
        targets = [(args.overlay, args.func)]
    else:
        ap.error('give OVERLAY FUNC, or --scan-hook TOKEN')

    flagged = 0
    for ov, fn in targets:
        bad, err = check(ov, fn)
        if err:
            print("  ?? %-10s %-18s %s" % (ov, fn, err))
            continue
        if bad:
            flagged += 1
            print("  ⚠️  %-10s %-18s %s" % (ov, fn, ', '.join(bad)))
        else:
            print("  ok %-10s %-18s" % (ov, fn))
    print("\n%d function(s) checked, %d flagged" % (len(targets), flagged))
    return 1 if flagged else 0


if __name__ == '__main__':
    sys.exit(main())
