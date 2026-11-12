#!/usr/bin/env python3
"""
Map o.bin debug symbols to SLPM-86.053 executable.

The o.bin file from SLPS-00065 contains function names from an earlier build.
This tool attempts to match them to the final SLPM-86.053 executable by:
1. Parsing symbols from o.bin
2. Searching for matching code patterns in SLPM-86.053
3. Generating a symbol file for use with Ghidra/IDA

Usage:
    python3 map_obin_symbols.py
"""

import sys
import os
import struct
import subprocess
from pathlib import Path

def parse_objdump_symbols(symbol_file):
    """Parse symbols from objdump output."""
    symbols = []

    with open(symbol_file, 'r') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('SYMBOL') or line.startswith('/'):
                continue

            # Format: [  N] e ADDR st X sc Y indx Z     NAME
            # Example: [  0] e 8004c000 st 1 sc 5 indx fffff     warm_reset_init

            parts = line.split()
            if len(parts) >= 10:
                try:
                    # Find the address (starts with 8)
                    addr_idx = None
                    for i, p in enumerate(parts):
                        if p.startswith('8') and len(p) == 8:
                            try:
                                addr = int(p, 16)
                                addr_idx = i
                                break
                            except:
                                pass

                    if addr_idx is not None:
                        name = parts[-1]
                        if not name.startswith('.') and name not in ['indx', 'fffff']:
                            symbols.append({
                                'addr': addr,
                                'name': name
                            })
                except:
                    pass

    return symbols

def read_exe_bytes(exe_path, addr, size=16):
    """Read bytes from executable at given address."""
    # SLPM_86.053 loads at 0x80010000, text starts at offset 0x800
    base_addr = 0x80010000
    text_offset = 0x800

    file_offset = (addr - base_addr) + text_offset

    if file_offset < 0:
        return None

    try:
        with open(exe_path, 'rb') as f:
            f.seek(file_offset)
            return f.read(size)
    except:
        return None

def find_function_in_exe(exe_data, obin_addr, obin_base=0x80042000):
    """
    Try to find a function from o.bin in the SLPM executable.

    The o.bin has a different base address than SLPM, so we need to
    search by code patterns rather than direct address matching.
    """
    # This is a simplified approach - a full implementation would
    # disassemble and match instruction patterns
    return None

def generate_ghidra_script(symbols, output_path):
    """Generate a Ghidra Python script to apply symbol names."""
    with open(output_path, 'w') as f:
        f.write("# Ghidra script to apply o.bin symbols to Tokimeki Memorial\n")
        f.write("# Run this in Ghidra's Script Manager\n\n")
        f.write("from ghidra.program.model.symbol import SourceType\n\n")
        f.write("def apply_symbols():\n")
        f.write("    fm = currentProgram.getFunctionManager()\n")
        f.write("    st = currentProgram.getSymbolTable()\n")
        f.write("    \n")
        f.write("    # Symbols from o.bin (addresses are from original build)\n")
        f.write("    # May need adjustment for SLPM-86.053\n")
        f.write("    symbols = [\n")

        for sym in symbols:
            f.write(f"        (0x{sym['addr']:08x}, \"{sym['name']}\"),\n")

        f.write("    ]\n")
        f.write("    \n")
        f.write("    for addr, name in symbols:\n")
        f.write("        try:\n")
        f.write("            func = fm.getFunctionAt(toAddr(addr))\n")
        f.write("            if func:\n")
        f.write("                func.setName(name, SourceType.USER_DEFINED)\n")
        f.write("                print(f'Renamed {addr:08x} to {name}')\n")
        f.write("        except:\n")
        f.write("            pass\n")
        f.write("\n")
        f.write("apply_symbols()\n")

    print(f"Generated Ghidra script: {output_path}")

def generate_symbol_list(symbols, output_path):
    """Generate a simple symbol list for reference."""
    with open(output_path, 'w') as f:
        f.write("# Tokimeki Memorial Function Symbols from o.bin\n")
        f.write("# Format: ADDRESS NAME\n")
        f.write("# Note: Addresses are from original SLPS-00065 build\n")
        f.write("#       May differ slightly in SLPM-86.053 (PS the Best)\n\n")

        for sym in sorted(symbols, key=lambda x: x['addr']):
            f.write(f"0x{sym['addr']:08X} {sym['name']}\n")

    print(f"Generated symbol list: {output_path}")

def generate_ida_script(symbols, output_path):
    """Generate an IDA Python script."""
    with open(output_path, 'w') as f:
        f.write("# IDA Python script to apply o.bin symbols\n\n")
        f.write("import idc\n\n")
        f.write("symbols = [\n")

        for sym in symbols:
            f.write(f"    (0x{sym['addr']:08x}, \"{sym['name']}\"),\n")

        f.write("]\n\n")
        f.write("for addr, name in symbols:\n")
        f.write("    idc.set_name(addr, name, idc.SN_NOWARN)\n")
        f.write("    print(f'Set {addr:08x} = {name}')\n")

    print(f"Generated IDA script: {output_path}")

def main():
    base_dir = Path(__file__).resolve().parents[2]
    symbol_file = base_dir / "build" / "o_bin_symbols.txt"
    output_dir = base_dir / "build" / "symbols"

    output_dir.mkdir(parents=True, exist_ok=True)

    print("=== o.bin Symbol Mapper ===")
    print(f"Reading symbols from: {symbol_file}")

    if not symbol_file.exists():
        print("Error: Symbol file not found. Run objdump first:")
        print("  mipsel-linux-gnu-objdump -t o.bin > build/o_bin_symbols.txt")
        sys.exit(1)

    symbols = parse_objdump_symbols(symbol_file)
    print(f"Parsed {len(symbols)} symbols")

    # Filter to functions only (addresses in typical code range)
    func_symbols = [s for s in symbols if 0x80040000 <= s['addr'] < 0x80100000]
    print(f"Function symbols: {len(func_symbols)}")

    # Generate output files
    generate_symbol_list(func_symbols, output_dir / "tokimeki_symbols.txt")
    generate_ghidra_script(func_symbols, output_dir / "apply_symbols_ghidra.py")
    generate_ida_script(func_symbols, output_dir / "apply_symbols_ida.py")

    # Show some statistics
    print("\n=== Symbol Statistics ===")

    # Count by prefix
    prefixes = {}
    for sym in func_symbols:
        name = sym['name']
        if '_' in name:
            prefix = name.split('_')[0]
        else:
            prefix = name[:4] if len(name) > 4 else name
        prefixes[prefix] = prefixes.get(prefix, 0) + 1

    print("\nTop 20 function prefixes:")
    for prefix, count in sorted(prefixes.items(), key=lambda x: -x[1])[:20]:
        print(f"  {prefix}: {count}")

    # Show some interesting function names
    print("\n=== Sample Function Names ===")
    interesting = ['main', 'game', 'girl', 'date', 'event', 'save', 'load', 'menu']
    for keyword in interesting:
        matches = [s for s in func_symbols if keyword in s['name'].lower()]
        if matches:
            print(f"\n{keyword.upper()} functions:")
            for m in matches[:5]:
                print(f"  0x{m['addr']:08X} {m['name']}")

if __name__ == "__main__":
    main()
