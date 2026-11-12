#!/usr/bin/env python3
"""Banned PostProc directive enforcement.

Centralises the PROJECT POLICY list of banned PostProc directives.
All test harnesses, build pipelines, and status generators MUST import
and call check_banned_postproc() before applying or reporting on PostProc.

The ban is absolute. See docs/reference/POSTPROC-ANNOTATION-REFERENCE.md
and CLAUDE.md for the rationale.
"""

import json
import re
import sys
from pathlib import Path
from typing import List, Tuple

# ── Canonical banned list ──────────────────────────────────────────
# Each entry is (directive_name, reason).
# Matching is done against @PostProc: annotations in C source headers.
BANNED_DIRECTIVES: List[Tuple[str, str]] = [
    (
        "patch_insn_word",
        "Replaces entire instruction word with hardcoded target hex. "
        "That's not decompilation — it's just overwriting bytes.",
    ),
    (
        "patch_instruction",
        "Alias for patch_insn_word. Same violation, same ban.",
    ),
    (
        "fix_prologue_and_delays",
        "Fills jal/j delay slots by CONSUMING the nop (`sw; jal; nop` -> `jal; sw`) and separately ADDS a load-delay nop -- its own docstring. Deletion and creation at once. Armed 2026-09-17 after its seven registered users were de-registered (101 words vs 95, 14.7% without it).",
    ),
    (
        "strip_interior_nops",
        "DELETES interior nops and shifts the remainder. Zeroing a verified-dead TRAILING nop is allowed; interior deletion is fabrication by subtraction. Armed 2026-09-17.",
    ),
    (
        "collapse_shift_move",
        "Collapses a shift/move pattern into fewer words -- instruction deletion. Armed 2026-09-17 (13 words vs 10 without it).",
    ),
    (
        "fix_andi_pattern",
        "Collapses a multi-word andi pattern into fewer words -- instruction deletion. Armed 2026-09-17.",
    ),
    (
        "add_leaf_frame",
        "INSERTS `addiu sp,sp,-N` at the start of .text (shifting all code) and "
        "OVERWRITES the compiled nop in the `jr ra` delay slot -- its own "
        "docstring says so. Two named bans at once: insert_instruction_and_shift, "
        "and painting a compiled nop (a nop IS `sll $zero,$zero,0`). IDO -O2 "
        "elides leaf frames; if retail has one your body is a word short and the "
        "function is unmatchable from C. Armed 2026-09-17 after its three "
        "registered users were de-registered (0/10 at 9 words without it).",
    ),
    (
        "strip_arg_homes",
        "DELETES instructions at given .text offsets and compacts the remaining "
        "code (its own docstring: 'Remove instructions at specified .text "
        "offsets and compact'). Fabrication by SUBTRACTION -- the mirror of "
        "insert_instruction_and_shift. Armed 2026-09-17 after the four "
        "registered users were audited and de-registered: one deleted a genuine "
        "argument-home store the compiler emits and retail lacks, three deleted "
        "a spill reload that retail keeps in a register. If your count is wrong "
        "without it, the function is unmatchable from C -- file it asm_only.",
    ),
    (
        "propagate_stack_to_reg",
        "REPLACES a compiled `sw` with a register copy (`or rd,rs,$zero`) and "
        "optionally NOPs the matching `lw` -- its own docstring: 'replaces a "
        "`sw` instruction at a specified offset with a register copy "
        "instruction'. Changing an instruction's OPERATION is the named "
        "painting class (`sw`->`sh`, `bne`->`xori`, `or`->`beq`), and NOPing "
        "the load deletes a real compiled instruction; both words come from "
        "the target, not the compiler. The 2026-09-17 directive audit cleared "
        "it on a COUNT test -- it rewrites in place, so the word count never "
        "moves -- which is the wrong test for this class. Armed 2026-09-17 "
        "after all nine users were remediated: the six registered main-binary "
        "users and week_day_init closed BYTE-PERFECT honestly (honest "
        "baselines 18/19, 30/31, 19/21, 36/38, 54/56, 96/98, 29/32), the "
        "duplicate head stub holiday_club.c was deleted, and "
        "BUNKA_SD/func_801354D0 was demoted to an honest semi. Two causes, "
        "both in the C: an invented parameter whose argument-home `sw` was "
        "the painted word, and a `return mode;` that keeps the switch "
        "selector live across the arm calls when retail's return value is an "
        "accumulator each exit path overwrites.",
    ),
    (
        "patch_load_signedness",
        "CONVERTS one load opcode into another -- `lh`<->`lhu`, `lb`<->`lbu` "
        "(its own opcode table). Changing an instruction's OPERATION is the "
        "named painting class (`sw`->`sh`, `bne`->`xori`, `or`->`beq`), and "
        "the patched word's signedness comes from the target, not the "
        "compiler. The 2026-09-17 directive audit cleared it on a COUNT test "
        "-- it rewrites in place, so the word count never moves -- which is "
        "the wrong test for this class. Armed 2026-09-17 after both "
        "registered users closed BYTE-PERFECT honestly (honest baselines "
        "40/41 and 53/54, both count-exact). Both causes were C type "
        "spellings that uopt propagates into the reloaded web: "
        "SD_VABClose needed `(s16)(u16)arg0` -- the inner cast builds the "
        "CSE-able truncation node the 41-word shape depends on, the outer "
        "one re-signs the reload -- and SD_VabLoad needed `0x10000` "
        "rather than `0x10000u` on an unrelated `sllv`, whose `u` suffix "
        "was typing the web. If a load's signedness is wrong, the type in "
        "the C is wrong.",
    ),
    (
        "insert_instruction_and_shift",
        "Inserts a new instruction and shifts subsequent code. "
        "Creates instructions from thin air.",
    ),
    (
        "insert_move_and_shift",
        "Variant of insert_instruction_and_shift. Same violation.",
    ),
    (
        "write_insn_at_offset",
        "Writes an entire instruction word at a binary offset. "
        "Same as patch_insn_word — overwrites compiled output with hardcoded bytes.",
    ),
    (
        "prepend_thunk",
        "Inserts hardcoded instruction words at the start of .text and shifts "
        "everything forward (--insns 0x...). Creates instructions the compiler "
        "never emitted, copied from the target — insert_instruction_and_shift + "
        "patch_insn_word combined. CLAUDE.md names it 'the BANNED prepend_thunk'.",
    ),
    (
        "append_trailing_instructions",
        "Appends hardcoded instruction words to the end of .text, growing the "
        "section. Fabricates trailing output (often the next function's bytes to "
        "fake an over-counted symbol). Use @SymbolSize to trim an over-counted "
        "window instead; otherwise it is patch_insn_word at the tail.",
    ),
    (
        "append_trailing",
        "Alias / abbreviation of append_trailing_instructions. Same violation.",
    ),
    (
        "epilogue_share_into_branches",
        "Replaces NOPs in branch-delay slots with a given (hardcoded) instruction "
        "word, duplicating an epilogue insn into slots the compiler left as nop. "
        "Creates instructions from nothing — insert_instruction in disguise.",
    ),
]

# Regex patterns for matching banned directives in @PostProc: lines.
# Each compiles to a pattern that matches the directive name at word-boundary
# start within a PostProc annotation.
_BANNED_PATTERNS = [
    (re.compile(rf"(?<!\w){name}\b"), name, reason)
    for name, reason in BANNED_DIRECTIVES
]


# ── Contextual ban: the fabricated cross-prologue-head stub ────────
# `strip_trailing_instructions` is ALLOWED in general (it removes the main
# binary's types.h `jr ra; nop` thunks -- the compiler's own foreign output).
# It is BANNED in one specific shape, which is why this needs a shape check
# rather than a name in BANNED_DIRECTIVES.
#
# The shape: IDO -O2 hoists a function's first global read ABOVE the prologue
# (`lui $rX,hi; l{w,h,bu,b} $rX,lo($rX)`).  Some .sym maps registered those two
# words as their own symbol.  They have no `jr ra` and fall through into the
# prologue -- they are not functions and cannot be called.  A .c written for
# one returns the global (4 insns: lui, load, jr ra, nop) and then uses
# `strip_trailing_instructions --count 2` to chop the `jr ra; nop` THAT THE
# TARGET DOES NOT CONTAIN.  The remaining 2 words then "match".
#
# That is fabrication by subtraction: the compiler was never going to emit
# those two words alone.  The honest fix is ABSORPTION -- see
# tools/overlay/absorb_stub.py and CLAUDE.md.
_HEAD_STUB_MAX_BYTES = 16

_SIZE_RE = re.compile(r"@SymbolSize:\s*(\d+)")
_SIZE_COMMENT_RE = re.compile(r"Size:\s*(\d+)\s*bytes")
_STRIP_TRAILING_RE = re.compile(r"(?<!\w)strip_trailing_instructions\b")


def check_fabricated_head_stub(
    source_path: Path,
    *,
    strict: bool = True,
) -> List[Tuple[str, str, int]]:
    """Detect a `strip_trailing_instructions` fake cross-prologue-head stub.

    Flags a file that BOTH declares a tiny symbol (<= 16 bytes, i.e. at most
    4 instructions) AND applies `strip_trailing_instructions`.  A genuine
    function that small has nothing legitimate to strip; the combination is
    the fabricated-head signature.

    Returns [(violation_name, reason, line_number)]; empty means clean.
    """
    violations: List[Tuple[str, str, int]] = []

    try:
        with open(source_path, "r", encoding="utf-8") as f:
            lines = f.readlines()
    except Exception:
        return violations

    text = "".join(lines)

    # @SymbolSize is read ANCHORED to a directive line (a prose mention must
    # not change a symbol's declared size); the header `Size: N bytes` comment
    # is the fallback.
    from parse_postproc import parse_symbol_size_text
    size = parse_symbol_size_text(text)
    if size is None:
        m = _SIZE_COMMENT_RE.search(text)
        if m:
            size = int(m.group(1))
    if size is None or size > _HEAD_STUB_MAX_BYTES:
        return violations

    for lineno, line in enumerate(lines, start=1):
        if "@PostProc:" in line and _STRIP_TRAILING_RE.search(line):
            violations.append((
                "strip_trailing_instructions (fabricated head stub)",
                f"Applied to a {size}-byte symbol. A cross-prologue head has no "
                f"`jr ra` to strip -- this chops a return the target does not "
                f"contain, fabricating a match by subtraction. Absorb the head "
                f"into its host instead: tools/overlay/absorb_stub.py.",
                lineno,
            ))

    if strict and violations:
        name, reason, lineno = violations[0]
        print(
            f"⛔ FABRICATED HEAD STUB in {source_path}:{lineno}\n"
            f"   {reason}\n"
            f"\n"
            f"   Fix: python3 tools/overlay/absorb_stub.py <OVERLAY> "
            f"{Path(source_path).stem} --apply\n"
            f"   then make the host read the global as its first statement so\n"
            f"   IDO -O2 hoists the load above the prologue on its own.\n"
            f"\n"
            f"   If the host cannot reproduce the head, delete the fabricated\n"
            f"   body and leave a comment-only `Status: ASM-ONLY` marker. Do\n"
            f"   NOT keep the strip_trailing_instructions directive.",
            file=sys.stderr,
        )
        sys.exit(1)

    return violations


def check_banned_postproc(
    source_path: Path,
    *,
    strict: bool = True,
) -> List[Tuple[str, str, int]]:
    """Scan a C source file for banned PostProc directives.

    Returns a list of (directive_name, reason, line_number) tuples.
    Empty list means the file is clean.

    Args:
        source_path: Path to the .c file to scan.
        strict: If True (default), raise SystemExit on first violation.
                If False, just return the violations list.
    """
    violations: List[Tuple[str, str, int]] = []

    # ⚠️ 2026-09-17: this used to `return violations` (i.e. report CLEAN) on ANY
    # read failure.  A checker that answers "clean" when it cannot read its
    # input is worse than no checker: callers who passed the file CONTENTS
    # instead of the PATH -- an easy mistake, since the parameter is just
    # `source_path` -- got a silent all-clear on every file they tested.  That
    # produced several bogus "banned_postproc: clean" claims before it was
    # noticed.  Fail loudly instead.
    text = str(source_path)
    if "\n" in text or "@PostProc:" in text:
        raise TypeError(
            "check_banned_postproc() takes a PATH, not file contents. "
            "You passed a string that looks like source text."
        )
    try:
        with open(source_path, "r", encoding="utf-8") as f:
            lines = f.readlines()
    except OSError as e:
        raise OSError(
            f"check_banned_postproc() could not read {source_path!r}: {e}. "
            "Refusing to report CLEAN on an unread file."
        ) from e

    for lineno, line in enumerate(lines, start=1):
        if "@CompileEnv:" in line and _CODEMOTION_RE.search(line):
            violations.append((
                "IDO52_SKIP_PASSES=codemotion",
                "MISCOMPILES: deletes loads and leaves their consumers reading "
                "uninitialised stack slots; the shrink is a count compensator "
                "(CLAUDE.md, UOPT-HOOKS-REFERENCE.md Hook 15).", lineno))
        # Only check @PostProc: annotation lines
        if "@PostProc:" not in line:
            continue

        for pattern, name, reason in _BANNED_PATTERNS:
            if pattern.search(line):
                violations.append((name, reason, lineno))
        for m in _SCRIPT_TEXT_RE.finditer(line):
            bad = script_path_violation({"type": "custom_script",
                                         "script": m.group(1).rstrip("*/")})
            if bad:
                violations.append(("out-of-tree custom_script", bad, lineno))

    # RESTRICTED classes, parsed with the build's own parser (prose-immune).
    if not violations and any(
            re.search(rf"(?<!\w){n}\b", "".join(lines)) for n in
            list(_RESTRICTED_NAMES) + ["zero"]):
        violations = check_restricted_postproc_steps(
            _steps_from_source(source_path), source_path, strict=strict)

    if strict and violations:
        directive, reason, lineno = violations[0]
        print(
            f"⛔ BANNED POSTPROC VIOLATION in {source_path}:{lineno}\n"
            f"   Directive: {directive}\n"
            f"   Reason:    {reason}\n"
            f"\n"
            f"   This directive is BANNED by project policy.\n"
            f"   See docs/reference/POSTPROC-ANNOTATION-REFERENCE.md.\n"
            f"\n"
            f"   If you need to fix individual instructions, use ALLOWED\n"
            f"   alternatives:\n"
            f"     • @SymbolSize: N        (THE cutout for prepend_thunk /\n"
            f"                              append_trailing: when the .sym size\n"
            f"                              over-counts into the NEXT, symbol-less\n"
            f"                              function, trim the compare window to the\n"
            f"                              real body. Trims — never fabricates. If\n"
            f"                              the body still doesn't match after the\n"
            f"                              trim, it was real fabrication: rewrite\n"
            f"                              the C or mark BLOCKER.)\n"
            f"     • patch_insn_reg_field  (register field only)\n"
            f"     • patch_insn_imm16      (16-bit immediate only)\n"
            f"     • swap_registers        (full register swap)\n"
            f"     • swap_lui_addr_form    (address load form swap)\n"
            f"     • swap_spill_with_move  (dead-store → move)\n"
            f"     • scheduler_normalizer  (instruction reordering)\n"
            f"\n"
            f"   If the C source fundamentally doesn't match, rewrite the C.\n"
            f"   Do NOT paper over mismatches with hardcoded bytes.",
            file=sys.stderr,
        )
        sys.exit(1)

    return violations


def check_banned_postproc_steps(
    steps: List[dict],
    *,
    source_path: str = "<unknown>",
    strict: bool = True,
) -> List[dict]:
    """Check a parsed PostProc step list for banned directive types.

    Use this when you've already parsed the steps and want to check
    the *type* field rather than re-scanning the source text.

    Returns a list of violating step dicts.
    Empty list means all steps are clean.

    Args:
        steps: List of parsed PostProc step dicts (each has 'type' key).
        source_path: Filename for error messages (string or Path).
        strict: If True (default), raise SystemExit on first violation.
    """
    banned_types = {name for name, _ in BANNED_DIRECTIVES}
    # Also check shortened/internal type names
    banned_types.add("insert_instruction")
    banned_types.add("write_insn")  # write_insn_at_offset internal type
    # 2026-10-03 (red-team finding 5): the overlay parser turns
    # `prepend_thunk --insns ...` into the step type `prepend_thunk_insns`,
    # which the exact-name set missed -- the build would have applied it.
    banned_types.update(_BANNED_TYPE_ALIASES)

    violations = [s for s in steps if _is_banned_type(s.get("type"), banned_types)]

    # 2026-10-03 (red-team finding 1): a custom_script / asm_intercept named by
    # an ABSOLUTE or out-of-tree path runs code nobody can review or the
    # detector can read in context (overlay_postproc._resolve_script_path and
    # build_level3_c both accepted one).  Refuse it outright.
    if not violations:
        for s in steps:
            bad = script_path_violation(s)
            if bad:
                violations.append(s)
                if strict:
                    print(f"⛔ OUT-OF-TREE SCRIPT in {source_path}\n"
                          f"   Step:    {s.get('type')} {s.get('script')}\n"
                          f"   Reason:  {bad}", file=sys.stderr)
                    sys.exit(1)

    # RESTRICTED classes (banned for new use; grandfathered users listed).
    # Runs AFTER the absolute bans so an absolute violation reports first.
    if not violations:
        restricted = check_restricted_postproc_steps(steps, source_path, strict=strict)
        violations = [s for s in steps
                      if s.get("type") in {c for c, _, _ in restricted}
                      or (ZERO_REG_PAINT in {c for c, _, _ in restricted}
                          and s.get("type") == "patch_insn_reg_field")]

    if strict and violations:
        step = violations[0]
        step_type = step["type"]
        # Find the matching reason
        reason = "Unknown banned directive"
        for name, r in BANNED_DIRECTIVES:
            if name == step_type:
                reason = r
                break
        if step_type == "insert_instruction":
            reason = "Inserts instruction and shifts code. Banned by policy."

        print(
            f"⛔ BANNED POSTPROC STEP in {source_path}\n"
            f"   Step type: {step_type}\n"
            f"   Reason:    {reason}\n"
            f"\n"
            f"   This step will NOT be applied. Remove it from the source.",
            file=sys.stderr,
        )
        sys.exit(1)

    return violations


# Internal step TYPES a parser can produce for a banned directive name.
_BANNED_TYPE_ALIASES = {"prepend_thunk_insns", "append_trailing",
                        "write_insn_at_offset", "insert_instruction"}


def _is_banned_type(t, banned_types) -> bool:
    """Exact banned type, or `<banned>_<suffix>` (an internal variant of a
    banned directive, e.g. prepend_thunk_insns)."""
    if not t:
        return False
    if t in banned_types:
        return True
    return any(t.startswith(b + "_") for b in banned_types)


_SCRIPT_STEP_TYPES = ("custom_script", "asm_intercept")


def script_path_violation(step) -> str:
    """Why a custom_script/asm_intercept step's script path is refused, or ''.

    Allowed: a RELATIVE path (or bare name) that stays inside the repository
    when resolved against any of the search roots the pipelines use.
    Refused: an absolute path, a `~` path, or one whose `..` components climb
    out of the repository."""
    if step.get("type") not in _SCRIPT_STEP_TYPES:
        return ""
    name = str(step.get("script") or "")
    if not name:
        return ""
    if name.startswith("~") or Path(name).is_absolute():
        return ("absolute script path -- a custom_script must live in the "
                "repository (tools/ido, tools/postproc, tools/overlay/custom) "
                "and be named relative to it")
    root = _REPO_ROOT.resolve()
    for base in (_REPO_ROOT / "tools" / "ido", _REPO_ROOT / "tools" / "postproc",
                 _REPO_ROOT / "tools" / "overlay" / "custom", _REPO_ROOT):
        try:
            (base / name).resolve().relative_to(root)
        except ValueError:
            return ("script path resolves outside the repository "
                    f"({(base / name).resolve()})")
    return ""


_SCRIPT_TEXT_RE = re.compile(r"(?<!\w)(?:custom_script|asm_intercept)\s+(\S+)")

# 2026-10-03 (red-team finding 5): `IDO52_SKIP_PASSES=...codemotion...`
# MISCOMPILES (CLAUDE.md; Hook 15): it deletes loads and leaves their
# consumers reading uninitialised stack slots, and the shrink is a count
# compensator.  Nothing refused it.
_CODEMOTION_RE = re.compile(r"IDO52_SKIP_PASSES\s*=\s*\S*codemotion")


def codemotion_violation(env: dict) -> bool:
    """True when a parsed @CompileEnv dict enables the banned codemotion skip."""
    v = (env or {}).get("IDO52_SKIP_PASSES", "")
    return "codemotion" in str(v)


def count_banned_in_source(content: str) -> int:
    """Count banned PostProc annotations in a C source string.

    Lightweight scanner for status reporting — no strict exit.
    Returns the count of banned directive occurrences.
    """
    count = 0
    for line in content.splitlines():
        if "@PostProc:" not in line:
            continue
        for pattern, _, _ in _BANNED_PATTERNS:
            if pattern.search(line):
                count += 1
    return count


# ── Painting-script detection (added 2026-06-10) ────────────────────
# A custom_script that writes hardcoded target instruction words is
# patch_insn_word in disguise. The mass-generated scripts (commit
# 0b83e63f, "batch_fix_close10") all share the PATCHES_BY_POS /
# PATCHES_BY_OFFSET signature: a dict of {position: target_word}
# struct.pack_into'd over .text. Name-based banning cannot catch
# these, so consumers content-scan the referenced script.

_PAINTING_SIGNATURES = (
    "PATCHES_BY_POS",
    "PATCHES_BY_OFFSET",
    "Writing full target instruction words",
    "write_insn_at_offset",
)

# A script that READS the target binary and copies its instruction words into
# the compiled output is the purest form of painting (copying the answer).
_TARGET_COPY_SIGNATURES = (
    "target_words[",
    "target_data[",
)

# ── Opcode / mnemonic tables for the 3-tuple PATCHES op-change check ──
# Primary-opcode for I/J-type mnemonics, and SPECIAL (op==0) function codes.
_MNEM_OPCODE = {
    "lui": 0x0F, "addiu": 0x09, "addi": 0x08, "ori": 0x0D, "andi": 0x0C,
    "xori": 0x0E, "slti": 0x0A, "sltiu": 0x0B, "beq": 0x04, "bne": 0x05,
    "blez": 0x06, "bgtz": 0x07, "lb": 0x20, "lh": 0x21, "lwl": 0x22,
    "lw": 0x23, "lbu": 0x24, "lhu": 0x25, "lwr": 0x26, "sb": 0x28,
    "sh": 0x29, "swl": 0x2A, "sw": 0x2B, "swr": 0x2E, "j": 0x02, "jal": 0x03,
    "cop2": 0x12, "lwc2": 0x32, "swc2": 0x3A,
}
_MNEM_SPECIAL = {
    "sll": 0x00, "srl": 0x02, "sra": 0x03, "sllv": 0x04, "srlv": 0x06,
    "srav": 0x07, "jr": 0x08, "jalr": 0x09, "syscall": 0x0C, "break": 0x0D,
    "mfhi": 0x10, "mthi": 0x11, "mflo": 0x12, "mtlo": 0x13, "mult": 0x18,
    "multu": 0x19, "div": 0x1A, "divu": 0x1B, "add": 0x20, "addu": 0x21,
    "sub": 0x22, "subu": 0x23, "and": 0x24, "or": 0x25, "xor": 0x26,
    "nor": 0x27, "slt": 0x2A, "sltu": 0x2B, "movz": 0x0A, "movn": 0x0B,
    "nop": 0x00, "move": 0x21,  # move = addu/or alias; treat as ALU
}
# REGIMM (op==1: bltz/bgez/...) — these encode the sub-op in the rt field.
_MNEM_REGIMM = {"bltz", "bgez", "bltzal", "bgezal", "bgezl", "bltzl"}


def _word_op_signature(w: int):
    """Return a comparable opcode signature for a 32-bit instruction word.

    ('op', primary)            for I/J-type
    ('special', funct)         for op==0 R-type
    ('regimm',)                for op==1
    """
    op = w >> 26
    if op == 0x00:
        return ("special", w & 0x3F)
    if op == 0x01:
        return ("regimm",)
    return ("op", op)


def _mnem_op_signature(mnem: str):
    """Return the opcode signature implied by a mnemonic (the new insn's
    leading token, or the `[was X]` old token). None if unknown."""
    m = mnem.lower()
    if m in _MNEM_REGIMM:
        return ("regimm",)
    if m in _MNEM_SPECIAL:
        # move/nop etc. share the SPECIAL space; for op-change purposes we
        # only care that BOTH map to ('special', ...). Use ('special', None)
        # as a wildcard so an addu-family alias never spuriously "op-changes".
        return ("special", None)
    if m in _MNEM_OPCODE:
        return ("op", _MNEM_OPCODE[m])
    return None


def _patches_op_change_or_nop_paint(text: str) -> bool:
    """Detect the 3-tuple PATCHES painting form:

        PATCHES = [(0xOFFSET, 0xHARDCODED_WORD, "newmnem ... [was OLDMNEM ...]"), ...]

    This is BANNED painting when, for any row:
      * the hardcoded word's OPERATION differs from the `[was OLDMNEM]`
        operation (e.g. lw→sltiu, sw→sh, jal→sb, nop→real-insn), OR
      * the hardcoded word is 0x00000000 (a nop) overwriting a non-nop
        `[was OLDMNEM]` (nop-painting a live instruction away).

    A LEGIT register-swap PATCHES list keeps the SAME operation in every row
    (only the register/imm fields move), so it never trips this. The `[was X]`
    comment is authoritative: it records what the compiler actually emitted.
    """
    if not re.search(r"PATCHES\s*=\s*\[", text):
        return False
    # 3-tuple rows: (0xOFF, 0xWORD, "DESC")
    rows = re.findall(
        r"\(\s*0x[0-9a-fA-F]+\s*,\s*(0x[0-9a-fA-F]{8})\s*,\s*['\"]([^'\"]*)['\"]",
        text,
    )
    for word_s, desc in rows:
        w = int(word_s, 16)
        was = re.search(r"\[was\s+([A-Za-z][A-Za-z0-9.]*)", desc)
        if not was:
            continue
        old_mnem = was.group(1)
        old_sig = _mnem_op_signature(old_mnem)
        if old_sig is None:
            continue
        # nop-painting a live instruction: new word is a bare nop, old wasn't.
        if w == 0x00000000 and old_mnem.lower() != "nop":
            return True
        new_sig = _word_op_signature(w)
        # Wildcard SPECIAL match: if both are SPECIAL we don't treat differing
        # funct as an op-change (register-swap can shift e.g. or↔addu via the
        # `move` alias); only a SPACE change (op vs special vs regimm) counts.
        if old_sig[0] == "special" and new_sig[0] == "special":
            continue
        if old_sig[0] != new_sig[0]:
            return True
        if old_sig[0] == "op" and new_sig[0] == "op" and old_sig[1] != new_sig[1]:
            return True
    return False


def _is_real_insn_word(w: int) -> bool:
    """A 32-bit literal that looks like a genuine, non-trivial MIPS instruction
    word (not a field mask, not a RAM address, not a bare nop). Used to decide
    whether a hardcoded literal written into .text is a pasted instruction."""
    if w == 0x00000000:
        return False
    # RAM address (0x80xxxxxx) or a low/high field mask — not an instruction.
    if (w >> 24) == 0x80:
        return False
    if (w & 0xFFFF) == 0xFFFF or (w >> 16) == 0xFFFF or (w & 0xFFFF) == 0x0000:
        # masks like 0x0000FFFF / 0xFFFF0000 / a lui-with-zero-imm
        return False
    return True


def _word_pair_paint(text: str) -> bool:
    """Detect the (compiled_old, target_new) word-pair painting form used by the
    instruction-by-instruction patchers (custom_VALEN / BUNKAKEN func_80134408
    family) and the {offset: target_word} dict form (BUNKAKEN 801406BC/8013E2C8).

    For an (old, new) pair the NEW word is the target's answer; it is painting
    when the operation changes (move→beq, sh-base-reg→…, bne→nop) — i.e. the
    same op-change / nop-paint-live test used for PATCHES, but the OLD word is a
    literal next to the NEW one (no `[was X]` comment needed).

    For a {offset: word} dict the lone hardcoded word is treated like a PATCHES
    entry: a real load/store/lui/branch insn word pasted at a position.
    """
    # (1) (0xOLD, 0xNEW) pairs: two adjacent 8-hex literals in a tuple.
    for old_s, new_s in re.findall(
        r"\(\s*(0x[0-9a-fA-F]{8})\s*,\s*(0x[0-9a-fA-F]{8})\s*\)", text
    ):
        old = int(old_s, 16)
        new = int(new_s, 16)
        if old == new:
            continue
        # nop-painting a live instruction
        if new == 0x00000000 and old != 0x00000000:
            return True
        old_sig = _word_op_signature(old)
        new_sig = _word_op_signature(new)
        # SPECIAL↔SPECIAL differing funct is a register/alias move — not an
        # op-change for our purposes.
        if old_sig[0] == "special" and new_sig[0] == "special":
            continue
        if old_sig[0] != new_sig[0]:
            return True
        if old_sig[0] == "op" and old_sig[1] != new_sig[1]:
            return True
    # (2) {offset: target_word} dict entries: `0xOFF: 0xWORD,`
    for off_s, word_s in re.findall(
        r"(0x[0-9a-fA-F]+|\d+)\s*:\s*(0x[0-9a-fA-F]{8})\s*,", text
    ):
        w = int(word_s, 16)
        op = w >> 26
        if not _is_real_insn_word(w):
            continue
        # Only count clear instruction shapes to stay FP-safe: lui, loads/stores,
        # branches (beq/bne/blez/bgtz), and jumps (j/jal).
        if op == 0x0F or 0x20 <= op < 0x30 or 0x02 <= op <= 0x07:
            return True
    return False


_MNEM_NAME_TOKENS = (
    set(_MNEM_OPCODE) | set(_MNEM_SPECIAL) | _MNEM_REGIMM
)


def _named_insn_constant_paint(text: str) -> bool:
    """Detect DESCRIPTIVELY-NAMED full-instruction-word constants pasted into
    .text:

        XORI_V0_V0_1  = 0x38420001   # xori $v0, $v0, 1
        BEQ_V0_ZERO_7 = 0x10400007   # beq  $v0, $zero, 7
        ADDU_T7_T6_V0 = 0x01C27821   # addu $t7, $t6, $v0
        MOVE_A1_V0    = 0x00402825   # move $a1, $v0  (inserted)

    The tell — distinct from any legit fixup — is a constant whose NAME spells
    out the exact target mnemonic + register operands (`<MNEM>_<REG>_<REG>...`),
    assigned a real instruction word, in a script that writes words into .text.
    A legit register/frame patcher names things generically (FRAME_DELTA,
    NEW_SP) and READS+masks the compiled word; it never hardcodes a whole insn
    under its assembly name. Requiring the mnemonic-prefixed name keeps this
    FP-safe (it will NOT fire on `insns[N]=0xWORD` frame/move repaints, which
    have no descriptive constant name)."""
    writes = bool(
        re.search(r"pack_into|struct\.pack\(", text)
        or re.search(r"\b(insns|words|code)\s*\[[^\]]+\]\s*=", text)
    )
    if not writes:
        return False
    for raw in text.splitlines():
        line = raw.split("#", 1)[0].strip()
        m = re.match(r"([A-Z][A-Z0-9_]+)\s*=\s*(0x[0-9a-fA-F]{8})\s*$", line)
        if not m:
            continue
        name, word_s = m.group(1), m.group(2)
        # Name must START with a MIPS mnemonic token followed by `_` (register
        # operands spelled out) — the descriptive-paste signature.
        head = name.split("_", 1)[0].lower()
        if head not in _MNEM_NAME_TOKENS:
            continue
        w = int(word_s, 16)
        if not _is_real_insn_word(w):
            continue
        # The constant must actually be referenced again (not a dead define):
        # at least one further occurrence of the name (it gets packed/passed to
        # a writer helper). The mnemonic-encoding name + matching real insn word
        # is already unambiguous; this guards only against orphan defines.
        if len(re.findall(r"\b" + re.escape(name) + r"\b", text)) >= 2:
            return True
    return False


def _whole_word_slot_paste(text: str) -> bool:
    """Detect 3+ WHOLE-WORD hardcoded slot assignments:

        insns[5]  = 0x11c00006
        insns[23] = 0x24080001
        ...

    A legit register/frame patcher READS+masks (`insns[N] = (insns[N] & MASK) |
    (reg << k)`) so it never assigns a bare hardcoded word; the occasional
    frame patcher that does (1–2 sp-adjust slots) stays under the threshold.
    3+ bare whole-word pastes = the instruction-by-instruction painter
    (custom_VALEN_func_80133AC4 / SetDispMask reorder-paste family)."""
    n = 0
    for m in re.finditer(
        r"(?:^|\n)\s*(?:insns|words|code)\s*\[[^\]]+\]\s*=\s*(0x[0-9a-fA-F]{8})\s*(?:#[^\n]*)?(?:\n|$)",
        text,
    ):
        w = int(m.group(1), 16)
        if _is_real_insn_word(w) or w == 0x00000000:
            n += 1
    return n >= 3


def _constructs_hardcoded_insn(text: str) -> bool:
    """Detect a CONSTRUCTED hardcoded instruction word built from literal
    opcode/register shifts with NO compiled-derived component — e.g.

        lw_at_insn = (0x23 << 26) | (14 << 21) | (1 << 16) | 0x0000

    (lw $at,0($t6) assembled entirely from literals, then pack_into'd). This
    is patch_insn_word in disguise: the word is the target's answer, not a
    transform of the compiled word.

    A LEGIT fixup mixes in a compiled-derived term (`(insn2 & 0xFFFF)`,
    `old & ~0x1f`, a variable, etc.) — so a load/store/lui/branch built
    PURELY from integer literals is the tell. We require the opcode field
    itself to be a literal `(0xNN << 26)` to avoid matching mask constants."""
    for raw in text.splitlines():
        line = raw.split("#", 1)[0]
        if "(" not in line or "<<" not in line:
            continue
        # Must look like an instruction-word construction: a `<< 26` opcode term.
        op_m = re.search(r"\(\s*(0x[0-9a-fA-F]+|\d+)\s*<<\s*26\s*\)", line)
        if not op_m:
            continue
        # The whole RHS (after '=' if present) must be a pure literal/shift
        # OR-chain: no identifiers feeding bits in (those would be
        # compiled-derived). Strip the LHS name.
        rhs = line.split("=", 1)[1] if "=" in line and "==" not in line else line
        # Tokens that indicate a compiled-derived component: a bare identifier
        # (not a hex/dec literal, not a shift/mask operator). `& 0xFFFF` of a
        # variable, etc. If any alpha identifier appears in the OR-chain RHS,
        # this is a transform, not a pure paste.
        # Remove all numeric literals and operators; if letters remain → derived.
        scrub = re.sub(r"0x[0-9a-fA-F]+|\b\d+\b", "", rhs)
        scrub = re.sub(r"[\s()<>|&^~+\-*/,]", "", scrub)
        if scrub:  # leftover identifier chars → compiled-derived → legit
            continue
        op_val = int(op_m.group(1), 0)
        LUI = 0x0F
        MEM = set(range(0x20, 0x30))
        if op_val == LUI or op_val in MEM:
            return True
    return False


def _literal_field_assembled_word(text: str) -> bool:
    """2026-08-09 detector-gap fix: an instruction word assembled ENTIRELY from
    literals and named register constants and dropped into a slot, with NO
    `<< 26` opcode term (so `_constructs_hardcoded_insn`, which anchors on the
    opcode field, never sees it).  SPECIAL-class words have op == 0, so the
    opcode term is simply absent:

        insns[19] = (T2 << 16) | (T8 << 11) | (16 << 6) | 0x02   # srl t8,t2,16
        move_v0_t3 = (T3 << 21) | (V0 << 11) | 0x25              # move v0,t3

    Tell: an OR-chain of `<< 21|16|11|6` shifts whose every operand is a literal
    or an ALL-CAPS constant (a register name), terminated by a small funct
    literal -- and no term read out of the compiled word."""
    for raw in text.splitlines():
        line = raw.split("#", 1)[0]
        if "<<" not in line or "|" not in line:
            continue
        if "==" in line or re.match(r"\s*(if|elif|assert|while)\b", line):
            continue
        rhs = line.split("=", 1)[1] if re.search(r"=(?!=)", line) else line
        shifts = re.findall(r"<<\s*(\d+)", rhs)
        if len(shifts) < 2 or not set(shifts) & {"21", "16", "11", "6"}:
            continue
        # every shifted operand must be a literal or an ALL-CAPS constant
        operands = re.findall(r"\(\s*([A-Za-z_]\w*|0x[0-9a-fA-F]+|\d+)\s*<<", rhs)
        if not operands:
            continue
        for o in operands:
            if o[0].isalpha() or o[0] == "_":
                if not re.fullmatch(r"[A-Z][A-Z0-9_]*", o):
                    break            # a lowercase/derived variable -> a transform
        else:
            # No compiled-derived term anywhere in the chain: strip literals,
            # ALL-CAPS constants and operators; any residue means a variable
            # read out of the compiled word feeds the result (a transform, not
            # a paste) -- e.g. `(new_word & ~(0x1F << 21)) | (REG_FP << 21)`.
            scrub = re.sub(r"0x[0-9a-fA-F]+|\b\d+\b|\b[A-Z][A-Z0-9_]*\b", "", rhs)
            scrub = re.sub(r"[\s()<>|&^~+\-*/,\[\]]", "", scrub)
            if not scrub:
                return True
    return False


def _writes_hardcoded_insn(text: str) -> bool:
    """Generic painting detector (2026-06-22): the script WRITES a hardcoded
    instruction word the compiler never produced — a `lui` with a real %hi, or a
    load/store with a real offset, or an inserted op. Catches the bare-list /
    single-`pack_into` idiom that the PATCHES=[(off,word)] check missed (the
    func_80138594 / TACO cross-prologue painting family).

    Distinguished from LEGIT register repaints, which READ+transform existing
    words: a legit script's literal appears in an `==`/`if` guard, or is a
    low16==0 field mask, or is a branch/arith opcode (not lui/load/store)."""
    LUI = 0x0F
    MEM = set(range(0x20, 0x30))  # lb..swr loads and stores
    for raw in text.splitlines():
        line = raw.split("#", 1)[0]
        if not line.strip():
            continue
        if "==" in line or re.search(r"\bif\b", line):
            continue  # a guard / read of the compiled word, not a paste
        write_ctx = bool(
            re.search(r"pack_into|struct\.pack|=\s*\[|=\s*0x|\.append\(|\]\s*=", line)
        ) or bool(re.fullmatch(r"\s*0x[0-9a-fA-F]{8}\s*,?\s*", line))
        if not write_ctx:
            continue
        for m in re.findall(r"0x([0-9a-fA-F]{8})", line):
            w = int(m, 16)
            op = w >> 26
            imm = w & 0xFFFF
            if imm == 0:
                continue  # field mask, lui-with-no-lo, or nop — not a paste
            if op == LUI or op in MEM:
                return True
    return False


def _compiled_derived_names(text: str) -> set:
    """Names bound to a field EXTRACTED from a compiled instruction word.

    `opcode = (word >> 26) & 0x3F` / `op = insn >> 26` / `rs = (w >> 21) & 0x1f`.
    A `(opcode << 26) | ...` rebuild using such a name is a field-preserving
    re-encode (the allowed `swap_registers` / `patch_insn_reg_field` class), not
    an operation change -- see postproc_shadow_file_name.py / remap_tregs_756ac.py.
    """
    names = set()
    for m in re.finditer(
        r"(?:^|\n)\s*(\w+)\s*=\s*\(?\s*\w+\s*>>\s*\d+", text
    ):
        names.add(m.group(1))
    # `def op(i): return (i >> 26) & 0x3f` accessor form
    for m in re.finditer(r"def\s+(\w+)\s*\([^)]*\)\s*:\s*return\s*\(?\s*\w+\s*>>\s*\d+", text):
        names.add(m.group(1))
    return names


def _opcode_field_write(text: str) -> bool:
    """2026-08-01 detector-gap fix (batch-28 audit): a custom script that WRITES
    an opcode field is painting, whether it constructs a word from scratch
    (`(15 << 26) | (rt << 16) | imm` — a parameterized fabricator helper the
    pure-literal check above skips) or swaps the opcode of a compiled word
    (`(insn & 0x03FFFFFF) | (op << 26)` — an operation change, e.g.
    slti->sltiu, bne->beq). The ONLY legit uses of the opcode field in a
    custom script are READS (`w >> 26`) and mask builds (`0x3F << 26`), which
    appear in guards. Catches custom_80048B70 / custom_set_256_mode /
    custom_func_80078CE0.patch_opcode.

    2026-08-09 gap fix: the operand may be PARENTHESISED
    (`((new_opcode & 0x3F) << 26)` in custom_event_face2.patch_opcode) -- the old
    regex demanded the mask immediately after a bare token and so missed it.
    Precision fix in the same pass: an operand NAME that was extracted from the
    compiled word (`opcode = (word >> 26) & 0x3F`) is a field-preserving
    re-encode, not an operation change (postproc_shadow_file_name.py,
    remap_tregs_756ac.py) -- those are no longer flagged here."""
    derived = _compiled_derived_names(text)
    for raw in text.splitlines():
        line = raw.split("#", 1)[0]
        if "<< 26" not in line and "<<26" not in line:
            continue
        if "==" in line or "!=" in line or re.match(r"\s*(if|elif|assert)\b", line):
            continue  # guard/read context
        # Operand forms: bare literal / bare name / parenthesised expression,
        # each optionally masked with 0x3f before the shift.
        m = re.search(
            r"\(\s*(?:"
            r"\(\s*(?P<pname>\w+)\s*&\s*0x3[fF]\s*\)"      # ((name & 0x3f)
            r"|0x(?P<hex>[0-9a-fA-F]+)"                    # (0xNN
            r"|(?P<dec>\d+)"                               # (NN
            r"|(?P<name>\w+)"                              # (name
            r")\s*(?:&\s*0x3[fF]\s*)?<<\s*26\s*\)",
            line,
        )
        if not m:
            continue
        lit = m.group("hex") or m.group("dec")
        if lit is not None and int(lit, 16 if m.group("hex") else 10) == 0x3F:
            continue  # full-field mask build, a read helper
        operand = m.group("pname") or m.group("name")
        if lit is None and operand in derived:
            continue  # re-encode of the compiled word's own opcode
        # write context: assignment, return, or call argument
        if re.search(r"=(?!=)|\breturn\b|\w+\s*\([^)]*<<\s*26", line):
            return True
    return False


def _hardcoded_insn_call_arg(text: str) -> bool:
    """2026-08-01 detector-gap fix: a hardcoded instruction word smuggled in
    as a CALL ARGUMENT — `set_insn(elf, off, i, 0x27BD0030)` — which the
    assignment-context regex in _writes_hardcoded_insn never sees. Flag any
    call whose name smells like an instruction write and whose argument is an
    opcode-shaped 8-hex literal (not a RAM address 0x80*, not a field mask,
    imm16 != 0). Catches custom_func_80078CE0's set_insn pastes."""
    for raw in text.splitlines():
        line = raw.split("#", 1)[0]
        if "==" in line or re.match(r"\s*(if|elif|assert)\b", line):
            continue
        for m in re.finditer(
            r"\b(\w*(?:insn|word|patch|emit|put|store)\w*)\s*\([^)]*\b0x([0-9a-fA-F]{8})\b",
            line, re.IGNORECASE,
        ):
            w = int(m.group(2), 16)
            imm = w & 0xFFFF
            if (w >> 24) == 0x80 or imm == 0 or (w & 0xFFFF0000) in (0xFFFF0000, 0):
                continue  # address / mask / lui-no-lo
            return True
    return False


def _deletes_or_inserts_insns(text: str) -> bool:
    """2026-08-01 detector-gap fix: instruction DELETION or INSERTION with a
    shift — fabrication by subtraction/addition regardless of how the words
    are spelled. Three shipped idioms: (a) byte-shift removal
    (`remove_offset` + copying the tail down by 4), (b) rebuilding the
    instruction list while skipping entries (`out_insns` built under a
    conditional `continue`), (c) list surgery on the insn array
    (del/pop/insert). Catches custom_func_800733A4 sisters,
    custom_func_8007AC78, custom_func_800412D8.strip_arg_homes."""
    if re.search(r"\bremove_offset\b", text):
        return True
    if re.search(r"\b(?:del\s+\w*insns?\[|\w*insns?\.pop\(|\w*insns?\.insert\()", text):
        return True
    if re.search(r"\bout_insns?\b", text):
        # rebuilt stream: fabrication when entries can be skipped or reordered
        if re.search(r"\bcontinue\b|\bskip", text, re.IGNORECASE):
            return True
    if re.search(r"(?:remove|delete|drop|strip)\w*\s+(?:the\s+)?(?:sw|lw|mid|instruction|insn)",
                 text, re.IGNORECASE) and re.search(r"[+\-]\s*4\b", text):
        return True
    # ── 2026-08-09 gap fixes (main-binary custom_script audit) ──
    # (d) helper/constant names that ARE the deletion, whatever the casing:
    #     REMOVE_OFF / delete_insn_at() / shrink_text() / strip_arg_homes --shrink
    if re.search(r"\bREMOVE_OFF\w*\b|\bdelete_insn_at\b|\bshrink_text\b"
                 r"|\btruncate_text_section\b", text):
        return True
    # (d2) the shift-delete loop: `for i in range(K, n-1): insns[i] = insns[i+1]`
    #      followed by a decrement of the count (custom_func_8009AE74).
    if re.search(r"(\w+)\s*\[\s*(\w+)\s*\]\s*=\s*\1\s*\[\s*\2\s*\+\s*1\s*\]", text):
        return True
    if re.search(r"strip_arg_homes", text) and "--shrink" in text:
        return True
    # (e) slice-concat deletion: `insns[:14] + insns[15:]` / `c[:2] + c[3:]`
    if re.search(r"(\w+)\s*\[\s*:\s*(\d+)\s*\]\s*\+\s*\1\s*\[\s*(\d+)\s*:", text):
        return True
    # (f) `.pop(N)` / `del name[N]` on ANY list (not just *insns*), when the
    #     script also writes .text -- the 8009FE30/80090D40 family.
    if re.search(r"\b\w+\.pop\(\s*\d+\s*\)|\bdel\s+\w+\[\s*\d+", text) and \
            re.search(r"pack_into|struct\.pack", text):
        return True
    # (g) an explicit skip marker in the emit stream: `# SKIP c[7]`,
    #     `output_order.append(-1)` (a fabricated nop slot).
    if re.search(r"#\s*SKIP\s+\w+\[", text):
        return True
    if re.search(r"\boutput_order\.append\(\s*-1\s*\)", text):
        return True
    # (h) the section-size rewrite that is the *signature* of a count change:
    #     packing a NEW .text size derived from a delta rather than a constant.
    if re.search(r"\btext_growth\b|\btext_delta\b|\bnew_text_size\s*-\s*text_size\b",
                 text):
        return True
    return False


# Opcodes that a genuine MIPS I instruction word can carry. Used to decide
# whether an 8-hex literal being WRITTEN into .text is a pasted instruction.
_VALID_PRIMARY_OPS = (
    {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
     0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
     0x10, 0x11, 0x12, 0x13}
    | set(range(0x20, 0x2F))
    | {0x30, 0x31, 0x32, 0x33, 0x38, 0x39, 0x3A, 0x3B}
)

# Call names that are unambiguously "write this word into .text". Broadened
# 2026-08-09: the audit found pastes smuggled through one-line local helpers
# named `add`, `emit`, `put`, `W`, `s`, `patch`, `write_u32`, `set_instr`,
# `write_instr`, `write_word`, `set_ins` -- none of which matched the old
# insn|word|patch|emit|put|store name filter.
_WRITER_CALL_RE = re.compile(
    r"\b("
    r"\w*(?:insn|instr|word|emit|patch|put|store|write|set_ins|set_instr)\w*"
    r"|add|emit|put|s|W|T|p32|w32|wr|wr32"
    r")\s*\(",
    re.IGNORECASE,
)


def _is_pasted_insn_literal(w: int) -> bool:
    """A 32-bit literal that is plausibly a whole MIPS instruction word being
    pasted (as opposed to a mask, a RAM address, an ELF constant, or zero)."""
    if w == 0 or w > 0xFFFFFFFF:
        return False
    if (w >> 24) == 0x80:
        return False                      # RAM address
    if w in (0xFFFFFFFF, 0x0000FFFF, 0xFFFF0000, 0x03FFFFFF, 0xFC000000):
        return False                      # field masks
    if (w & 0xFFFF) == 0xFFFF and (w >> 16) in (0x0000, 0xFFFF):
        return False
    return (w >> 26) in _VALID_PRIMARY_OPS


def _pasted_word_write(text: str) -> bool:
    """2026-08-09 detector-gap fix (main-binary custom_script audit).

    The generic form the previous checks all missed: a hardcoded full
    instruction word reaching `.text` through ANY spelling --

        add(0x3c13801f)                 # custom_func_80061B3C
        emit(23, 0x02401021)            # custom_func_8009B3C0
        write_u32(data, w81_off, 0x00008025)   # custom_func_8004D3E8
        set_instr(59, 0x3086ffff)       # custom_func_8004FAD0
        out[7] = 0x2C41000F             # custom_func_8005454C
        new_insns[16] = (0x25F80200, None)     # custom_func_800556BC_v2
        new.append(0x27bdffc0)          # custom_func_8009E62C
        new12 = 0x8C630000 | 0xBD6C ; set_ins(12, new12)  # postproc_func_8009FDD8

    `_writes_hardcoded_insn` only fired on lui/load/store opcodes in an
    assignment context, so every pasted addiu / sltiu / ori / branch / R-type
    slipped through, as did every bare call-argument paste.

    Guarded for precision:
      * the script must actually write .text (`pack_into` / `struct.pack` /
        a slice assignment into `data`);
      * comparison / assertion lines are skipped (a literal in an `==` guard is
        a READ of the compiled word, which is exactly what a legit script does);
      * the literal must decode to a plausible instruction (`_is_pasted_insn_literal`).
    """
    if not re.search(r"pack_into|struct\.pack|\bdata\s*\[[^\]]*\]\s*=", text):
        return False

    # Names bound to a hardcoded instruction word, so `x = 0x...; write(x)`
    # is caught too (postproc_func_8009FDD8 / func_80068CA8_postproc).
    literal_names = set()
    for m in re.finditer(
        r"(?:^|\n)\s*([A-Za-z_]\w*)\s*=\s*0x([0-9a-fA-F]{8})\s*(?:\|[^\n=]*)?(?:#[^\n]*)?$",
        text, re.MULTILINE,
    ):
        if _is_pasted_insn_literal(int(m.group(2), 16)):
            literal_names.add(m.group(1))

    for raw in text.splitlines():
        line = raw.split("#", 1)[0]
        if not line.strip():
            continue
        if "==" in line or "!=" in line or re.match(r"\s*(if|elif|assert|while)\b", line):
            continue                      # guard / read of the compiled word
        if re.search(r"unpack|\.index\(|\.find\(|range\(", line):
            continue                      # ELF parsing, not a .text write

        # (1) a named hardcoded word handed to a writer call
        if literal_names and _WRITER_CALL_RE.search(line):
            for n in literal_names:
                if re.search(r"\b" + re.escape(n) + r"\b", line):
                    return True

        for hx in re.findall(r"0x([0-9a-fA-F]{8})\b", line):
            w = int(hx, 16)
            if not _is_pasted_insn_literal(w):
                continue
            # (2) slot assignment to ANY list: `out[7] = 0x...`,
            #     `new_insns[16] = (0x..., None)`
            if re.search(r"\w+\s*\[[^\]]+\]\s*=\s*\(?\s*0x" + hx, line, re.IGNORECASE):
                return True
            # (3) appended to an emit list: `new.append(0x...)`
            if re.search(r"\.append\(\s*\(?\s*0x" + hx, line, re.IGNORECASE):
                return True
            # (4) a bare call-argument paste into a writer helper
            if _WRITER_CALL_RE.search(line):
                return True
            # (5) direct struct.pack of the literal
            if re.search(r"pack(?:_into)?\s*\([^)]*0x" + hx, line, re.IGNORECASE):
                return True
    return False


_ASM_MNEMONIC_LINE_RE = re.compile(
    r"""[\\]t|\t"""
    r"""(?:addiu|addu|subu|and|andi|or|ori|xor|xori|nor|sll|srl|sra|slt|sltu"""
    r"""|slti|sltiu|lui|lw|lh|lhu|lb|lbu|sw|sh|sb|beq|bne|beqz|bnez|blez|bgtz"""
    r"""|bgez|bltz|j|jal|jr|jalr|mult|multu|div|divu|mfhi|mflo|move|li|nop|b)\b"""
)


def _handwritten_asm_replacement(text: str) -> bool:
    """2026-08-09 detector-gap fix: a custom_script whose `transform()` IGNORES
    the compiler output and returns a literal assembly listing that is then
    assembled into `.text`.  CLAUDE.md bans this by name: "custom_script that
    replaces the .text section -- same as hand-writing ASM".

    Two shipped spellings:
      * `return r\"\"\"\\t.set noreorder ... \"\"\".split('\\n')`
        (asm_intercept_func_8008EB8C.py -- 234 hand-written instructions)
      * `result.append('\\taddiu\\t$sp, $sp, -80')` line by line
        (transform_func_8008bee8.py)
    """
    # (a) a triple-quoted block that contains MIPS mnemonics and a label/directive
    for m in re.finditer(r'r?"""(.*?)"""', text, re.DOTALL):
        body = m.group(1)
        if ".set noreorder" in body or re.search(r"^\s*\.ent\b", body, re.MULTILINE):
            if len(_ASM_MNEMONIC_LINE_RE.findall(body)) >= 4:
                return True
    # (b) line-by-line emission of literal assembly
    appends = re.findall(r"\.append\(\s*(['\"])(\\t[^'\"]*)\1", text)
    n = sum(1 for _, s in appends if _ASM_MNEMONIC_LINE_RE.search(s))
    if n >= 4:
        return True
    # (c) an asm-text transform that DROPS instruction lines -- the
    #     transform_fallthrough.py shape: match `jr\t$ra` and `continue`
    #     without appending, deleting a return the compiler did emit.
    #     Fabrication by subtraction, the same family as the purged head stubs.
    if re.search(r"['\"](?:\\t)?jr\\t\$(?:ra|31)['\"]", text) or \
       re.search(r"\.startswith\(\s*['\"](?:\\t)?(?:jr|jal|b|beq|bne)\b", text):
        if re.search(r"^\s*continue\s*$", text, re.MULTILINE):
            return True
    return False


def _nop_paint_over_live(text: str) -> bool:
    """2026-08-09 detector-gap fix: writing a NOP into a slot that the compiler
    filled with a real instruction.  A nop IS `sll $zero,$zero,0`, so painting
    one is fabrication exactly like painting any other word -- CLAUDE.md calls
    out "`nop` -> real insn" and the reverse is the same act.

    Spellings found in the audit:
        W(27, 0x00000000)                    # custom_func_800521DC
        new_words.append(0x00000000)         # custom_GsTMDfastTNF3
        new_insns = [ ..., NOP, ... ]        # custom_func_800414A0
        emit(NOP)                            # custom_func_80072A0C

    The ONE allowed nop write is zeroing a verified-dead TRAILING nop or
    appending trailing padding, so a line (or its comment) that says
    trailing/pad/align is not flagged.
    """
    if not re.search(r"pack_into|struct\.pack|\bdata\s*\[[^\]]*\]\s*=", text):
        return False
    nop_names = set()
    for m in re.finditer(r"(?:^|\n)\s*([A-Z_][A-Z0-9_]*)\s*=\s*0x0{8}\b", text):
        nop_names.add(m.group(1))
    tokens = [r"0x0{8}"] + [re.escape(n) for n in nop_names]
    tok = "(?:" + "|".join(tokens) + ")"
    for raw in text.splitlines():
        low = raw.lower()
        if "trailing" in low or "pad" in low or "align" in low:
            continue
        line = raw.split("#", 1)[0]
        if not line.strip():
            continue
        if "==" in line or "!=" in line or re.match(r"\s*(if|elif|assert|while)\b", line):
            continue
        if re.match(r"\s*[A-Z_][A-Z0-9_]*\s*=\s*0x0{8}\s*$", line):
            continue                     # the constant definition itself
        if re.search(r"\.append\(\s*" + tok + r"\s*\)", line):
            return True
        if re.search(r"\w+\s*\[[^\]]+\]\s*=\s*" + tok + r"\b", line):
            return True
        if re.search(r"^\s*" + tok + r"\s*,\s*$", line):
            return True                  # a NOP element inside a rebuilt list
        if _WRITER_CALL_RE.search(line) and re.search(
                r"\(\s*[^)]*,\s*" + tok + r"\s*\)", line):
            return True
    return False


def _rebuilt_text_resize(text: str) -> bool:
    """2026-08-09 detector-gap fix: the script rebuilds `.text` from a Python
    list and writes a NEW section size derived from that list's LENGTH.

    `new_text_size = len(result) * 4` + `pack_into('<I', data, text_sh + 0x14,
    new_text_size)` means the emitted instruction COUNT differs from what the
    compiler produced -- the CLAUDE.md audit test ("if deleting all PostProc
    produces the wrong instruction count, the function is unmatchable from C").
    Catches custom_SsUtAutoPan (36->35), custom_GsTMDfastTNF3 (37->38).
    A trailing-nop strip computes its size from a counter, not from a rebuilt
    list, so it does not trip this."""
    if not re.search(r"\blen\s*\(\s*\w+\s*\)\s*\*\s*4\b", text):
        if not re.search(r"\bgrowth\b|\bnew_size\s*-\s*text_size\b", text):
            return False
    return bool(re.search(
        r"pack_into\s*\(\s*['\"]<I['\"]\s*,\s*\w+\s*,\s*\w+\s*\+\s*(?:0x14|20)\b", text
    ) or re.search(r"\bgrow(?:th)?\b.*\.text|\.text.*\bgrow", text, re.IGNORECASE))


def _lambda_word_paint(text: str) -> bool:
    """2026-08-09 detector-gap fix: a hardcoded word returned by a lambda in a
    patch table -- `(0x7c, lambda w: 0x24020001)` (postproc_func_8006EF68).
    The lambda ignores its `w` argument entirely, so this is patch_insn_word."""
    for m in re.finditer(r"lambda\s+\w*\s*:\s*(0x[0-9a-fA-F]{8})\b", text):
        if _is_pasted_insn_literal(int(m.group(1), 16)):
            return True
        if int(m.group(1), 16) == 0:
            return True
    return False


def _referenced_transform_paints(script_path, text: str, _depth: int = 0) -> bool:
    """2026-08-09 detector-gap fix: `asm_intercept` wrappers keep the painting
    in a SEPARATE transform module (`TRANSFORM = os.path.join(SCRIPT_DIR,
    "transform_func_8008bee8.py")`).  The wrapper itself is clean, so the
    content scan must follow the reference one level."""
    if _depth > 1:
        return False
    from pathlib import Path
    here = Path(script_path).resolve().parent
    for m in re.finditer(r"['\"](transform_[\w.]+?)\.py['\"]", text):
        cand = here / (m.group(1) + ".py")
        if cand.exists() and str(cand) != str(script_path):
            if is_painting_script(cand):
                return True
    return False


def _banned_directive_via_subprocess(text: str) -> bool:
    """2026-08-09 detector-gap fix: a BANNED directive smuggled into a
    custom_script's subprocess argv, where the name-based
    `check_banned_postproc` (which only scans `@PostProc:` lines) cannot see it.

    `subprocess.run([... prepend_thunk.py, obj, out, '--insns', '0x3C0E800D',
    '0x8DCEFC30'])` is `insert_instruction_and_shift` + `patch_insn_word`
    combined -- asm_intercept_SsUtKeyOn.py, custom_func_80059E48.py.
    """
    for name, _ in BANNED_DIRECTIVES:
        if re.search(r"['\"]?\b" + re.escape(name) + r"(?:\.py)?\b", text):
            return True
    if re.search(r"['\"]--insns['\"]", text):
        return True
    return False


_painting_cache: dict = {}


def is_painting_script(script_path) -> bool:
    """Return True if the custom_script at script_path writes hardcoded
    instruction words (banned patch_insn_word semantics).

    Detects two idioms: (1) the named signatures above; (2) a script that
    struct-packs a list of hardcoded 32-bit *instruction* words (not RAM
    addresses, which start 0x80) into the object — the generic painting shape.

    Results are cached per path. Unreadable/missing scripts return False.
    """
    key = str(script_path)
    if key in _painting_cache:
        return _painting_cache[key]
    verdict = False
    try:
        with open(script_path, "r", encoding="utf-8", errors="ignore") as f:
            text = f.read(65536)
        if any(sig in text for sig in _PAINTING_SIGNATURES):
            verdict = True
        elif any(sig in text for sig in _TARGET_COPY_SIGNATURES):
            verdict = True
        else:
            # Precise: a PATCHES list whose entries pair an offset with a full
            # hardcoded instruction word (the target's answer), NOT a mask or
            # address. Legit reorder/reg-patch scripts READ the compiled word
            # and transform it; painting scripts hardcode the target word.
            # Signature: a `(0xOFF, 0xWORD` tuple where WORD is an opcode-shaped
            # 8-hex literal that is not a mask (0xFFFF*/*FFFF/0x0000*) nor a
            # RAM address (0x80*).
            if re.search(r"PATCHES\s*=\s*\[", text):
                # 2026-08-09: the index may be DECIMAL, not hex
                # (return_schedule_regs.py: `PATCHES = [(249, 0x0058c821), ...]`)
                tuples = re.findall(
                    r"\(\s*(?:0x[0-9a-fA-F]+|\d+)\s*,\s*(0x[0-9a-fA-F]{8})", text)
                def _is_insn_word(w):
                    wl = w.lower()
                    return not (wl.startswith("0x80") or wl.endswith("ffff")
                                or wl.startswith("0xffff") or wl.startswith("0x0000"))
                if sum(1 for w in tuples if _is_insn_word(w)) >= 2:
                    verdict = True
            # 3-tuple PATCHES op-change / nop-paint form (patch_name_ent_77b8
            # family): a single hardcoded op-changing or live-insn-nopping row
            # is painting even when the >=2-insn-word heuristic above misses it
            # (e.g. one sltiu + several nops over live loads/stores).
            if not verdict:
                verdict = _patches_op_change_or_nop_paint(text)
            # (old_word, new_word) pair-mapping and {offset: word} dict forms
            # (custom_VALEN / BUNKAKEN func_80134408 / 801406BC / 8013E2C8).
            if not verdict:
                verdict = _word_pair_paint(text)
            # NAMED full-instruction-word constants / insns[N]=0xWORD pastes
            # (patch_valen_3a00, unfold_const_one_6798 families).
            if not verdict:
                verdict = _named_insn_constant_paint(text)
            # 3+ whole-word hardcoded slot pastes (custom_VALEN_func_80133AC4 /
            # SetDispMask instruction-by-instruction reorder-paste).
            if not verdict:
                verdict = _whole_word_slot_paste(text)
            # Constructed fully-literal instruction word
            # (custom_volatile_load_getter family).
            if not verdict:
                verdict = _constructs_hardcoded_insn(text)
            if not verdict:
                verdict = _writes_hardcoded_insn(text)
            # 2026-08-01 gap fixes: opcode-field writes (constructor helpers,
            # operation changes), hardcoded words as call arguments, and
            # instruction deletion/insertion with shift.
            if not verdict:
                verdict = _opcode_field_write(text)
            if not verdict:
                verdict = _hardcoded_insn_call_arg(text)
            if not verdict:
                verdict = _deletes_or_inserts_insns(text)
            # 2026-08-09 gap fixes (main-binary custom_script audit): a pasted
            # instruction word in ANY spelling, a hand-written .text
            # replacement, and a banned directive smuggled through subprocess.
            if not verdict:
                verdict = _pasted_word_write(text)
            if not verdict:
                verdict = _handwritten_asm_replacement(text)
            if not verdict:
                verdict = _banned_directive_via_subprocess(text)
            if not verdict:
                verdict = _nop_paint_over_live(text)
            if not verdict:
                verdict = _rebuilt_text_resize(text)
            if not verdict:
                verdict = _lambda_word_paint(text)
            if not verdict:
                verdict = _literal_field_assembled_word(text)
            if not verdict:
                verdict = _referenced_transform_paints(script_path, text)
    except Exception:
        verdict = False
    _painting_cache[key] = verdict
    return verdict


# ── RESTRICTED directives: BANNED for new use, current users grandfathered ──
# Added 2026-09-27.  Each of these
# was confirmed by READING the tool (the count test cannot see any of them:
# every one rewrites in place).  They are banned outright for any function
# not already using them; the functions that use them today are listed, with
# a per-class site count, in postproc_grandfathered.json next to this file.
# That list is a REMEDIATION WORKLIST, not a permission: it may only shrink.
# A grandfathered function that adds MORE sites of a class is refused too.
#
# Two classes carry a sanctioned exception, spelled on the directive line:
#   * nop_at_offset ... --dead-arg-home   (a verified dead argument-home
#     store; the site is then VERIFIED by tools/analysis/
#     audit_plain_vs_shipped.py, which ignores only a provably dead RAW
#     `sw $aN,frame+4N($sp)` and flags every other deleted word).
#   * patch_insn_imm16 ... --expect        (each patch states the old value).
#     NOT honoured yet: overlay_postproc.py converts every patch to
#     `--patch-noverify`, so the old value is never checked.  Until that
#     pipeline passes `--patch OFF:FROM:TO`, the qualifier is ignored and
#     new use stays refused -- see IMM16_EXPECT_ENFORCED.
RESTRICTED_DIRECTIVES: List[Tuple[str, str]] = [
    (
        "fill_delay_slots",
        "COPIES THE RETAIL WORD onto a compiled delay-slot nop "
        "(tools/postproc/fill_delay_slots.py: `tgt_next` is read from the target "
        "EXE and `struct.pack_into(... offset+4, tgt_next)`); nothing is moved "
        "out of the body, so a nop becomes an instruction the compiler never "
        "emitted.  patch_insn_word in disguise.  Fix the C so IDO fills the "
        "slot (a missing argument, a computed return) or file it asm_only.",
    ),
    (
        "swap_spill_with_move",
        "REWRITES a compiled `sw $rT,M($sp)` into `or $rD,$rT,$zero` "
        "(tools/ido/swap_spill_with_move.py builds funct 0x25 over the sw; the "
        "destination register comes from the directive).  Changes the "
        "instruction's OPERATION: the same transform as the banned "
        "propagate_stack_to_reg.  The C spills a value retail keeps in a "
        "register -- usually an unused local or a taken address.",
    ),
    (
        "invert_branch",
        "FLIPS a branch's opcode -- beq<->bne, blez<->bgtz, bltz<->bgez "
        "(tools/ido/invert_branch.py INVERT_OPCODE / INVERT_REGIMM) -- and leaves "
        "the displacement alone, so the shipped predicate is the negation of "
        "the C's.  The PC port compiles that C.  Fix the condition in the C.",
    ),
    (
        "nop_at_offset",
        "OVERWRITES any word with a nop, unconditionally "
        "(tools/ido/nop_at_offset.py packs 0 over whatever is there, with no "
        "check of what it deletes).  The audit found call arguments in jal "
        "delay slots, return-value moves, branches, real stores and whole "
        "blocks deleted this way.  Only a verified DEAD ARGUMENT-HOME STORE may "
        "be deleted: mark it `--dead-arg-home` and let "
        "audit_plain_vs_shipped.py verify it -- or better, use Hook 73 "
        "`@CompileEnv: IDO52_NO_ENTRY_HOME_STORE`, which stops IDO emitting it.",
    ),
    (
        "patch_insn_imm16",
        "WRITES a 16-bit immediate taken from the target.  The overlay pipeline "
        "always passes `--patch-noverify` (overlay_postproc.py drops the middle "
        "field), so the compiled value it replaces is never checked: a stale or "
        "wrong-offset patch silently rewrites a different constant, field "
        "offset or branch.  New use needs `--expect` (the old value) AND a "
        "pipeline that verifies it -- not yet wired (IMM16_EXPECT_ENFORCED).",
    ),
    (
        "adjust_branch_offset",
        "WRITES a branch displacement given on the directive line "
        "(tools/ido/adjust_branch_offset.py packs `--imm` over the branch's "
        "immediate; overlay_postproc.py passes it unchecked).  In practice the "
        "value is read off the retail word, so the shipped branch lands where "
        "retail's does, not where the C's does: audit_plain_vs_shipped.py found "
        "GsMapModelingData's two branches retargeted from the C's label to "
        "retail's (2026-10-03).  Armed 2026-10-03 (red-team finding 3).  Fix "
        "the C's control flow so the compiler emits the displacement.",
    ),
    (
        "convert_ori_to_addiu",
        "REWRITES `ori rt,rs,K` into `addiu rt,rs,K` (same immediate field).  "
        "The two compute the same value only when the low 16 bits of rs are "
        "zero AND K < 0x8000; its docstring's 'equivalent when rs == rt' is "
        "false in general.  main icon_disp_switch: `lui 0x800e; ori 0xe760` "
        "(0x800EE760) shipped as `addiu -0x18a0` (0x800DE760) -- a 64 KiB "
        "different address (2026-10-03).  Armed 2026-10-03 (red-team "
        "finding 3).  Write the address/constant retail uses in the C.",
    ),
    (
        "convert_addiu_to_ori",
        "REWRITES `addiu rt,rs,K` into `ori rt,rs,K`, checking only that the "
        "PREVIOUS word is some `lui`, and decrements that lui's immediate "
        "whether or not it feeds rs (tools/ido/convert_addiu_to_ori.py).  "
        "Unlisted until 2026-10-03; armed then (red-team finding 3).",
    ),
    (
        "convert_addiu_to_slti",
        "REWRITES `addiu rt,$zero,K; bne rs,rt` into `slti rt,rs,K; bne rt,$zero` "
        "(tools/ido/convert_addiu_to_slti.py).  Opcode change, and its docstring's "
        "'semantically identical' is false: the first tests rs != K, the second "
        "rs < K.  Write the comparison retail makes.",
    ),
]

# Pseudo-class: a patch_insn_reg_field / register paint to or from $zero.
# `$zero` is the constant 0, not an allocation (audit_paint_bans.py class 4,
# and gen_reg_patches refuses to emit it): repointing a read off `$zero`
# asserts the register holds 0 there, repointing onto it asserts the reverse.
# This retires CLAUDE.md's old "write the literal + patch_insn_imm16 /
# patch_insn_reg_field back" dispatcher recipe: write `sel + 1` in the C.
ZERO_REG_PAINT = "zero_reg_paint"
_ZERO_REG_PAINT_REASON = (
    "PAINTS A REGISTER FIELD TO OR FROM $zero.  $zero is the constant 0, not a "
    "register allocation (audit_paint_bans.py class 4; gen_reg_patches refuses "
    "to emit it), so the paint changes the value the instruction reads or "
    "stores -- the 'literal + patch_insn_imm16 selector+1' recipe is this.  "
    "Write the expression retail computes (e.g. `sel + 1`) in the C."
)

# Flip to True only once overlay_postproc.py passes `--patch OFF:FROM:TO`
# (verified) for a patch_insn_imm16 line that carries `--expect`.
IMM16_EXPECT_ENFORCED = False

_REPO_ROOT = Path(__file__).resolve().parents[2]
GRANDFATHER_FILE = Path(__file__).resolve().parent / "postproc_grandfathered.json"
_RESTRICTED_NAMES = {n for n, _ in RESTRICTED_DIRECTIVES}
_RESTRICTED_REASON = dict(RESTRICTED_DIRECTIVES)
_RESTRICTED_REASON[ZERO_REG_PAINT] = _ZERO_REG_PAINT_REASON
_QUALIFIER = {"nop_at_offset": "--dead-arg-home", "patch_insn_imm16": "--expect"}
_grandfather_cache = None
_registry_cache = None


def _load_grandfathered() -> dict:
    global _grandfather_cache
    if _grandfather_cache is None:
        try:
            with open(GRANDFATHER_FILE, "r", encoding="utf-8") as f:
                _grandfather_cache = json.load(f).get("classes", {})
        except FileNotFoundError:
            _grandfather_cache = {}
    return _grandfather_cache


def _registry():
    """(name -> (address, source), source -> (address, name)) for main C."""
    global _registry_cache
    if _registry_cache is None:
        byname, bysrc = {}, {}
        try:
            with open(_REPO_ROOT / "config" / "level3_functions.json") as f:
                for e in json.load(f)["functions"]:
                    a = int(e["address"], 16)
                    byname[e["name"]] = (a, e.get("source", ""))
                    bysrc[e.get("source", "")] = (a, e["name"])
        except (OSError, ValueError, KeyError):
            pass
        _registry_cache = (byname, bysrc)
    return _registry_cache


def _overlay_names() -> set:
    d = _REPO_ROOT / "src" / "overlay"
    return {p.name for p in d.iterdir() if p.is_dir()} if d.is_dir() else set()


def _is_overlay_path(p: Path) -> bool:
    """src/overlay/OV/x.c, or (outside the tree, e.g. a worktree or a test
    fixture) any .../OV/x.c whose parent directory is an overlay name."""
    try:
        rel = p.resolve().relative_to(_REPO_ROOT)
        return len(rel.parts) >= 3 and rel.parts[:2] == ("src", "overlay")
    except ValueError:
        return p.parent.name in _overlay_names()


def _identify(ident: str):
    """Map a build/test identity to (ids, source Path or None).

    Overlays are `OV/func_XXXXXXXX`.  Main functions are keyed by ADDRESS
    (`main/0x8005ADDC`), because names are renamed and addresses are not;
    the build passes the registry NAME, the test harnesses pass a PATH."""
    ident = str(ident)
    byname, bysrc = _registry()
    p = Path(ident)
    if p.suffix == ".c":
        try:
            rel = p.resolve().relative_to(_REPO_ROOT)
        except ValueError:
            rel = p
        parts = rel.parts
        if len(parts) >= 3 and parts[0] == "src" and parts[1] == "overlay":
            return {f"{parts[2]}/{p.stem}"}, _REPO_ROOT / rel
        if _is_overlay_path(p):
            return {f"{p.parent.name}/{p.stem}"}, p
        ids = {f"main/{p.stem}"}
        if str(rel) in bysrc:
            ids.add("main/0x%08X" % bysrc[str(rel)][0])
        if p.stem in byname:
            ids.add("main/0x%08X" % byname[p.stem][0])
        m = re.fullmatch(r"func_([0-9A-Fa-f]{8})", p.stem)
        if m:
            ids.add("main/0x%08X" % int(m.group(1), 16))
        return ids, _REPO_ROOT / rel
    if "/" in ident:
        ov, fn = ident.split("/", 1)
        if ov in _overlay_names():
            src = _REPO_ROOT / "src" / "overlay" / ov / f"{fn}.c"
            return {f"{ov}/{fn}"}, src if src.exists() else None
    ids = {f"main/{ident}"}
    src = None
    if ident in byname:
        ids.add("main/0x%08X" % byname[ident][0])
        src = _REPO_ROOT / byname[ident][1]
    m = re.fullmatch(r"func_([0-9A-Fa-f]{8})", ident)
    if m:
        ids.add("main/0x%08X" % int(m.group(1), 16))
    return ids, (src if src is not None and src.exists() else None)


def _norm_int(x) -> str:
    try:
        return "%#x" % int(str(x), 0)
    except (TypeError, ValueError):
        return str(x)


def restricted_site_details(steps: List[dict]) -> dict:
    """{class: [site signature, ...]} -- WHAT each restricted site does, not
    just how many there are.  Grandfathering by count alone let a
    grandfathered function swap one patched (offset, value) for a different
    one at the same count (red-team finding 5); a signature pins the
    offset AND the value written."""
    out: dict = {}

    def add(c, sig):
        out.setdefault(c, []).append(sig)
    for s in steps or []:
        t = s.get("type")
        if t in _RESTRICTED_NAMES:
            if t == "patch_insn_imm16":
                ps = [str(p) for p in s.get("patches", []) if ":" in str(p)]
                for p in ps:
                    q = p.split(":")
                    add(t, ":".join([_norm_int(q[0])] + q[1:]))
                if not ps:
                    add(t, "?")
            elif t in ("invert_branch", "convert_ori_to_addiu", "convert_addiu_to_ori"):
                offs = s.get("offsets") or []
                for o in offs:
                    add(t, _norm_int(o))
                if not offs:
                    add(t, "all" if t == "convert_ori_to_addiu" else "?")
            elif t == "adjust_branch_offset":
                add(t, "%s:%s" % (_norm_int(s.get("offset")), _norm_int(s.get("imm"))))
            elif t == "nop_at_offset":
                add(t, _norm_int(s.get("offset")))
            else:
                add(t, "1")
        if t in ("patch_insn_reg_field",):
            for p in s.get("patches", []):
                q = str(p).split(":")
                if len(q) == 4 and "zero" in (q[2], q[3]):
                    add(ZERO_REG_PAINT, ":".join([_norm_int(q[0])] + q[1:]))
    return out


def restricted_sites(steps: List[dict]) -> dict:
    """{class: number of sites} for the restricted classes in a step list."""
    return {c: len(v) for c, v in restricted_site_details(steps).items()}


def _qualified_sites(src) -> dict:
    """Sites spelled with their sanctioned qualifier on the directive line."""
    out: dict = {}
    if src is None:
        return out
    try:
        from parse_postproc import directive_bodies
        content = Path(src).read_text(errors="replace")
    except Exception:
        return out
    for body in directive_bodies(content, "@PostProc:"):
        for name, flag in _QUALIFIER.items():
            if name == "patch_insn_imm16" and not IMM16_EXPECT_ENFORCED:
                continue
            if re.search(rf"(?<!\w){name}\b", body) and flag in body:
                n = 1
                if name == "patch_insn_imm16":
                    n = len(re.findall(r"0x[0-9A-Fa-f]+:\S+", body)) or 1
                out[name] = out.get(name, 0) + n
    return out


def check_restricted_postproc_steps(steps: List[dict], ident, *,
                                    strict: bool = True) -> List[Tuple[str, str, int]]:
    """Refuse a restricted class in a function that is not grandfathered for it
    (or uses more sites of it than it is grandfathered for)."""
    details = restricted_site_details(steps)
    sites = {c: len(v) for c, v in details.items()}
    if not sites:
        return []
    ids, src = _identify(ident)
    gf = _load_grandfathered()
    qual = _qualified_sites(src)
    violations = []
    for cls, n in sorted(sites.items()):
        ents = [gf.get(cls, {}).get(i) for i in ids]
        ents = [e for e in ents if e]
        allowed = max((e.get("sites", 0) for e in ents), default=0)
        unqualified = n - min(qual.get(cls, 0), n)
        if unqualified > allowed:
            violations.append((cls, _RESTRICTED_REASON[cls],
                               f"{unqualified} site(s), grandfathered {allowed}"))
            continue
        # (offset, value) pinning: when the entry records WHICH sites were
        # grandfathered, every unqualified site in use must be one of them.
        pinned = [e for e in ents if e.get("detail")]
        if pinned and unqualified:
            from collections import Counter
            have = Counter(details[cls])
            ok = any(not (have - Counter(e["detail"])) for e in pinned)
            if not ok and not qual.get(cls):
                extra = sorted((have - Counter(pinned[0]["detail"])).elements())
                violations.append((cls, _RESTRICTED_REASON[cls],
                                   f"site(s) {extra[:4]} not among the grandfathered "
                                   f"(offset, value) pairs"))
    if strict and violations:
        cls, reason, detail = violations[0]
        print(
            f"⛔ RESTRICTED POSTPROC in {ident} ({', '.join(sorted(ids))})\n"
            f"   Class:   {cls}  [{detail}]\n"
            f"   Reason:  {reason}\n"
            f"\n"
            f"   This class is BANNED for new use (2026-09-27).  "
            f"The functions that used it\n"
            f"   before the ban are a remediation worklist in "
            f"{GRANDFATHER_FILE.relative_to(_REPO_ROOT)};\n"
            f"   do NOT add to it.  Fix the C, or mark the function asm_only with a\n"
            f"   blocker category in config/c_unmatchable.txt.",
            file=sys.stderr,
        )
        sys.exit(1)
    return [(c, r, 0) for c, r, _ in violations]


def _steps_from_source(source_path) -> List[dict]:
    """Parse a .c file with the SAME parser its build uses."""
    import io
    import contextlib
    p = Path(source_path)
    sink = io.StringIO()
    with contextlib.redirect_stdout(sink), contextlib.redirect_stderr(sink):
        if _is_overlay_path(p):
            ov_dir = str(_REPO_ROOT / "tools" / "overlay")
            if ov_dir not in sys.path:
                sys.path.insert(0, ov_dir)
            from overlay_postproc import parse_postproc
            return parse_postproc(p)
        from parse_postproc import parse_postproc_header
        return parse_postproc_header(p) or []


def restricted_census(write_prune: bool = False, arm=None, pin: bool = False) -> int:
    """Compare every C file's restricted-class use with the grandfather list.

    Prints the worklist sizes, any function over its allowance (a violation)
    and any entry that has been remediated.  With write_prune=True the file is
    rewritten with remediated entries REMOVED and site counts lowered -- it
    never adds an entry or raises a count."""
    gf = _load_grandfathered()
    now: dict = {}
    byname, bysrc = _registry()
    srcs = [(f"main/0x{a:08X}", _REPO_ROOT / s) for s, (a, _n) in bysrc.items()
            if s.endswith(".c")]
    for f in sorted((_REPO_ROOT / "src" / "overlay").glob("*/*.c")):
        srcs.append((f"{f.parent.name}/{f.stem}", f))
    now_detail: dict = {}
    for ident, path in srcs:
        if not path.exists():
            continue
        det = restricted_site_details(_steps_from_source(path))
        qual = _qualified_sites(path)
        for cls, v in det.items():
            n = len(v) - min(qual.get(cls, 0), len(v))
            if n:
                now.setdefault(cls, {})[ident] = n
                now_detail.setdefault(cls, {})[ident] = sorted(v)
    bad = 0
    classes = sorted(set(gf) | set(now) | _RESTRICTED_NAMES | {ZERO_REG_PAINT})
    print("restricted class          grandfathered  in use  over  remediated")
    for cls in classes:
        g = gf.get(cls, {})
        u = now.get(cls, {})
        over = [i for i in u if u[i] > g.get(i, {}).get("sites", 0)]
        done = [i for i in g if i not in u]
        bad += len(over)
        print(f"  {cls:24s} {len(g):12d} {len(u):7d} {len(over):5d} {len(done):11d}")
        for i in over:
            print(f"      OVER: {i} uses {u[i]}, grandfathered {g.get(i, {}).get('sites', 0)}")
    if write_prune or arm or pin:
        with open(GRANDFATHER_FILE, "r", encoding="utf-8") as f:
            doc = json.load(f)
        classes = doc.setdefault("classes", {})
        for cls in arm or ():
            # ONE-TIME arming of a class that is newly RESTRICTED: its users
            # at the moment of the ban become the worklist.  Refused for a
            # class already in the file -- that would be a re-grandfathering.
            if cls not in _RESTRICTED_NAMES:
                raise SystemExit(f"--arm {cls}: not a RESTRICTED class")
            if cls in classes:
                raise SystemExit(f"--arm {cls}: already armed; the worklist only shrinks")
            classes[cls] = {i: {"sites": n, "detail": now_detail[cls][i]}
                            for i, n in sorted(now.get(cls, {}).items())}
            doc.setdefault("armed", {})[cls] = __import__("datetime").date.today().isoformat()
            print(f"armed {cls}: {len(classes[cls])} function(s) grandfathered")
        if pin:
            # Narrow existing entries to the (offset, value) pairs in use now.
            # Never widens: an entry is pinned only when its in-use sites fit
            # its recorded count.
            for cls, g in classes.items():
                for i, ent in g.items():
                    d = now_detail.get(cls, {}).get(i)
                    if d and "detail" not in ent and len(d) <= ent["sites"]:
                        ent["detail"] = d
        for cls, g in classes.items():
            u = now.get(cls, {})
            for i in list(g):
                if i not in u:
                    del g[i]
                else:
                    g[i]["sites"] = min(g[i]["sites"], u[i])
                    if "detail" in g[i]:
                        from collections import Counter
                        keep = Counter(g[i]["detail"]) & Counter(now_detail[cls][i])
                        g[i]["detail"] = sorted(keep.elements())
        doc["counts"] = {c: {"functions": len(v), "sites": sum(x["sites"] for x in v.values())}
                         for c, v in sorted(doc.get("classes", {}).items())}
        GRANDFATHER_FILE.write_text(json.dumps(doc, indent=1, sort_keys=True) + "\n")
        print(f"pruned {GRANDFATHER_FILE}")
    return 1 if bad else 0


if __name__ == "__main__":
    import argparse
    _ap = argparse.ArgumentParser(description="banned/restricted PostProc census")
    _ap.add_argument("--census", action="store_true",
                     help="report restricted-class use vs the grandfather worklist")
    _ap.add_argument("--prune", action="store_true",
                     help="with --census: drop remediated entries (never adds)")
    _ap.add_argument("--arm", action="append", default=[], metavar="CLASS",
                     help="with --census: ONE-TIME grandfathering of a class newly added "
                          "to RESTRICTED_DIRECTIVES (refused if the class is already armed)")
    _ap.add_argument("--pin", action="store_true",
                     help="with --census: record each entry's (offset, value) site "
                          "signatures so a same-count substitution is refused (narrows only)")
    _a = _ap.parse_args()
    sys.exit(restricted_census(_a.prune, _a.arm, _a.pin) if _a.census else 0)
