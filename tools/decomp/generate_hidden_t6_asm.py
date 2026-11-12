#!/usr/bin/env python3
"""
Generate assembly files for hidden-t6 pattern functions.

These functions use a t6 register value set by a thunk/trampoline
before the function prologue. This pattern cannot be expressed in C.

Usage:
    python3 tools/decomp/generate_hidden_t6_asm.py --address 0x8005A2C8 --size 52
    python3 tools/decomp/generate_hidden_t6_asm.py --batch addresses.txt
    python3 tools/decomp/generate_hidden_t6_asm.py --scan  # Auto-detect candidates
"""

import argparse
import struct
import os
import sys
from pathlib import Path

# MIPS register names
REGS = [
    'zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
    't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
    's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
    't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra'
]

def decode_mips(word, addr):
    """Decode a single MIPS instruction."""
    op = (word >> 26) & 0x3f
    rs = (word >> 21) & 0x1f
    rt = (word >> 16) & 0x1f
    rd = (word >> 11) & 0x1f
    shamt = (word >> 6) & 0x1f
    funct = word & 0x3f
    imm = word & 0xffff
    simm = imm - 0x10000 if imm >= 0x8000 else imm
    target = (addr & 0xf0000000) | ((word & 0x3ffffff) << 2)

    if word == 0:
        return 'nop', {}

    if op == 0:  # R-type
        if funct == 0x00:
            return f'sll     ${REGS[rd]}, ${REGS[rt]}, {shamt}', {'type': 'shift'}
        elif funct == 0x02:
            return f'srl     ${REGS[rd]}, ${REGS[rt]}, {shamt}', {'type': 'shift'}
        elif funct == 0x03:
            return f'sra     ${REGS[rd]}, ${REGS[rt]}, {shamt}', {'type': 'shift'}
        elif funct == 0x08:
            return f'jr      ${REGS[rs]}', {'type': 'jr', 'rs': rs}
        elif funct == 0x09:
            return f'jalr    ${REGS[rd]}, ${REGS[rs]}', {'type': 'jalr', 'rd': rd, 'rs': rs}
        elif funct == 0x21:
            if rt == 0:
                return f'move    ${REGS[rd]}, ${REGS[rs]}', {'type': 'move', 'encoding': 'addu'}
            return f'addu    ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}', {'type': 'arith'}
        elif funct == 0x23:
            return f'subu    ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}', {'type': 'arith'}
        elif funct == 0x25:
            if rt == 0:
                return f'move    ${REGS[rd]}, ${REGS[rs]}', {'type': 'move', 'encoding': 'or'}
            return f'or      ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}', {'type': 'logic'}
        elif funct == 0x26:
            return f'xor     ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}', {'type': 'logic'}
        elif funct == 0x2a:
            return f'slt     ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}', {'type': 'compare'}
        elif funct == 0x2b:
            return f'sltu    ${REGS[rd]}, ${REGS[rs]}, ${REGS[rt]}', {'type': 'compare'}
        else:
            return f'.word   0x{word:08x}', {'type': 'unknown'}
    elif op == 0x02:
        return f'j       0x{target:08x}', {'type': 'j', 'target': target}
    elif op == 0x03:
        return f'jal     func_{target:08X}', {'type': 'jal', 'target': target}
    elif op == 0x04:
        branch_target = addr + 4 + (simm << 2)
        return f'beqz    ${REGS[rs]}, .L{branch_target:08X}' if rt == 0 else f'beq     ${REGS[rs]}, ${REGS[rt]}, .L{branch_target:08X}', {'type': 'branch', 'target': branch_target}
    elif op == 0x05:
        branch_target = addr + 4 + (simm << 2)
        return f'bnez    ${REGS[rs]}, .L{branch_target:08X}' if rt == 0 else f'bne     ${REGS[rs]}, ${REGS[rt]}, .L{branch_target:08X}', {'type': 'branch', 'target': branch_target}
    elif op == 0x08:
        return f'addi    ${REGS[rt]}, ${REGS[rs]}, {simm}', {'type': 'imm_arith'}
    elif op == 0x09:
        return f'addiu   ${REGS[rt]}, ${REGS[rs]}, {simm}', {'type': 'imm_arith'}
    elif op == 0x0a:
        return f'slti    ${REGS[rt]}, ${REGS[rs]}, {simm}', {'type': 'compare'}
    elif op == 0x0b:
        return f'sltiu   ${REGS[rt]}, ${REGS[rs]}, {simm}', {'type': 'compare'}
    elif op == 0x0c:
        return f'andi    ${REGS[rt]}, ${REGS[rs]}, 0x{imm:04x}', {'type': 'logic'}
    elif op == 0x0d:
        return f'ori     ${REGS[rt]}, ${REGS[rs]}, 0x{imm:04x}', {'type': 'logic'}
    elif op == 0x0f:
        return f'lui     ${REGS[rt]}, 0x{imm:04x}', {'type': 'lui'}
    elif op == 0x10:
        return f'b       .L{addr + 4 + (simm << 2):08X}', {'type': 'branch'}
    elif op == 0x20:
        return f'lb      ${REGS[rt]}, {simm}(${REGS[rs]})', {'type': 'load'}
    elif op == 0x21:
        return f'lh      ${REGS[rt]}, {simm}(${REGS[rs]})', {'type': 'load'}
    elif op == 0x23:
        return f'lw      ${REGS[rt]}, {simm}(${REGS[rs]})', {'type': 'load'}
    elif op == 0x24:
        return f'lbu     ${REGS[rt]}, {simm}(${REGS[rs]})', {'type': 'load'}
    elif op == 0x25:
        return f'lhu     ${REGS[rt]}, {simm}(${REGS[rs]})', {'type': 'load'}
    elif op == 0x28:
        return f'sb      ${REGS[rt]}, {simm}(${REGS[rs]})', {'type': 'store'}
    elif op == 0x29:
        return f'sh      ${REGS[rt]}, {simm}(${REGS[rs]})', {'type': 'store'}
    elif op == 0x2b:
        return f'sw      ${REGS[rt]}, {simm}(${REGS[rs]})', {'type': 'store'}
    else:
        return f'.word   0x{word:08x}', {'type': 'unknown'}


def extract_function(exe_path, addr, size):
    """Extract function bytes from executable."""
    with open(exe_path, 'rb') as f:
        load_addr = 0x80041000
        offset = 0x800 + (addr - load_addr)
        f.seek(offset)
        data = f.read(size)

    instructions = []
    for i in range(0, size, 4):
        word = struct.unpack('<I', data[i:i+4])[0]
        curr_addr = addr + i
        instr, meta = decode_mips(word, curr_addr)
        instructions.append((curr_addr, word, instr, meta))

    return instructions


def detect_hidden_register(instructions):
    """Detect which hidden register the function uses."""
    # Check first few instructions after prologue
    hidden_regs = set()
    for addr, word, instr, meta in instructions[:5]:
        # Check for t6 usage before it's defined
        if '$t6' in instr and 'addiu' not in instr.split()[0]:
            if 'beqz' in instr or 'bnez' in instr or 'beq' in instr or 'bne' in instr:
                hidden_regs.add('t6')
            elif 'addiu' in instr and '$t7, $t6' in instr:
                hidden_regs.add('t6')
            elif 'sltu' in instr or 'slt' in instr:
                hidden_regs.add('t6')
        # Check for v0/v1 usage
        if '$v0' in instr or '$v1' in instr:
            if 'sltu' in instr or 'jalr' in instr:
                hidden_regs.add('v0' if '$v0' in instr else 'v1')
        # Check for at usage
        if '$at' in instr:
            if 'sb' in instr or 'sw' in instr or 'sh' in instr:
                hidden_regs.add('at')

    return hidden_regs


def find_branch_targets(instructions):
    """Find all branch targets within the function."""
    targets = set()
    for addr, word, instr, meta in instructions:
        if meta.get('type') == 'branch':
            targets.add(meta['target'])
    return targets


def generate_asm(addr, size, instructions, output_path, hidden_regs):
    """Generate assembly file."""
    name = f'func_{addr:08X}'
    branch_targets = find_branch_targets(instructions)

    # Determine region (compiler boundary at 0x80086000)
    region = "GCC" if addr < 0x80086000 else "IDO"

    # Build header
    lines = []
    lines.append(f'/* {name} - Hidden {"/".join(hidden_regs)} parameter function')
    lines.append(f' * Address: 0x{addr:08X}')
    lines.append(f' * Size: {size} bytes ({size // 4} instructions)')
    lines.append(f' * Region: {region}')
    lines.append(f' *')
    lines.append(f' * NOTE: Uses {"/".join(hidden_regs)} register(s) from PRECEDING code (thunk)')
    lines.append(f' */')
    lines.append('')
    lines.append('.set noreorder')
    lines.append('.set noat')
    lines.append('')
    lines.append(f'.globl {name}')
    lines.append(f'{name}:')

    # Add instructions
    for i, (iaddr, word, instr, meta) in enumerate(instructions):
        # Add label if this is a branch target
        if iaddr in branch_targets and iaddr != addr:
            lines.append(f'.L{iaddr:08X}:')

        # Format instruction with comment for hidden reg usage
        comment = ''
        if i < 3 and any(f'${r}' in instr for r in hidden_regs):
            comment = f'    /* {"/".join(hidden_regs)} from thunk */'

        lines.append(f'    {instr}{comment}')

    lines.append('')

    # Write file
    os.makedirs(os.path.dirname(output_path), exist_ok=True)
    with open(output_path, 'w') as f:
        f.write('\n'.join(lines))

    return output_path


def scan_for_hidden_t6(exe_path, start=0x80041000, end=0x800C0000):
    """Scan binary for functions with hidden t6 parameter pattern."""
    candidates = []

    with open(exe_path, 'rb') as f:
        f.seek(0x800)  # Skip header

        for addr in range(start, end, 4):
            offset = 0x800 + (addr - 0x80041000)
            f.seek(offset)
            data = f.read(20)  # First 5 instructions

            if len(data) < 20:
                break

            words = [struct.unpack('<I', data[i:i+4])[0] for i in range(0, 20, 4)]

            # Check for prologue: addiu sp, sp, -X
            if (words[0] & 0xffff0000) == 0x27bd0000:
                # Check if second instruction uses t6 (beqz/bnez t6, or sltu with t6)
                second = words[1]
                op = (second >> 26) & 0x3f
                rs = (second >> 21) & 0x1f

                # beqz $t6 (op=4, rs=14) or bnez $t6 (op=5, rs=14)
                if (op == 4 or op == 5) and rs == 14:
                    # Estimate function size (look for jr $ra)
                    size = 0
                    for i in range(5, 100):
                        f.seek(offset + i * 4)
                        w = struct.unpack('<I', f.read(4))[0]
                        if w == 0x03e00008:  # jr $ra
                            size = (i + 2) * 4  # Include delay slot
                            break

                    if size > 0:
                        candidates.append((addr, size))

    return candidates


def main():
    parser = argparse.ArgumentParser(description='Generate ASM for hidden-t6 functions')
    parser.add_argument('--exe', default='disks/SLPS_000.65', help='Path to executable')
    parser.add_argument('--address', type=lambda x: int(x, 0), help='Function address')
    parser.add_argument('--size', type=int, help='Function size in bytes')
    parser.add_argument('--output', help='Output directory', default='asm/main/matched')
    parser.add_argument('--batch', help='File with address,size pairs')
    parser.add_argument('--scan', action='store_true', help='Scan for candidates')
    parser.add_argument('--dry-run', action='store_true', help='Print without writing')

    args = parser.parse_args()

    if args.scan:
        print("Scanning for hidden-t6 pattern candidates...")
        candidates = scan_for_hidden_t6(args.exe)
        print(f"Found {len(candidates)} candidates:")
        for addr, size in candidates[:50]:
            print(f"  0x{addr:08X}: {size} bytes")
        if len(candidates) > 50:
            print(f"  ... and {len(candidates) - 50} more")
        return

    if args.batch:
        with open(args.batch, 'r') as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith('#'):
                    continue
                parts = line.split()
                addr = int(parts[0], 0)
                size = int(parts[1])

                instructions = extract_function(args.exe, addr, size)
                hidden_regs = detect_hidden_register(instructions)

                if hidden_regs:
                    output = os.path.join(args.output, f'func_{addr:08X}.s')
                    if args.dry_run:
                        print(f"Would create: {output} ({size}B, hidden: {hidden_regs})")
                    else:
                        generate_asm(addr, size, instructions, output, hidden_regs)
                        print(f"Created: {output}")
        return

    if args.address and args.size:
        instructions = extract_function(args.exe, args.address, args.size)
        hidden_regs = detect_hidden_register(instructions)

        if not hidden_regs:
            hidden_regs = {'t6'}  # Default assumption

        output = os.path.join(args.output, f'func_{args.address:08X}.s')

        if args.dry_run:
            print(f"Would create: {output} ({args.size}B, hidden: {hidden_regs})")
            print("\nDisassembly:")
            for addr, word, instr, meta in instructions:
                print(f"  {addr:08x}: {instr}")
        else:
            generate_asm(args.address, args.size, instructions, output, hidden_regs)
            print(f"Created: {output}")
        return

    parser.print_help()


if __name__ == '__main__':
    main()
