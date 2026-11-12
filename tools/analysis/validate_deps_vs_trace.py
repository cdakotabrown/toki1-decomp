#!/usr/bin/env python3
"""
Validate scheduler_normalizer dependency model against as1 -R traces.

Parses as1's -R trace output (which shows the real dependency DAG used
for scheduling) and compares it against the normalizer's DependencyAnalyzer.

Reports:
- MISSING: trace has edge A->B but normalizer does not (under-conservative)
- EXTRA: normalizer has edge A->B but trace does not (over-conservative)

Usage:
    python3 tools/analysis/validate_deps_vs_trace.py [--trace-dir DIR]
"""

import argparse
import glob
import os
import re
import struct
import subprocess
import sys
from collections import defaultdict
from dataclasses import dataclass, field
from typing import Dict, List, Optional, Set, Tuple

# Add parent tools directory to path for imports
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', 'postproc'))
from scheduler_normalizer import DependencyAnalyzer, Instruction


@dataclass
class TraceNode:
    """A node from an as1 -R trace basic block"""
    node_num: int
    inst_word: int
    relocation: int
    lineno: int
    before_count: int
    aftercycles: int
    maxhazard: int
    afternodes: List[Tuple[int, ...]]  # (target_node, latency[, hazard])


@dataclass
class TraceBB:
    """A basic block from an as1 -R trace"""
    nodes: Dict[int, TraceNode]  # node_num -> TraceNode
    initial_nodes: List[int]

    def instruction_order(self) -> List[int]:
        """Return node numbers in original instruction order (highest first)"""
        return sorted(self.nodes.keys(), reverse=True)


def parse_trace(trace_path: str) -> List[TraceBB]:
    """Parse an as1 -R trace file into basic blocks with dependency edges."""
    bbs = []
    current_nodes = {}
    current_initial = []
    in_scheduling = False  # True when reading "Picking node" lines

    with open(trace_path) as f:
        lines = f.readlines()

    i = 0
    while i < len(lines):
        line = lines[i].strip()

        # New basic block starts with node definitions
        m_node = re.match(
            r'Node\s+(\d+):\s+inst\s+([0-9a-fA-F]+),\s+relocation\s+(\d+),\s+lineno\s+(\d+)',
            line)
        if m_node:
            if in_scheduling:
                # We were in scheduling mode, save previous BB
                if current_nodes:
                    bbs.append(TraceBB(nodes=current_nodes, initial_nodes=current_initial))
                current_nodes = {}
                current_initial = []
                in_scheduling = False

            node_num = int(m_node.group(1))
            inst_word = int(m_node.group(2), 16)
            reloc = int(m_node.group(3))
            lineno = int(m_node.group(4))

            # Parse the properties line (next line)
            i += 1
            if i < len(lines):
                props_line = lines[i].strip()
                m_props = re.match(
                    r'before\s+(\d+),\s+aftercycles\s+(\d+),\s+maxhazard\s+(\d+)',
                    props_line)
                before = int(m_props.group(1)) if m_props else 0
                aftercycles = int(m_props.group(2)) if m_props else 0
                maxhazard = int(m_props.group(3)) if m_props else 0

            # Parse afternodes line (next line)
            i += 1
            afternodes = []
            if i < len(lines):
                an_line = lines[i].strip()
                m_an = re.match(r'afternodes:\s*(.*)', an_line)
                if m_an:
                    an_str = m_an.group(1).strip()
                    if an_str:
                        for part in an_str.split():
                            nums = tuple(int(x) for x in part.split('/'))
                            afternodes.append(nums)

            current_nodes[node_num] = TraceNode(
                node_num=node_num,
                inst_word=inst_word,
                relocation=reloc,
                lineno=lineno,
                before_count=before,
                aftercycles=aftercycles,
                maxhazard=maxhazard,
                afternodes=afternodes
            )
            i += 1
            continue

        # Initial nodes line marks start of scheduling phase
        m_init = re.match(r'Initial nodes:\s*(.*)', line)
        if m_init:
            init_str = m_init.group(1).strip()
            if init_str:
                current_initial = [int(x) for x in init_str.split()]
                in_scheduling = True
            elif current_nodes:
                # Empty initial nodes for a non-empty BB
                current_initial = []
                in_scheduling = True
            i += 1
            continue

        # Picking node lines - part of scheduling, skip
        if line.startswith('Picking node') or line.startswith('node '):
            i += 1
            continue

        i += 1

    # Save last BB
    if current_nodes:
        bbs.append(TraceBB(nodes=current_nodes, initial_nodes=current_initial))

    # Filter out empty BBs
    return [bb for bb in bbs if len(bb.nodes) >= 2]


def disassemble_o_file(o_path: str) -> Dict[int, str]:
    """Disassemble .o file, return {hex_word: disasm_text} lookup.

    Since the same hex word always disassembles to the same text, we can
    build a lookup table. For instructions with different immediates/offsets
    that map to different hex words, each gets its own entry.
    """
    result = {}
    try:
        out = subprocess.check_output(
            ['mipsel-linux-gnu-objdump', '-d', '-M', 'reg-names=o32', o_path],
            stderr=subprocess.DEVNULL, text=True)
    except (subprocess.CalledProcessError, FileNotFoundError):
        return result

    for line in out.splitlines():
        # Format: "   0:   27bdffe8        addiu   sp,sp,-24"
        m = re.match(r'\s*[0-9a-f]+:\s+([0-9a-f]{8})\s+(.+)', line)
        if m:
            word = int(m.group(1), 16)
            text = m.group(2).strip()
            # Normalize whitespace
            text = re.sub(r'\s+', ' ', text)
            # Remove trailing comments
            text = re.sub(r'\s*#.*', '', text)
            result[word] = text

    return result


def decode_instruction_minimal(word: int) -> str:
    """Minimal MIPS I decoder for common instructions.

    This is a fallback when objdump can't decode (e.g., for instructions
    not in the .o file due to scheduling reorder).
    """
    if word == 0:
        return "nop"

    opcode = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    sa = (word >> 6) & 0x1F
    funct = word & 0x3F
    imm = word & 0xFFFF
    simm = imm if imm < 0x8000 else imm - 0x10000

    reg_names = [
        'zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
        't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
        's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
        't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra'
    ]
    R = lambda n: reg_names[n]

    if opcode == 0:  # R-type
        r_ops = {
            0: 'sll', 2: 'srl', 3: 'sra', 4: 'sllv', 6: 'srlv', 7: 'srav',
            8: 'jr', 9: 'jalr',
            16: 'mfhi', 17: 'mthi', 18: 'mflo', 19: 'mtlo',
            24: 'mult', 25: 'multu', 26: 'div', 27: 'divu',
            32: 'add', 33: 'addu', 34: 'sub', 35: 'subu',
            36: 'and', 37: 'or', 38: 'xor', 39: 'nor',
            42: 'slt', 43: 'sltu'
        }
        name = r_ops.get(funct, f'r_unk_{funct}')

        if funct == 8:  # jr
            return f"jr {R(rs)}"
        elif funct == 9:  # jalr
            return f"jalr {R(rd)},{R(rs)}"
        elif funct in (16, 18):  # mfhi/mflo
            return f"{name} {R(rd)}"
        elif funct in (17, 19):  # mthi/mtlo
            return f"{name} {R(rs)}"
        elif funct in (24, 25, 26, 27):  # mult/div
            return f"{name} {R(rs)},{R(rt)}"
        elif funct in (0, 2, 3):  # sll/srl/sra
            if word == 0:
                return "nop"
            return f"{name} {R(rd)},{R(rt)},{sa}"
        elif funct in (4, 6, 7):  # sllv/srlv/srav
            return f"{name} {R(rd)},{R(rt)},{R(rs)}"
        else:
            # Detect move (or rd, zero, rt) or (addu rd, zero, rt)
            if funct == 37 and rs == 0:  # or rd, zero, rt -> move
                return f"move {R(rd)},{R(rt)}"
            if funct == 33 and rs == 0:  # addu rd, zero, rt -> move
                return f"move {R(rd)},{R(rt)}"
            return f"{name} {R(rd)},{R(rs)},{R(rt)}"

    elif opcode == 1:  # REGIMM
        regimm_ops = {0: 'bltz', 1: 'bgez', 16: 'bltzal', 17: 'bgezal'}
        name = regimm_ops.get(rt, f'regimm_{rt}')
        return f"{name} {R(rs)},0x{imm:04x}"

    elif opcode in (2, 3):  # J-type
        target = word & 0x3FFFFFF
        name = 'j' if opcode == 2 else 'jal'
        return f"{name} 0x{target << 2:08x}"

    else:  # I-type
        i_ops = {
            4: 'beq', 5: 'bne', 6: 'blez', 7: 'bgtz',
            8: 'addi', 9: 'addiu', 10: 'slti', 11: 'sltiu',
            12: 'andi', 13: 'ori', 14: 'xori', 15: 'lui',
            32: 'lb', 33: 'lh', 34: 'lwl', 35: 'lw',
            36: 'lbu', 37: 'lhu', 38: 'lwr',
            40: 'sb', 41: 'sh', 42: 'swl', 43: 'sw', 46: 'swr'
        }
        name = i_ops.get(opcode, f'i_unk_{opcode}')

        if opcode in (4, 5):  # beq/bne
            return f"{name} {R(rs)},{R(rt)},0x{imm:04x}"
        elif opcode in (6, 7):  # blez/bgtz
            return f"{name} {R(rs)},0x{imm:04x}"
        elif opcode == 15:  # lui
            return f"lui {R(rt)},0x{imm:04x}"
        elif opcode in (32, 33, 34, 35, 36, 37, 38):  # loads
            return f"{name} {R(rt)},{simm}({R(rs)})"
        elif opcode in (40, 41, 42, 43, 46):  # stores
            return f"{name} {R(rt)},{simm}({R(rs)})"
        elif opcode in (12, 13, 14):  # logical immediate
            return f"{name} {R(rt)},{R(rs)},0x{imm:04x}"
        else:  # arithmetic immediate
            return f"{name} {R(rt)},{R(rs)},{simm}"

    return f"unknown 0x{word:08x}"


def get_disasm_text(word: int, lookup: Dict[int, str]) -> str:
    """Get disassembly text for an instruction word."""
    if word in lookup:
        return lookup[word]
    return decode_instruction_minimal(word)


def build_trace_dep_graph(bb: TraceBB) -> Dict[int, Set[int]]:
    """Build dependency graph from trace afternodes.

    Returns deps[i] = set of instruction indices that instruction i depends on,
    where i is the index in instruction order (0 = first instruction = highest node num).

    afternodes: "Node A has afternode B" means B depends on A.
    """
    node_nums = bb.instruction_order()  # highest first
    node_to_idx = {n: i for i, n in enumerate(node_nums)}

    deps = defaultdict(set)
    for node_num, node in bb.nodes.items():
        src_idx = node_to_idx[node_num]
        for an in node.afternodes:
            tgt_node = an[0]
            if tgt_node in node_to_idx:
                tgt_idx = node_to_idx[tgt_node]
                # tgt depends on src
                deps[tgt_idx].add(src_idx)

    return deps


def build_normalizer_dep_graph(instructions: List[Instruction]) -> Dict[int, Set[int]]:
    """Build dependency graph using the normalizer's DependencyAnalyzer."""
    n = len(instructions)
    deps = defaultdict(set)
    for i in range(n):
        for j in range(i):
            if DependencyAnalyzer.has_dependency(instructions[j], instructions[i]):
                deps[i].add(j)
    return deps


def compare_dep_graphs(trace_deps: Dict[int, Set[int]],
                       norm_deps: Dict[int, Set[int]],
                       n: int) -> Tuple[List[Tuple[int, int]], List[Tuple[int, int]]]:
    """Compare trace and normalizer dependency graphs.

    Returns (missing, extra):
    - missing: edges in trace but not in normalizer (UNDER-conservative = BUG)
    - extra: edges in normalizer but not in trace (OVER-conservative = suboptimal)
    """
    missing = []  # trace has, normalizer doesn't
    extra = []    # normalizer has, trace doesn't

    for i in range(n):
        trace_set = trace_deps.get(i, set())
        norm_set = norm_deps.get(i, set())

        for dep in trace_set - norm_set:
            missing.append((dep, i))  # dep -> i edge missing from normalizer

        for dep in norm_set - trace_set:
            extra.append((dep, i))  # dep -> i edge extra in normalizer

    return missing, extra


def analyze_trace(trace_dir: str, verbose: bool = False) -> dict:
    """Analyze a single trace directory."""
    func_name = os.path.basename(trace_dir)
    trace_path = os.path.join(trace_dir, 'as1_R_trace.txt')
    o_path = os.path.join(trace_dir, 'as1.o')

    if not os.path.exists(trace_path):
        return {'func': func_name, 'error': 'no trace file'}

    # Parse trace
    bbs = parse_trace(trace_path)

    # Build disassembly lookup from .o file
    disasm_lookup = {}
    if os.path.exists(o_path):
        disasm_lookup = disassemble_o_file(o_path)

    results = {
        'func': func_name,
        'num_bbs': len(bbs),
        'total_missing': 0,
        'total_extra': 0,
        'total_edges_trace': 0,
        'total_edges_norm': 0,
        'bb_details': []
    }

    for bb_idx, bb in enumerate(bbs):
        node_nums = bb.instruction_order()
        n = len(node_nums)

        # Build instructions for normalizer
        instructions = []
        for idx, node_num in enumerate(node_nums):
            node = bb.nodes[node_num]
            text = get_disasm_text(node.inst_word, disasm_lookup)
            raw_bytes = struct.pack('<I', node.inst_word)
            insn = DependencyAnalyzer.parse_instruction(
                addr=idx * 4, text=text, idx=idx, raw_bytes=raw_bytes)
            insn.has_relocation = node.relocation != 0
            instructions.append(insn)

        # Build both dep graphs
        trace_deps = build_trace_dep_graph(bb)
        norm_deps = build_normalizer_dep_graph(instructions)

        # Count edges
        trace_edge_count = sum(len(s) for s in trace_deps.values())
        norm_edge_count = sum(len(s) for s in norm_deps.values())

        # Compare
        missing, extra = compare_dep_graphs(trace_deps, norm_deps, n)

        results['total_missing'] += len(missing)
        results['total_extra'] += len(extra)
        results['total_edges_trace'] += trace_edge_count
        results['total_edges_norm'] += norm_edge_count

        bb_detail = {
            'bb_idx': bb_idx,
            'n_insns': n,
            'trace_edges': trace_edge_count,
            'norm_edges': norm_edge_count,
            'missing': missing,
            'extra': extra,
            'instructions': [(nn, bb.nodes[nn].inst_word,
                             get_disasm_text(bb.nodes[nn].inst_word, disasm_lookup))
                            for nn in node_nums]
        }
        results['bb_details'].append(bb_detail)

        if verbose and (missing or extra):
            print(f"\n  BB{bb_idx} ({n} insns):")
            for idx, (nn, word, text) in enumerate(bb_detail['instructions']):
                print(f"    [{idx}] Node {nn}: {word:08x}  {text}")

            if missing:
                print(f"    MISSING (trace has, normalizer doesn't):")
                for src, dst in missing:
                    src_text = bb_detail['instructions'][src][2]
                    dst_text = bb_detail['instructions'][dst][2]
                    print(f"      [{src}] -> [{dst}]:  {src_text} -> {dst_text}")

            if extra:
                print(f"    EXTRA (normalizer has, trace doesn't):")
                for src, dst in extra:
                    src_text = bb_detail['instructions'][src][2]
                    dst_text = bb_detail['instructions'][dst][2]
                    print(f"      [{src}] -> [{dst}]:  {src_text} -> {dst_text}")

    return results


def main():
    parser = argparse.ArgumentParser(
        description='Validate normalizer deps against as1 -R traces')
    parser.add_argument('--trace-dir', default='.tmp_sched_traces',
                       help='Directory containing trace subdirectories')
    parser.add_argument('-v', '--verbose', action='store_true',
                       help='Show per-BB details for mismatches')
    parser.add_argument('--func', type=str, default=None,
                       help='Analyze only this function')
    args = parser.parse_args()

    trace_base = args.trace_dir
    if not os.path.isabs(trace_base):
        trace_base = os.path.join(os.getcwd(), trace_base)

    if not os.path.isdir(trace_base):
        print(f"Error: trace directory not found: {trace_base}")
        sys.exit(1)

    # Find all trace directories
    trace_dirs = sorted(glob.glob(os.path.join(trace_base, '*')))
    trace_dirs = [d for d in trace_dirs if os.path.isdir(d)
                  and os.path.exists(os.path.join(d, 'as1_R_trace.txt'))]

    if args.func:
        trace_dirs = [d for d in trace_dirs if args.func in os.path.basename(d)]

    if not trace_dirs:
        print("No trace directories found")
        sys.exit(1)

    print(f"Validating normalizer deps against {len(trace_dirs)} as1 -R traces\n")

    total_missing = 0
    total_extra = 0
    total_trace_edges = 0
    total_norm_edges = 0
    total_bbs = 0

    for trace_dir in trace_dirs:
        func_name = os.path.basename(trace_dir)
        results = analyze_trace(trace_dir, verbose=args.verbose)

        if 'error' in results:
            print(f"  {func_name}: {results['error']}")
            continue

        status = "OK" if results['total_missing'] == 0 and results['total_extra'] == 0 else "MISMATCH"
        missing_str = f", MISSING={results['total_missing']}" if results['total_missing'] else ""
        extra_str = f", EXTRA={results['total_extra']}" if results['total_extra'] else ""

        print(f"  {func_name}: {results['num_bbs']} BBs, "
              f"trace_edges={results['total_edges_trace']}, "
              f"norm_edges={results['total_edges_norm']}"
              f"{missing_str}{extra_str} [{status}]")

        total_missing += results['total_missing']
        total_extra += results['total_extra']
        total_trace_edges += results['total_edges_trace']
        total_norm_edges += results['total_edges_norm']
        total_bbs += results['num_bbs']

    print(f"\n{'='*60}")
    print(f"Summary: {total_bbs} basic blocks across {len(trace_dirs)} functions")
    print(f"  Trace edges:      {total_trace_edges}")
    print(f"  Normalizer edges: {total_norm_edges}")
    print(f"  Missing (under-conservative): {total_missing}")
    print(f"  Extra (over-conservative):    {total_extra}")

    if total_missing == 0:
        print(f"\n  SAFE: Normalizer has ALL trace dependencies (no under-conservative gaps)")
    else:
        print(f"\n  WARNING: Normalizer MISSING {total_missing} trace edges!")
        print(f"  These are potential safety violations where the normalizer")
        print(f"  may allow reorderings that as1 would not.")

    if total_extra > 0:
        print(f"\n  NOTE: Normalizer has {total_extra} extra edges vs trace.")
        print(f"  These are over-conservative but safe — they prevent some")
        print(f"  valid reorderings that as1 would allow.")

    extra_pct = (total_extra / total_trace_edges * 100) if total_trace_edges > 0 else 0
    print(f"\n  Over-conservatism: {extra_pct:.1f}% extra edges relative to trace")


if __name__ == '__main__':
    main()
