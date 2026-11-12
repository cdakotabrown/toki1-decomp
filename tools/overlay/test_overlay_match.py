#!/usr/bin/env python3
"""Test single overlay function byte match.

Compiles ONE overlay C file through the full PostProc pipeline,
extracts .text bytes, and compares against the target EXN file.

Usage:
    python3 tools/overlay/test_overlay_match.py GEKO func_80132244
    python3 tools/overlay/test_overlay_match.py GEKO func_80132244 --verbose
"""
import sys
import os
import re
import struct
import subprocess
import tempfile
from pathlib import Path
from typing import Dict, List, Optional, Tuple

# Project root
ROOT = Path(__file__).parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "build"))
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

from parse_postproc import (parse_compile_flags, parse_compile_env, parse_compiler,
                            parse_symbol_start_text, parse_symbol_size_text)
from ido_flags import (base_flags, compile_flag_tail, glue_option_values,
                       parse_extra_cflags)
from banned_postproc import check_banned_postproc
from overlay_postproc import parse_postproc, apply_postproc, get_target_exn, fixup_relocations

# Printed on EVERY win (byte-perfect or reloc-only). A match is only real if the
# compiler produced it honestly — see CLAUDE.md "The One Rule That Gets Reverted".
AUDIT_REMINDER = """
  ────────────────────────────────────────────────────────────────────
  HONESTY AUDIT REQUIRED before you record this as a win:
  A match is a WIN only if IDO produced these bytes from faithful C. A
  "frankenword" (bytes match, semantics differ) is a FAILURE, not a win.
   1. If the @PostProc header has a `custom_script`, it must ONLY read +
      transform register/offset fields or fix relocations — it must NOT
      write hardcoded instruction words. Verify:
        python3 -c "import sys;sys.path.insert(0,'tools/build');\\
        from banned_postproc import is_painting_script as p;\\
        print('BANNED PAINTING' if p('tools/overlay/<script>.py') else 'ok')"
   2. Only ALLOWED PostProc (CLAUDE.md). BANNED, no exceptions: patch_insn_word,
      patch_instruction, insert_*/insert_move_and_shift, write_insn_at_offset,
      prepend_thunk, append_trailing_instructions, epilogue_share_into_branches.
   3. The C must be a faithful decompilation of the function's logic — not bytes
      bent to match. If you forced it, revert and mark the function a BLOCKER.
  ────────────────────────────────────────────────────────────────────"""

# Paths
SRC_OVERLAY_DIR = ROOT / "src" / "overlay"
CONFIG_DIR = ROOT / "config" / "overlay"
ASSETS_DIR = ROOT / "assets" / "overlays"
INCLUDE_DIR = ROOT / "include"

# Tools
IDO_CC = ROOT / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
GCCWRAP = ROOT / "tools" / "gcc" / "gccwrap.py"

# gccwrap.py flags that belong to the wrapper, not cc1 (mirrors
# build_level3_c.py's handling of @CompileFlags for GCC functions)
GCCWRAP_FLAGS = {"--fill-jalr", "--fill-epilogue", "--fill-all",
                 "--no-delay-slots", "--no-cross-jumping"}

# Compile flags. Base line comes from tools/build/ido_flags.py (one source of
# truth); the per-file PIC mode is added at the compile site via pic_flags().
IDO_FLAGS = base_flags([INCLUDE_DIR, f"{INCLUDE_DIR}/psyq"])

# Overlay base address
OVERLAY_BASE = 0x80132000


def parse_sym_file(overlay_name: str) -> Dict[str, Tuple[int, int]]:
    """Parse overlay symbol file to get function addresses and sizes.

    Returns dict mapping func_name -> (address, size)
    """
    sym_file = CONFIG_DIR / f"{overlay_name}.sym"
    if not sym_file.exists():
        return {}

    funcs = {}
    with open(sym_file) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            # Format: 0x80132244:232:func_80132244
            parts = line.split(':')
            if len(parts) >= 3:
                addr = int(parts[0], 16)
                size = int(parts[1])
                name = parts[2]
                funcs[name] = (addr, size)
    return funcs




_WINDOW_CACHE: Dict[str, list] = {}


def _overlay_windows(overlay_name: str) -> list:
    """[(start, end, func)] compare windows of every function in an overlay:
    each .c's own @SymbolStart/@SymbolSize, else its .sym entry."""
    if overlay_name in _WINDOW_CACHE:
        return _WINDOW_CACHE[overlay_name]
    funcs = parse_sym_file(overlay_name)
    out = []
    for name, (a, z) in funcs.items():
        c = SRC_OVERLAY_DIR / overlay_name / f"{name}.c"
        st, ss = a, z
        if c.exists():
            t = c.read_text(errors='replace')
            st = parse_symbol_start_text(t) or a
            ss = parse_symbol_size_text(t) or z
        out.append((st, st + ss, name))
    _WINDOW_CACHE[overlay_name] = out
    return out


def window_tail_gap(overlay_name: str, func_name: str, content: str = None):
    """Bytes of the .sym symbol that the compare window NEVER covers.

    2026-10-03 (red-team finding 5): a `@SymbolSize` smaller than the .sym
    size trims the compare window, and nothing checked what the trimmed tail
    holds -- RENSYU/func_80132040 was "byte-perfect" over 160 of its 168
    bytes.  The tail is fine when it is alignment padding (zero words) or
    is covered by ANOTHER function's window (a cross-prologue head the next
    function absorbed with @SymbolStart); anything else is unchecked code.
    A source line `@TrailingPadding:` documents a deliberate exception.

    Returns (uncovered_bytes, detail)."""
    funcs = parse_sym_file(overlay_name)
    if func_name not in funcs:
        return 0, ''
    sym_addr, sym_size = funcs[func_name]
    if content is None:
        src = SRC_OVERLAY_DIR / overlay_name / f"{func_name}.c"
        if not src.exists():
            return 0, ''
        content = src.read_text(errors='replace')
    ss = parse_symbol_size_text(content)
    if ss is None:
        return 0, ''
    st = parse_symbol_start_text(content) or sym_addr
    win_end = st + ss
    sym_end = sym_addr + sym_size
    if win_end >= sym_end:
        return 0, ''
    if re.search(r'^\s*(?:/?\*+)?\s*@TrailingPadding:', content, re.M):
        return 0, 'documented @TrailingPadding'
    try:
        tail = get_target_bytes(overlay_name, win_end, sym_end - win_end)
    except FileNotFoundError:
        return 0, ''
    others = [(a, b) for a, b, n in _overlay_windows(overlay_name) if n != func_name]
    unc = []
    for k in range(0, len(tail) // 4 * 4, 4):
        a = win_end + k
        w = struct.unpack_from('<I', tail, k)[0]
        if w == 0 or any(x <= a < y for x, y in others):
            continue
        unc.append(a)
    if not unc:
        return 0, ''
    return 4 * len(unc), ('window 0x%X..0x%X ends %d bytes before the .sym '
                          'symbol (0x%X); %d non-padding byte(s) there are '
                          'covered by no function window'
                          % (st, win_end, sym_end - win_end, sym_end, 4 * len(unc)))


def extract_text_bytes(obj_file: Path, func_name: str = None, target_size: int = None) -> bytes:
    """Extract .text section bytes from object file.

    If func_name is provided, extract only the bytes for that symbol.
    If target_size is provided, use that instead of the ELF symbol size
    (IDO often reports smaller symbol sizes due to alignment padding).
    Otherwise return the entire .text section.
    """
    if func_name:
        # Find symbol offset and size
        sym_result = subprocess.run(
            ["mipsel-linux-gnu-objdump", "-t", str(obj_file)],
            capture_output=True, text=True
        )
        sym_offset = None
        sym_size = None
        for line in sym_result.stdout.splitlines():
            # Format: "00000010 g     F .text  00000060 func_8013D49C"
            parts = line.split()
            if len(parts) >= 6 and parts[-1] == func_name and '.text' in line:
                try:
                    sym_offset = int(parts[0], 16)
                    sym_size = int(parts[-2], 16)
                    break
                except ValueError:
                    continue
        if sym_offset is not None and sym_size is not None:
            text_result = subprocess.run(
                ["mipsel-linux-gnu-objcopy", "-O", "binary", "-j", ".text",
                 str(obj_file), "/dev/stdout"],
                capture_output=True
            )
            # Use target_size if provided (covers IDO alignment padding)
            extract_size = target_size if target_size else sym_size
            return text_result.stdout[sym_offset:sym_offset + extract_size]
    result = subprocess.run(
        ["mipsel-linux-gnu-objcopy", "-O", "binary", "-j", ".text",
         str(obj_file), "/dev/stdout"],
        capture_output=True
    )
    return result.stdout


def _elf_symbol_size(obj_file: Path, func_name: str) -> Optional[int]:
    """st_size of the function's FUNC symbol in the (post-PostProc) object,
    or None if not found. This is the compiler's own statement of how long
    the emitted function is -- the D4 overrun check compares it against the
    compare window."""
    sym_result = subprocess.run(
        ["mipsel-linux-gnu-objdump", "-t", str(obj_file)],
        capture_output=True, text=True
    )
    for line in sym_result.stdout.splitlines():
        parts = line.split()
        if len(parts) >= 6 and parts[-1] == func_name and '.text' in line:
            try:
                return int(parts[-2], 16)
            except ValueError:
                continue
    return None


def _func_window_offset(obj_file: Path, func_name: str) -> int:
    """Return the .text offset where `func_name`'s body begins.

    This is the same window start `extract_text_bytes` uses (the symbol's
    st_value). Reloc offsets from objdump are .text-relative; subtracting
    this gives a function-window-relative offset that lines up with the
    compare loop's word index. Falls back to the smallest FUNC st_value
    (get_func_text_offset) if the named symbol isn't found.
    """
    sym_result = subprocess.run(
        ["mipsel-linux-gnu-objdump", "-t", str(obj_file)],
        capture_output=True, text=True
    )
    for line in sym_result.stdout.splitlines():
        parts = line.split()
        if len(parts) >= 6 and parts[-1] == func_name and '.text' in line:
            try:
                return int(parts[0], 16)
            except ValueError:
                continue
    from overlay_postproc import get_func_text_offset
    return get_func_text_offset(obj_file)


def get_target_bytes(overlay_name: str, func_addr: int, func_size: int) -> bytes:
    """Extract target bytes from EXN file."""
    exn_file = ASSETS_DIR / f"{overlay_name}.EXN"
    if not exn_file.exists():
        raise FileNotFoundError(f"EXN file not found: {exn_file}")

    # EXN files are raw binary starting at OVERLAY_BASE
    offset = func_addr - OVERLAY_BASE

    with open(exn_file, 'rb') as f:
        f.seek(offset)
        return f.read(func_size)


def compare_bytes(target: bytes, compiled: bytes, verbose: bool = False,
                  reloc_words: Optional[Dict[int, str]] = None) -> Tuple[int, int, int, int, List[int]]:
    """Compare target and compiled bytes.

    Returns: (n_target, n_compiled, exact_match, reloc_match, mismatches)

    HONESTY (2026-06-24): a differing word counts as a relocation match ONLY
    when the compiled+PostProc'd object actually carries a relocation at that
    word, AND the diff is confined to the field that relocation rewrites:
      - R_MIPS_26  -> the 26-bit jump target (opcodes must match)
      - R_MIPS_HI16-> the low 16 bits of a `lui` (upper 16 must match)
      - R_MIPS_LO16-> the low 16 bits of an addiu/ori/lw/sw (rs/rt must match;
                      the ORI<->ADDIU linker form-swap is also permitted here)
    A word that differs at a NON-reloc offset is a GENUINE mismatch (a semi),
    even if it is an I-type immediate diff (a frame-size `addiu sp`, a stack
    `lw/sw` offset, a constant `ori/addiu`). `reloc_words` maps function-window
    word index -> GNU reloc type name; it is derived from the .o's .rel.text.

    `reloc_words=None` falls back to the legacy opcode-only mask (kept only for
    callers that cannot supply the reloc set — the authoritative pipeline
    always supplies it).
    """
    target_words = []
    compiled_words = []

    for i in range(0, len(target), 4):
        if i + 4 <= len(target):
            target_words.append(struct.unpack('<I', target[i:i+4])[0])

    for i in range(0, len(compiled), 4):
        if i + 4 <= len(compiled):
            compiled_words.append(struct.unpack('<I', compiled[i:i+4])[0])

    n_target = len(target_words)
    n_compiled = len(compiled_words)
    n_compare = min(n_target, n_compiled)

    exact_match = 0
    reloc_match = 0
    mismatches = []

    for i in range(n_compare):
        tw = target_words[i]
        cw = compiled_words[i]
        if tw == cw:
            exact_match += 1
            reloc_match += 1
            continue

        opcode_t = (tw >> 26) & 0x3f
        opcode_c = (cw >> 26) & 0x3f
        is_reloc_field_diff = False

        if reloc_words is None:
            # Legacy opcode-only mask (over-masks; see docstring).
            if opcode_t == opcode_c and opcode_t in (2, 3):
                is_reloc_field_diff = True
            elif opcode_t == opcode_c and (tw & 0xffff0000) == (cw & 0xffff0000):
                is_reloc_field_diff = True
        else:
            rtype = reloc_words.get(i)
            if rtype is None:
                # No relocation here -> the diff is real codegen, never masked.
                is_reloc_field_diff = False
            elif rtype == 'R_MIPS_26':
                # Jump target: opcodes must match; only the 26-bit field varies.
                if opcode_t == opcode_c and opcode_t in (2, 3):
                    is_reloc_field_diff = True
            elif rtype == 'R_MIPS_HI16':
                # lui hi16: upper 16 bits (opcode + rt) must match.
                if opcode_t == opcode_c and (tw & 0xffff0000) == (cw & 0xffff0000):
                    is_reloc_field_diff = True
            elif rtype == 'R_MIPS_LO16':
                # addiu/ori/lw/sw lo16: upper 16 (opcode + rs/rt) must match...
                if opcode_t == opcode_c and (tw & 0xffff0000) == (cw & 0xffff0000):
                    is_reloc_field_diff = True
                # ...or the ORI(13)<->ADDIU(9) linker form-swap (same rs/rt).
                elif {opcode_t, opcode_c} == {9, 13} and \
                        (tw & 0x03ff0000) == (cw & 0x03ff0000):
                    is_reloc_field_diff = True
            # Any other reloc type at this offset: require an actual match
            # (handled above by tw==cw); otherwise it is a mismatch.

        if is_reloc_field_diff:
            reloc_match += 1
        else:
            mismatches.append(i)
            if verbose and len(mismatches) <= 200:
                rt = reloc_words.get(i) if reloc_words is not None else None
                tag = f"  [{rt}]" if rt else "  [no-reloc]"
                print(f"    [{i:3d}] target={tw:08x} compiled={cw:08x}{tag}")

    return n_target, n_compiled, exact_match, reloc_match, mismatches


def test_overlay_function(overlay_name: str, func_name: str, verbose: bool = False,
                          fix_relocs: bool = False, return_bytes: bool = False,
                          keep_dir: str = None, semcheck: bool = False,
                          extra_cflags: Optional[List[str]] = None,
                          src_override: Optional[Path] = None) -> Dict:
    """Test a single overlay function for byte-perfect match.

    Args:
        overlay_name: Overlay name (e.g., GEKO)
        func_name: Function name (e.g., func_80132244)
        verbose: Print detailed output
        fix_relocs: Apply relocation fixup to achieve byte-perfect match
        extra_cflags: extra compiler flags appended to the compile line
            (flag SCREENING; see tools/overlay/screen_flag.py). Empty/None
            reproduces the stock command line exactly.
        src_override: compile THIS file instead of src/overlay/<OV>/<FN>.c.
            Screening harnesses use it to evaluate a modified COPY of a source
            (e.g. with a PostProc directive suppressed) without ever writing to
            the tracked .c.

    Returns dict with:
        - matched: True if byte-perfect
        - reloc_only: True if only relocation diffs (before fixup)
        - n_target: target instruction count
        - n_compiled: compiled instruction count
        - exact_pct: exact match percentage
        - reloc_pct: relocation-masked match percentage
    """
    postproc_failed = False
    truncated_surplus = False
    extra_cflags = list(extra_cflags or [])
    os.chdir(ROOT)

    # Get function info from symbol file
    funcs = parse_sym_file(overlay_name)
    if func_name not in funcs:
        return {'error': f"Function {func_name} not found in {overlay_name}.sym"}

    func_addr, func_size = funcs[func_name]

    # Find source file
    src_file = (Path(src_override) if src_override
                else SRC_OVERLAY_DIR / overlay_name / f"{func_name}.c")
    if not src_file.exists():
        return {'error': f"Source file not found: {src_file}"}

    # Check for @SymbolSize override in source file
    with open(src_file) as f:
        content = f.read()
    # ANCHORED parse: the directive counts only on a line that OPENS with it.
    # A header that merely EXPLAINS the fix ("Correct fix: ONE C function
    # windowed with @SymbolStart: 0xADDR / @SymbolSize: N") must NOT re-window
    # the compare for a file that never adopted it.
    symbol_size_override = parse_symbol_size_text(content)
    if symbol_size_override is not None:
        func_size = symbol_size_override

    # @SymbolStart override: the .sym draws this function's start late because IDO
    # parked its leading setup (li/lui) in the prior function's dead epilogue space.
    # @SymbolStart: 0xADDR sets the function's TRUE start so the compare window covers
    # the hoisted setup the compiler genuinely emits (honest boundary correction — the
    # producer trims its tail with @SymbolSize). Pair with globals-first C.
    symbol_start_override = parse_symbol_start_text(content)
    if symbol_start_override is not None:
        func_addr = symbol_start_override

    # Check for ASM-ONLY marker — these functions are byte-perfect by
    # construction (extracted from EXN) and don't compile through C.
    is_asm_only = "Status: ASM-ONLY" in content or "@AsmOnly" in content
    if is_asm_only:
        return {
            'matched': True,
            'reloc_only': False,
            'asm_only': True,
            'n_target': func_size // 4,
            'n_compiled': func_size // 4,
            'exact_match': func_size // 4,
            'reloc_match': func_size // 4,
            'exact_pct': 100.0,
            'reloc_pct': 100.0,
            'mismatches': 0,
        }

    # Target ASM file (for some PostProc)
    asm_file = ROOT / "asm" / "overlay" / overlay_name / f"{func_name}.s"

    if verbose:
        print(f"Function:  {func_name}")
        print(f"Overlay:   {overlay_name}")
        print(f"Address:   {hex(func_addr)}")
        print(f"Size:      {func_size} bytes ({func_size // 4} instructions)")

    # ── BANNED POSTPROC CHECK ──────────────────────────────
    # Fail fast if source uses banned directives.
    check_banned_postproc(src_file, strict=True)
    # Reject the fabricated cross-prologue-head stub: strip_trailing_instructions
    # on a <=16-byte symbol chops a `jr ra` the target does not contain.
    # Absorb the head into its host instead (tools/overlay/absorb_stub.py).
    from banned_postproc import check_fabricated_head_stub
    check_fabricated_head_stub(src_file, strict=True)
    # Also reject custom_scripts that paint hardcoded target instruction words.
    from banned_postproc import is_painting_script
    import re as _re
    for _l in open(src_file, errors='ignore'):
        _m = _re.search(r'@PostProc:\s*custom_script\s+(\S+)', _l)
        if not _m:
            continue
        _name = _m.group(1).rstrip('*/ ')
        _cands = [_name] + [os.path.join(b, _name) for b in
                            ('tools/ido', 'tools/overlay/custom', 'tools/overlay', 'tools/postproc', 'tools')]
        _sp = next((c for c in _cands if os.path.exists(c)), None)
        if _sp and is_painting_script(_sp):
            print(f"⛔ BANNED: custom_script {_name} paints hardcoded instruction "
                  f"words (patch_insn_word in disguise) for {func_name}", file=sys.stderr)
            sys.exit(1)
    # ─────────────────────────────────────────────────────────

    # Parse PostProc
    postproc_steps = parse_postproc(src_file)
    extra_flags = parse_compile_flags(src_file)
    compile_env = parse_compile_env(src_file)
    compiler = parse_compiler(src_file)

    if verbose and postproc_steps:
        print(f"PostProc:  {len(postproc_steps)} steps")

    # Get target bytes
    try:
        target_bytes = get_target_bytes(overlay_name, func_addr, func_size)
    except FileNotFoundError as e:
        return {'error': str(e)}

    # Compile in temp directory
    with tempfile.TemporaryDirectory() as tmpdir:
        tmpdir = Path(tmpdir)
        obj_file = tmpdir / f"{func_name}.o"

        if compiler.startswith("gcc"):
            # Psy-Q GCC pipeline via gccwrap.py (cpp + cc1 + maspsx + as)
            version_number = compiler[3:]  # "gcc281" -> "281"
            cmd = [sys.executable, str(GCCWRAP), str(src_file),
                   "-o", str(obj_file),
                   "--gcc-version", version_number,
                   "-I", str(INCLUDE_DIR), "-I", str(INCLUDE_DIR / "psyq")]
            cc1_extra = []
            for flag in list(extra_flags) + extra_cflags:
                if flag in GCCWRAP_FLAGS:
                    cmd.append(flag)
                else:
                    cc1_extra.append(flag)
            if cc1_extra:
                cmd.extend(["--extra-flags", " ".join(cc1_extra)])
        else:
            cmd = ([str(IDO_CC)] + IDO_FLAGS
                   + compile_flag_tail(src_file, compile_flags=extra_flags,
                                       extra_cflags=extra_cflags)
                   + ["-c", str(src_file), "-o", str(obj_file)])

        if verbose:
            print(f"\nCompiling ({compiler})...")
            if extra_cflags:
                print(f"  extra-cflags: {' '.join(extra_cflags)}")

        try:
            # Merge compile environment with current environment
            env = os.environ.copy()
            # Isolate toolchain intermediates per-invocation so concurrent /
            # tight-loop runs don't collide on fixed-name temp files (cfe/ugen/
            # uopt/as1/maspsx). tmpdir is already unique (TemporaryDirectory).
            env["TMPDIR"] = str(tmpdir)
            if compile_env:
                env.update(compile_env)
                if verbose:
                    print(f"  CompileEnv: {compile_env}")
            result = subprocess.run(cmd, capture_output=True, text=True, timeout=60,
                                    env=env, cwd=str(tmpdir))
            if result.returncode != 0:
                return {'error': f"Compile failed: {result.stderr[:200]}"}
            if verbose and result.stderr:
                # Surface hook firings ([UGEN] / [UOPT] / [CSE] / [DEADSTORE] etc.)
                hook_lines = [ln for ln in result.stderr.splitlines()
                              if ln.startswith('[') and ln.endswith(']') is False
                              and any(ln.startswith(p) for p in
                                      ('[UGEN]', '[UOPT]', '[CSE]', '[DEADSTORE]',
                                       '[LICM]', '[STRENGTH]', '[PRECOLOR]'))]
                for ln in hook_lines:
                    print(f"  {ln}")
        except Exception as e:
            return {'error': f"Compile error: {e}"}

        # Apply PostProc
        postproc_failed = False
        if postproc_steps:
            if verbose:
                print("Applying PostProc...")
            postproc_failed = not apply_postproc(
                obj_file, postproc_steps, func_addr, overlay_name,
                verbose=verbose,
                target_asm=asm_file if asm_file.exists() else None)
            if postproc_failed:
                # D1: never let a silently-broken chain read as codegen.
                print("=" * 60)
                print("⚠️  POSTPROC CHAIN FAILED (failing step named above).")
                print("   The diff below compares a PARTIALLY-processed object;")
                print("   it is NOT codegen evidence. Fix or remove the stale")
                print("   directive before drawing any conclusion.")
                print("=" * 60)

        # Extract compiled bytes
        compiled_bytes = extract_text_bytes(obj_file, func_name, target_size=func_size)

        # ── Build the AUTHORITATIVE relocation-offset set ──────────────
        # compare_bytes() masks a differing word ONLY where the final
        # PostProc'd object carries an actual relocation (R_MIPS_26/HI16/
        # LO16), and only the field that reloc rewrites. This replaces the
        # old by-opcode-type mask, which silently hid genuine codegen diffs
        # (frame-size `addiu sp`, stack `lw/sw` offsets, `ori/addiu`
        # constants) as if they were relocations.
        from overlay_postproc import get_reloc_offsets
        reloc_text = get_reloc_offsets(obj_file)
        # Reloc offsets are .text-relative; translate to function-window word
        # index. The function body begins at func_text_off within .text
        # (leading thunks, if any, live before it).
        func_text_off = _func_window_offset(obj_file, func_name)
        reloc_words = {}
        for toff, rtype in reloc_text.items():
            widx = (toff - func_text_off)
            if widx >= 0 and widx % 4 == 0:
                reloc_words[widx // 4] = rtype

        # Keep intermediate files if requested
        if keep_dir:
            keep_path = Path(keep_dir) / overlay_name / func_name
            keep_path.mkdir(parents=True, exist_ok=True)
            import shutil
            # Copy the final object file
            shutil.copy2(str(obj_file), str(keep_path / f"{func_name}.o"))
            # Copy any .pp*.o intermediate files
            for pp in sorted(obj_file.parent.glob(f"{func_name}.pp*.o")):
                shutil.copy2(str(pp), str(keep_path / pp.name))
            if verbose:
                print(f"  Kept intermediates in {keep_path}")

        # Truncate to target size if longer -- but NEVER silently (D4). The
        # truncation trap is what let 162 silent head stubs count as wins:
        # a compile LONGER than the symbol window had its surplus real
        # instructions (e.g. the jr ra; nop the target lacks) chopped
        # invisibly. Surplus ZERO words are legitimate assembler alignment;
        # surplus real instructions mean this is not a match.
        truncated_surplus = False
        elf_sym_size = _elf_symbol_size(obj_file, func_name)
        if elf_sym_size is not None and elf_sym_size > func_size:
            # Scan only THIS function's emission ([window, symbol end)) --
            # bytes past the symbol belong to other symbols / alignment and
            # are not evidence about this function.
            extended = extract_text_bytes(obj_file, func_name,
                                          target_size=elf_sym_size)
            surplus = extended[func_size:]
            for i in range(0, len(surplus) - (len(surplus) % 4), 4):
                if surplus[i:i+4] != b'\x00\x00\x00\x00':
                    truncated_surplus = True
                    break
        if truncated_surplus:
            print("⚠️  COMPILED BODY OVERRUNS THE SYMBOL WINDOW with real "
                  "instructions (emission %d bytes vs window %d, first real "
                  "word at +0x%x past the window). The masked compare would "
                  "silently truncate them; refusing to call this a match."
                  % (elf_sym_size, func_size, i))
        if len(compiled_bytes) > func_size:
            compiled_bytes = compiled_bytes[:func_size]

        # Apply relocation fixup if requested
        if fix_relocs and len(compiled_bytes) == len(target_bytes):
            if verbose:
                print("Applying relocation fixup...")
            compiled_bytes = fixup_relocations(compiled_bytes, target_bytes)

    # Opt-in structural check of the FINAL PostProc'd .text (--semcheck).
    # Proves DAMAGE (branch-in-delay-slot, $ra clobbered between restore and
    # return, frame access after sp dealloc, load-use hazard); it cannot prove
    # correctness, so it is never a default gate.
    semcheck_defects = []
    if semcheck:
        try:
            from semcheck import words_from_bytes, analyze
            semcheck_defects = analyze(words_from_bytes(compiled_bytes), 'FINAL')
        except Exception as e:                                  # noqa: BLE001
            semcheck_defects = [f'semcheck unavailable: {e}']

    # Compare
    n_target, n_compiled, exact_match, reloc_match, mismatches = compare_bytes(
        target_bytes, compiled_bytes, verbose, reloc_words=reloc_words
    )

    is_matched = (n_target == n_compiled and exact_match == n_target)
    is_reloc_only = (n_target == n_compiled and reloc_match == n_target and not is_matched)

    # D1: a broken chain can never claim a win -- the compare ran against a
    # partially-processed object. D4: neither can a compile whose surplus
    # real instructions were chopped by window truncation.
    if postproc_failed or truncated_surplus:
        is_matched = False
        is_reloc_only = False

    result = {
        'matched': is_matched,
        'reloc_only': is_reloc_only,
        'postproc_failed': postproc_failed,
        'truncated_surplus': truncated_surplus,
        'n_target': n_target,
        'n_compiled': n_compiled,
        'exact_match': exact_match,
        'reloc_match': reloc_match,
        'exact_pct': 100 * exact_match / max(n_target, 1),
        'reloc_pct': 100 * reloc_match / max(n_target, 1),
        'mismatches': len(mismatches),
        'semcheck': semcheck_defects,
    }
    if return_bytes:
        result['compiled_bytes'] = bytes(compiled_bytes)
        result['target_bytes'] = bytes(target_bytes)

    if verbose:
        print(f"\n{'='*60}")
        print(f"Results:")
        print(f"  Target:   {n_target} instructions")
        print(f"  Compiled: {n_compiled} instructions")
        print(f"  Exact:    {exact_match}/{n_target} ({result['exact_pct']:.1f}%)")
        print(f"  Masked:   {reloc_match}/{n_target} ({result['reloc_pct']:.1f}%)")

        if is_matched:
            print(f"\n  *** BYTE-PERFECT MATCH! ***")
            print(AUDIT_REMINDER)
        elif is_reloc_only:
            print(f"\n  *** RELOCATION-ONLY DIFFS (effectively matched) ***")
            print(AUDIT_REMINDER)
        elif mismatches:
            print(f"\n  {len(mismatches)} mismatches")

        print(f"{'='*60}")

    return result


def main():
    import argparse
    parser = argparse.ArgumentParser(description='Test single overlay function byte match')
    parser.add_argument('overlay', help='Overlay name (e.g., GEKO)')
    parser.add_argument('func_name', help='Function name (e.g., func_80132244)')
    parser.add_argument('--verbose', '-v', action='store_true')
    parser.add_argument('--fix-relocs', action='store_true',
                       help='Apply relocation fixup to achieve byte-perfect match')
    parser.add_argument('--keep', type=str, default=None, metavar='DIR',
                       help='Keep intermediate files in DIR/overlay/func/')
    parser.add_argument('--semcheck', action='store_true',
                       help="Run the structural checker (tools/overlay/semcheck.py) "
                            "on the FINAL PostProc'd .text and print any "
                            "branch-in-delay-slot / $ra-clobber / post-dealloc "
                            "frame access / load-use damage")
    parser.add_argument('--no-resolve', action='store_true',
                       help='Skip the authoritative resolved-link verdict on '
                            'reloc_only results (the over-masking guard)')
    parser.add_argument('--extra-cflags', action='append', default=None,
                       metavar='FLAGS',
                       help='Extra compiler flags appended to the compile line, '
                            'for FLAG SCREENING (e.g. --extra-cflags -Wo,-rege,0). '
                            'Repeatable; one value may hold several flags '
                            'separated by WHITESPACE. Commas are NOT separators '
                            '(IDO pass-through flags embed them). An explicit '
                            '-Wc,-picN here overrides the derived PIC mode. '
                            'Absent => command line is byte-identical to before.')
    parser.add_argument('--src', type=str, default=None, metavar='FILE',
                       help='Compile FILE instead of src/overlay/<OV>/<FUNC>.c '
                            '(screening harnesses pass a temp COPY so they never '
                            'write to a tracked .c). The compare window still '
                            'comes from the .sym plus this file\'s own '
                            '@SymbolStart/@SymbolSize.')
    # --check-write-insn-regression REMOVED: write_insn_at_offset is now BANNED.
    args = parser.parse_args(glue_option_values(sys.argv[1:]))

    extra_cflags = parse_extra_cflags(args.extra_cflags)

    result = test_overlay_function(args.overlay.upper(), args.func_name, args.verbose,
                                   fix_relocs=args.fix_relocs,
                                   keep_dir=args.keep, semcheck=args.semcheck,
                                   extra_cflags=extra_cflags,
                                   src_override=args.src)

    if 'error' in result:
        print(f"Error: {result['error']}")
        sys.exit(1)

    if args.semcheck:
        defects = result.get('semcheck') or []
        if defects:
            print(f"SEMCHECK: {len(defects)} structural defect(s) in the FINAL "
                  f"PostProc'd object -- this is PostProc DAMAGE, not a win:")
            for d in defects:
                print(f"    {d}")
        else:
            print("semcheck: clean (no structural damage in the final object)")

    gap, gap_detail = window_tail_gap(
        args.overlay.upper(), args.func_name,
        Path(args.src).read_text(errors='replace') if args.src else None)
    if gap and (result['matched'] or result['reloc_only']):
        print(f"⚠️  WINDOW TRUNCATED -- {gap_detail}.  NOT A WIN: those bytes were "
              f"never compared.  Widen @SymbolSize to the symbol (or split the .sym "
              f"if the tail is another function); `@TrailingPadding:` in the header "
              f"documents a deliberate exception.")

    if not args.verbose:
        if result['matched']:
            print(f"BYTE-PERFECT: {args.func_name}" +
                  (" (window truncated -- see above)" if gap else ""))
            print(AUDIT_REMINDER)
        elif result['reloc_only']:
            print(f"RELOC-ONLY: {args.func_name} ({result['exact_pct']:.1f}% exact)")
            print(AUDIT_REMINDER)
        else:
            print(f"MISMATCH: {args.func_name} ({result['exact_pct']:.1f}% exact, {result['n_compiled']}/{result['n_target']} insns)")

    if result.get('postproc_failed'):
        print("⚠️  POSTPROC CHAIN FAILED — the numbers above compare a "
              "PARTIALLY-processed object. Fix or remove the stale directive.")

    # AUTHORITATIVE over-masking guard: the masked verdict above can OVER-REPORT
    # reloc_only (it masks any diff that lands on a reloc-typed instruction, even
    # a genuine wrong-constant/offset codegen diff). On any reloc_only verdict,
    # settle it with the name-derived resolved link (ld computes the immediates
    # from symbol names + strict byte-compare) -- the same ground truth used to
    # re-baseline the project. Disable with --no-resolve.
    if result.get('reloc_only') and not args.no_resolve:
        verdict = _resolved_verdict(args.overlay.upper(), args.func_name,
                                    extra_cflags=extra_cflags,
                                    src_override=args.src)
        if gap:
            verdict = verdict.replace(
                "TRUE WIN — record it.",
                "BUT THE WINDOW IS TRUNCATED (see above) -- NOT a true win.")
        print(verdict)

    if gap:
        sys.exit(1)
    sys.exit(0 if result['matched'] or result['reloc_only'] else 1)


def _resolved_verdict(overlay, func, extra_cflags=None, src_override=None):
    """Authoritative resolved-link check for a masked reloc_only verdict."""
    try:
        import resolve_overlay_relocs as R
    except Exception:
        sys.path.insert(0, str(Path(__file__).resolve().parent))
        try:
            import resolve_overlay_relocs as R
        except Exception as e:  # noqa: BLE001
            return f"  [resolved-link check unavailable: {e}]"
    try:
        r = R.resolve_function(overlay, func, extra_cflags=extra_cflags,
                               src_override=src_override)
    except Exception as e:  # noqa: BLE001
        return f"  [resolved-link check failed: {e}]"
    line = "  ── AUTHORITATIVE resolved-link verdict ─────────────────────────"
    if 'error' in r:
        return f"{line}\n  [resolve error: {r['error']}]"
    if r.get('matched'):
        return (f"{line}\n  ✅ BYTE-PERFECT after name-derived link (0 unresolved). "
                f"TRUE WIN — record it.")
    unresolved = r.get('unresolved') or []
    if unresolved:
        return (f"{line}\n  ◐ RELOC-ONLY, .rodata-blocked (unresolved: "
                f"{','.join(unresolved)}). Effective & legit — the real build "
                f"resolves it; not promotable to byte_perfect without a rodata map.")
    return (f"{line}\n  ⚠️  NOT A WIN — still differs after resolving ALL named "
            f"symbols ({len(r.get('diffs_after', []))} real codegen diffs). The masked "
            f"reloc_only verdict was MISLEADING; this is honestly SEMI. Keep working on the C.")


if __name__ == '__main__':
    main()
