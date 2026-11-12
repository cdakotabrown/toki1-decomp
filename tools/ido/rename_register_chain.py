#!/usr/bin/env python3
"""
rename_register_chain.py - Simultaneous Register Chain Renaming Tool

Renames registers in a chain where multiple registers shift simultaneously.
Unlike swap_registers (which does bidirectional 1:1 swaps), this tool handles
chains like v0->t8->t9 where v0 becomes t8 and t8 becomes t9 in a single pass.

This is a legitimate post-processing step because IDO's register allocator can
assign any valid register to a variable. The chain represents a valid allocation
difference between compiled output and target.

Usage:
    # Single chain: v0 becomes t8, old t8 becomes t9
    python3 rename_register_chain.py input.o output.o --chain v0:t8:t9

    # Multiple independent renames
    python3 rename_register_chain.py input.o output.o --chain v0:t8:t9 --chain s1:s2

    # Skip rename at specific .text offsets (e.g., preserve a call-arg setup `addiu a0, X`)
    # Offsets are object-relative (after the 0x10 thunk pad, same convention as other PostProc tools).
    python3 rename_register_chain.py input.o output.o --chain a0:v0 --exclude-offsets 0x40,0x44

    # Verify against target
    python3 rename_register_chain.py input.o output.o --chain v0:t8:t9 \
        --target-exe disks/SLPS_000.65 --address 0x800956F0 --size 100

Date: February 2026
"""

import argparse
import struct
import sys
import shutil
import subprocess
import tempfile
import os
from typing import List, Tuple, Dict, Optional, Set

REGS = {
    'zero': 0, 'at': 1,
    'v0': 2, 'v1': 3,
    'a0': 4, 'a1': 5, 'a2': 6, 'a3': 7,
    't0': 8, 't1': 9, 't2': 10, 't3': 11,
    't4': 12, 't5': 13, 't6': 14, 't7': 15,
    's0': 16, 's1': 17, 's2': 18, 's3': 19,
    's4': 20, 's5': 21, 's6': 22, 's7': 23,
    't8': 24, 't9': 25,
    'k0': 26, 'k1': 27,
    'gp': 28, 'sp': 29, 'fp': 30, 'ra': 31
}

REG_NAMES = {v: k for k, v in REGS.items()}


def parse_chain(chain_str: str) -> Dict[int, int]:
    """
    Parse a chain specification like 'v0:t8:t9' into a rename mapping.

    Chain 'A:B:C' means: A->B, B->C (simultaneously)
    Chain 'A:B' means: A->B (unidirectional rename)

    Returns dict mapping old_reg -> new_reg
    """
    parts = [p.strip() for p in chain_str.split(':')]
    if len(parts) < 2:
        raise ValueError(f"Chain must have at least 2 registers: '{chain_str}'")

    mapping = {}
    for i in range(len(parts) - 1):
        old_name = parts[i]
        new_name = parts[i + 1]

        old_reg = REGS.get(old_name)
        new_reg = REGS.get(new_name)

        if old_reg is None:
            raise ValueError(f"Unknown register '{old_name}'")
        if new_reg is None:
            raise ValueError(f"Unknown register '{new_name}'")

        if old_reg in mapping:
            raise ValueError(f"Register ${old_name} appears multiple times as source in chain")

        mapping[old_reg] = new_reg

    return mapping


def apply_chain_rename(instr: int, mapping: Dict[int, int]) -> Tuple[int, bool]:
    """
    Apply a chain rename mapping to a single MIPS instruction.

    All renames in the mapping are applied simultaneously (snapshot semantics):
    first read all old values, then write all new values.

    Returns (new_instruction, changed)
    """
    opcode = (instr >> 26) & 0x3F
    rs = (instr >> 21) & 0x1F
    rt = (instr >> 16) & 0x1F
    rd = (instr >> 11) & 0x1F

    new_rs = mapping.get(rs, rs)
    new_rt = mapping.get(rt, rt)
    new_rd = rd
    if opcode == 0:
        new_rd = mapping.get(rd, rd)

    changed = (new_rs != rs) or (new_rt != rt) or (opcode == 0 and new_rd != rd)

    if not changed:
        return instr, False

    new_instr = instr
    new_instr = (new_instr & ~(0x1F << 21)) | (new_rs << 21)
    new_instr = (new_instr & ~(0x1F << 16)) | (new_rt << 16)
    if opcode == 0:
        new_instr = (new_instr & ~(0x1F << 11)) | (new_rd << 11)

    return new_instr, True


def extract_text_section(elf_path: str):
    """Extract .text section offset and size from ELF file."""
    with open(elf_path, 'rb') as f:
        magic = f.read(4)
        if magic != b'\x7fELF':
            raise ValueError(f"{elf_path} is not an ELF file")

        f.seek(5)
        ei_data = f.read(1)[0]
        big_endian = (ei_data == 2)
        endian_fmt = '>' if big_endian else '<'

        f.seek(32)
        e_shoff = struct.unpack(endian_fmt + 'I', f.read(4))[0]
        f.seek(46)
        e_shentsize = struct.unpack(endian_fmt + 'H', f.read(2))[0]
        e_shnum = struct.unpack(endian_fmt + 'H', f.read(2))[0]
        e_shstrndx = struct.unpack(endian_fmt + 'H', f.read(2))[0]

        f.seek(e_shoff + e_shstrndx * e_shentsize + 16)
        shstrtab_offset = struct.unpack(endian_fmt + 'I', f.read(4))[0]

        for i in range(e_shnum):
            f.seek(e_shoff + i * e_shentsize)
            sh_name_idx = struct.unpack(endian_fmt + 'I', f.read(4))[0]

            pos = f.tell()
            f.seek(shstrtab_offset + sh_name_idx)
            name = b''
            while True:
                c = f.read(1)
                if c == b'\x00' or not c:
                    break
                name += c
            f.seek(pos)

            if name == b'.text':
                f.seek(e_shoff + i * e_shentsize + 16)
                text_offset = struct.unpack(endian_fmt + 'I', f.read(4))[0]
                text_size = struct.unpack(endian_fmt + 'I', f.read(4))[0]
                return text_offset, text_size, endian_fmt

    raise ValueError(".text section not found")


def apply_chain_to_elf(input_path: str, output_path: str,
                       mapping: Dict[int, int],
                       verbose: bool = False,
                       exclude_offsets: Optional[Set[int]] = None,
                       byte_range: Optional[Tuple[int, int]] = None,
                       occurrences: Optional[Dict[int, Set[int]]] = None) -> int:
    """
    Apply chain rename to instructions in .text section of an ELF file.

    Args:
        exclude_offsets: Set of .text offsets (object-relative) to skip.
        byte_range: Optional (start, end) byte range (.text-relative,
            half-open) outside of which no rename is applied. Combine with
            `occurrences` to bound a per-occurrence rename to a single
            live range. None = whole .text.
        occurrences: Optional dict mapping a *source* register (in `mapping`)
            to a set of 1-indexed occurrence numbers to rename. An
            "occurrence" of register X is an instruction that *would* be
            modified by the rename for X (i.e. has X in rs/rt/rd where the
            rename would apply). Occurrences are counted WITHIN
            `byte_range` if one is provided, otherwise across the whole
            .text. A source register absent from this dict is renamed at
            every occurrence (subject to byte_range / exclude_offsets).

    Returns number of instructions modified.
    """
    if exclude_offsets is None:
        exclude_offsets = set()
    if occurrences is None:
        occurrences = {}

    shutil.copy(input_path, output_path)

    text_offset, text_size, endian_fmt = extract_text_section(output_path)

    if byte_range is not None:
        range_start, range_end = byte_range
        range_start = max(0, range_start)
        range_end = min(text_size, range_end)
    else:
        range_start, range_end = 0, text_size

    if verbose:
        print(f"  .text at offset {text_offset}, size {text_size}")
        chain_desc = ", ".join(
            f"${REG_NAMES[k]}->${REG_NAMES[v]}"
            for k, v in mapping.items()
        )
        print(f"  Chain: {chain_desc}")
        if byte_range is not None:
            print(f"  Byte range: 0x{range_start:x}..0x{range_end:x}")
        if exclude_offsets:
            excl_desc = ", ".join(f"0x{o:x}" for o in sorted(exclude_offsets))
            print(f"  Excluding offsets: {excl_desc}")
        if occurrences:
            for src, occ in occurrences.items():
                occ_list = ",".join(str(n) for n in sorted(occ))
                print(f"  Occurrences for ${REG_NAMES[src]}: {{{occ_list}}}")

    # Per-source occurrence counters (1-indexed at the first instance).
    occ_seen: Dict[int, int] = {src: 0 for src in occurrences}

    total_changes = 0
    skipped_excluded = 0
    skipped_range = 0
    skipped_occurrence = 0
    with open(output_path, 'r+b') as f:
        for i in range(0, text_size, 4):
            pos = text_offset + i
            f.seek(pos)
            instr_bytes = f.read(4)
            if len(instr_bytes) < 4:
                break

            instr = struct.unpack(endian_fmt + 'I', instr_bytes)[0]
            new_instr, would_change = apply_chain_rename(instr, mapping)

            if not would_change:
                continue

            if i < range_start or i >= range_end:
                if verbose:
                    print(f"    Offset 0x{i:04x}: SKIPPED (out of range) "
                          f"would have been 0x{instr:08x} -> 0x{new_instr:08x}")
                skipped_range += 1
                continue

            if i in exclude_offsets:
                if verbose:
                    print(f"    Offset 0x{i:04x}: SKIPPED (excluded) "
                          f"would have been 0x{instr:08x} -> 0x{new_instr:08x}")
                skipped_excluded += 1
                continue

            # Occurrence filter: when active, only sources that actually
            # appear in `occurrences` are gated. Other sources still rename
            # at every site. An instruction can carry multiple of the
            # chain's source regs in different fields; we count an
            # occurrence for each gated source it actually uses.
            srcs_in_instr: List[int] = []
            opcode = (instr >> 26) & 0x3F
            rs = (instr >> 21) & 0x1F
            rt = (instr >> 16) & 0x1F
            rd = (instr >> 11) & 0x1F
            for src in occurrences:
                used = (rs == src) or (rt == src) or (opcode == 0 and rd == src)
                if used:
                    srcs_in_instr.append(src)

            if srcs_in_instr:
                permitted_sources = set()
                for src in srcs_in_instr:
                    occ_seen[src] += 1
                    if occ_seen[src] in occurrences[src]:
                        permitted_sources.add(src)
                blocked_sources = set(srcs_in_instr) - permitted_sources
                if blocked_sources:
                    # Apply only the permitted subset of the chain to this insn.
                    partial_mapping = {
                        k: v for k, v in mapping.items()
                        if k not in blocked_sources
                    }
                    if not partial_mapping:
                        if verbose:
                            print(f"    Offset 0x{i:04x}: SKIPPED (occurrence not selected)")
                        skipped_occurrence += 1
                        continue
                    new_instr, would_change = apply_chain_rename(instr, partial_mapping)
                    if not would_change:
                        skipped_occurrence += 1
                        continue

            if verbose:
                print(f"    Offset 0x{i:04x}: 0x{instr:08x} -> 0x{new_instr:08x}")
            f.seek(pos)
            f.write(struct.pack(endian_fmt + 'I', new_instr))
            total_changes += 1

    if verbose:
        if skipped_excluded:
            print(f"  Skipped {skipped_excluded} instruction(s) at excluded offsets")
        if skipped_range:
            print(f"  Skipped {skipped_range} instruction(s) outside byte range")
        if skipped_occurrence:
            print(f"  Skipped {skipped_occurrence} instruction(s) by occurrence filter")

    return total_changes


def disassemble_binary(data: bytes, vram_addr: int = 0) -> List[Tuple[int, int, str]]:
    """Disassemble binary data using objdump."""
    with tempfile.NamedTemporaryFile(suffix='.bin', delete=False) as tmp:
        tmp.write(data)
        tmp_path = tmp.name

    try:
        result = subprocess.run([
            "mipsel-linux-gnu-objdump",
            "-D", "-b", "binary", "-m", "mips:3000",
            "--adjust-vma", hex(vram_addr),
            tmp_path
        ], capture_output=True, text=True)

        instructions = []
        in_disasm = False
        for line in result.stdout.split('\n'):
            if '<.data>:' in line:
                in_disasm = True
                continue
            if in_disasm and ':' in line:
                parts = line.strip().split('\t')
                if len(parts) >= 3:
                    addr_hex = parts[0].split(':')[0].strip()
                    asm_text = '\t'.join(parts[2:]).strip()
                    offset = int(addr_hex, 16) - vram_addr
                    if offset >= 0 and offset + 4 <= len(data):
                        instr_word = struct.unpack('<I', data[offset:offset+4])[0]
                        instructions.append((offset, instr_word, asm_text))
        return instructions
    finally:
        os.unlink(tmp_path)


def verify_against_target(output_path: str, exe_path: str,
                          vram_addr: int, size: int,
                          verbose: bool = False) -> float:
    """Compare the fixed .o file against the target executable."""
    TEXT_START = 0x80041000
    FILE_BASE = 0x800

    file_offset = (vram_addr - TEXT_START) + FILE_BASE
    with open(exe_path, 'rb') as f:
        f.seek(file_offset)
        target_data = f.read(size)

    text_offset, text_size, endian_fmt = extract_text_section(output_path)
    with open(output_path, 'rb') as f:
        f.seek(text_offset)
        compiled_data = f.read(text_size)

    target_instrs = disassemble_binary(target_data, vram_addr)
    compiled_instrs = disassemble_binary(compiled_data, 0)

    min_len = min(len(target_instrs), len(compiled_instrs))
    matches = 0
    diffs = []

    for i in range(min_len):
        t_off, t_word, t_asm = target_instrs[i]
        c_off, c_word, c_asm = compiled_instrs[i]

        if t_word == c_word:
            matches += 1
        else:
            t_opcode = (t_word >> 26) & 0x3F
            c_opcode = (c_word >> 26) & 0x3F
            t_rs = (t_word >> 21) & 0x1F
            c_rs = (c_word >> 21) & 0x1F
            t_rt = (t_word >> 16) & 0x1F
            c_rt = (c_word >> 16) & 0x1F

            is_reloc = (t_opcode == c_opcode and t_rs == c_rs and t_rt == c_rt and
                        t_opcode in [0x0F, 0x23, 0x21, 0x25, 0x24, 0x20, 0x2B, 0x29, 0x28,
                                     0x04, 0x05, 0x06, 0x07, 0x02, 0x03, 0x09, 0x0D])
            if is_reloc:
                matches += 1
            else:
                diffs.append((i, t_word, t_asm, c_word, c_asm))

    total = max(len(target_instrs), len(compiled_instrs))
    pct = 100.0 * matches / total if total > 0 else 0

    if verbose or diffs:
        print(f"\n  Match: {matches}/{total} ({pct:.1f}%)")
        if diffs:
            print(f"  Remaining diffs: {len(diffs)}")
            for idx, tw, ta, cw, ca in diffs:
                print(f"    [{idx:3d}] target: {ta:40s} compiled: {ca}")

    if matches == total:
        print("\n  PERFECT MATCH!")

    return pct


def main():
    parser = argparse.ArgumentParser(
        description='Simultaneous register chain renaming',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
    # Chain rename: v0->t8, t8->t9
    python3 rename_register_chain.py input.o output.o --chain v0:t8:t9

    # Simple unidirectional rename: v0->t8
    python3 rename_register_chain.py input.o output.o --chain v0:t8

    # Multiple chains
    python3 rename_register_chain.py input.o output.o --chain v0:t8:t9 --chain s1:s2

    # With target verification
    python3 rename_register_chain.py input.o output.o --chain v0:t8:t9 \\
        --target-exe disks/SLPS_000.65 --address 0x800956F0 --size 100
        """)

    parser.add_argument('input', help='Input .o file')
    parser.add_argument('output', help='Output .o file')
    parser.add_argument('--chain', action='append', required=True,
                       help='Register chain (e.g., v0:t8:t9). Can be specified multiple times.')
    parser.add_argument('--exclude-offsets',
                       help='Comma-separated list of .text offsets (object-relative, '
                            'e.g. "0xFC" or "0xFC,0x40") to skip. Mirrors '
                            'swap_registers_auto --exclude-offsets semantics.')
    parser.add_argument('--range', dest='byte_range', metavar='START:END',
                       help='Restrict renames to .text-relative byte range '
                            '[START, END) (e.g., "0x40:0x80"). Half-open. '
                            'Recommended when combined with --occurrence to '
                            'bound a per-occurrence rename to a single live '
                            'range and avoid downstream register clobbering.')
    parser.add_argument('--occurrence', action='append', default=[],
                       metavar='N[,N...]:src:dst',
                       help='Rename only the listed 1-indexed occurrences of '
                            'register `src` to `dst` (others are left alone). '
                            'Occurrences are counted within --range if given, '
                            'else the whole .text. Repeatable for different '
                            'sources. Example: "1,3:v0:t7" renames the 1st '
                            'and 3rd `v0` use to `t7`. The src:dst pair must '
                            'also appear in some --chain.')
    parser.add_argument('--target-exe', help='Target executable for verification')
    parser.add_argument('--address', help='VRAM address for verification')
    parser.add_argument('--size', type=int, help='Function size for verification')
    parser.add_argument('-v', '--verbose', action='store_true')

    args = parser.parse_args()

    combined_mapping = {}
    for chain_str in args.chain:
        mapping = parse_chain(chain_str)
        for k, v in mapping.items():
            if k in combined_mapping:
                old_name = REG_NAMES[k]
                print(f"Error: ${old_name} already has a mapping in another chain",
                      file=sys.stderr)
                sys.exit(1)
            combined_mapping[k] = v

    exclude_offsets = set()
    if args.exclude_offsets:
        for offset_str in args.exclude_offsets.split(','):
            offset_str = offset_str.strip()
            if offset_str:
                exclude_offsets.add(int(offset_str, 0))

    byte_range: Optional[Tuple[int, int]] = None
    if args.byte_range:
        try:
            start_str, end_str = args.byte_range.split(':')
            byte_range = (int(start_str, 0), int(end_str, 0))
        except ValueError:
            print(f"Error: --range must be 'START:END' (got '{args.byte_range}')",
                  file=sys.stderr)
            sys.exit(1)
        if byte_range[0] >= byte_range[1]:
            print(f"Error: --range start (0x{byte_range[0]:x}) must precede "
                  f"end (0x{byte_range[1]:x})", file=sys.stderr)
            sys.exit(1)

    occurrences: Dict[int, Set[int]] = {}
    for spec in args.occurrence:
        try:
            occ_part, src_name, dst_name = spec.split(':')
        except ValueError:
            print(f"Error: --occurrence must be 'N[,N...]:src:dst' (got '{spec}')",
                  file=sys.stderr)
            sys.exit(1)
        src = REGS.get(src_name.strip())
        dst = REGS.get(dst_name.strip())
        if src is None or dst is None:
            print(f"Error: unknown register in --occurrence '{spec}'", file=sys.stderr)
            sys.exit(1)
        if combined_mapping.get(src) != dst:
            print(f"Error: --occurrence src:dst (${src_name}:${dst_name}) does not "
                  f"match any --chain entry", file=sys.stderr)
            sys.exit(1)
        nums: Set[int] = set()
        for n_str in occ_part.split(','):
            n_str = n_str.strip()
            if not n_str:
                continue
            try:
                n = int(n_str, 0)
            except ValueError:
                print(f"Error: invalid occurrence index '{n_str}' in '{spec}'",
                      file=sys.stderr)
                sys.exit(1)
            if n < 1:
                print(f"Error: occurrence indices are 1-based (got {n})",
                      file=sys.stderr)
                sys.exit(1)
            nums.add(n)
        if src in occurrences:
            occurrences[src].update(nums)
        else:
            occurrences[src] = nums

    chain_desc = ", ".join(
        f"${REG_NAMES[k]}->${REG_NAMES[v]}"
        for k, v in combined_mapping.items()
    )
    excl_str = ""
    if exclude_offsets:
        excl_str = f" (excluding {len(exclude_offsets)} offset(s): " + \
                   ", ".join(f"0x{o:x}" for o in sorted(exclude_offsets)) + ")"
    range_str = ""
    if byte_range is not None:
        range_str = f" [range 0x{byte_range[0]:x}..0x{byte_range[1]:x}]"
    occ_str = ""
    if occurrences:
        parts = []
        for src, occ in occurrences.items():
            occ_list = ",".join(str(n) for n in sorted(occ))
            parts.append(f"${REG_NAMES[src]}={{{occ_list}}}")
        occ_str = f" [occurrences {' '.join(parts)}]"
    print(f"Register chain rename: {chain_desc}{excl_str}{range_str}{occ_str}")

    changes = apply_chain_to_elf(args.input, args.output, combined_mapping,
                                 args.verbose, exclude_offsets,
                                 byte_range, occurrences)
    print(f"Modified {changes} instruction(s)")

    if args.target_exe and args.address:
        vram_addr = int(args.address, 16)
        size = args.size
        if size is None:
            print("Warning: --size not specified, auto-detecting", file=sys.stderr)
            size = 256
        verify_against_target(args.output, args.target_exe, vram_addr, size, args.verbose)


if __name__ == '__main__':
    main()
