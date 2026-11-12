#!/usr/bin/env python3
"""
Verify that a compiled function matches the original bytes in the game executable.

This tool:
1. Compiles a C source file with IDO 5.2
2. Extracts the function bytes from the compiled object
3. Compares against the target bytes from the original executable
4. Reports match/mismatch status

Usage:
    python3 verify_function.py <source.c> --exe <original.exe> --address 0x80041000 --size 172

Or with function config:
    python3 verify_function.py <source.c> --exe <original.exe> --config config/level3_functions.json
"""

import argparse
import json
import os
import struct
import subprocess
import sys
import tempfile
from pathlib import Path


# PS1 header size
HEADER_SIZE = 2048

# IDO compiler path
IDO_CC = "tools/ido-static-recomp/build/5.2/out/cc"
IDO_FLAGS = ["-EL", "-G0", "-mips1", "-O2", "-non_shared"]


def get_dest_addr(exe_path: Path) -> int:
    """Get the destination address from PS1 executable header."""
    with open(exe_path, 'rb') as f:
        f.seek(0x18)
        return struct.unpack('<I', f.read(4))[0]


def extract_original_bytes(exe_path: Path, address: int, size: int) -> bytes:
    """Extract bytes from the original executable at the given address."""
    dest_addr = get_dest_addr(exe_path)
    offset = address - dest_addr + HEADER_SIZE

    with open(exe_path, 'rb') as f:
        f.seek(offset)
        return f.read(size)


def compile_function(source_path: Path, include_dirs: list = None) -> Path:
    """Compile a C source file with IDO 5.2 and return the object file path."""

    if not Path(IDO_CC).exists():
        print(f"ERROR: IDO 5.2 compiler not found at {IDO_CC}")
        print("Run: make setup-ido52")
        sys.exit(1)

    # Create temp object file
    obj_fd, obj_path = tempfile.mkstemp(suffix='.o')
    os.close(obj_fd)

    cmd = [IDO_CC] + IDO_FLAGS + ["-c", str(source_path), "-o", obj_path]

    # Add include directories
    if include_dirs:
        for inc in include_dirs:
            cmd.extend(["-I", inc])
    cmd.extend(["-I", "include"])

    result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode != 0:
        print(f"Compilation failed:")
        print(result.stderr)
        os.unlink(obj_path)
        return None

    return Path(obj_path)


def extract_text_section(obj_path: Path) -> bytes:
    """Extract the .text section from an ELF object file."""

    # Use objcopy to extract .text section
    text_fd, text_path = tempfile.mkstemp(suffix='.bin')
    os.close(text_fd)

    cmd = ["mipsel-linux-gnu-objcopy", "-O", "binary", "-j", ".text",
           str(obj_path), text_path]

    result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode != 0:
        # Try with mipsel-linux-gnu-objcopy
        cmd[0] = "mipsel-linux-gnu-objcopy"
        result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode != 0:
        print(f"objcopy failed: {result.stderr}")
        os.unlink(text_path)
        return None

    with open(text_path, 'rb') as f:
        data = f.read()

    os.unlink(text_path)
    return data


def compare_bytes(original: bytes, compiled: bytes, address: int, verbose: bool = False) -> bool:
    """Compare original and compiled bytes, report differences."""

    if len(compiled) == 0:
        print("ERROR: Compiled function is empty")
        return False

    # Trim compiled to match size (it may have padding)
    compiled = compiled[:len(original)]

    if original == compiled:
        return True

    if verbose:
        print(f"\nByte comparison at 0x{address:08X}:")
        print(f"  Original size: {len(original)} bytes")
        print(f"  Compiled size: {len(compiled)} bytes")

        # Find first difference
        for i, (o, c) in enumerate(zip(original, compiled)):
            if o != c:
                print(f"\n  First difference at offset +{i} (0x{address + i:08X}):")
                print(f"    Original: 0x{o:02X}")
                print(f"    Compiled: 0x{c:02X}")

                # Show surrounding context
                start = max(0, i - 4)
                end = min(len(original), i + 8)

                print(f"\n  Context (words):")
                for j in range(start, end, 4):
                    if j + 4 <= len(original):
                        o_word = struct.unpack('<I', original[j:j+4])[0]
                        c_word = struct.unpack('<I', compiled[j:j+4])[0] if j + 4 <= len(compiled) else 0
                        marker = " <-- HERE" if i >= j and i < j + 4 else ""
                        print(f"    +{j:3d}: orig=0x{o_word:08X} comp=0x{c_word:08X}{marker}")
                break

    return False


def disassemble_bytes(data: bytes, base_addr: int = 0) -> str:
    """Disassemble bytes using mips-linux-gnu-objdump."""

    # Write bytes to temp file
    bin_fd, bin_path = tempfile.mkstemp(suffix='.bin')
    os.write(bin_fd, data)
    os.close(bin_fd)

    # Create minimal ELF for objdump
    # For now, just return hex dump
    result = []
    for i in range(0, len(data), 4):
        if i + 4 <= len(data):
            word = struct.unpack('<I', data[i:i+4])[0]
            result.append(f"0x{base_addr + i:08X}: 0x{word:08X}")

    os.unlink(bin_path)
    return '\n'.join(result)


def verify_function(source_path: Path, exe_path: Path, address: int, size: int,
                    verbose: bool = False) -> bool:
    """Verify a single function matches the original."""

    print(f"Verifying {source_path.name}...")
    print(f"  Address: 0x{address:08X}")
    print(f"  Size: {size} bytes")

    # Extract original bytes
    original = extract_original_bytes(exe_path, address, size)
    print(f"  Original bytes extracted: {len(original)} bytes")

    # Compile function
    obj_path = compile_function(source_path)
    if obj_path is None:
        return False

    # Extract compiled bytes
    compiled = extract_text_section(obj_path)
    os.unlink(obj_path)

    if compiled is None:
        return False

    print(f"  Compiled bytes: {len(compiled)} bytes")

    # Compare
    if compare_bytes(original, compiled, address, verbose):
        print("  MATCH!")
        return True
    else:
        print("  MISMATCH")
        if verbose:
            print("\n  Original disassembly:")
            print(disassemble_bytes(original, address))
            print("\n  Compiled disassembly:")
            print(disassemble_bytes(compiled, address))
        return False


def main():
    parser = argparse.ArgumentParser(
        description='Verify compiled function matches original bytes'
    )
    parser.add_argument('source', help='C source file to verify')
    parser.add_argument('--exe', required=True, help='Original PS1 executable')
    parser.add_argument('--address', type=lambda x: int(x, 0),
                        help='Function address (e.g., 0x80041000)')
    parser.add_argument('--size', type=int, help='Function size in bytes')
    parser.add_argument('--config', help='Function config JSON file')
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='Show detailed comparison')

    args = parser.parse_args()

    source_path = Path(args.source)
    exe_path = Path(args.exe)

    if not source_path.exists():
        print(f"ERROR: Source file not found: {source_path}")
        sys.exit(1)

    if not exe_path.exists():
        print(f"ERROR: Executable not found: {exe_path}")
        sys.exit(1)

    # Get function info
    if args.config:
        # Load from config
        with open(args.config) as f:
            config = json.load(f)

        # Find function by source file name
        func_name = source_path.stem
        func_info = None
        for func in config.get('functions', []):
            if func.get('name') == func_name:
                func_info = func
                break

        if not func_info:
            print(f"ERROR: Function {func_name} not found in config")
            sys.exit(1)

        address = func_info.get('address')
        size = func_info.get('size')
    else:
        if not args.address or not args.size:
            print("ERROR: Must specify --address and --size, or --config")
            sys.exit(1)
        address = args.address
        size = args.size

    # Verify
    if verify_function(source_path, exe_path, address, size, args.verbose):
        sys.exit(0)
    else:
        sys.exit(1)


if __name__ == '__main__':
    main()
