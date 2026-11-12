#!/usr/bin/env python3
"""Regenerate asm/main/matched/*.s for the BIOS-trampoline table at
0x800B4830-0x800B48D0 and 0x800B5650-0x800B5A60.

WHAT THIS WINDOW IS
-------------------
Two runs of 16-byte entries.  47 of the 51 have the identical shape

    li   $t2, 0xA0 / 0xB0 / 0xC0      ; the PSX kernel jump-vector
    jr   $t2                          ; ... entered with the call number
    li   $t1, N                       ; ... in $t1, from the DELAY SLOT
    nop                               ; alignment to 16 bytes

which is the standard Sony LIBAPI trampoline into the BIOS A/B/C tables.
The other four are `syscall` wrappers and $sp/$gp readers of the same
16-byte pitch.

WHY asm_only
------------
`jr $t2` with the argument set up in the branch-delay slot, a bare
`syscall`, and reads/writes of $sp and $gp are all outside C89 (this
project bans GCC inline-asm extensions, and IDO 5.2 has none).  There is
no C body -- of any quality -- that emits these four words.  The
functions are terminal as asm_only, filed under blocker category
`bios_trampoline` in config/c_unmatchable.txt.

NAMES ARE NOT CLAIMED
---------------------
Symbols stay func_<ADDR>.  The (table, index) pair IS established -- it is
literally the two immediates -- and the PSX-SPX name for that index is
recorded in the header as documentation.  That is not the same as proving
which Sony LIBAPI symbol Konami linked, so no name is asserted.

Every emitted .s is assembled and compared against the retail bytes.
asm/main/matched/*.s is gitignored; this script is the tracked recipe.

    python3 tools/analysis/gen_bios_thunk_asm.py [--check] [--only NAME]
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

# PSX-SPX kernel jump-table names, keyed (vector, index).  Documentation only:
# the symbol emitted is always func_<ADDR>.
A = {0x0E: 'abs', 0x13: 'setjmp', 0x15: 'strcat', 0x16: 'strncat',
     0x17: 'strcmp', 0x18: 'strncmp', 0x19: 'strcpy', 0x1A: 'strncpy',
     0x1B: 'strlen', 0x27: 'bcopy', 0x28: 'bzero', 0x2A: 'memcpy',
     0x2C: 'memmove', 0x2E: 'memchr', 0x2F: 'rand', 0x30: 'srand',
     0x39: 'InitHeap', 0x3F: 'printf', 0x44: 'FlushCache', 0x49: 'GPU_cw',
     0x51: 'LoadExec', 0x70: '_bu_init', 0x71: '_96_init',
     0x72: '_96_remove', 0xAB: '_card_info', 0xAC: '_card_async_load_directory',
     0xAD: '_card_auto'}
B = {0x07: 'DeliverEvent', 0x08: 'OpenEvent', 0x09: 'CloseEvent',
     0x0A: 'WaitEvent', 0x0B: 'TestEvent', 0x0C: 'EnableEvent',
     0x0D: 'DisableEvent', 0x12: 'InitPad', 0x13: 'StartPad',
     0x14: 'StopPad', 0x15: 'OutdatedPadInitAndStart',
     0x16: 'OutdatedPadGetButtons', 0x17: 'ReturnFromException',
     0x18: 'SetDefaultExitFromException', 0x19: 'SetCustomExitFromException',
     0x32: 'FileOpen', 0x34: 'FileRead', 0x35: 'FileWrite',
     0x36: 'FileClose', 0x38: 'exit', 0x3F: 'puts', 0x41: 'format',
     0x45: 'erase', 0x4A: 'InitCard', 0x4B: 'StartCard',
     0x4E: '_card_write', 0x50: '_new_card',
     0x5B: 'ChangeClearPad', 0x5C: '_card_status'}
C = {0x0A: 'ChangeClearRCnt'}
TABLES = {0xA0: ('A', A), 0xB0: ('B', B), 0xC0: ('C', C)}

# The four non-trampoline entries of the same 16-byte pitch.  Each is its own
# hard C blocker; the text is the header's BLOCKER paragraph.
SPECIAL = {
    0x800B56D0: ('Reads $sp into $v0 and installs $a0 as the new $sp, from the '
                 'branch-delay slot of the `jr $ra` (so the switch happens on '
                 'the way out).  C89 can neither read nor assign the stack '
                 'pointer.'),
    0x800B56F0: ('Returns $gp.  C89 cannot name the global pointer.'),
    0x800B58F0: ('Returns $sp.  C89 cannot name the stack pointer.'),
    0x800B5730: ('`li $a0,1; syscall` -- kernel syscall 1 '
                 '(EnterCriticalSection).  C89 has no syscall.'),
    0x800B5750: ('`li $a0,2; syscall` -- kernel syscall 2 '
                 '(ExitCriticalSection).  C89 has no syscall.'),
}

# One entry per 16-byte block, in address order.  The blocks are derived from
# the image, not hand-typed: this list only records WHICH blocks belong to this
# sweep (i.e. the semi_stub windows this tool owns), so that the
# already-terminal neighbours (0x800B4870, 0x800B5780..0x800B58BC) are left
# alone.
WINDOWS = [
    (0x800B4830, 48), (0x800B4860, 16), (0x800B4880, 16), (0x800B4890, 16),
    (0x800B48A0, 48), (0x800B5650, 128), (0x800B56D0, 32), (0x800B56F0, 64),
    (0x800B5730, 32), (0x800B5750, 48), (0x800B58F0, 32), (0x800B5910, 16),
    (0x800B5920, 32), (0x800B5940, 240), (0x800B5A30, 16), (0x800B5A40, 16),
    (0x800B5A50, 16),
    # 2026-10-03: the two LIBAPI runs that were never entered anywhere (found by
    # tools/analysis/audit_main_coverage.py).  Each 16-byte block is a whole
    # Sony archive member, exact in every SDK on disk (2.5/3.3/3.5/3.6/4.0/4.5)
    # and carrying no ccpsx source record: A74 InitCARD, A75 StartCARD,
    # C112 _bu_init, C173 _card_auto, A92 _card_status, C171 _card_info,
    # C172 _card_load (0x800B4760-0x800B47D0); A80 _new_card, A78 _card_write,
    # C40 bzero (0x800B4800-0x800B4830).  The note above used to list
    # 0x800B4780..0x800B4820 as "already-terminal neighbours"; that was wrong,
    # no listing and no registry entry covered them.
    (0x800B4760, 112), (0x800B4800, 48),
]

REGS = ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3', 't0', 't1', 't2',
        't3', 't4', 't5', 't6', 't7', 't8', 't9', 's0', 's1', 's2', 's3',
        's4', 's5', 's6', 's7', 'k0', 'k1', 'gp', 'sp', 'fp', 's8', 'ra']
RE_REG = re.compile(r'\b(%s)\b' % '|'.join(REGS))


def load():
    with open(IMG_PATH, 'rb') as fh:
        return fh.read()


def blocks():
    """Every 16-byte entry owned by this sweep, as (addr,)."""
    out = []
    for a, n in WINDOWS:
        for off in range(0, n, 16):
            out.append(a + off)
    return out


def disasm(img, addr, size):
    p = os.path.join(TMP, 'bios_thunk_d.bin')
    o = (addr - BASE) + FOFF
    with open(p, 'wb') as fh:
        fh.write(img[o:o + size])
    out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-z', '-b', 'binary',
                          '-m', 'mips:3000', '-EL',
                          '--adjust-vma=0x%X' % addr, p],
                         capture_output=True, text=True).stdout
    rows = {}
    for line in out.splitlines():
        m = re.match(r'^\s*([0-9a-f]{8}):\s+([0-9a-f]{8})\s+(.*)$', line)
        if m:
            rows[int(m.group(1), 16)] = (int(m.group(2), 16),
                                         re.sub(r'\s+', ' ', m.group(3).strip()))
    return rows


def classify(ws):
    """Return (vector, index) for a BIOS trampoline, else None."""
    if (ws[0] & 0xFFFF0000) == 0x240A0000 and ws[1] == 0x01400008 \
            and (ws[2] & 0xFFFF0000) == 0x24090000 and ws[3] == 0:
        return (ws[0] & 0xFFFF, ws[2] & 0xFFFF)
    return None


def header(addr, ws):
    name = 'func_%08X' % addr
    h = [name,
         'Address: 0x%08X' % addr,
         'Size: 16 bytes (4 instructions)',
         'Region: Library (0x80086000-0x800BFFFF)',
         'Status: asm_only (byte-perfect ASM, no C)',
         '']
    t = classify(ws)
    if t:
        vec, idx = t
        tn, tbl = TABLES.get(vec, ('?', {}))
        nm = tbl.get(idx)
        h += ['Shape: Sony LIBAPI kernel trampoline.',
              '',
              '    li   $t2, 0x%02X       ; PSX BIOS %s-table jump vector' % (vec, tn),
              '    jr   $t2',
              '    li   $t1, 0x%02X       ; DELAY SLOT: call number' % idx,
              '    nop                 ; pad to the table pitch of 16 bytes',
              '',
              'The (table, index) pair is not inferred -- it is the two',
              'immediates.  PSX-SPX documents %s(0x%02X) as `%s`.'
              % (tn, idx, nm if nm else '(index not in the PSX-SPX list)'),
              'That is documentation.  The SYMBOL stays func_<ADDR>: knowing which',
              'kernel entry the thunk targets does not establish which Sony LIBAPI',
              'symbol Konami linked, and this project retracted 23 names that were',
              'guessed that way.',
              '',
              'BLOCKER: bios_trampoline -- asm_only.  `jr $t2` with the call number',
              'established in the branch-delay slot has no C89 form: C cannot jump',
              'to a computed kernel vector, cannot pass an argument in $t1, and',
              'cannot place an instruction in a delay slot.  GCC inline asm is',
              'banned by this project (C89 only, no extensions), and IDO 5.2 has no',
              'asm statement at all.  Filed in config/c_unmatchable.txt.']
    else:
        h += ['Shape: 16-byte entry in the same LIBAPI trampoline table, but not',
              'a kernel-vector thunk.',
              '',
              'BLOCKER: bios_trampoline -- asm_only.  ' + SPECIAL[addr],
              'C89 (no GCC inline-asm extensions, and IDO 5.2 has no asm',
              'statement) cannot emit any of it.  Filed in',
              'config/c_unmatchable.txt.']
    h += ['',
          'Generated by tools/analysis/gen_bios_thunk_asm.py from',
          'disks/SLPS_000.65 and verified by reassembly against the retail',
          'bytes.  asm/main/matched/*.s is gitignored; the generator is the',
          'tracked artefact.']
    return h


def build(img, addr):
    o = (addr - BASE) + FOFF
    ws = list(struct.unpack('<4I', img[o:o + 16]))
    rows = disasm(img, addr, 16)
    name = 'func_%08X' % addr
    lines = ['.set noreorder', '.set noat', '', '.text', '.globl %s' % name,
             '', '/*']
    lines += [(' * ' + x).rstrip() for x in header(addr, ws)]
    lines += [' */', '%s:' % name]
    for i, w in enumerate(ws):
        a = addr + 4 * i
        txt = rows.get(a, (w, '.word 0x%08X' % w))[1]
        txt = re.sub(r'\s*#.*$', '', txt)
        txt = re.sub(r'\s*<[^>]*>\s*$', '', txt)
        # objdump renders BOTH `or rd,rs,zero` and `addu rd,rs,zero` as `move`,
        # and GNU as resolves the `move` macro to `or`.  Key off the funct field.
        mv = re.match(r'^move\s+(\w+),(\w+)$', txt)
        if mv and (w >> 26) == 0:
            fn = w & 0x3F
            mn = 'addu' if fn == 0x21 else ('or' if fn == 0x25 else None)
            if mn:
                txt = '%s\t%s,%s,zero' % (mn, mv.group(1), mv.group(2))
        lines.append('    ' + RE_REG.sub(lambda m: '$' + m.group(1), txt))
    return '\n'.join(lines) + '\n'


def verify(img, path, addr):
    obj = os.path.join(TMP, 'bios_thunk_v.o')
    binp = os.path.join(TMP, 'bios_thunk_v.bin')
    r = subprocess.run(['mipsel-linux-gnu-as', '-march=mips1', '-o', obj, path],
                       capture_output=True, text=True)
    if r.returncode != 0:
        return (False, r.stderr.strip()[:300])
    subprocess.run(['mipsel-linux-gnu-objcopy', '-O', 'binary',
                    '--only-section=.text', obj, binp], capture_output=True)
    with open(binp, 'rb') as fh:
        got = fh.read()
    o = (addr - BASE) + FOFF
    want = img[o:o + 16]
    if len(got) < 16 or got[16:].strip(b'\0'):
        return (False, 'length %d vs 16' % len(got))
    if got[:16] != want:
        return (False, 'word diff')
    return (True, 'OK 16 bytes')


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--check', action='store_true')
    ap.add_argument('--only')
    a = ap.parse_args()
    print('ROOT = %s' % ROOT)
    img = load()
    if not a.check:
        os.makedirs(OUT, exist_ok=True)
    bad = 0
    n = 0
    for addr in blocks():
        name = 'func_%08X' % addr
        if a.only and a.only != name:
            continue
        path = os.path.join(OUT, name + '.s')
        if a.check and not os.path.exists(path):
            print('%-20s MISSING' % name)
            bad += 1
            continue
        if not a.check:
            with open(path, 'w') as fh:
                fh.write(build(img, addr))
        ok, msg = verify(img, path, addr)
        print('%-20s 0x%08X  %s' % (name, addr, msg))
        if not ok:
            bad += 1
        n += 1
    print('%d entries, %d failures' % (n, bad))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
