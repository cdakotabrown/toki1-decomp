#!/usr/bin/env python3
"""Semantic-damage checker — analyses the FINAL PostProc'd object (the thing
that actually gets committed / compared), not an intermediate.

A correct compiler + legal-PostProc pipeline never emits branch-in-delay-slot,
$ra-clobber-between-restore-and-return, a jal delay slot that writes $ra, a
frame access after the sp dealloc, or an R3000 load-use hazard. Any such defect
in the final object is PostProc damage — most often an un-fenced whole-function
`scheduler_normalizer` reordering across a branch (CLAUDE.md calls the
whole-function run on multi-case functions an ANTI-PATTERN).

This is a STRUCTURAL check: it can prove damage, it cannot prove correctness.
It is opt-in (`test_overlay_match.py --semcheck`), not a default gate.

Usage:
    # one function (compiles it through the real pipeline first)
    python3 tools/overlay/semcheck.py TACO func_80145600

    # a whole overlay / the whole tree (only functions carrying a
    # reorder-class PostProc, which is the damage class)
    python3 tools/overlay/semcheck.py --all
    python3 tools/overlay/semcheck.py --all --overlay TACO --jobs 8
    python3 tools/overlay/semcheck.py --all --every        # no PostProc filter

    # analyse an object / raw .text you already have
    python3 tools/overlay/semcheck.py --object path/to/func.o --func func_X
"""
import os
import re
import sys
import glob
import struct
import subprocess
import tempfile

# repo root: tools/overlay/semcheck.py -> up 3. (The previous revision went up
# only 2, so ROOT was `tools/` and every path it built was wrong — the sweep
# found nothing and the single-function mode could not locate the object.)
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OBJCOPY = "mipsel-linux-gnu-objcopy"
OBJDUMP = "mipsel-linux-gnu-objdump"

REGS = ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
        't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
        's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
        't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra']

# PostProc steps that can REORDER or RE-SLOT instructions — the only ones that
# can introduce a structural defect. `--all` checks these by default.
REORDER_POSTPROC = re.compile(
    r'scheduler_normalizer|swap_instructions|unfill_branch_delay|'
    r'unfill_jal_delay|hoist_out_of_delay_slot|fill_delay_slots|'
    r'custom_script')

# Known-benign findings, carried so a sweep stays actionable. Entries are
# "OVERLAY/func_NAME" (whole function) or "OVERLAY/func_NAME:substring"
# (that one defect only). Keep this SHORT and justify every line — an
# allowlist is how a real defect gets ignored.
ALLOWLIST = {
    # TACO/func_8014C548 is a TRUE byte_perfect win (resolver: 217/217 words
    # exact, len t=868 c=868, 0 unresolved), so the lw/use adjacency it
    # reports is in KONAMI'S OWN CODE, not PostProc damage. Allowlisted for
    # the LOAD-USE finding only — any other defect on it still reports.
    'TACO/func_8014C548:LOAD-USE',
    # (--allowlist FILE adds more entries in the same syntax)
}

OP = lambda w: (w >> 26) & 0x3f          # noqa: E731
RS = lambda w: (w >> 21) & 0x1f          # noqa: E731
RT = lambda w: (w >> 16) & 0x1f          # noqa: E731
RD = lambda w: (w >> 11) & 0x1f          # noqa: E731
FUNCT = lambda w: w & 0x3f               # noqa: E731

BRANCH_OPS = {0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x14, 0x15, 0x16, 0x17}
STORE_OPS = {0x28, 0x29, 0x2b, 0x2a, 0x2e, 0x38, 0x39, 0x3b, 0x3a, 0x3e}
LOAD_OPS = {0x20, 0x21, 0x23, 0x24, 0x25, 0x22, 0x26, 0x30, 0x31, 0x33,
            0x35, 0x37}


def text_bytes(obj, func):
    """Return the .text bytes of func's window from an object file."""
    out = subprocess.run([OBJDUMP, '-t', str(obj)],
                         capture_output=True, text=True).stdout
    off = 0
    size = None
    for line in out.splitlines():
        p = line.split()
        if len(p) >= 6 and p[-1] == func and '.text' in line:
            off = int(p[0], 16)
            size = int(p[-2], 16)
            break
    with tempfile.NamedTemporaryFile(suffix='.bin', delete=False) as tf:
        tmp = tf.name
    subprocess.run([OBJCOPY, '-O', 'binary', '-j', '.text', str(obj), tmp],
                   capture_output=True)
    raw = open(tmp, 'rb').read()
    os.unlink(tmp)
    if size:
        return raw[off:off + size]
    return raw[off:]


def is_branch(w):
    o = OP(w)
    if o in BRANCH_OPS:
        return True
    if o == 0x01:                                   # REGIMM bltz/bgez/...
        return True
    if o == 0 and FUNCT(w) in (0x08, 0x09):         # jr, jalr
        return True
    if o in (0x11,) and RS(w) == 0x08:              # bc1t/bc1f
        return True
    return False


def is_jr_ra(w):
    return OP(w) == 0 and FUNCT(w) == 0x08 and RS(w) == 31


def is_jal(w):
    return OP(w) == 0x03 or (OP(w) == 0 and FUNCT(w) == 0x09)


def dest_reg(w):
    """Register written, or None."""
    o = OP(w)
    if w == 0:
        return None
    if o == 0:
        f = FUNCT(w)
        if f in (0x08,):                    # jr
            return None
        if f in (0x09,):                    # jalr
            return RD(w) or 31
        if f in (0x0c, 0x0d):               # syscall/break
            return None
        if f in (0x10, 0x12):               # mfhi mflo
            return RD(w)
        if f in (0x11, 0x13):               # mthi mtlo
            return None
        if f in (0x18, 0x19, 0x1a, 0x1b):   # mult/multu/div/divu
            return None
        return RD(w)
    if o == 0x01:
        return 31 if RT(w) in (0x10, 0x11) else None    # bltzal/bgezal
    if o == 0x03:
        return 31                                        # jal
    if o in (0x02,) or o in (0x04, 0x05, 0x06, 0x07, 0x14, 0x15, 0x16, 0x17):
        return None
    if o in STORE_OPS:
        return None
    if o in (0x0f,):                        # lui
        return RT(w)
    if o in (0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e):
        return RT(w)
    if o in LOAD_OPS:
        return RT(w)
    if o == 0x11:                           # cop1
        return RT(w) if RS(w) in (0x00, 0x01) else None
    if o == 0x12:                           # cop2 (GTE)
        return RT(w) if RS(w) in (0x00, 0x02) else None
    return None


def src_regs(w):
    o = OP(w)
    if w == 0:
        return set()
    if o == 0:
        f = FUNCT(w)
        if f in (0x00, 0x02, 0x03):         # sll srl sra (shamt)
            return {RT(w)}
        if f in (0x08, 0x09):               # jr / jalr
            return {RS(w)}
        if f in (0x10, 0x12):               # mfhi mflo
            return set()
        return {RS(w), RT(w)}
    if o in (0x0f,):                        # lui
        return set()
    if o in (0x02, 0x03):                   # j / jal
        return set()
    if o == 0x01:
        return {RS(w)}
    if o in (0x04, 0x05):
        return {RS(w), RT(w)}
    if o in (0x06, 0x07, 0x14, 0x15, 0x16, 0x17):
        return {RS(w)}
    if o in STORE_OPS:
        return {RS(w), RT(w)}
    if o in LOAD_OPS:
        return {RS(w)}
    return {RS(w)}


def imm(w):
    v = w & 0xffff
    return v - 0x10000 if v & 0x8000 else v


def analyze(words, tag='FINAL'):
    """Return a list of structural-defect strings for a word list."""
    d = []
    n = len(words)
    for i in range(n - 1):
        if is_branch(words[i]) and is_branch(words[i + 1]):
            d.append(f"{tag}: ILLEGAL branch-in-delay-slot at word {i+1} "
                     f"(0x{words[i]:08x} -> 0x{words[i+1]:08x})")
    for i in range(n - 1):
        if is_jal(words[i]) and dest_reg(words[i + 1]) == 31:
            d.append(f"{tag}: jal delay-slot CLOBBERS $ra at word {i+1}")
    for i in range(n):
        if is_jr_ra(words[i]):
            lw = None
            for j in range(i - 1, -1, -1):
                if OP(words[j]) == 0x23 and RT(words[j]) == 31:
                    lw = j
                    break
            if lw is None:
                # Only a defect if the function actually CALLS something before
                # this return (a spill with no call leaves $ra untouched).
                if (any(OP(w) == 0x2b and RT(w) == 31 for w in words)
                        and any(is_jal(w) for w in words[:i])):
                    d.append(f"{tag}: jr $ra at {i} but NO `lw $ra` restore "
                             f"(function spills $ra)")
                continue
            for k in list(range(lw + 1, i)) + [i + 1]:
                if k < n and dest_reg(words[k]) == 31:
                    d.append(f"{tag}: $ra CLOBBERED at word {k} between "
                             f"`lw $ra` ({lw}) and `jr $ra` ({i}) "
                             f"[0x{words[k]:08x}]")
    for i in range(n):
        w = words[i]
        if OP(w) == 0x09 and RS(w) == 29 and RT(w) == 29 and imm(w) > 0:
            for k in range(i + 1, n):
                w2 = words[k]
                if (OP(w2) in LOAD_OPS or OP(w2) in STORE_OPS) and RS(w2) == 29:
                    if imm(w2) < imm(w):
                        d.append(f"{tag}: frame access at word {k} "
                                 f"(off {imm(w2)}) AFTER sp dealloc "
                                 f"+{imm(w)} at {i}")
                    break
                if is_jr_ra(w2):
                    break
    for i in range(n - 1):
        if OP(words[i]) in LOAD_OPS:
            dr = dest_reg(words[i])
            if (dr and dr != 0 and dr in src_regs(words[i + 1])
                    and not is_branch(words[i])):
                d.append(f"{tag}: LOAD-USE hazard word {i}->{i+1} (${REGS[dr]})")
    return d


def words_from_bytes(tb):
    """Words of a .text blob, trailing assembler alignment zeros dropped."""
    while len(tb) >= 4 and tb[-4:] == b'\x00\x00\x00\x00':
        tb = tb[:-4]
    return list(struct.unpack('<%dI' % (len(tb) // 4), tb))


def final_obj_words(overlay, func, keep):
    kd = os.path.join(ROOT, keep, overlay, func)
    o = os.path.join(kd, f'{func}.o')
    if not os.path.exists(o):
        return None
    return words_from_bytes(text_bytes(o, func))


def load_allowlist(path):
    allow = set(ALLOWLIST)
    if path and os.path.exists(path):
        for line in open(path):
            line = line.split('#')[0].strip()
            if line:
                allow.add(line)
    return allow


def filter_allowed(overlay, func, defects, allow):
    key = f'{overlay}/{func}'
    if key in allow:
        return []
    out = []
    for d in defects:
        if not any(a.startswith(key + ':') and a.split(':', 1)[1] in d
                   for a in allow):
            out.append(d)
    return out


def check_one(overlay, func, keep, run=True, allow=frozenset()):
    """Compile through the real pipeline (unless run=False) and analyse."""
    if run:
        subprocess.run([sys.executable,
                        os.path.join(ROOT, 'tools/overlay/test_overlay_match.py'),
                        overlay, func, '--keep', os.path.join(ROOT, keep),
                        '--no-resolve'],
                       capture_output=True, cwd=ROOT, timeout=300)
    w = final_obj_words(overlay, func, keep)
    if w is None:
        return None
    return filter_allowed(overlay, func, analyze(w, 'FINAL'), allow)


def sweep_candidates(overlay=None, every=False, status=None):
    """(overlay, func) pairs to sweep: reorder-class PostProc unless --every.

    `status` (e.g. 'byte_perfect') restricts to functions with that status in
    config/overlay_match_status.json — the control set for validating the
    checker itself (a defect on a byte_perfect function is either real damage
    or a checker false positive; nothing else).
    """
    pat = os.path.join(ROOT, 'src', 'overlay',
                       overlay if overlay else '*', '*.c')
    keep_status = None
    if status:
        import json
        with open(os.path.join(ROOT, 'config',
                               'overlay_match_status.json')) as f:
            js = json.load(f)
        keep_status = {(ov, fn) for ov, fns in js.items()
                       for fn, v in fns.items()
                       if isinstance(v, dict) and v.get('status') == status}
    out = []
    for p in sorted(glob.glob(pat)):
        ov = os.path.basename(os.path.dirname(p))
        fn = os.path.basename(p)[:-2]
        try:
            head = open(p, errors='replace').read(20000)
        except OSError:
            continue
        if 'Status: ASM-ONLY' in head or '@AsmOnly' in head:
            continue
        if keep_status is not None and (ov, fn) not in keep_status:
            continue
        if every or REORDER_POSTPROC.search(head):
            out.append((ov, fn))
    return out


def main():
    import argparse
    from concurrent.futures import ThreadPoolExecutor, as_completed
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('overlay', nargs='?')
    ap.add_argument('func', nargs='?')
    ap.add_argument('--keep', default='wip-scratch/keep')
    ap.add_argument('--no-run', action='store_true',
                    help='analyse the kept object without recompiling')
    ap.add_argument('--all', action='store_true', help='tree sweep')
    ap.add_argument('--every', action='store_true',
                    help='with --all: check every function, not just the '
                         'reorder-class PostProc ones')
    ap.add_argument('--jobs', type=int, default=4)
    ap.add_argument('--status', default=None,
                    help="with --all: only functions with this "
                         "overlay_match_status.json status (e.g. byte_perfect)")
    ap.add_argument('--allowlist', default=None,
                    help='file of "OVERLAY/func[:substring]" known-benign lines')
    ap.add_argument('--object', help='analyse this object file directly')
    a = ap.parse_args()
    allow = load_allowlist(a.allowlist)

    if a.object:
        if not a.func:
            ap.error('--object needs --func-style positional FUNC name')
        w = words_from_bytes(text_bytes(a.object, a.func))
        d = analyze(w, 'FINAL')
        print('\n'.join('  ' + x for x in d) if d else f'{a.object}: clean')
        return 1 if d else 0

    if a.all:
        cands = sweep_candidates(a.overlay, a.every, a.status)
        print(f"semcheck sweep: {len(cands)} function(s)"
              f"{'' if a.every else ' carrying a reorder-class PostProc'}")
        bad = 0
        done = 0
        with ThreadPoolExecutor(max_workers=a.jobs) as ex:
            futs = {ex.submit(check_one, ov, fn, a.keep, not a.no_run, allow):
                    (ov, fn) for ov, fn in cands}
            for f in as_completed(futs):
                ov, fn = futs[f]
                done += 1
                try:
                    d = f.result()
                except Exception as e:                      # noqa: BLE001
                    print(f"{ov}/{fn}: ERROR {e}")
                    continue
                if d:
                    bad += 1
                    print(f"{ov}/{fn}:")
                    for x in d:
                        print("  " + x)
                if done % 100 == 0:
                    print(f"  ... {done}/{len(cands)} ({bad} with defects)")
        print(f"\nsemcheck: {bad}/{len(cands)} function(s) show structural "
              f"damage in the FINAL object")
        return 1 if bad else 0

    if not (a.overlay and a.func):
        ap.error('give OVERLAY FUNC, or --all, or --object')
    d = check_one(a.overlay, a.func, a.keep, not a.no_run, allow)
    if d is None:
        print(f"{a.overlay}/{a.func}: NO OBJECT")
        return 2
    if d:
        print(f"{a.overlay}/{a.func}:")
        for x in d:
            print("  " + x)
        return 1
    print(f"{a.overlay}/{a.func}: clean")
    return 0


if __name__ == '__main__':
    sys.exit(main())
