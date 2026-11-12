#!/usr/bin/env python3
"""
quick_diff.py - Fast command-line comparison for function matching

Quick and easy comparison between original and compiled function assembly.
Shows instruction count, register usage, stack frame size, and highlights
differences in a color-coded format.

Features:
- Fast single-command comparison
- Color-coded diff output (green=match, red=mismatch)
- Stack frame analysis
- Register usage comparison
- Integration with existing build system

Usage:
    python3 tools/quick_diff.py func_8004B338
    python3 tools/quick_diff.py func_8004B338 --no-color
    python3 tools/quick_diff.py func_8004B338 --side-by-side
    python3 tools/quick_diff.py func_8004B338 --context 5
"""

import argparse
import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Dict, List, Optional, Tuple, Set

# Project paths
ROOT_DIR = Path(__file__).parent.parent
ORIGINAL_EXE = ROOT_DIR / "disks" / "SLPM_86.053"
FUNCTION_MAP = ROOT_DIR / "build" / "function_analysis" / "function_map.json"
DECOMP_CONFIG = ROOT_DIR / "config" / "decompiled_functions.json"
BUILD_DIR = ROOT_DIR / "build" / "jp"

# Constants
TEXT_SECTION_START = 0x80041000
FILE_OFFSET_BASE = 0x800

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

# Disable colors if not a TTY or on Windows without ANSI support
USE_COLORS = sys.stdout.isatty() and os.name != 'nt'


def color(text: str, color_code: str) -> str:
    """Apply color to text if colors are enabled."""
    if USE_COLORS:
        return f"{color_code}{text}{Colors.RESET}"
    return text


def load_function_map() -> Dict:
    """Load the function map."""
    if not FUNCTION_MAP.exists():
        print(f"Error: Function map not found at {FUNCTION_MAP}")
        print("Run: make analyze")
        sys.exit(1)

    with open(FUNCTION_MAP, 'r') as f:
        return json.load(f)


def load_decomp_config() -> Dict:
    """Load the decompiled functions configuration."""
    if not DECOMP_CONFIG.exists():
        return {"functions": []}

    with open(DECOMP_CONFIG, 'r') as f:
        return json.load(f)


def get_function_info(func_name: str, function_map: Dict) -> Optional[Dict]:
    """Get function metadata from the function map."""
    for func in function_map.get('functions', []):
        if func['name'] == func_name:
            return func
    return None


def vram_to_file_offset(vram_addr: int) -> int:
    """Convert VRAM address to file offset."""
    return (vram_addr - TEXT_SECTION_START) + FILE_OFFSET_BASE


def extract_original_bytes(vram_start: int, size: int) -> bytes:
    """Extract function bytes from the original executable."""
    file_offset = vram_to_file_offset(vram_start)

    with open(ORIGINAL_EXE, 'rb') as f:
        f.seek(file_offset)
        return f.read(size)


def compile_function(func_name: str, source_file: str) -> Optional[bytes]:
    """Compile a C source file and extract the function bytes."""
    source_path = ROOT_DIR / source_file
    if not source_path.exists():
        return None

    temp_dir = BUILD_DIR / "quick_diff_temp"
    temp_dir.mkdir(parents=True, exist_ok=True)

    asm_file = temp_dir / f"{func_name}.s"
    obj_file = temp_dir / f"{func_name}.o"

    # Compile C to assembly
    cpp_cmd = [
        "cpp",
        "-I", str(ROOT_DIR / "include"),
        "-Iinclude",
        "-undef", "-Wall", "-lang-c", "-fno-builtin",
        str(source_path)
    ]

    cc1_cmd = [
        str(ROOT_DIR / "tools" / "psyq-gcc-260" / "cc1-psx-26"),
        "-G0", "-O2", "-funsigned-char", "-fpeephole",
        "-ffunction-cse", "-fpcc-struct-return", "-fcommon",
        "-fverbose-asm", "-msoft-float",
        "-o", str(asm_file)
    ]

    try:
        cpp_proc = subprocess.Popen(cpp_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        cc1_proc = subprocess.Popen(cc1_cmd, stdin=cpp_proc.stdout,
                                     stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        cpp_proc.stdout.close()
        _, cc1_err = cc1_proc.communicate()

        if cc1_proc.returncode != 0:
            return None

        # Assemble (must run from project root for include paths)
        as_cmd = [
            "mipsel-linux-gnu-as",
            "-march=r3000", "-O1", "-G0",
            "-I", str(ROOT_DIR / "include"),
            "-o", str(obj_file),
            str(asm_file)
        ]

        result = subprocess.run(as_cmd, capture_output=True, cwd=str(ROOT_DIR))
        if result.returncode != 0:
            return None

        # Extract .text section
        objcopy_cmd = [
            "mipsel-linux-gnu-objcopy",
            "-O", "binary",
            "-j", ".text",
            str(obj_file),
            str(temp_dir / f"{func_name}.bin")
        ]

        result = subprocess.run(objcopy_cmd, capture_output=True)
        if result.returncode != 0:
            return None

        bin_file = temp_dir / f"{func_name}.bin"
        if bin_file.exists():
            with open(bin_file, 'rb') as f:
                return f.read()

        return None
    except Exception:
        return None


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
                if len(parts) >= 3:
                    addr_str = parts[0].split(':')[0].strip()
                    try:
                        addr = int(addr_str, 16)
                    except ValueError:
                        continue
                    hex_bytes = parts[1].strip()
                    # Join parts 2+ as instruction (opcode + operands)
                    instr = '\t'.join(parts[2:]).strip()
                    instructions.append((addr, hex_bytes, instr))

        return instructions
    finally:
        os.unlink(tmp_path)


def analyze_instructions(instructions: List[Tuple[int, str, str]]) -> Dict:
    """Analyze instruction list for patterns."""
    analysis = {
        'instruction_count': len(instructions),
        'stack_frame_size': 0,
        'ra_offset': None,
        'registers_used': set(),
        'branches': 0,
        'jumps': 0,
        'loads': 0,
        'stores': 0,
        'calls': 0,
    }

    for addr, hex_bytes, instr in instructions:
        # Extract registers
        reg_pattern = r'\$([a-z][a-z0-9]*|[0-9]+)'
        regs = re.findall(reg_pattern, instr)
        analysis['registers_used'].update(regs)

        # Detect stack frame allocation
        if 'addiu' in instr and 'sp,sp,-' in instr:
            match = re.search(r'-(\d+)', instr)
            if match:
                analysis['stack_frame_size'] = int(match.group(1))

        # Detect ra save
        if 'sw' in instr and 'ra,' in instr:
            match = re.search(r',(\d+)\(sp\)', instr)
            if match:
                analysis['ra_offset'] = int(match.group(1))

        # Count instruction types
        if any(b in instr for b in ['beq', 'bne', 'bgez', 'bgtz', 'blez', 'bltz']):
            analysis['branches'] += 1
        if 'j' in instr and 'jal' not in instr and 'jr' not in instr:
            analysis['jumps'] += 1
        if 'jal' in instr:
            analysis['calls'] += 1
        if any(l in instr for l in ['lw', 'lh', 'lb', 'lhu', 'lbu', 'lwl', 'lwr']):
            analysis['loads'] += 1
        if any(s in instr for s in ['sw', 'sh', 'sb', 'swl', 'swr']):
            analysis['stores'] += 1

    return analysis


def print_analysis(label: str, analysis: Dict, color_code: str = Colors.WHITE):
    """Print instruction analysis."""
    print(color(f"\n{label} Analysis:", color_code + Colors.BOLD))
    print(f"  Instructions:   {analysis['instruction_count']}")
    print(f"  Stack frame:    {analysis['stack_frame_size']} bytes")
    print(f"  RA offset:      {analysis['ra_offset']}")
    print(f"  Branches:       {analysis['branches']}")
    print(f"  Calls (jal):    {analysis['calls']}")
    print(f"  Loads:          {analysis['loads']}")
    print(f"  Stores:         {analysis['stores']}")
    print(f"  Registers:      {', '.join(sorted(analysis['registers_used']))}")


def diff_instructions(orig: List[Tuple[int, str, str]],
                      comp: List[Tuple[int, str, str]],
                      side_by_side: bool = True,
                      context: int = 0) -> Tuple[int, int]:
    """
    Diff two instruction lists and print differences.

    Returns (matching_count, total_count).
    """
    max_len = max(len(orig), len(comp))
    matching = 0
    differences = []

    for i in range(max_len):
        orig_instr = orig[i][2] if i < len(orig) else None
        comp_instr = comp[i][2] if i < len(comp) else None

        if orig_instr == comp_instr:
            matching += 1
            differences.append((i, orig_instr, comp_instr, True))
        else:
            differences.append((i, orig_instr, comp_instr, False))

    # Print diff
    print(color("\nInstruction Comparison:", Colors.BOLD))
    print("-" * 80)

    if side_by_side:
        print(f"{'Idx':>4}  {'Original':<35} {'Compiled':<35}")
        print("-" * 80)

        # Show context around differences
        in_context = False
        last_shown = -context - 1

        for i, (idx, orig_instr, comp_instr, is_match) in enumerate(differences):
            # Determine if we should show this line
            show = not is_match

            # Check if within context of a difference
            if context > 0:
                # Check if there's a difference within context range
                for j in range(max(0, i - context), min(len(differences), i + context + 1)):
                    if not differences[j][3]:  # Not a match
                        show = True
                        break

            if show:
                if i > last_shown + 1:
                    print("  ...")

                orig_str = orig_instr if orig_instr else "(missing)"
                comp_str = comp_instr if comp_instr else "(missing)"

                if is_match:
                    line_color = Colors.WHITE
                    marker = "  "
                else:
                    line_color = Colors.RED
                    marker = "! "

                print(color(f"{marker}{idx:>3}  {orig_str:<35} {comp_str:<35}", line_color))
                last_shown = i

        if last_shown < len(differences) - 1:
            print("  ...")

    else:
        # Unified diff style
        for idx, orig_instr, comp_instr, is_match in differences:
            if not is_match:
                if orig_instr:
                    print(color(f"-{idx:>3}  {orig_instr}", Colors.RED))
                if comp_instr:
                    print(color(f"+{idx:>3}  {comp_instr}", Colors.GREEN))

    print("-" * 80)

    return matching, max_len


def quick_diff(func_name: str, side_by_side: bool = True,
               context: int = 0, no_color: bool = False) -> bool:
    """
    Perform quick diff on a function.

    Returns True if perfect match.
    """
    global USE_COLORS
    if no_color:
        USE_COLORS = False

    # Load configs
    function_map = load_function_map()
    decomp_config = load_decomp_config()

    # Get function info
    func_info = get_function_info(func_name, function_map)
    if not func_info:
        print(f"Error: Function '{func_name}' not found in function map")
        return False

    # Get decompiled function info
    decomp_func = None
    for func in decomp_config.get('functions', []):
        if func['name'] == func_name:
            decomp_func = func
            break

    if not decomp_func:
        print(f"Error: Function '{func_name}' not in decompiled_functions.json")
        return False

    vram_start = int(func_info['vram_start'], 16)
    size = func_info['size']
    source_file = decomp_func['source_file']

    # Header
    print(color("=" * 80, Colors.BOLD))
    print(color(f"QUICK DIFF: {func_name}", Colors.BOLD + Colors.CYAN))
    print(color("=" * 80, Colors.BOLD))
    print()
    print(f"VRAM:        {func_info['vram_start']} - {func_info['vram_end']}")
    print(f"Size:        {size} bytes ({size // 4} instructions)")
    print(f"Source:      {source_file}")
    print(f"Complexity:  {func_info['complexity']}")

    # Extract original bytes
    original_bytes = extract_original_bytes(vram_start, size)

    # Compile
    compiled_bytes = compile_function(func_name, source_file)

    if compiled_bytes is None:
        print(color("\nCompilation FAILED!", Colors.RED + Colors.BOLD))
        return False

    # Disassemble
    orig_instrs = disassemble_bytes(original_bytes, vram_start)
    comp_instrs = disassemble_bytes(compiled_bytes, vram_start)

    # Analyze both
    orig_analysis = analyze_instructions(orig_instrs)
    comp_analysis = analyze_instructions(comp_instrs)

    # Print size comparison
    print()
    print(f"Original:    {len(original_bytes)} bytes ({len(orig_instrs)} instructions)")
    print(f"Compiled:    {len(compiled_bytes)} bytes ({len(comp_instrs)} instructions)")

    if len(original_bytes) != len(compiled_bytes):
        diff = len(compiled_bytes) - len(original_bytes)
        print(color(f"Size diff:   {diff:+d} bytes ({diff // 4:+d} instructions)",
                    Colors.YELLOW))

    # Compare stack frames
    if orig_analysis['stack_frame_size'] != comp_analysis['stack_frame_size']:
        print(color(f"\nStack frame mismatch:", Colors.YELLOW))
        print(f"  Original: {orig_analysis['stack_frame_size']} bytes")
        print(f"  Compiled: {comp_analysis['stack_frame_size']} bytes")

    # Print detailed analysis if verbose
    print_analysis("Original", orig_analysis, Colors.CYAN)
    print_analysis("Compiled", comp_analysis, Colors.MAGENTA)

    # Byte comparison
    matching_bytes = sum(1 for a, b in zip(original_bytes, compiled_bytes) if a == b)
    max_bytes = max(len(original_bytes), len(compiled_bytes))
    byte_similarity = (matching_bytes / max_bytes) * 100 if max_bytes > 0 else 0

    # Instruction diff
    matching_instrs, total_instrs = diff_instructions(
        orig_instrs, comp_instrs, side_by_side, context
    )

    instr_similarity = (matching_instrs / total_instrs) * 100 if total_instrs > 0 else 0

    # Summary
    print()
    print(color("=" * 40, Colors.BOLD))

    if original_bytes == compiled_bytes:
        print(color("  PERFECT MATCH! (Level 3)", Colors.GREEN + Colors.BOLD))
        print(color("=" * 40, Colors.BOLD))
        return True
    else:
        print(color(f"  NOT MATCHING", Colors.YELLOW + Colors.BOLD))
        print(color("=" * 40, Colors.BOLD))
        print()
        print(f"Byte similarity:        {byte_similarity:.1f}%")
        print(f"Instruction similarity: {instr_similarity:.1f}%")
        print()

        # Suggestions
        print(color("Suggestions:", Colors.CYAN))

        if orig_analysis['stack_frame_size'] != comp_analysis['stack_frame_size']:
            print("- Stack frame size differs - check padding/alignment")
            print("  Try: volatile variables, struct padding, union tricks")

        if len(orig_instrs) != len(comp_instrs):
            diff = len(comp_instrs) - len(orig_instrs)
            if diff > 0:
                print(f"- Compiled is {diff} instructions larger")
                print("  Try: simpler code, remove unnecessary operations")
            else:
                print(f"- Compiled is {-diff} instructions smaller")
                print("  Try: add padding nops or volatile operations")

        if instr_similarity > 80:
            print("- Very close match! Try decomp-permuter:")
            print(f"  python3 tools/permute_function.py {func_name}")

        return False


def main():
    parser = argparse.ArgumentParser(
        description="Fast command-line comparison for function matching",
        epilog="""
Examples:
    python3 tools/quick_diff.py func_8004B338
    python3 tools/quick_diff.py func_8004B338 --no-color
    python3 tools/quick_diff.py func_8004B338 --side-by-side
    python3 tools/quick_diff.py func_8004B338 --context 3
        """,
        formatter_class=argparse.RawDescriptionHelpFormatter
    )

    parser.add_argument(
        'function',
        help='Function name to compare (e.g., func_8004B338)'
    )
    parser.add_argument(
        '--no-color',
        action='store_true',
        help='Disable colored output'
    )
    parser.add_argument(
        '-s', '--side-by-side',
        action='store_true',
        default=True,
        help='Show side-by-side comparison (default)'
    )
    parser.add_argument(
        '-u', '--unified',
        action='store_true',
        help='Show unified diff style'
    )
    parser.add_argument(
        '-c', '--context',
        type=int,
        default=0,
        help='Number of context lines around differences (default: 0 = show all)'
    )

    args = parser.parse_args()

    side_by_side = not args.unified

    is_match = quick_diff(
        args.function,
        side_by_side=side_by_side,
        context=args.context,
        no_color=args.no_color
    )

    sys.exit(0 if is_match else 1)


if __name__ == "__main__":
    main()
