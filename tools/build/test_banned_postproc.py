#!/usr/bin/env python3
"""Regression tests for banned_postproc: is_painting_script() and the
RESTRICTED classes (banned for new use, pre-ban users grandfathered).

Guards the 2026-08-09 main-binary `custom_script` audit
(docs/reference/CUSTOM-SCRIPT-AUDIT.md).  That audit read all 163 live
`custom_script` directives under src/main/ by hand and found the detector was
missing whole classes of painting -- 32 of them in the SHIPPED build.

Every POSITIVE fixture below is reduced from a real script that the detector
returned False for before the fix.  Every NEGATIVE fixture is reduced from a
real script that is genuinely ALLOWED (register-field / imm16 / permutation
work) and must stay unflagged -- the precision half matters just as much,
because a false positive makes the build refuse a legitimate object.

Run directly:  python3 tools/build/test_banned_postproc.py
"""
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from banned_postproc import is_painting_script, _painting_cache


# ── POSITIVE fixtures: each MUST be detected as painting ──────────────

# custom_func_80061B3C.py / custom_func_80053A8C.py: a whole instruction word
# pasted as a bare CALL ARGUMENT. The old detector only looked at assignment
# and pack_into contexts, so `add(0x...)` was invisible.
BARE_CALL_ARG_PASTE = '''
import struct
def main(data, text_off):
    new = []
    def add(w):
        new.append(w)
    add(0x3c13801f)          # lui s3,0x801f  (fabricated)
    add(0x26730004)          # addiu s3,s3,4  (fabricated)
    for i, w in enumerate(new):
        struct.pack_into("<I", data, text_off + i * 4, w)
'''

# custom_func_8004D3E8.py: `write_u32(data, off, 0x00008025)` -- a `lw`
# replaced by `move s0,zero`. Neither the name filter nor the assignment
# regex matched.
WRITER_HELPER_PASTE = '''
import struct
def write_u32(data, off, val):
    data[off:off+4] = struct.pack("<I", val)
def main(data, text_off):
    write_u32(data, text_off + 81 * 4, 0x00008025)
'''

# custom_func_8005454C.py: slot assignment on a list NOT named insns/words/code.
NONSTANDARD_SLOT_PASTE = '''
import struct
def main(data, text_off, n):
    out = [0] * n
    out[7] = 0x2C41000F      # sltiu at,v0,15  (fabricated)
    for i, w in enumerate(out):
        struct.pack_into("<I", data, text_off + i * 4, w)
'''

# custom_func_800556BC_v2.py: pasted addiu/R-type words. The old
# _writes_hardcoded_insn only fired on lui/load/store opcodes.
NON_MEMORY_OPCODE_PASTE = '''
import struct
def main(data, txt_off):
    new_insns = [None] * 3
    new_insns[16] = (0x25F80200, None)   # addiu t8,t7,0x200
    new_insns[18] = (0x03198021, None)   # addu s0,t8,t9
    for i, (insn, _) in enumerate(new_insns):
        struct.pack_into("<I", data, txt_off + i * 4, insn)
'''

# postproc_func_8009FDD8.py: the literal is laundered through a local name
# before the write.
NAMED_LITERAL_PASTE = '''
import struct
def main(data, off):
    move_v0_v1 = 0x00601021
    def set_ins(i, w):
        struct.pack_into("<I", data, off + i * 4, w)
    set_ins(17, move_v0_v1)
'''

# custom_event_face2.py: patch_opcode with the operand PARENTHESISED --
# `((new_opcode & 0x3F) << 26)`. sh -> sb is an operation change.
PARENTHESISED_OPCODE_WRITE = '''
import struct
def patch_opcode(data, text_off, idx, new_opcode):
    pos = text_off + idx * 4
    insn = struct.unpack("<I", data[pos:pos+4])[0]
    insn = (insn & 0x03FFFFFF) | ((new_opcode & 0x3F) << 26)
    struct.pack_into("<I", data, pos, insn)
'''

# custom_func_800521DC.py / custom_get_h_tokimeki.py: a nop painted over a
# live instruction (a nop IS sll $zero,$zero,0 -- fabrication either way).
NOP_PAINT_OVER_LIVE = '''
import struct
def main(data, text_off):
    def W(i, v):
        struct.pack_into("<I", data, text_off + i * 4, v)
    W(27, 0x00000000)        # was: move a0,v0
'''

# custom_SsUtAutoPan.py / custom_GsTMDfastTNF3.py: the emitted instruction
# COUNT differs from the compiled count, written back as a new section size.
REBUILT_TEXT_RESIZE = '''
import struct
def main(data, text_sh_off, text_off, insns):
    result = insns[:20] + insns[21:]          # drop one
    new_text_size = len(result) * 4
    struct.pack_into("<I", data, text_sh_off + 0x14, new_text_size)
    for i, w in enumerate(result):
        struct.pack_into("<I", data, text_off + i * 4, w)
'''

# custom_func_8009AE74.py: the shift-delete loop.
SHIFT_DELETE_LOOP = '''
import struct
def main(data, text_off, insns, n):
    for i in range(25, n - 1):
        insns[i] = insns[i + 1]
    n -= 1
    struct.pack_into("<I", data, text_off, insns[0])
'''

# custom_func_80085890.py: a SPECIAL-class word assembled purely from literals
# and register constants -- no `<< 26` term, so the opcode-anchored check
# never saw it.
LITERAL_FIELD_ASSEMBLY = '''
import struct
T2, T8 = 10, 24
def main(data, text_off, insns):
    insns[19] = (T2 << 16) | (T8 << 11) | (16 << 6) | 0x02   # srl t8,t2,0x10
    struct.pack_into("<I", data, text_off + 19 * 4, insns[19])
'''

# postproc_func_8006EF68.py: a hardcoded word returned by a lambda that
# ignores the compiled word entirely.
LAMBDA_WORD_PAINT = '''
import struct
PATCH_TABLE = [
    (0x7c, lambda w: 0x24020001),   # nop -> addiu v0,zero,1
]
def main(data, text_off):
    for off, fn in PATCH_TABLE:
        struct.pack_into("<I", data, text_off + off, fn(0))
'''

# return_schedule_regs.py: the canonical PATCHES list, but with DECIMAL
# indices instead of hex offsets.
DECIMAL_PATCHES_LIST = '''
import struct
PATCHES = [
    (249, 0x0058c821),
    (313, 0x1560003e),   # bnez t3,+62 <- bne a2,at,+63 (opcode change)
]
def main(data, text_off):
    for i, w in PATCHES:
        struct.pack_into("<I", data, text_off + i * 4, w)
'''

# asm_intercept_func_8008EB8C.py: the compiler output is discarded and a
# hand-written listing is assembled instead. CLAUDE.md bans this by name.
HANDWRITTEN_ASM_TRIPLE_QUOTED = '''
def transform(lines):
    return r"""\tset noreorder
\t.ent SsUtKeyOn
SsUtKeyOn:
\taddiu\t$sp, $sp, -56
\tlui\t$14, %hi(D_8013140C)
\tlw\t$14, %lo(D_8013140C)($14)
\tsw\t$31, 44($sp)
\tjr\t$31
\tnop
\t.end SsUtKeyOn""".split("\\n")
'''

# transform_func_8008bee8.py: the same thing built line by line.
HANDWRITTEN_ASM_APPENDS = r'''
def transform(lines):
    result = []
    result.append('\taddiu\t$sp, $sp, -80')
    result.append('\tsll\t$5, $5, 24')
    result.append('\tsra\t$5, $5, 24')
    result.append('\tbeq\t$4, $0, $L32')
    result.append('\tjr\t$31')
    return result
'''

# transform_fallthrough.py (reached through asm_intercept_func_80044AC0.py and
# its two sisters): an asm-text transform that DELETES the compiler's `jr ra`
# so the body "falls through". Fabrication by subtraction -- same family as the
# purged cross-prologue head stubs.
ASM_TRANSFORM_DELETES_RETURN = r'''
def transform(lines):
    result = []
    skip_next = False
    for line in lines:
        if skip_next:
            skip_next = False
            result.append(line)
            continue
        if 'jr\t$ra' in line or 'jr\t$31' in line:
            skip_next = True
            continue
        result.append(line)
    return result
'''

# The wrapper that references it is clean on its own -- the content scan has to
# follow the TRANSFORM reference one level to see the painting.
ASM_INTERCEPT_WRAPPER = '''
import os, sys, subprocess
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
TRANSFORM = os.path.join(SCRIPT_DIR, "transform_fallthrough_fixture.py")
def main():
    subprocess.run([sys.executable, "asm_intercept.py", "src.c", sys.argv[2], TRANSFORM])
'''

# asm_intercept_SsUtKeyOn.py / custom_func_80059E48.py: a BANNED directive
# smuggled into a subprocess argv, where check_banned_postproc cannot see it.
BANNED_DIRECTIVE_VIA_SUBPROCESS = '''
import subprocess, sys
def main(body_o, output_o, prepend_script):
    subprocess.run([sys.executable, prepend_script, body_o, output_o,
                    "--insns", "0x3C0E800D", "0x8DCEFC30"])
'''


# ── NEGATIVE fixtures: each MUST stay unflagged ───────────────────────

# custom__spu_ioctl.py / custom_func_8007B1D4.py: read-modify-write of a
# register field only.
REGISTER_FIELD_PATCH = '''
import struct
def patch_reg_field(word, shift, old_reg, new_reg):
    mask = 0x1F << shift
    if ((word >> shift) & 0x1F) != old_reg:
        return word
    return (word & ~mask) | (new_reg << shift)
def main(data, base):
    w = struct.unpack("<I", data[base:base+4])[0]
    w = patch_reg_field(w, 16, 15, 8)
    struct.pack_into("<I", data, base, w)
'''

# postproc_shadow_file_name.py / remap_tregs_756ac.py / patch_stack_complete.py:
# the word is REBUILT from fields decoded out of the compiled word. The
# `(opcode << 26)` here preserves the operation -- this is the allowed
# swap_registers / patch_insn_reg_field class and must not be flagged.
DERIVED_OPCODE_REENCODE = '''
import struct
def remap_reg_in_insn(word, old, new):
    opcode = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    sa = (word >> 6) & 0x1F
    funct = word & 0x3F
    if rs == old: rs = new
    if rt == old: rt = new
    return (opcode << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (sa << 6) | funct
def main(data, off):
    w = struct.unpack("<I", data[off:off+4])[0]
    struct.pack_into("<I", data, off, remap_reg_in_insn(w, 15, 8))
'''

# custom_8007837C.py: frame-immediate rewrite + s7<->s8 swap. Uses
# `(0x1F << 21)` masks and `(REG_FP << 21)` constants, but every term is
# folded into a word READ from the object.
FRAME_AND_REGISTER_SWAP = '''
import struct
REG_S7, REG_FP = 23, 30
def main(data, off):
    word = struct.unpack("<I", data[off:off+4])[0]
    if ((word >> 26) & 0x3F) == 0x09 and (word & 0xFFFF) == 0xFF30:
        word = (word & 0xFFFF0000) | ((-224) & 0xFFFF)
    new_word = word
    if ((word >> 21) & 0x1F) == REG_S7:
        new_word = (new_word & ~(0x1F << 21)) | (REG_FP << 21)
    struct.pack_into("<I", data, off, new_word)
'''

# custom_func_8009C314.py / fix_func_80085BB4.py: a pure permutation of words
# already present in the object.
PURE_PERMUTATION = '''
import struct
def main(data, text_off):
    perm = [4, 5, 0, 1, 2, 3, 6]
    block = [struct.unpack("<I", data[text_off + i*4:text_off + i*4 + 4])[0]
             for i in range(7)]
    for i, src in enumerate(perm):
        struct.pack_into("<I", data, text_off + i * 4, block[src])
'''

# cd_read.py: appending a trailing nop to an already-complete body is
# explicitly permitted.
TRAILING_NOP_APPEND = '''
def main(data, insert_pos):
    # append trailing padding nop
    return data[:insert_pos] + b"\\x00\\x00\\x00\\x00" + data[insert_pos:]
'''

# custom_func_8005E694.py: only drives allowed named tools.
TOOL_DRIVER = '''
import subprocess, sys, os
def main(inp, out, tools_ido, tools_postproc, target_exe):
    subprocess.run([sys.executable, os.path.join(tools_ido, "patch_stack_complete.py"),
                    inp, "t1.o", "--old", "104", "--new", "112"])
    subprocess.run([sys.executable, os.path.join(tools_ido, "swap_registers.py"),
                    "t1.o", "t2.o", "--from", "v1", "--to", "a1"])
    subprocess.run([sys.executable, os.path.join(tools_postproc, "scheduler_normalizer.py"),
                    "t2.o", "--target", target_exe, "--address", "0x8005E694", "-o", out])
'''

# A guard that COMPARES against a known compiled word is a read, not a paste.
LITERAL_IN_GUARD_ONLY = '''
import struct
def main(data, off):
    insn = struct.unpack("<I", data[off:off+4])[0]
    if insn == 0x27bdffe0:
        insn = (insn & 0xFFFF0000) | ((-48) & 0xFFFF)
    assert insn != 0x00402025, "unexpected move a0,v0"
    struct.pack_into("<I", data, off, insn)
'''


POSITIVES = [
    ("bare call-arg paste", BARE_CALL_ARG_PASTE),
    ("writer-helper paste", WRITER_HELPER_PASTE),
    ("non-standard slot paste", NONSTANDARD_SLOT_PASTE),
    ("non-memory opcode paste", NON_MEMORY_OPCODE_PASTE),
    ("named-literal paste", NAMED_LITERAL_PASTE),
    ("parenthesised opcode write", PARENTHESISED_OPCODE_WRITE),
    ("nop painted over live insn", NOP_PAINT_OVER_LIVE),
    ("rebuilt .text resize", REBUILT_TEXT_RESIZE),
    ("shift-delete loop", SHIFT_DELETE_LOOP),
    ("literal field assembly", LITERAL_FIELD_ASSEMBLY),
    ("lambda word paint", LAMBDA_WORD_PAINT),
    ("decimal PATCHES list", DECIMAL_PATCHES_LIST),
    ("hand-written asm (triple-quoted)", HANDWRITTEN_ASM_TRIPLE_QUOTED),
    ("hand-written asm (appends)", HANDWRITTEN_ASM_APPENDS),
    ("banned directive via subprocess", BANNED_DIRECTIVE_VIA_SUBPROCESS),
    ("asm transform deletes jr ra", ASM_TRANSFORM_DELETES_RETURN),
]

NEGATIVES = [
    ("register field patch", REGISTER_FIELD_PATCH),
    ("derived-opcode re-encode", DERIVED_OPCODE_REENCODE),
    ("frame + register swap", FRAME_AND_REGISTER_SWAP),
    ("pure permutation", PURE_PERMUTATION),
    ("trailing nop append", TRAILING_NOP_APPEND),
    ("tool driver", TOOL_DRIVER),
    ("literal in guard only", LITERAL_IN_GUARD_ONLY),
]


def _check(source: str) -> bool:
    with tempfile.NamedTemporaryFile("w", suffix=".py", delete=False) as f:
        f.write(source)
        path = f.name
    try:
        _painting_cache.pop(path, None)
        return is_painting_script(path)
    finally:
        Path(path).unlink(missing_ok=True)


def _check_wrapper_follows_transform() -> bool:
    """The wrapper is clean; the painting is in the transform module it names.
    is_painting_script must follow that reference one level."""
    with tempfile.TemporaryDirectory() as d:
        (Path(d) / "transform_fallthrough_fixture.py").write_text(
            ASM_TRANSFORM_DELETES_RETURN)
        wrapper = Path(d) / "asm_intercept_wrapper_fixture.py"
        wrapper.write_text(ASM_INTERCEPT_WRAPPER)
        _painting_cache.pop(str(wrapper), None)
        return is_painting_script(wrapper)


# ── RESTRICTED-class fixtures (2026-09-27 hardening) ──────────────────
# Classes banned for NEW use while their pre-ban users are grandfathered as a
# remediation worklist (tools/build/postproc_grandfathered.json).  Each class
# was confirmed by reading its tool; see banned_postproc.RESTRICTED_DIRECTIVES.
# A NEW function is one that is in no grandfather entry; the identities below
# are addresses no function lives at.
NEW_OVL = "DATE/func_8015FFF0"
NEW_MAIN = "func_800FFFF0"

RESTRICTED_POSITIVES = [
    ("fill_delay_slots, new main function", NEW_MAIN,
     [{"type": "fill_delay_slots"}]),
    ("swap_spill_with_move, new overlay function", NEW_OVL,
     [{"type": "swap_spill_with_move", "offset": 0x24, "dst": "v0"}]),
    ("invert_branch, new overlay function", NEW_OVL,
     [{"type": "invert_branch", "offsets": [0x40]}]),
    ("nop_at_offset (unmarked), new overlay function", NEW_OVL,
     [{"type": "nop_at_offset", "offset": 0x94}]),
    ("patch_insn_imm16, new overlay function", NEW_OVL,
     [{"type": "patch_insn_imm16", "patches": ["0x84:24:44"]}]),
    ("convert_addiu_to_slti, new overlay function", NEW_OVL,
     [{"type": "convert_addiu_to_slti", "offset": 0x58}]),
    ("zero<->reg paint (rs:zero:v0), new overlay function", NEW_OVL,
     [{"type": "patch_insn_reg_field", "patches": ["0x40:rs:zero:v0"]}]),
    ("zero<->reg paint (rt:v0:zero), new main function", NEW_MAIN,
     [{"type": "patch_insn_reg_field", "patches": ["0x40:rt:v0:zero"]}]),
    # 2026-10-03 (red-team finding 3): armed restricted classes
    ("adjust_branch_offset, new overlay function", NEW_OVL,
     [{"type": "adjust_branch_offset", "offset": 0x2c, "imm": 3}]),
    ("adjust_branch_offset, new main function", NEW_MAIN,
     [{"type": "adjust_branch_offset", "offset": 0x28, "imm": 0xf}]),
    ("convert_ori_to_addiu, new overlay function", NEW_OVL,
     [{"type": "convert_ori_to_addiu", "offsets": [19]}]),
    ("convert_ori_to_addiu (all), new main function", NEW_MAIN,
     [{"type": "convert_ori_to_addiu", "offsets": []}]),
    ("convert_addiu_to_ori, new overlay function", NEW_OVL,
     [{"type": "convert_addiu_to_ori", "offsets": [0x30]}]),
]

# 2026-10-03 (red-team findings 1 and 5): absolute bans the step-type check
# must apply to whatever the parsers produce.
BANNED_STEP_POSITIVES = [
    ("prepend_thunk_insns (overlay parser's type for prepend_thunk --insns)",
     [{"type": "prepend_thunk_insns", "insns": ["0x3c0e800d"]}]),
    ("custom_script by absolute path",
     [{"type": "custom_script", "script": "/tmp/redteam/regfix_rs.py", "extra_args": []}]),
    ("custom_script climbing out of the tree",
     [{"type": "custom_script", "script": "../../../../tmp/regfield.py", "extra_args": []}]),
    ("custom_script by ~ path",
     [{"type": "custom_script", "script": "~/immfix.py", "extra_args": []}]),
    ("asm_intercept by absolute path",
     [{"type": "asm_intercept", "script": "/tmp/x.py"}]),
]
BANNED_STEP_NEGATIVES = [
    ("in-tree custom_script by bare name",
     [{"type": "custom_script", "script": "CdRead.py", "extra_args": []}]),
    ("in-tree custom_script by repo-relative path",
     [{"type": "custom_script", "script": "tools/overlay/custom/zero_dead_lui_at.py",
       "extra_args": []}]),
]
SRC_CODEMOTION = """/* @CompileEnv: IDO52_SKIP_PASSES=loopunroll,codemotion */
void func_8015FFF0(void) {}
"""
SRC_ABS_SCRIPT = """/* @PostProc: custom_script /tmp/scratch/regfix_rs.py 0x8 0xc */
void func_8015FFF0(void) {}
"""
SRC_PROSE_CODEMOTION = """/* func_8015FFF0 -- IDO52_SKIP_PASSES=codemotion was retired here
 * (it miscompiles); prose naming it is not a directive. */
void func_8015FFF0(void) {}
"""

RESTRICTED_NEGATIVES = [
    ("ordinary register paint, new function", NEW_OVL,
     [{"type": "patch_insn_reg_field", "patches": ["0x40:rs:t6:v0", "0x44:rt:t7:t6"]}]),
    ("allowed reorder steps, new function", NEW_MAIN,
     [{"type": "swap_instructions", "offset1": 0x10, "offset2": 0x14},
      {"type": "scheduler_normalizer"}]),
    ("no steps at all", NEW_OVL, []),
]

# Path-mode fixtures: the test harnesses call check_banned_postproc(PATH),
# which must parse with the build's own parser (prose-immune) and must read a
# sanctioned qualifier off the directive line.
SRC_NOP_UNMARKED = """/* @PostProc: nop_at_offset --offset 0x94 */
void func_8015FFF0(void) {}
"""
SRC_NOP_DEAD_ARG_HOME = """/* @PostProc: nop_at_offset --offset 0x94 --dead-arg-home */
void func_8015FFF0(void) {}
"""
SRC_IMM16_EXPECT = """/* @PostProc: patch_insn_imm16 0x84:24:44 --expect */
void func_8015FFF0(void) {}
"""
SRC_PROSE_ONLY = """/* func_8015FFF0 -- the old @PostProc: nop_at_offset --offset 0x94 was
 * retired; header prose that merely NAMES a restricted step is not a directive.
 */
void func_8015FFF0(void) {}
"""


def _restricted_fixture(cls):
    """One site of each restricted class, for the empty-worklist checks."""
    return {
        "fill_delay_slots": [{"type": "fill_delay_slots"}],
        "swap_spill_with_move": [{"type": "swap_spill_with_move", "offset": 0x24,
                                  "dst": "v0"}],
        "invert_branch": [{"type": "invert_branch", "offsets": [0x40]}],
        "nop_at_offset": [{"type": "nop_at_offset", "offset": 0x94}],
        "patch_insn_imm16": [{"type": "patch_insn_imm16", "patches": ["0x84:24:44"]}],
        "convert_addiu_to_slti": [{"type": "convert_addiu_to_slti", "offset": 0x58}],
        "zero_reg_paint": [{"type": "patch_insn_reg_field",
                            "patches": ["0x40:rs:zero:v0"]}],
        "adjust_branch_offset": [{"type": "adjust_branch_offset", "offset": 0x2c,
                                  "imm": 3}],
        "convert_ori_to_addiu": [{"type": "convert_ori_to_addiu", "offsets": [0x30]}],
        "convert_addiu_to_ori": [{"type": "convert_addiu_to_ori", "offsets": [0x30]}],
    }.get(cls)


def _empty_worklist_cases(bp, failures, byname, step_of):
    """With nothing grandfathered, (1) every restricted class is refused for
    REAL functions -- a registered main function by name and by address, and
    an overlay function -- so an identity that resolves to a stale entry
    cannot slip through, and (2) the grandfathering mechanism itself still
    works on a synthetic in-memory entry (allowed at its own count, refused
    one above it), so the day a class is re-armed the check is not dead."""
    n = 0
    classes = sorted(bp._RESTRICTED_NAMES | {bp.ZERO_REG_PAINT})
    idents = []
    for name, (_a, src) in sorted(byname.items()):
        if src.endswith(".c"):
            idents += [name, "func_%08X" % _a]
            break
    ovs = sorted((bp._REPO_ROOT / "src" / "overlay").glob("*/*.c"))
    if ovs:
        idents.append(f"{ovs[0].parent.name}/{ovs[0].stem}")
    if len(idents) < 3:
        failures.append("restricted (empty worklist): no real function to test against")
    for cls in classes:
        steps = _restricted_fixture(cls)
        if steps is None:
            failures.append(f"restricted (empty worklist): no fixture for class {cls} "
                            f"-- add one to _restricted_fixture")
            continue
        for ident in idents:
            n += 1
            if not bp.check_restricted_postproc_steps(steps, ident, strict=False):
                failures.append(f"FALSE NEGATIVE (restricted, empty worklist): {cls} "
                                f"NOT refused for {ident}")
    saved = bp._grandfather_cache
    try:
        for cls, make in sorted(step_of.items())[:2]:
            bp._grandfather_cache = {cls: {f"main/0x{0x800FFFF0:08X}": {"sites": 2}}}
            n += 2
            if bp.check_restricted_postproc_steps(make(2), NEW_MAIN, strict=False):
                failures.append(f"FALSE POSITIVE (restricted, synthetic entry): {cls} "
                                f"refused at its own 2 site(s)")
            if not bp.check_restricted_postproc_steps(make(3), NEW_MAIN, strict=False):
                failures.append(f"FALSE NEGATIVE (restricted, synthetic entry): {cls} "
                                f"allowed 3 site(s), recorded 2")
    finally:
        bp._grandfather_cache = saved
    return n


def _only_shrinks_cases(bp, failures):
    """The grandfather file only shrinks: no (class, function) entry that
    HEAD does not have, no site count above HEAD's, no site signature HEAD
    did not pin, and every restricted class present (armed) so the one-time
    `--census --arm` refuses to re-grandfather it."""
    import json
    import subprocess
    n = 0
    cur = bp._load_grandfathered()
    for cls in sorted(bp._RESTRICTED_NAMES | {bp.ZERO_REG_PAINT}):
        n += 1
        if cls not in cur:
            failures.append(f"restricted: class {cls} is not armed in "
                            f"{bp.GRANDFATHER_FILE.name} (--arm could re-grandfather it)")
    rel = bp.GRANDFATHER_FILE.relative_to(bp._REPO_ROOT)
    r = subprocess.run(["git", "-C", str(bp._REPO_ROOT), "show", f"HEAD:{rel}"],
                       capture_output=True, text=True)
    if r.returncode != 0:
        return n                       # no git history to compare against
    head = json.loads(r.stdout).get("classes", {})
    from collections import Counter
    for cls, ents in sorted(cur.items()):
        for ident, ent in sorted(ents.items()):
            n += 1
            h = head.get(cls, {}).get(ident)
            if h is None:
                failures.append(f"restricted: NEW grandfathered entry {cls} {ident} "
                                f"(not in HEAD) -- the worklist only shrinks")
            elif ent.get("sites", 0) > h.get("sites", 0):
                failures.append(f"restricted: {cls} {ident} grandfathered "
                                f"{ent.get('sites')} site(s), HEAD {h.get('sites')}")
            elif h.get("detail") and (Counter(ent.get("detail") or ["?"] * ent.get("sites", 0))
                                      - Counter(h["detail"])):
                failures.append(f"restricted: {cls} {ident} pins a site HEAD did not")
    return n


def _restricted_step_cases(failures):
    import banned_postproc as bp
    for name, ident, steps in RESTRICTED_POSITIVES:
        if not bp.check_restricted_postproc_steps(steps, ident, strict=False):
            failures.append(f"FALSE NEGATIVE (restricted): {name} was NOT refused")
        if not bp.check_banned_postproc_steps(steps, source_path=ident, strict=False):
            failures.append(f"FALSE NEGATIVE (restricted, build path): {name}")
    for name, ident, steps in RESTRICTED_NEGATIVES:
        if bp.check_restricted_postproc_steps(steps, ident, strict=False):
            failures.append(f"FALSE POSITIVE (restricted): {name} was refused")

    # Grandfathered users pass at their recorded site count and are refused
    # one site above it.  Picked from the live worklist so the test survives
    # remediation; main entries are resolved address -> current NAME, which is
    # what the build passes (names are renamed, addresses are not).
    gf = bp._load_grandfathered()
    byname, _ = bp._registry()
    addr2name = {a: n for n, (a, _s) in byname.items()}
    step_of = {
        "nop_at_offset": lambda n: [{"type": "nop_at_offset", "offset": 4 * i} for i in range(n)],
        "swap_spill_with_move": lambda n: [{"type": "swap_spill_with_move", "offset": 4 * i,
                                            "dst": "v0"} for i in range(n)],
        "fill_delay_slots": lambda n: [{"type": "fill_delay_slots"}] * n,
        "patch_insn_imm16": lambda n: [{"type": "patch_insn_imm16",
                                        "patches": ["0x%x:8:1" % (4 * i) for i in range(n)]}],
        "zero_reg_paint": lambda n: [{"type": "patch_insn_reg_field",
                                      "patches": ["0x%x:rs:zero:v0" % (4 * i) for i in range(n)]}],
    }
    step_of["adjust_branch_offset"] = lambda n: [
        {"type": "adjust_branch_offset", "offset": 4 * i, "imm": 1} for i in range(n)]
    step_of["convert_ori_to_addiu"] = lambda n: [
        {"type": "convert_ori_to_addiu", "offsets": [4 * i for i in range(n)]}]

    def from_detail(cls, detail):
        """The grandfathered sites themselves, rebuilt from their signatures."""
        if cls == "patch_insn_imm16":
            return [{"type": "patch_insn_imm16", "patches": list(detail)}]
        if cls == "zero_reg_paint":
            return [{"type": "patch_insn_reg_field", "patches": list(detail)}]
        if cls == "adjust_branch_offset":
            return [{"type": "adjust_branch_offset", "offset": int(d.split(":")[0], 0),
                     "imm": int(d.split(":")[1], 0)} for d in detail]
        if cls == "convert_ori_to_addiu":
            return [{"type": "convert_ori_to_addiu",
                     "offsets": [] if detail == ["all"] else [int(d, 0) for d in detail]}]
        return None
    checked = 0
    for cls, make in step_of.items():
        for ident, ent in sorted(gf.get(cls, {}).items())[:1]:
            if ident.startswith("main/"):
                name = addr2name.get(int(ident.split("/")[1], 16))
                if name is None:
                    continue
                ident = name
            n = ent["sites"]
            checked += 1
            own = from_detail(cls, ent["detail"]) if ent.get("detail") else make(n)
            if bp.check_restricted_postproc_steps(own, ident, strict=False):
                failures.append(f"FALSE POSITIVE (restricted): grandfathered {ident} "
                                f"refused at its own {n} {cls} site(s)")
            plus = own + make(n + 1)[-1:] if ent.get("detail") else make(n + 1)
            if not bp.check_restricted_postproc_steps(plus, ident, strict=False):
                failures.append(f"FALSE NEGATIVE (restricted): grandfathered {ident} "
                                f"allowed {n + 1} {cls} site(s), recorded {n}")
            if ent.get("detail"):
                # same COUNT, different (offset, value): refused since
                # 2026-10-03 (red-team finding 5, grandfathering by count)
                checked += 1
                if not bp.check_restricted_postproc_steps(make(n), ident, strict=False):
                    failures.append(f"FALSE NEGATIVE (restricted): grandfathered {ident} "
                                    f"allowed {n} SUBSTITUTED {cls} site(s)")
    if checked == 0:
        if any(gf.get(c) for c in gf):
            failures.append("restricted: no grandfathered entry could be exercised")
        else:
            # 2026-10-04: the worklist is EMPTY -- every restricted class at
            # 0 users.  That is the success state, not a failure: check
            # instead that every class is refused for every function.
            checked += _empty_worklist_cases(bp, failures, byname, step_of)
    checked += _only_shrinks_cases(bp, failures)

    # Path mode.
    def path_case(src_text):
        with tempfile.TemporaryDirectory() as d:
            (Path(d) / "DATE").mkdir()          # parent dir names the overlay
            f = Path(d) / "DATE" / "func_8015FFF0.c"
            f.write_text(src_text)
            return bp.check_banned_postproc(f, strict=False)
    if not path_case(SRC_NOP_UNMARKED):
        failures.append("FALSE NEGATIVE (restricted, path): unmarked nop_at_offset")
    if path_case(SRC_NOP_DEAD_ARG_HOME):
        failures.append("FALSE POSITIVE (restricted, path): nop_at_offset --dead-arg-home")
    if bp.IMM16_EXPECT_ENFORCED == bool(path_case(SRC_IMM16_EXPECT)):
        failures.append("restricted, path: patch_insn_imm16 --expect verdict does not "
                        "follow IMM16_EXPECT_ENFORCED")
    if path_case(SRC_PROSE_ONLY):
        failures.append("FALSE POSITIVE (restricted, path): header prose naming a step")
    if not path_case(SRC_CODEMOTION):
        failures.append("FALSE NEGATIVE (path): @CompileEnv IDO52_SKIP_PASSES=...codemotion")
    if not path_case(SRC_ABS_SCRIPT):
        failures.append("FALSE NEGATIVE (path): custom_script by absolute path")
    if path_case(SRC_PROSE_CODEMOTION):
        failures.append("FALSE POSITIVE (path): prose naming the codemotion token")
    for name, steps in BANNED_STEP_POSITIVES:
        if not bp.check_banned_postproc_steps(steps, source_path=NEW_OVL, strict=False):
            failures.append(f"FALSE NEGATIVE (banned step): {name}")
    for name, steps in BANNED_STEP_NEGATIVES:
        if bp.check_banned_postproc_steps(steps, source_path=NEW_OVL, strict=False):
            failures.append(f"FALSE POSITIVE (banned step): {name}")
    return (len(RESTRICTED_POSITIVES) + len(RESTRICTED_NEGATIVES) + 2 * checked + 7
            + len(BANNED_STEP_POSITIVES) + len(BANNED_STEP_NEGATIVES))


def main() -> int:
    failures = []

    n_restricted = _restricted_step_cases(failures)

    if not _check_wrapper_follows_transform():
        failures.append(
            "FALSE NEGATIVE: asm_intercept wrapper whose TRANSFORM module "
            "deletes `jr ra` was NOT detected as painting")

    for name, src in POSITIVES:
        if not _check(src):
            failures.append(f"FALSE NEGATIVE: {name} was NOT detected as painting")

    for name, src in NEGATIVES:
        if _check(src):
            failures.append(f"FALSE POSITIVE: {name} was wrongly flagged as painting")

    # unfill_branch_delay may only ever remove a genuine as1 delay-slot COPY
    # (2026-10-04): its own synthetic-ELF self-test must pass, i.e. a slot
    # that is not bit-identical to the new branch target is refused.
    import io
    import contextlib
    sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "ido"))
    import unfill_branch_delay
    with contextlib.redirect_stdout(io.StringIO()) as ubd_out:
        ubd_rc = unfill_branch_delay.self_test()
    if ubd_rc != 0:
        failures.append("unfill_branch_delay self-test: "
                        + ubd_out.getvalue().strip().replace("\n", " | "))

    total = len(POSITIVES) + len(NEGATIVES) + 2 + n_restricted
    if failures:
        print(f"FAILED {len(failures)}/{total}")
        for f in failures:
            print("  " + f)
        return 1
    print(f"OK: {len(POSITIVES)} painting fixtures detected, "
          f"{len(NEGATIVES)} legitimate fixtures unflagged, "
          f"{n_restricted} restricted-class checks, "
          f"unfill_branch_delay copy-check self-test ({total} total)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
