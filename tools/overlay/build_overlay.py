#!/usr/bin/env python3
"""
build_overlay.py - Build an EXN overlay from C source

Compiles each C function with IDO 5.2, applies PostProc transformations,
links with the overlay linker script, and adds the EXN checksum trailer.

Usage:
    python3 tools/overlay/build_overlay.py OPTION
    python3 tools/overlay/build_overlay.py --all
    python3 tools/overlay/build_overlay.py --verify OPTION  # Check insn counts

Output:
    build/overlay/OPTION/OPTION.EXN
"""

import os
import re
import sys
import struct
import subprocess
from pathlib import Path
from typing import List, Optional, Dict, Any

# Project root
ROOT = Path(__file__).parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

# Import unified PostProc module
from overlay_postproc import (
    parse_postproc as parse_postproc_unified,
    apply_postproc as apply_postproc_unified,
    get_func_info_from_sym
)
# Prose-immune directive-line matcher (single source of truth in
# tools/build/parse_postproc.py). Without it, a header that merely mentions
# "@CompileFlags:" in prose feeds garbage words to the compiler
# ("uget: input file not initialized").
sys.path.insert(0, str(ROOT / "tools" / "build"))
from parse_postproc import directive_bodies
import overlay_names
from ido_flags import base_flags, pic_flags

# Honest relocation resolution: name-encoded symbol addresses (see
# resolve_overlay_relocs.py). Each
# extern func_/D_ symbol carries its true RAM address in its NAME, so we define
# it at that address and let ld apply R_MIPS_26/HI16/LO16 for real — NOT
# copy-from-target. This is what turns reloc_only -> byte_perfect at link time.
from resolve_overlay_relocs import (
    get_reloc_symbols,
    name_to_addr,
)

# Paths
SRC_OVERLAY_DIR = ROOT / "src" / "overlay"
ASM_OVERLAY_DIR = ROOT / "asm" / "overlay"
BUILD_DIR = ROOT / "build" / "overlay"
CONFIG_DIR = ROOT / "config" / "overlay"
INCLUDE_DIR = ROOT / "include"

# Tools
IDO_CC = ROOT / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
GCCWRAP = ROOT / "tools" / "gcc" / "gccwrap.py"
MIPS_LD = "mipsel-linux-gnu-ld"  # Little-endian for PS1
MIPS_OBJCOPY = "mipsel-linux-gnu-objcopy"
MIPS_OBJDUMP = "mipsel-linux-gnu-objdump"

# PostProc tools
STRIP_TRAILING = ROOT / "tools" / "ido" / "strip_trailing_instructions.py"
STRIP_LEADING = ROOT / "tools" / "ido" / "strip_leading_instructions.py"
PATCH_STACK = ROOT / "tools" / "ido" / "patch_stack_complete.py"
SWAP_REGISTERS = ROOT / "tools" / "ido" / "swap_registers_auto.py"
SCHEDULER_NORMALIZER = ROOT / "tools" / "postproc" / "scheduler_normalizer.py"
PAD_TRAILING_NOP = ROOT / "tools" / "ido" / "pad_trailing_nop.py"
CONVERT_ORI_TO_ADDIU = ROOT / "tools" / "ido" / "convert_ori_to_addiu.py"

# Compile flags. Base line from tools/build/ido_flags.py (one source of
# truth); the per-file PIC mode is added at the compile site via pic_flags().
IDO_FLAGS = base_flags([INCLUDE_DIR])

# Overlay base address
OVERLAY_BASE = 0x80132000


def parse_compile_flags(c_file: Path) -> List[str]:
    """Parse @CompileFlags annotation from C file header."""
    try:
        with open(c_file, 'r') as f:
            content = f.read()
    except Exception:
        return []

    # Find a real @CompileFlags directive line (not a prose mention)
    for body in directive_bodies(content, '@CompileFlags:'):
        line = body.strip()
        # Remove trailing comment chars
        line = re.sub(r'\s*\*+/?\s*$', '', line)
        # Split on whitespace and return flags
        return line.split()
    return []


def parse_compile_env(c_file: Path) -> Dict[str, str]:
    """Parse @CompileEnv annotation from C file header.

    Looks for a line like:
        * @CompileEnv: IDO52_CONST_NOINREG_VALUES=32 IDO52_NO_LICM=1

    Returns a dict of KEY=VALUE pairs (empty if none).
    """
    try:
        with open(c_file, 'r') as f:
            content = f.read()
    except Exception:
        return {}

    env_vars = {}
    for body in directive_bodies(content, '@CompileEnv:'):
        env_str = re.sub(r'\s*\*+/?\s*$', '', body.strip())
        if not env_str:
            continue
        for token in env_str.split():
            if '=' in token:
                key, value = token.split('=', 1)
                env_vars[key] = value
    # 2026-10-03 (red-team finding 5): IDO52_SKIP_PASSES=...codemotion
    # MISCOMPILES (deletes loads; the shrink is a count compensator --
    # CLAUDE.md, UOPT-HOOKS-REFERENCE.md Hook 15) and nothing refused it.
    if 'codemotion' in env_vars.get('IDO52_SKIP_PASSES', ''):
        raise SystemExit(
            "BANNED @CompileEnv in {}: IDO52_SKIP_PASSES={} -- codemotion "
            "miscompiles (Hook 15); remove it.".format(c_file, env_vars['IDO52_SKIP_PASSES']))
    return env_vars


def parse_postproc(c_file: Path) -> List[Dict[str, Any]]:
    """Parse @PostProc annotations from C file header."""
    steps = []
    try:
        with open(c_file, 'r') as f:
            content = f.read()
    except Exception:
        return steps

    # Find real @PostProc directive lines (not prose mentions)
    for body in directive_bodies(content, '@PostProc:'):
        line = body.strip()
        # Remove trailing comment chars
        line = re.sub(r'\s*\*+/?\s*$', '', line)

        # Handle strip_trailing_nop (shorthand) and strip_trailing_instructions (full)
        if 'strip_trailing_nop' in line or 'strip_trailing_instructions' in line:
            m = re.search(r'--count\s+(\d+)', line)
            count = int(m.group(1)) if m else 1
            steps.append({'type': 'strip_trailing', 'count': count})

        elif 'strip_leading' in line:
            m = re.search(r'--count\s+(\d+)', line)
            count = int(m.group(1)) if m else 1
            steps.append({'type': 'strip_leading', 'count': count})

        elif 'patch_stack' in line:
            old_m = re.search(r'--old\s+(\d+)', line)
            new_m = re.search(r'--new\s+(\d+)', line)
            if old_m and new_m:
                steps.append({
                    'type': 'patch_stack',
                    'old': int(old_m.group(1)),
                    'new': int(new_m.group(1))
                })

        elif 'scheduler_normalizer' in line:
            steps.append({'type': 'scheduler_normalizer'})

        elif 'pad_trailing_nop' in line:
            m = re.search(r'--count\s+(\d+)', line)
            count = int(m.group(1)) if m else 1
            steps.append({'type': 'pad_trailing', 'count': count})

        elif 'convert_ori_to_addiu' in line:
            steps.append({'type': 'convert_ori_to_addiu'})

        elif 'swap_registers' in line:
            # Parse: swap_registers FROM:TO [--unidirectional]
            m = re.search(r'swap_registers\s+(\w+)[:\s]+(\w+)', line)
            if m:
                unidirectional = '--unidirectional' in line
                steps.append({
                    'type': 'swap_registers',
                    'from_reg': m.group(1),
                    'to_reg': m.group(2),
                    'unidirectional': unidirectional
                })

        elif 'prepend_thunk' in line:
            # prepend_thunk --insns 0xXXXXXXXX adds 1 instruction
            steps.append({'type': 'prepend_thunk'})

    return steps


def get_target_insn_count(asm_file: Path) -> int:
    """Extract target instruction count from ASM file header."""
    try:
        with open(asm_file, 'r') as f:
            content = f.read(500)
        match = re.search(r'Size: \d+ bytes \((\d+) instructions?\)', content)
        if match:
            return int(match.group(1))
    except Exception:
        pass
    return 0


def get_obj_insn_count(obj_file: Path) -> int:
    """Count instructions in compiled object file."""
    try:
        result = subprocess.run(
            [MIPS_OBJDUMP, "-d", str(obj_file)],
            capture_output=True, text=True
        )
        # Count instruction lines (format: "   addr:  bytes  mnemonic")
        return len(re.findall(r'^\s+[0-9a-f]+:', result.stdout, re.MULTILINE))
    except Exception:
        return 0


def check_tools() -> bool:
    """Check required tools are available"""
    if not IDO_CC.exists():
        print(f"[ERROR] IDO compiler not found: {IDO_CC}")
        print("Run: make setup-ido52")
        return False

    # Check for MIPS linker
    try:
        subprocess.run([MIPS_LD, "--version"], capture_output=True, check=True)
    except (subprocess.CalledProcessError, FileNotFoundError):
        print(f"[ERROR] MIPS linker not found: {MIPS_LD}")
        print("Install: sudo apt install binutils-mips-linux-gnu")
        return False

    return True


def apply_postproc(obj_file: Path, steps: List[Dict], verbose: bool = False) -> bool:
    """Apply PostProc steps to compiled object file."""
    if not steps:
        return True

    current = obj_file
    for i, step in enumerate(steps):
        step_type = step['type']
        next_obj = obj_file.with_suffix(f'.pp{i}.o')

        if step_type == 'strip_trailing':
            count = step['count']
            cmd = [
                sys.executable, str(STRIP_TRAILING),
                str(current), str(next_obj),
                "--count", str(count), "--nop-only"
            ]
            if verbose:
                print(f"    PostProc: strip_trailing --count {count}")

        elif step_type == 'strip_leading':
            count = step['count']
            cmd = [
                sys.executable, str(STRIP_LEADING),
                str(current), str(next_obj),
                "--count", str(count)
            ]
            if verbose:
                print(f"    PostProc: strip_leading --count {count}")

        elif step_type == 'patch_stack':
            cmd = [
                sys.executable, str(PATCH_STACK),
                str(current), str(next_obj),
                "--old", str(step['old']),
                "--new", str(step['new'])
            ]
            if verbose:
                print(f"    PostProc: patch_stack --old {step['old']} --new {step['new']}")

        elif step_type == 'scheduler_normalizer':
            # Need target ASM file for scheduler_normalizer
            # Skip for now - requires more infrastructure
            if verbose:
                print(f"    PostProc: scheduler_normalizer (skipped - needs target)")
            continue

        elif step_type == 'pad_trailing':
            count = step['count']
            cmd = [
                sys.executable, str(PAD_TRAILING_NOP),
                str(current), str(next_obj),
                "--count", str(count)
            ]
            if verbose:
                print(f"    PostProc: pad_trailing_nop --count {count}")

        elif step_type == 'convert_ori_to_addiu':
            cmd = [
                sys.executable, str(CONVERT_ORI_TO_ADDIU),
                str(current), str(next_obj)
            ]
            if verbose:
                print(f"    PostProc: convert_ori_to_addiu")

        elif step_type == 'swap_registers':
            from_reg = step['from_reg']
            to_reg = step['to_reg']
            unidirectional = step.get('unidirectional', False)
            cmd = [
                sys.executable, str(SWAP_REGISTERS),
                str(current), str(next_obj),
                '--swaps', f"{from_reg}:{to_reg}"
            ]
            if unidirectional:
                cmd.append('--unidirectional')
            if verbose:
                uni_str = " (unidirectional)" if unidirectional else ""
                print(f"    PostProc: swap_registers {from_reg}:{to_reg}{uni_str}")

        else:
            if verbose:
                print(f"    PostProc: unknown type {step_type}")
            continue

        try:
            result = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
            if result.returncode != 0:
                if verbose:
                    print(f"    PostProc FAILED: {result.stderr[:100]}")
                return False
            current = next_obj
        except Exception as e:
            if verbose:
                print(f"    PostProc ERROR: {e}")
            return False

    # Copy final result back to original name
    if current != obj_file:
        import shutil
        shutil.copy(current, obj_file)

    return True


def compile_function(c_file: Path, obj_dir: Path, apply_pp: bool = True,
                    verbose: bool = False, overlay_name: str = None) -> Optional[Path]:
    """Compile a single C file with IDO and apply PostProc.

    If the C file has `Status: ASM-ONLY` in its header, skip C compilation and
    instead extract the original function bytes from the source EXN, wrapping
    them in a minimal ELF .o that the linker can splice in. This handles
    hand-written ASM (raw .word, $at use, BGEZAL, etc.) that cannot be
    matched from C source.

    Args:
        c_file: Path to C source file
        obj_dir: Directory for output object file
        apply_pp: Whether to apply PostProc steps
        verbose: Print verbose output
        overlay_name: Overlay name (e.g., "EVENT") for PostProc context
    """
    obj_file = obj_dir / f"{c_file.stem}.o"
    func_name = c_file.stem

    # Function address: from the name (func_80XXXXXX) or, for a function that
    # has since been given a real name, from the overlay's landed-name table.
    func_addr = 0
    if overlay_name:
        func_addr = overlay_names.func_addr(overlay_name, func_name) or 0
    elif func_name.startswith("func_"):
        try:
            func_addr = int(func_name[5:], 16)
        except ValueError:
            pass

    # Check for ASM-ONLY marker — if present, use raw bytes from EXN
    try:
        c_content = c_file.read_text()
    except Exception:
        c_content = ""
    is_asm_only = "Status: ASM-ONLY" in c_content or "@AsmOnly" in c_content
    if is_asm_only and overlay_name:
        # Look up function size from sym file
        sym_file = CONFIG_DIR / f"{overlay_name}.sym"
        func_size = None
        if sym_file.exists():
            for line in sym_file.read_text().splitlines():
                line = line.strip()
                if not line or line.startswith('#'):
                    continue
                parts = line.split(':')
                if len(parts) >= 3 and parts[2] == func_name:
                    try:
                        func_size = int(parts[1])
                    except ValueError:
                        pass
                    break
        if func_size is None:
            print(f"  [ASM-ONLY-FAIL] {c_file.name}: no size in {overlay_name}.sym")
            return None
        cmd = [
            sys.executable,
            str(ROOT / "tools" / "overlay" / "build_asm_only_obj.py"),
            "--overlay", overlay_name,
            "--func", func_name,
            "--size", str(func_size),
            "--output", str(obj_file),
        ]
        try:
            result = subprocess.run(cmd, capture_output=True, text=True, timeout=10)
            if result.returncode != 0:
                print(f"  [ASM-ONLY-FAIL] {c_file.name}: {result.stderr[:200]}")
                return None
            if verbose:
                print(f"  [ASM-ONLY] {c_file.name} -> raw {func_size} bytes from EXN")
            return obj_file
        except Exception as e:
            print(f"  [ASM-ONLY-ERROR] {c_file.name}: {e}")
            return None

    # Get any custom compile flags
    extra_flags = parse_compile_flags(c_file)
    compile_env = parse_compile_env(c_file)
    compiler_m = re.search(r'@Compiler:\s*(gcc\d+)', c_content)
    if compiler_m:
        # Psy-Q GCC pipeline via gccwrap.py (cpp + cc1 + maspsx + as)
        version_number = compiler_m.group(1)[3:]  # "gcc281" -> "281"
        gccwrap_flags = {"--fill-jalr", "--fill-epilogue", "--fill-all",
                         "--no-delay-slots", "--no-cross-jumping"}
        cmd = [sys.executable, str(GCCWRAP), str(c_file),
               "-o", str(obj_file),
               "--gcc-version", version_number,
               "-I", str(INCLUDE_DIR), "-I", str(INCLUDE_DIR / "psyq")]
        cc1_extra = []
        for flag in extra_flags:
            if flag in gccwrap_flags:
                cmd.append(flag)
            else:
                cc1_extra.append(flag)
        if cc1_extra:
            cmd.extend(["--extra-flags", " ".join(cc1_extra)])
    else:
        cmd = ([str(IDO_CC)] + IDO_FLAGS
               + pic_flags(c_file, compile_flags=extra_flags)
               + extra_flags + ["-c", str(c_file), "-o", str(obj_file)])

    try:
        env = os.environ.copy()
        if compile_env:
            env.update(compile_env)
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=60, env=env)
        if result.returncode != 0:
            print(f"  [FAIL] {c_file.name}: {result.stderr[:200]}")
            return None

        # Apply PostProc if requested
        if apply_pp and overlay_name:
            steps = parse_postproc_unified(c_file)
            if steps:
                if not apply_postproc_unified(obj_file, steps, func_addr, overlay_name, verbose):
                    if verbose:
                        print(f"  [POSTPROC FAIL] {c_file.name}")
                    # Continue anyway - PostProc failure is not fatal
        elif apply_pp:
            # Fallback to old parse_postproc for backward compatibility
            steps = parse_postproc(c_file)
            if steps:
                if not apply_postproc(obj_file, steps, verbose):
                    if verbose:
                        print(f"  [POSTPROC FAIL] {c_file.name}")

        return obj_file
    except subprocess.TimeoutExpired:
        print(f"  [TIMEOUT] {c_file.name}")
        return None
    except Exception as e:
        print(f"  [ERROR] {c_file.name}: {e}")
        return None


def generate_defsym_fragment(obj_files: List[Path], out_path: Path,
                             overlay_name: Optional[str] = None) -> Dict[str, int]:
    """Scan compiled objects for relocation symbols and emit a GNU ld fragment
    defining every NAME-encoded `func_/D_/...` symbol at its true RAM address.

    This is the honest relocation resolution: a symbol's address comes only from
    its NAME (`func_80011DFC` -> 0x80011DFC), never from the target binary, so
    ld applies R_MIPS_26/HI16/LO16 for real. INCLUDE-ing this at link makes the
    JAL/%hi/%lo immediates correct -> reloc_only functions become byte_perfect.

    Returns a dict with stats: 'defined' (name-encoded symbols defined),
    'unresolved' (sorted list of non-name-encoded reloc symbols, e.g. .rodata
    or function-local statics that need the overlay runtime data base).
    """
    all_syms: Dict[str, int] = {}
    unresolved: set = set()
    for obj in obj_files:
        try:
            relocs = get_reloc_symbols(obj)
        except Exception:
            continue
        for _off, _typ, sym in relocs:
            addr = name_to_addr(sym, overlay_name)
            if addr is not None:
                all_syms[sym] = addr
            elif not sym.startswith('.'):
                # Function-local static data (D_func_X_N) etc — ld would fail on
                # these undefined externs. Define at sentinel 0 so the link
                # completes; the affected function simply cannot be byte_perfect
                # (it needs the overlay's runtime data base, handled separately).
                unresolved.add(sym)

    lines = [
        f"/* Symbol address definitions (generated by build_overlay.py). */",
        f"/* Every address is decoded from the symbol NAME, so ld resolves   */",
        f"/* cross-overlay / main-binary relocations honestly (not copied).  */",
        "",
    ]
    for sym, addr in sorted(all_syms.items()):
        lines.append(f"{sym} = 0x{addr:08X};")
    if unresolved:
        # Sentinel must sit in the same 256MB segment as the overlay (top 4
        # bits = 0x8) or an R_MIPS_26 (jal) to it truncates and fails the link.
        # 0x80000000 is wrong-on-purpose: these functions cannot be byte_perfect
        # (they reference local statics / .rodata needing the runtime data base),
        # so they land in residual — but the link still completes.
        lines.append("")
        lines.append("/* Unresolved local-data symbols, defined at segment-base */")
        lines.append("/* sentinel so the link completes; stay reloc_only/residual. */")
        for sym in sorted(unresolved):
            lines.append(f"{sym} = 0x80000000;")
    out_path.write_text("\n".join(lines) + "\n")
    return {'defined': len(all_syms), 'unresolved': sorted(unresolved)}


def link_overlay(obj_files: List[Path], linker_script: Path, output: Path,
                 obj_dir: Path, resolve: bool = True,
                 overlay_name: Optional[str] = None) -> bool:
    """Link object files into overlay binary.

    When resolve=True (default), generate a name-encoded symbol-address fragment
    and INCLUDE it so ld resolves cross-references for real (reloc_only ->
    byte_perfect). The link then runs WITHOUT --noinhibit-exec so a genuinely
    undefined symbol is a hard error. If the resolved link fails for any reason,
    we fall back to the legacy --noinhibit-exec link (leaving cross-refs at 0)
    so the build never regresses.
    """
    elf_output = output.with_suffix('.elf')

    defsym_path = obj_dir / "_resolved_symbols.ld"
    resolved_script = obj_dir / "linker_resolved.ld"

    if resolve:
        stats = generate_defsym_fragment(obj_files, defsym_path, overlay_name)
        print(f"[link] resolved {stats['defined']} name-encoded symbols"
              + (f", {len(stats['unresolved'])} unresolved local-data "
                 f"(sentinel 0x80000000 -> residual)"
                 if stats['unresolved'] else ""))
        # Build a linker script that INCLUDEs the defsym fragment. The fragment
        # must be visible at link; the simplest robust way is to prepend an
        # INCLUDE directive into a copy of the overlay linker script (top level,
        # outside SECTIONS, so the assignments are global symbol definitions).
        base = linker_script.read_text()
        resolved_script.write_text(f'INCLUDE "{defsym_path.name}"\n\n' + base)
        cmd = [
            MIPS_LD,
            "-T", str(resolved_script),
            "-L", str(obj_dir),
            "--no-warn-mismatch",
            "-o", str(elf_output),
        ] + [str(f) for f in obj_files]
        try:
            result = subprocess.run(cmd, capture_output=True, text=True,
                                    cwd=str(obj_dir))
        except Exception as e:
            result = None
            print(f"[LINK ERROR resolved] {e}")
        if result is not None and result.returncode == 0:
            return _objcopy_binary(elf_output, output)
        if result is not None:
            print(f"[LINK WARN] resolved link failed, falling back to "
                  f"--noinhibit-exec: {result.stderr.strip()[:300]}")

    # Legacy / fallback link: allow undefined symbols, leave cross-refs at 0.
    cmd = [
        MIPS_LD,
        "-T", str(linker_script),
        "-L", str(obj_dir),
        "--noinhibit-exec",  # Allow undefined symbols (external refs)
        "-o", str(elf_output),
    ] + [str(f) for f in obj_files]

    try:
        result = subprocess.run(cmd, capture_output=True, text=True, cwd=str(obj_dir))
        if result.returncode != 0:
            print(f"[LINK ERROR] {result.stderr[:500]}")
            return False
    except Exception as e:
        print(f"[LINK ERROR] {e}")
        return False

    return _objcopy_binary(elf_output, output)


def _objcopy_binary(elf_output: Path, output: Path) -> bool:
    """Extract raw .text/.data binary from the linked ELF."""

    # Extract raw binary
    cmd = [
        MIPS_OBJCOPY,
        "-O", "binary",
        str(elf_output),
        str(output.with_suffix('.bin'))
    ]

    try:
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode != 0:
            print(f"[OBJCOPY ERROR] {result.stderr[:500]}")
            return False
    except Exception as e:
        print(f"[OBJCOPY ERROR] {e}")
        return False

    return True


def add_exn_checksum(bin_file: Path, exn_file: Path) -> bool:
    """Add EXN checksum trailer and pad to 192KB"""
    try:
        data = bytearray(bin_file.read_bytes())

        # Pad to 192KB - 4 bytes (leave room for checksum)
        target_size = 192 * 1024 - 4
        if len(data) < target_size:
            data.extend([0] * (target_size - len(data)))
        elif len(data) > target_size:
            print(f"[WARN] Binary too large: {len(data)} > {target_size}")
            data = data[:target_size]

        # Calculate checksum: sum of all bytes, 16-bit, byte-swapped
        s16 = sum(data) & 0xFFFF
        swapped = ((s16 >> 8) & 0xFF) | ((s16 & 0xFF) << 8)
        checksum = 0x10120000 | swapped

        # Append checksum
        data.extend(struct.pack('<I', checksum))

        exn_file.write_bytes(data)
        return True

    except Exception as e:
        print(f"[CHECKSUM ERROR] {e}")
        return False


def build_overlay(name: str, force: bool = False) -> bool:
    """Build a single overlay"""
    print(f"\n{'='*60}")
    print(f"Building {name}.EXN")
    print(f"{'='*60}")

    src_dir = SRC_OVERLAY_DIR / name
    build_dir = BUILD_DIR / name
    obj_dir = build_dir / "obj"

    # Check source exists
    if not src_dir.exists():
        print(f"[ERROR] Source directory not found: {src_dir}")
        return False

    # Check linker script exists
    linker_script = build_dir / "linker.ld"
    if not linker_script.exists():
        print(f"[ERROR] Linker script not found: {linker_script}")
        print("Run: python3 tools/overlay/generate_linker.py --all")
        return False

    # Create object directory
    obj_dir.mkdir(parents=True, exist_ok=True)

    # Find all C files
    c_files = sorted(src_dir.glob("*.c"))
    print(f"[{name}] Found {len(c_files)} C files")

    if not c_files:
        print(f"[WARN] No C files found in {src_dir}")
        return False

    # Compile each file
    obj_files = []
    success = 0
    fail = 0

    for i, c_file in enumerate(c_files):
        if (i + 1) % 20 == 0:
            print(f"  Compiling... {i+1}/{len(c_files)}")

        obj = compile_function(c_file, obj_dir, overlay_name=name)
        if obj:
            obj_files.append(obj)
            success += 1
        else:
            fail += 1

    print(f"[{name}] Compiled {success}/{len(c_files)} ({fail} failed)")

    if fail > 0:
        print(f"[WARN] {fail} files failed to compile")
        # Continue anyway for partial build

    if not obj_files:
        print(f"[ERROR] No object files produced")
        return False

    # Sort object files by address (from filename)
    def get_addr(obj_path):
        a = overlay_names.func_addr(name, obj_path.stem)
        return a if a is not None else 0xFFFFFFFF

    obj_files.sort(key=get_addr)

    # Link
    print(f"[{name}] Linking...")
    bin_output = build_dir / f"{name}.bin"
    if not link_overlay(obj_files, linker_script, bin_output, obj_dir, overlay_name=name):
        return False

    # Add checksum
    print(f"[{name}] Adding checksum...")
    exn_output = build_dir / f"{name}.EXN"
    if not add_exn_checksum(bin_output, exn_output):
        return False

    print(f"[{name}] Built: {exn_output}")
    print(f"[{name}] Size: {exn_output.stat().st_size} bytes")

    return True


def get_all_overlays() -> List[str]:
    """Get list of all overlay names"""
    overlays = []
    if SRC_OVERLAY_DIR.exists():
        for d in SRC_OVERLAY_DIR.iterdir():
            if d.is_dir() and not d.name.startswith('.'):
                overlays.append(d.name)
    return sorted(overlays)


def verify_overlay(name: str, verbose: bool = False) -> Dict[str, int]:
    """Verify instruction counts for all functions in an overlay.

    Returns dict with 'total', 'matched', 'unmatched', 'no_target' counts.
    """
    src_dir = SRC_OVERLAY_DIR / name
    asm_dir = ASM_OVERLAY_DIR / name
    build_dir = BUILD_DIR / name
    obj_dir = build_dir / "obj"

    results = {'total': 0, 'matched': 0, 'unmatched': 0, 'no_target': 0,
               'compile_fail': 0, 'will_match': 0, 'mismatches': []}

    if not src_dir.exists():
        print(f"[ERROR] Source directory not found: {src_dir}")
        return results

    obj_dir.mkdir(parents=True, exist_ok=True)

    c_files = sorted(src_dir.glob("*.c"))
    results['total'] = len(c_files)

    for c_file in c_files:
        func_name = c_file.stem
        asm_file = asm_dir / f"{func_name}.s"

        # Get target instruction count
        target = get_target_insn_count(asm_file)
        if target == 0:
            results['no_target'] += 1
            continue

        # Get PostProc steps
        postproc_steps = parse_postproc(c_file)
        has_strip_trailing = any(s['type'] == 'strip_trailing' for s in postproc_steps)

        # Compile WITHOUT PostProc for raw count (PostProc doesn't work on obj files)
        obj = compile_function(c_file, obj_dir, apply_pp=False, verbose=False)
        if not obj:
            results['compile_fail'] += 1
            continue

        # Get compiled instruction count
        compiled = get_obj_insn_count(obj)
        diff = compiled - target

        # Check if PostProc annotation would fix the diff
        expected_diff = diff

        # prepend_thunk adds 1 instruction per --insns argument (fixes diff < 0)
        has_prepend = any(s['type'] == 'prepend_thunk' for s in postproc_steps)
        if has_prepend and diff < 0:
            prepend_count = sum(1 for s in postproc_steps if s['type'] == 'prepend_thunk')
            expected_diff = diff + prepend_count

        # strip_trailing removes instructions (fixes diff > 0)
        if has_strip_trailing and diff > 0:
            strip_count = sum(s['count'] for s in postproc_steps if s['type'] == 'strip_trailing')
            expected_diff = diff - strip_count

        # pad_trailing_nop adds instructions (fixes diff < 0)
        has_pad_trailing = any(s['type'] == 'pad_trailing' for s in postproc_steps)
        if has_pad_trailing and diff < 0:
            pad_count = sum(s['count'] for s in postproc_steps if s['type'] == 'pad_trailing')
            expected_diff = diff + pad_count

        if diff == 0:
            results['matched'] += 1
            if verbose:
                print(f"  [MATCH] {func_name}: {compiled} insns")
        elif expected_diff == 0:
            results['will_match'] += 1
            if verbose:
                print(f"  [WILL MATCH] {func_name}: {compiled} - {diff} (has PostProc)")
        else:
            results['unmatched'] += 1
            results['mismatches'].append((func_name, target, compiled, diff))
            if verbose:
                sign = '+' if diff > 0 else ''
                print(f"  [DIFF {sign}{diff}] {func_name}: target={target}, compiled={compiled}")

    return results


def print_verify_summary(name: str, results: Dict[str, int]):
    """Print verification summary for an overlay."""
    total = results['total']
    matched = results['matched']
    will_match = results.get('will_match', 0)
    unmatched = results['unmatched']
    no_target = results['no_target']
    compile_fail = results['compile_fail']

    effective_matched = matched + will_match
    pct = (effective_matched / total * 100) if total > 0 else 0
    print(f"\n[{name}] Verification Results:")
    print(f"  Total functions: {total}")
    print(f"  Matched:         {matched}")
    print(f"  Will match:      {will_match} (PostProc annotated)")
    print(f"  Effective match: {effective_matched} ({pct:.1f}%)")
    print(f"  Unmatched:       {unmatched}")
    print(f"  No target ASM:   {no_target}")
    print(f"  Compile fail:    {compile_fail}")

    # Show top mismatches by diff
    mismatches = results.get('mismatches', [])
    if mismatches:
        # Sort by absolute diff
        mismatches.sort(key=lambda x: abs(x[3]))
        print(f"\n  Closest mismatches (easiest to fix):")
        for func, target, compiled, diff in mismatches[:10]:
            sign = '+' if diff > 0 else ''
            print(f"    {sign}{diff}: {func} (target={target}, compiled={compiled})")


def main():
    if len(sys.argv) < 2:
        print("Usage: python3 tools/overlay/build_overlay.py <OVERLAY_NAME>")
        print("       python3 tools/overlay/build_overlay.py --all")
        print("       python3 tools/overlay/build_overlay.py --verify <OVERLAY_NAME>")
        print("       python3 tools/overlay/build_overlay.py --verify-all")
        print("")
        print("Available overlays:")
        for name in get_all_overlays():
            print(f"  {name}")
        sys.exit(1)

    # Check tools
    if not check_tools():
        sys.exit(1)

    if sys.argv[1] == "--verify-all":
        overlays = get_all_overlays()
        total_matched = 0
        total_will_match = 0
        total_funcs = 0
        for name in overlays:
            print(f"\n{'='*60}")
            print(f"Verifying {name}")
            print(f"{'='*60}")
            results = verify_overlay(name, verbose=False)
            print_verify_summary(name, results)
            total_matched += results['matched']
            total_will_match += results.get('will_match', 0)
            total_funcs += results['total']
        effective = total_matched + total_will_match
        print(f"\n{'='*60}")
        pct = (effective / total_funcs * 100) if total_funcs > 0 else 0
        print(f"Overall: {total_matched} matched + {total_will_match} will match = {effective}/{total_funcs} ({pct:.1f}%)")
        print(f"{'='*60}")

    elif sys.argv[1] == "--verify":
        if len(sys.argv) < 3:
            print("Usage: --verify <OVERLAY_NAME>")
            sys.exit(1)
        name = sys.argv[2].upper()
        results = verify_overlay(name, verbose=True)
        print_verify_summary(name, results)

    elif sys.argv[1] == "--all":
        overlays = get_all_overlays()
        success = 0
        for name in overlays:
            if build_overlay(name):
                success += 1
        print(f"\n{'='*60}")
        print(f"Built {success}/{len(overlays)} overlays")
        print(f"{'='*60}")

    else:
        name = sys.argv[1].upper()
        if not build_overlay(name):
            sys.exit(1)

    print("\n[DONE] Overlay build complete")


if __name__ == "__main__":
    main()
