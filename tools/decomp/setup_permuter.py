#!/usr/bin/env python3
"""
Setup and run permuter for a function.

Usage:
    python3 setup_permuter.py func_XXXXXXXX [--iterations N] [--run]
"""

import argparse
import json
import os
import re
import shutil
import struct
import subprocess
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).parent.parent.parent
PERMUTER_DIR = PROJECT_ROOT / 'build' / 'decomp-permuter'
WIP_DIR = PROJECT_ROOT / 'src' / 'main' / 'wip'
IDO_CC = PROJECT_ROOT / 'tools' / 'ido-static-recomp' / 'build' / '5.2' / 'out' / 'cc'
EXE_PATH = PROJECT_ROOT / 'disks' / 'SLPS_000.65'
INCLUDE_DIR = PROJECT_ROOT / 'include'


def get_function_info(func_name):
    """Get function address and size."""
    func_map_path = PROJECT_ROOT / 'build' / 'function_analysis' / 'function_map.json'

    if func_map_path.exists():
        with open(func_map_path) as f:
            data = json.load(f)
        for func in data['functions']:
            if func['name'].lower() == func_name.lower():
                return int(func['vram_start'], 16), func['size']

    # Extract from name
    match = re.search(r'func_([0-9A-Fa-f]{8})', func_name, re.IGNORECASE)
    if match:
        return int(match.group(1), 16), None

    return None, None


def get_target_bytes(addr, size):
    """Extract target bytes from executable."""
    with open(EXE_PATH, 'rb') as f:
        exe_data = f.read()

    load_addr = struct.unpack('<I', exe_data[0x18:0x1C])[0]
    header_size = 0x800
    offset = addr - load_addr + header_size

    return exe_data[offset:offset + size]


def create_target_asm(func_name, addr, size, output_path):
    """Create target.s assembly file for permuter."""
    target_bytes = get_target_bytes(addr, size)

    with open(output_path, 'wb') as f:
        f.write(target_bytes)

    # Disassemble
    result = subprocess.run([
        'mipsel-linux-gnu-objdump', '-D', '-b', 'binary',
        '-m', 'mips:3000', '-EL',
        f'--adjust-vma=0x{addr:08X}',
        str(output_path)
    ], capture_output=True, text=True)

    # Parse and reformat to permuter format
    lines = []
    lines.append(f'.global {func_name}')
    lines.append(f'{func_name}:')

    for line in result.stdout.split('\n'):
        if ':' in line and '\t' in line:
            # Extract instruction part
            parts = line.split('\t')
            if len(parts) >= 2:
                instr = '\t'.join(parts[1:]).strip()
                if instr and not instr.startswith('.'):
                    lines.append(f'\t{instr}')

    with open(output_path, 'w') as f:
        f.write('\n'.join(lines))

    return True


def create_compile_script(func_dir):
    """Create compile.sh for permuter."""
    script = f'''#!/bin/bash
{IDO_CC} -O2 -G0 -mips1 -non_shared -c "$1" -o "$2" -I {INCLUDE_DIR}
'''
    script_path = func_dir / 'compile.sh'
    with open(script_path, 'w') as f:
        f.write(script)
    os.chmod(script_path, 0o755)


def setup_permuter(func_name, verbose=False):
    """Set up permuter directory for a function."""
    addr, size = get_function_info(func_name)
    if addr is None:
        print(f"Error: Could not find function {func_name}")
        return False

    if size is None or size == 0:
        print(f"Warning: Unknown size for {func_name}, using 256")
        size = 256

    func_dir = PERMUTER_DIR / func_name
    func_dir.mkdir(exist_ok=True)

    # Find WIP source
    wip_file = None
    for f in WIP_DIR.glob(f'{func_name}*.c'):
        wip_file = f
        break

    if wip_file is None:
        print(f"Error: No WIP file found for {func_name}")
        return False

    # Copy WIP to base.c
    shutil.copy(wip_file, func_dir / 'base.c')

    # Create target.s
    create_target_asm(func_name, addr, size, func_dir / 'target.s')

    # Create compile.sh
    create_compile_script(func_dir)

    if verbose:
        print(f"Set up {func_name}:")
        print(f"  Address: 0x{addr:08X}")
        print(f"  Size: {size} bytes")
        print(f"  WIP: {wip_file}")
        print(f"  Directory: {func_dir}")

    return True


def run_permuter(func_name, iterations=10000, verbose=False):
    """Run permuter on function."""
    func_dir = PERMUTER_DIR / func_name

    if not (func_dir / 'base.c').exists():
        if not setup_permuter(func_name, verbose):
            return False

    cmd = [
        sys.executable, str(PERMUTER_DIR / 'permuter.py'),
        str(func_dir),
        '--iterations', str(iterations)
    ]

    if verbose:
        print(f"Running: {' '.join(cmd)}")

    # Run permuter
    result = subprocess.run(cmd, cwd=PERMUTER_DIR)
    return result.returncode == 0


def main():
    parser = argparse.ArgumentParser(
        description='Setup and run permuter for a function'
    )
    parser.add_argument('func_name', help='Function name (e.g., init_ring_status)')
    parser.add_argument('--iterations', '-n', type=int, default=10000,
                        help='Number of permuter iterations')
    parser.add_argument('--run', '-r', action='store_true',
                        help='Run permuter after setup')
    parser.add_argument('--verbose', '-v', action='store_true')

    args = parser.parse_args()

    success = setup_permuter(args.func_name, args.verbose)
    if not success:
        sys.exit(1)

    print(f"Set up permuter for {args.func_name}")

    if args.run:
        print(f"Running permuter ({args.iterations} iterations)...")
        run_permuter(args.func_name, args.iterations, args.verbose)


if __name__ == '__main__':
    main()
