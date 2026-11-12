#!/usr/bin/env python3
"""
Frame Size Inference Tool

Automatically infers per-function frame sizes from the target binary by parsing
function prologues and comparing to IDO's default behavior.

This tool:
1. Parses each function's prologue in the target binary
2. Extracts frame size from 'addiu sp,sp,-N'
3. Identifies saved registers from 'sw $reg,offset(sp)' instructions
4. Compiles a minimal C stub with IDO to get default frame size
5. Outputs frames.json with per-function frame metadata

This automates the --old/--new detection for patch_stack_complete.py.

Usage:
    python3 tools/analysis/infer_frame_sizes.py               # All functions
    python3 tools/analysis/infer_frame_sizes.py --sample 20   # First 20 functions
    python3 tools/analysis/infer_frame_sizes.py --address 0x80041000
    python3 tools/analysis/infer_frame_sizes.py --output frames.json
"""

import argparse
import json
import os
import struct
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
CODE_END = 0x800C0000

# MIPS instruction encoding
REG_SP = 29
REG_RA = 31
OP_ADDIU = 0x09
OP_SW = 0x2B
OP_LW = 0x23

# MIPS register names
REG_NAMES = {
    0: 'zero', 1: 'at', 2: 'v0', 3: 'v1',
    4: 'a0', 5: 'a1', 6: 'a2', 7: 'a3',
    8: 't0', 9: 't1', 10: 't2', 11: 't3',
    12: 't4', 13: 't5', 14: 't6', 15: 't7',
    16: 's0', 17: 's1', 18: 's2', 19: 's3',
    20: 's4', 21: 's5', 22: 's6', 23: 's7',
    24: 't8', 25: 't9', 26: 'k0', 27: 'k1',
    28: 'gp', 29: 'sp', 30: 'fp', 31: 'ra'
}


def decode_instruction(word: int) -> Tuple[int, int, int, int]:
    """Decode MIPS I-type instruction into (opcode, rs, rt, imm)."""
    opcode = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    imm = word & 0xFFFF
    # Sign-extend immediate
    if imm & 0x8000:
        imm = imm - 0x10000
    return opcode, rs, rt, imm


def vram_to_file_offset(vram_addr: int) -> int:
    """Convert VRAM address to file offset in SLPS_000.65."""
    return (vram_addr - TEXT_SECTION_START) + FILE_OFFSET_BASE


def read_function_bytes(vram_addr: int, size: int) -> bytes:
    """Read function bytes from the target binary."""
    file_offset = vram_to_file_offset(vram_addr)

    with open(ORIGINAL_EXE, 'rb') as f:
        f.seek(file_offset)
        return f.read(size)


def parse_prologue(func_bytes: bytes, func_name: str, vram_addr: int) -> Optional[Dict]:
    """
    Parse function prologue to extract frame information.

    Returns dict with:
        - frame_size: Stack frame size (from addiu sp,sp,-N)
        - saved_regs: List of registers saved to stack
        - prologue_length: Number of instructions in prologue
        - has_prologue: Whether function has standard prologue
    """
    if len(func_bytes) < 4:
        return None

    result = {
        'frame_size': None,
        'saved_regs': [],
        'prologue_length': 0,
        'has_prologue': False,
        'analysis_notes': []
    }

    # Check first instruction for stack allocation (PS1 is LITTLE ENDIAN!)
    first_word = struct.unpack('<I', func_bytes[0:4])[0]
    opcode, rs, rt, imm = decode_instruction(first_word)

    # Standard prologue: addiu sp,sp,-N
    if opcode == OP_ADDIU and rs == REG_SP and rt == REG_SP and imm < 0:
        result['frame_size'] = -imm
        result['has_prologue'] = True
        result['prologue_length'] = 1
    else:
        # Check if this is a thunk (code before prologue)
        # Look ahead up to 8 instructions for the real prologue
        for i in range(1, min(8, len(func_bytes) // 4)):
            offset = i * 4
            word = struct.unpack('<I', func_bytes[offset:offset+4])[0]
            opcode, rs, rt, imm = decode_instruction(word)

            if opcode == OP_ADDIU and rs == REG_SP and rt == REG_SP and imm < 0:
                result['frame_size'] = -imm
                result['has_prologue'] = True
                result['prologue_length'] = i + 1
                result['analysis_notes'].append(f'Prologue found at offset {i} (thunk pattern)')
                break

    if not result['has_prologue']:
        result['analysis_notes'].append('No prologue detected (leaf function or thunk)')
        return result

    # Parse register saves (sw instructions following prologue)
    max_scan = min(20, len(func_bytes) // 4)  # Scan up to 20 instructions
    saved_regs = []

    for i in range(result['prologue_length'], max_scan):
        offset = i * 4
        if offset + 4 > len(func_bytes):
            break

        word = struct.unpack('<I', func_bytes[offset:offset+4])[0]
        opcode, rs, rt, imm = decode_instruction(word)

        # sw $reg,offset($sp)
        if opcode == OP_SW and rs == REG_SP:
            reg_name = REG_NAMES.get(rt, f'r{rt}')
            saved_regs.append({
                'register': reg_name,
                'register_num': rt,
                'offset': imm,
                'instruction_index': i
            })
        # Stop scanning when we hit non-save instruction patterns
        elif opcode not in (OP_SW, OP_ADDIU, 0x00):  # sw, addiu, nop/R-type
            # Check if it's a move (or/addu)
            r_funct = word & 0x3F
            if not (opcode == 0 and r_funct in (0x21, 0x25)):  # addu, or
                break

    result['saved_regs'] = saved_regs

    return result


def compile_stub_get_frame(func_name: str, loopunroll0: bool = False) -> Optional[int]:
    """
    Compile a minimal C stub with IDO to get default frame size.

    Returns the frame size IDO produces, or None on error.
    """
    if not IDO_COMPILER.exists():
        print(f"Warning: IDO compiler not found at {IDO_COMPILER}")
        print("Cannot determine IDO default frame size. Run: make setup-ido52")
        return None

    # Create stub that will generate a frame (needs function call to force stack allocation)
    stub_code = f"""
/* Minimal stub to test IDO frame generation */
extern int external_func(int);

int {func_name}(int a, int b, int c, int d) {{
    int local = a + b + c + d;
    return external_func(local);
}}
"""

    with tempfile.TemporaryDirectory() as tmpdir:
        stub_path = Path(tmpdir) / f"{func_name}_stub.c"
        obj_path = Path(tmpdir) / f"{func_name}_stub.o"

        # Write stub
        stub_path.write_text(stub_code)

        # Compile with IDO
        cmd = [
            str(IDO_COMPILER),
            "-O2", "-G0", "-mips1", "-EL", "-non_shared",
            "-I", str(ROOT_DIR / "include"),
            "-c", str(stub_path),
            "-o", str(obj_path)
        ]

        if loopunroll0:
            cmd.insert(2, "-Wo,-loopunroll,0")

        try:
            result = subprocess.run(cmd, capture_output=True, text=True, timeout=10)
            if result.returncode != 0:
                return None

            # Use objdump to disassemble and extract frame size
            objdump_cmd = ['mipsel-linux-gnu-objdump', '-d', str(obj_path)]
            objdump_result = subprocess.run(objdump_cmd, capture_output=True, text=True, timeout=10)

            if objdump_result.returncode != 0:
                return None

            # Parse objdump output for "addiu sp,sp,-N"
            import re
            for line in objdump_result.stdout.split('\n'):
                match = re.search(r'addiu\s+sp,sp,(-?\d+)', line)
                if match:
                    offset = int(match.group(1))
                    if offset < 0:
                        return -offset

            return None

        except Exception as e:
            print(f"Warning: Failed to compile stub for {func_name}: {e}")
            return None


def analyze_function(func: Dict, verbose: bool = False) -> Dict:
    """
    Analyze a single function to determine frame size information.

    Returns dict with:
        - function_name
        - address
        - size
        - region
        - target_frame_size
        - ido_default_frame
        - frame_delta
        - saved_registers
        - needs_patching
        - analysis_notes
    """
    func_name = func['name']
    vram_addr = int(func['address'], 16)
    size = func['size']
    region = func.get('region', 'UNKNOWN')

    result = {
        'function_name': func_name,
        'address': func['address'],
        'size': size,
        'region': region,
        'target_frame_size': None,
        'ido_default_frame': None,
        'frame_delta': None,
        'saved_registers': [],
        'needs_patching': False,
        'analysis_notes': []
    }

    if verbose:
        print(f"\nAnalyzing {func_name} @ {func['address']} ({region} region, {size} bytes)")

    # Read function bytes
    try:
        func_bytes = read_function_bytes(vram_addr, size)
    except Exception as e:
        result['analysis_notes'].append(f'Failed to read function bytes: {e}')
        return result

    # Parse prologue from target binary
    prologue_info = parse_prologue(func_bytes, func_name, vram_addr)
    if not prologue_info:
        result['analysis_notes'].append('Failed to parse prologue')
        return result

    result['target_frame_size'] = prologue_info['frame_size']
    result['saved_registers'] = [r['register'] for r in prologue_info['saved_regs']]
    result['analysis_notes'].extend(prologue_info['analysis_notes'])

    if not prologue_info['has_prologue']:
        result['analysis_notes'].append('No standard prologue (leaf function or asm-only)')
        if verbose:
            print(f"  No prologue detected")
        return result

    if verbose:
        print(f"  Target frame size: {result['target_frame_size']} bytes")
        if result['saved_registers']:
            print(f"  Saved registers: {', '.join(result['saved_registers'])}")

    # Get IDO default frame size (compile a stub)
    ido_frame = compile_stub_get_frame(func_name)
    if ido_frame is not None:
        result['ido_default_frame'] = ido_frame
        result['frame_delta'] = result['target_frame_size'] - ido_frame
        result['needs_patching'] = (result['frame_delta'] != 0)

        if verbose:
            print(f"  IDO default frame: {ido_frame} bytes")
            print(f"  Delta: {result['frame_delta']} bytes")
            print(f"  Needs patching: {result['needs_patching']}")
    else:
        result['analysis_notes'].append('Could not determine IDO default frame size')
        if verbose:
            print(f"  Could not compile stub to determine IDO frame size")

    return result


def main():
    parser = argparse.ArgumentParser(
        description='Infer per-function frame sizes from target binary',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )
    parser.add_argument('--sample', type=int, metavar='N',
                        help='Analyze only first N functions (for testing)')
    parser.add_argument('--address', metavar='0xADDR',
                        help='Analyze single function at address')
    parser.add_argument('--output', '-o', default='build/frames.json',
                        help='Output JSON file (default: build/frames.json)')
    parser.add_argument('--verbose', '-v', action='store_true',
                        help='Verbose output')
    parser.add_argument('--region', choices=['GCC', 'IDO', 'all'], default='all',
                        help='Filter by region (default: all)')

    args = parser.parse_args()

    # Load level3 functions config
    if not LEVEL3_CONFIG.exists():
        print(f"Error: Level3 config not found: {LEVEL3_CONFIG}")
        return 1

    with open(LEVEL3_CONFIG, 'r') as f:
        config = json.load(f)

    functions = config.get('functions', [])

    # Filter functions
    if args.address:
        functions = [f for f in functions if f['address'] == args.address]
        if not functions:
            print(f"Error: Function at {args.address} not found in config")
            return 1

    if args.region != 'all':
        functions = [f for f in functions if f.get('region') == args.region]

    if args.sample:
        functions = functions[:args.sample]

    print(f"Analyzing {len(functions)} functions...")

    # Analyze each function
    results = []
    stats = {
        'total': len(functions),
        'with_prologue': 0,
        'needs_patching': 0,
        'no_prologue': 0,
        'ido_compile_failed': 0
    }

    for i, func in enumerate(functions):
        if args.verbose or (i % 50 == 0 and not args.verbose):
            progress = (i + 1) / len(functions) * 100
            print(f"Progress: {i+1}/{len(functions)} ({progress:.1f}%)")

        result = analyze_function(func, verbose=args.verbose)
        results.append(result)

        # Update stats
        if result['target_frame_size'] is not None:
            stats['with_prologue'] += 1
        else:
            stats['no_prologue'] += 1

        if result['needs_patching']:
            stats['needs_patching'] += 1

        if result['ido_default_frame'] is None and result['target_frame_size'] is not None:
            stats['ido_compile_failed'] += 1

    # Output results
    output_data = {
        'metadata': {
            'tool': 'infer_frame_sizes.py',
            'target_binary': str(ORIGINAL_EXE),
            'level3_config': str(LEVEL3_CONFIG),
            'total_functions_analyzed': len(results),
            'filter_region': args.region
        },
        'statistics': stats,
        'functions': results
    }

    # Ensure output directory exists
    output_path = Path(args.output)
    output_path.parent.mkdir(parents=True, exist_ok=True)

    with open(output_path, 'w') as f:
        json.dump(output_data, f, indent=2)

    print(f"\n{'='*70}")
    print(f"Frame Size Analysis Complete")
    print(f"{'='*70}")
    print(f"Total functions analyzed:     {stats['total']}")
    print(f"Functions with prologue:      {stats['with_prologue']}")
    print(f"Functions without prologue:   {stats['no_prologue']}")
    print(f"Functions needing patching:   {stats['needs_patching']}")
    print(f"IDO compilation failures:     {stats['ido_compile_failed']}")
    print(f"\nResults written to: {output_path}")

    # Show frame size distribution
    frame_sizes = {}
    for r in results:
        if r['target_frame_size'] is not None:
            size = r['target_frame_size']
            frame_sizes[size] = frame_sizes.get(size, 0) + 1

    if frame_sizes:
        print(f"\nFrame Size Distribution:")
        for size in sorted(frame_sizes.keys()):
            count = frame_sizes[size]
            bar = '#' * (count // 5)
            print(f"  {size:3d} bytes: {count:4d} functions {bar}")

    return 0


if __name__ == '__main__':
    sys.exit(main())
