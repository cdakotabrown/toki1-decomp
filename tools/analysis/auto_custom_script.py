#!/usr/bin/env python3
"""
auto_custom_script.py - Automatically generate custom_script PostProc files from diff analysis.

Analyzes compiled-vs-target instruction differences and generates a Python custom_script
that applies register remaps and instruction swaps to fix them.

Usage:
    python3 tools/analysis/auto_custom_script.py func_80045ABC -c src/main/semi-matched/func_80045ABC.c
    python3 tools/analysis/auto_custom_script.py func_80045ABC -c src/main/semi-matched/func_80045ABC.c --apply
    python3 tools/analysis/auto_custom_script.py func_80045ABC -c src/main/semi-matched/func_80045ABC.c --dry-run

Options:
    --apply       Write the generated script to tools/ido/custom_<func>.py
    --dry-run     Show what would be generated without writing
    --verbose     Show detailed diff analysis
    --no-compile  Skip compilation, use pre-compiled object from --obj
    --obj FILE    Use pre-compiled object file instead of compiling
"""

import argparse
import json
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile
from collections import Counter, defaultdict
from pathlib import Path
from typing import Dict, List, Optional, Tuple

ROOT_DIR = Path(__file__).parent.parent.parent
ORIGINAL_EXE = ROOT_DIR / "disks" / "SLPS_000.65"
LEVEL3_CONFIG = ROOT_DIR / "config" / "level3_functions.json"
IDO_COMPILER = ROOT_DIR / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"

TEXT_SECTION_START = 0x80041000
FILE_OFFSET_BASE = 0x800
GAME_REGION_END = 0x80086000

# MIPS register names
REG_NAMES = {
    0: "zero", 1: "at", 2: "v0", 3: "v1",
    4: "a0", 5: "a1", 6: "a2", 7: "a3",
    8: "t0", 9: "t1", 10: "t2", 11: "t3",
    12: "t4", 13: "t5", 14: "t6", 15: "t7",
    16: "s0", 17: "s1", 18: "s2", 19: "s3",
    20: "s4", 21: "s5", 22: "s6", 23: "s7",
    24: "t8", 25: "t9", 26: "k0", 27: "k1",
    28: "gp", 29: "sp", 30: "fp", 31: "ra",
}

# Opcodes for load/store instructions
LOAD_STORE_OPS = {0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2B}
BRANCH_OPS = {4, 5, 6, 7, 1}
JUMP_OPS = {2, 3}

# Register classes
VOLATILE_REGS = set(range(0, 16)) | {24, 25}  # zero-t7, t8, t9
SAVED_REGS = set(range(16, 24))  # s0-s7


def parse_func_info_from_header(source_file: Path) -> Optional[Dict]:
    """Parse function info from C file header comments."""
    try:
        with open(source_file) as f:
            header = f.read(5000)
    except Exception:
        return None

    addr_m = re.search(r'Address:\s*(0x[0-9a-fA-F]+)', header)
    size_m = re.search(r'Size:\s*(\d+)\s*bytes', header)
    if not addr_m or not size_m:
        return None

    name = source_file.stem
    return {
        'name': name,
        'address': addr_m.group(1),
        'size': int(size_m.group(1)),
    }


def get_function_info(func_name: str, source_file: Optional[Path] = None) -> Optional[Dict]:
    """Get function metadata from level3_functions.json or C file header."""
    if LEVEL3_CONFIG.exists():
        with open(LEVEL3_CONFIG) as f:
            config = json.load(f)
        for func in config.get('functions', []):
            if func['name'] == func_name:
                if func.get('size', 0) == 0:
                    asm_path = ROOT_DIR / "asm" / "main" / "matched" / f"{func_name}.s"
                    if asm_path.exists():
                        with open(asm_path) as af:
                            for line in af:
                                m = re.search(r'Size:\s*(\d+)\s*bytes', line)
                                if m:
                                    func['size'] = int(m.group(1))
                                    break
                if func.get('size', 0) > 0:
                    return func

    # Fallback: parse from C file header
    if source_file and source_file.exists():
        return parse_func_info_from_header(source_file)
    return None


def find_text_section(data: bytes) -> Tuple[Optional[int], Optional[int]]:
    """Find .text section offset and size in ELF object file."""
    e_shoff = struct.unpack('<I', data[0x20:0x24])[0]
    e_shentsize = struct.unpack('<H', data[0x2e:0x30])[0]
    e_shnum = struct.unpack('<H', data[0x30:0x32])[0]
    e_shstrndx = struct.unpack('<H', data[0x32:0x34])[0]
    shstr_off = struct.unpack('<I', data[e_shoff + e_shstrndx * e_shentsize + 0x10:
                                        e_shoff + e_shstrndx * e_shentsize + 0x14])[0]
    for i in range(e_shnum):
        sh = e_shoff + i * e_shentsize
        name_off = struct.unpack('<I', data[sh:sh + 4])[0]
        name = b''
        j = shstr_off + name_off
        while j < len(data) and data[j] != 0:
            name += bytes([data[j]])
            j += 1
        if name == b'.text':
            return (struct.unpack('<I', data[sh + 0x10:sh + 0x14])[0],
                    struct.unpack('<I', data[sh + 0x14:sh + 0x18])[0])
    return None, None


def extract_target_words(func_info: Dict) -> List[int]:
    """Extract target instruction words from the original executable."""
    addr = int(func_info['address'], 16)
    size = func_info['size']
    file_off = (addr - TEXT_SECTION_START) + FILE_OFFSET_BASE
    with open(ORIGINAL_EXE, 'rb') as f:
        f.seek(file_off)
        data = f.read(size)
    return [struct.unpack('<I', data[i:i + 4])[0] for i in range(0, len(data), 4)]


def extract_compiled_words(obj_path: Path) -> List[int]:
    """Extract instruction words from compiled object .text section."""
    with open(obj_path, 'rb') as f:
        data = f.read()
    text_off, text_size = find_text_section(data)
    if text_off is None:
        return []
    return [struct.unpack('<I', data[text_off + i * 4:text_off + i * 4 + 4])[0]
            for i in range(text_size // 4)]


# Directive lines only OPEN with the keyword (after comment markers). Prose
# that quotes a directive mid-sentence must not be parsed as a directive.
_COMPILEFLAGS_LINE = re.compile(
    r'^\s*(?:(?:/\*+|\*+|//)\s*)*@CompileFlags:\s*(.*)$')
_POSTPROC_LINE = re.compile(
    r'^\s*(?:(?:/\*+|\*+|//)\s*)*@PostProc:\s*(.*)$')


def parse_compile_flags(source_file: Path) -> List[str]:
    """Parse @CompileFlags from source file header."""
    flags = []
    try:
        with open(source_file) as f:
            for line in f:
                if line.strip() == '*/':
                    break
                m = _COMPILEFLAGS_LINE.match(line)
                if m:
                    flag_str = m.group(1).strip().rstrip('*').strip()
                    flags.extend(flag_str.split())
    except Exception:
        pass
    return flags


def parse_existing_postproc(source_file: Path) -> List[str]:
    """Parse existing @PostProc directives from source file (excluding custom_script)."""
    steps = []
    try:
        with open(source_file) as f:
            content = f.read(20000)
        for line in content.split('\n'):
            m = _POSTPROC_LINE.match(line)
            if m:
                pp = m.group(1).strip().rstrip('*').rstrip('/').strip()
                if pp and 'custom_script' not in pp:
                    steps.append(pp)
    except Exception:
        pass
    return steps


def compile_source(source_file: Path, output_obj: Path, func_info: Dict) -> bool:
    """Compile C source with IDO 5.2 and apply basic PostProc."""
    if not IDO_COMPILER.exists():
        print("ERROR: IDO compiler not found. Run: make setup-ido52")
        return False

    cmd = [
        str(IDO_COMPILER), "-O2", "-G0", "-mips1", "-EL", "-non_shared",
        "-I", str(ROOT_DIR / "include"),
    ]

    extra_flags = parse_compile_flags(source_file)
    if extra_flags:
        cmd.extend(extra_flags)

    cmd.extend(["-c", str(source_file), "-o", str(output_obj)])

    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Compilation FAILED: {result.stderr}")
        return False

    # Apply basic PostProc steps (patch_stack_complete, convert_or_to_addu, swap_registers)
    postproc_steps = parse_existing_postproc(source_file)
    for step in postproc_steps:
        parts = step.split()
        tool_name = parts[0]
        tool_path = ROOT_DIR / "tools" / "ido" / f"{tool_name}.py"
        if not tool_path.exists():
            tool_path = ROOT_DIR / "tools" / "postproc" / f"{tool_name}.py"
        if not tool_path.exists():
            continue

        next_obj = output_obj.with_suffix('.pp.o')
        pp_cmd = [sys.executable, str(tool_path), str(output_obj), str(next_obj)] + parts[1:]
        pp_result = subprocess.run(pp_cmd, capture_output=True, text=True)
        if pp_result.returncode == 0 and next_obj.exists():
            shutil.move(str(next_obj), str(output_obj))

    return True


# ---- MIPS instruction field extraction ----

def decode_opcode(w: int) -> int:
    return (w >> 26) & 0x3F

def decode_rs(w: int) -> int:
    return (w >> 21) & 0x1F

def decode_rt(w: int) -> int:
    return (w >> 16) & 0x1F

def decode_rd(w: int) -> int:
    return (w >> 11) & 0x1F

def decode_shamt(w: int) -> int:
    return (w >> 6) & 0x1F

def decode_funct(w: int) -> int:
    return w & 0x3F

def decode_imm(w: int) -> int:
    return w & 0xFFFF


def reg_fields_for_opcode(op: int) -> List[str]:
    """Return which register fields are meaningful for this opcode."""
    if op == 0:
        return ['rs', 'rt', 'rd']
    elif op in JUMP_OPS:
        return []
    elif op in BRANCH_OPS:
        return ['rs', 'rt']
    elif op == 0x0F:  # lui
        return ['rt']
    elif op in LOAD_STORE_OPS:
        return ['rs', 'rt']
    elif op == 0x09:  # addiu
        return ['rs', 'rt']
    else:
        return ['rs', 'rt']


def get_reg_field(w: int, field: str) -> int:
    shifts = {'rs': 21, 'rt': 16, 'rd': 11}
    return (w >> shifts[field]) & 0x1F


def set_reg_field(w: int, field: str, val: int) -> int:
    shifts = {'rs': 21, 'rt': 16, 'rd': 11}
    shift = shifts[field]
    return (w & ~(0x1F << shift)) | ((val & 0x1F) << shift)


def is_reloc_dependent(w: int) -> bool:
    """Check if this instruction has a relocation-dependent immediate."""
    op = decode_opcode(w)
    if op in JUMP_OPS:
        return True
    if op in BRANCH_OPS:
        return True
    if op == 0x0F:  # lui
        return True
    rs = decode_rs(w)
    if op in LOAD_STORE_OPS and rs != 29:
        return True
    if op == 0x09 and rs != 29:  # addiu non-sp
        return True
    return False


def mask_relocatable(w: int) -> int:
    """Mask out relocation-dependent bits for comparison."""
    op = decode_opcode(w)
    if op in JUMP_OPS:
        return w & 0xFC000000
    if op in BRANCH_OPS:
        return w & 0xFFFF0000
    if op == 0x0F:  # lui
        return w & 0xFFFF0000
    rs = decode_rs(w)
    if op in LOAD_STORE_OPS and rs != 29:
        return w & 0xFFFF0000
    if op == 0x09 and rs != 29:
        return w & 0xFFFF0000
    return w


# ---- Diff analysis ----

class MismatchInfo:
    """Information about a single instruction mismatch."""
    def __init__(self, idx: int, comp_w: int, tgt_w: int):
        self.idx = idx
        self.offset = idx * 4
        self.comp_w = comp_w
        self.tgt_w = tgt_w
        self.comp_op = decode_opcode(comp_w)
        self.tgt_op = decode_opcode(tgt_w)
        self.same_opcode = self.comp_op == self.tgt_op
        self.same_funct = True
        if self.comp_op == 0 and self.tgt_op == 0:
            self.same_funct = decode_funct(comp_w) == decode_funct(tgt_w)

        # Classify mismatch
        self.reg_diffs = {}  # field -> (comp_reg, tgt_reg)
        self.imm_diff = False
        self.opcode_diff = not self.same_opcode
        self.funct_diff = not self.same_funct

        if self.same_opcode:
            fields = reg_fields_for_opcode(self.comp_op)
            for field in fields:
                cr = get_reg_field(comp_w, field)
                tr = get_reg_field(tgt_w, field)
                if cr != tr:
                    self.reg_diffs[field] = (cr, tr)

            # Check immediate
            if mask_relocatable(comp_w) == mask_relocatable(tgt_w):
                pass  # relocation difference, not a real mismatch
            elif decode_imm(comp_w) != decode_imm(tgt_w):
                self.imm_diff = True

    @property
    def is_reg_only(self) -> bool:
        return self.same_opcode and self.same_funct and self.reg_diffs and not self.imm_diff

    @property
    def is_reloc_only(self) -> bool:
        return mask_relocatable(self.comp_w) == mask_relocatable(self.tgt_w)


def find_basic_blocks(words: List[int]) -> List[int]:
    """Find basic block start indices."""
    starts = {0}
    n = len(words)
    for i, w in enumerate(words):
        op = decode_opcode(w)
        funct = decode_funct(w)
        imm = w & 0xFFFF
        if imm >= 0x8000:
            imm -= 0x10000
        if op in BRANCH_OPS:
            target = i + 1 + imm
            if 0 <= target < n:
                starts.add(target)
            starts.add(i + 2)
        elif (op == 0 and funct == 8) or op == 2:  # jr, j
            starts.add(i + 2)
        elif op == 3:  # jal
            starts.add(i + 2)
    return sorted(s for s in starts if 0 <= s < n)


def analyze_diff(comp_words: List[int], tgt_words: List[int], verbose: bool = False) -> Dict:
    """Analyze differences between compiled and target instructions.

    Returns a dict with:
        - mismatches: list of MismatchInfo
        - global_swaps: list of (reg_a, reg_b) global register swaps
        - per_block_remaps: dict of block_idx -> {comp_reg: tgt_reg}
        - instruction_swaps: list of (idx_a, idx_b) adjacent swap pairs
        - sp_offset_patches: list of (idx, comp_imm, tgt_imm)
        - stats: summary statistics
    """
    n = min(len(comp_words), len(tgt_words))

    # Collect all mismatches
    mismatches = []
    exact_matches = 0
    reloc_matches = 0

    for i in range(n):
        cw, tw = comp_words[i], tgt_words[i]
        if cw == tw:
            exact_matches += 1
            continue
        if mask_relocatable(cw) == mask_relocatable(tw):
            reloc_matches += 1
            continue
        mismatches.append(MismatchInfo(i, cw, tw))

    # -- Detect global register swaps --
    # Count (comp_reg -> tgt_reg) transitions across all mismatches
    reg_transitions = Counter()
    for mm in mismatches:
        if mm.is_reg_only:
            for field, (cr, tr) in mm.reg_diffs.items():
                reg_transitions[(cr, tr)] += 1

    # Find symmetric pairs: if A->B and B->A both appear, it's a swap
    global_swaps = []
    swap_candidates = set()
    for (cr, tr), count in reg_transitions.most_common():
        if cr == tr:
            continue
        rev_count = reg_transitions.get((tr, cr), 0)
        pair = (min(cr, tr), max(cr, tr))
        if pair in swap_candidates:
            continue
        if count >= 2 and rev_count >= 1:
            # Strong evidence of a swap
            global_swaps.append(pair)
            swap_candidates.add(pair)
        elif count >= 3 and rev_count == 0:
            # Unidirectional remap (one reg consistently maps to another)
            global_swaps.append((cr, tr))
            swap_candidates.add(pair)

    # Also detect unidirectional remaps where one register consistently
    # appears as the "wrong" one
    unidirectional_remaps = []
    for (cr, tr), count in reg_transitions.most_common():
        pair = (min(cr, tr), max(cr, tr))
        if pair in swap_candidates:
            continue
        rev_count = reg_transitions.get((tr, cr), 0)
        if count >= 2 and rev_count == 0:
            unidirectional_remaps.append((cr, tr))
            swap_candidates.add(pair)

    # -- Detect per-block register remaps --
    block_starts = find_basic_blocks(tgt_words)
    per_block_remaps = {}

    # Simulate applying global swaps to see remaining mismatches
    simulated = list(comp_words[:n])
    for a, b in global_swaps:
        for i in range(n):
            w = simulated[i]
            fields = reg_fields_for_opcode(decode_opcode(w))
            for field in fields:
                r = get_reg_field(w, field)
                if r == a:
                    w = set_reg_field(w, field, b)
                elif r == b:
                    w = set_reg_field(w, field, a)
            simulated[i] = w

    for bi in range(len(block_starts)):
        bs = block_starts[bi]
        be = block_starts[bi + 1] if bi + 1 < len(block_starts) else n
        block_transitions = Counter()
        for i in range(bs, min(be, n)):
            sw, tw = simulated[i], tgt_words[i]
            if sw == tw or mask_relocatable(sw) == mask_relocatable(tw):
                continue
            sop, top = decode_opcode(sw), decode_opcode(tw)
            if sop != top:
                continue
            fields = reg_fields_for_opcode(sop)
            for field in fields:
                sr = get_reg_field(sw, field)
                tr = get_reg_field(tw, field)
                if sr != tr:
                    block_transitions[(sr, tr)] += 1

        if block_transitions:
            # Build a mapping for this block
            mapping = {}
            used = set()
            for (sr, tr), count in sorted(block_transitions.items(), key=lambda x: -x[1]):
                if sr not in mapping and tr not in used and count >= 1:
                    mapping[sr] = tr
                    used.add(tr)
            if mapping:
                per_block_remaps[bi] = (bs, be, mapping)

    # -- Detect adjacent instruction swaps --
    # After applying register remaps, look for remaining mismatches
    # that could be fixed by swapping adjacent instructions
    sim2 = list(simulated)
    for bi, (bs, be, mapping) in per_block_remaps.items():
        for i in range(bs, min(be, n)):
            w = sim2[i]
            fields = reg_fields_for_opcode(decode_opcode(w))
            for field in fields:
                r = get_reg_field(w, field)
                if r in mapping:
                    w = set_reg_field(w, field, mapping[r])
            sim2[i] = w

    instruction_swaps = []
    i = 0
    while i < n - 1:
        cw0, cw1 = sim2[i], sim2[i + 1]
        tw0, tw1 = tgt_words[i], tgt_words[i + 1]
        # Check if swapping fixes both
        if cw0 != tw0 and cw1 != tw1:
            if (mask_relocatable(cw0) == mask_relocatable(tw1) and
                    mask_relocatable(cw1) == mask_relocatable(tw0)):
                instruction_swaps.append((i, i + 1))
                i += 2
                continue
        i += 1

    # -- Detect SP-offset patches --
    sp_offset_patches = []
    for mm in mismatches:
        if not mm.same_opcode:
            continue
        op = mm.comp_op
        if op not in LOAD_STORE_OPS and op != 0x09:
            continue
        c_rs = decode_rs(mm.comp_w)
        t_rs = decode_rs(mm.tgt_w)
        c_rt = decode_rt(mm.comp_w)
        t_rt = decode_rt(mm.tgt_w)
        if c_rs == 29 and t_rs == 29 and c_rt == t_rt:
            c_imm = decode_imm(mm.comp_w)
            t_imm = decode_imm(mm.tgt_w)
            if c_imm != t_imm:
                sp_offset_patches.append((mm.idx, c_imm, t_imm))

    stats = {
        'total_insns': n,
        'comp_size': len(comp_words),
        'tgt_size': len(tgt_words),
        'exact_matches': exact_matches,
        'reloc_matches': reloc_matches,
        'mismatches': len(mismatches),
        'size_match': len(comp_words) == len(tgt_words),
        'reg_only_mismatches': sum(1 for m in mismatches if m.is_reg_only),
        'opcode_mismatches': sum(1 for m in mismatches if m.opcode_diff),
        'global_swaps': len(global_swaps),
        'per_block_remaps': len(per_block_remaps),
        'instruction_swaps': len(instruction_swaps),
        'sp_offset_patches': len(sp_offset_patches),
    }

    return {
        'mismatches': mismatches,
        'global_swaps': global_swaps,
        'unidirectional_remaps': unidirectional_remaps,
        'per_block_remaps': per_block_remaps,
        'instruction_swaps': instruction_swaps,
        'sp_offset_patches': sp_offset_patches,
        'stats': stats,
        'block_starts': block_starts,
    }


def format_reg(r: int) -> str:
    return REG_NAMES.get(r, f"r{r}")


def simulate_script(comp_words: List[int], tgt_words: List[int], analysis: Dict) -> Tuple[int, int]:
    """Simulate applying the generated transforms and count matches before/after."""
    n = min(len(comp_words), len(tgt_words))
    before = sum(1 for i in range(n) if mask_relocatable(comp_words[i]) == mask_relocatable(tgt_words[i]))

    words = list(comp_words[:n])

    # Apply global swaps
    for a, b in analysis['global_swaps']:
        for i in range(n):
            w = words[i]
            fields = reg_fields_for_opcode(decode_opcode(w))
            for field in fields:
                r = get_reg_field(w, field)
                if r == a:
                    w = set_reg_field(w, field, b)
                elif r == b:
                    w = set_reg_field(w, field, a)
            words[i] = w

    # Apply unidirectional remaps
    for cr, tr in analysis['unidirectional_remaps']:
        for i in range(n):
            w = words[i]
            fields = reg_fields_for_opcode(decode_opcode(w))
            for field in fields:
                r = get_reg_field(w, field)
                if r == cr:
                    w = set_reg_field(w, field, tr)
            words[i] = w

    # Apply per-block remaps
    for bi, (bs, be, mapping) in analysis['per_block_remaps'].items():
        for i in range(bs, min(be, n)):
            w = words[i]
            fields = reg_fields_for_opcode(decode_opcode(w))
            for field in fields:
                r = get_reg_field(w, field)
                if r in mapping:
                    w = set_reg_field(w, field, mapping[r])
            words[i] = w

    # Apply instruction swaps
    for a, b in analysis['instruction_swaps']:
        words[a], words[b] = words[b], words[a]

    # Apply SP offset patches
    for idx, _comp_imm, tgt_imm in analysis['sp_offset_patches']:
        w = words[idx]
        w = (w & 0xFFFF0000) | (tgt_imm & 0xFFFF)
        words[idx] = w

    after = sum(1 for i in range(n) if mask_relocatable(words[i]) == mask_relocatable(tgt_words[i]))
    return before, after


def generate_script(func_name: str, analysis: Dict) -> str:
    """Generate a custom_script Python file from the analysis."""
    lines = []
    lines.append('#!/usr/bin/env python3')
    lines.append(f'"""Auto-generated custom_script for {func_name}.')
    lines.append('')
    lines.append('Transformations:')

    step = 1
    if analysis['global_swaps']:
        for a, b in analysis['global_swaps']:
            lines.append(f'  {step}. Global register swap: {format_reg(a)} <-> {format_reg(b)}')
            step += 1
    if analysis['unidirectional_remaps']:
        for cr, tr in analysis['unidirectional_remaps']:
            lines.append(f'  {step}. Unidirectional remap: {format_reg(cr)} -> {format_reg(tr)}')
            step += 1
    if analysis['per_block_remaps']:
        for bi, (bs, be, mapping) in sorted(analysis['per_block_remaps'].items()):
            maps = ', '.join(f'{format_reg(k)}->{format_reg(v)}' for k, v in mapping.items())
            lines.append(f'  {step}. Block {bi} remap (insns {bs}-{be-1}): {maps}')
            step += 1
    if analysis['instruction_swaps']:
        for a, b in analysis['instruction_swaps']:
            lines.append(f'  {step}. Swap instructions at offsets 0x{a*4:X} and 0x{b*4:X}')
            step += 1
    if analysis['sp_offset_patches']:
        for idx, c_imm, t_imm in analysis['sp_offset_patches']:
            lines.append(f'  {step}. SP offset patch at 0x{idx*4:X}: 0x{c_imm:X} -> 0x{t_imm:X}')
            step += 1

    # Simulation results
    lines.append('')
    stats = analysis['stats']
    lines.append(f'Stats: {stats["total_insns"]} insns, '
                 f'{stats["mismatches"]} mismatches '
                 f'({stats["reg_only_mismatches"]} reg-only, '
                 f'{stats["opcode_mismatches"]} opcode)')
    lines.append('"""')
    lines.append('import struct')
    lines.append('import sys')
    lines.append('')
    lines.append('')

    # Helper functions
    lines.append('def find_text_section(data):')
    lines.append('    """Find .text section offset and size in ELF."""')
    lines.append('    e_shoff = struct.unpack("<I", data[0x20:0x24])[0]')
    lines.append('    e_shentsize = struct.unpack("<H", data[0x2e:0x30])[0]')
    lines.append('    e_shnum = struct.unpack("<H", data[0x30:0x32])[0]')
    lines.append('    e_shstrndx = struct.unpack("<H", data[0x32:0x34])[0]')
    lines.append('    shstr_off = struct.unpack("<I", data[e_shoff + e_shstrndx * e_shentsize + 0x10:')
    lines.append('                                        e_shoff + e_shstrndx * e_shentsize + 0x14])[0]')
    lines.append('    for i in range(e_shnum):')
    lines.append('        sh = e_shoff + i * e_shentsize')
    lines.append('        name_off = struct.unpack("<I", data[sh:sh + 4])[0]')
    lines.append('        name = b""')
    lines.append('        j = shstr_off + name_off')
    lines.append('        while j < len(data) and data[j] != 0:')
    lines.append('            name += bytes([data[j]])')
    lines.append('            j += 1')
    lines.append('        if name == b".text":')
    lines.append('            return (struct.unpack("<I", data[sh + 0x10:sh + 0x14])[0],')
    lines.append('                    struct.unpack("<I", data[sh + 0x14:sh + 0x18])[0])')
    lines.append('    return None, None')
    lines.append('')
    lines.append('')

    # Decode helpers
    lines.append('def decode_opcode(w):')
    lines.append('    return (w >> 26) & 0x3F')
    lines.append('')
    lines.append('')

    # Register field constants
    lines.append('LOAD_STORE_OPS = {0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2B}')
    lines.append('BRANCH_OPS = {4, 5, 6, 7, 1}')
    lines.append('JUMP_OPS = {2, 3}')
    lines.append('')
    lines.append('')
    lines.append('def reg_fields_for_opcode(op):')
    lines.append('    if op == 0:')
    lines.append('        return [21, 16, 11]  # rs, rt, rd')
    lines.append('    elif op in JUMP_OPS:')
    lines.append('        return []')
    lines.append('    elif op in BRANCH_OPS:')
    lines.append('        return [21, 16]')
    lines.append('    elif op == 0x0F:  # lui')
    lines.append('        return [16]')
    lines.append('    else:')
    lines.append('        return [21, 16]  # I-type: rs, rt')
    lines.append('')
    lines.append('')

    # Main function
    lines.append('def main():')
    lines.append('    if len(sys.argv) != 3:')
    lines.append(f'        print(f"Usage: {{sys.argv[0]}} input.o output.o")')
    lines.append('        sys.exit(1)')
    lines.append('')
    lines.append('    with open(sys.argv[1], "rb") as f:')
    lines.append('        data = bytearray(f.read())')
    lines.append('')
    lines.append('    text_off, text_size = find_text_section(data)')
    lines.append('    if text_off is None:')
    lines.append('        print("ERROR: .text section not found")')
    lines.append('        sys.exit(1)')
    lines.append('')
    lines.append('    n = text_size // 4')
    lines.append('    words = []')
    lines.append('    for i in range(n):')
    lines.append('        w = struct.unpack("<I", data[text_off + i * 4:text_off + i * 4 + 4])[0]')
    lines.append('        words.append(w)')
    lines.append('')

    # Generate transform code
    if analysis['global_swaps']:
        lines.append('    # --- Global register swaps ---')
        for a, b in analysis['global_swaps']:
            lines.append(f'    # Swap {format_reg(a)} ({a}) <-> {format_reg(b)} ({b})')
            lines.append(f'    for i in range(n):')
            lines.append(f'        w = words[i]')
            lines.append(f'        op = decode_opcode(w)')
            lines.append(f'        for shift in reg_fields_for_opcode(op):')
            lines.append(f'            r = (w >> shift) & 0x1F')
            lines.append(f'            if r == {a}:')
            lines.append(f'                w = (w & ~(0x1F << shift)) | ({b} << shift)')
            lines.append(f'            elif r == {b}:')
            lines.append(f'                w = (w & ~(0x1F << shift)) | ({a} << shift)')
            lines.append(f'        words[i] = w')
            lines.append('')

    if analysis['unidirectional_remaps']:
        lines.append('    # --- Unidirectional register remaps ---')
        for cr, tr in analysis['unidirectional_remaps']:
            lines.append(f'    # Remap {format_reg(cr)} ({cr}) -> {format_reg(tr)} ({tr})')
            lines.append(f'    for i in range(n):')
            lines.append(f'        w = words[i]')
            lines.append(f'        op = decode_opcode(w)')
            lines.append(f'        for shift in reg_fields_for_opcode(op):')
            lines.append(f'            r = (w >> shift) & 0x1F')
            lines.append(f'            if r == {cr}:')
            lines.append(f'                w = (w & ~(0x1F << shift)) | ({tr} << shift)')
            lines.append(f'        words[i] = w')
            lines.append('')

    if analysis['per_block_remaps']:
        lines.append('    # --- Per-block register remaps ---')
        for bi, (bs, be, mapping) in sorted(analysis['per_block_remaps'].items()):
            maps = ', '.join(f'{format_reg(k)}->{format_reg(v)}' for k, v in mapping.items())
            lines.append(f'    # Block {bi} (insns {bs}-{be-1}): {maps}')
            lines.append(f'    for i in range({bs}, min({be}, n)):')
            lines.append(f'        w = words[i]')
            lines.append(f'        op = decode_opcode(w)')
            lines.append(f'        for shift in reg_fields_for_opcode(op):')
            lines.append(f'            r = (w >> shift) & 0x1F')
            for cr, tr in mapping.items():
                lines.append(f'            if r == {cr}:')
                lines.append(f'                w = (w & ~(0x1F << shift)) | ({tr} << shift)')
            lines.append(f'        words[i] = w')
            lines.append('')

    if analysis['instruction_swaps']:
        lines.append('    # --- Instruction swaps ---')
        for a, b in analysis['instruction_swaps']:
            lines.append(f'    # Swap instructions at offsets 0x{a*4:X} and 0x{b*4:X}')
            lines.append(f'    words[{a}], words[{b}] = words[{b}], words[{a}]')
        lines.append('')

    if analysis['sp_offset_patches']:
        lines.append('    # --- SP-offset patches ---')
        for idx, c_imm, t_imm in analysis['sp_offset_patches']:
            lines.append(f'    # Patch sp-offset at 0x{idx*4:X}: 0x{c_imm:X} -> 0x{t_imm:X}')
            lines.append(f'    words[{idx}] = (words[{idx}] & 0xFFFF0000) | 0x{t_imm:04X}')
        lines.append('')

    # Write back
    lines.append('    # Write back to .text section')
    lines.append('    for i in range(n):')
    lines.append('        struct.pack_into("<I", data, text_off + i * 4, words[i])')
    lines.append('')
    lines.append('    with open(sys.argv[2], "wb") as f:')
    lines.append('        f.write(data)')
    lines.append('')
    lines.append('')
    lines.append('if __name__ == "__main__":')
    lines.append('    main()')
    lines.append('')

    return '\n'.join(lines)


def print_analysis(analysis: Dict, verbose: bool = False):
    """Print a human-readable summary of the analysis."""
    stats = analysis['stats']
    print(f"\n=== Diff Analysis ===")
    print(f"Compiled: {stats['comp_size']} insns, Target: {stats['tgt_size']} insns")
    print(f"Size match: {'YES' if stats['size_match'] else 'NO (CANNOT generate custom_script)'}")
    print(f"Exact matches: {stats['exact_matches']}/{stats['total_insns']} "
          f"({100*stats['exact_matches']/stats['total_insns']:.1f}%)")
    print(f"Reloc matches: +{stats['reloc_matches']}")
    print(f"Mismatches: {stats['mismatches']} "
          f"({stats['reg_only_mismatches']} reg-only, "
          f"{stats['opcode_mismatches']} opcode)")

    if analysis['global_swaps']:
        print(f"\nGlobal swaps detected ({len(analysis['global_swaps'])}):")
        for a, b in analysis['global_swaps']:
            print(f"  {format_reg(a)} <-> {format_reg(b)}")

    if analysis['unidirectional_remaps']:
        print(f"\nUnidirectional remaps detected ({len(analysis['unidirectional_remaps'])}):")
        for cr, tr in analysis['unidirectional_remaps']:
            print(f"  {format_reg(cr)} -> {format_reg(tr)}")

    if analysis['per_block_remaps']:
        print(f"\nPer-block remaps ({len(analysis['per_block_remaps'])} blocks):")
        for bi, (bs, be, mapping) in sorted(analysis['per_block_remaps'].items()):
            maps = ', '.join(f'{format_reg(k)}->{format_reg(v)}' for k, v in mapping.items())
            print(f"  Block {bi} (insns {bs}-{be-1}): {maps}")

    if analysis['instruction_swaps']:
        print(f"\nInstruction swaps ({len(analysis['instruction_swaps'])}):")
        for a, b in analysis['instruction_swaps']:
            print(f"  0x{a*4:X} <-> 0x{b*4:X}")

    if analysis['sp_offset_patches']:
        print(f"\nSP-offset patches ({len(analysis['sp_offset_patches'])}):")
        for idx, c_imm, t_imm in analysis['sp_offset_patches']:
            print(f"  0x{idx*4:X}: sp+0x{c_imm:X} -> sp+0x{t_imm:X}")

    if verbose:
        print(f"\nAll mismatches:")
        for mm in analysis['mismatches']:
            flags = []
            if mm.opcode_diff:
                flags.append("OPCODE")
            if mm.funct_diff:
                flags.append("FUNCT")
            if mm.reg_diffs:
                reg_str = ', '.join(f'{f}:{format_reg(cr)}->{format_reg(tr)}'
                                    for f, (cr, tr) in mm.reg_diffs.items())
                flags.append(f"REG({reg_str})")
            if mm.imm_diff:
                flags.append("IMM")
            print(f"  [{mm.idx:3d}] 0x{mm.offset:04X}: "
                  f"{mm.comp_w:08X} vs {mm.tgt_w:08X}  {' '.join(flags)}")


def main():
    parser = argparse.ArgumentParser(
        description='Auto-generate custom_script PostProc from diff analysis')
    parser.add_argument('func_name', help='Function name (e.g., func_80045ABC)')
    parser.add_argument('-c', '--source', help='C source file path')
    parser.add_argument('--obj', help='Pre-compiled object file (skip compilation)')
    parser.add_argument('--apply', action='store_true',
                        help='Write generated script to tools/ido/')
    parser.add_argument('--dry-run', action='store_true',
                        help='Show generated script without writing')
    parser.add_argument('--verbose', '-v', action='store_true',
                        help='Show detailed diff analysis')
    parser.add_argument('--validate', action='store_true',
                        help='Validate by compiling and applying the generated script')
    args = parser.parse_args()

    func_name = args.func_name

    # Find source file first (needed for header-based info fallback)
    source_file = None
    if args.source:
        source_file = Path(args.source)
    else:
        for subdir in ['semi-matched', 'wip', 'matched']:
            candidate = ROOT_DIR / "src" / "main" / subdir / f"{func_name}.c"
            if candidate.exists():
                source_file = candidate
                break

    if source_file is None or not source_file.exists():
        print(f"ERROR: Source file not found for {func_name}")
        sys.exit(1)

    func_info = get_function_info(func_name, source_file)
    if not func_info:
        print(f"ERROR: Function {func_name} not found in config or C header")
        sys.exit(1)

    print(f"Function: {func_name}")
    print(f"Address: {func_info['address']}, Size: {func_info['size']} bytes "
          f"({func_info['size']//4} insns)")

    print(f"Source: {source_file}")

    # Get compiled words
    if args.obj:
        obj_path = Path(args.obj)
        comp_words = extract_compiled_words(obj_path)
    else:
        with tempfile.TemporaryDirectory() as tmpdir:
            obj_path = Path(tmpdir) / f"{func_name}.o"
            if not compile_source(source_file, obj_path, func_info):
                sys.exit(1)
            comp_words = extract_compiled_words(obj_path)

    if not comp_words:
        print("ERROR: Could not extract compiled instructions")
        sys.exit(1)

    # Get target words
    tgt_words = extract_target_words(func_info)

    print(f"Compiled: {len(comp_words)} insns, Target: {len(tgt_words)} insns")

    # Analyze
    analysis = analyze_diff(comp_words, tgt_words, args.verbose)
    print_analysis(analysis, args.verbose)

    # Simulate
    before, after = simulate_script(comp_words, tgt_words, analysis)
    n = min(len(comp_words), len(tgt_words))
    print(f"\nSimulation: {before}/{n} -> {after}/{n} masked matches "
          f"({100*before/n:.1f}% -> {100*after/n:.1f}%)")

    if not analysis['stats']['size_match']:
        print("\nWARNING: Instruction count mismatch -- cannot generate a valid custom_script.")
        print("custom_script HARD RULE: no instruction count changes allowed.")
        sys.exit(1)

    if after <= before:
        print("\nWARNING: Generated transforms do not improve the match.")
        if not args.dry_run and not args.apply:
            sys.exit(0)

    # Generate script
    script = generate_script(func_name, analysis)

    if args.dry_run or (not args.apply):
        print(f"\n=== Generated Script ===")
        print(script)

    if args.apply:
        script_path = ROOT_DIR / "tools" / "ido" / f"custom_{func_name}.py"
        with open(script_path, 'w') as f:
            f.write(script)
        os.chmod(script_path, 0o755)
        print(f"\nWritten: {script_path}")

        # Validate if requested
        if args.validate:
            print("\n=== Validation ===")
            with tempfile.TemporaryDirectory() as tmpdir:
                obj_path = Path(tmpdir) / f"{func_name}.o"
                out_path = Path(tmpdir) / f"{func_name}_pp.o"
                if compile_source(source_file, obj_path, func_info):
                    result = subprocess.run(
                        [sys.executable, str(script_path), str(obj_path), str(out_path)],
                        capture_output=True, text=True)
                    if result.returncode == 0:
                        pp_words = extract_compiled_words(out_path)
                        matches = sum(1 for i in range(min(len(pp_words), len(tgt_words)))
                                      if mask_relocatable(pp_words[i]) == mask_relocatable(tgt_words[i]))
                        total = min(len(pp_words), len(tgt_words))
                        print(f"After script: {matches}/{total} masked matches "
                              f"({100*matches/total:.1f}%)")
                    else:
                        print(f"Script execution FAILED: {result.stderr}")


if __name__ == '__main__':
    main()
