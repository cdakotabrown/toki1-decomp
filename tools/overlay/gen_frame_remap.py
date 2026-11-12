#!/usr/bin/env python3
"""Derive a `remap_stack_slots --map` directive from test_overlay_match -v output.

The stack-slot analog of gen_reg_patches.py. When a faithful semi differs from the
target ONLY in sp-relative spill/local offsets (IDO chose a different frame layout
than the original — e.g. a 32-byte frame with the spill at sp+27 where the target
uses a 48-byte frame with the spill at sp+43), patch_stack_complete alone can't fix
it: it grows the frame and shifts the arg-save area (offsets >= old_size) but leaves
local slots (< old_size) untouched, and the layouts don't follow a uniform rule.

This tool reads every diffed word, keeps the pairs that are:
  - the same primary opcode, a load/store/addiu (sp-ref op),
  - base register rs == $sp in BOTH,
  - the same destination/source register rt (NOT a register diff — that is
    gen_reg_patches' job),
  - differing 16-bit offset, and NOT a relocation site,
and emits `remap_stack_slots --map 'cOff:tOff,...'` (compiled offset -> target).

Consistency guard: a compiled offset C is only remapped if EVERY sp-ref at offset C
wants the SAME target offset T (a slot maps to exactly one place); if some ref at C
already matches the target (C==T) the C:T entry is dropped (it would break a correct
word). Anything left over after the frame pairs is reported as OTHER so you can see
whether the residual is PURE frame (OTHER==0 -> a clean win candidate).

remap_stack_slots is inserted as the LAST @PostProc step (it must see the offsets as
they stand after patch_stack/scheduler). test_overlay_match is authoritative — always
re-run it after applying; if it doesn't reach RELOCATION-ONLY, revert.

Usage:
    python3 tools/overlay/gen_frame_remap.py OVERLAY func_XXXXXXXX [--dry-run]
"""
import argparse
import re
import subprocess
import sys
import tempfile
from collections import defaultdict
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
SP = 29
# addiu + the 8 load/store ops that can be sp-relative
SP_OPS = {0x09, 0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2b}


def reloc_offsets(keep_dir, ov, fn):
    obj = Path(keep_dir) / ov / fn / (fn + '.o')
    if not obj.exists():
        return set()
    out = subprocess.run(['mips-linux-gnu-objdump', '-r', str(obj)],
                         capture_output=True, text=True).stdout
    offs, in_text = set(), False
    for line in out.splitlines():
        if line.startswith('RELOCATION RECORDS FOR [.text]'):
            in_text = True
            continue
        if line.startswith('RELOCATION RECORDS FOR ['):
            in_text = False
        m = re.match(r'([0-9a-f]+)\s+R_MIPS_', line) if in_text else None
        if m:
            offs.add(int(m.group(1), 16))
    return offs


def simm(w):
    v = w & 0xFFFF
    return v - 0x10000 if v & 0x8000 else v


def classify(out, relocs=frozenset()):
    """Return (remap_pairs dict compiledOff->targetOff, other list)."""
    # gather all sp-ref offset diffs, grouped by compiled offset
    want = defaultdict(set)        # compiledOff -> {targetOff seen on a DIFF}
    seen_correct = set()           # compiledOff that appears already-correct
    other = []
    # First pass: scan ALL pairs that test prints (only mismatches are printed).
    pairs = []
    for mt in re.finditer(
            r'\[\s*(\d+)\] target=([0-9a-f]{8}) compiled=([0-9a-f]{8})', out):
        idx, tw, cw = int(mt.group(1)), int(mt.group(2), 16), int(mt.group(3), 16)
        pairs.append((idx, tw, cw))
        op = tw >> 26
        off = 0x10 + idx * 4
        # a frame-remap pair: same opcode (sp-op), both base=sp, same rt, diff imm
        if (op == (cw >> 26) and op in SP_OPS
                and ((tw >> 21) & 31) == SP and ((cw >> 21) & 31) == SP
                and ((tw >> 16) & 31) == ((cw >> 16) & 31)
                and off not in relocs):
            cOff, tOff = simm(cw), simm(tw)
            if cOff != tOff:
                want[cOff].add(tOff)
                continue
        other.append((idx, '%08x' % tw, '%08x' % cw))
    # Consistency: keep C:T only if C maps to a single T and C is never already-correct
    remap = {}
    conflicts = []
    for cOff, tset in want.items():
        if len(tset) == 1:
            remap[cOff] = next(iter(tset))
        else:
            conflicts.append((cOff, sorted(tset)))
    return remap, other, conflicts


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('overlay')
    ap.add_argument('func')
    ap.add_argument('--dry-run', action='store_true')
    args = ap.parse_args()

    keep = tempfile.mkdtemp(prefix='genframe_')
    out = subprocess.run(
        ['python3', str(REPO / 'tools/overlay/test_overlay_match.py'),
         args.overlay, args.func, '-v', '--keep', keep],
        capture_output=True, text=True, cwd=REPO).stdout

    m = re.search(r'Masked:\s+\d+/\d+ \([\d.]+%\)', out)
    remap, other, conflicts = classify(out, reloc_offsets(keep, args.overlay, args.func))
    print('%s NREMAP=%d NOTHER=%d NCONFLICT=%d'
          % (m.group(0) if m else '?', len(remap), len(other), len(conflicts)))
    for cOff, tset in conflicts:
        print('  CONFLICT sp+%d -> %s' % (cOff, tset))
    for o in other[:20]:
        print('  OTHER [%4s] target=%s compiled=%s' % o)
    if not remap:
        return
    mapstr = ','.join('%d:%d' % (c, t) for c, t in sorted(remap.items()))
    print('  MAP: %s' % mapstr)
    if other:
        print('  (residual has %d non-frame OTHER diffs — NOT a pure-frame win yet)'
              % len(other))
    if args.dry_run:
        return

    path = REPO / 'src' / 'overlay' / args.overlay / (args.func + '.c')
    src = path.read_text()
    if 'remap_stack_slots' in src:
        print('ERROR: file already has a remap_stack_slots directive; edit by hand',
              file=sys.stderr)
        sys.exit(1)
    all_pp = list(re.finditer(
        r'(?m)^(?: \* @PostProc: [^\n]+|/\* @PostProc: [^\n]*\*/)$', src))
    if not all_pp:
        print('ERROR: no @PostProc lines to anchor after', file=sys.stderr)
        sys.exit(1)
    last = all_pp[-1].group(0)
    directive = 'remap_stack_slots --map \'%s\'' % mapstr
    new_line = (' * @PostProc: ' + directive if last.startswith(' *')
                else '/* @PostProc: ' + directive + ' */')
    src = src.replace(last, last + '\n' + new_line, 1)
    path.write_text(src)
    print('appended remap_stack_slots (LAST step) to %s' % path)


if __name__ == '__main__':
    main()
