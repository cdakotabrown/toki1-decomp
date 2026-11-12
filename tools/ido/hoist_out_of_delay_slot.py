#!/usr/bin/env python3
"""Hoist a delay-slot instruction out, swapping it with its branch.

This is the inverse of `unfill_branch_delay` in spirit, but structurally
DIFFERENT: instead of replacing the delay slot with NOP and decrementing
the branch target, this tool SWAPS the branch and its delay-slot
instruction in place.

Use case: IDO's scheduler hoisted an instruction (typically `sw $ra` /
`lw`/`sll` etc.) into the delay slot AFTER the branch, while the target
binary kept that instruction BEFORE the branch (or in a different order).
For example:

    IDO:
        sltiu   $at, $a0, 16          # offset N-4
        bne     $at, $zero, X         # offset N    (branch, target X)
        sw      $ra, 20($sp)          # offset N+4  (HOISTED into delay)
        sll     $t6, $a0, 2           # offset N+8

    Target:
        sltiu   $at, $a0, 16          # offset N-4
        sw      $ra, 20($sp)          # offset N    (the hoisted insn)
        bne     $at, $zero, X-1       # offset N+4  (branch slid down by 1)
        sll     $t6, $a0, 2           # offset N+8

Algorithm:
  1. Read insn at `--offset N` (must be a recognized branch / jal).
  2. Read insn at N+4 (the delay-slot insn). Sanity-check that it is NOT
     itself a branch / jump (those are illegal in delay slots anyway).
  3. Write delay-slot insn at offset N (it now executes BEFORE the branch).
  4. Write branch at offset N+4 (now one word later).
  5. For PC-relative branches (op 0x01/0x04/0x05/0x06/0x07) the signed
     16-bit offset is decremented by 1 to compensate for the branch
     having moved 1 word later (target absolute address unchanged).
  6. For absolute jumps (j 0x02 / jal 0x03) the encoded target is the
     absolute word address, so no adjustment is needed.

POLICY: This is a structural swap. No bytes inserted/deleted; no
hardcoded byte patches. The only field rewritten on the branch is its
signed 16-bit immediate (PC-relative branches only).
"""
import struct
import sys
import argparse


# Branch op decoding
COND_BRANCH_OPS = {0x01, 0x04, 0x05, 0x06, 0x07}
ABS_JUMP_OPS = {0x02, 0x03}


def _is_branch_or_jump(insn: int) -> bool:
    """Return True if `insn` is any branch / jump (for delay-slot sanity)."""
    op = (insn >> 26) & 0x3F
    if op in COND_BRANCH_OPS or op in ABS_JUMP_OPS:
        return True
    # SPECIAL: jr (0x08) and jalr (0x09) are op=0 funct=0x08/0x09
    if op == 0x00:
        funct = insn & 0x3F
        if funct in (0x08, 0x09):
            return True
    # COP1 branches (op=0x11, rs=0x08 BC1F/BC1T)
    if op == 0x11:
        rs = (insn >> 21) & 0x1F
        if rs == 0x08:
            return True
    return False


def main():
    parser = argparse.ArgumentParser(
        description='Swap a branch instruction with the insn in its delay '
                    'slot (hoist the delay-slot insn out to BEFORE the branch).')
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output ELF object file')
    parser.add_argument('--offset', required=True,
                        help='Offset of branch instruction (hex), '
                             '.text-relative')
    parser.add_argument('--allow-branch-in-delay', action='store_true',
                        help='Skip sanity check that delay-slot is not '
                             'itself a branch (rarely useful)')
    args = parser.parse_args()

    branch_off = int(args.offset, 16)

    with open(args.input, 'rb') as f:
        data = bytearray(f.read())

    # Parse ELF to find .text
    e_shoff = struct.unpack('<I', data[32:36])[0]
    e_shnum = struct.unpack('<H', data[48:50])[0]
    e_shstrndx = struct.unpack('<H', data[50:52])[0]
    sections = []
    for i in range(e_shnum):
        off = e_shoff + i * 40
        sh = struct.unpack('<10I', data[off:off+40])
        sections.append(sh)
    strtab = sections[e_shstrndx]
    str_off = strtab[4]

    def get_name(idx):
        end = data.index(b'\x00', str_off + idx)
        return data[str_off + idx:end].decode()

    text_off = None
    text_size = 0
    for sh in sections:
        if get_name(sh[0]) == '.text':
            text_off = sh[4]
            text_size = sh[5]
            break

    if text_off is None:
        print("Error: no .text section found")
        sys.exit(1)

    if branch_off + 8 > text_size:
        print(f"Error: offset 0x{branch_off:x} + delay slot exceeds "
              f".text size 0x{text_size:x}")
        sys.exit(1)

    boff = text_off + branch_off
    doff = boff + 4

    branch_insn = struct.unpack('<I', data[boff:boff+4])[0]
    delay_insn = struct.unpack('<I', data[doff:doff+4])[0]

    op = (branch_insn >> 26) & 0x3F

    is_cond = op in COND_BRANCH_OPS
    is_abs = op in ABS_JUMP_OPS
    is_jr_jalr = False
    is_cop1 = False
    if op == 0x00 and (branch_insn & 0x3F) in (0x08, 0x09):
        is_jr_jalr = True
    if op == 0x11 and ((branch_insn >> 21) & 0x1F) == 0x08:
        is_cop1 = True

    if not (is_cond or is_abs or is_jr_jalr or is_cop1):
        print(f"Error: instruction at 0x{branch_off:x} (0x{branch_insn:08x}) "
              f"is not a recognized branch/jump (op=0x{op:02x})")
        sys.exit(1)

    # Sanity: delay-slot must not itself be a branch
    if not args.allow_branch_in_delay and _is_branch_or_jump(delay_insn):
        print(f"Error: delay-slot insn at 0x{branch_off+4:x} "
              f"(0x{delay_insn:08x}) is itself a branch/jump. "
              f"Use --allow-branch-in-delay to override.")
        sys.exit(1)

    # Sanity: delay-slot NOP would be a no-op swap; warn but proceed
    if delay_insn == 0x00000000:
        print(f"  Note: delay slot at 0x{branch_off+4:x} is NOP; "
              f"swap is structurally trivial.")

    # Perform the swap:
    #   offset N    <- delay_insn (the hoisted insn now lands here)
    #   offset N+4  <- branch_insn (slides 1 word later)
    new_branch = branch_insn

    if is_cond or is_cop1:
        # PC-relative: decrement signed 16-bit offset by 1
        imm = branch_insn & 0xFFFF
        simm = imm if imm < 0x8000 else imm - 0x10000
        new_simm = simm - 1
        # Range check
        if new_simm < -0x8000 or new_simm > 0x7FFF:
            print(f"Error: adjusted branch offset {new_simm} out of range")
            sys.exit(1)
        new_imm = new_simm & 0xFFFF
        new_branch = (branch_insn & 0xFFFF0000) | new_imm
        print(f"  Branch at 0x{branch_off:04x} -> 0x{branch_off+4:04x}: "
              f"PC-rel offset {simm} -> {new_simm}")
    elif is_abs:
        # j/jal: absolute target unchanged
        target = branch_insn & 0x03FFFFFF
        print(f"  Branch at 0x{branch_off:04x} -> 0x{branch_off+4:04x}: "
              f"absolute target 0x{target:07x} unchanged")
    elif is_jr_jalr:
        # Register-indirect; no immediate to adjust
        print(f"  Branch at 0x{branch_off:04x} -> 0x{branch_off+4:04x}: "
              f"register-indirect, no offset adjustment")

    struct.pack_into('<I', data, boff, delay_insn)
    struct.pack_into('<I', data, doff, new_branch)
    print(f"  Slot 0x{branch_off:04x}: 0x{branch_insn:08x} -> "
          f"0x{delay_insn:08x} (hoisted up)")
    print(f"  Slot 0x{branch_off+4:04x}: 0x{delay_insn:08x} -> "
          f"0x{new_branch:08x} (branch shifted)")

    with open(args.output, 'wb') as f:
        f.write(data)

    print("Success")


if __name__ == '__main__':
    main()
