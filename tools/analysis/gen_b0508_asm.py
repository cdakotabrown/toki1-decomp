#!/usr/bin/env python3
"""Regenerate asm/main/matched/*.s for the 0x800B0508-0x800B1650 library window.

The window used to be one 4,424-byte blob registered as `func_800B0508`.  Its
true composition is 24 separate functions, established three independent ways:

  * whole-member alignment of three Psy-Q LIBGTE archive members against the
    retail image (0 masked mismatches each) -- see
    tools/analysis/match_psyq_lib.py;
  * `jr ra` + delay-slot walk over the retail disassembly (every Sony boundary
    coincides with one);
  * `jal` call sites in the retail image landing exactly on those addresses.

Every emitted .s is verified to assemble back to the retail bytes.  ASM files
are gitignored (they are game code); this script is the tracked recipe.

    python3 tools/analysis/gen_b0508_asm.py [--check]
"""
import argparse
import os
import re
import struct
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
IMG_PATH = os.path.join(ROOT, 'disks', 'SLPS_000.65')
OUT = os.path.join(ROOT, 'asm', 'main', 'matched')
BASE = 0x80041000
FOFF = 0x800
TMP = os.environ.get('TMPDIR', '/tmp')

REGS = ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3', 't0', 't1', 't2',
        't3', 't4', 't5', 't6', 't7', 't8', 't9', 's0', 's1', 's2', 's3',
        's4', 's5', 's6', 's7', 'k0', 'k1', 'gp', 'sp', 'fp', 's8', 'ra']
RE_REG = re.compile(r'\b(%s)\b' % '|'.join(REGS))

V = ('LIBGTE member TMD (Psy-Q 2.5), byte-verified: the whole member aligns at '
     '0x800AFD40 -- 1068 words, 972 fully compared, 0 masked mismatches.')
S = ('LIBGTE member SMP_00 (Psy-Q 4.5), byte-verified: the whole member aligns '
     'at 0x800B0E90 -- 132 words, 132 fully compared (the member has no '
     'relocations at all), 0 mismatches.')
D = ('LIBGTE member DIVF3A (Psy-Q 2.5), byte-verified: the whole member aligns '
     'at 0x800B12E0 -- 220 words, 212 fully compared, 0 masked mismatches.')

COMMON = ['BLOCKER: asm_only -- hand-written Sony GTE assembly (COP2 / LWC2 /',
          'SWC2 / MTC2 / CTC2).  No C compiler emits COP2 instructions, and the',
          'archive member records no source path (Sony shipped these as .s).',
          'The terminal state is asm_only, not semi_matched.']

FUNCS = [
    (0x800B0508, 356, 'gte_texture_gouraud3', V, None),
    (0x800B066C, 244, 'gte_flat_fog3', V, None),
    (0x800B0760, 288, 'gte_texture_flat_fog3', V, None),
    (0x800B0880, 320, 'gte_gouraud_fog3', V, None),
    (0x800B09C0, 356, 'gte_texture_gouraud_fog3', V, None),
    (0x800B0B24, 372, 'gte_scope_tf4_B', V, None),
    (0x800B0C98, 344, 'gte_scope_tf4', V, None),
    (0x800B0DF0, 160, 'exception_handler_install',
     'UNIDENTIFIED: no shipped 2.5/4.5 archive member matches (best probe 47.5%).',
     ['Shape: a BIOS exception-handler installer.  +0x00 stashes $ra in a global',
      'with lui/sw (no stack frame at all), +0x08 jal 0x800B5730, +0x10 calls the',
      'BIOS through `li $t2,0xB0; jalr $t2; li $t1,0x56` (B(56h) = GetC0Table),',
      'reads table entry 6 (ExceptionHandler) and copies the 56-byte template at',
      '0x800B0E58..0x800B0E90 over it, then jal 0x800B5740 / 0x800B5750 and',
      'restores $ra.',
      '',
      'NOTE: bytes +0x68..+0xA0 of this 160-byte window are that TEMPLATE.  It is',
      'data, not a function: 0 jal sites and 0 word references reach 0x800B0E58,',
      'it is only ever read by the copy loop above, and it uses $k0/$k1 and mfc0.',
      'It is kept inside this window rather than split off for that reason.',
      '',
      'BLOCKER: asm_only -- $k0/$k1, mfc0, a BIOS jalr with li in the delay slot,',
      'and an embedded relocatable code template are all unreachable from C.']),
    (0x800B0E90, 40, 'LightColor', S, None),
    (0x800B0EB8, 40, 'DpqColorLight', S, None),
    (0x800B0EE0, 60, 'DpqColor3', S, None),
    (0x800B0F1C, 36, 'Intpl', S, None),
    (0x800B0F40, 40, 'Square12', S, None),
    (0x800B0F68, 40, 'Square0', S, None),
    (0x800B0F90, 32, 'AverageZ3', S, None),
    (0x800B0FB0, 36, 'AverageZ4', S, None),
    (0x800B0FD4, 88, 'OuterProduct12', S, None),
    (0x800B102C, 88, 'OuterProduct0', S, None),
    (0x800B1084, 28, 'Lzc', S, None),
    (0x800B10A0, 140, 'InvSquareRoot',
     'LIBGTE member MSC (Psy-Q 2.5), symbol InvSquareRoot: 35 words, 33 fully '
     'compared, 0 masked mismatches.  This is a SINGLE-SYMBOL alignment -- the '
     'surrounding member does not align -- so it identifies the name, it is not '
     'a member-level proof.  The boundary itself is independent of the name.',
     ['BLOCKER: asm_only -- MTC2 $30 / MFC2 $31 (GTE LZCS/LZCR count-leading-',
      'zeros) plus a %hi/%lo reciprocal-square-root table lookup at 0x800E4A74.']),
    (0x800B112C, 200, 'vector_normalize',
     'UNIDENTIFIED by byte match.  HYPOTHESIS, not proven: LIBGTE VectorNormal '
     'from an SDK version we do not ship.  2.5 LIBGTE MSC VectorNormal has the '
     'identical 200-byte size but only 18 of 50 words agree, so the '
     'implementation differs.  Do not treat the name as established.',
     ['Shape: long VECTOR in (lw +0/+4/+8 from $a0), long VECTOR out (sw to $a1);',
      'sum of squares -> MTC2 $30 / MFC2 $31 -> rsqrt table at 0x800E4A74 -> srav.',
      'Called from 0x800A456C, 0x800A45A0 and 0x800A45E0.',
      'BLOCKER: asm_only -- the MTC2/MFC2 GTE LZCS/LZCR idiom has no C form.']),
    (0x800B11F4, 236, 'vector_normalize_short',
     'UNIDENTIFIED by byte match.  HYPOTHESIS, not proven: LIBGTE VectorNormalS, '
     'with VectorNormalSS as the alternate entry at 0x800B1208.  Supported by '
     'size, adjacency and semantics only -- the shipped 2.5/4.5 bytes differ.',
     ['Shape: two entries into one body.  +0x00 loads a long VECTOR (lw) from $a0;',
      '+0x14 (0x800B1208) loads a short SVECTOR (lh) from $a0; both fall into the',
      'common tail at +0x24, which stores an SVECTOR (sh) to $a1.  Same LZCS/LZCR',
      '+ rsqrt-table idiom as vector_normalize.',
      'BLOCKER: asm_only -- alt-entry fragment (the second entry has no prologue',
      'and falls through) plus the MTC2/MFC2 GTE idiom.']),
    (0x800B12E0, 8, 'RCpolyF3', D,
     ['BLOCKER: asm_only -- alt-entry fragment.  Two instructions, no jr ra; it',
      'falls through into RCpolyF3A at 0x800B12E8.']),
    (0x800B12E8, 872, 'RCpolyF3A', D, None),
]


def load():
    with open(IMG_PATH, 'rb') as fh:
        return fh.read()


def disasm(img, addr, size):
    p = os.path.join(TMP, 'b0508_d.bin')
    o = (addr - BASE) + FOFF
    with open(p, 'wb') as fh:
        fh.write(img[o:o + size])
    out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-b', 'binary',
                          '-m', 'mips:3000', '-EL',
                          '--adjust-vma=0x%X' % addr, p],
                         capture_output=True, text=True).stdout
    rows = {}
    for line in out.splitlines():
        m = re.match(r'^\s*([0-9a-f]{8}):\s+([0-9a-f]{8})\s+(.*)$', line)
        if m:
            rows[int(m.group(1), 16)] = (int(m.group(2), 16), m.group(3).strip())
    return rows


def build(img, addr, size, name, header):
    o = (addr - BASE) + FOFF
    ws = list(struct.unpack('<%dI' % (size // 4), img[o:o + size]))
    rows = disasm(img, addr, size)
    labels = {}
    for i, w in enumerate(ws):
        a = addr + 4 * i
        txt = rows.get(a, (w, ''))[1]
        m = re.search(r'0x([0-9a-f]+)$', txt)
        op = w >> 26
        if (op in (1, 4, 5, 6, 7, 0x14, 0x15, 0x16, 0x17)) and m:
            t = int(m.group(1), 16)
            if addr <= t < addr + size:
                labels[t] = '.L%08X' % t
    alias = 'func_%08X' % addr
    lines = ['.set noreorder', '.set noat', '', '.text', '.globl %s' % name]
    if alias != name:
        # Address-derived alias: tooling (and callers that only know the
        # address) look the function up as func_XXXXXXXX.  A second global
        # label at the same address costs no bytes.
        lines.append('.globl %s' % alias)
    lines += ['', '/*']
    lines += [(' * ' + h).rstrip() for h in header]
    lines += [' */']
    if alias != name:
        lines.append('%s:' % alias)
    lines.append('%s:' % name)
    for i, w in enumerate(ws):
        a = addr + 4 * i
        if a in labels:
            lines.append('%s:' % labels[a])
        txt = rows.get(a, (w, '.word 0x%08X' % w))[1]
        txt = re.sub(r'\s*#.*$', '', txt)
        txt = re.sub(r'\s*<[^>]*>\s*$', '', txt)
        mv = re.match(r'^move\s+(\w+),(\w+)$', txt)
        if mv and (w >> 26) == 0:
            fn = w & 0x3F
            mn = 'addu' if fn == 0x21 else ('or' if fn == 0x25 else None)
            if mn:
                txt = '%s\t%s,%s,zero' % (mn, mv.group(1), mv.group(2))
        if (w >> 26) == 0x10:
            txt = '.word 0x%08X   /* %s */' % (w, txt)
        m = re.search(r'0x([0-9a-f]+)$', txt)
        if m:
            t = int(m.group(1), 16)
            if t in labels:
                txt = txt[:m.start()] + labels[t]
        lines.append('    ' + RE_REG.sub(lambda mm: '$' + mm.group(1), txt))
    return '\n'.join(lines) + '\n'


def verify(img, path, addr, size):
    obj = os.path.join(TMP, 'b0508_v.o')
    binp = os.path.join(TMP, 'b0508_v.bin')
    r = subprocess.run(['mipsel-linux-gnu-as', '-march=mips1', '-o', obj, path],
                       capture_output=True, text=True)
    if r.returncode != 0:
        return (False, r.stderr.strip()[:300])
    subprocess.run(['mipsel-linux-gnu-objcopy', '-O', 'binary',
                    '--only-section=.text', obj, binp], capture_output=True)
    with open(binp, 'rb') as fh:
        got = fh.read()
    o = (addr - BASE) + FOFF
    want = img[o:o + size]
    if len(got) < size or got[size:].strip(b'\0'):
        return (False, 'length %d vs %d' % (len(got), size))
    got = got[:size]
    if got != want:
        bad = [i for i in range(0, size, 4) if got[i:i + 4] != want[i:i + 4]]
        return (False, '%d word diffs, first +0x%X' % (len(bad), bad[0]))
    return (True, 'OK %d bytes' % size)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--check', action='store_true',
                    help='verify only, do not write')
    a = ap.parse_args()
    print('ROOT = %s' % ROOT)
    img = load()
    if not a.check:
        os.makedirs(OUT, exist_ok=True)
    total = 0
    bad = 0
    for addr, size, name, prov, extra in FUNCS:
        hdr = [name,
               'Address: 0x%08X' % addr,
               'Size: %d bytes (%d instructions)' % (size, size // 4),
               'Region: Library (0x80086000-0x800BFFFF)',
               'Status: asm_only (byte-perfect ASM, no C)',
               '',
               'Provenance: ' + prov,
               '']
        hdr += (extra if extra else COMMON)
        txt = build(img, addr, size, name, hdr)
        path = os.path.join(OUT, name + '.s')
        if a.check and not os.path.exists(path):
            print('%-26s MISSING' % name)
            bad += 1
            continue
        if not a.check:
            with open(path, 'w') as fh:
                fh.write(txt)
        ok, msg = verify(img, path, addr, size)
        print('%-26s 0x%08X %5d  %s' % (name, addr, size, msg))
        if not ok:
            bad += 1
        total += size
    print('total %d bytes (window 0x800B0508..0x800B1650 = 4424), failures %d'
          % (total, bad))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
