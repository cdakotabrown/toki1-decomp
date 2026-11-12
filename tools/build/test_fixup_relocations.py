#!/usr/bin/env python3
"""Regression tests for build_level3_c.fixup_relocations (2026-10-03).

Until 2026-10-03 the main-binary fixup copied the retail word over ANY
compiled `lui` with the same rt -- raw constants included -- and over any
j/jal, without checking which symbol the C named.  main icon_disp_switch shipped
"BYTE-PERFECT" with a raw cast `0x800EE760` + `convert_ori_to_addiu` that
really encodes 0x800DE760: the fixup copied retail's lui and hid it.  Jump
tables in .rodata were never compared at all.

The fixup now rewrites a field ONLY at the compiled object's own
R_MIPS_26 / R_MIPS_HI16 / R_MIPS_LO16 records, and only after checking that
the NAME resolves to the address the retail word encodes (or, for the TU's
own .rodata/.data, that the CONTENTS match retail -- jump tables entry by
entry).  These fixtures pin both halves: what must be refused, and what must
still be fixed up (a false refusal makes the build fail a correct function).

Synthetic fixtures run anywhere; the two COMPILED fixtures (the
icon_disp_switch shape) need the IDO recomp and run inside the venv.

Run directly:  python3 tools/build/test_fixup_relocations.py
"""
import os
import struct
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import build_level3_c as b  # noqa: E402

ADDR = 0x80041000                    # file offset 0x800
FO = 0x800


def image(words, extra=b'', extra_addr=None, size=0x4000):
    """A fake executable: header + `words` at ADDR (+ data at extra_addr)."""
    img = bytearray(FO + size)
    for i, w in enumerate(words):
        struct.pack_into('<I', img, FO + 4 * i, w)
    if extra:
        o = extra_addr - ADDR + FO
        img[o:o + len(extra)] = extra
    return img


def body(words):
    return b''.join(struct.pack('<I', w) for w in words)


def run(compiled, retail_img, info):
    errs = []
    out = b.fixup_relocations(body(compiled), ADDR, retail_img, FO,
                              func_name='fixture', obj_info=info, errors=errs)
    return out, errs


def info(text_relocs, secs=None, sec_relocs=None, datasyms=None):
    rel = {'.text': text_relocs}
    rel.update(sec_relocs or {})
    return {'relocs': rel, 'secs': secs or {}, 'datasyms': datasyms or {},
            'strip': 0, 'func_addr': ADDR}


LUI_V0 = lambda imm: 0x3c020000 | (imm & 0xffff)          # lui v0,imm
LW_V0 = lambda imm: 0x8c420000 | (imm & 0xffff)           # lw v0,imm(v0)
ADDIU_V0 = lambda imm: 0x24420000 | (imm & 0xffff)        # addiu v0,v0,imm
ORI_V0 = lambda imm: 0x34420000 | (imm & 0xffff)          # ori v0,v0,imm
JR_RA, NOP = 0x03e00008, 0
JAL = lambda a: 0x0c000000 | ((a >> 2) & 0x3ffffff)
BEQ = lambda d: 0x10400000 | (d & 0xffff)                 # beqz v0,d

failures = []


def check(name, cond):
    print(f"  {'ok  ' if cond else 'FAIL'} {name}")
    if not cond:
        failures.append(name)


def synthetic():
    print("synthetic fixtures:")
    # 1. RAW constant lui (no relocation record) that differs: never copied.
    #    The icon_disp_switch shape: lui 0x800e / addiu -0x18a0 (= 0x800DE760)
    #    where retail has lui 0x800f / addiu -0x18a0 (= 0x800EE760).
    retail = [LUI_V0(0x800f), ADDIU_V0(-0x18a0), JR_RA, NOP]
    comp = [LUI_V0(0x800e), ADDIU_V0(-0x18a0), JR_RA, NOP]
    out, errs = run(comp, image(retail), info([]))
    check("raw-constant lui is NOT copied from retail", out != body(retail))

    # 2. The same address through a NAMED extern: copied, and equal.
    comp = [LUI_V0(0), ADDIU_V0(0), JR_RA, NOP]
    rel = [[0, 5, 'D_800EE760', 'undef', None, 0],
           [4, 6, 'D_800EE760', 'undef', None, 0]]
    out, errs = run(comp, image(retail), info(rel))
    check("named %hi/%lo pair with the right name is fixed up",
          out == body(retail) and not errs)

    # 3. A WRONG name at a real relocation: refused.
    rel_bad = [[0, 5, 'D_800DE760', 'undef', None, 0],
               [4, 6, 'D_800DE760', 'undef', None, 0]]
    out, errs = run(comp, image(retail), info(rel_bad))
    check("wrong %hi/%lo name is refused", out != body(retail) and errs)

    # 4. A negative %lo (the borrow): D_800BA198 = lui 0x800c + -0x5e68.
    retail4 = [LUI_V0(0x800c), LW_V0(-0x5e68), JR_RA, NOP]
    comp4 = [LUI_V0(0), LW_V0(0), JR_RA, NOP]
    rel4 = [[0, 5, 'D_800BA198', 'undef', None, 0],
            [4, 6, 'D_800BA198', 'undef', None, 0]]
    out, errs = run(comp4, image(retail4), info(rel4))
    check("negative %lo carry resolves", out == body(retail4) and not errs)
    rel4b = [[0, 5, 'D_800C9DC0', 'undef', None, 0],
             [4, 6, 'D_800C9DC0', 'undef', None, 0]]
    out, errs = run(comp4, image(retail4), info(rel4b))
    check("borrow misread as an addition is refused", errs != [])

    # 5. jal: right name fixed up, wrong name refused, unresolved refused.
    retail5 = [JAL(0x80051234), NOP, JR_RA, NOP]
    comp5 = [JAL(0), NOP, JR_RA, NOP]
    out, errs = run(comp5, image(retail5),
                    info([[0, 4, 'func_80051234', 'undef', None, 0]]))
    check("jal to the right name is fixed up", out == body(retail5) and not errs)
    out, errs = run(comp5, image(retail5),
                    info([[0, 4, 'func_80051238', 'undef', None, 0]]))
    check("jal to a wrong name is refused", out != body(retail5) and errs)
    out, errs = run(comp5, image(retail5),
                    info([[0, 4, 'no_such_symbol_xyz', 'undef', None, 0]]))
    check("jal to an UNRESOLVED name is refused", out != body(retail5) and errs)

    # 6. jal with NO relocation record (e.g. a fabricated word): not copied.
    out, errs = run([JAL(0x80051230), NOP, JR_RA, NOP], image(retail5), info([]))
    check("j/jal without a relocation record is NOT copied", out != body(retail5))

    # 7. Branch displacement and same-opcode imm16 at non-reloc sites.
    retail7 = [BEQ(2), NOP, ORI_V0(5), JR_RA, NOP]
    out, errs = run([BEQ(3), NOP, ORI_V0(6), JR_RA, NOP], image(retail7), info([]))
    o = [struct.unpack_from('<I', out, 4 * i)[0] for i in range(5)]
    check("branch displacement is NOT copied", o[0] == BEQ(3))
    check("non-address imm16 is NOT copied", o[2] == ORI_V0(6))

    # 8. Jump table: compiled .rodata relocated against .text must equal the
    #    retail table at the address the retail pair encodes.
    tab_addr = 0x80042000
    retail_tab = struct.pack('<3I', ADDR + 0x10, ADDR + 0x18, ADDR + 0x20)
    retail8 = [LUI_V0(0x8004), LW_V0(0x2000), JR_RA, NOP]
    comp8 = [LUI_V0(0), LW_V0(0), JR_RA, NOP]
    rel8 = [[0, 5, '.rodata', 'section', '.rodata', 0],
            [4, 6, '.rodata', 'section', '.rodata', 0]]
    good = struct.pack('<3I', 0x10, 0x18, 0x20)
    srel = {'.rodata': [[0, 2, '.text', 'section', '.text', 0],
                        [4, 2, '.text', 'section', '.text', 0],
                        [8, 2, '.text', 'section', '.text', 0]]}
    img8 = image(retail8, retail_tab, tab_addr)
    out, errs = run(comp8, img8, info(rel8, {'.rodata': good.hex()}, srel))
    check("matching jump table is fixed up", out == body(retail8) and not errs)
    bad = struct.pack('<3I', 0x18, 0x10, 0x20)           # two cases swapped
    out, errs = run(comp8, img8, info(rel8, {'.rodata': bad.hex()}, srel))
    check("jump table with swapped case targets is refused",
          out != body(retail8) and any('JUMP TABLE' in e for e in errs))

    # 9. A string literal in .rodata: contents must match retail.
    s_addr = 0x80043000
    retail9 = [LUI_V0(0x8004), ADDIU_V0(0x3000), JR_RA, NOP]
    comp9 = [LUI_V0(0), ADDIU_V0(0), JR_RA, NOP]
    img9 = image(retail9, b'HELLO\0\0\0', s_addr)
    out, errs = run(comp9, img9, info(rel8, {'.rodata': b'HELLO\0\0\0'.hex()}))
    check("matching .rodata string is fixed up", out == body(retail9) and not errs)
    out, errs = run(comp9, img9, info(rel8, {'.rodata': b'HELLP\0\0\0'.hex()}))
    check("different .rodata string is refused", errs != [])

    # 10. File-static .bss object: its (mdebug) NAME must denote the address.
    rel10 = [[0, 5, '.bss', 'section', '.bss', 0],
             [4, 6, '.bss', 'section', '.bss', 0]]
    out, errs = run(comp, image(retail), info(rel10, datasyms={
        '.bss': [[0, 0, 'D_800EE760']]}))
    check("named static .bss object is fixed up", out == body(retail) and not errs)
    out, errs = run(comp, image(retail), info(rel10, datasyms={
        '.bss': [[0, 0, 'D_800EE764']]}))
    check("misnamed static .bss object is refused", errs != [])
    out, errs = run(comp, image(retail), info(rel10))
    check("anonymous .bss object is refused (unverifiable)", errs != [])

    # 11. No relocation information at all: nothing is copied.
    errs = []
    b.LAST_OBJ_INFO = None
    out = b.fixup_relocations(body([LUI_V0(0), ADDIU_V0(0), JR_RA, NOP]), ADDR,
                              image(retail), FO, errors=errs)
    check("no relocation info -> nothing copied", out != body(retail) and errs)


def compiled():
    """The icon_disp_switch shape, through the real IDO pipeline."""
    if not b.IDO_CC.exists():
        print("compiled fixtures: SKIPPED (IDO recomp not built)")
        return
    print("compiled fixtures (IDO 5.2):")
    good_c = ("extern unsigned char D_800EE760[];\n"
              "unsigned char f(void) { return D_800EE760[3]; }\n")
    raw_c = ("unsigned char f(void) { return *(unsigned char *)0x800DE763; }\n")
    with tempfile.TemporaryDirectory() as td:
        def comp(src):
            p = os.path.join(td, 'f.c')
            with open(p, 'w') as fh:
                fh.write(src)
            by = b.compile_c_function(p, 'f', ADDR, postproc_steps=[])
            return by, b.LAST_OBJ_INFO
        gb, ginfo = comp(good_c)
        # retail = the extern version with its relocations resolved by hand
        words = [struct.unpack_from('<I', gb, i)[0] for i in range(0, len(gb), 4)]
        hi_off = [r[0] for r in ginfo['relocs']['.text'] if r[1] == 5][0]
        lo_off = [r[0] for r in ginfo['relocs']['.text'] if r[1] == 6][0]
        words[hi_off // 4] = (words[hi_off // 4] & 0xffff0000) | 0x800f
        words[lo_off // 4] = (words[lo_off // 4] & 0xffff0000) | 0xe763
        img = image(words)
        errs = []
        out = b.fixup_relocations(gb, ADDR, img, FO, obj_info=ginfo, errors=errs)
        check("compiled extern D_800EE760[3] is fixed up to retail",
              out[:len(words) * 4] == body(words) and not errs)
        rb, rinfo = comp(raw_c)
        errs = []
        out = b.fixup_relocations(rb, ADDR, img, FO, obj_info=rinfo, errors=errs)
        check("compiled raw cast 0x800DE763 is NOT made to match",
              out[:len(words) * 4] != body(words))


if __name__ == '__main__':
    synthetic()
    compiled()
    print(f"\n{len(failures)} failure(s)")
    sys.exit(1 if failures else 0)
