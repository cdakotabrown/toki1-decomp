#!/usr/bin/env python3
"""
Scheduling Diagnostics Helper

Parses as1 -R trace output and displays a compact per-node table showing
all scheduling priority fields. Highlights tiebreak points and assesses
whether divergences are fixable via #line, normalizer, or DAG shaping.

Usage:
    python3 tools/analysis/sched_diag.py <trace_file> [--target-exe EXE --address ADDR]

The trace file is the output of running as1 with the -R flag:
    $QEMU -L $IDO $IDO/usr/lib/as1 -R -EL -O2 -mips1 input.G -o output.o
"""

import argparse
import re
import struct
import sys
from typing import List, Dict, Optional, Tuple
from dataclasses import dataclass, field


# MIPS instruction disassembly (minimal, for display purposes)
OPCODES = {
    0x23: 'lw', 0x21: 'lh', 0x25: 'lhu', 0x20: 'lb', 0x24: 'lbu',
    0x2B: 'sw', 0x29: 'sh', 0x28: 'sb',
    0x0F: 'lui', 0x09: 'addiu', 0x0D: 'ori', 0x0C: 'andi',
    0x04: 'beq', 0x05: 'bne', 0x06: 'blez', 0x07: 'bgtz',
    0x02: 'j', 0x03: 'jal',
    0x0A: 'slti', 0x0B: 'sltiu',
}

REG_NAMES = [
    'zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
    't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
    's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
    't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra'
]


def disasm_word(word: int) -> str:
    """Minimal MIPS disassembly for display."""
    opcode = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    imm = word & 0xFFFF
    simm = imm if imm < 0x8000 else imm - 0x10000

    if word == 0:
        return 'nop'

    if opcode == 0:
        funct = word & 0x3F
        sa = (word >> 6) & 0x1F
        if funct == 0x08:
            return f'jr {REG_NAMES[rs]}'
        elif funct == 0x09:
            return f'jalr {REG_NAMES[rd]},{REG_NAMES[rs]}'
        elif funct == 0x25:
            return f'or {REG_NAMES[rd]},{REG_NAMES[rs]},{REG_NAMES[rt]}'
        elif funct == 0x21:
            if rt == 0:
                return f'move {REG_NAMES[rd]},{REG_NAMES[rs]}'
            return f'addu {REG_NAMES[rd]},{REG_NAMES[rs]},{REG_NAMES[rt]}'
        elif funct == 0x2A:
            return f'slt {REG_NAMES[rd]},{REG_NAMES[rs]},{REG_NAMES[rt]}'
        elif funct == 0x2B:
            return f'sltu {REG_NAMES[rd]},{REG_NAMES[rs]},{REG_NAMES[rt]}'
        elif funct == 0x00:
            return f'sll {REG_NAMES[rd]},{REG_NAMES[rt]},{sa}'
        elif funct == 0x03:
            return f'sra {REG_NAMES[rd]},{REG_NAMES[rt]},{sa}'
        elif funct == 0x02:
            return f'srl {REG_NAMES[rd]},{REG_NAMES[rt]},{sa}'
        elif funct == 0x18:
            return f'mult {REG_NAMES[rs]},{REG_NAMES[rt]}'
        elif funct == 0x1A:
            return f'div {REG_NAMES[rs]},{REG_NAMES[rt]}'
        elif funct == 0x10:
            return f'mfhi {REG_NAMES[rd]}'
        elif funct == 0x12:
            return f'mflo {REG_NAMES[rd]}'
        elif funct == 0x24:
            return f'and {REG_NAMES[rd]},{REG_NAMES[rs]},{REG_NAMES[rt]}'
        elif funct == 0x27:
            return f'nor {REG_NAMES[rd]},{REG_NAMES[rs]},{REG_NAMES[rt]}'
        elif funct == 0x26:
            return f'xor {REG_NAMES[rd]},{REG_NAMES[rs]},{REG_NAMES[rt]}'
        elif funct == 0x23:
            return f'subu {REG_NAMES[rd]},{REG_NAMES[rs]},{REG_NAMES[rt]}'
        return f'R-type funct=0x{funct:02x}'

    if opcode == 1:
        if rt == 0:
            return f'bltz {REG_NAMES[rs]},{simm}'
        elif rt == 1:
            return f'bgez {REG_NAMES[rs]},{simm}'
        elif rt == 16:
            return f'bltzal {REG_NAMES[rs]},{simm}'
        elif rt == 17:
            return f'bgezal {REG_NAMES[rs]},{simm}'

    name = OPCODES.get(opcode, f'op{opcode}')

    if opcode in (0x02, 0x03):
        target = (word & 0x03FFFFFF) << 2
        return f'{name} 0x{target:x}'
    elif opcode in (0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2B):
        return f'{name} {REG_NAMES[rt]},{simm}({REG_NAMES[rs]})'
    elif opcode == 0x0F:
        return f'lui {REG_NAMES[rt]},0x{imm:04x}'
    elif opcode == 0x09:
        if rs == 0:
            return f'li {REG_NAMES[rt]},{simm}'
        return f'addiu {REG_NAMES[rt]},{REG_NAMES[rs]},{simm}'
    elif opcode in (0x0D, 0x0C, 0x0E):
        names = {0x0D: 'ori', 0x0C: 'andi', 0x0E: 'xori'}
        return f'{names[opcode]} {REG_NAMES[rt]},{REG_NAMES[rs]},0x{imm:x}'
    elif opcode in (0x0A, 0x0B):
        names = {0x0A: 'slti', 0x0B: 'sltiu'}
        return f'{names[opcode]} {REG_NAMES[rt]},{REG_NAMES[rs]},{simm}'
    elif opcode in (0x04, 0x05, 0x06, 0x07):
        if opcode in (0x06, 0x07):
            return f'{name} {REG_NAMES[rs]},{simm}'
        return f'{name} {REG_NAMES[rs]},{REG_NAMES[rt]},{simm}'

    return f'{name} (0x{word:08x})'


@dataclass
class Node:
    """A scheduling node from as1 -R trace."""
    node_id: int
    inst_word: int
    relocation: int
    lineno: int
    before: int
    aftercycles: int
    maxhazard: int
    afternodes: List[Tuple[int, int]]  # (node_id, weight) pairs
    disasm: str = ''


@dataclass
class PickEvent:
    """A scheduling pick event."""
    node_id: int
    inst_num: int
    time: int
    offset: int
    candidates: List[Dict]  # [{node_id, inst_num, time, aftercycles, latency, besttime}]


@dataclass
class BasicBlockTrace:
    """Trace data for one basic block."""
    nodes: Dict[int, Node]
    initial_nodes: List[int]
    picks: List[PickEvent]


def parse_trace(trace_text: str) -> List[BasicBlockTrace]:
    """Parse as1 -R trace output into structured data."""
    blocks = []
    lines = trace_text.splitlines()
    i = 0

    while i < len(lines):
        line = lines[i].strip()

        # Look for node definitions
        if line.startswith('Node '):
            nodes = {}
            initial_nodes = []
            picks = []

            # Parse all nodes in this block
            while i < len(lines):
                line = lines[i].strip()
                m = re.match(r'Node\s+(\d+):\s+inst\s+([0-9a-f]+),\s+relocation\s+(\d+),\s+lineno\s+(\d+)', line)
                if not m:
                    break
                node_id = int(m.group(1))
                inst_word = int(m.group(2), 16)
                reloc = int(m.group(3))
                lineno = int(m.group(4))

                i += 1
                line2 = lines[i].strip() if i < len(lines) else ''
                m2 = re.match(r'before\s+(\d+),\s+aftercycles\s+(\d+),\s+maxhazard\s+(\d+)', line2)
                before = int(m2.group(1)) if m2 else 0
                aftercycles = int(m2.group(2)) if m2 else 0
                maxhazard = int(m2.group(3)) if m2 else 0

                i += 1
                line3 = lines[i].strip() if i < len(lines) else ''
                afternodes = []
                if line3.startswith('afternodes:'):
                    parts = line3[len('afternodes:'):].strip()
                    if parts:
                        for item in parts.split():
                            # Format: node_id/weight or node_id/weight/hazard
                            fields = item.split('/')
                            an_id = int(fields[0])
                            an_weight = int(fields[1]) if len(fields) > 1 else 1
                            afternodes.append((an_id, an_weight))

                node = Node(
                    node_id=node_id,
                    inst_word=inst_word,
                    relocation=reloc,
                    lineno=lineno,
                    before=before,
                    aftercycles=aftercycles,
                    maxhazard=maxhazard,
                    afternodes=afternodes,
                    disasm=disasm_word(inst_word)
                )
                nodes[node_id] = node
                i += 1

            # Parse initial nodes
            while i < len(lines):
                line = lines[i].strip()
                if line.startswith('Initial nodes:'):
                    parts = line[len('Initial nodes:'):].strip()
                    if parts:
                        initial_nodes = [int(x) for x in parts.split()]
                    i += 1
                    break
                i += 1

            # Parse pick events
            current_candidates = []
            while i < len(lines):
                line = lines[i].strip()
                if line.startswith('Node ') or (line.startswith('Initial nodes:') and not line.endswith(':')):
                    break
                if not line or line == 'Initial nodes:':
                    i += 1
                    continue

                # Candidate line: "  node 5 (INST 2), time = 0, aftercycles = 3, latency = 1, besttime = 0"
                cm = re.match(r'node\s+(\d+)\s+\(INST\s+(\d+)\),\s+time\s*=\s*(\d+),\s+aftercycles\s*=\s*(\d+),\s+latency\s*=\s*(\d+),\s+besttime\s*=\s*(\d+)', line)
                if cm:
                    current_candidates.append({
                        'node_id': int(cm.group(1)),
                        'inst_num': int(cm.group(2)),
                        'time': int(cm.group(3)),
                        'aftercycles': int(cm.group(4)),
                        'latency': int(cm.group(5)),
                        'besttime': int(cm.group(6)),
                    })
                    i += 1
                    continue

                # Pick line: "Picking node 5 (INST 2), at 0, best_addr = 0"
                pm = re.match(r'Picking\s+node\s+(\d+)\s+\(INST\s+(\d+)\),\s+at\s+([0-9a-fA-F]+),\s+best_addr\s*=\s*(\d+)', line)
                if pm:
                    picks.append(PickEvent(
                        node_id=int(pm.group(1)),
                        inst_num=int(pm.group(2)),
                        time=current_candidates[0]['time'] if current_candidates else 0,
                        offset=int(pm.group(3), 16),
                        candidates=list(current_candidates)
                    ))
                    current_candidates = []
                    i += 1
                    continue

                i += 1

            if nodes:
                blocks.append(BasicBlockTrace(
                    nodes=nodes,
                    initial_nodes=initial_nodes,
                    picks=picks
                ))
        else:
            i += 1

    return blocks


def analyze_tiebreaks(bb: BasicBlockTrace) -> List[Dict]:
    """Find tiebreak decisions and classify them."""
    tiebreaks = []
    for pick in bb.picks:
        if len(pick.candidates) < 2:
            continue

        # Check if multiple candidates have the max aftercycles
        max_ac = max(c['aftercycles'] for c in pick.candidates)
        tied = [c for c in pick.candidates if c['aftercycles'] == max_ac]

        if len(tied) >= 2:
            picked = pick.node_id
            picked_node = bb.nodes.get(picked)
            others = [c for c in tied if c['node_id'] != picked]

            # Determine what broke the tie
            if picked_node:
                picked_lineno = picked_node.lineno
                other_linenos = [bb.nodes[c['node_id']].lineno for c in others
                                if c['node_id'] in bb.nodes]

                # Check if lineno differs (off12-fixable)
                if other_linenos and any(ln != picked_lineno for ln in other_linenos):
                    fixability = '#line-fixable'
                else:
                    fixability = 'ready-list-order'
            else:
                fixability = 'unknown'

            tiebreaks.append({
                'time': pick.time,
                'picked': picked,
                'tied_nodes': [c['node_id'] for c in tied],
                'aftercycles': max_ac,
                'fixability': fixability,
                'n_tied': len(tied),
            })

    return tiebreaks


def print_node_table(bb: BasicBlockTrace, bb_idx: int):
    """Print compact per-node table for a basic block."""
    nodes = bb.nodes
    if not nodes:
        return

    print(f"\n{'='*72}")
    print(f"Basic Block {bb_idx} ({len(nodes)} nodes)")
    print(f"{'='*72}")

    # Header
    fmt = '{:>4s} | {:>5s} | {:>4s} | {:>6s} | {:>3s} | {:>5s} | {:>5s} | {:<28s}'
    print(fmt.format('Node', 'off12', 'bfr', 'weight', 'dep', 'off48', 'hazd', 'Instruction'))
    print('-'*72)

    # Sort by node ID descending (matches ugen output order)
    for nid in sorted(nodes.keys(), reverse=True):
        node = nodes[nid]
        # Compute dep: max weight from afternodes
        dep = max((w for _, w in node.afternodes), default=0)
        off48 = 0  # Not directly in trace, usually 0

        row = fmt.format(
            str(nid),
            str(node.lineno),
            str(node.before),
            str(node.aftercycles),
            str(dep),
            str(off48),
            str(node.maxhazard),
            node.disasm[:28]
        )
        print(row)

    # Print scheduling decisions
    if bb.picks:
        print(f"\nScheduling decisions:")
        print(f"{'Time':>4s} | {'Picked':>6s} | {'Candidates':>30s} | {'Decision'}")
        print('-'*72)
        for pick in bb.picks:
            if len(pick.candidates) < 2:
                candidates_str = '(only choice)'
                decision = ''
            else:
                max_ac = max(c['aftercycles'] for c in pick.candidates)
                tied = [c for c in pick.candidates if c['aftercycles'] == max_ac]
                candidates_str = ', '.join(
                    f"N{c['node_id']}(ac={c['aftercycles']})"
                    for c in pick.candidates
                )
                if len(candidates_str) > 30:
                    candidates_str = candidates_str[:27] + '...'

                if len(tied) == 1:
                    decision = 'clear winner (weight)'
                elif len(tied) >= 2:
                    picked_node = nodes.get(pick.node_id)
                    other_nodes = [nodes.get(c['node_id']) for c in tied
                                  if c['node_id'] != pick.node_id]
                    other_nodes = [n for n in other_nodes if n]

                    if picked_node and other_nodes:
                        if any(n.lineno != picked_node.lineno for n in other_nodes):
                            decision = f'TIEBREAK off12 ({len(tied)}-way)'
                        else:
                            decision = f'TIEBREAK list-order ({len(tied)}-way)'
                    else:
                        decision = f'TIEBREAK ({len(tied)}-way)'
                else:
                    decision = ''

            picked_node = nodes.get(pick.node_id)
            picked_str = f"N{pick.node_id}" + (f"({picked_node.disasm[:16]})" if picked_node else '')

            print(f"{pick.time:>4d} | {picked_str:>6s} | {candidates_str:>30s} | {decision}")

    # Analyze and print tiebreaks
    tiebreaks = analyze_tiebreaks(bb)
    if tiebreaks:
        print(f"\nTiebreak analysis:")
        for tb in tiebreaks:
            nodes_str = ', '.join(
                f"N{nid}(ln={nodes[nid].lineno})" if nid in nodes else f"N{nid}"
                for nid in tb['tied_nodes']
            )
            print(f"  time={tb['time']}: {tb['n_tied']}-way tie at ac={tb['aftercycles']}"
                  f" -> picked N{tb['picked']} | {tb['fixability']}")
            print(f"    candidates: {nodes_str}")


def compare_with_target(bb: BasicBlockTrace, target_bytes: bytes, base_addr: int):
    """Compare scheduled output with target binary."""
    n = len(bb.picks)
    if n == 0:
        return

    print(f"\nTarget comparison:")
    print(f"{'Pos':>3s} | {'Compiled':>10s} | {'Target':>10s} | {'Match'}")
    print('-'*42)

    mismatches = 0
    for i, pick in enumerate(bb.picks):
        node = bb.nodes.get(pick.node_id)
        if not node:
            continue
        compiled_word = node.inst_word
        if i * 4 + 4 <= len(target_bytes):
            target_word = struct.unpack('<I', target_bytes[i*4:i*4+4])[0]
            # Mask branch offsets and jump targets for comparison
            c_cmp = compiled_word
            t_cmp = target_word
            opcode = (compiled_word >> 26) & 0x3F
            if opcode in {1, 4, 5, 6, 7, 20, 21, 22, 23}:
                c_cmp &= 0xFFFF0000
                t_cmp &= 0xFFFF0000
            elif opcode in {2, 3}:
                c_cmp &= 0xFC000000
                t_cmp &= 0xFC000000

            match = 'OK' if c_cmp == t_cmp else 'DIFF'
            if c_cmp != t_cmp:
                # Check if only offset differs (frame-offset)
                if (compiled_word & 0xFFFF0000) == (target_word & 0xFFFF0000):
                    match = 'FRAME'
                mismatches += 1
            print(f"{i:>3d} | {disasm_word(compiled_word):>28s} | "
                  f"{disasm_word(target_word):>28s} | {match}")
        else:
            print(f"{i:>3d} | {disasm_word(compiled_word):>28s} | {'(no data)':>28s} |")

    if mismatches == 0:
        print(f"  -> PERFECT MATCH")
    else:
        print(f"  -> {mismatches}/{n} mismatches")


def main():
    parser = argparse.ArgumentParser(
        description='Scheduling diagnostics helper - parse as1 -R traces',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )
    parser.add_argument('trace_file', help='as1 -R trace output file')
    parser.add_argument('--target-exe', help='Target executable for comparison')
    parser.add_argument('--address', help='Function address (hex)')

    args = parser.parse_args()

    with open(args.trace_file, 'r') as f:
        trace_text = f.read()

    blocks = parse_trace(trace_text)

    if not blocks:
        print("No scheduling blocks found in trace.")
        return 1

    print(f"Parsed {len(blocks)} basic block(s)")

    # Load target bytes if provided
    target_bytes = None
    base_addr = 0
    if args.target_exe and args.address:
        try:
            addr = int(args.address, 16) if args.address.startswith('0x') else int(args.address)
            with open(args.target_exe, 'rb') as f:
                data = f.read()
            text_start = struct.unpack('<I', data[0x18:0x1c])[0]
            file_offset = 0x800 + (addr - text_start)
            # Read enough bytes for all blocks
            total_insns = sum(len(bb.picks) for bb in blocks)
            target_bytes = data[file_offset:file_offset + total_insns * 4]
            base_addr = addr
        except Exception as e:
            print(f"Warning: Could not load target: {e}")

    target_offset = 0
    for i, bb in enumerate(blocks):
        print_node_table(bb, i)
        if target_bytes:
            bb_size = len(bb.picks) * 4
            compare_with_target(bb, target_bytes[target_offset:target_offset + bb_size], base_addr + target_offset)
            target_offset += bb_size

    # Summary
    print(f"\n{'='*72}")
    print("SUMMARY")
    print(f"{'='*72}")
    total_tiebreaks = 0
    line_fixable = 0
    list_order = 0
    for bb in blocks:
        tbs = analyze_tiebreaks(bb)
        total_tiebreaks += len(tbs)
        for tb in tbs:
            if tb['fixability'] == '#line-fixable':
                line_fixable += 1
            elif tb['fixability'] == 'ready-list-order':
                list_order += 1

    print(f"  Total tiebreak decisions: {total_tiebreaks}")
    print(f"    #line-fixable:    {line_fixable}")
    print(f"    ready-list-order: {list_order}")

    if line_fixable > 0:
        print(f"\n  Recommendation: Use #line directives to fix {line_fixable} tiebreak(s)")
    if list_order > 0:
        print(f"  Recommendation: Use scheduler_normalizer.py for {list_order} list-order tiebreak(s)")
    if total_tiebreaks == 0:
        print(f"  No tiebreaks found (scheduling is deterministic)")

    return 0


if __name__ == '__main__':
    sys.exit(main())
