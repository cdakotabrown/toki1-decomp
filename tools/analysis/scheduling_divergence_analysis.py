#!/usr/bin/env python3
"""
Statistical analysis of scheduling divergence across matched functions.

For each matched C source:
1. Extract address and size from header comments
2. Compile with IDO 5.2
3. Extract .text section
4. Compare instruction-by-instruction against target binary
5. Categorize differences as reloc/sched/codegen

Outputs results to docs/SCHEDULING_DIVERGENCE_STATISTICS.md
"""

import os
import re
import struct
import subprocess
import sys
import tempfile
from collections import defaultdict
from pathlib import Path

PROJECT = Path(".")
MATCHED_DIR = PROJECT / "src" / "main" / "matched"
TARGET_EXE = PROJECT / "disks" / "SLPS_000.65"
IDO_CC = PROJECT / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
OBJCOPY = "mipsel-linux-gnu-objcopy"

TEXT_FILE_OFFSET = 0x800
TEXT_LOAD_ADDR = 0x80041000

OPCODE_MASK   = 0xFC000000
RS_MASK       = 0x03E00000
RT_MASK       = 0x001F0000
RD_MASK       = 0x0000F800
SHAMT_MASK    = 0x000007C0
FUNCT_MASK    = 0x0000003F
IMM_MASK      = 0x0000FFFF
TARGET_MASK   = 0x03FFFFFF


def get_opcode(instr):
    return (instr >> 26) & 0x3F

def get_funct(instr):
    return instr & 0x3F

def is_j_type(instr):
    return get_opcode(instr) in (2, 3)

def is_branch(instr):
    return get_opcode(instr) in (1, 4, 5, 6, 7)

def get_skeleton(instr):
    op = get_opcode(instr)
    if op == 0:
        return instr
    elif is_j_type(instr):
        return instr & OPCODE_MASK
    else:
        return instr & (OPCODE_MASK | RS_MASK | RT_MASK)

def get_instr_signature(instr):
    op = get_opcode(instr)
    if op == 0:
        return instr
    elif is_j_type(instr):
        return instr & OPCODE_MASK
    else:
        return instr & (OPCODE_MASK | RS_MASK | RT_MASK)

def get_instr_class(instr):
    op = get_opcode(instr)
    if instr == 0:
        return "nop"
    if op == 0:
        funct = get_funct(instr)
        if funct in (0, 2, 3, 4, 6, 7):
            return "shift"
        if funct in (8, 9):
            return "jr/jalr"
        if funct in (16, 18):
            return "mfhi/mflo"
        if funct in (24, 25, 26, 27):
            return "mult/div"
        return "alu-r"
    if op == 0x0F:
        return "lui"
    if op in (2, 3):
        return "j/jal"
    if op in (1, 4, 5, 6, 7):
        return "branch"
    if op in (8, 9, 0xA, 0xB, 0xC, 0xD, 0xE):
        return "alu-i"
    if op in (0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26):
        return "load"
    if op in (0x28, 0x29, 0x2A, 0x2B, 0x2E):
        return "store"
    return f"other-{op}"


def parse_source_header(filepath):
    """
    Parse a matched C source file header to extract address, size, compile flags.
    Returns (address, size_bytes, extra_flags, region) or None if unparseable.
    """
    with open(filepath, 'r', errors='replace') as f:
        header = f.read(3000)

    address = None
    size_bytes = None

    # Try "Address: 0xNNNNNNNN"
    m = re.search(r'Address:\s*0x([0-9A-Fa-f]+)', header)
    if m:
        address = int(m.group(1), 16)

    # Try "VRAM: 0xNNNNNNNN"
    if address is None:
        m = re.search(r'VRAM:\s*0x([0-9A-Fa-f]+)', header)
        if m:
            address = int(m.group(1), 16)

    # Try to extract from function name in filename
    if address is None:
        fname = filepath.stem
        m = re.match(r'func_(8[0-9A-Fa-f]{7})', fname)
        if m:
            # We know the address from the name but we can't determine size
            address = int(m.group(1), 16)

    if address is None:
        return None

    # Try "Size: NN bytes"
    m = re.search(r'Size:\s*(\d+)\s*bytes', header)
    if m:
        size_bytes = int(m.group(1))

    # Try "Address: 0xNNNN - 0xNNNN (NN bytes)"
    if size_bytes is None:
        m = re.search(r'0x[0-9A-Fa-f]+\s*-\s*0x([0-9A-Fa-f]+)\s*\((\d+)\s*bytes\)', header)
        if m:
            size_bytes = int(m.group(2))

    # Try "Address: 0xNNNN - 0xNNNN" (compute from range)
    if size_bytes is None:
        m = re.search(r'(?:Address|VRAM):\s*0x([0-9A-Fa-f]+)\s*-\s*0x([0-9A-Fa-f]+)', header)
        if m:
            start = int(m.group(1), 16)
            end = int(m.group(2), 16)
            size_bytes = end - start

    # Try "NN instructions" without explicit byte count
    if size_bytes is None:
        m = re.search(r'(\d+)\s*instructions', header)
        if m:
            size_bytes = int(m.group(1)) * 4

    if size_bytes is None:
        return None

    # Extract compile flags
    extra_flags = []
    m = re.search(r'@CompileFlags:\s*(.+)', header)
    if m:
        extra_flags = m.group(1).strip().split()

    region = "library" if address >= 0x80086000 else "game"

    return address, size_bytes, extra_flags, region


def compile_source(filepath, extra_flags=None):
    with tempfile.NamedTemporaryFile(suffix='.o', delete=False) as tf:
        obj_path = tf.name
    with tempfile.NamedTemporaryFile(suffix='.bin', delete=False) as tf:
        bin_path = tf.name

    try:
        cmd = [
            str(IDO_CC),
            '-O2', '-G0', '-mips1', '-EL', '-non_shared',
            '-I', str(PROJECT / 'include'),
            '-c', str(filepath),
            '-o', obj_path,
        ]
        if extra_flags:
            idx = cmd.index('-O2') + 1
            for flag in extra_flags:
                cmd.insert(idx, flag)
                idx += 1

        result = subprocess.run(
            cmd, capture_output=True, text=True, timeout=30, cwd=str(PROJECT)
        )
        if result.returncode != 0:
            return None

        result2 = subprocess.run(
            [OBJCOPY, '-O', 'binary', '-j', '.text', obj_path, bin_path],
            capture_output=True, text=True, timeout=10
        )
        if result2.returncode != 0:
            return None

        with open(bin_path, 'rb') as f:
            return f.read()

    except (subprocess.TimeoutExpired, Exception):
        return None
    finally:
        for p in [obj_path, bin_path]:
            try:
                os.unlink(p)
            except:
                pass


def read_target_bytes(address, size):
    file_offset = TEXT_FILE_OFFSET + (address - TEXT_LOAD_ADDR)
    with open(TARGET_EXE, 'rb') as f:
        f.seek(file_offset)
        return f.read(size)


def is_or_move(instr):
    if get_opcode(instr) != 0:
        return False
    if get_funct(instr) != 0x25:
        return False
    return ((instr >> 16) & 0x1F) == 0

def is_addu_move(instr):
    if get_opcode(instr) != 0:
        return False
    if get_funct(instr) != 0x21:
        return False
    return ((instr >> 16) & 0x1F) == 0

def is_move_encoding_diff(ci, ti):
    if is_or_move(ci) and is_addu_move(ti):
        return ((ci >> 11) & 0x1F) == ((ti >> 11) & 0x1F) and \
               ((ci >> 21) & 0x1F) == ((ti >> 21) & 0x1F)
    if is_addu_move(ci) and is_or_move(ti):
        return ((ci >> 11) & 0x1F) == ((ti >> 11) & 0x1F) and \
               ((ci >> 21) & 0x1F) == ((ti >> 21) & 0x1F)
    return False

def is_frame_size_diff(ci, ti):
    if get_opcode(ci) != 9 or get_opcode(ti) != 9:
        return False
    if ((ci >> 21) & 0x1F) != 29 or ((ci >> 16) & 0x1F) != 29:
        return False
    if ((ti >> 21) & 0x1F) != 29 or ((ti >> 16) & 0x1F) != 29:
        return False
    return (ci & IMM_MASK) != (ti & IMM_MASK)

def is_sp_offset_diff(ci, ti):
    skel_c = get_skeleton(ci)
    skel_t = get_skeleton(ti)
    if skel_c != skel_t:
        return False
    # Both must use sp as base
    if ((ci >> 21) & 0x1F) != 29:
        return False
    return (ci & IMM_MASK) != (ti & IMM_MASK)


def compare_instructions(compiled_bytes, target_bytes, num_instrs):
    results = []
    compiled_instrs = []
    target_instrs = []

    for i in range(num_instrs):
        off = i * 4
        ci = struct.unpack_from('<I', compiled_bytes, off)[0] if off + 4 <= len(compiled_bytes) else 0
        ti = struct.unpack_from('<I', target_bytes, off)[0] if off + 4 <= len(target_bytes) else 0
        compiled_instrs.append(ci)
        target_instrs.append(ti)

    compiled_sigs = defaultdict(list)
    target_sigs = defaultdict(list)
    for i, instr in enumerate(compiled_instrs):
        compiled_sigs[get_instr_signature(instr)].append(i)
    for i, instr in enumerate(target_instrs):
        target_sigs[get_instr_signature(instr)].append(i)

    for i in range(num_instrs):
        ci = compiled_instrs[i]
        ti = target_instrs[i]

        if ci == ti:
            results.append((i, ci, ti, 'exact'))
        elif is_move_encoding_diff(ci, ti):
            results.append((i, ci, ti, 'move_enc'))
        elif is_frame_size_diff(ci, ti):
            results.append((i, ci, ti, 'frame'))
        elif is_sp_offset_diff(ci, ti):
            results.append((i, ci, ti, 'sp_offset'))
        elif get_skeleton(ci) == get_skeleton(ti):
            results.append((i, ci, ti, 'reloc'))
        else:
            ci_sig = get_instr_signature(ci)
            ti_sig = get_instr_signature(ti)
            ci_in_target = ci_sig in target_sigs
            ti_in_compiled = ti_sig in compiled_sigs

            if ci_in_target and ti_in_compiled:
                results.append((i, ci, ti, 'sched'))
            else:
                results.append((i, ci, ti, 'codegen'))

    return results


def mips_disasm_simple(instr):
    op = get_opcode(instr)
    reg_names = [
        "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
        "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
        "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
        "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"
    ]
    rs = (instr >> 21) & 0x1F
    rt = (instr >> 16) & 0x1F
    rd = (instr >> 11) & 0x1F
    shamt = (instr >> 6) & 0x1F
    funct = instr & 0x3F
    imm = instr & 0xFFFF
    simm = imm if imm < 0x8000 else imm - 0x10000

    if instr == 0:
        return "nop"
    if op == 0:
        fnames = {
            0: "sll", 2: "srl", 3: "sra", 4: "sllv", 6: "srlv", 7: "srav",
            8: "jr", 9: "jalr", 12: "syscall", 13: "break",
            16: "mfhi", 17: "mthi", 18: "mflo", 19: "mtlo",
            24: "mult", 25: "multu", 26: "div", 27: "divu",
            32: "add", 33: "addu", 34: "sub", 35: "subu",
            36: "and", 37: "or", 38: "xor", 39: "nor",
            42: "slt", 43: "sltu",
        }
        fname = fnames.get(funct, f"r_func{funct}")
        if funct in (0, 2, 3):
            return f"{fname} ${reg_names[rd]},${reg_names[rt]},{shamt}"
        elif funct == 8:
            return f"jr ${reg_names[rs]}"
        elif funct == 9:
            return f"jalr ${reg_names[rd]},${reg_names[rs]}"
        elif funct in (16, 18):
            return f"{fname} ${reg_names[rd]}"
        elif funct in (24, 25, 26, 27):
            return f"{fname} ${reg_names[rs]},${reg_names[rt]}"
        else:
            return f"{fname} ${reg_names[rd]},${reg_names[rs]},${reg_names[rt]}"
    elif op == 0x0F:
        return f"lui ${reg_names[rt]},0x{imm:04X}"
    elif op in (2, 3):
        target = instr & TARGET_MASK
        tnames = {2: "j", 3: "jal"}
        return f"{tnames[op]} 0x{target << 2:08X}"
    elif op == 1:
        rnames = {0: "bltz", 1: "bgez", 16: "bltzal", 17: "bgezal"}
        rname = rnames.get(rt, f"regimm{rt}")
        return f"{rname} ${reg_names[rs]},{simm}"
    elif op in (4, 5, 6, 7):
        bnames = {4: "beq", 5: "bne", 6: "blez", 7: "bgtz"}
        if op in (6, 7):
            return f"{bnames[op]} ${reg_names[rs]},{simm}"
        return f"{bnames[op]} ${reg_names[rs]},${reg_names[rt]},{simm}"
    elif op in (8, 9, 0xA, 0xB, 0xC, 0xD, 0xE):
        inames = {8: "addi", 9: "addiu", 0xA: "slti", 0xB: "sltiu",
                  0xC: "andi", 0xD: "ori", 0xE: "xori"}
        if op in (0xC, 0xD, 0xE):
            return f"{inames[op]} ${reg_names[rt]},${reg_names[rs]},0x{imm:04X}"
        return f"{inames[op]} ${reg_names[rt]},${reg_names[rs]},{simm}"
    elif op >= 0x20:
        lsnames = {
            0x20: "lb", 0x21: "lh", 0x22: "lwl", 0x23: "lw",
            0x24: "lbu", 0x25: "lhu", 0x26: "lwr",
            0x28: "sb", 0x29: "sh", 0x2A: "swl", 0x2B: "sw", 0x2E: "swr",
        }
        lsname = lsnames.get(op, f"ls_op{op}")
        return f"{lsname} ${reg_names[rt]},{simm}(${reg_names[rs]})"
    return f"op{op:02X} 0x{instr:08X}"


def analyze_scheduling_patterns(all_diffs):
    patterns = defaultdict(int)
    class_swaps = defaultdict(int)
    displacement_hist = defaultdict(int)  # How far instructions are displaced

    for func_name, diffs in all_diffs.items():
        sched_diffs = [(i, ci, ti) for i, ci, ti, cat in diffs if cat == 'sched']
        all_compiled = [ci for _, ci, _, _ in diffs]
        all_target = [ti for _, _, ti, _ in diffs]

        for idx, ci, ti in sched_diffs:
            ci_class = get_instr_class(ci)
            ti_class = get_instr_class(ti)
            ci_dis = mips_disasm_simple(ci).split()[0]
            ti_dis = mips_disasm_simple(ti).split()[0]
            patterns[f"compiled:{ci_dis} vs target:{ti_dis}"] += 1
            class_swaps[f"{ci_class} <-> {ti_class}"] += 1

            # Find where the compiled instruction appears in target
            ci_sig = get_instr_signature(ci)
            for j, tinstr in enumerate(all_target):
                if get_instr_signature(tinstr) == ci_sig:
                    displacement_hist[abs(j - idx)] += 1
                    break

    return patterns, class_swaps, displacement_hist


def analyze_prologue_diffs(all_diffs):
    """Check if scheduling diffs are concentrated in prologues."""
    prologue_sched = 0
    body_sched = 0
    for func_name, diffs in all_diffs.items():
        num = len(diffs)
        prologue_end = min(6, num // 2)  # First 6 instructions or half the function
        for idx, ci, ti, cat in diffs:
            if cat == 'sched':
                if idx < prologue_end:
                    prologue_sched += 1
                else:
                    body_sched += 1
    return prologue_sched, body_sched


def main():
    print("Scheduling Divergence Analysis")
    print("=" * 60)

    sources = sorted(MATCHED_DIR.glob("*.c"))
    print(f"Found {len(sources)} matched C source files")

    results = []
    all_diffs = {}
    skipped = {"no_header": 0, "compile_fail": 0, "size_mismatch": 0, "too_small": 0, "bad_addr": 0}
    compile_errors = []

    for i, src in enumerate(sources):
        name = src.stem
        if name == "README":
            continue

        parsed = parse_source_header(src)
        if parsed is None:
            skipped["no_header"] += 1
            continue

        address, size_bytes, extra_flags, region = parsed
        num_instrs = size_bytes // 4

        if num_instrs < 2:
            skipped["too_small"] += 1
            continue

        if address < TEXT_LOAD_ADDR or address >= TEXT_LOAD_ADDR + 0x80000:
            skipped["bad_addr"] += 1
            continue

        compiled_bytes = compile_source(src, extra_flags)
        if compiled_bytes is None:
            skipped["compile_fail"] += 1
            compile_errors.append(name)
            continue

        target_bytes = read_target_bytes(address, size_bytes)
        compiled_len = len(compiled_bytes)

        if compiled_len < size_bytes:
            if compiled_len < size_bytes - 4:
                skipped["size_mismatch"] += 1
                continue
            compiled_bytes = compiled_bytes + b'\x00' * (size_bytes - compiled_len)

        diffs = compare_instructions(compiled_bytes, target_bytes, num_instrs)

        counts = defaultdict(int)
        for _, _, _, cat in diffs:
            counts[cat] += 1

        effective_match = counts['exact'] + counts['reloc'] + counts['move_enc'] + counts['frame'] + counts['sp_offset']
        match_pct = (effective_match / num_instrs * 100) if num_instrs > 0 else 0

        results.append({
            'name': name,
            'address': address,
            'region': region,
            'num_instrs': num_instrs,
            'size_bytes': size_bytes,
            'exact': counts['exact'],
            'reloc': counts['reloc'],
            'move_enc': counts['move_enc'],
            'frame': counts['frame'],
            'sp_offset': counts['sp_offset'],
            'sched': counts['sched'],
            'codegen': counts['codegen'],
            'match_pct': match_pct,
            'compiled_size': compiled_len,
        })

        if counts['sched'] > 0 or counts['codegen'] > 0:
            all_diffs[name] = diffs

        if (i + 1) % 50 == 0:
            print(f"  Processed {i+1}/{len(sources)} files...")

    print(f"\nProcessed: {len(results)} functions")
    print(f"Skipped: {sum(skipped.values())} (no_header={skipped['no_header']}, "
          f"compile_fail={skipped['compile_fail']}, size_mismatch={skipped['size_mismatch']}, "
          f"too_small={skipped['too_small']}, bad_addr={skipped['bad_addr']})")
    if compile_errors:
        print(f"Compile failures: {', '.join(compile_errors[:20])}")

    results.sort(key=lambda r: (r['match_pct'], -r['sched']))

    sched_patterns, class_swaps, displacement_hist = analyze_scheduling_patterns(all_diffs)
    prologue_sched, body_sched = analyze_prologue_diffs(all_diffs)

    generate_report(results, all_diffs, sched_patterns, class_swaps,
                    displacement_hist, prologue_sched, body_sched,
                    skipped, compile_errors)

    print(f"\nReport written to docs/SCHEDULING_DIVERGENCE_STATISTICS.md")


def generate_report(results, all_diffs, sched_patterns, class_swaps,
                    displacement_hist, prologue_sched, body_sched,
                    skipped, compile_errors):

    total = len(results)
    if total == 0:
        print("No results to report!")
        return

    zero_any_diff = sum(1 for r in results if r['sched'] == 0 and r['codegen'] == 0
                        and r['move_enc'] == 0 and r['frame'] == 0 and r['sp_offset'] == 0)
    zero_sched_codegen = sum(1 for r in results if r['sched'] == 0 and r['codegen'] == 0)
    has_sched = sum(1 for r in results if r['sched'] > 0)
    has_codegen = sum(1 for r in results if r['codegen'] > 0)
    has_move_enc = sum(1 for r in results if r['move_enc'] > 0)
    has_frame = sum(1 for r in results if r['frame'] > 0)
    has_sp_offset = sum(1 for r in results if r['sp_offset'] > 0)
    perfect_match = sum(1 for r in results if r['exact'] == r['num_instrs'])

    total_instrs = sum(r['num_instrs'] for r in results)
    total_exact = sum(r['exact'] for r in results)
    total_reloc = sum(r['reloc'] for r in results)
    total_move_enc = sum(r['move_enc'] for r in results)
    total_frame = sum(r['frame'] for r in results)
    total_sp_offset = sum(r['sp_offset'] for r in results)
    total_sched = sum(r['sched'] for r in results)
    total_codegen = sum(r['codegen'] for r in results)

    sched_dist = defaultdict(int)
    for r in results:
        sched_dist[r['sched']] += 1

    codegen_dist = defaultdict(int)
    for r in results:
        codegen_dist[r['codegen']] += 1

    game_results = [r for r in results if r['region'] == 'game']
    lib_results = [r for r in results if r['region'] == 'library']

    size_buckets = [
        ('tiny (1-5 instrs)', 1, 5),
        ('small (6-15 instrs)', 6, 15),
        ('medium (16-30 instrs)', 16, 30),
        ('large (31-60 instrs)', 31, 60),
        ('xlarge (61+ instrs)', 61, 99999),
    ]

    L = []

    L.append("# Scheduling Divergence Statistics")
    L.append("")
    L.append("## Overview")
    L.append("")
    L.append("**Generated**: 2026-02-06")
    L.append("")
    L.append("**Method**: Raw IDO 5.2 compilation (no post-processing) vs target binary (SLPS_000.65)")
    L.append("")
    L.append("This analysis compiles each matched C source with IDO 5.2 using the same flags")
    L.append("as the build system, extracts the .text section, and compares instruction-by-instruction")
    L.append("against the target binary. **No post-processing** (patch_stack, convert_or_to_addu,")
    L.append("swap_registers, etc.) is applied -- this shows the raw compiler output differences.")
    L.append("")
    L.append("### Difference Categories")
    L.append("")
    L.append("| Category | Meaning |")
    L.append("|----------|---------|")
    L.append("| **exact** | Byte-identical instruction |")
    L.append("| **reloc** | Same opcode+registers, different immediate (linker relocation) |")
    L.append("| **move_enc** | Same move but different encoding (`or` vs `addu` with $zero) |")
    L.append("| **frame** | `addiu sp,sp,X` with different X (frame size difference) |")
    L.append("| **sp_offset** | Same load/store from sp, different offset (frame layout) |")
    L.append("| **sched** | Same instruction exists elsewhere in function (pure reordering) |")
    L.append("| **codegen** | Different instruction entirely (register alloc, branch, etc.) |")
    L.append("")
    L.append(f"**Functions analyzed**: {total}")
    L.append(f"**Functions skipped**: {sum(skipped.values())} "
             f"(no header: {skipped['no_header']}, compile fail: {skipped['compile_fail']}, "
             f"size mismatch: {skipped['size_mismatch']}, too small: {skipped['too_small']}, "
             f"bad addr: {skipped['bad_addr']})")
    L.append("")

    if compile_errors:
        L.append("<details>")
        L.append("<summary>Compile failures (%d)</summary>" % len(compile_errors))
        L.append("")
        for e in compile_errors:
            L.append(f"- {e}")
        L.append("")
        L.append("</details>")
        L.append("")

    # === INSTRUCTION-LEVEL SUMMARY ===
    L.append("## Instruction-Level Summary")
    L.append("")
    L.append("| Category | Count | Percentage |")
    L.append("|----------|------:|------------|")
    L.append(f"| Total instructions | {total_instrs} | 100.0% |")
    L.append(f"| Exact matches | {total_exact} | {total_exact/total_instrs*100:.1f}% |")
    L.append(f"| Relocation-only diffs | {total_reloc} | {total_reloc/total_instrs*100:.1f}% |")
    L.append(f"| Move encoding diffs (or vs addu) | {total_move_enc} | {total_move_enc/total_instrs*100:.1f}% |")
    L.append(f"| Frame size diffs (addiu sp) | {total_frame} | {total_frame/total_instrs*100:.1f}% |")
    L.append(f"| SP offset diffs (lw/sw sp) | {total_sp_offset} | {total_sp_offset/total_instrs*100:.1f}% |")
    L.append(f"| **Scheduling diffs** | **{total_sched}** | **{total_sched/total_instrs*100:.1f}%** |")
    L.append(f"| **Codegen diffs** | **{total_codegen}** | **{total_codegen/total_instrs*100:.1f}%** |")
    effective = total_exact + total_reloc + total_move_enc + total_frame + total_sp_offset
    L.append(f"| **Effective match (exact+reloc+known-fixable)** | **{effective}** | **{effective/total_instrs*100:.1f}%** |")
    L.append("")

    # === FUNCTION-LEVEL SUMMARY ===
    L.append("## Function-Level Summary")
    L.append("")
    L.append("| Metric | Count | Percentage |")
    L.append("|--------|------:|------------|")
    L.append(f"| Byte-identical (exact-only match) | {perfect_match} | {perfect_match/total*100:.1f}% |")
    L.append(f"| Only exact+reloc diffs (no structural changes) | {zero_any_diff} | {zero_any_diff/total*100:.1f}% |")
    L.append(f"| No scheduling or codegen diffs | {zero_sched_codegen} | {zero_sched_codegen/total*100:.1f}% |")
    L.append(f"| Has move encoding diffs | {has_move_enc} | {has_move_enc/total*100:.1f}% |")
    L.append(f"| Has frame size diffs | {has_frame} | {has_frame/total*100:.1f}% |")
    L.append(f"| Has SP offset diffs | {has_sp_offset} | {has_sp_offset/total*100:.1f}% |")
    L.append(f"| **Has scheduling diffs** | **{has_sched}** | **{has_sched/total*100:.1f}%** |")
    L.append(f"| **Has codegen diffs** | **{has_codegen}** | **{has_codegen/total*100:.1f}%** |")
    L.append("")

    # === SCHEDULING DIFF DISTRIBUTION ===
    L.append("## Scheduling Diff Distribution")
    L.append("")
    L.append("| Sched diffs per function | Functions | Percentage | Cumulative |")
    L.append("|-------------------------:|----------:|------------|------------|")
    cumulative = 0
    for n in sorted(sched_dist.keys()):
        cumulative += sched_dist[n]
        L.append(f"| {n} | {sched_dist[n]} | {sched_dist[n]/total*100:.1f}% | {cumulative/total*100:.1f}% |")
    L.append("")

    # === CODEGEN DIFF DISTRIBUTION ===
    if has_codegen > 0:
        L.append("## Codegen Diff Distribution")
        L.append("")
        L.append("| Codegen diffs per function | Functions | Percentage |")
        L.append("|---------------------------:|----------:|------------|")
        for n in sorted(codegen_dist.keys()):
            L.append(f"| {n} | {codegen_dist[n]} | {codegen_dist[n]/total*100:.1f}% |")
        L.append("")

    # === PROLOGUE VS BODY ===
    L.append("## Prologue vs Body Location of Scheduling Diffs")
    L.append("")
    total_sched_loc = prologue_sched + body_sched
    if total_sched_loc > 0:
        L.append(f"| Location | Count | Percentage |")
        L.append(f"|----------|------:|------------|")
        L.append(f"| Prologue (first 6 instrs or first half) | {prologue_sched} | {prologue_sched/total_sched_loc*100:.1f}% |")
        L.append(f"| Body (remainder) | {body_sched} | {body_sched/total_sched_loc*100:.1f}% |")
        L.append("")
        L.append("This shows whether scheduling diffs cluster in function prologues (register saves)")
        L.append("or are distributed throughout the function body.")
    else:
        L.append("No scheduling diffs to analyze by location.")
    L.append("")

    # === DISPLACEMENT HISTOGRAM ===
    if displacement_hist:
        L.append("## Scheduling Displacement Distance")
        L.append("")
        L.append("How far are instructions displaced from their expected position?")
        L.append("")
        L.append("| Distance (instructions) | Count | Percentage |")
        L.append("|------------------------:|------:|------------|")
        total_displ = sum(displacement_hist.values())
        for d in sorted(displacement_hist.keys()):
            if d <= 10 or displacement_hist[d] > 2:
                L.append(f"| {d} | {displacement_hist[d]} | {displacement_hist[d]/total_displ*100:.1f}% |")
        L.append("")

    # === REGION COMPARISON ===
    L.append("## Region Comparison")
    L.append("")
    L.append("| Metric | Game Region | Library Region |")
    L.append("|--------|------------:|---------------:|")

    def region_stats(subset, label):
        if not subset:
            return {}
        ri = sum(r['num_instrs'] for r in subset)
        return {
            'count': len(subset),
            'instrs': ri,
            'sched_funcs': sum(1 for r in subset if r['sched'] > 0),
            'sched_instrs': sum(r['sched'] for r in subset),
            'codegen_funcs': sum(1 for r in subset if r['codegen'] > 0),
            'codegen_instrs': sum(r['codegen'] for r in subset),
            'move_enc': sum(r['move_enc'] for r in subset),
            'frame': sum(r['frame'] for r in subset),
            'sp_offset': sum(r['sp_offset'] for r in subset),
        }

    gs = region_stats(game_results, "Game")
    ls = region_stats(lib_results, "Library")

    def safe_pct(n, d):
        return f"{n/d*100:.1f}%" if d > 0 else "N/A"

    L.append(f"| Functions | {gs.get('count',0)} | {ls.get('count',0)} |")
    L.append(f"| Total instructions | {gs.get('instrs',0)} | {ls.get('instrs',0)} |")
    L.append(f"| Sched diff functions | {gs.get('sched_funcs',0)} ({safe_pct(gs.get('sched_funcs',0), gs.get('count',1))}) | {ls.get('sched_funcs',0)} ({safe_pct(ls.get('sched_funcs',0), ls.get('count',1))}) |")
    L.append(f"| Sched diff instructions | {gs.get('sched_instrs',0)} ({safe_pct(gs.get('sched_instrs',0), gs.get('instrs',1))}) | {ls.get('sched_instrs',0)} ({safe_pct(ls.get('sched_instrs',0), ls.get('instrs',1))}) |")
    L.append(f"| Codegen diff functions | {gs.get('codegen_funcs',0)} ({safe_pct(gs.get('codegen_funcs',0), gs.get('count',1))}) | {ls.get('codegen_funcs',0)} ({safe_pct(ls.get('codegen_funcs',0), ls.get('count',1))}) |")
    L.append(f"| Codegen diff instructions | {gs.get('codegen_instrs',0)} ({safe_pct(gs.get('codegen_instrs',0), gs.get('instrs',1))}) | {ls.get('codegen_instrs',0)} ({safe_pct(ls.get('codegen_instrs',0), ls.get('instrs',1))}) |")
    L.append(f"| Move encoding diffs | {gs.get('move_enc',0)} | {ls.get('move_enc',0)} |")
    L.append(f"| Frame size diffs | {gs.get('frame',0)} | {ls.get('frame',0)} |")
    L.append(f"| SP offset diffs | {gs.get('sp_offset',0)} | {ls.get('sp_offset',0)} |")
    L.append("")

    # === SIZE CORRELATION ===
    L.append("## Size Correlation")
    L.append("")
    L.append("| Size Bucket | Functions | With Sched | Rate | Avg Sched | With Codegen | Codegen Rate |")
    L.append("|-------------|----------:|-----------:|-----:|----------:|-------------:|-------------:|")
    for label, lo, hi in size_buckets:
        bucket = [r for r in results if lo <= r['num_instrs'] <= hi]
        if bucket:
            with_sched = sum(1 for r in bucket if r['sched'] > 0)
            with_codegen = sum(1 for r in bucket if r['codegen'] > 0)
            avg_sched = sum(r['sched'] for r in bucket) / len(bucket)
            L.append(f"| {label} | {len(bucket)} | {with_sched} | {safe_pct(with_sched, len(bucket))} | {avg_sched:.1f} | {with_codegen} | {safe_pct(with_codegen, len(bucket))} |")
    L.append("")

    # === SCHEDULING DIFF PATTERNS ===
    if sched_patterns:
        L.append("## Scheduling Diff Instruction Patterns")
        L.append("")
        L.append("What instruction opcode is at position X in compiled output vs what the target has at the same position:")
        L.append("")
        L.append("| Pattern (compiled vs target at same position) | Count |")
        L.append("|-----------------------------------------------|------:|")
        for pattern, count in sorted(sched_patterns.items(), key=lambda x: -x[1])[:40]:
            L.append(f"| `{pattern}` | {count} |")
        L.append("")

    if class_swaps:
        L.append("## Instruction Class Swaps in Scheduling Diffs")
        L.append("")
        L.append("| Class swap | Count |")
        L.append("|------------|------:|")
        for swap, count in sorted(class_swaps.items(), key=lambda x: -x[1])[:30]:
            L.append(f"| `{swap}` | {count} |")
        L.append("")

    # === FULL FUNCTION TABLE ===
    L.append("## Full Function Table")
    L.append("")
    L.append("Sorted by effective match percentage (lowest first).")
    L.append("")
    L.append("| # | Function | Address | Rgn | Instrs | Exact | Reloc | MvEnc | Frame | SpOff | Sched | CdGen | Match% |")
    L.append("|--:|----------|---------|-----|-------:|------:|------:|------:|------:|------:|------:|------:|-------:|")

    for i, r in enumerate(results):
        rgn = "G" if r['region'] == 'game' else "L"
        L.append(
            f"| {i+1} | {r['name']} | 0x{r['address']:08X} | {rgn} | {r['num_instrs']} | "
            f"{r['exact']} | {r['reloc']} | {r['move_enc']} | {r['frame']} | {r['sp_offset']} | "
            f"{r['sched']} | {r['codegen']} | {r['match_pct']:.1f}% |"
        )
    L.append("")

    # === DETAILED DIFFS ===
    funcs_with_diffs = [(name, diffs) for name, diffs in all_diffs.items()]
    funcs_with_diffs.sort(key=lambda x: sum(1 for _, _, _, c in x[1] if c in ('sched', 'codegen')), reverse=True)

    if funcs_with_diffs:
        L.append("## Detailed Diff Analysis")
        L.append("")
        L.append("Instruction-level differences for functions with scheduling or codegen diffs.")
        L.append("Top 40 by total non-trivial diff count.")
        L.append("")

        for name, diffs in funcs_with_diffs[:40]:
            sched_count = sum(1 for _, _, _, c in diffs if c == 'sched')
            codegen_count = sum(1 for _, _, _, c in diffs if c == 'codegen')
            move_count = sum(1 for _, _, _, c in diffs if c == 'move_enc')
            frame_count = sum(1 for _, _, _, c in diffs if c == 'frame')
            sp_count = sum(1 for _, _, _, c in diffs if c == 'sp_offset')
            reloc_count = sum(1 for _, _, _, c in diffs if c == 'reloc')

            L.append(f"### {name}")
            L.append(f"{len(diffs)} instrs | sched:{sched_count} codegen:{codegen_count} reloc:{reloc_count} move_enc:{move_count} frame:{frame_count} sp_off:{sp_count}")
            L.append("")

            non_exact = [(i, ci, ti, cat) for i, ci, ti, cat in diffs if cat != 'exact']
            if non_exact:
                L.append("| Idx | Category | Compiled | Target |")
                L.append("|----:|----------|----------|--------|")

                for idx, ci, ti, cat in non_exact:
                    ci_dis = mips_disasm_simple(ci)
                    ti_dis = mips_disasm_simple(ti)
                    if cat in ('sched', 'codegen'):
                        L.append(f"| {idx} | **{cat}** | `{ci_dis}` | `{ti_dis}` |")
                    else:
                        L.append(f"| {idx} | {cat} | `{ci_dis}` | `{ti_dis}` |")
                L.append("")

    # === ANALYSIS ===
    L.append("## Analysis and Conclusions")
    L.append("")

    L.append("### Key Findings")
    L.append("")
    effective = total_exact + total_reloc + total_move_enc + total_frame + total_sp_offset
    L.append(f"1. **Overall effective match rate**: {effective/total_instrs*100:.1f}% "
             f"({effective}/{total_instrs} instructions match exactly or have known-fixable diffs)")
    L.append(f"2. **Functions with zero scheduling/codegen diffs**: {zero_sched_codegen}/{total} ({zero_sched_codegen/total*100:.1f}%)")
    L.append(f"3. **Scheduling diffs**: {total_sched}/{total_instrs} instructions ({total_sched/total_instrs*100:.2f}%)")
    L.append(f"4. **Codegen diffs**: {total_codegen}/{total_instrs} instructions ({total_codegen/total_instrs*100:.2f}%)")
    L.append(f"5. **Byte-identical functions**: {perfect_match}/{total} ({perfect_match/total*100:.1f}%)")
    L.append("")

    if has_sched > 0:
        sched_vals = sorted([r['sched'] for r in results if r['sched'] > 0])
        avg_sched = total_sched / has_sched
        median_sched = sched_vals[len(sched_vals)//2]
        max_sched = max(sched_vals)

        L.append("### Scheduling Characteristics")
        L.append("")
        L.append(f"- Functions with scheduling diffs: {has_sched}/{total} ({has_sched/total*100:.1f}%)")
        L.append(f"- Average scheduling diffs when present: {avg_sched:.1f}")
        L.append(f"- Median scheduling diffs when present: {median_sched}")
        L.append(f"- Maximum scheduling diffs: {max_sched}")
        total_sched_loc = prologue_sched + body_sched
        if total_sched_loc > 0:
            L.append(f"- Prologue-located scheduling diffs: {prologue_sched}/{total_sched_loc} ({prologue_sched/total_sched_loc*100:.1f}%)")
            L.append(f"- Body-located scheduling diffs: {body_sched}/{total_sched_loc} ({body_sched/total_sched_loc*100:.1f}%)")
        L.append("")

    L.append("### What the Categories Mean for Matching")
    L.append("")
    L.append("- **exact + reloc**: These match. Relocation diffs are resolved by the linker.")
    L.append("- **move_enc**: Fixed by `convert_or_to_addu.py` (library region only).")
    L.append("- **frame + sp_offset**: Fixed by `patch_stack_complete.py`.")
    L.append("- **sched**: The **irreducible scheduling gap** -- same instructions, different order.")
    L.append("  Requires finding the correct as1 version or building a post-link rewriter.")
    L.append("- **codegen**: Genuinely different instructions -- register allocation differences,")
    L.append("  branch differences, etc. Requires C source changes or register swap tools.")
    L.append("")

    L.append("### Implications for the Decompilation Project")
    L.append("")
    if total_sched + total_codegen > 0:
        unfixable = total_sched + total_codegen
        trivial = total_move_enc + total_frame + total_sp_offset
        L.append(f"- **Trivially fixable** by post-processing: {trivial} instructions ({trivial/total_instrs*100:.1f}%)")
        L.append(f"- **Unfixable without source/tool changes**: {unfixable} instructions ({unfixable/total_instrs*100:.2f}%)")
        L.append(f"  - Pure scheduling: {total_sched} ({total_sched/total_instrs*100:.2f}%)")
        L.append(f"  - Codegen: {total_codegen} ({total_codegen/total_instrs*100:.2f}%)")
        L.append("")
        L.append("The scheduling gap remains the primary systematic difference. Resolving it would")
        L.append("enable many more functions to reach 100% match status after post-processing.")
    L.append("")

    report_path = PROJECT / "docs" / "SCHEDULING_DIVERGENCE_STATISTICS.md"
    with open(report_path, 'w') as f:
        f.write('\n'.join(L) + '\n')


if __name__ == '__main__':
    main()
