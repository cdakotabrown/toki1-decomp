#!/usr/bin/env python3
"""emit_asm_only_window.py -- byte-exact asm_only listings for main-binary
windows filed as hand-written GTE assembly in config/c_unmatchable.txt that
are NOT in config/psyq_lib_symbols.json (no whole-member archive alignment was
recorded for them, so they must not be added to that identification table).

asm/main/matched/*.s is .gitignored; this tracked script plus a legal disc
image reproduces the listings:

    python3 tools/analysis/emit_asm_only_window.py [--force]

Every word is emitted as `.word 0x........` with the disassembly in a trailing
comment -- byte-exact by construction, and it needs no assembler pass that can
re-encode COP2.  The table below is the tracked source of truth; add a window
here in the same commit as its config/c_unmatchable.txt entry.
"""
import argparse
import os
import struct
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BIN = os.path.join(ROOT, 'disks', 'SLPS_000.65')
OUT = os.path.join(ROOT, 'asm', 'main', 'matched')
DIS = os.path.join(ROOT, 'tools', 'analysis', 'dis.sh')

# name, address, size, blocker text (mirrors config/c_unmatchable.txt)
WINDOWS = [
    ('RotTransPers', 0x800B1650, 688,
     'LIBGTE hand-written COP2 assembly: a window of ~17 tiny GTE wrappers '
     '(lwc2/swc2/cop2/cfc2/mfc2 bodies, `jr ra` each) -- the LIBGTE members '
     'Konami linked VERBATIM (docs/reference/PSYQ-LIB-IDENTIFICATION.md).  No C '
     'compiler emits COP2; terminal state asm_only.  Sibling filings: '
     'SetBackColor/5024/50D0/50DC.'),
    ('SetDQB', 0x800A50E8, 884,
     'LIBGTE hand-written COP2 assembly: a window of ~14 tiny GTE register '
     'loaders/storers (ctc2/swc2/mtc2 bodies, `jr ra` each) directly after the '
     'filed SetDQA.  No C compiler emits COP2; terminal state asm_only.'),
    ('uwasa0', 0x80060E20, 84,
     'dead_selector_copy_placement (game region, IDO 5.2): retail copies the '
     'switch selector into $v0 (`move v0,v1`) in the ENTRY block -- it fills '
     'the first beqz slot -- and the default path returns it; every honest C '
     'spelling (switch/if-chain, default `return mode`, K&R fall-off, copies, '
     'goto, Hook 58 COPYPROP_VAR) puts that copy in the default block (20 '
     'words, 21 in retail).  The callees take no arguments; the former C '
     'invented a `mode` argument and painted a0->v0 to fake the copy '
     '(honesty v2, 2026-10-03).'),
    ('_mk_xpndsp', 0x8009E330, 708,
     'aspsx_reorder_across_join (library band, Psy-Q GCC 2.6.0 + ASPSX): the '
     'shipped C was faithful but 96 swap_instructions expressed ASPSX\'s '
     'scheduling, and two of them exchanged the if/else arms\' `j` words -- '
     'a `j` keeps its target, so the object jumped to the wrong join '
     '(word 69 for 43, 43 for 70) and only the build copying retail\'s jump '
     'targets made it match (found by the 2026-10-03 relocation-record fixup). '
     'The honest pipeline (`--delay-slot-load-nop --fill-load-delay`) gets both '
     'joins right but is 178 words for 177 with ASPSX load-delay fills nobody '
     'emulates yet.'),
    ('GsMapModelingData', 0x8009D220, 112,
     'gcc_dbr_fallthrough_fill (library band, Psy-Q GCC 2.6.0): retail fills '
     'the loop-guard `blez` delay slot with the FALL-THROUGH preheader bump '
     '(`addiu a0,a0,4`) and keeps `move a2,zero` above the branch; every cc1 '
     '(2.6.0/2.6.3/2.7.2) fills it with the preceding `move a2,zero`, and '
     'without dbr the fill tools take the earliest word.  The old file bridged '
     'it with a branch swap + adjust_branch_offset (banned for new use).  The '
     'epilogue-label bug in fill_gcc_delay_slots --fill-epilogue that the '
     'other adjust compensated is fixed (2026-10-03).'),
    # 2026-10-03: four LIBGTE windows no listing and no registry entry covered
    # (found by tools/analysis/audit_main_coverage.py).  Sony asm members: no
    # ccpsx source record in any SDK 2.5-4.5 archive.  Alignment is under each
    # member's own relocation mask (none of these members has relocations).
    ('RotAverageNclip3', 0x800A550C, 136,
     'sony_libgte_verbatim: LIBGTE member CMB_06 (RotAverageNclip3), 36 words, '
     'aligned at 0x800A550C 34/36 in SDK 3.3/3.5/3.6/4.0/4.5 (a revision '
     'variant -- no archive on disk holds this exact build); 16 of 34 words '
     'are COP2 (lwc2/swc2/cop2 RTPT/NCLIP/AVSZ3).  No ccpsx source record. '
     'Sibling of the whole-member CMB_05 / CMB_15 listings either side.  No C '
     'compiler emits COP2; terminal asm_only.'),
    ('func_800A56E4', 0x800A56E4, 732,
     'sony_libgte_verbatim: five LIBGTE asm members back to back, each a '
     'revision variant of the SDK 3.3-4.5 build: CMB_12 RotColorDpq 19/20 at '
     '0x800A56E4, CMB_13 RotColorDpq3 38/40 at 0x800A5730, CMB_08 '
     'RotAverageNclipColorDpq3 50/52 at 0x800A57C8, CMB_09 '
     'RotAverageNclipColorCol3 50/52 at 0x800A5890, CMB_14 RotColorMatDpq '
     '26/28 at 0x800A5958; 99 of 183 words are COP2.  No ccpsx source record '
     'in any version.  No C compiler emits COP2; terminal asm_only.'),
    ('func_800A5A40', 0x800A5A40, 352,
     'sony_libgte_verbatim: LIBGTE member MTX_003 (CompMatrix), 88 words, '
     'aligned at 0x800A5A40 85/88 in SDK 3.6/4.0/4.5 (and the 3.3 MTX_00 '
     'super-member 645/648 over the run); 29 of 88 words are COP2.  No ccpsx '
     'source record.  No C compiler emits COP2; terminal asm_only.'),
    ('func_800A6460', 0x800A6460, 80,
     'sony_libgte_verbatim: LIBGTE member FGO_00 (TransposeMatrix), WHOLE '
     'member 20/20 words, all 20 compared at 32 bits (no relocations), exact '
     'in SDK 3.3/3.5/3.6 at 0x800A6460.  No ccpsx source record: a hand-written '
     'Sony asm member (0 COP2 -- an lh/sh transpose; like RotMatrix, a '
     'verbatim Sony object is asm_only whatever its COP2 count).  Konami linked '
     'LIBGTE verbatim (docs/reference/PSYQ-LIB-IDENTIFICATION.md).  Terminal '
     'asm_only.'),
    # 2026-11-05: the LIBGTE listings that existed only as local, untracked
    # files (asm/main/matched/ is gitignored); now emitted from the disc too.
    ('InitGeom', 0x800A3540, 128,
     'GTE ctc2 hand-asm (InitGeom: mfc0/mtc0 SR + ctc2 constant block)'),
    ('MulMatrix', 0x800A49A0, 268,
     'sony_libgte_verbatim: 4.5 LIBGTE.LIB member MTX_03 +0x0 (MulMatrix), 23/67 COP2 words; whole member aligned 67/68 at 0x800A49A0 (miss = trailing pad)'),
    ('MulMatrix2', 0x800A4AAC, 268,
     'sony_libgte_verbatim: 4.5 LIBGTE.LIB member MTX_04 +0x0 (MulMatrix2), 23/67 COP2 words; whole member aligned 67/68 at 0x800A4AAC (miss = trailing pad)'),
    ('ApplyMatrixSV', 0x800A4C08, 92,
     'sony_libgte_verbatim: 4.5 LIBGTE.LIB member MTX_06 +0x0 (ApplyMatrixSV), 11/23 COP2 words; whole member aligned 23/24 at 0x800A4C08 (miss = trailing pad)'),
    ('TransMatrix', 0x800A4C64, 36,
     "libgte TransMatrix, linked verbatim from Sony's hand-written object (three batched lw into t0-t2; neither IDO nor GCC 2.6/2.7 emit it)"),
    ('ScaleMatrix', 0x800A4C88, 292,
     'GTE hand-asm: LIBGTE ScaleMatrix'),
    ('SetRotMatrix', 0x800A4DAC, 48,
     'GTE hand-asm: libgte SetRotMatrix -- five lw from the MATRIX'),
    ('SetLightMatrix', 0x800A4DDC, 48,
     'GTE hand-asm loader (5 COP2 words in 12)'),
    ('SetColorMatrix', 0x800A4E0C, 48,
     'GTE hand-asm loader (5 COP2 words in 12)'),
    ('SetTransMatrix', 0x800A4E3C, 36,
     'GTE hand-asm loader (3 COP2 words in 9)'),
    ('SquareRoot0', 0x800A4E60, 420,
     'GTE hand-asm (12-leaf GTE blob, 12 jr ra, 23 COP2 words in 105) -- ctc2/mfc2 loaders and readers'),
    ('SetBackColor', 0x800A5004, 32,
     'GTE hand-asm loader (3 COP2 words in 8)'),
    ('SetFarColor', 0x800A5024, 172,
     'GTE ctc2/mtc2 hand-asm (window covers ~7 tiny SetRotMatrix/SetVector-style loaders)'),
    ('SetGeomScreen', 0x800A50D0, 12,
     'GTE hand-asm reader (1 COP2 word in 3)'),
    ('SetDQA', 0x800A50DC, 12,
     'GTE hand-asm: ctc2 a0,$27 wrapper (libgte SetXXX control-register store)'),
    ('MulMatrix0', 0x800A5BA0, 728,
     'sony_libgte_verbatim: 4.5 LIBGTE.LIB members MTX_000 +0x0 (MulMatrix0, 67/68 at 0x800A5BA0) + MTX_002 (MulRotMatrix0, 57/60 at 0x800A5CAC) + MTX_001 (MulRotMatrix, 58/60 at 0x800A5D90), back-to-back, misses = trailing pads only; 59/182 COP2 words'),
    ('ApplyMatrixLV', 0x800A5F88, 692,
     'GTE hand-asm (25 COP2 words in 173, 3 jr ra) -- no C compiler emits COP2'),
    ('PushMatrix', 0x800A623C, 160,
     'hand-written assembly -- a GTE control-register dump: eight cfc2 $0..$7'),
    ('PopMatrix', 0x800A62DC, 304,
     'GTE hand-asm: libgte PopMatrix + ReadRotMatrix + ReadLightMatrix in one 304-byte window. Saves $ra to a GLOBAL (sw ra,D_800E4750), uses `addi` (no C compiler emits addi), ctc2 $0-$7 from the matrix stack slot, cfc2 $0-$7 / $8-$15 stores. Same class as func_800A623C / func_800A4DAC.'),
    ('GsTMDfastTNG3', 0x800A7AF0, 376,
     'GTE hand-asm: libgs TMDF GT3-family sorter (lwc2 $0-$5 vertex loads, RTPT 0x280030, NCLIP 0x1400006, AVSZ3 0x158002d, mtc2/mfc2, six swc2 stores into the 40-byte 0x09-tag packet). Sits between the verbatim TMDF_FT3 and TMDF_G4 LIBGTE members (PSYQ-LIB-IDENTIFICATION.md) and is the same hand-written family; no C spelling reaches cop2.'),
    ('GsTMDfastTG3NL', 0x800A7C68, 348,
     'sony_libgte_verbatim: 2.5 LIBGTE.LIB member TMDF_GT3 +0x178 (GsTMDfastTG3NL), 21/87 COP2 words; member aligned 399/400 at 0x800A7AF0 (miss = trailing pad)'),
    ('GsTMDfastTG3L', 0x800A7DC4, 436,
     'Sony LIBGTE VERBATIM: LIBGTE 2.5 member TMDF_GT3 +0x2d4 (GsTMDfastTG3L), 26/109 COP2 words; whole 1600-byte member matches retail 399/400 from 0x800A7AF0'),
    ('GsTMDfastTG3LFG', 0x800A7F78, 872,
     'GTE hand-asm (52 COP2 words in 218, 2 jr ra)'),
    ('rand', 0x800B4870, 16,
     'PS1 BIOS call thunk (li t2,0xA0; jr t2; li t1,0x2F) -- hand-written, no compiler emits jr t2'),
]


def disasm(addr, size):
    out = subprocess.run(['bash', DIS, '0x%08X' % addr, str(size)],
                         capture_output=True, text=True).stdout
    m = {}
    for ln in out.splitlines():
        p = ln.split('\t')
        if len(p) >= 3 and p[0].strip().endswith(':'):
            try:
                a = int(p[0].strip()[:-1], 16)
            except ValueError:
                continue
            m[a] = ' '.join(x.strip() for x in p[2:])
    return m


def emit(name, addr, size, why, force):
    path = os.path.join(OUT, name + '.s')
    if os.path.exists(path) and not force:
        print('exists (use --force): %s' % path)
        return
    data = open(BIN, 'rb').read()
    off = (addr - 0x80041000) + 0x800
    m = disasm(addr, size)
    L = ['.set noreorder', '.set noat', '', '.text', '.globl %s' % name, '',
         '/*', ' * %s' % name, ' * Address: 0x%08X' % addr,
         ' * Size: %d bytes (%d instructions)' % (size, size // 4),
         (' * Region: Library (0x80086000-0x800BFFFF)' if addr >= 0x80086000
          else ' * Region: Game (0x80041000-0x80085FFF)'),
         ' * Status: asm_only (byte-perfect ASM, no C)', ' *',
         ' * BLOCKER: ' + why, ' *',
         ' * Emitted by tools/analysis/emit_asm_only_window.py from disks/SLPS_000.65',
         ' * (byte-exact by construction); the disassembly is in the trailing comments.',
         ' */', '%s:' % name]
    for i in range(0, size, 4):
        w = struct.unpack('<I', data[off + i:off + i + 4])[0]
        L.append('    .word 0x%08x    /* %08x  %s */' % (w, addr + i, m.get(addr + i, '')))
    os.makedirs(OUT, exist_ok=True)
    open(path, 'w').write('\n'.join(L) + '\n')
    print('wrote %s (%d words)' % (path, size // 4))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--force', action='store_true')
    ap.add_argument('--only')
    args = ap.parse_args()
    if not os.path.exists(BIN):
        sys.exit('missing %s' % BIN)
    for name, addr, size, why in WINDOWS:
        if args.only and args.only != name:
            continue
        emit(name, addr, size, why, args.force)


if __name__ == '__main__':
    main()
