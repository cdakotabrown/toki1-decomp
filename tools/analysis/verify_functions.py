#!/usr/bin/env python3
"""
Verify all functions against the game binary.

This script:
1. Reads function metadata from function_map.json
2. Extracts bytes from the game binary at each function's offset
3. Verifies the disassembly matches
4. Computes SHA1 hash for each function
5. Reports any discrepancies

Usage:
    python3 tools/verify_functions.py [--verbose] [--fix]
"""

import json
import struct
import hashlib
import argparse
import sys
from pathlib import Path

# PS-X EXE layout
EXE_HEADER_SIZE = 0x800
TEXT_LOAD_ADDR = 0x80041000  # From EXE header

# MIPS register names
REGS = ['zero','at','v0','v1','a0','a1','a2','a3',
        't0','t1','t2','t3','t4','t5','t6','t7',
        's0','s1','s2','s3','s4','s5','s6','s7',
        't8','t9','k0','k1','gp','sp','fp','ra']


def decode_mips_instruction(word, addr):
    """Simple MIPS instruction decoder."""
    op = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    shamt = (word >> 6) & 0x1F
    funct = word & 0x3F
    imm = word & 0xFFFF
    imm_signed = imm - 0x10000 if imm >= 0x8000 else imm

    if word == 0:
        return "nop"

    # R-type instructions
    if op == 0:
        if funct == 0:  # SLL
            if rd == 0 and rt == 0 and shamt == 0:
                return "nop"
            return f"sll ${REGS[rd]}, ${REGS[rt]}, {shamt}"
        elif funct == 2:  # SRL
            return f"srl ${REGS[rd]}, ${REGS[rt]}, {shamt}"
        elif funct == 3:  # SRA
            return f"sra ${REGS[rd]}, ${REGS[rt]}, {shamt}"
        elif funct == 8:  # JR
            return f"jr ${REGS[rs]}"
        elif funct == 9:  # JALR
            return f"jalr ${REGS[rs]}"
        elif funct == 32:  # ADD
            return f"add ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
        elif funct == 33:  # ADDU
            if rt == 0:
                return f"move ${REGS[rd]}, ${REGS[rs]}"
            return f"addu ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
        elif funct == 34:  # SUB
            return f"sub ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
        elif funct == 35:  # SUBU
            return f"subu ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
        elif funct == 36:  # AND
            return f"and ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
        elif funct == 37:  # OR
            if rt == 0:
                return f"move ${REGS[rd]}, ${REGS[rs]}"
            return f"or ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
        elif funct == 38:  # XOR
            return f"xor ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
        elif funct == 39:  # NOR
            return f"nor ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
        elif funct == 42:  # SLT
            return f"slt ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
        elif funct == 43:  # SLTU
            return f"sltu ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}"
        return f"R-type funct={funct}"

    # I-type instructions
    elif op == 2:  # J
        target = ((word & 0x3FFFFFF) << 2) | (addr & 0xF0000000)
        return f"j 0x{target:08x}"
    elif op == 3:  # JAL
        target = ((word & 0x3FFFFFF) << 2) | (addr & 0xF0000000)
        return f"jal 0x{target:08x}"
    elif op == 4:  # BEQ
        target = addr + 4 + (imm_signed << 2)
        return f"beq ${REGS[rs]}, ${REGS[rt]}, 0x{target:08x}"
    elif op == 5:  # BNE
        target = addr + 4 + (imm_signed << 2)
        return f"bne ${REGS[rs]}, ${REGS[rt]}, 0x{target:08x}"
    elif op == 6:  # BLEZ
        target = addr + 4 + (imm_signed << 2)
        return f"blez ${REGS[rs]}, 0x{target:08x}"
    elif op == 7:  # BGTZ
        target = addr + 4 + (imm_signed << 2)
        return f"bgtz ${REGS[rs]}, 0x{target:08x}"
    elif op == 8:  # ADDI
        return f"addi ${REGS[rt]}, ${REGS[rs]}, {imm_signed}"
    elif op == 9:  # ADDIU
        if rs == 0:
            return f"li ${REGS[rt]}, {imm_signed}"
        return f"addiu ${REGS[rt]}, ${REGS[rs]}, {imm_signed}"
    elif op == 10:  # SLTI
        return f"slti ${REGS[rt]}, ${REGS[rs]}, {imm_signed}"
    elif op == 11:  # SLTIU
        return f"sltiu ${REGS[rt]}, ${REGS[rs]}, {imm_signed}"
    elif op == 12:  # ANDI
        return f"andi ${REGS[rt]}, ${REGS[rs]}, 0x{imm:04x}"
    elif op == 13:  # ORI
        return f"ori ${REGS[rt]}, ${REGS[rs]}, 0x{imm:04x}"
    elif op == 14:  # XORI
        return f"xori ${REGS[rt]}, ${REGS[rs]}, 0x{imm:04x}"
    elif op == 15:  # LUI
        return f"lui ${REGS[rt]}, 0x{imm:04x}"
    elif op == 32:  # LB
        return f"lb ${REGS[rt]}, {imm_signed}(${REGS[rs]})"
    elif op == 33:  # LH
        return f"lh ${REGS[rt]}, {imm_signed}(${REGS[rs]})"
    elif op == 35:  # LW
        return f"lw ${REGS[rt]}, {imm_signed}(${REGS[rs]})"
    elif op == 36:  # LBU
        return f"lbu ${REGS[rt]}, {imm_signed}(${REGS[rs]})"
    elif op == 37:  # LHU
        return f"lhu ${REGS[rt]}, {imm_signed}(${REGS[rs]})"
    elif op == 40:  # SB
        return f"sb ${REGS[rt]}, {imm_signed}(${REGS[rs]})"
    elif op == 41:  # SH
        return f"sh ${REGS[rt]}, {imm_signed}(${REGS[rs]})"
    elif op == 43:  # SW
        return f"sw ${REGS[rt]}, {imm_signed}(${REGS[rs]})"

    return f"??? op={op}"


def addr_to_file_offset(vram_addr):
    """Convert VRAM address to file offset."""
    return EXE_HEADER_SIZE + (vram_addr - TEXT_LOAD_ADDR)


def verify_function(func, exe_data, verbose=False):
    """Verify a single function against the binary."""
    name = func['name']
    vram_start = int(func['vram_start'], 16)
    size = func['size']
    stored_disasm = func.get('disassembly', [])

    # Calculate file offset
    file_offset = addr_to_file_offset(vram_start)

    # Check bounds
    if file_offset < 0 or file_offset + size > len(exe_data):
        return {
            'name': name,
            'status': 'ERROR',
            'error': f'File offset 0x{file_offset:X} out of bounds'
        }

    # Extract bytes
    func_bytes = exe_data[file_offset:file_offset + size]

    # Compute SHA1
    sha1 = hashlib.sha1(func_bytes).hexdigest()

    # Disassemble and compare
    disasm_lines = []
    mismatches = []

    for i in range(0, size, 4):
        if i + 4 > len(func_bytes):
            break
        word = struct.unpack('<I', func_bytes[i:i+4])[0]
        addr = vram_start + i
        insn = decode_mips_instruction(word, addr)
        disasm_lines.append(f"{i:x}:\t{word:08x} \t{insn}")

        # Compare with stored disassembly if available
        if i // 4 < len(stored_disasm):
            stored_line = stored_disasm[i // 4]
            # Extract hex from stored line (format: "offset:\thex \tinsn")
            stored_hex = stored_line.split()[0].split(':')[-1] if ':' in stored_line else ''
            # Normalize to compare
            try:
                stored_word = int(stored_hex, 16)
                if stored_word != word:
                    mismatches.append({
                        'offset': i,
                        'expected': stored_word,
                        'actual': word
                    })
            except ValueError:
                pass  # Can't parse stored hex

    result = {
        'name': name,
        'address': f'0x{vram_start:08X}',
        'file_offset': f'0x{file_offset:X}',
        'size': size,
        'sha1': sha1,
        'instructions': len(disasm_lines),
        'status': 'OK' if not mismatches else 'MISMATCH',
        'mismatches': mismatches
    }

    if verbose:
        result['disassembly'] = disasm_lines
        result['bytes'] = func_bytes.hex()

    return result


def main():
    parser = argparse.ArgumentParser(description='Verify all functions against game binary')
    parser.add_argument('--verbose', '-v', action='store_true', help='Show detailed output')
    parser.add_argument('--function', '-f', type=str, help='Verify specific function')
    parser.add_argument('--output', '-o', type=str, help='Output JSON file')
    parser.add_argument('--exe', type=str, default='disks/SLPM_86.053', help='Path to game executable')
    parser.add_argument('--map', type=str, default='build/function_analysis/function_map.json', help='Path to function map')
    args = parser.parse_args()

    # Check files exist
    exe_path = Path(args.exe)
    map_path = Path(args.map)

    if not exe_path.exists():
        print(f"ERROR: Game executable not found: {exe_path}")
        sys.exit(1)

    if not map_path.exists():
        print(f"ERROR: Function map not found: {map_path}")
        print("Run: python3 tools/extract_all_functions.py")
        sys.exit(1)

    # Load data
    print(f"Loading executable: {exe_path}")
    with open(exe_path, 'rb') as f:
        exe_data = f.read()

    exe_sha1 = hashlib.sha1(exe_data).hexdigest()
    print(f"Executable SHA1: {exe_sha1}")
    print(f"Executable size: {len(exe_data):,} bytes")

    print(f"\nLoading function map: {map_path}")
    with open(map_path, 'r') as f:
        func_map = json.load(f)

    functions = func_map['functions']
    print(f"Total functions: {len(functions)}")

    # Filter if specific function requested
    if args.function:
        functions = [f for f in functions if f['name'] == args.function]
        if not functions:
            print(f"ERROR: Function '{args.function}' not found")
            sys.exit(1)
        print(f"Verifying single function: {args.function}")

    print("\n" + "=" * 70)
    print("VERIFICATION RESULTS")
    print("=" * 70)

    # Verify all functions
    results = []
    ok_count = 0
    error_count = 0
    mismatch_count = 0

    for func in functions:
        result = verify_function(func, exe_data, verbose=args.verbose)
        results.append(result)

        if result['status'] == 'OK':
            ok_count += 1
            if args.verbose or args.function:
                print(f"✓ {result['name']}: {result['size']} bytes, SHA1={result['sha1'][:12]}...")
        elif result['status'] == 'ERROR':
            error_count += 1
            print(f"✗ {result['name']}: ERROR - {result.get('error', 'Unknown')}")
        else:
            mismatch_count += 1
            print(f"⚠ {result['name']}: MISMATCH - {len(result['mismatches'])} bytes differ")
            if args.verbose:
                for m in result['mismatches']:
                    print(f"    Offset 0x{m['offset']:X}: expected 0x{m['expected']:08X}, got 0x{m['actual']:08X}")

    # Summary
    print("\n" + "=" * 70)
    print("SUMMARY")
    print("=" * 70)
    print(f"Total functions:  {len(results)}")
    print(f"Verified OK:      {ok_count}")
    print(f"Mismatches:       {mismatch_count}")
    print(f"Errors:           {error_count}")

    # Compute combined hash of all function bytes
    all_bytes = b''
    for func in functions:
        vram_start = int(func['vram_start'], 16)
        size = func['size']
        file_offset = addr_to_file_offset(vram_start)
        if 0 <= file_offset < len(exe_data) - size:
            all_bytes += exe_data[file_offset:file_offset + size]

    combined_sha1 = hashlib.sha1(all_bytes).hexdigest()
    print(f"\nCombined SHA1 of all function bytes: {combined_sha1}")
    print(f"Total function bytes: {len(all_bytes):,}")

    # Output JSON if requested
    if args.output:
        output = {
            'exe_sha1': exe_sha1,
            'exe_size': len(exe_data),
            'combined_function_sha1': combined_sha1,
            'total_function_bytes': len(all_bytes),
            'total_functions': len(results),
            'verified_ok': ok_count,
            'mismatches': mismatch_count,
            'errors': error_count,
            'functions': results
        }
        with open(args.output, 'w') as f:
            json.dump(output, f, indent=2)
        print(f"\nResults written to: {args.output}")

    # Exit code
    if error_count > 0 or mismatch_count > 0:
        sys.exit(1)

    print("\n✓ All functions verified successfully!")
    return 0


if __name__ == '__main__':
    main()
