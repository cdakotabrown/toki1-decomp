#!/usr/bin/env python3
"""
auto_fix_near_match.py - Automated near-match fixer for WIP functions

This tool applies common post-processing fixes to get WIP functions to 100% match.
It intelligently tries multiple fix strategies and reports the best result.

FEATURES:
---------
- Compiles with IDO 5.2 (tries both with and without -Wo,-loopunroll,0)
- Applies frame size patching for game region (+16 bytes)
- Applies move encoding conversion for library region (or->addu)
- Tries common register swaps (v0<->v1, t6<->t7, t6<->t8, at<->v0)
- Tries delay slot filling from target binary
- Combines multiple fixes to maximize match percentage
- Reports best match achieved and what fixes were applied

USAGE:
------
  # Single function by name
  python3 auto_fix_near_match.py uwasa_init0

  # Single function with specific C file
  python3 auto_fix_near_match.py uwasa_init0 -c /tmp/myimpl.c

  # Batch mode: process all WIP files
  python3 auto_fix_near_match.py --all-wip

  # Batch mode: specific list
  python3 auto_fix_near_match.py uwasa_init0 dtd_on_tpage dtd_on

  # Verbose output
  python3 auto_fix_near_match.py uwasa_init0 -v

  # Output patched object file
  python3 auto_fix_near_match.py uwasa_init0 --output /tmp/best.o

OUTPUT:
-------
The tool reports:
  - Match percentage before any fixes
  - Each fix attempted and its result
  - Best match percentage achieved
  - Which combination of fixes produced the best result
  - Diff showing remaining differences (if not 100%)

EXAMPLES:
---------
  # Quick check on a single function
  $ python3 auto_fix_near_match.py uwasa_init0

  uwasa_init0 @ 0x80060B90 (300 bytes)
  ================================================================================
  [1/5] Compiling with IDO 5.2...
        Base match: 85.0% (68/80 instructions)

  [2/5] Trying loop unroll disabled...
        Match: 85.0% (no improvement)

  [3/5] Trying register swaps...
        v0<->v1: 87.5% (+2.5%)
        t6<->t7: 85.0% (no improvement)
        Combined (v0<->v1): 87.5%

  [4/5] Trying delay slot filling...
        Match: 92.5% (+5.0%)

  [5/5] Best result: 92.5%
        Fixes applied: frame_patch, v0<->v1, delay_slots
        Remaining differences: 6 instructions

  # Batch process all WIP
  $ python3 auto_fix_near_match.py --all-wip --summary

  SUMMARY: 45 functions processed
    100% match:  3 (uwasa_init0, dtd_on, dtd_on_tpage)
    90-99%:     12
    80-89%:     15
    <80%:       15

FIX STRATEGIES:
---------------
1. loop_unroll_0: Disable IDO's loop unrolling (-Wo,-loopunroll,0)
2. frame_patch: Adjust frame size (24->40 for game region)
3. move_encoding: Convert or->addu (library region only)
4. register_swaps: Try common register allocation swaps
   - v0<->v1: Most common difference
   - t6<->t7: IDO temporary allocation
   - t6<->t8: IDO temporary allocation variant
   - at<->v0: Assembler temporary vs v0
5. delay_slots: Fill empty delay slots from target binary
6. reschedule: Reorder instructions within basic blocks to match target
   (fixes as1 assembler scheduling differences)

SAFETY:
-------
- Register swaps are verified to not make match worse
- Multiple swaps are tried in combination
- Best result is always preserved

AUTHORS:
  Tokimeki Memorial decompilation project (February 2026)

LICENSE:
  MIT License - Free to use and modify
"""

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List, Optional, Tuple

# Project paths
ROOT_DIR = Path(__file__).parent.parent.parent
ORIGINAL_EXE = ROOT_DIR / "disks" / "SLPS_000.65"
LEVEL3_CONFIG = ROOT_DIR / "config" / "level3_functions.json"
IDO_COMPILER = ROOT_DIR / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"

# Tool paths
FRAME_PATCH_SCRIPT = ROOT_DIR / "tools" / "ido" / "patch_stack_complete.py"
MOVE_ENCODING_SCRIPT = ROOT_DIR / "tools" / "ido" / "convert_or_to_addu.py"
SWAP_REGISTERS_SCRIPT = ROOT_DIR / "tools" / "ido" / "swap_registers_auto.py"
DELAY_SLOTS_SCRIPT = ROOT_DIR / "tools" / "postproc" / "fill_delay_slots.py"
SCHEDULER_NORMALIZE_SCRIPT = ROOT_DIR / "tools" / "postproc" / "scheduler_normalizer.py"

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


@dataclass
class FixResult:
    """Result of applying a fix."""
    name: str
    match_percent: float
    match_count: int
    total_count: int
    improved: bool
    obj_path: Optional[Path] = None
    error: Optional[str] = None


@dataclass
class FunctionResult:
    """Result of processing a function."""
    name: str
    address: int
    size: int
    base_match: float
    best_match: float
    best_fixes: List[str] = field(default_factory=list)
    best_obj: Optional[Path] = None
    all_results: List[FixResult] = field(default_factory=list)
    remaining_diffs: List[Dict] = field(default_factory=list)
    error: Optional[str] = None


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


def find_source_file(func_name: str, provided_path: Optional[str] = None) -> Optional[Path]:
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

    # Check src/main/matched/
    matched_path = ROOT_DIR / "src" / "main" / "matched" / f"{func_name}.c"
    if matched_path.exists():
        return matched_path

    # Check /tmp/
    tmp_path = Path(f"/tmp/{func_name}.c")
    if tmp_path.exists():
        return tmp_path

    return None


def compile_with_ido(source_file: Path, output_obj: Path, loopunroll0: bool = False) -> bool:
    """Compile C source with IDO 5.2."""
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

    cmd.extend([
        "-I", str(ROOT_DIR / "include"),
        "-c", str(source_file),
        "-o", str(output_obj)
    ])

    result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode != 0:
        return False

    return True


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


def get_relocations(obj_file: Path) -> Dict[int, str]:
    """
    Get relocation information from an object file.

    Returns dict mapping offset to relocation type.
    """
    cmd = [
        "mipsel-linux-gnu-objdump", "-r", str(obj_file)
    ]
    result = subprocess.run(cmd, capture_output=True, text=True)

    relocs = {}
    for line in result.stdout.split('\n'):
        # Parse lines like: "00000008 R_MIPS_26         text_sys_init"
        match = re.match(r'([0-9a-f]+)\s+(R_MIPS_\w+)', line)
        if match:
            offset = int(match.group(1), 16)
            reloc_type = match.group(2)
            relocs[offset] = reloc_type

    return relocs


def is_relocation_compatible(orig_word: int, comp_word: int, reloc_type: str) -> bool:
    """
    Check if an instruction difference is due to relocation.

    For R_MIPS_26 (jal/j): The lower 26 bits are the target, which will be
    filled in by the linker. We only need to verify the opcode matches.

    For R_MIPS_HI16/R_MIPS_LO16: The immediate field will be filled in.
    We verify the opcode and registers match.
    """
    # Extract opcodes (upper 6 bits)
    orig_opcode = (orig_word >> 26) & 0x3F
    comp_opcode = (comp_word >> 26) & 0x3F

    if reloc_type == "R_MIPS_26":
        # J/JAL: opcode must match (J=2, JAL=3)
        return orig_opcode == comp_opcode and orig_opcode in (2, 3)

    elif reloc_type in ("R_MIPS_HI16", "R_MIPS_LO16"):
        # LUI, ADDIU, etc.: opcode and registers must match
        orig_rs = (orig_word >> 21) & 0x1F
        orig_rt = (orig_word >> 16) & 0x1F
        comp_rs = (comp_word >> 21) & 0x1F
        comp_rt = (comp_word >> 16) & 0x1F

        return orig_opcode == comp_opcode and orig_rs == comp_rs and orig_rt == comp_rt

    return False


def compare_bytes(original: bytes, compiled: bytes,
                  relocations: Optional[Dict[int, str]] = None) -> Tuple[int, int, List[int]]:
    """
    Compare two byte sequences instruction by instruction.

    Accounts for relocations when comparing - instructions with relocations
    are considered matching if the opcode and non-relocated fields match.

    Returns (matching_count, total_count, differing_offsets).
    """
    if relocations is None:
        relocations = {}

    # Pad to same length
    max_len = max(len(original), len(compiled))
    orig_padded = original + b'\x00' * (max_len - len(original))
    comp_padded = compiled + b'\x00' * (max_len - len(compiled))

    total = max_len // 4
    matching = 0
    diffs = []

    for i in range(total):
        offset = i * 4
        orig_word_bytes = orig_padded[offset:offset+4]
        comp_word_bytes = comp_padded[offset:offset+4]

        if orig_word_bytes == comp_word_bytes:
            matching += 1
        elif offset in relocations:
            # Check if difference is due to relocation
            import struct
            orig_word = struct.unpack('<I', orig_word_bytes)[0]
            comp_word = struct.unpack('<I', comp_word_bytes)[0]

            if is_relocation_compatible(orig_word, comp_word, relocations[offset]):
                matching += 1
            else:
                diffs.append(offset)
        else:
            diffs.append(offset)

    return matching, total, diffs


def apply_frame_patch(input_obj: Path, output_obj: Path, old_size: int = 24, new_size: int = 40) -> bool:
    """Apply frame size patching."""
    cmd = [
        "python3", str(FRAME_PATCH_SCRIPT),
        str(input_obj),
        str(output_obj),
        "--old", str(old_size),
        "--new", str(new_size)
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)
    return result.returncode == 0


def apply_move_encoding(input_obj: Path, output_obj: Path) -> bool:
    """Apply or->addu move encoding conversion."""
    cmd = [
        "python3", str(MOVE_ENCODING_SCRIPT),
        str(input_obj),
        str(output_obj)
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)
    return result.returncode == 0


def apply_register_swap(input_obj: Path, output_obj: Path, swap: str,
                       target_addr: int, unidirectional: bool = True) -> bool:
    """Apply a register swap."""
    cmd = [
        "python3", str(SWAP_REGISTERS_SCRIPT),
        str(input_obj),
        str(output_obj),
        "--swaps", swap
    ]

    if unidirectional:
        cmd.append("--unidirectional")

    result = subprocess.run(cmd, capture_output=True, text=True)
    return result.returncode == 0


def apply_delay_slots(input_obj: Path, output_obj: Path, target_addr: int) -> bool:
    """Apply delay slot filling from target."""
    cmd = [
        "python3", str(DELAY_SLOTS_SCRIPT),
        str(input_obj),
        str(output_obj),
        "--target-exe", str(ORIGINAL_EXE),
        "--address", hex(target_addr)
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)
    return result.returncode == 0


def apply_scheduler_normalize(input_obj: Path, output_obj: Path, target_addr: int) -> bool:
    """Apply scheduler normalization to reorder instructions matching target."""
    if not SCHEDULER_NORMALIZE_SCRIPT.exists():
        return False

    shutil.copy(str(input_obj), str(output_obj))

    cmd = [
        "python3", str(SCHEDULER_NORMALIZE_SCRIPT),
        str(output_obj),
        "--target", str(ORIGINAL_EXE),
        "--address", hex(target_addr)
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)
    return result.returncode == 0


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
                    instr = '\t'.join(parts[2:]).strip() if len(parts) > 2 else ""
                    instructions.append((addr, hex_bytes, instr))

        return instructions
    finally:
        os.unlink(tmp_path)


def print_diff(original_bytes: bytes, compiled_bytes: bytes, vram_addr: int,
               relocations: Optional[Dict[int, str]] = None, max_lines: int = 20) -> None:
    """Print instruction diff between original and compiled."""
    import struct as st

    if relocations is None:
        relocations = {}

    orig_instrs = disassemble_bytes(original_bytes, vram_addr)
    comp_instrs = disassemble_bytes(compiled_bytes, vram_addr)

    # Pad to same length
    max_len = max(len(orig_instrs), len(comp_instrs))

    print(f"\n{'Idx':>4}  {'Addr':>10}  {'Original':<35}  {'Compiled':<35}")
    print("=" * 90)

    shown = 0
    for i in range(max_len):
        orig_addr, orig_hex, orig_instr = orig_instrs[i] if i < len(orig_instrs) else (0, "", "")
        comp_addr, comp_hex, comp_instr = comp_instrs[i] if i < len(comp_instrs) else (0, "", "")

        # Check if they match
        offset = i * 4
        orig_word_bytes = original_bytes[offset:offset+4] if offset < len(original_bytes) else b'\x00\x00\x00\x00'
        comp_word_bytes = compiled_bytes[offset:offset+4] if offset < len(compiled_bytes) else b'\x00\x00\x00\x00'

        if orig_word_bytes == comp_word_bytes:
            continue

        # Check if this is a relocation-compatible difference
        if offset in relocations and len(orig_word_bytes) == 4 and len(comp_word_bytes) == 4:
            orig_word = st.unpack('<I', orig_word_bytes)[0]
            comp_word = st.unpack('<I', comp_word_bytes)[0]
            if is_relocation_compatible(orig_word, comp_word, relocations[offset]):
                continue  # Skip - this is a relocation, will be resolved by linker

        addr_str = f"0x{vram_addr + offset:08x}"
        orig_str = f"{orig_hex} {orig_instr}"[:35]
        comp_str = f"{comp_hex} {comp_instr}"[:35]

        # Mark relocations differently
        if offset in relocations:
            print(color(f"{i:>4}  {addr_str}  {orig_str:<35}  {comp_str:<35} [reloc mismatch]", Colors.RED))
        else:
            print(color(f"{i:>4}  {addr_str}  {orig_str:<35}  {comp_str:<35}", Colors.YELLOW))
        shown += 1

        if shown >= max_lines:
            # Count remaining real differences
            remaining = 0
            for j in range(i+1, max_len):
                joff = j * 4
                o_bytes = original_bytes[joff:joff+4] if joff < len(original_bytes) else b'\x00\x00\x00\x00'
                c_bytes = compiled_bytes[joff:joff+4] if joff < len(compiled_bytes) else b'\x00\x00\x00\x00'
                if o_bytes != c_bytes:
                    if joff in relocations and len(o_bytes) == 4 and len(c_bytes) == 4:
                        o_w = st.unpack('<I', o_bytes)[0]
                        c_w = st.unpack('<I', c_bytes)[0]
                        if is_relocation_compatible(o_w, c_w, relocations[joff]):
                            continue
                    remaining += 1
            if remaining > 0:
                print(f"... and {remaining} more differences")
            break


def process_function(func_name: str, source_path: Optional[str] = None,
                    verbose: bool = False, output_path: Optional[Path] = None,
                    quick: bool = False) -> FunctionResult:
    """
    Process a single function with all fix strategies.

    Args:
        func_name: Name of function to process
        source_path: Optional custom path to C source
        verbose: Show detailed output
        output_path: Where to save best result object file
        quick: Skip register swaps and delay slots (faster but less thorough)

    Returns FunctionResult with best match achieved.
    """
    # Get function info
    func_info = get_function_info(func_name)
    if not func_info:
        return FunctionResult(
            name=func_name,
            address=0,
            size=0,
            base_match=0.0,
            best_match=0.0,
            error=f"Function '{func_name}' not found in level3_functions.json"
        )

    vram_addr = int(func_info['address'], 16)
    size = func_info['size']

    if size == 0:
        return FunctionResult(
            name=func_name,
            address=vram_addr,
            size=0,
            base_match=0.0,
            best_match=0.0,
            error=f"Function size is 0 - cannot compare"
        )

    is_game_region = vram_addr < GAME_REGION_END
    region_name = "Game" if is_game_region else "Library"

    # Find source file
    source_file = find_source_file(func_name, source_path)
    if not source_file:
        return FunctionResult(
            name=func_name,
            address=vram_addr,
            size=size,
            base_match=0.0,
            best_match=0.0,
            error=f"No C source file found for {func_name}"
        )

    # Extract original bytes
    original_bytes = extract_original_bytes(vram_addr, size)

    # Create temp directory for all intermediate files
    temp_dir = Path(tempfile.mkdtemp(prefix="autofix_"))

    result = FunctionResult(
        name=func_name,
        address=vram_addr,
        size=size,
        base_match=0.0,
        best_match=0.0
    )

    try:
        # =====================================================================
        # Step 1: Base compilation with IDO 5.2
        # =====================================================================
        if verbose:
            print(color(f"\n[1/5] Compiling with IDO 5.2...", Colors.CYAN))

        base_obj = temp_dir / f"{func_name}_base.o"
        if not compile_with_ido(source_file, base_obj):
            result.error = "IDO compilation failed"
            return result

        # Apply region-specific post-processing
        processed_obj = temp_dir / f"{func_name}_pp.o"

        if is_game_region:
            # Detect and patch frame size
            detected_frame = detect_frame_size(base_obj)
            if detected_frame:
                target_frame = detected_frame + 16
                if not apply_frame_patch(base_obj, processed_obj, detected_frame, target_frame):
                    # Try without patching
                    shutil.copy(base_obj, processed_obj)
                elif verbose:
                    print(f"      Frame patched: {detected_frame} -> {target_frame}")
            else:
                shutil.copy(base_obj, processed_obj)
        else:
            # Library region: convert move encoding
            if not apply_move_encoding(base_obj, processed_obj):
                shutil.copy(base_obj, processed_obj)
            elif verbose:
                print("      Move encoding converted (or -> addu)")

        # Get base match
        compiled_bytes = extract_text_section(processed_obj)
        if not compiled_bytes:
            result.error = "Failed to extract .text section"
            return result

        # Get relocations for proper comparison
        relocations = get_relocations(processed_obj)

        matching, total, diffs = compare_bytes(original_bytes, compiled_bytes, relocations)
        base_percent = (matching / total * 100) if total > 0 else 0
        result.base_match = base_percent
        result.best_match = base_percent
        result.best_obj = processed_obj
        result.best_fixes = ["frame_patch" if is_game_region else "move_encoding"]

        if verbose:
            print(f"      Base match: {base_percent:.1f}% ({matching}/{total} instructions)")

        if matching == total:
            if verbose:
                print(color("      PERFECT MATCH!", Colors.GREEN + Colors.BOLD))
            return result

        # Keep track of best result
        best_bytes = compiled_bytes
        best_percent = base_percent
        best_obj_path = processed_obj
        best_fixes = list(result.best_fixes)

        # =====================================================================
        # Step 2: Try with loop unroll disabled
        # =====================================================================
        if verbose:
            print(color(f"\n[2/5] Trying loop unroll disabled...", Colors.CYAN))

        unroll_obj = temp_dir / f"{func_name}_unroll0.o"
        if compile_with_ido(source_file, unroll_obj, loopunroll0=True):
            # Apply same post-processing
            unroll_pp = temp_dir / f"{func_name}_unroll0_pp.o"

            if is_game_region:
                detected_frame = detect_frame_size(unroll_obj)
                if detected_frame:
                    apply_frame_patch(unroll_obj, unroll_pp, detected_frame, detected_frame + 16)
                else:
                    shutil.copy(unroll_obj, unroll_pp)
            else:
                apply_move_encoding(unroll_obj, unroll_pp)

            unroll_bytes = extract_text_section(unroll_pp)
            if unroll_bytes:
                unroll_relocs = get_relocations(unroll_pp)
                unroll_matching, unroll_total, _ = compare_bytes(original_bytes, unroll_bytes, unroll_relocs)
                unroll_percent = (unroll_matching / unroll_total * 100) if unroll_total > 0 else 0

                result.all_results.append(FixResult(
                    name="loopunroll0",
                    match_percent=unroll_percent,
                    match_count=unroll_matching,
                    total_count=unroll_total,
                    improved=unroll_percent > best_percent,
                    obj_path=unroll_pp
                ))

                if unroll_percent > best_percent:
                    if verbose:
                        print(color(f"      Match: {unroll_percent:.1f}% (+{unroll_percent - best_percent:.1f}%)", Colors.GREEN))
                    best_percent = unroll_percent
                    best_bytes = unroll_bytes
                    best_obj_path = unroll_pp
                    best_fixes = ["loopunroll0", "frame_patch" if is_game_region else "move_encoding"]
                elif verbose:
                    print(f"      Match: {unroll_percent:.1f}% (no improvement)")

        if best_percent == 100:
            result.best_match = best_percent
            result.best_fixes = best_fixes
            result.best_obj = best_obj_path
            return result

        # =====================================================================
        # Step 3: Try register swaps (skip if quick mode)
        # =====================================================================
        if quick:
            if verbose:
                print(color(f"\n[3/5] Skipping register swaps (quick mode)", Colors.CYAN))
        else:
            if verbose:
                print(color(f"\n[3/5] Trying register swaps...", Colors.CYAN))

            # Common register swaps to try - comprehensive set covering IDO patterns
            register_swaps = [
                # Return/temp register swaps
                ("v0:v1", "v0<->v1"),
                # IDO temporary register swaps
                ("t6:t7", "t6<->t7"),
                ("t6:t8", "t6<->t8"),
                ("t7:t8", "t7<->t8"),
                # Cross-prologue: IDO allocates globals to different regs
                ("a0:v0", "a0<->v0"),
                ("a0:v1", "a0<->v1"),
                ("a1:v0", "a1<->v0"),
                ("a1:v1", "a1<->v1"),
                ("at:v0", "at<->v0"),
                ("at:v1", "at<->v1"),
                # Parameter to temporary (non-standard calling convention)
                ("a0:t6", "a0<->t6"),
                ("a0:t7", "a0<->t7"),
                ("a1:t6", "a1<->t6"),
                ("a1:t7", "a1<->t7"),
                ("a1:t8", "a1<->t8"),
                ("a2:t6", "a2<->t6"),
                ("a2:t7", "a2<->t7"),
                # Return value to temporary
                ("v0:t6", "v0<->t6"),
                ("v0:t7", "v0<->t7"),
                ("v1:t6", "v1<->t6"),
                ("v1:t7", "v1<->t7"),
                # Callee-save register swaps
                ("s0:s1", "s0<->s1"),
                ("s0:s2", "s0<->s2"),
                ("s1:s2", "s1<->s2"),
            ]

            swap_results = []
            current_best_swap_obj = best_obj_path
            current_best_swap_percent = best_percent
            current_best_swap_bytes = best_bytes
            successful_swaps = []

            for swap_str, swap_name in register_swaps:
                swap_obj = temp_dir / f"{func_name}_swap_{swap_name.replace('<->', '_')}.o"

                if apply_register_swap(current_best_swap_obj, swap_obj, swap_str, vram_addr):
                    swap_bytes = extract_text_section(swap_obj)
                    if swap_bytes:
                        swap_relocs = get_relocations(swap_obj)
                        swap_matching, swap_total, _ = compare_bytes(original_bytes, swap_bytes, swap_relocs)
                        swap_percent = (swap_matching / swap_total * 100) if swap_total > 0 else 0

                        swap_result = FixResult(
                            name=f"swap_{swap_name}",
                            match_percent=swap_percent,
                            match_count=swap_matching,
                            total_count=swap_total,
                            improved=swap_percent > current_best_swap_percent,
                            obj_path=swap_obj
                        )
                        swap_results.append(swap_result)
                        result.all_results.append(swap_result)

                        if swap_percent > current_best_swap_percent:
                            if verbose:
                                print(color(f"      {swap_name}: {swap_percent:.1f}% (+{swap_percent - current_best_swap_percent:.1f}%)", Colors.GREEN))
                            current_best_swap_percent = swap_percent
                            current_best_swap_bytes = swap_bytes
                            current_best_swap_obj = swap_obj
                            successful_swaps.append(swap_name)
                        elif verbose:
                            print(f"      {swap_name}: {swap_percent:.1f}% (no improvement)")

            # Update best if swaps improved
            if current_best_swap_percent > best_percent:
                best_percent = current_best_swap_percent
                best_bytes = current_best_swap_bytes
                best_obj_path = current_best_swap_obj
                best_fixes = best_fixes + successful_swaps

        if best_percent == 100:
            result.best_match = best_percent
            result.best_fixes = best_fixes
            result.best_obj = best_obj_path
            return result

        # =====================================================================
        # Step 4: Try delay slot filling (skip if quick mode)
        # =====================================================================
        if quick:
            if verbose:
                print(color(f"\n[4/5] Skipping delay slot filling (quick mode)", Colors.CYAN))
        else:
            if verbose:
                print(color(f"\n[4/5] Trying delay slot filling...", Colors.CYAN))

            delay_obj = temp_dir / f"{func_name}_delay.o"
            if apply_delay_slots(best_obj_path, delay_obj, vram_addr):
                delay_bytes = extract_text_section(delay_obj)
                if delay_bytes:
                    delay_relocs = get_relocations(delay_obj)
                    delay_matching, delay_total, _ = compare_bytes(original_bytes, delay_bytes, delay_relocs)
                    delay_percent = (delay_matching / delay_total * 100) if delay_total > 0 else 0

                    result.all_results.append(FixResult(
                        name="delay_slots",
                        match_percent=delay_percent,
                        match_count=delay_matching,
                        total_count=delay_total,
                        improved=delay_percent > best_percent,
                        obj_path=delay_obj
                    ))

                    if delay_percent > best_percent:
                        if verbose:
                            print(color(f"      Match: {delay_percent:.1f}% (+{delay_percent - best_percent:.1f}%)", Colors.GREEN))
                        best_percent = delay_percent
                        best_bytes = delay_bytes
                        best_obj_path = delay_obj
                        best_fixes.append("delay_slots")
                    elif verbose:
                        print(f"      Match: {delay_percent:.1f}% (no improvement)")

        # =====================================================================
        # Step 4.5: Try scheduler normalization (skip if quick mode or perfect)
        # =====================================================================
        if quick or best_percent >= 100:
            if verbose and not quick and best_percent < 100:
                print(color(f"\n[4.5/5] Skipping scheduler normalization (quick mode)", Colors.CYAN))
        else:
            if verbose:
                print(color(f"\n[4.5/5] Trying scheduler normalization...", Colors.CYAN))

            norm_obj = temp_dir / f"{func_name}_norm.o"
            if apply_scheduler_normalize(best_obj_path, norm_obj, vram_addr):
                norm_bytes = extract_text_section(norm_obj)
                if norm_bytes:
                    norm_relocs = get_relocations(norm_obj)
                    norm_matching, norm_total, _ = compare_bytes(original_bytes, norm_bytes, norm_relocs)
                    norm_percent = (norm_matching / norm_total * 100) if norm_total > 0 else 0

                    result.all_results.append(FixResult(
                        name="scheduler_normalize",
                        match_percent=norm_percent,
                        match_count=norm_matching,
                        total_count=norm_total,
                        improved=norm_percent > best_percent,
                        obj_path=norm_obj
                    ))

                    if norm_percent > best_percent:
                        if verbose:
                            print(color(f"      Match: {norm_percent:.1f}% (+{norm_percent - best_percent:.1f}%)", Colors.GREEN))
                        best_percent = norm_percent
                        best_bytes = norm_bytes
                        best_obj_path = norm_obj
                        best_fixes.append("scheduler_normalize")
                    elif verbose:
                        print(f"      Match: {norm_percent:.1f}% (no improvement)")

        # =====================================================================
        # Step 5: Report best result
        # =====================================================================
        result.best_match = best_percent
        result.best_fixes = best_fixes
        result.best_obj = best_obj_path

        if verbose:
            print(color(f"\n[5/5] Best result: {best_percent:.1f}%", Colors.BOLD))
            if best_fixes:
                print(f"      Fixes applied: {', '.join(best_fixes)}")

            best_relocs = get_relocations(best_obj_path) if best_obj_path else {}
            _, total, diffs = compare_bytes(original_bytes, best_bytes, best_relocs)
            if diffs:
                print(f"      Remaining differences: {len(diffs)} instructions")
                print_diff(original_bytes, best_bytes, vram_addr, best_relocs)

        # Copy best result to output if requested
        if output_path and best_obj_path:
            shutil.copy(best_obj_path, output_path)
            if verbose:
                print(f"\n      Best object saved to: {output_path}")

        return result

    finally:
        # Cleanup temp directory (keep if output was requested)
        if not output_path:
            shutil.rmtree(temp_dir, ignore_errors=True)


def get_all_wip_functions() -> List[str]:
    """Get list of all WIP function names."""
    wip_dir = ROOT_DIR / "src" / "main" / "wip"
    functions = []

    for f in wip_dir.glob("func_*.c"):
        # Skip versioned files (e.g., func_80044F8C_v2.c)
        name = f.stem
        if '_v' in name or '_body' in name or '_param' in name or '_reread' in name:
            continue
        functions.append(name)

    return sorted(functions)


def main():
    parser = argparse.ArgumentParser(
        description="Automated near-match fixer for WIP functions",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
    # Single function
    python3 auto_fix_near_match.py uwasa_init0

    # With custom source file
    python3 auto_fix_near_match.py uwasa_init0 -c /tmp/myimpl.c

    # Batch mode
    python3 auto_fix_near_match.py --all-wip

    # Multiple functions
    python3 auto_fix_near_match.py uwasa_init0 dtd_on

    # Save best result
    python3 auto_fix_near_match.py uwasa_init0 --output /tmp/best.o
        """
    )

    parser.add_argument(
        'functions',
        nargs='*',
        help='Function names to process'
    )
    parser.add_argument(
        '-c', '--source',
        help='Path to C source file (for single function mode)'
    )
    parser.add_argument(
        '--all-wip',
        action='store_true',
        help='Process all WIP functions'
    )
    parser.add_argument(
        '--output', '-o',
        type=Path,
        help='Output path for best object file (single function mode)'
    )
    parser.add_argument(
        '-v', '--verbose',
        action='store_true',
        help='Verbose output'
    )
    parser.add_argument(
        '--summary',
        action='store_true',
        help='Print summary only (for batch mode)'
    )
    parser.add_argument(
        '--min-match',
        type=float,
        default=0,
        help='Only show functions with match %% >= this value'
    )
    parser.add_argument(
        '--json',
        action='store_true',
        help='Output results in JSON format'
    )
    parser.add_argument(
        '--quick',
        action='store_true',
        help='Quick mode - skip register swaps and delay slots (only frame/move encoding)'
    )

    args = parser.parse_args()

    # Determine which functions to process
    if args.all_wip:
        functions = get_all_wip_functions()
        if not functions:
            print("No WIP functions found in src/main/wip/")
            sys.exit(1)
        print(f"Found {len(functions)} WIP functions to process")
    elif args.functions:
        functions = args.functions
    else:
        parser.print_help()
        sys.exit(1)

    # Process functions
    results = []

    for i, func_name in enumerate(functions):
        if len(functions) > 1:
            print(color(f"\n{'='*80}", Colors.BOLD))
            print(color(f"[{i+1}/{len(functions)}] {func_name}", Colors.BOLD + Colors.CYAN))
            print(color(f"{'='*80}", Colors.BOLD))
        else:
            # Get info for header
            func_info = get_function_info(func_name)
            if func_info:
                addr = int(func_info['address'], 16)
                size = func_info['size']
                print(color(f"\n{func_name} @ 0x{addr:08x} ({size} bytes)", Colors.BOLD + Colors.CYAN))
                print(color("=" * 80, Colors.BOLD))

        result = process_function(
            func_name,
            source_path=args.source if len(functions) == 1 else None,
            verbose=args.verbose or len(functions) == 1,
            output_path=args.output if len(functions) == 1 else None,
            quick=args.quick
        )

        results.append(result)

        if result.error:
            print(color(f"  Error: {result.error}", Colors.RED))
        elif len(functions) > 1 and not args.summary:
            status = "MATCH!" if result.best_match == 100 else f"{result.best_match:.1f}%"
            if result.best_match == 100:
                print(color(f"  Result: {status}", Colors.GREEN + Colors.BOLD))
            elif result.best_match >= 90:
                print(color(f"  Result: {status}", Colors.GREEN))
            elif result.best_match >= 80:
                print(color(f"  Result: {status}", Colors.YELLOW))
            else:
                print(f"  Result: {status}")

    # Print summary for batch mode
    if len(functions) > 1:
        print(color(f"\n{'='*80}", Colors.BOLD))
        print(color("SUMMARY", Colors.BOLD + Colors.CYAN))
        print(color(f"{'='*80}", Colors.BOLD))

        perfect = [r for r in results if r.best_match == 100]
        high = [r for r in results if 90 <= r.best_match < 100]
        medium = [r for r in results if 80 <= r.best_match < 90]
        low = [r for r in results if r.best_match < 80 and not r.error]
        errors = [r for r in results if r.error]

        print(f"\nProcessed: {len(functions)} functions")
        print(f"  100% match:    {len(perfect):3d}")
        print(f"  90-99%:        {len(high):3d}")
        print(f"  80-89%:        {len(medium):3d}")
        print(f"  <80%:          {len(low):3d}")
        print(f"  Errors:        {len(errors):3d}")

        if perfect:
            print(color(f"\nPerfect matches:", Colors.GREEN + Colors.BOLD))
            for r in perfect:
                print(f"  {r.name}")

        if args.min_match > 0:
            filtered = [r for r in results if r.best_match >= args.min_match and not r.error]
            print(f"\nFunctions with >= {args.min_match}% match:")
            for r in sorted(filtered, key=lambda x: -x.best_match):
                fixes = ', '.join(r.best_fixes) if r.best_fixes else 'none'
                print(f"  {r.name}: {r.best_match:.1f}% (fixes: {fixes})")

    # JSON output if requested
    if args.json:
        import json as json_mod
        json_results = []
        for r in results:
            json_results.append({
                'name': r.name,
                'address': f"0x{r.address:08x}" if r.address else None,
                'size': r.size,
                'base_match': r.base_match,
                'best_match': r.best_match,
                'fixes_applied': r.best_fixes,
                'error': r.error
            })
        print(json_mod.dumps({'results': json_results}, indent=2))

    # Exit code: 0 if any perfect match, 1 otherwise
    perfect_count = sum(1 for r in results if r.best_match == 100)
    sys.exit(0 if perfect_count > 0 else 1)


if __name__ == "__main__":
    main()
