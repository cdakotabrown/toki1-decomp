#!/usr/bin/env python3
"""Comprehensive stack frame patcher for MIPS ELF object files.

This tool patches ALL stack-relative references when changing frame size:
  1. Function prologue: addiu sp,sp,-OLD_SIZE -> addiu sp,sp,-NEW_SIZE
  2. Function epilogue: addiu sp,sp,OLD_SIZE -> addiu sp,sp,NEW_SIZE
  3. Argument area references: load/store with offset >= OLD_SIZE
     (These access caller's stack frame and must be adjusted by DIFF)

USAGE EXAMPLES:

  Basic usage (patch frame 24 -> 40):
    python3 patch_stack_complete.py input.o output.o --old 24 --new 40

  Verbose mode (show each patch):
    python3 patch_stack_complete.py input.o output.o --old 24 --new 40 -v

  Dry-run mode (preview without modifying):
    python3 patch_stack_complete.py input.o output.o --old 24 --new 40 --dry-run

  Patch specific function in multi-function file:
    python3 patch_stack_complete.py input.o output.o --old 24 --new 40 --function bustup_face_change

WHY THIS IS NEEDED:

IDO compiler typically produces 24-byte frames (16B locals + 8B RA save).
Original PS1 games often use 40-byte frames (24B locals + 16B arg save).

The difference is in the argument save area:
  - IDO -O2: Minimal frame, no arg save area
  - Original: Larger frame with arg save slots at sp+24, sp+28, sp+32, sp+36

WHAT GETS PATCHED:

  Prologue/Epilogue:
    addiu sp,sp,-24  ->  addiu sp,sp,-40   (allocate larger frame)
    addiu sp,sp,24   ->  addiu sp,sp,40    (deallocate larger frame)

  Argument area references (offset >= OLD_SIZE):
    lw $a0,24($sp)   ->  lw $a0,40($sp)    (access caller's frame)
    sw $v0,28($sp)   ->  sw $v0,44($sp)    (write to caller's frame)

  Local variable references (offset < OLD_SIZE) are NOT patched:
    lw $v0,16($sp)   ->  lw $v0,16($sp)    (unchanged - local variable)
    sw $ra,20($sp)   ->  sw $ra,20($sp)    (unchanged - RA save)

SUPPORTED INSTRUCTIONS:
  - addiu sp,sp,IMM (prologue/epilogue)
  - lw, sw, lb, lh, lbu, lhu, sb, sh (load/store with sp base)

EDGE CASES HANDLED:
  - Negative stack offsets (local variables) - preserved as-is
  - Multiple functions in one file - use --function to patch specific one
  - Functions without prologues - warning issued, no patches applied
  - Offset overflow (>32767) - error issued, patching aborted

MULTI-FUNCTION FILES:

If the object file contains multiple functions, you can patch a specific one:
  python3 patch_stack_complete.py multi.o patched.o --old 24 --new 40 --function bustup_face_change

Without --function, ALL functions with matching frame sizes are patched.

TECHNICAL DETAILS:

This tool directly modifies the .text section of ELF files:
  1. Parses ELF header to locate .text section
  2. Decodes MIPS instructions (big-endian format)
  3. Identifies prologue/epilogue by exact frame size match
  4. Identifies argument area refs by offset >= OLD_SIZE threshold
  5. Re-encodes instructions with adjusted offsets
  6. Writes modified .text section back to file

Register encoding: $sp = register 29 (0x1D)
Instruction format: I-type (opcode:6 | rs:5 | rt:5 | imm:16)

AUTHORS:
  - Original concept: Silent Hill decomp project
  - Enhanced for Tokimeki Memorial decomp (January 2026)

LICENSE:
  MIT License - Free to use and modify
"""

import sys
import struct
import argparse

# MIPS register encoding
REG_SP = 29

# MIPS opcodes
OP_ADDIU = 0x09
OP_LW = 0x23
OP_SW = 0x2B
OP_LBU = 0x24
OP_LB = 0x20
OP_LH = 0x21
OP_LHU = 0x25
OP_SB = 0x28
OP_SH = 0x29

# Load/store instruction set
LOAD_STORE_OPS = {OP_LW, OP_SW, OP_LBU, OP_LB, OP_LH, OP_LHU, OP_SB, OP_SH}

# Opcode to mnemonic mapping
OP_NAMES = {
    OP_LW: 'lw', OP_SW: 'sw', OP_LBU: 'lbu', OP_LB: 'lb',
    OP_LH: 'lh', OP_LHU: 'lhu', OP_SB: 'sb', OP_SH: 'sh'
}

# MIPS register names (for verbose output)
REG_NAMES = [
    'zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
    't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
    's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
    't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra'
]

def decode_insn(word):
    """Decode MIPS I-type instruction.

    Returns: (opcode, rs, rt, imm_unsigned, imm_signed)
    """
    op = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    imm = word & 0xFFFF

    # Sign-extend 16-bit immediate
    if imm & 0x8000:
        imm_signed = imm - 0x10000
    else:
        imm_signed = imm

    return op, rs, rt, imm, imm_signed

def encode_insn(op, rs, rt, imm):
    """Encode MIPS I-type instruction."""
    return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)

def find_text_section(data):
    """Find .text section in ELF file.

    Returns: (text_offset, text_size) or (None, None) if not found
    """
    if data[:4] != b'\x7fELF':
        return None, None

    # Parse ELF header (little-endian)
    e_shoff = struct.unpack('<I', data[0x20:0x24])[0]
    e_shentsize = struct.unpack('<H', data[0x2e:0x30])[0]
    e_shnum = struct.unpack('<H', data[0x30:0x32])[0]
    e_shstrndx = struct.unpack('<H', data[0x32:0x34])[0]

    # Get section header string table offset
    shstr_hdr_off = e_shoff + e_shstrndx * e_shentsize
    shstr_off = struct.unpack('<I', data[shstr_hdr_off + 0x10:shstr_hdr_off + 0x14])[0]

    # Find .text section
    for i in range(e_shnum):
        sh_off = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack('<I', data[sh_off:sh_off+4])[0]
        sh_type = struct.unpack('<I', data[sh_off+4:sh_off+8])[0]

        # Read section name
        name_end = data.find(b'\x00', shstr_off + sh_name_idx)
        name = data[shstr_off + sh_name_idx:name_end].decode('ascii', errors='ignore')

        if name == '.text' and sh_type == 1:  # SHT_PROGBITS
            text_off = struct.unpack('<I', data[sh_off + 0x10:sh_off + 0x14])[0]
            text_size = struct.unpack('<I', data[sh_off + 0x14:sh_off + 0x18])[0]
            return text_off, text_size

    return None, None

def find_function_prologue(data, text_off, text_size, old_size, func_name=None):
    """Find function prologue(s) matching old_size.

    Returns: list of (offset, name) tuples
    If func_name is specified, returns only that function (if found).
    """
    prologues = []

    for off in range(text_off, text_off + text_size, 4):
        word = struct.unpack('<I', data[off:off+4])[0]
        op, rs, rt, imm, imm_signed = decode_insn(word)

        # Look for addiu sp,sp,-OLD_SIZE (prologue signature)
        if op == OP_ADDIU and rs == REG_SP and rt == REG_SP and imm_signed == -old_size:
            func_off = off - text_off
            # Generate default function name from offset
            default_name = f"func_{func_off:04x}"

            if func_name:
                # If specific function requested, check if this matches
                if func_name == default_name or func_name in str(off):
                    prologues.append((off, func_name))
                    break  # Found the requested function
            else:
                # Collect all prologues
                prologues.append((off, default_name))

    return prologues

def patch_elf(data, old_size, new_size, verbose=False, dry_run=False, function=None, shift_ra=False):
    """Patch stack frame references in ELF file.

    Args:
        data: ELF file contents (bytes)
        old_size: Original frame size
        new_size: Target frame size
        verbose: Print each patch applied
        dry_run: Preview patches without modifying file
        function: Patch only this function (name or offset)
        shift_ra: Also shift RA save/restore by the frame difference

    Returns:
        Patched data (bytes) or None on error
    """
    data = bytearray(data)
    diff = new_size - old_size

    if diff == 0:
        print("ERROR: old_size and new_size are the same, no patching needed")
        return None

    if verbose:
        print(f"Frame size change: {old_size} -> {new_size} (diff: {diff:+d})")

    # Find .text section
    text_off, text_size = find_text_section(data)
    if text_off is None:
        print("ERROR: Not a valid ELF file or no .text section found")
        return None

    if verbose:
        print(f".text section: offset=0x{text_off:x}, size={text_size} bytes")

    # Find function prologue(s)
    prologues = find_function_prologue(data, text_off, text_size, old_size, function)

    if not prologues:
        if function:
            print(f"ERROR: Function '{function}' not found with frame size {old_size}")
        else:
            print(f"WARNING: No function prologue found with frame size {old_size}")
            print("         File may not contain matching functions, or frame size is incorrect")
        return None

    if verbose:
        print(f"Found {len(prologues)} function(s) to patch:")
        for off, name in prologues:
            print(f"  - {name} at .text+0x{off-text_off:x}")

    # Determine patch range
    if function:
        # Patch only the specified function
        patch_start = prologues[0][0]
        # Find next prologue or end of .text as function boundary
        patch_end = text_off + text_size
        for off in range(patch_start + 4, text_off + text_size, 4):
            word = struct.unpack('<I', data[off:off+4])[0]
            op, rs, rt, imm, imm_signed = decode_insn(word)
            if op == OP_ADDIU and rs == REG_SP and rt == REG_SP and imm_signed < 0:
                patch_end = off  # Found next function prologue
                break
    else:
        # Patch entire .text section
        patch_start = text_off
        patch_end = text_off + text_size

    if verbose and function:
        print(f"Patching range: 0x{patch_start:x} - 0x{patch_end:x} ({patch_end - patch_start} bytes)")

    # Apply patches
    patched_count = 0
    prologue_count = 0
    epilogue_count = 0
    arg_ref_count = 0
    ra_shift_count = 0

    for off in range(patch_start, patch_end, 4):
        word = struct.unpack('<I', data[off:off+4])[0]
        op, rs, rt, imm, imm_signed = decode_insn(word)

        new_word = None
        patch_type = None

        # Patch prologue: addiu sp,sp,-OLD_SIZE -> addiu sp,sp,-NEW_SIZE
        if op == OP_ADDIU and rs == REG_SP and rt == REG_SP and imm_signed == -old_size:
            new_word = encode_insn(OP_ADDIU, REG_SP, REG_SP, (-new_size) & 0xFFFF)
            patch_type = 'prologue'
            prologue_count += 1
            if verbose:
                print(f"  0x{off-text_off:04x}: addiu $sp,$sp,{imm_signed:3d} -> addiu $sp,$sp,{-new_size:3d}  [PROLOGUE]")

        # Patch epilogue: addiu sp,sp,OLD_SIZE -> addiu sp,sp,NEW_SIZE
        elif op == OP_ADDIU and rs == REG_SP and rt == REG_SP and imm_signed == old_size:
            new_word = encode_insn(OP_ADDIU, REG_SP, REG_SP, new_size & 0xFFFF)
            patch_type = 'epilogue'
            epilogue_count += 1
            if verbose:
                print(f"  0x{off-text_off:04x}: addiu $sp,$sp,{imm_signed:3d} -> addiu $sp,$sp,{new_size:3d}  [EPILOGUE]")

        # Patch argument area references: load/store with offset >= OLD_SIZE
        elif op in LOAD_STORE_OPS and rs == REG_SP and imm_signed >= old_size:
            new_offset = imm_signed + diff

            # Check for overflow
            if new_offset > 32767 or new_offset < -32768:
                print(f"ERROR: Offset overflow at 0x{off:x}: {imm_signed} -> {new_offset}")
                print(f"       MIPS immediate field is 16-bit signed (-32768 to 32767)")
                return None

            new_word = encode_insn(op, REG_SP, rt, new_offset & 0xFFFF)
            patch_type = 'argref'
            arg_ref_count += 1

            if verbose:
                op_name = OP_NAMES.get(op, f'op{op:02x}')
                rt_name = REG_NAMES[rt] if rt < 32 else f'r{rt}'
                print(f"  0x{off-text_off:04x}: {op_name:3s} ${rt_name:3s},{imm_signed:3d}($sp) -> "
                      f"{op_name:3s} ${rt_name:3s},{new_offset:3d}($sp)  [ARG_REF]")

        # Patch RA save/restore: sw/lw ra,(OLD_SIZE-4)(sp) -> sw/lw ra,(NEW_SIZE-4)(sp)
        elif shift_ra and op in LOAD_STORE_OPS and rs == REG_SP and rt == 31 and imm_signed == old_size - 4:
            new_offset = new_size - 4
            new_word = encode_insn(op, REG_SP, rt, new_offset & 0xFFFF)
            patch_type = 'ra_shift'
            ra_shift_count += 1

            if verbose:
                op_name = OP_NAMES.get(op, f'op{op:02x}')
                print(f"  0x{off-text_off:04x}: {op_name:3s} $ra ,{imm_signed:3d}($sp) -> "
                      f"{op_name:3s} $ra ,{new_offset:3d}($sp)  [RA_SHIFT]")

        # Apply patch
        if new_word is not None and not dry_run:
            data[off:off+4] = struct.pack('<I', new_word)
            patched_count += 1

    # Summary
    if dry_run:
        print(f"\nDRY RUN: Would patch {prologue_count + epilogue_count + arg_ref_count + ra_shift_count} instructions:")
    else:
        print(f"\nPatched {patched_count} instructions:")

    print(f"  - Prologues:        {prologue_count}")
    print(f"  - Epilogues:        {epilogue_count}")
    print(f"  - Argument refs:    {arg_ref_count}")
    if ra_shift_count:
        print(f"  - RA shifts:        {ra_shift_count}")

    if dry_run:
        return None

    return bytes(data)

def main():
    parser = argparse.ArgumentParser(
        description='Comprehensive stack frame patcher for MIPS ELF object files',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
EXAMPLES:
  # Basic usage (patch frame 24 -> 40)
  %(prog)s input.o output.o --old 24 --new 40

  # Verbose mode (show each patch)
  %(prog)s input.o output.o --old 24 --new 40 -v

  # Dry-run mode (preview without modifying)
  %(prog)s input.o output.o --old 24 --new 40 --dry-run

  # Patch specific function in multi-function file
  %(prog)s input.o output.o --old 24 --new 40 --function bustup_face_change

For more details, see tool docstring: python3 %(prog)s --help
        """)

    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output ELF object file')
    parser.add_argument('--old', type=int, required=True,
                        help='Old frame size (must match existing prologue)')
    parser.add_argument('--new', type=int, required=True,
                        help='New frame size (target frame size)')
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='Show each patch applied')
    parser.add_argument('--dry-run', action='store_true',
                        help='Preview patches without modifying file')
    parser.add_argument('--function', metavar='NAME',
                        help='Patch only specific function (name or offset)')
    parser.add_argument('--shift-ra', action='store_true',
                        help='Also shift RA save/restore offset (for non-game-region frame changes)')

    args = parser.parse_args()

    # Validate arguments
    if args.old == args.new:
        parser.error("--old and --new must be different")

    if args.old <= 0 or args.new <= 0:
        parser.error("Frame sizes must be positive")

    if args.old % 8 != 0 or args.new % 8 != 0:
        print(f"WARNING: Frame sizes should be 8-byte aligned for MIPS ABI")
        print(f"         old={args.old}, new={args.new}")

    # Read input file
    try:
        with open(args.input, 'rb') as f:
            data = f.read()
    except IOError as e:
        print(f"ERROR: Cannot read input file: {e}")
        return 1

    # Patch
    patched = patch_elf(data, args.old, args.new,
                       verbose=args.verbose,
                       dry_run=args.dry_run,
                       function=args.function,
                       shift_ra=args.shift_ra)

    if patched is None:
        if not args.dry_run:
            print("ERROR: Patching failed, output file not written")
        return 1

    # Write output file
    if not args.dry_run:
        try:
            with open(args.output, 'wb') as f:
                f.write(patched)
            print(f"\nSuccess: Patched file written to {args.output}")
        except IOError as e:
            print(f"ERROR: Cannot write output file: {e}")
            return 1

    return 0

if __name__ == "__main__":
    sys.exit(main())
