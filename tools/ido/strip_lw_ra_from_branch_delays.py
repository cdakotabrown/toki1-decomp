#!/usr/bin/env python3
"""Strip `lw $ra, N($sp)` instructions out of branch-delay slots and
decrement the preceding branch offset by 1.

Inverse of `epilogue_share_into_branches.py`. Used when IDO's late
scheduler hoists a shared `lw $ra` into multiple branch-delay slots
while the target binary keeps a single shared epilogue. Replacing the
`lw $ra` with `nop` and shrinking each branch's signed offset by one
word brings the compiled output back to the target's shape without
changing instruction count or function size.

For each --offset SLOT:
  1. Verify SLOT holds an `lw $ra, K($sp)` instruction.
  2. Verify SLOT-4 holds a conditional branch (REGIMM/BEQ/BNE/BLEZ/BGTZ).
  3. Replace the SLOT instruction with `nop` (0x00000000).
  4. Decrement the branch's 16-bit signed offset by 1 (target now lives
     one word *earlier* relative to the branch's PC, since the shared
     `lw $ra` no longer fires in the delay slot — control flows one
     word past the new nop and lands on what used to be the lw target).

Symmetrical to epilogue_share_into_branches. Pure ELF text rewrite —
no instruction insertions, no shifts.

Usage:
    strip_lw_ra_from_branch_delays.py in.o out.o \\
        --offsets 0x5c,0x88,0xa0

Optionally restrict to a specific stack offset for the lw:
    strip_lw_ra_from_branch_delays.py in.o out.o \\
        --offsets 0x5c,0x88 --sp-offset 0x14

Tier A3 of the 2026-05-11 blocker brainstorm (kept in the project's private archive).
"""
from __future__ import annotations

import argparse
import shutil
import struct
import sys
from pathlib import Path
from typing import List, Optional

sys.path.insert(0, str(Path(__file__).parent))
from rename_register_chain import extract_text_section  # noqa: E402

OP_LW = 0x23
REG_RA = 31
REG_SP = 29

# Primary opcodes for conditional branches. REGIMM (op=0x01) covers
# bltz/bgez/bltzal/bgezal; their immediates have the same semantics.
COND_BRANCH_OPS = {0x01, 0x04, 0x05, 0x06, 0x07}

# Unconditional absolute jumps: j (0x02), jal (0x03). Their delay slots can
# also hold an IDO-hoisted lw $ra; --allow-uncond enables this case. No
# offset adjustment is needed (the targets are absolute).
UNCOND_JUMP_OPS = {0x02, 0x03}

# SPECIAL opcode (0x00) function-field for jr and jalr.
FUNC_JR = 0x08
FUNC_JALR = 0x09


def is_uncond_jump_or_jalr(word: int) -> bool:
    op = (word >> 26) & 0x3F
    if op in UNCOND_JUMP_OPS:
        return True
    if op == 0x00:
        func = word & 0x3F
        return func in (FUNC_JR, FUNC_JALR)
    return False


def is_lw_ra_sp(word: int, sp_offset: Optional[int]) -> Optional[int]:
    """Return the K offset if word == `lw $ra, K($sp)`, else None.

    If sp_offset is given, require the K to match exactly.
    """
    op = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    imm = word & 0xFFFF
    if op != OP_LW or rs != REG_SP or rt != REG_RA:
        return None
    k = imm - 0x10000 if imm >= 0x8000 else imm
    if sp_offset is not None and k != sp_offset:
        return None
    return k


def is_cond_branch(word: int) -> bool:
    op = (word >> 26) & 0x3F
    return op in COND_BRANCH_OPS


def decrement_branch_offset(word: int) -> int:
    """Subtract 1 from the 16-bit signed offset field of a branch."""
    imm = word & 0xFFFF
    signed = imm - 0x10000 if imm >= 0x8000 else imm
    signed -= 1
    new_imm = signed & 0xFFFF
    return (word & 0xFFFF0000) | new_imm


def apply(
    input_path: str,
    output_path: str,
    offsets: List[int],
    sp_offset: Optional[int],
    verbose: bool,
    allow_uncond: bool = False,
) -> bool:
    shutil.copy(input_path, output_path)
    text_offset, text_size, endian_fmt = extract_text_section(output_path)

    NOP = 0x00000000
    ok = True
    with open(output_path, "r+b") as f:
        for off in offsets:
            if off < 4 or off + 4 > text_size or off % 4:
                print(
                    f"error: offset 0x{off:x} out of range or unaligned "
                    f"(.text size 0x{text_size:x})",
                    file=sys.stderr,
                )
                ok = False
                continue

            f.seek(text_offset + off - 4)
            pred = struct.unpack(endian_fmt + "I", f.read(4))[0]
            lw = struct.unpack(endian_fmt + "I", f.read(4))[0]

            sp_k = is_lw_ra_sp(lw, sp_offset)
            if sp_k is None:
                wanted = (
                    f"lw $ra, {sp_offset}($sp)" if sp_offset is not None else "lw $ra, K($sp)"
                )
                print(
                    f"warning: offset 0x{off:x} not {wanted} (got 0x{lw:08x}), skipping",
                    file=sys.stderr,
                )
                continue

            cond = is_cond_branch(pred)
            uncond = is_uncond_jump_or_jalr(pred)

            if cond:
                new_pred = decrement_branch_offset(pred)
                pred_kind = "cond-branch"
            elif uncond and allow_uncond:
                # No offset to adjust (absolute jump / register-indirect).
                # Just NOP the lw $ra in the delay slot.
                new_pred = pred
                pred_kind = "uncond-jump"
            else:
                hint = (
                    "; pass --allow-uncond to accept jal/jr/jalr/j predecessors"
                    if uncond and not allow_uncond
                    else ""
                )
                print(
                    f"warning: offset 0x{off-4:x} preceding 0x{off:x} not a "
                    f"conditional branch (got 0x{pred:08x}), skipping{hint}",
                    file=sys.stderr,
                )
                continue

            if verbose:
                if cond:
                    old_off = pred & 0xFFFF
                    old_signed = old_off - 0x10000 if old_off >= 0x8000 else old_off
                    new_off = new_pred & 0xFFFF
                    new_signed = new_off - 0x10000 if new_off >= 0x8000 else new_off
                    print(
                        f"  0x{off-4:04x}: 0x{pred:08x} -> 0x{new_pred:08x}  "
                        f"({pred_kind} offset {old_signed:+d} -> {new_signed:+d})"
                    )
                else:
                    print(f"  0x{off-4:04x}: 0x{pred:08x} (unchanged, {pred_kind})")
                print(
                    f"  0x{off:04x}: 0x{lw:08x} -> 0x{NOP:08x}  "
                    f"(lw $ra, {sp_k}($sp) -> nop)"
                )

            f.seek(text_offset + off - 4)
            f.write(struct.pack(endian_fmt + "I", new_pred))
            f.write(struct.pack(endian_fmt + "I", NOP))

    return ok


def parse_offsets(spec: str) -> List[int]:
    out: List[int] = []
    for tok in spec.split(","):
        tok = tok.strip()
        if not tok:
            continue
        out.append(int(tok, 0))
    return out


def main() -> int:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("input", help="input .o file")
    p.add_argument("output", help="output .o file")
    p.add_argument(
        "--offsets",
        required=True,
        help="comma-separated .text-relative offsets of the `lw $ra` "
             "instructions to remove (e.g., 0x5c,0x88,0xa0)",
    )
    p.add_argument(
        "--sp-offset",
        type=lambda s: int(s, 0),
        default=None,
        help="if set, require lw $ra's stack offset to match exactly "
             "(e.g., --sp-offset 0x14 for `lw $ra, 0x14($sp)`)",
    )
    p.add_argument(
        "--allow-uncond",
        action="store_true",
        help="also accept jal/jalr/jr/j in the predecessor slot (no branch "
             "offset adjustment is needed; the delay-slot lw_ra is simply "
             "NOP'd out)",
    )
    p.add_argument("-v", "--verbose", action="store_true")
    args = p.parse_args()
    offsets = parse_offsets(args.offsets)
    if not offsets:
        print("error: --offsets is empty", file=sys.stderr)
        return 1
    return 0 if apply(args.input, args.output, offsets, args.sp_offset, args.verbose,
                      allow_uncond=args.allow_uncond) else 1


if __name__ == "__main__":
    raise SystemExit(main())
