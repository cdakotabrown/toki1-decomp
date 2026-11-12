#!/usr/bin/env python3
"""
ido_diff.py - IDO 5.2 based comparison tool for Tokimeki Memorial decompilation

This tool compiles C source with IDO 5.2, applies appropriate post-processing
based on region, and shows a detailed diff against the original binary.

Features:
- Auto-detects region from function address
- Applies correct post-processing (frame patching for game region, move encoding for library)
- Shows side-by-side instruction diff with color coding
- Automatically tries register swapping if match % > 80%
- Extracts function info from level3_functions.json

Usage:
    python3 tools/decomp/ido_diff.py func_80077D6C                    # auto-find C in wip/
    python3 tools/decomp/ido_diff.py func_80077D6C -c /tmp/test.c     # specific C file
    python3 tools/decomp/ido_diff.py func_80077D6C --loopunroll0      # disable unrolling
"""

import argparse
import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Dict, List, Optional, Tuple

# Project paths
ROOT_DIR = Path(__file__).parent.parent.parent
ORIGINAL_EXE = ROOT_DIR / "disks" / "SLPS_000.65"
LEVEL3_CONFIG = ROOT_DIR / "config" / "level3_functions.json"
IDO_COMPILER = ROOT_DIR / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"

# Constants
TEXT_SECTION_START = 0x80041000
FILE_OFFSET_BASE = 0x800
GAME_REGION_END = 0x80086000

# ANSI color codes
class Colors:
    RED = '\033[91m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    BLUE = '\033[94m'
    MAGENTA = '\033[95m'
    CYAN = '\033[96m'
    WHITE = '\033[97m'
    BOLD = '\033[1m'
    RESET = '\033[0m'

USE_COLORS = sys.stdout.isatty()


def color(text: str, color_code: str) -> str:
    """Apply color to text if colors are enabled."""
    if USE_COLORS:
        return f"{color_code}{text}{Colors.RESET}"
    return text


def load_level3_config() -> Dict:
    """Load the level3 functions configuration."""
    if not LEVEL3_CONFIG.exists():
        print(f"Error: Level3 config not found at {LEVEL3_CONFIG}")
        sys.exit(1)

    with open(LEVEL3_CONFIG, 'r') as f:
        return json.load(f)


def get_function_info(func_name: str) -> Optional[Dict]:
    """Get function metadata from level3_functions.json."""
    config = load_level3_config()

    for func in config.get('functions', []):
        if func['name'] == func_name:
            # If size is 0, try to get size from ASM file header
            if func.get('size', 0) == 0:
                func['size'] = get_size_from_asm(func_name)
            return func
    return None


def get_size_from_asm(func_name: str) -> int:
    """Extract function size from ASM file header comment."""
    asm_path = ROOT_DIR / "asm" / "main" / "matched" / f"{func_name}.s"
    if not asm_path.exists():
        return 0
    try:
        with open(asm_path, 'r') as f:
            for line in f:
                match = re.search(r'Size:\s*(\d+)\s*bytes', line)
                if match:
                    return int(match.group(1))
                if not line.startswith(('#', '/', '*', ' ')):
                    break
        return 0
    except Exception:
        return 0


def vram_to_file_offset(vram_addr: int) -> int:
    """Convert VRAM address to file offset."""
    return (vram_addr - TEXT_SECTION_START) + FILE_OFFSET_BASE


def extract_original_bytes(vram_start: int, size: int) -> bytes:
    """Extract function bytes from the original executable."""
    file_offset = vram_to_file_offset(vram_start)

    with open(ORIGINAL_EXE, 'rb') as f:
        f.seek(file_offset)
        return f.read(size)


def find_source_file(func_name: str, provided_path: Optional[str]) -> Optional[Path]:
    """Find the C source file for the function."""
    if provided_path:
        path = Path(provided_path)
        if path.exists():
            return path
        print(f"Error: Provided source file not found: {provided_path}")
        return None

    # Check src/main/wip/
    wip_path = ROOT_DIR / "src" / "main" / "wip" / f"{func_name}.c"
    if wip_path.exists():
        return wip_path

    # Check /tmp/
    tmp_path = Path(f"/tmp/{func_name}.c")
    if tmp_path.exists():
        return tmp_path

    print(f"Error: Could not find source file for {func_name}")
    print(f"  Checked: {wip_path}")
    print(f"  Checked: {tmp_path}")
    print(f"  Use -c to specify a custom path")
    return None


# Directive lines only OPEN with the keyword (after comment markers). Prose
# that quotes "@CompileFlags:" mid-sentence must not be parsed as a directive.
_COMPILEFLAGS_LINE = re.compile(
    r'^\s*(?:(?:/\*+|\*+|//)\s*)*@CompileFlags:\s*(.*)$')


def parse_compile_flags_from_source(source_file: Path):
    """Parse @CompileFlags from source file header."""
    flags = []
    try:
        with open(source_file) as f:
            for line in f:
                if line.strip() == '*/':
                    break
                m = _COMPILEFLAGS_LINE.match(line)
                if m:
                    flag_str = m.group(1).strip().rstrip('*').strip()
                    flags.extend(flag_str.split())
    except Exception:
        pass
    return flags

def compile_with_ido(source_file: Path, output_obj: Path, loopunroll0: bool = False) -> bool:
    """Compile C source with IDO 5.2."""
    # This tool is IDO-only. A `@Compiler: gcc*` file compiled with IDO
    # produces a completely different body, and the diff it prints is
    # meaningless noise that has been mistaken for a codegen blocker.
    # Refuse rather than lie; test_single_match.py drives both compilers.
    try:
        with open(source_file) as f:
            head = f.read(20000)
    except Exception:
        head = ''
    m = re.search(r'^\s*(?:(?:/\*+|\*+|//)\s*)*@Compiler:\s*(\S+)', head, re.M)
    if m and m.group(1).strip().rstrip('*/').startswith('gcc'):
        print(color(f"Refusing: {source_file} is marked @Compiler: "
                    f"{m.group(1).strip().rstrip('*/')} — ido_diff.py only "
                    f"drives IDO 5.2.", Colors.RED + Colors.BOLD))
        print("Use: tools/analysis/test_single_match.py <FUNC> "
              "(drives PSY-Q GCC via tools/gcc/gccwrap.py)")
        return False

    if not IDO_COMPILER.exists():
        print(f"Error: IDO compiler not found at {IDO_COMPILER}")
        print("Run: make setup-ido52")
        return False

    cmd = [
        str(IDO_COMPILER),
        "-O2",
        "-G0",
        "-mips1",
        "-EL",  # CRITICAL: Little-endian for PS1
        "-non_shared",
    ]

    if loopunroll0:
        cmd.extend(["-Wo,-loopunroll,0"])

    # Parse @CompileFlags from source
    extra_flags = parse_compile_flags_from_source(source_file)
    if extra_flags:
        cmd.extend(extra_flags)

    cmd.extend([
        "-I", str(ROOT_DIR / "include"),
        "-c", str(source_file),
        "-o", str(output_obj)
    ])

    result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode != 0:
        print(color("Compilation FAILED!", Colors.RED + Colors.BOLD))
        print(result.stderr)
        return False

    return True


def detect_frame_size(obj_file: Path) -> Optional[int]:
    """Detect frame size from compiled object by finding addiu sp,sp,-N."""
    cmd = [
        "mipsel-linux-gnu-objdump", "-d", str(obj_file)
    ]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        return None

    for line in result.stdout.split('\n'):
        if 'addiu' in line and 'sp,sp,' in line and '-' in line:
            match = re.search(r'sp,sp,(-\d+)', line)
            if match:
                return abs(int(match.group(1)))
    return None


def post_process_game_region(input_obj: Path, output_obj: Path) -> bool:
    """Apply game region post-processing (frame size increment by 16)."""
    patch_script = ROOT_DIR / "tools" / "ido" / "patch_stack_complete.py"

    # Game region adds +16 bytes to all frame sizes.
    # Detect the frame size from the compiled output and patch it.
    detected = detect_frame_size(input_obj)
    if detected:
        frame_pairs = [(detected, detected + 16)]
        # Also try exact IDO default as fallback
        if detected != 24:
            frame_pairs.append((24, 40))
    else:
        # Fallback: try common sizes
        frame_pairs = [(s, s + 16) for s in range(24, 256, 8)]

    for old, new in frame_pairs:
        cmd = [
            "python3", str(patch_script),
            str(input_obj),
            str(output_obj),
            "--old", str(old),
            "--new", str(new)
        ]

        result = subprocess.run(cmd, capture_output=True, text=True)

        if result.returncode == 0:
            print(f"  Patched frame {old} -> {new} bytes")
            return True

    # If no patching worked, just copy the file (maybe frame is already correct)
    print(color("  Warning: No frame patching applied (may not be needed)", Colors.YELLOW))
    import shutil
    shutil.copy(input_obj, output_obj)
    return True


def post_process_library_region(input_obj: Path, output_obj: Path) -> bool:
    """Apply library region post-processing (or->addu conversion)."""
    convert_script = ROOT_DIR / "tools" / "ido" / "convert_or_to_addu.py"

    cmd = [
        "python3", str(convert_script),
        str(input_obj),
        str(output_obj)
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode != 0:
        print(color("Move encoding conversion FAILED!", Colors.RED + Colors.BOLD))
        print(result.stderr)
        return False

    return True


def try_register_swap(input_obj: Path, output_obj: Path, vram_addr: int) -> bool:
    """Try automatic register swapping."""
    swap_script = ROOT_DIR / "tools" / "ido" / "swap_registers_auto.py"

    cmd = [
        "python3", str(swap_script),
        str(input_obj),
        str(output_obj),
        "--target-exe", str(ORIGINAL_EXE),
        "--address", hex(vram_addr),
        "--unidirectional"
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode != 0:
        return False

    return True


def try_scheduler_normalize(input_obj: Path, output_obj: Path, vram_addr: int) -> bool:
    """Try scheduler normalization to reorder instructions to match target."""
    normalizer_script = ROOT_DIR / "tools" / "postproc" / "scheduler_normalizer.py"

    if not normalizer_script.exists():
        return False

    import shutil
    shutil.copy2(str(input_obj), str(output_obj))

    cmd = [
        "python3", str(normalizer_script),
        str(output_obj),
        "--target", str(ORIGINAL_EXE),
        "--address", hex(vram_addr)
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)
    return result.returncode == 0


def extract_text_section(obj_file: Path) -> Optional[bytes]:
    """Extract .text section from object file."""
    bin_file = obj_file.with_suffix('.bin')

    cmd = [
        "mipsel-linux-gnu-objcopy",
        "-O", "binary",
        "-j", ".text",
        str(obj_file),
        str(bin_file)
    ]

    result = subprocess.run(cmd, capture_output=True)

    if result.returncode != 0 or not bin_file.exists():
        return None

    with open(bin_file, 'rb') as f:
        data = f.read()

    bin_file.unlink()
    return data


def disassemble_bytes(data: bytes, base_addr: int = 0) -> List[Tuple[int, str, str]]:
    """
    Disassemble raw bytes using objdump.

    Returns list of (address, hex_bytes, instruction) tuples.
    """
    with tempfile.NamedTemporaryFile(suffix='.bin', delete=False) as tmp:
        tmp.write(data)
        tmp_path = tmp.name

    try:
        cmd = [
            "mipsel-linux-gnu-objdump",
            "-D", "-b", "binary", "-m", "mips:3000",
            "--adjust-vma", hex(base_addr),
            tmp_path
        ]
        result = subprocess.run(cmd, capture_output=True, text=True)

        instructions = []
        for line in result.stdout.split('\n'):
            if ':' in line and '\t' in line:
                parts = line.strip().split('\t')
                if len(parts) >= 2:
                    addr_str = parts[0].split(':')[0].strip()
                    try:
                        addr = int(addr_str, 16)
                    except ValueError:
                        continue
                    hex_bytes = parts[1].strip()
                    # Join parts 2+ as instruction
                    instr = '\t'.join(parts[2:]).strip() if len(parts) > 2 else ""
                    instructions.append((addr, hex_bytes, instr))

        return instructions
    finally:
        os.unlink(tmp_path)


def disassemble_obj(obj_file: Path, base_addr: int = 0) -> List[Tuple[int, str, str]]:
    """
    Disassemble an object file, showing relocations as notes.

    Returns list of (address, hex_bytes, instruction) tuples.
    """
    cmd = [
        "mipsel-linux-gnu-objdump",
        "-d", "-r",  # Include relocations
        "--adjust-vma", hex(base_addr),
        str(obj_file)
    ]
    result = subprocess.run(cmd, capture_output=True, text=True)

    instructions = []
    for line in result.stdout.split('\n'):
        # Skip relocation lines (they start with whitespace and contain R_MIPS)
        if 'R_MIPS' in line:
            continue
        if ':' in line and '\t' in line:
            parts = line.strip().split('\t')
            if len(parts) >= 2:
                addr_str = parts[0].split(':')[0].strip()
                try:
                    addr = int(addr_str, 16)
                except ValueError:
                    continue
                hex_bytes = parts[1].strip()
                # Join parts 2+ as instruction
                instr = '\t'.join(parts[2:]).strip() if len(parts) > 2 else ""
                instructions.append((addr, hex_bytes, instr))

    return instructions


def normalize_instruction(instr: str) -> str:
    """Normalize instruction for comparison (handle relocations)."""
    # Remove relocation markers
    instr = re.sub(r'<[^>]+>', '', instr)
    # Normalize whitespace
    instr = ' '.join(instr.split())
    return instr.strip()


def compare_instructions(instr1: str, instr2: str) -> Tuple[bool, str]:
    """
    Compare two instructions.

    Returns (is_match, difference_type).
    difference_type: 'match', 'opcode_diff', 'register_diff', 'value_diff', 'relocation'
    """
    # Normalize
    i1 = normalize_instruction(instr1)
    i2 = normalize_instruction(instr2)

    if i1 == i2:
        return True, 'match'

    # Check if only difference is relocation (jal/lui/addiu with unresolved addr)
    # These would resolve correctly during linking, so count as match
    opcode1 = i1.split()[0] if i1 else ""
    opcode2 = i2.split()[0] if i2 else ""

    if opcode1 == opcode2 and opcode1 in ('jal', 'b', 'beq', 'bne', 'beqz', 'bnez',
                                            'bgez', 'bltz', 'blez', 'bgtz'):
        if '<' in instr2 or 'R_MIPS' in instr2:
            return True, 'relocation'

    if opcode1 == opcode2 and opcode1 in ('lui', 'addiu', 'ori', 'lw', 'sw', 'lh',
                                           'sh', 'lb', 'sb', 'lbu', 'lhu'):
        if '0x0' in i2 or '<' in instr2 or 'R_MIPS' in instr2:
            return True, 'relocation'

    if 'R_MIPS' in instr2 or '<' in instr2:
        return False, 'relocation'

    if opcode1 != opcode2:
        return False, 'opcode_diff'

    # Check if only registers differ
    regs1 = set(re.findall(r'\$\w+', i1))
    regs2 = set(re.findall(r'\$\w+', i2))

    if regs1 != regs2:
        return False, 'register_diff'

    return False, 'value_diff'


def diff_instructions(orig: List[Tuple[int, str, str]],
                      comp: List[Tuple[int, str, str]]) -> Tuple[int, int, List]:
    """
    Diff two instruction lists.

    Returns (matching_count, total_count, differences).
    differences is list of (idx, orig_instr, comp_instr, diff_type).
    """
    max_len = max(len(orig), len(comp))
    matching = 0
    differences = []

    for i in range(max_len):
        orig_addr, orig_hex, orig_instr = orig[i] if i < len(orig) else (0, "", "")
        comp_addr, comp_hex, comp_instr = comp[i] if i < len(comp) else (0, "", "")

        if not orig_instr and not comp_instr:
            continue

        is_match, diff_type = compare_instructions(orig_instr, comp_instr)

        if is_match:
            matching += 1

        differences.append((i, orig_addr, orig_hex, orig_instr, comp_hex, comp_instr, diff_type))

    return matching, max_len, differences


def print_diff(differences: List, show_all: bool = False) -> None:
    """Print instruction diff with color coding."""
    print(color("\nInstruction Comparison:", Colors.BOLD))
    print("=" * 100)
    print(f"{'Idx':>4}  {'Addr':>10}  {'Original':<40}  {'Compiled':<40}")
    print("=" * 100)

    last_shown = -100

    for i, (idx, orig_addr, orig_hex, orig_instr, comp_hex, comp_instr, diff_type) in enumerate(differences):
        # Decide whether to show this line
        show = show_all or diff_type != 'match'

        if not show:
            # Check if next few lines have differences
            for j in range(i, min(i + 3, len(differences))):
                if differences[j][6] != 'match':
                    show = True
                    break

        if show:
            # Show ellipsis if we skipped lines
            if i > last_shown + 1 and last_shown >= 0:
                print(color("  ...", Colors.WHITE))

            addr_str = f"0x{orig_addr:08x}" if orig_addr else " " * 10
            orig_str = orig_instr if orig_instr else "(missing)"
            comp_str = comp_instr if comp_instr else "(missing)"

            # Color based on diff type
            if diff_type == 'match':
                line_color = Colors.GREEN
                marker = "  "
            elif diff_type == 'relocation':
                line_color = Colors.YELLOW
                marker = "~ "
            elif diff_type == 'register_diff':
                line_color = Colors.YELLOW
                marker = "! "
            else:
                line_color = Colors.RED
                marker = "X "

            print(color(f"{marker}{idx:>3}  {addr_str}  {orig_str:<40}  {comp_str:<40}", line_color))
            last_shown = i

    if last_shown < len(differences) - 1:
        print(color("  ...", Colors.WHITE))

    print("=" * 100)


def ido_diff(func_name: str, source_path: Optional[str] = None,
             loopunroll0: bool = False, verbose: bool = False,
             no_postproc: bool = False) -> bool:
    """
    Perform IDO-based diff on a function.

    Returns True if perfect match.
    """
    # Get function info
    func_info = get_function_info(func_name)
    if not func_info:
        print(f"Error: Function '{func_name}' not found in level3_functions.json")
        return False

    vram_addr = int(func_info['address'], 16)
    size = func_info['size']
    region = func_info.get('region', 'UNKNOWN')

    # Determine region from address if not in config
    is_game_region = vram_addr < GAME_REGION_END
    region_name = "Game" if is_game_region else "Library"

    # Header
    print(color("=" * 100, Colors.BOLD))
    print(color(f"IDO DIFF: {func_name}", Colors.BOLD + Colors.CYAN))
    print(color("=" * 100, Colors.BOLD))
    print()
    print(f"Address:     0x{vram_addr:08x}")
    print(f"Size:        {size} bytes ({size // 4} instructions)")
    print(f"Region:      {region_name} (0x{vram_addr:08x} {'<' if is_game_region else '>='} 0x{GAME_REGION_END:08x})")

    # Find source file
    source_file = find_source_file(func_name, source_path)
    if not source_file:
        return False

    print(f"Source:      {source_file}")

    # Create temp directory
    temp_dir = Path(tempfile.mkdtemp(prefix="ido_diff_"))

    try:
        # Step 1: Compile with IDO
        print(color("\n[1/4] Compiling with IDO 5.2...", Colors.CYAN))
        compiled_obj = temp_dir / f"{func_name}.o"

        if not compile_with_ido(source_file, compiled_obj, loopunroll0):
            return False

        print(color("  ✓ Compilation successful", Colors.GREEN))

        # Step 2: Post-process based on region
        processed_obj = temp_dir / f"{func_name}_pp.o"

        if no_postproc:
            print(color("\n[2/4] Post-processing skipped (--no-postproc)", Colors.CYAN))
            import shutil
            shutil.copy(compiled_obj, processed_obj)
        else:
            print(color(f"\n[2/4] Post-processing ({region_name} region)...", Colors.CYAN))
            if is_game_region:
                print("  Patching frame size 24 -> 40 bytes...")
                if not post_process_game_region(compiled_obj, processed_obj):
                    return False
            else:
                print("  Converting move encoding (or -> addu)...")
                if not post_process_library_region(compiled_obj, processed_obj):
                    return False

            print(color("  ✓ Post-processing successful", Colors.GREEN))

        # Step 3: Extract and compare
        print(color("\n[3/4] Extracting and disassembling...", Colors.CYAN))

        # Extract original bytes
        original_bytes = extract_original_bytes(vram_addr, size)
        print(f"  Original: {len(original_bytes)} bytes")

        # Extract compiled bytes
        compiled_bytes = extract_text_section(processed_obj)
        if not compiled_bytes:
            print(color("  Failed to extract .text section", Colors.RED))
            return False

        print(f"  Compiled: {len(compiled_bytes)} bytes")

        # Disassemble
        orig_instrs = disassemble_bytes(original_bytes, vram_addr)
        comp_instrs = disassemble_obj(processed_obj, vram_addr)

        print(f"  Original: {len(orig_instrs)} instructions")
        print(f"  Compiled: {len(comp_instrs)} instructions")

        # Compare
        matching, total, differences = diff_instructions(orig_instrs, comp_instrs)
        match_percent = (matching / total * 100) if total > 0 else 0

        # Print diff
        print_diff(differences, show_all=verbose)

        # Summary
        print()
        print(color("=" * 50, Colors.BOLD))

        if original_bytes == compiled_bytes:
            print(color("  PERFECT MATCH! ✓", Colors.GREEN + Colors.BOLD))
            print(color("=" * 50, Colors.BOLD))
            print(
                "  HONESTY AUDIT before recording this win: a 'frankenword'\n"
                "  (bytes match, semantics differ) is a FAILURE. Confirm NO\n"
                "  banned PostProc (patch_insn_word/insert_*/prepend_thunk/\n"
                "  append_trailing_instructions/epilogue_share) and NO painting\n"
                "  custom_script (write of hardcoded instruction words); the C\n"
                "  must be a faithful decompilation. If forced, revert -> BLOCKER.")
            return True
        else:
            print(color(f"  Match: {matching}/{total} instructions ({match_percent:.1f}%)", Colors.YELLOW + Colors.BOLD))
            print(color("=" * 50, Colors.BOLD))

            # Try register swap if close
            if (not no_postproc) and match_percent > 80 and match_percent < 100:
                print(color("\n[4/4] Trying automatic register swap...", Colors.CYAN))

                swapped_obj = temp_dir / f"{func_name}_swapped.o"
                if try_register_swap(processed_obj, swapped_obj, vram_addr):
                    print(color("  ✓ Register swap applied", Colors.GREEN))

                    # Extract and compare again
                    swapped_bytes = extract_text_section(swapped_obj)
                    if swapped_bytes:
                        if swapped_bytes == original_bytes:
                            print(color("\n  PERFECT MATCH after register swap! ✓", Colors.GREEN + Colors.BOLD))
                            print(f"\n  Use: tools/ido/swap_registers_auto.py")
                            return True
                        else:
                            swap_instrs = disassemble_obj(swapped_obj, vram_addr)
                            swap_matching, swap_total, swap_diff = diff_instructions(orig_instrs, swap_instrs)
                            swap_percent = (swap_matching / swap_total * 100) if swap_total > 0 else 0

                            if swap_percent > match_percent:
                                print(color(f"  Improved: {swap_matching}/{swap_total} ({swap_percent:.1f}%)", Colors.GREEN))
                                print("\n  Diff after register swap:")
                                print_diff(swap_diff, show_all=verbose)
                            else:
                                print(color("  No improvement from register swap", Colors.YELLOW))
                else:
                    print(color("  Register swap failed", Colors.YELLOW))

            # Try scheduler normalization if not yet perfect
            best_obj = processed_obj
            best_pct = match_percent

            # Use swapped version if it improved things
            if (not no_postproc) and match_percent > 80 and match_percent < 100:
                swapped_test = temp_dir / f"{func_name}_swapped.o"
                if swapped_test.exists():
                    sw_bytes = extract_text_section(swapped_test)
                    if sw_bytes:
                        sw_instrs = disassemble_obj(swapped_test, vram_addr)
                        sw_m, sw_t, _ = diff_instructions(orig_instrs, sw_instrs)
                        sw_pct = (sw_m / sw_t * 100) if sw_t > 0 else 0
                        if sw_pct > best_pct:
                            best_obj = swapped_test
                            best_pct = sw_pct

            if (not no_postproc) and best_pct > 50 and best_pct < 100:
                print(color("\n[5/4] Trying scheduler normalization...", Colors.CYAN))

                normalized_obj = temp_dir / f"{func_name}_norm.o"
                if try_scheduler_normalize(best_obj, normalized_obj, vram_addr):
                    norm_bytes = extract_text_section(normalized_obj)
                    if norm_bytes:
                        if norm_bytes == original_bytes:
                            print(color("\n  PERFECT MATCH after scheduler normalization! ✓", Colors.GREEN + Colors.BOLD))
                            print(f"\n  Use: tools/postproc/scheduler_normalizer.py")
                            return True

                        norm_instrs = disassemble_obj(normalized_obj, vram_addr)
                        norm_matching, norm_total, norm_diff = diff_instructions(orig_instrs, norm_instrs)
                        norm_percent = (norm_matching / norm_total * 100) if norm_total > 0 else 0

                        if norm_percent > best_pct:
                            print(color(f"  Improved: {norm_matching}/{norm_total} ({norm_percent:.1f}%)", Colors.GREEN))
                            print("\n  Diff after normalization:")
                            print_diff(norm_diff, show_all=verbose)
                        else:
                            print(color("  No improvement from scheduler normalization", Colors.YELLOW))
                else:
                    print(color("  Scheduler normalization not applicable", Colors.YELLOW))

            return False

    finally:
        # Cleanup temp directory
        import shutil
        shutil.rmtree(temp_dir, ignore_errors=True)


def main():
    parser = argparse.ArgumentParser(
        description="IDO 5.2 based comparison tool for Tokimeki Memorial decompilation",
        epilog="""
Examples:
    python3 tools/decomp/ido_diff.py func_80077D6C
    python3 tools/decomp/ido_diff.py func_80077D6C -c /tmp/test.c
    python3 tools/decomp/ido_diff.py func_80077D6C --loopunroll0
        """,
        formatter_class=argparse.RawDescriptionHelpFormatter
    )

    parser.add_argument(
        'function',
        help='Function name to compare (e.g., func_80077D6C)'
    )
    parser.add_argument(
        '-c', '--source',
        help='Path to C source file (default: auto-find in wip/ or /tmp/)'
    )
    parser.add_argument(
        '--loopunroll0',
        action='store_true',
        help='Disable loop unrolling (-Wo,-loopunroll,0)'
    )
    parser.add_argument(
        '--no-postproc',
        action='store_true',
        help='Skip frame/move post-processing and diff raw IDO output'
    )
    parser.add_argument(
        '-v', '--verbose',
        action='store_true',
        help='Show all instructions (not just differences)'
    )

    args = parser.parse_args()

    is_match = ido_diff(
        args.function,
        source_path=args.source,
        loopunroll0=args.loopunroll0,
        verbose=args.verbose,
        no_postproc=args.no_postproc
    )

    sys.exit(0 if is_match else 1)


if __name__ == "__main__":
    main()
