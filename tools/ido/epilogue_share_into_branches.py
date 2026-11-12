#!/usr/bin/env python3
"""Share an epilogue instruction into one or more branch delay slots.

Inverse of unfill_branch_delay.py. Used when the target binary
duplicates a single instruction (typically `lw $ra, N($sp)`) into
multiple early-exit branch delay slots while IDO emits a single
copy at the function epilogue with `nop`-filled delay slots.

For each --into-offset SLOT:
  1. Replace the NOP at SLOT with the given instruction word.
  2. Increment the preceding branch's 16-bit signed offset by 1
     (target moves one word forward, since the shared insn now
     executes in the delay slot rather than at the branch target).

The branch instruction itself is at SLOT - 4. SLOT must currently
hold a NOP (0x00000000); fail otherwise to avoid silent data loss.

Usage:
    epilogue_share_into_branches.py in.o out.o \\
        --insn 0x8fbf0014 \\
        --into-offsets 0x5c,0x88,0xa0
"""
import struct
import sys
import argparse


# MIPS conditional-branch primary opcodes (op field = bits 31..26).
# REGIMM (op=0x01) covers bltz/bgez and friends; rt holds the sub-op,
# but the immediate semantics are the same.
COND_BRANCH_OPS = {0x01, 0x04, 0x05, 0x06, 0x07}


def find_text_section(data: bytearray):
    e_shoff = struct.unpack('<I', data[32:36])[0]
    e_shnum = struct.unpack('<H', data[48:50])[0]
    e_shstrndx = struct.unpack('<H', data[50:52])[0]
    sections = []
    for i in range(e_shnum):
        off = e_shoff + i * 40
        sections.append(struct.unpack('<10I', data[off:off+40]))
    str_off = sections[e_shstrndx][4]

    def get_name(idx):
        end = data.index(b'\x00', str_off + idx)
        return data[str_off + idx:end].decode()

    for sh in sections:
        if get_name(sh[0]) == '.text':
            return sh[4], sh[5]
    return None, None


def share_into_slot(data: bytearray, text_off: int, slot_off: int, insn: int,
                    keep_offset: bool = False) -> None:
    """Replace NOP at slot_off with insn; bump preceding branch's target by 1."""
    abs_slot = text_off + slot_off
    abs_branch = abs_slot - 4

    cur = struct.unpack('<I', data[abs_slot:abs_slot+4])[0]
    if cur != 0:
        raise ValueError(
            f"slot at 0x{slot_off:x} is not NOP (got 0x{cur:08x}); "
            f"refusing to overwrite"
        )

    branch_word = struct.unpack('<I', data[abs_branch:abs_branch+4])[0]
    op = (branch_word >> 26) & 0x3f
    if op not in COND_BRANCH_OPS:
        raise ValueError(
            f"insn at 0x{slot_off-4:x} (op=0x{op:02x}) is not a "
            f"conditional branch; cannot share into its delay slot"
        )

    if keep_offset:
        # Used when the target binary keeps the shared insn (typically
        # `lw $ra`) in BOTH the delay slot AND the function epilogue — the
        # branch target stays the same; we only inject the redundant copy.
        struct.pack_into('<I', data, abs_slot, insn)
        print(
            f"  Injected 0x{insn:08x} into delay slot at 0x{slot_off:04x}; "
            f"branch offset preserved (--keep-offset)"
        )
        return

    imm = branch_word & 0xffff
    simm = imm if imm < 0x8000 else imm - 0x10000
    new_simm = simm + 1
    if new_simm > 0x7fff or new_simm < -0x8000:
        raise ValueError(
            f"branch at 0x{slot_off-4:x}: incremented offset {new_simm} "
            f"out of signed 16-bit range"
        )
    new_imm = new_simm & 0xffff
    new_branch = (branch_word & 0xffff0000) | new_imm

    struct.pack_into('<I', data, abs_branch, new_branch)
    struct.pack_into('<I', data, abs_slot, insn)
    print(
        f"  Shared 0x{insn:08x} into delay slot at 0x{slot_off:04x}; "
        f"branch at 0x{slot_off-4:04x} target {simm} -> {new_simm}"
    )



def share_into_jal_slot(data: bytearray, text_off: int, slot_off: int, insn: int) -> None:
    """Like share_into_slot but for JAL+nop+slot pattern.

    The predecessor at slot_off-4 is a NOP (JAL's delay slot); the JAL is at
    slot_off-8. With keep_offset semantics we do NOT bump the JAL's 26-bit
    target (the lower 16 bits overlap with the 16-bit branch offset logic
    and corrupting them would shift the call destination).
    """
    abs_slot = text_off + slot_off
    abs_branch = abs_slot - 4
    abs_jal = abs_slot - 8

    cur = struct.unpack_from('<I', data, abs_slot)[0]
    if cur != 0:
        raise ValueError(
            f"slot at 0x{slot_off:x} is not NOP (got 0x{cur:08x}); "
            f"refusing to overwrite"
        )

    delay_word = struct.unpack_from('<I', data, abs_branch)[0]
    if delay_word != 0:
        raise ValueError(
            f"insn at 0x{slot_off-4:x} (0x{delay_word:08x}) is not the JAL "
            f"delay-slot NOP expected for the JAL+nop+slot pattern"
        )

    jal_word = struct.unpack_from('<I', data, abs_jal)[0]
    op = (jal_word >> 26) & 0x3f
    if op != 0x03:  # JAL
        raise ValueError(
            f"insn at 0x{slot_off-8:x} (op=0x{op:02x}) is not a JAL; "
            f"cannot share into JAL+nop+slot pattern"
        )

    # Inject the instruction WITHOUT touching the JAL's 26-bit target.
    struct.pack_into('<I', data, abs_slot, insn)
    print(
        f"  Injected 0x{insn:08x} into slot at 0x{slot_off:04x} after JAL at "
        f"0x{slot_off-8:04x} (keep-offset semantics; JAL target preserved)"
    )


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                  formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('input', help='Input ELF object file')
    ap.add_argument('output', help='Output ELF object file')
    ap.add_argument('--insn', required=True,
                    help='32-bit instruction word to inject (hex, e.g. 0x8fbf0014 for lw $ra,20($sp))')
    ap.add_argument('--into-offsets', required=True,
                    help='Comma-separated branch delay-slot offsets in .text (hex)')
    ap.add_argument('--keep-offset', action='store_true',
                    help='Inject the instruction WITHOUT decrementing the branch '
                         'target. Use when the target binary keeps the shared insn '
                         '(typically `lw $ra`) in BOTH the delay slot AND the '
                         'function epilogue (the +1 offset adjustment would skip '
                         'over the duplicate at the epilogue).')
    ap.add_argument('--jal-predecessor', action='store_true',
                    help='Allow the predecessor to be a JAL+nop pair. The slot '
                         'must be 2 insns after the JAL. Implies --keep-offset '
                         '(the JAL 26-bit target is never modified).')
    args = ap.parse_args()

    insn = int(args.insn, 16)
    if insn < 0 or insn > 0xffffffff:
        print(f"--insn 0x{insn:x} is not a 32-bit value", file=sys.stderr)
        sys.exit(1)

    slot_offsets = [int(s.strip(), 16) for s in args.into_offsets.split(',') if s.strip()]
    if not slot_offsets:
        print("--into-offsets requires at least one offset", file=sys.stderr)
        sys.exit(1)

    with open(args.input, 'rb') as f:
        data = bytearray(f.read())

    text_off, text_size = find_text_section(data)
    if text_off is None:
        print("No .text section found", file=sys.stderr)
        sys.exit(1)

    for slot_off in slot_offsets:
        if slot_off < 4 or slot_off + 4 > text_size:
            raise ValueError(f"slot offset 0x{slot_off:x} outside .text (size 0x{text_size:x})")
        if args.jal_predecessor:
            share_into_jal_slot(data, text_off, slot_off, insn)
        else:
            share_into_slot(data, text_off, slot_off, insn, keep_offset=args.keep_offset)

    with open(args.output, 'wb') as f:
        f.write(data)
    print(f"Wrote {args.output}")


if __name__ == '__main__':
    main()
