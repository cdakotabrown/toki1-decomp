#!/usr/bin/env python3
"""Fast single-function byte-match test harness.

Compiles ONE C file through the FULL PostProc pipeline (including
custom_script, asm_intercept, prepend_thunk), extracts .text bytes,
and compares against the target in SLPS_000.65.

Much faster than build_level3_c.py (~2-5 seconds vs ~5 minutes).

Usage:
    python3 tools/analysis/test_single_match.py func_8009D334
    python3 tools/analysis/test_single_match.py func_8009D334 -c src/main/matched/func_8009D334.c
    python3 tools/analysis/test_single_match.py func_8009D334 --verbose
"""
import sys, os, struct, json, importlib.util

# Add build tools to path (use absolute path to ensure correct resolution)
_script_dir = os.path.dirname(os.path.abspath(__file__))
_build_dir = os.path.normpath(os.path.join(_script_dir, '..', 'build'))
sys.path.insert(0, _build_dir)

from build_level3_c import (compile_c_function as compile_single_function,
                            fixup_relocations, report_nonreloc_fixups)
from parse_postproc import (parse_postproc_header, parse_compile_flags, parse_compiler,
                            parse_compile_env, parse_symbol_start, parse_symbol_size)
from banned_postproc import check_banned_postproc

# Printed on every win — a match is real only if IDO produced it from faithful C.
_AUDIT_REMINDER = """
  ────────────────────────────────────────────────────────────────────
  HONESTY AUDIT REQUIRED before you record this as a win:
  A "frankenword" (bytes match, semantics differ) is a FAILURE, not a win.
   1. If the @PostProc header has a `custom_script`, it must ONLY read +
      transform register/offset fields or fix relocs — NOT write hardcoded
      instruction words. Verify with banned_postproc.is_painting_script.
   2. Only ALLOWED PostProc (CLAUDE.md). BANNED, no exceptions: patch_insn_word,
      patch_instruction, insert_*, write_insn_at_offset, prepend_thunk,
      append_trailing_instructions, epilogue_share_into_branches.
   3. The C must be a faithful decompilation — not bytes bent to match.
      If you forced it, revert and mark the function a BLOCKER.
  ────────────────────────────────────────────────────────────────────"""


# MIPS opcodes whose low half-word is a 16-bit immediate a relocation can
# occupy: lui / arithmetic-immediate / every load and store.  Deliberately
# EXCLUDES opcode 0 (SPECIAL -- rd/shamt/funct live there), the branches
# (0x01, 0x04-0x07: an imm16, but a DISPLACEMENT, so masking it would hide a
# wrong branch target) and the jumps (0x02/0x03: a 26-bit target).
_IMM16_RELOC_OPCODES = frozenset({
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,          # addi..lui
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,                # lb..lwr
    0x28, 0x29, 0x2a, 0x2b, 0x2e,                            # sb..swr
    0x31, 0x32, 0x39, 0x3a,                                  # lwc1/2, swc1/2
})

def main():
    # Ensure we're in the project root
    root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    os.chdir(root)

    import argparse
    parser = argparse.ArgumentParser(description='Test single function byte match')
    parser.add_argument('func_name', help='Function name')
    parser.add_argument('-c', '--source', help='Path to C source file')
    parser.add_argument('--verbose', '-v', action='store_true')
    args = parser.parse_args()

    func_name = args.func_name

    # Find source
    source = args.source
    if source is None:
        for d in ['src/main/matched', 'src/main/semi-matched', 'src/main/wip']:
            p = os.path.join(d, f'{func_name}.c')
            if os.path.exists(p):
                source = p
                break
    if source is None or not os.path.exists(source):
        print(f"Error: No source file found for {func_name}")
        sys.exit(1)

    # Get function info
    config_path = 'config/level3_functions.json'
    func_addr = 0
    func_size = 0
    if os.path.exists(config_path):
        with open(config_path) as f:
            funcs = json.load(f)
        if isinstance(funcs, dict) and 'functions' in funcs:
            funcs = {x['name']: x for x in funcs['functions']}
        if func_name in funcs:
            info = funcs[func_name]
            func_addr = info.get('address', 0)
            if isinstance(func_addr, str): func_addr = int(func_addr, 16)
            func_size = info.get('size', 0)
            if isinstance(func_size, str): func_size = int(func_size, 0)

    # Fallback: parse from header
    if func_addr == 0 or func_size == 0:
        import re
        with open(source) as f:
            for line in f:
                if '*/' in line: break
                m = re.search(r'Address:\s*(0x[0-9a-fA-F]+)', line)
                if m: func_addr = int(m.group(1), 16)
                m = re.search(r'Size:\s*(\d+)\s*bytes', line)
                if m: func_size = int(m.group(1))

    if func_addr == 0 or func_size == 0:
        print(f"Error: Could not determine address ({hex(func_addr)}) or size ({func_size}) for {func_name}")
        sys.exit(1)

    # ── @SymbolStart / @SymbolSize boundary windowing ─────────
    # The .sym/config draws some main-binary cross-prologue functions 1-2
    # instructions LATE (IDO parks the function's hoisted li/lui setup in the
    # prior function's dead epilogue space). @SymbolStart sets the TRUE start
    # so the compare window covers the setup the compiler genuinely emits from
    # globals-first C; @SymbolSize trims/extends the window length. This is
    # SANCTIONED boundary recovery — it only windows the symbol's real bytes,
    # it cannot fabricate. The compiled .text already carries the hoisted setup
    # at its front (no offset shift needed: the function is the whole TU).
    sym_start = parse_symbol_start(source)
    sym_size = parse_symbol_size(source)
    if sym_start is not None and sym_start > func_addr:
        # A LATER start is legal in exactly one case: the recorded address is
        # the PREVIOUS function's branch-delay slot.  17 main-binary symbols
        # are like that (audit_phantom_symbols.py category "delay-slot",
        # recorded in config/main_symbol_boundaries.json).
        #
        # ⚠️ That word is genuinely SHARED and the verdict alone does NOT
        # settle who owns it.  IDO parks a function's hoisted setup in the
        # prior function's dead epilogue space, so the same word can be the
        # previous function's delay slot AND this function's first emitted
        # instruction -- func_80045418, func_8004660C and vblank_function are
        # all byte-perfect over the window that INCLUDES it.  So this is
        # OPT-IN per file, never automatic: write the @SymbolStart only when
        # your C does not emit that word.  Applying it tree-wide from the
        # config breaks those three.
        _bp = os.path.join(os.path.dirname(os.path.dirname(
            os.path.dirname(os.path.abspath(__file__)))),
            'config', 'main_symbol_boundaries.json')
        _b = {}
        try:
            with open(_bp) as _f:
                _b = json.load(_f).get('boundaries', {}).get(
                    f'0x{func_addr:08X}', {})
        except Exception:
            pass
        if (_b.get('category') == 'delay-slot'
                and _b.get('true_start')
                and int(_b['true_start'], 16) == sym_start):
            print(f"  Boundary: 0x{func_addr:08X} is the previous function's "
                  f"branch-delay slot; windowing from 0x{sym_start:08X} "
                  f"(config/main_symbol_boundaries.json category delay-slot)")
            if sym_size is None:
                func_size -= (sym_start - func_addr)
            func_addr = sym_start
            sym_start = None
        else:
            print(f"Error: @SymbolStart {hex(sym_start)} is AFTER the symbol "
                  f"address {hex(func_addr)} — @SymbolStart only recovers an "
                  f"EARLIER true start, never a later one, unless "
                  f"config/main_symbol_boundaries.json classifies "
                  f"{hex(func_addr)} as a delay-slot symbol with that exact "
                  f"true_start.")
            sys.exit(1)
    if sym_start is not None:
        delta = func_addr - sym_start
        func_addr = sym_start
        # Grow the window by the recovered leading bytes unless @SymbolSize
        # gives an explicit length.
        if sym_size is None:
            func_size += delta
    if sym_size is not None:
        func_size = sym_size

    # ── BANNED POSTPROC CHECK ──────────────────────────────
    # Fail fast if source uses banned directives.
    check_banned_postproc(source, strict=True)
    # ─────────────────────────────────────────────────────────

    # Parse PostProc from source
    postproc_steps = parse_postproc_header(source)
    extra_flags = parse_compile_flags(source)
    compiler = parse_compiler(source) or "ido52"
    compile_env = parse_compile_env(source)

    print(f"Function:  {func_name}")
    print(f"Source:    {source}")
    print(f"Address:   {hex(func_addr)}")
    print(f"Size:      {func_size} bytes ({func_size // 4} instructions)")
    print(f"Compiler:  {compiler}")
    if postproc_steps:
        print(f"PostProc:  {len(postproc_steps)} steps")
        for step in postproc_steps:
            print(f"  - {step}")
    if extra_flags:
        print(f"Flags:     {extra_flags}")
    if compile_env:
        print(f"Env:       {compile_env}")

    # Extract target bytes
    exe_path = 'disks/SLPS_000.65'
    if not os.path.exists(exe_path):
        print(f"Error: {exe_path} not found")
        sys.exit(1)

    offset = (func_addr - 0x80041000) + 0x800
    with open(exe_path, 'rb') as f:
        f.seek(offset)
        target_bytes = f.read(func_size)

    # Compile with full PostProc pipeline
    print(f"\nCompiling with full PostProc pipeline...")
    try:
        compiled_bytes = compile_single_function(
            source, func_name, func_addr,
            verbose=args.verbose,
            postproc_steps=postproc_steps,
            extra_flags=extra_flags,
            compiler=compiler,
            compile_env=compile_env
        )
    except Exception as e:
        print(f"Compilation failed: {e}")
        sys.exit(1)

    if compiled_bytes is None:
        print("Compilation returned None")
        sys.exit(1)

    # ── THE TRUNCATED-COMPARE TRAP ─────────────────────────────
    # The compare window is the TARGET symbol's size. A compile that is LONGER
    # than the window used to be silently truncated here and the truncated
    # length reported as "Compiled: N" — so a too-small window (a wrong
    # @SymbolSize, a fabricated head stub, a body that genuinely emits more
    # instructions than the symbol holds) reported "N/N BYTE-PERFECT" while the
    # surplus words were never looked at. Keep the FULL length, report both,
    # and refuse to call it a match when real (non-zero) words overrun.
    #
    # BENIGN exception: IDO pads .text to a 16-byte boundary with ZERO words.
    # Zero-word surplus shorter than one 16-byte block is that padding.
    full_len = len(compiled_bytes)
    n_compiled_full = full_len // 4
    overrun = compiled_bytes[func_size:] if full_len > func_size else b''
    overrun_words = [struct.unpack('<I', overrun[i:i+4])[0]
                     for i in range(0, len(overrun) - 3, 4)]
    nonzero_overrun = [(func_size // 4 + i, w)
                       for i, w in enumerate(overrun_words) if w != 0]
    benign_pad = bool(overrun) and not nonzero_overrun and len(overrun) < 16

    # TRUE BODY LENGTH (2026-09-06): "Compiled: N" is the window length when the
    # compile is shorter than the window (the compare pads with zero words), and
    # the untruncated length includes IDO's 16-byte .text pad.  Count the body
    # itself: drop trailing zero words, keep the delay slot of a terminal jump.
    _all_words = [struct.unpack('<I', compiled_bytes[i:i+4])[0]
                  for i in range(0, full_len - 3, 4)]
    _last = len(_all_words) - 1
    while _last >= 0 and _all_words[_last] == 0:
        _last -= 1
    n_body = _last + 1
    if _last >= 0:
        _w = _all_words[_last]; _op = _w >> 26
        if _op in (2, 3) or (_op == 0 and (_w & 0x3f) in (8, 9)) or _op in (1, 4, 5, 6, 7):
            n_body += 1          # j/jal, jr/jalr, or a branch: its delay slot is a real nop
    n_body = min(n_body, n_compiled_full)

    if len(compiled_bytes) > func_size:
        compiled_bytes = compiled_bytes[:func_size]

    # Apply relocation fixup (patches jal/lui/addiu immediates from target binary)
    file_offset = (func_addr - 0x80041000) + 0x800
    with open(exe_path, 'rb') as f:
        exe_data = f.read()
    # Collect what the fixup masks at NON-relocation sites: those are genuine
    # codegen diffs (frame offsets, branch displacements, non-address
    # immediates) that the fixup silently corrects from the target.
    fixup_audit = []
    # 2026-10-03: the fixup rewrites ONLY fields at the compiled object's own
    # R_MIPS_26/HI16/LO16 records, after checking that the symbol the C named
    # (or the TU's own .rodata/.data contents, jump tables entry by entry)
    # matches what the retail word encodes.  Everything else -- a raw
    # constant lui, a branch displacement -- is compared as compiled.
    reloc_errors = []
    compiled_bytes = fixup_relocations(compiled_bytes, func_addr, exe_data,
                                       file_offset, func_name=func_name,
                                       audit=fixup_audit,
                                       errors=reloc_errors)

    # Compare
    target_words = []
    compiled_words = []
    for i in range(0, func_size, 4):
        if i + 4 <= len(target_bytes):
            target_words.append(struct.unpack('<I', target_bytes[i:i+4])[0])
        if i + 4 <= len(compiled_bytes):
            compiled_words.append(struct.unpack('<I', compiled_bytes[i:i+4])[0])

    n_target = len(target_words)
    n_compiled = len(compiled_words)
    n_compare = min(n_target, n_compiled)

    # Words that carry a REAL relocation record in the final object: only
    # these may be "masked" (2026-10-03 -- a raw-constant lui used to count).
    import build_level3_c as _b3
    _info = _b3.LAST_OBJ_INFO or {}
    _reloc_words = {r[0] // 4 for r in _info.get('relocs', {}).get('.text', [])}

    exact_match = 0
    reloc_match = 0  # Matches ignoring relocation fields
    mismatches = []

    for i in range(n_compare):
        tw = target_words[i]
        cw = compiled_words[i]
        if tw == cw:
            exact_match += 1
            reloc_match += 1
        else:
            # Check if only relocation fields differ (lui/addiu/lw/sw immediates).
            #
            # 2026-09-16: this mask used to be applied to EVERY opcode, which is
            # wrong for anything that has no imm16 in the low half-word.  For
            # R-type (opcode 0) those bits are rd/shamt/funct, so
            #   move $v0,$zero (0x00001025) vs nop (0x00000000)
            # compared "equal" (same opcode, same top half-word) -- and worse,
            # addu vs subu on the same two operands differ only in funct.  Such
            # words were counted in Masked AND dropped from the mismatch list,
            # so the position list under-reported real codegen differences.
            # Restrict the mask to opcodes that genuinely carry a 16-bit
            # immediate a relocation can land in.
            opcode_t = (tw >> 26) & 0x3f
            opcode_c = (cw >> 26) & 0x3f
            if (i in _reloc_words and opcode_t == opcode_c
                    and opcode_t in _IMM16_RELOC_OPCODES
                    and (tw & 0xffff0000) == (cw & 0xffff0000)):
                reloc_match += 1
            else:
                mismatches.append(i)

    print(f"\n{'='*60}")
    print(f"Results:")
    print(f"  Target:   {n_target} instructions")
    if n_compiled_full != n_compiled:
        kind = "benign .text pad" if benign_pad else "OVERRUNS THE WINDOW"
        print(f"  Compiled: {n_compiled} instructions "
              f"(untruncated: {n_compiled_full} — {kind})")
    else:
        print(f"  Compiled: {n_compiled} instructions")
    print(f"  Body:     {n_body} words (trailing zero pad excluded; the honest count)")
    print(f"  Exact:    {exact_match}/{n_compare} ({100*exact_match/max(n_compare,1):.1f}%)")
    print(f"  Masked:   {reloc_match}/{n_compare} ({100*reloc_match/max(n_compare,1):.1f}%)")
    if fixup_audit:
        print(f"  Fixup:    {len(fixup_audit)} word(s) taken from the TARGET at "
              f"NON-relocation sites (real codegen diffs, masked)")
        report_nonreloc_fixups(strict=args.verbose, out=print, audit=fixup_audit)
    if reloc_errors:
        print(f"  Reloc:    {len(reloc_errors)} relocation check(s) REFUSED "
              f"(field left as compiled; NOT a match):")
        for e in reloc_errors[:40]:
            print(f"    {e}")

    if nonzero_overrun:
        print(f"\n  *** WINDOW OVERRUN — NOT A MATCH ***")
        print(f"  The compile is {n_compiled_full} instructions but the symbol "
              f"window is only {n_target}.")
        print(f"  {len(nonzero_overrun)} REAL (non-zero) instruction(s) past the "
              f"window were never compared:")
        for idx, w in nonzero_overrun[:8]:
            print(f"    [{idx:3d}] compiled={w:08x}  (outside target window)")
        print(f"  A shorter window cannot certify a longer body. Fix the C, the "
              f"@SymbolSize/@SymbolStart window, or the .sym boundary.")
        print(f"{'='*60}")
        sys.exit(1)
    if benign_pad:
        print(f"  Note:     {len(overrun)//4} trailing ZERO word(s) past the "
              f"window = IDO 16-byte .text padding (benign)")

    if reloc_errors:
        print(f"\n  *** RELOCATION CHECK FAILED -- NOT A MATCH ***")
        print(f"{'='*60}")
        sys.exit(1)
    if n_target == n_compiled and exact_match == n_target:
        print(f"\n  *** BYTE-PERFECT MATCH! ***")
        print(_AUDIT_REMINDER)
    elif n_target == n_compiled and reloc_match == n_target:
        print(f"\n  *** RELOCATION-ONLY DIFFS (effectively matched) ***")
        print(_AUDIT_REMINDER)
    elif mismatches:
        print(f"\n  {len(mismatches)} mismatches at positions: {mismatches[:20]}")
        if args.verbose:
            for idx in mismatches[:40]:
                tw = target_words[idx]
                cw = compiled_words[idx]
                print(f"    [{idx:3d}] target={tw:08x} compiled={cw:08x}")

    print(f"{'='*60}")
    sys.exit(0 if exact_match == n_target else 1)


if __name__ == '__main__':
    main()
