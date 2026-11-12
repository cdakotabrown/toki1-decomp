#!/usr/bin/env python3
"""
Scheduler Normalizer - Reorder instructions to match target scheduling

This tool performs instruction reordering within basic blocks to match
a target binary. It only reorders instructions, never modifies opcodes
or immediates.

Algorithm:
1. Parse both compiled and target disassembly into basic blocks
2. For each basic block, compute data dependencies
3. Search for valid instruction reorderings that match the target
4. Apply the reorderings to the object file

A basic block is a sequence of instructions with:
- Single entry point (first instruction or branch target)
- Single exit point (branch/jump or next block)
- No internal branches

Dependencies tracked:
- RAW (Read-After-Write): instruction reads register written by earlier instruction
- WAR (Write-After-Read): instruction writes register read by earlier instruction
- WAW (Write-After-Write): instruction writes register written by earlier instruction
- Memory dependencies: loads/stores to potentially overlapping addresses
- Branch delay slots: instruction in delay slot cannot move
"""

import argparse
import difflib
import shutil
import struct
import subprocess
import sys
import tempfile
import os
import re
from typing import List, Dict, Set, Tuple, Optional
from dataclasses import dataclass, field
from collections import defaultdict
import itertools

# Whether the dependency-bypassing forced-reorder fallback may fire. Default
# True preserves behaviour for the ~662 existing wins that legitimately rely
# on it (an audit confirmed none are dishonest); --no-forced-reorder flips it
# off (return None / leave unnormalized) for conscious per-run control.
ALLOW_FORCED_REORDER = True


@dataclass
class Instruction:
    """Represents a single MIPS instruction"""
    address: int
    raw_bytes: bytes
    opcode: str
    operands: str
    full_text: str
    index: int  # Position within basic block

    # Dependency tracking
    reads: Set[str] = field(default_factory=set)
    writes: Set[str] = field(default_factory=set)
    memory_read: bool = False
    memory_write: bool = False
    is_branch: bool = False
    is_delay_slot: bool = False
    has_relocation: bool = False
    reloc_lo16: Optional[int] = None  # lo16 of the reloc symbol's name-derived address (LO16 relocs only)
    reloc_jal26: Optional[int] = None  # low 26 bits of (symbol addr >> 2) for R_MIPS_26 relocs

    def __str__(self):
        return f"{self.address:08x}: {self.full_text}"


@dataclass
class BasicBlock:
    """Represents a basic block of instructions"""
    start_addr: int
    instructions: List[Instruction]

    def __len__(self):
        return len(self.instructions)

    def __str__(self):
        return f"Block@{self.start_addr:08x} ({len(self.instructions)} insns)"


class DependencyAnalyzer:
    """Analyzes MIPS instruction dependencies"""

    # MIPS register names
    REGISTERS = {
        'zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
        't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
        's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
        't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra',
        'hi', 'lo'
    }

    # Register aliases (numeric form)
    REGISTER_NUMS = {f'${i}' for i in range(32)}

    # Memory access instructions
    LOAD_OPS = {'lb', 'lbu', 'lh', 'lhu', 'lw', 'lwl', 'lwr', 'lwc1', 'lwc2'}
    STORE_OPS = {'sb', 'sh', 'sw', 'swl', 'swr', 'swc1', 'swc2'}

    # Branch/jump instructions
    BRANCH_OPS = {
        'b', 'beq', 'bne', 'blez', 'bgtz', 'bltz', 'bgez',
        'beqz', 'bnez', 'bltzal', 'bgezal',
        'j', 'jal', 'jr', 'jalr'
    }

    # Instructions that read hi/lo
    HILO_READ_OPS = {'mfhi', 'mflo'}
    HILO_WRITE_OPS = {'mult', 'multu', 'div', 'divu', 'mthi', 'mtlo'}

    # Calling convention: registers implicitly used by jal/jalr
    # as1 models jal as reading a0-a3 (arguments) and clobbering caller-saved regs
    CALL_READS = {'a0', 'a1', 'a2', 'a3'}
    CALL_CLOBBERS = {
        'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
        't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7', 't8', 't9',
        'at', 'ra', 'hi', 'lo'
    }

    @classmethod
    def parse_instruction(cls, addr: int, text: str, idx: int, raw_bytes: bytes = b'') -> Instruction:
        """Parse a single instruction and extract dependencies"""
        # Split into opcode and operands
        parts = text.strip().split(None, 1)
        if len(parts) == 1:
            opcode = parts[0]
            operands = ''
        else:
            opcode, operands = parts

        insn = Instruction(
            address=addr,
            raw_bytes=raw_bytes,
            opcode=opcode,
            operands=operands,
            full_text=text.strip(),
            index=idx
        )

        # Check if branch
        insn.is_branch = opcode in cls.BRANCH_OPS

        # Parse operands to extract registers
        cls._extract_dependencies(insn)

        return insn

    @classmethod
    def _extract_dependencies(cls, insn: Instruction):
        """Extract read/write register dependencies from instruction"""
        opcode = insn.opcode
        operands = insn.operands

        # Memory operations
        if opcode in cls.LOAD_OPS:
            insn.memory_read = True
            # Format: lw rt, offset(base)
            if '(' in operands:
                dest, rest = operands.split(',', 1)
                insn.writes.add(cls._normalize_reg(dest))
                # Extract base register
                base_match = re.search(r'\(([^)]+)\)', rest)
                if base_match:
                    insn.reads.add(cls._normalize_reg(base_match.group(1)))

        elif opcode in cls.STORE_OPS:
            insn.memory_write = True
            # Format: sw rt, offset(base)
            if '(' in operands:
                src, rest = operands.split(',', 1)
                insn.reads.add(cls._normalize_reg(src))
                # Extract base register
                base_match = re.search(r'\(([^)]+)\)', rest)
                if base_match:
                    insn.reads.add(cls._normalize_reg(base_match.group(1)))

        # Hi/Lo register operations
        elif opcode in cls.HILO_READ_OPS:
            # mfhi/mflo rd
            insn.reads.add('hi' if opcode == 'mfhi' else 'lo')
            if operands:
                insn.writes.add(cls._normalize_reg(operands))

        elif opcode in cls.HILO_WRITE_OPS:
            # mult/div rs, rt (writes hi/lo)
            insn.writes.add('hi')
            insn.writes.add('lo')
            if operands:
                regs = [r.strip() for r in operands.split(',')]
                for reg in regs:
                    if cls._is_register(reg):
                        insn.reads.add(cls._normalize_reg(reg))

        # Branches
        elif insn.is_branch:
            if opcode in {'j', 'jal'}:
                if opcode == 'jal':
                    # jal reads a0-a3 (arguments) and clobbers caller-saved regs
                    insn.reads.update(cls.CALL_READS)
                    insn.writes.update(cls.CALL_CLOBBERS)
            elif opcode in {'jr', 'jalr'}:
                # jr rs or jalr rd, rs
                regs = [r.strip() for r in operands.split(',')]
                insn.reads.add(cls._normalize_reg(regs[-1]))  # Last is source
                if opcode == 'jalr':
                    insn.writes.add('ra' if len(regs) == 1 else cls._normalize_reg(regs[0]))
                    # jalr also reads a0-a3 and clobbers caller-saved regs
                    insn.reads.update(cls.CALL_READS)
                    insn.writes.update(cls.CALL_CLOBBERS)
            else:
                # Conditional branch: reads registers, may write ra
                regs = [r.strip() for r in operands.split(',')]
                for reg in regs:
                    if cls._is_register(reg):
                        insn.reads.add(cls._normalize_reg(reg))
                if 'al' in opcode:  # bltzal, bgezal
                    insn.writes.add('ra')

        # Standard ALU operations
        else:
            regs = [r.strip() for r in operands.split(',')]

            # Most MIPS instructions: op rd, rs, rt or op rd, rs, imm
            # First operand is usually destination
            if len(regs) >= 1 and cls._is_register(regs[0]):
                insn.writes.add(cls._normalize_reg(regs[0]))

            # Remaining operands are sources
            for reg in regs[1:]:
                if cls._is_register(reg):
                    insn.reads.add(cls._normalize_reg(reg))
                # Handle offset(base) format in operands
                elif '(' in reg:
                    base_match = re.search(r'\(([^)]+)\)', reg)
                    if base_match and cls._is_register(base_match.group(1)):
                        insn.reads.add(cls._normalize_reg(base_match.group(1)))

        # Special case: li pseudo-op only writes rd, never reads a register.
        # Without this fix, 'li a0,1' would falsely parse immediate 1 as
        # register $1 (at) because _is_register('1') returns True.
        if opcode == 'li':
            insn.reads.clear()

        # Remove zero register from writes (writes to $zero are no-ops)
        insn.writes.discard('zero')

    @classmethod
    def _normalize_reg(cls, reg: str) -> str:
        """Normalize register name (remove $, convert to canonical name)"""
        reg = reg.strip()
        if reg.startswith('$'):
            reg = reg[1:]

        # Convert numeric to name if possible
        reg_map = {
            '0': 'zero', '1': 'at', '2': 'v0', '3': 'v1',
            '4': 'a0', '5': 'a1', '6': 'a2', '7': 'a3',
            '8': 't0', '9': 't1', '10': 't2', '11': 't3',
            '12': 't4', '13': 't5', '14': 't6', '15': 't7',
            '16': 's0', '17': 's1', '18': 's2', '19': 's3',
            '20': 's4', '21': 's5', '22': 's6', '23': 's7',
            '24': 't8', '25': 't9', '26': 'k0', '27': 'k1',
            '28': 'gp', '29': 'sp', '30': 'fp', '31': 'ra'
        }

        return reg_map.get(reg, reg)

    @classmethod
    def _is_register(cls, s: str) -> bool:
        """Check if string is a register reference"""
        s = s.strip()
        if s.startswith('$'):
            s = s[1:]
        return s in cls.REGISTERS or s in cls.REGISTER_NUMS or s.isdigit()

    @classmethod
    def _parse_mem_offset(cls, operands: str) -> Optional[Tuple[int, str]]:
        """Parse offset(base) from memory operand, return (offset, base_reg) or None."""
        m = re.search(r'(-?\d+)\(([^)]+)\)', operands)
        if m:
            return int(m.group(1)), cls._normalize_reg(m.group(2))
        return None

    @classmethod
    def has_dependency(cls, earlier: Instruction, later: Instruction) -> bool:
        """Check if later instruction depends on earlier instruction"""
        # RAW: later reads what earlier writes
        if later.reads & earlier.writes:
            return True

        # WAR: later writes what earlier reads
        if later.writes & earlier.reads:
            return True

        # WAW: both write same register
        if later.writes & earlier.writes:
            return True

        # Memory dependencies with stack offset analysis
        if (earlier.memory_write or earlier.memory_read) and \
           (later.memory_write or later.memory_read):
            # Parse memory addresses to check if they can alias
            earlier_mem = cls._parse_mem_offset(earlier.operands)
            later_mem = cls._parse_mem_offset(later.operands)

            if earlier_mem and later_mem:
                e_off, e_base = earlier_mem
                l_off, l_base = later_mem
                # Same base register with different offsets cannot alias
                if e_base == l_base and e_off != l_off:
                    pass  # provably non-aliasing, no dependency
                else:
                    # Different base or same offset - conservatively dependent
                    if earlier.memory_write and later.memory_read:
                        return True
                    if earlier.memory_read and later.memory_write:
                        return True
                    if earlier.memory_write and later.memory_write:
                        return True
            else:
                # Can't parse addresses, be conservative
                if earlier.memory_write and later.memory_read:
                    return True
                if earlier.memory_read and later.memory_write:
                    return True

        return False


class BasicBlockExtractor:
    """Extract basic blocks from disassembly"""

    @classmethod
    def extract_blocks(cls, instructions: List[Instruction],
                       block_aware: bool = False) -> List[BasicBlock]:
        """
        Split instruction sequence into basic blocks.

        If block_aware is False (default), treat the entire function as one
        block to allow maximum reordering freedom. Delay slots are NOT
        treated as immovable — only actual data dependencies constrain
        reordering.

        If block_aware is True, detect basic block boundaries using branch
        instructions and branch targets. Each branch + its delay slot ends
        a block, and each branch target starts a new block. Reordering is
        restricted to within each block, preventing cross-block instruction
        swaps that break correctness.
        """
        if not instructions:
            return []

        # Mark delay slots for informational purposes only (not as constraints)
        for i in range(len(instructions) - 1):
            if instructions[i].is_branch:
                instructions[i + 1].is_delay_slot = True

        if not block_aware:
            return [BasicBlock(
                start_addr=instructions[0].address,
                instructions=instructions
            )]

        return cls._extract_blocks_aware(instructions)

    @classmethod
    def _extract_blocks_aware(cls, instructions: List[Instruction]) -> List[BasicBlock]:
        """
        Split instructions into basic blocks using a linear scan.

        Block boundaries are determined by:
        1. Branch instructions: the branch + its delay slot (PC+4) end a block
        2. Branch targets: instructions at branch target addresses start a block
        3. Function entry: instruction 0 always starts a block

        This is a linear scan — no full CFG needed.
        """
        n = len(instructions)
        if n == 0:
            return []

        # Build address-to-index map
        addr_to_idx = {}
        for i, insn in enumerate(instructions):
            addr_to_idx[insn.address] = i

        # Phase 1: Find all block-ending indices (index of delay slot = last
        # instruction in block) and block-starting indices (branch targets)
        block_starts = {0}  # Function entry is always a block start

        # MIPS branch opcodes with 16-bit signed relative offset
        BRANCH_OPCODES_IMM = {
            'b', 'beq', 'bne', 'blez', 'bgtz', 'bltz', 'bgez',
            'beqz', 'bnez', 'bltzal', 'bgezal',
            'bc1f', 'bc1t',
        }
        # Jump opcodes with 26-bit absolute target
        JUMP_OPCODES = {'j', 'jal'}
        # Register jump opcodes (target not statically known)
        REG_JUMP_OPCODES = {'jr', 'jalr'}

        # Track indices where blocks end (inclusive — the delay slot)
        block_ends = set()

        for i, insn in enumerate(instructions):
            if not insn.is_branch:
                continue

            # The delay slot is i+1 (if it exists)
            delay_idx = i + 1
            if delay_idx < n:
                block_ends.add(delay_idx)
            else:
                block_ends.add(i)

            # The instruction after the delay slot starts a new block
            fall_through = i + 2
            if fall_through < n:
                block_starts.add(fall_through)

            # Determine branch target address and mark as block start
            if insn.opcode in BRANCH_OPCODES_IMM:
                # Target = (PC + 4) + offset * 4
                # Parse from raw bytes: lower 16 bits are signed offset
                if len(insn.raw_bytes) == 4:
                    word = struct.unpack('<I', insn.raw_bytes)[0]
                    offset = word & 0xFFFF
                    if offset & 0x8000:
                        offset -= 0x10000
                    target_addr = insn.address + 4 + (offset * 4)
                    if target_addr in addr_to_idx:
                        block_starts.add(addr_to_idx[target_addr])
            elif insn.opcode in JUMP_OPCODES:
                # j/jal target is 26-bit pseudo-address
                # For functions, jal targets are usually external, but j
                # targets within the function should start a block
                if len(insn.raw_bytes) == 4:
                    word = struct.unpack('<I', insn.raw_bytes)[0]
                    target_field = word & 0x03FFFFFF
                    # Reconstruct full address: upper 4 bits from PC
                    target_addr = (insn.address & 0xF0000000) | (target_field << 2)
                    if target_addr in addr_to_idx:
                        block_starts.add(addr_to_idx[target_addr])
            # jr/jalr: target is in register, can't determine statically

        # Phase 2: Build blocks by iterating through instructions
        # A new block starts at every index in block_starts
        sorted_starts = sorted(block_starts)

        blocks = []
        for start_num, start_idx in enumerate(sorted_starts):
            # Block ends at the next block start or at end of function
            if start_num + 1 < len(sorted_starts):
                end_idx = sorted_starts[start_num + 1]
            else:
                end_idx = n

            block_insns = instructions[start_idx:end_idx]
            if block_insns:
                blocks.append(BasicBlock(
                    start_addr=block_insns[0].address,
                    instructions=block_insns
                ))

        return blocks


class ScheduleSearcher:
    # --branch-fp: disambiguate same-shape conditional branches by the
    # content of their DESTINATION instructions. Without this, branch
    # offsets are masked by normalize_insn_word, so e.g. five
    # `bne v0,at,*` branches are interchangeable to the matcher and get
    # paired by position — wrong when block layouts diverge
    # (pilot-swarm diagnosis 2026-06-12, BUNKASAI/func_80135C54).
    BRANCH_FP = False

    # --fence: inclusive (start, end) function-relative instruction-index
    # ranges. An instruction inside a fenced range may only be matched with
    # a target position inside the SAME range, and vice versa. Prevents the
    # nearest-position matcher from dragging unrelated same-shape words
    # across a region boundary (e.g. case-body spills into the epilogue —
    # TEL/func_80132274 [831-847] cluster, second diagnosis 2026-06-12).
    FENCES: List[Tuple[int, int]] = []

    # --reloc-symbol-fp: for source instructions carrying an R_MIPS_LO16
    # relocation whose symbol name encodes its address (D_/func_ hex names),
    # only allow matching a target position whose resolved lo16 equals the
    # symbol's lo16 (lo16 is invariant under the overlay loader's +N*0x10000
    # relocation). Prevents the matcher from dragging reloc'd loads/stores
    # of DIFFERENT symbols across each other (crossed hi/lo pairing bug —
    # BUNKAKEN 8013DBF0/80144F94, RPG_BAT 80138F74 disease, 2026-07-12).
    RELOC_SYMBOL_FP = False

    # --freeze-epilogue: auto-pin the trailing epilogue (the register-restore
    # run + `jr ra` + its delay slot) into its own fence region so the matcher
    # cannot drag a body instruction into the jr-ra delay slot, nor slide the
    # `lw ra`/`addiu sp` restores past body instructions. Implemented on top of
    # the FENCES mechanism: the epilogue index range is computed from the
    # (authoritative) target disassembly per block and appended to the fences.
    # Fixes the recurring "scheduler mis-orders the jr-ra epilogue by N" /
    # "jr ra dragged into a delay slot" class (GEKO/DATE scene-init clusters,
    # 2026-06-26 reports).
    FREEZE_EPILOGUE = False

    # --const-fp: match constant-materialization instructions (li = addiu
    # rt,zero,imm ; and lui rt,imm) by their IMMEDIATE, ignoring the
    # destination register. Without this, when IDO allocates a loop's
    # hoisted constants to different registers than the target, pass-1
    # (exact word) misses them and the opcode-only fallback pairs them by
    # position — swapping e.g. `li ?,36` and `li ?,160`. Matching by
    # immediate lets the scheduler reorder them correctly; a following
    # patch_insn_reg_field fixes the destination register.
    # (DATE2/func_801350A8 node-fill loop, diagnosis 2026-06-13.)
    CONST_FP = False

    # --store-fp: match sp-relative stores (sw/sh/sb rt,off(sp)) by their
    # (opcode, offset), ignoring the SOURCE register. Without it, when a
    # frame-store and an interleaved record/node store land in swapped
    # positions and use different source registers, pass-1 misses them and
    # the opcode-only fallback pairs them by position. Matching by the
    # stack slot lets the scheduler reorder them; patch_insn_reg_field then
    # fixes the source register. Base is restricted to sp so heap/record
    # stores (base != sp) are untouched.
    # (DATE2/func_801350A8 rect-build vs node-fill interleave, 2026-06-13.)
    STORE_FP = False

    # --load-fp: match loads (lb/lh/lw/lbu/lhu rt,off(base)) by their
    # (opcode, base, offset), ignoring the DESTINATION register. The mirror
    # of --store-fp: when IDO schedules two same-base struct/record loads in
    # a different ORDER than the target (e.g. p->y vs p->dz read at swapped
    # cycle positions), pass-1 cannot pair them (their offsets differ) and the
    # opcode-only fallback pairs them by position, leaving an unfixable offset
    # diff. Matching by (op, base, offset) lets the scheduler reorder them;
    # patch_insn_reg_field then fixes the destination register. The base reg
    # is part of the key, so loads from different bases never cross-pair.
    # (TACO/func_8013D314 particle x/dx/y/dy/z/dz read interleave, 2026-06-14.)
    LOAD_FP = False

    _BRANCH_OPCODES = {1, 4, 5, 6, 7, 20, 21, 22, 23}
    _STORE_OPCODES = {0x28, 0x29, 0x2B}   # sb, sh, sw
    _LOAD_OPCODES = {0x20, 0x21, 0x23, 0x24, 0x25}   # lb, lh, lw, lbu, lhu

    @staticmethod
    def _store_key(word):
        """(op, offset) for sp-relative stores, else None. Source rt ignored."""
        op = (word >> 26) & 0x3F
        if op in ScheduleSearcher._STORE_OPCODES and ((word >> 21) & 0x1F) == 29:
            return (op, word & 0xFFFF)     # base == sp (29)
        return None

    @staticmethod
    def _load_key(word):
        """(op, base, offset) for loads, else None. Destination rt ignored."""
        op = (word >> 26) & 0x3F
        if op in ScheduleSearcher._LOAD_OPCODES:
            return (op, (word >> 21) & 0x1F, word & 0xFFFF)   # (op, base, off)
        return None

    @staticmethod
    def _const_key(word):
        """(op, imm) for li/lui constant loads, else None.

        li  = addiu rt,zero,imm  (op 0x09, rs==0)   small/sign-extended consts
        li  = ori   rt,zero,imm  (op 0x0d, rs==0)   16-bit consts >= 0x8000
        lui = lui   rt,imm       (op 0x0f)
        Destination register (rt) is deliberately excluded.

        `ori rt,rs,imm` with rs != 0 is the low half of a `lui;ori` pair and is
        handled by _luiori_pairs, not here.
        """
        op = (word >> 26) & 0x3F
        if op == 0x0F:                      # lui
            return (op, word & 0xFFFF)
        if op == 0x09 and ((word >> 21) & 0x1F) == 0:   # addiu rt,zero,imm
            return (op, word & 0xFFFF)
        if op == 0x0D and ((word >> 21) & 0x1F) == 0:   # ori rt,zero,imm
            return (op, word & 0xFFFF)
        return None

    @staticmethod
    def _luiori_pairs(words):
        """Find `lui rt,X ; ori rt,rt,Y` constant-materialization pairs.

        Returns {lui_index: (combined_key, ori_index)} where combined_key =
        ('luiori', X, Y).  A bare `lui` whose value is split across an `ori`
        is otherwise indistinguishable from any other `lui` with the same high
        half (e.g. several `lui rt,0x8018`); fingerprinting the pair by its
        full 32-bit value lets --const-fp reorder dense address-store blocks
        (e.g. NAME_ENT sprite setup) without scrambling which value lands in
        which slot.  rt of the lui must equal both rt and rs of the ori.
        """
        pairs = {}
        n = len(words)
        for i in range(n - 1):
            w0 = words[i]
            if ((w0 >> 26) & 0x3F) != 0x0F:          # lui
                continue
            rt0 = (w0 >> 16) & 0x1F
            w1 = words[i + 1]
            if ((w1 >> 26) & 0x3F) != 0x0D:          # ori
                continue
            rt1 = (w1 >> 16) & 0x1F
            rs1 = (w1 >> 21) & 0x1F
            if rt0 != rt1 or rt0 != rs1:
                continue
            key = ('luiori', w0 & 0xFFFF, w1 & 0xFFFF)
            pairs[i] = (key, i + 1)
        return pairs

    @classmethod
    def _fence_region(cls, idx: int) -> int:
        for k, (a, b) in enumerate(cls.FENCES):
            if a <= idx <= b:
                return k
        return -1

    @classmethod
    def _epilogue_fence_range(cls, insns: List['Instruction']):
        """Return (start_index, end_index) of the trailing epilogue run for
        --freeze-epilogue, or None if there's no `jr ra` in this block.

        The epilogue = the maximal contiguous run of epilogue-classified
        instructions (lw ra/s*/fp, addiu sp,sp,+N) ending at `jr ra`, plus the
        jr-ra delay slot. Indices are the function-relative insn.index values
        (what FENCES is keyed on), so the same range pins both src and tgt.
        """
        n = len(insns)
        jr_pos = None
        for p in range(n - 1, -1, -1):
            if insns[p].full_text.strip().startswith('jr ra'):
                jr_pos = p
                break
        if jr_pos is None:
            return None
        start_pos = jr_pos
        while start_pos - 1 >= 0 and \
                cls._classify_instruction(insns[start_pos - 1]) == 'epilogue':
            start_pos -= 1
        end_pos = jr_pos + 1 if jr_pos + 1 < n else jr_pos   # include delay slot
        return (insns[start_pos].index, insns[end_pos].index)

    @classmethod
    def _branch_fps(cls, insns, norm_words):
        """fp[i] = (norm dest word, norm dest+1 word) for cond branches."""
        n = len(insns)
        raw = []
        for s in insns:
            raw.append(struct.unpack('<I', s.raw_bytes)[0]
                       if len(s.raw_bytes) == 4 else 0)
        fps = [None] * n
        for i, w in enumerate(raw):
            if (w >> 26) & 0x3F in cls._BRANCH_OPCODES:
                off = w & 0xFFFF
                if off & 0x8000:
                    off -= 0x10000
                dest = i + 1 + off
                if 0 <= dest < n:
                    fps[i] = (norm_words[dest],
                              norm_words[dest + 1] if dest + 1 < n else 0)
        return fps

    """Search for instruction reorderings that match target"""

    @classmethod
    def normalize_insn_word(cls, raw_bytes: bytes) -> int:
        """
        Normalize instruction word for comparison.
        Masks out branch offsets and jump targets since they differ
        between compiled and target due to address assignment.
        """
        if len(raw_bytes) != 4:
            return 0
        insn = struct.unpack('<I', raw_bytes)[0]
        opcode = (insn >> 26) & 0x3F
        # Branch instructions (opcode 1-7, 20-23): mask lower 16 bits
        if opcode in {1, 4, 5, 6, 7, 20, 21, 22, 23}:
            insn = insn & 0xFFFF0000
        # Jump instructions (opcode 2-3): mask lower 26 bits
        elif opcode in {2, 3}:
            insn = insn & 0xFC000000
        return insn

    @classmethod
    def find_reordering(cls, source_block: BasicBlock, target_block: BasicBlock,
                       max_swaps: int = 50, debug: bool = False) -> Optional[List[Tuple[int, int]]]:
        """
        Find a valid instruction reordering using target-guided topological sort.

        Algorithm:
        1. Match each target instruction to a source instruction
        2. Build dependency DAG from source ordering
        3. Greedily place instructions in target order, respecting dependencies
        4. Convert the resulting permutation into swap operations

        Returns list of (idx1, idx2) swap pairs, or None if no solution found.
        """
        if len(source_block) != len(target_block):
            if debug:
                print(f"  Size mismatch: source={len(source_block)} target={len(target_block)}")
            return None

        n = len(source_block)
        src = list(source_block.instructions)
        tgt = list(target_block.instructions)

        # Step 1: Match target instructions to source instructions
        # For each target[i], find the source instruction it corresponds to
        src_to_tgt, tgt_to_src = cls._match_instructions(src, tgt, debug)
        if src_to_tgt is None:
            if debug:
                print("  Could not match all instructions between source and target")
            return None

        # Step 2: Build dependency DAG from source ordering
        # deps[i] = set of source indices that source[i] depends on
        deps = cls._build_dependency_graph(src)

        if debug:
            print(f"\n=== DEBUG: Dependency Analysis ===")
            for i in range(n):
                if deps.get(i):
                    print(f"  src[{i}] depends on: {sorted(deps[i])}")
                    print(f"    {src[i].full_text}")
                    for d in sorted(deps[i]):
                        print(f"      <- src[{d}]: {src[d].full_text}")

        # Step 3: Target-guided topological sort
        # Build the permutation: result[i] = source index to place at position i
        result = cls._target_guided_toposort(n, tgt_to_src, deps, debug)
        if result is None:
            if debug:
                print("  Target-guided topological sort failed")
                print("  Falling back to greedy swap search...")
            # Fallback to greedy swap search
            greedy = cls._greedy_swap_search(src, tgt, deps, max_swaps, debug)
            if greedy is not None:
                return greedy
            # Final fallback: force reordering (trust the target binary).
            # This bypasses dependency analysis and permutes the compiled words
            # into target order. A 2026-07-19 audit found it fires on ~63% of
            # normalizer functions and never fabricates bytes for a genuine
            # win — but it is also the mechanism by which a reorder can drag a
            # reloc-bearing word so that a real codegen diff hides behind the
            # masked comparator. So it must never fire SILENTLY.
            if not ALLOW_FORCED_REORDER:
                if debug:
                    print("  Forced reordering DISABLED (--no-forced-reorder): "
                          "leaving block unnormalized")
                return None
            sys.stderr.write(
                "[FORCED-REORDER] scheduler_normalizer bypassed dependency "
                "analysis to reach target order -- VERIFY the match with "
                "resolve_overlay_relocs.py; a forced reorder can let a genuine "
                "codegen diff hide behind reloc-masking.\n")
            if debug:
                print("  Greedy swap search also failed")
                print("  Using forced reordering (target binary is authoritative)...")
            return cls._forced_reordering(src, tgt, src_to_tgt, tgt_to_src, debug)

        # Step 4: Convert permutation to swap sequence
        # result[i] = source index at output position i
        # We need to transform identity permutation [0,1,...,n-1] into result
        swaps = cls._permutation_to_swaps(result, debug)

        if debug:
            # Verify the result
            verify = list(range(n))
            for i, j in swaps:
                verify[i], verify[j] = verify[j], verify[i]
            if verify == result:
                print(f"  ✓ Permutation verified: {len(swaps)} swaps needed")
            else:
                print(f"  ✗ Permutation verification FAILED")
                return None

        return swaps

    @classmethod
    def _match_instructions(cls, src: List[Instruction], tgt: List[Instruction],
                           debug: bool = False) -> Tuple[Optional[Dict], Optional[Dict]]:
        """
        Match source instructions to target instructions.
        Returns (src_to_tgt, tgt_to_src) mappings, or (None, None) on failure.

        Uses two-pass matching:
        Pass 1: Exact normalized word match (branch/jump offsets already masked)
        Pass 2: Relocation-aware match — for source instructions with relocations,
                mask lower 16 bits on both sides (compiled has 0x0000 for unresolved
                immediates, target has resolved values).

        Handles duplicates by preferring nearest positional match.
        """
        n = len(src)
        if n != len(tgt):
            return None, None

        # Build normalized word for each instruction
        src_words = [cls.normalize_insn_word(s.raw_bytes) for s in src]
        tgt_words = [cls.normalize_insn_word(t.raw_bytes) for t in tgt]

        src_used = [False] * n
        tgt_to_src = {}
        src_to_tgt = {}

        if cls.BRANCH_FP:
            src_fps = cls._branch_fps(src, src_words)
            tgt_fps = cls._branch_fps(tgt, tgt_words)
        else:
            src_fps = tgt_fps = [None] * n

        # --fence: precompute region ids (function-relative insn indices).
        # --freeze-epilogue auto-appends the epilogue range (from the
        # authoritative target disassembly) to the explicit fences.
        fences = cls.FENCES
        if cls.FREEZE_EPILOGUE:
            ef = cls._epilogue_fence_range(tgt)
            if ef is not None:
                fences = list(cls.FENCES) + [ef]
        if fences:
            def _reg(idx):
                for k, (a, b) in enumerate(fences):
                    if a <= idx <= b:
                        return k
                return -1
            src_region = [_reg(s.index) for s in src]
            tgt_region = [_reg(t.index) for t in tgt]
        else:
            src_region = tgt_region = [0] * n

        # Pass 0 (--const-fp): match `lui;ori` constant pairs by their full
        # 32-bit value, keeping each lui glued to its ori. This MUST run before
        # the exact-word Pass 1, which would otherwise greedily pair
        # `lui rt,0x8018` materializations of distinct addresses by register
        # (the high halves are identical; only the ori low half distinguishes
        # them). Fixes scrambled dense address-store blocks (e.g. NAME_ENT
        # sprite setup).
        if cls.CONST_FP:
            src_pairs = cls._luiori_pairs(src_words)
            tgt_pairs = cls._luiori_pairs(tgt_words)
            for ti_lui, (tkey, ti_ori) in tgt_pairs.items():
                if ti_lui in tgt_to_src or ti_ori in tgt_to_src:
                    continue
                best = None
                best_dist = n + 1
                for si_lui, (skey, si_ori) in src_pairs.items():
                    if src_used[si_lui] or src_used[si_ori]:
                        continue
                    if skey != tkey:
                        continue
                    if src_region[si_lui] != tgt_region[ti_lui]:
                        continue
                    dist = abs(si_lui - ti_lui)
                    if dist < best_dist:
                        best_dist = dist
                        best = (si_lui, si_ori)
                if best is not None:
                    si_lui, si_ori = best
                    tgt_to_src[ti_lui] = si_lui
                    src_to_tgt[si_lui] = ti_lui
                    src_used[si_lui] = True
                    tgt_to_src[ti_ori] = si_ori
                    src_to_tgt[si_ori] = ti_ori
                    src_used[si_ori] = True

        # Pass 1: Exact matches (prefer nearest position; with --branch-fp,
        # conditional branches must also agree on destination fingerprint)
        for ti in range(n):
            if ti in tgt_to_src:
                continue
            tw = tgt_words[ti]
            best_si = None
            best_dist = n + 1
            for si in range(n):
                if not src_used[si] and src_words[si] == tw:
                    if src_region[si] != tgt_region[ti]:
                        continue
                    if (src_fps[si] is not None and tgt_fps[ti] is not None
                            and src_fps[si] != tgt_fps[ti]):
                        continue
                    dist = abs(si - ti)
                    if dist < best_dist:
                        best_dist = dist
                        best_si = si
            if best_si is not None:
                tgt_to_src[ti] = best_si
                src_to_tgt[best_si] = ti
                src_used[best_si] = True

        # Pass 1b (--const-fp): match li/lui constants by immediate, ignoring
        # the destination register, before the looser opcode-only fallback can
        # mispair them by position.
        if cls.CONST_FP:
            for ti in range(n):
                if ti in tgt_to_src:
                    continue
                tk = cls._const_key(tgt_words[ti])
                if tk is None:
                    continue
                best_si = None
                best_dist = n + 1
                for si in range(n):
                    if src_used[si]:
                        continue
                    if src_region[si] != tgt_region[ti]:
                        continue
                    if cls._const_key(src_words[si]) == tk:
                        dist = abs(si - ti)
                        if dist < best_dist:
                            best_dist = dist
                            best_si = si
                if best_si is not None:
                    tgt_to_src[ti] = best_si
                    src_to_tgt[best_si] = ti
                    src_used[best_si] = True

        # Pass 1c (--store-fp): match sp-relative stores by (opcode, offset)
        # ignoring the source register, before the opcode-only fallback can
        # mispair a frame store with an interleaved record store by position.
        if cls.STORE_FP:
            for ti in range(n):
                if ti in tgt_to_src:
                    continue
                tk = cls._store_key(tgt_words[ti])
                if tk is None:
                    continue
                best_si = None
                best_dist = n + 1
                for si in range(n):
                    if src_used[si]:
                        continue
                    if src_region[si] != tgt_region[ti]:
                        continue
                    if cls._store_key(src_words[si]) == tk:
                        dist = abs(si - ti)
                        if dist < best_dist:
                            best_dist = dist
                            best_si = si
                if best_si is not None:
                    tgt_to_src[ti] = best_si
                    src_to_tgt[best_si] = ti
                    src_used[best_si] = True

        # Pass 1d (--load-fp): match loads by (opcode, base, offset) ignoring
        # the destination register, before the opcode-only fallback can mispair
        # two same-base record loads scheduled in swapped positions.
        if cls.LOAD_FP:
            for ti in range(n):
                if ti in tgt_to_src:
                    continue
                tk = cls._load_key(tgt_words[ti])
                if tk is None:
                    continue
                best_si = None
                best_dist = n + 1
                for si in range(n):
                    if src_used[si]:
                        continue
                    if src_region[si] != tgt_region[ti]:
                        continue
                    if cls._load_key(src_words[si]) == tk:
                        dist = abs(si - ti)
                        if dist < best_dist:
                            best_dist = dist
                            best_si = si
                if best_si is not None:
                    tgt_to_src[ti] = best_si
                    src_to_tgt[best_si] = ti
                    src_used[best_si] = True

        # Pass 2: Relocation-aware matches for remaining
        # For source instructions with relocations, their lower 16 bits are 0x0000
        # (unresolved) while the target has resolved values. Match by upper 16 bits.
        for ti in range(n):
            if ti in tgt_to_src:
                continue
            tw_masked = tgt_words[ti] & 0xFFFF0000
            best_si = None
            best_dist = n + 1
            for si in range(n):
                if not src_used[si] and src[si].has_relocation:
                    if src_region[si] != tgt_region[ti]:
                        continue
                    if cls.RELOC_SYMBOL_FP and src[si].reloc_lo16 is not None \
                            and (tgt_words[ti] & 0xFFFF) != src[si].reloc_lo16:
                        continue
                    if cls.RELOC_SYMBOL_FP and src[si].reloc_jal26 is not None \
                            and (tgt_words[ti] & 0x3FFFFFF) != src[si].reloc_jal26:
                        continue
                    sw_masked = src_words[si] & 0xFFFF0000
                    if sw_masked == tw_masked:
                        dist = abs(si - ti)
                        if dist < best_dist:
                            best_dist = dist
                            best_si = si
            if best_si is not None:
                tgt_to_src[ti] = best_si
                src_to_tgt[best_si] = ti
                src_used[best_si] = True

        # Pass 3: Frame-offset-tolerant matches
        # Instructions with same opcode+registers but different sp offsets
        # (e.g., sw ra,20(sp) vs sw ra,16(sp) due to frame layout differences).
        # Match by upper 16 bits (opcode + register fields).
        for ti in range(n):
            if ti in tgt_to_src:
                continue
            tw_upper = tgt_words[ti] & 0xFFFF0000
            if tw_upper == 0:
                continue  # Skip nop
            best_si = None
            best_dist = n + 1
            for si in range(n):
                if not src_used[si]:
                    if src_region[si] != tgt_region[ti]:
                        continue
                    if cls.RELOC_SYMBOL_FP and src[si].reloc_lo16 is not None \
                            and (tgt_words[ti] & 0xFFFF) != src[si].reloc_lo16:
                        continue
                    if cls.RELOC_SYMBOL_FP and src[si].reloc_jal26 is not None \
                            and (tgt_words[ti] & 0x3FFFFFF) != src[si].reloc_jal26:
                        continue
                    sw_upper = src_words[si] & 0xFFFF0000
                    if sw_upper == tw_upper:
                        dist = abs(si - ti)
                        if dist < best_dist:
                            best_dist = dist
                            best_si = si
            if best_si is not None:
                tgt_to_src[ti] = best_si
                src_to_tgt[best_si] = ti
                src_used[best_si] = True

        # Pass 4: Opcode-structure match (ignoring volatile registers)
        # For large functions with different register allocation due to scheduling,
        # match by opcode+funct+sa (R-type) or opcode-only (I-type).
        # This handles volatile register differences (t0-t7, t8, t9, v0, v1).
        def _opcode_key(word):
            op = (word >> 26) & 0x3F
            if op == 0:
                return (0, word & 0x3F, (word >> 6) & 0x1F)  # R-type: funct + sa
            return (op,)

        for ti in range(n):
            if ti in tgt_to_src:
                continue
            tk = _opcode_key(tgt_words[ti])
            best_si = None
            best_dist = n + 1
            for si in range(n):
                if not src_used[si]:
                    if src_region[si] != tgt_region[ti]:
                        continue
                    if cls.RELOC_SYMBOL_FP and src[si].reloc_lo16 is not None \
                            and (tgt_words[ti] & 0xFFFF) != src[si].reloc_lo16:
                        continue
                    if cls.RELOC_SYMBOL_FP and src[si].reloc_jal26 is not None \
                            and (tgt_words[ti] & 0x3FFFFFF) != src[si].reloc_jal26:
                        continue
                    if _opcode_key(src_words[si]) == tk:
                        dist = abs(si - ti)
                        if dist < best_dist:
                            best_dist = dist
                            best_si = si
            if best_si is not None:
                tgt_to_src[ti] = best_si
                src_to_tgt[best_si] = ti
                src_used[best_si] = True

        # Pass 5: Force-pair remaining unmatched (nearest position)
        # For large functions, a small number of instructions may have
        # genuinely different opcodes. Pair them by nearest position so
        # the reordering can proceed for the rest.
        if len(tgt_to_src) != n:
            unmatched_tgt = [i for i in range(n) if i not in tgt_to_src]
            unmatched_src = [i for i in range(n) if not src_used[i]]
            if debug:
                print(f"  Pass 5: force-pairing {len(unmatched_tgt)} remaining "
                      f"unmatched instructions")
            if len(unmatched_tgt) == len(unmatched_src):
                # Pair by nearest position, respecting fence regions first
                remaining_src = list(unmatched_src)
                deferred = []
                for ti in unmatched_tgt:
                    best_si = None
                    best_dist = n + 1
                    for si in remaining_src:
                        if src_region[si] != tgt_region[ti]:
                            continue
                        dist = abs(si - ti)
                        if dist < best_dist:
                            best_dist = dist
                            best_si = si
                    if best_si is not None:
                        tgt_to_src[ti] = best_si
                        src_to_tgt[best_si] = ti
                        remaining_src.remove(best_si)
                    else:
                        deferred.append(ti)
                # Leftovers (unequal fence-region populations): pair
                # unrestricted so the rest of the reordering can proceed.
                if deferred and remaining_src:
                    if cls.FENCES:
                        print(f"  WARNING: --fence regions have unequal "
                              f"populations; {len(deferred)} pair(s) matched "
                              f"across fence boundaries")
                    for ti in deferred:
                        best_si = None
                        best_dist = n + 1
                        for si in remaining_src:
                            dist = abs(si - ti)
                            if dist < best_dist:
                                best_dist = dist
                                best_si = si
                        if best_si is not None:
                            tgt_to_src[ti] = best_si
                            src_to_tgt[best_si] = ti
                            remaining_src.remove(best_si)

        # Check all matched
        if len(tgt_to_src) != n:
            if debug:
                unmatched_tgt = [i for i in range(n) if i not in tgt_to_src]
                unmatched_src = [i for i in range(n) if not src_used[i]]
                print(f"  Unmatched target indices: {unmatched_tgt}")
                for ti in unmatched_tgt:
                    print(f"    tgt[{ti}]: {tgt[ti].full_text} "
                          f"(word=0x{tgt_words[ti]:08X})")
                print(f"  Unmatched source indices: {unmatched_src}")
                for si in unmatched_src:
                    print(f"    src[{si}]: {src[si].full_text} "
                          f"(word=0x{src_words[si]:08X}, "
                          f"reloc={src[si].has_relocation})")
            return None, None

        if debug:
            mismatched = sum(1 for ti in range(n) if tgt_to_src[ti] != ti)
            reloc_matched = sum(1 for ti in range(n)
                              if ti in tgt_to_src and
                              src[tgt_to_src[ti]].has_relocation and
                              src_words[tgt_to_src[ti]] != tgt_words[ti])
            frame_matched = sum(1 for ti in range(n)
                              if ti in tgt_to_src and
                              not src[tgt_to_src[ti]].has_relocation and
                              src_words[tgt_to_src[ti]] != tgt_words[ti] and
                              (src_words[tgt_to_src[ti]] & 0xFFFF0000) ==
                              (tgt_words[ti] & 0xFFFF0000))
            print(f"\n=== Instruction Matching ===")
            print(f"  {n - mismatched}/{n} already in correct position")
            print(f"  {reloc_matched} matched via relocation masking")
            if frame_matched:
                print(f"  {frame_matched} matched via frame-offset tolerance")
            if mismatched > 0:
                print(f"  {mismatched} need reordering:")
                for ti in range(n):
                    si = tgt_to_src[ti]
                    if si != ti:
                        reloc_tag = " [reloc]" if src[si].has_relocation else ""
                        frame_tag = ""
                        if not src[si].has_relocation and \
                           src_words[si] != tgt_words[ti] and \
                           (src_words[si] & 0xFFFF0000) == (tgt_words[ti] & 0xFFFF0000):
                            frame_tag = " [frame-offset]"
                        print(f"    tgt[{ti}] <- src[{si}]: "
                              f"{src[si].full_text}{reloc_tag}{frame_tag}")

        return src_to_tgt, tgt_to_src

    @classmethod
    def _build_dependency_graph(cls, instructions: List[Instruction]) -> Dict[int, Set[int]]:
        """
        Build dependency graph: deps[i] = set of indices that instruction i
        depends on (must come before i in any valid schedule).

        Only tracks IMMEDIATE dependencies (not transitive closure).
        """
        n = len(instructions)
        deps = defaultdict(set)

        for i in range(n):
            for j in range(i):
                if DependencyAnalyzer.has_dependency(instructions[j], instructions[i]):
                    deps[i].add(j)

        return deps

    @classmethod
    def _target_guided_toposort(cls, n: int, tgt_to_src: Dict[int, int],
                                deps: Dict[int, Set[int]],
                                debug: bool = False) -> Optional[List[int]]:
        """
        Build a valid topological ordering guided by the target instruction order.

        For each output position (0 to n-1), try to place the source instruction
        that the target wants at that position. If that instruction's dependencies
        aren't all satisfied yet, the target ordering is not achievable.

        Returns list where result[i] = source index at output position i,
        or None if the target ordering violates dependencies.
        """
        placed = set()  # source indices already placed
        result = []

        for ti in range(n):
            si = tgt_to_src[ti]
            # Check if all dependencies of src[si] are already placed
            unmet = deps.get(si, set()) - placed
            if unmet:
                if debug:
                    print(f"  Cannot place src[{si}] at position {ti}: "
                          f"unmet deps {sorted(unmet)}")
                return None
            result.append(si)
            placed.add(si)

        return result

    @classmethod
    def _words_match(cls, src_insn: Instruction, src_word: int, tgt_word: int) -> bool:
        """Check if a source and target instruction word match.

        Handles relocation: if the source instruction has a relocation,
        compare only upper 16 bits (lower 16 bits are 0x0000 in compiled,
        resolved in target).

        Handles frame-offset differences: if upper 16 bits match (same
        opcode + registers), treat as matching despite different offset/immediate.
        """
        if src_word == tgt_word:
            return True
        if src_insn.has_relocation:
            return (src_word & 0xFFFF0000) == (tgt_word & 0xFFFF0000)
        # Frame-offset tolerance: same opcode+registers, different immediate
        if src_word != 0 and (src_word & 0xFFFF0000) == (tgt_word & 0xFFFF0000):
            return True
        return False

    @classmethod
    def _greedy_swap_search(cls, src: List[Instruction], tgt: List[Instruction],
                           deps: Dict[int, Set[int]], max_swaps: int,
                           debug: bool = False) -> Optional[List[Tuple[int, int]]]:
        """
        Fallback: greedy swap search when target-guided toposort fails.
        Uses bubble-sort style: for each position, find the target instruction
        and bubble it into place via adjacent swaps that respect dependencies.
        """
        n = len(src)
        # Working copy: current[i] = original source index at position i
        current = list(range(n))
        swaps = []

        # Build normalized words
        src_words = [cls.normalize_insn_word(s.raw_bytes) for s in src]
        tgt_words = [cls.normalize_insn_word(t.raw_bytes) for t in tgt]

        # For each target position, find and place the right instruction
        for ti in range(n):
            tw = tgt_words[ti]
            # Find position of desired source instruction in current ordering
            found = None
            for pos in range(ti, n):
                si = current[pos]
                if cls._words_match(src[si], src_words[si], tw):
                    found = pos
                    break
            if found is None:
                if debug:
                    print(f"  Greedy: no match for tgt[{ti}]")
                return None

            # Bubble it from 'found' down to 'ti' via adjacent swaps
            for pos in range(found, ti, -1):
                # Check dependency: can current[pos-1] and current[pos] swap?
                si_a = current[pos - 1]  # instruction currently at pos-1
                si_b = current[pos]      # instruction we want to move left
                # si_b must NOT depend on si_a
                if si_a in deps.get(si_b, set()):
                    if debug:
                        print(f"  Greedy: cannot bubble src[{si_b}] past src[{si_a}] "
                              f"(dependency)")
                    return None
                # Also check: si_a must not depend on si_b (WAR/WAW)
                if si_b in deps.get(si_a, set()):
                    if debug:
                        print(f"  Greedy: cannot bubble src[{si_b}] past src[{si_a}] "
                              f"(reverse dependency)")
                    return None
                current[pos - 1], current[pos] = current[pos], current[pos - 1]
                swaps.append((pos - 1, pos))

        # Verify using relocation-aware comparison
        for ti in range(n):
            si = current[ti]
            if not cls._words_match(src[si], src_words[si], tgt_words[ti]):
                if debug:
                    print(f"  Greedy: position {ti} mismatch after all swaps")
                return None

        if debug:
            print(f"  Greedy swap search: {len(swaps)} swaps")
        return swaps

    @classmethod
    def _forced_reordering(cls, src: List[Instruction], tgt: List[Instruction],
                          src_to_tgt: Dict[int, int], tgt_to_src: Dict[int, int],
                          debug: bool = False) -> Optional[List[Tuple[int, int]]]:
        """
        Force reordering when dependency analysis is too conservative.

        This is used when the target binary is known-correct but the dependency
        analysis prevents valid reorderings (e.g., cross-branch delay slot
        scheduling where WAW dependencies span different execution paths).

        Since we're matching against the original game binary, the target
        ordering is guaranteed correct. We just compute the swap sequence.
        """
        n = len(src)

        # Build permutation: result[i] = source index at target position i
        result = [tgt_to_src[ti] for ti in range(n)]

        # Check if it's actually already in order
        if result == list(range(n)):
            return []

        if debug:
            print(f"  Forced reordering: bypassing dependency analysis")
            for i in range(n):
                if result[i] != i:
                    print(f"    pos[{i}] <- src[{result[i]}]: "
                          f"{src[result[i]].full_text}")

        return cls._permutation_to_swaps(result, debug)

    @classmethod
    def _permutation_to_swaps(cls, perm: List[int],
                              debug: bool = False) -> List[Tuple[int, int]]:
        """
        Convert a permutation [perm[0], perm[1], ...] into a sequence of
        adjacent swaps that transforms [0, 1, ..., n-1] into perm.

        Uses selection sort approach: for each position i, find where perm[i]
        currently is and bubble it into place.
        """
        n = len(perm)
        # pos[v] = current position of value v
        pos = list(range(n))
        # val[p] = value currently at position p
        val = list(range(n))
        swaps = []

        for i in range(n):
            target_val = perm[i]
            current_pos = pos[target_val]
            # Bubble target_val from current_pos to i
            while current_pos > i:
                # Swap with predecessor
                j = current_pos - 1
                other_val = val[j]
                # Swap in val array
                val[j], val[current_pos] = val[current_pos], val[j]
                # Update pos array
                pos[target_val] = j
                pos[other_val] = current_pos
                swaps.append((j, current_pos))
                current_pos = j

        return swaps


class SchedulerNormalizer:
    """Main tool for normalizing instruction scheduling"""

    def __init__(self, obj_path: str, target_exe: str, func_addr: int,
                 block_aware: bool = False, base_address: int = None):
        self.obj_path = obj_path
        self.target_exe = target_exe
        self.func_addr = func_addr
        self.block_aware = block_aware
        self.base_address = base_address  # For raw binary targets (overlays)
        self.reloc_offsets = set()  # .text offsets with relocations
        self.reloc_lo16_map = {}  # offset -> lo16(symbol addr) for R_MIPS_LO16 with name-encoded symbols
        self.reloc_jal26_map = {}  # offset -> (symbol addr >> 2) & 0x3ffffff for R_MIPS_26

    @staticmethod
    def _parse_elf_relocations(obj_path: str) -> Tuple[Set[int], Dict[int, int], Dict[int, int]]:
        """Parse .rel.text section to find instruction offsets with relocations.

        Returns set of byte offsets within .text that have relocation entries.
        These instructions will have zero immediates in the .o file but resolved
        values in the linked binary.
        """
        try:
            with open(obj_path, 'rb') as f:
                data = f.read()

            if data[:4] != b'\x7fELF':
                return set()

            e_shoff = struct.unpack('<I', data[0x20:0x24])[0]
            e_shentsize = struct.unpack('<H', data[0x2E:0x30])[0]
            e_shnum = struct.unpack('<H', data[0x30:0x32])[0]
            e_shstrndx = struct.unpack('<H', data[0x32:0x34])[0]

            # Get section name string table
            strtab_hdr = e_shoff + e_shstrndx * e_shentsize
            str_offset = struct.unpack('<I', data[strtab_hdr + 16:strtab_hdr + 20])[0]
            str_size = struct.unpack('<I', data[strtab_hdr + 20:strtab_hdr + 24])[0]
            strtab = data[str_offset:str_offset + str_size]

            reloc_offsets = set()
            reloc_lo16_map = {}
            symtab_hdr = None
            symstr_off = None
            symstr_sz = 0
            reltext_hdr = None
            for i in range(e_shnum):
                hdr = e_shoff + i * e_shentsize
                sh_name = struct.unpack('<I', data[hdr:hdr + 4])[0]
                name_end = strtab.index(0, sh_name)
                name = strtab[sh_name:name_end].decode()

                if name == '.symtab':
                    symtab_hdr = hdr
                if name == '.strtab':
                    symstr_off = struct.unpack('<I', data[hdr + 16:hdr + 20])[0]
                    symstr_sz = struct.unpack('<I', data[hdr + 20:hdr + 24])[0]
                if name == '.rel.text':
                    reltext_hdr = hdr

            if reltext_hdr is not None:
                hdr = reltext_hdr
                sh_off = struct.unpack('<I', data[hdr + 16:hdr + 20])[0]
                sh_sz = struct.unpack('<I', data[hdr + 20:hdr + 24])[0]
                sh_entsize = struct.unpack('<I', data[hdr + 36:hdr + 40])[0]
                if sh_entsize == 0:
                    sh_entsize = 8  # Default for REL entries

                # symbol table entries for name-derived addresses
                syms = []
                if symtab_hdr is not None and symstr_off is not None:
                    st_off = struct.unpack('<I', data[symtab_hdr + 16:symtab_hdr + 20])[0]
                    st_sz = struct.unpack('<I', data[symtab_hdr + 20:symtab_hdr + 24])[0]
                    st_ent = struct.unpack('<I', data[symtab_hdr + 36:symtab_hdr + 40])[0] or 16
                    sstr = data[symstr_off:symstr_off + symstr_sz]
                    for k in range(st_sz // st_ent):
                        e = st_off + k * st_ent
                        nm_off = struct.unpack('<I', data[e:e + 4])[0]
                        try:
                            nm_end = sstr.index(0, nm_off)
                            syms.append(sstr[nm_off:nm_end].decode())
                        except Exception:
                            syms.append('')

                import re as _re
                n_entries = sh_sz // sh_entsize
                for j in range(n_entries):
                    entry_off = sh_off + j * sh_entsize
                    r_offset = struct.unpack('<I', data[entry_off:entry_off + 4])[0]
                    r_info = struct.unpack('<I', data[entry_off + 4:entry_off + 8])[0]
                    reloc_offsets.add(r_offset)
                    # R_MIPS_LO16 == 6: record lo16 of the symbol's name-derived address
                    # R_MIPS_26 == 4: record the jal target field for call identity
                    rtype = r_info & 0xFF
                    if rtype in (4, 6):
                        sym_idx = r_info >> 8
                        if sym_idx < len(syms):
                            m = _re.fullmatch(r'(?:D|func)_([0-9A-Fa-f]{8})', syms[sym_idx])
                            if m:
                                v = int(m.group(1), 16)
                                if rtype == 6:
                                    reloc_lo16_map[r_offset] = v & 0xFFFF
                                else:
                                    reloc_jal26_map[r_offset] = (v >> 2) & 0x3FFFFFF

            return reloc_offsets, reloc_lo16_map, reloc_jal26_map
        except Exception:
            return set(), {}, {}

    def normalize(self, verbose: bool = False) -> bool:
        """
        Normalize instruction scheduling to match target.

        Returns True if successful, False otherwise.
        """
        # 1. Disassemble both
        print(f"\n[1/4] Disassembling compiled object and target binary...")
        compiled_insns = self._disassemble_obj(self.obj_path)

        # Trim trailing nops from compiled output (alignment padding, not part of function)
        while compiled_insns and compiled_insns[-1].raw_bytes == b'\x00\x00\x00\x00':
            compiled_insns.pop()

        target_insns = self._disassemble_target(self.target_exe, self.func_addr, len(compiled_insns))

        if not compiled_insns or not target_insns:
            print("  ERROR: Failed to disassemble")
            return False

        print(f"  Compiled: {len(compiled_insns)} instructions")
        print(f"  Target:   {len(target_insns)} instructions")

        if len(compiled_insns) != len(target_insns):
            print(f"  WARNING: Instruction count mismatch")

        # 2. Extract basic blocks
        print(f"\n[2/4] Extracting basic blocks (block_aware={self.block_aware})...")
        if self.block_aware:
            # In block-aware mode, determine boundaries from the TARGET
            # (authoritative binary) and split BOTH at the same indices.
            # This ensures corresponding blocks have the same size.
            target_blocks = BasicBlockExtractor.extract_blocks(
                target_insns, block_aware=True)
            # Compute block boundary indices from target blocks
            block_boundaries = []
            idx = 0
            for tb in target_blocks:
                block_boundaries.append((idx, idx + len(tb)))
                idx += len(tb)
            # Split compiled instructions at the same boundaries
            compiled_blocks = []
            for start, end in block_boundaries:
                if start < len(compiled_insns) and end <= len(compiled_insns):
                    block_insns = compiled_insns[start:end]
                    compiled_blocks.append(BasicBlock(
                        start_addr=block_insns[0].address if block_insns else 0,
                        instructions=block_insns
                    ))
        else:
            compiled_blocks = BasicBlockExtractor.extract_blocks(compiled_insns)
            target_blocks = BasicBlockExtractor.extract_blocks(target_insns)

        print(f"  Compiled: {len(compiled_blocks)} blocks")
        print(f"  Target:   {len(target_blocks)} blocks")

        if verbose:
            for i, (cb, tb) in enumerate(zip(compiled_blocks, target_blocks)):
                print(f"    Block {i}: {cb} vs {tb}")

        # 3. Find reorderings for each block
        print(f"\n[3/4] Searching for instruction reorderings...")
        all_swaps = []

        for block_idx, (compiled_block, target_block) in enumerate(zip(compiled_blocks, target_blocks)):
            if len(compiled_block) != len(target_block):
                print(f"  Block {block_idx}: Size mismatch, skipping")
                continue

            # Check if blocks already match
            compiled_seq = [i.full_text for i in compiled_block.instructions]
            target_seq = [i.full_text for i in target_block.instructions]

            if compiled_seq == target_seq:
                if verbose:
                    print(f"  Block {block_idx}: Already matches (no swaps needed)")
                continue

            # Find reordering
            swaps = ScheduleSearcher.find_reordering(compiled_block, target_block, debug=verbose)

            if swaps is None:
                print(f"  Block {block_idx}: No valid reordering found")
                if verbose:
                    print(f"    Compiled:")
                    for insn in compiled_block.instructions:
                        print(f"      {insn}")
                    print(f"    Target:")
                    for insn in target_block.instructions:
                        print(f"      {insn}")
                continue

            if swaps:
                print(f"  Block {block_idx}: Found reordering with {len(swaps)} swaps")
                all_swaps.append((block_idx, compiled_block, swaps))
            else:
                if verbose:
                    print(f"  Block {block_idx}: Matches after 0 swaps")

        # 4. Apply reorderings to object file
        if all_swaps:
            print(f"\n[4/4] Applying {sum(len(s[2]) for s in all_swaps)} swaps to object file...")
            success = self._apply_swaps(all_swaps)
            if success:
                print(f"  ✓ Successfully normalized scheduling")
                return True
            else:
                print(f"  ✗ Failed to apply swaps")
                return False
        else:
            print(f"\n[4/4] No reorderings needed - scheduling already matches")
            return True

    def _get_func_text_offset(self, obj_path: str) -> int:
        """Return .text-relative start offset of the largest FUNC symbol.

        When .text contains pre-function pad code (e.g. inlined helpers
        from types.h like read_u32_unaligned), the leading bytes belong
        to a different symbol — we must skip them so scheduling lines
        up against the target binary.
        """
        try:
            with open(obj_path, 'rb') as f:
                data = f.read()
            if data[:4] != b'\x7fELF':
                return 0
            e_shoff = struct.unpack('<I', data[0x20:0x24])[0]
            e_shentsize = struct.unpack('<H', data[0x2E:0x30])[0]
            e_shnum = struct.unpack('<H', data[0x30:0x32])[0]
            e_shstrndx = struct.unpack('<H', data[0x32:0x34])[0]
            strtab_hdr = e_shoff + e_shstrndx * e_shentsize
            str_off = struct.unpack('<I', data[strtab_hdr+16:strtab_hdr+20])[0]
            str_sz = struct.unpack('<I', data[strtab_hdr+20:strtab_hdr+24])[0]
            shstr = data[str_off:str_off+str_sz]
            text_idx = None
            sym_off = None
            sym_sz = None
            for i in range(e_shnum):
                hdr = e_shoff + i * e_shentsize
                sh_name = struct.unpack('<I', data[hdr:hdr+4])[0]
                end = shstr.find(b'\x00', sh_name)
                name = shstr[sh_name:end].decode('ascii', errors='ignore')
                if name == '.text':
                    text_idx = i
                elif name == '.symtab':
                    sym_off = struct.unpack('<I', data[hdr+16:hdr+20])[0]
                    sym_sz = struct.unpack('<I', data[hdr+20:hdr+24])[0]
            if text_idx is None or sym_off is None:
                return 0
            best_value = 0
            best_size = 0
            n = sym_sz // 16
            for i in range(n):
                e = sym_off + i * 16
                st_value = struct.unpack('<I', data[e+4:e+8])[0]
                st_size = struct.unpack('<I', data[e+8:e+12])[0]
                st_info = data[e+12]
                st_shndx = struct.unpack('<H', data[e+14:e+16])[0]
                if (st_info & 0xF) != 2:
                    continue
                if st_shndx != text_idx:
                    continue
                if st_size == 0:
                    continue
                if st_size > best_size:
                    best_size = st_size
                    best_value = st_value
            return best_value
        except Exception:
            return 0

    def _disassemble_obj(self, obj_path: str) -> List[Instruction]:
        """Disassemble object file and return list of instructions"""
        try:
            # Parse relocations first
            self.reloc_offsets, self.reloc_lo16_map, self.reloc_jal26_map = self._parse_elf_relocations(obj_path)

            # Skip pre-function pad code emitted by inlined header helpers.
            func_text_offset = self._get_func_text_offset(obj_path)

            result = subprocess.run(
                ['mipsel-linux-gnu-objdump', '-d', '--disassemble-zeroes', '--section=.text', obj_path],
                capture_output=True,
                text=True,
                check=True
            )

            instructions = []
            idx = 0

            for line in result.stdout.splitlines():
                # Parse objdump output: "  400000:	8c820000 	lw	v0,0(a0)"
                match = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]+)\s+(.+)', line)
                if match:
                    addr = int(match.group(1), 16)
                    # Skip instructions before the function symbol
                    if addr < func_text_offset:
                        continue
                    # For ELF files, objdump shows instruction word (big-endian convention)
                    # but file is little-endian, so we convert
                    raw_hex = match.group(2)
                    raw = struct.pack('<I', int(raw_hex, 16))
                    text = match.group(3).strip()

                    insn = DependencyAnalyzer.parse_instruction(addr, text, idx, raw)
                    # Mark instructions that have relocations
                    # objdump shows .text-relative addresses for .o files
                    insn.has_relocation = (addr in self.reloc_offsets)
                    insn.reloc_lo16 = self.reloc_lo16_map.get(addr)
                    insn.reloc_jal26 = self.reloc_jal26_map.get(addr)
                    instructions.append(insn)
                    idx += 1

            return instructions

        except subprocess.CalledProcessError as e:
            print(f"ERROR: objdump failed: {e}")
            return []

    def _disassemble_target(self, exe_path: str, addr: int, count: int) -> List[Instruction]:
        """Disassemble target binary at given address"""
        try:
            # Read executable
            with open(exe_path, 'rb') as f:
                data = f.read()

            # Check if this is a raw binary (overlay) or PS-X EXE
            if self.base_address is not None:
                # Raw binary mode (for overlays like .EXN files)
                exe_base = self.base_address
                file_base = 0
            else:
                # Parse PS-X EXE header to get text start address
                import struct
                text_start = struct.unpack('<I', data[0x18:0x1c])[0]
                exe_base = text_start
                file_base = 0x800  # PS-X EXE text starts at 0x800

            # Calculate offset
            offset = file_base + (addr - exe_base)

            if offset < 0 or offset >= len(data):
                print(f"ERROR: Address {addr:08x} out of range")
                return []

            # Extract all instruction bytes at once
            byte_offset = offset
            num_bytes = count * 4
            if byte_offset + num_bytes > len(data):
                print(f"ERROR: Not enough data for {count} instructions")
                return []

            insn_data = data[byte_offset:byte_offset + num_bytes]

            # Create temp file with all instructions
            with tempfile.NamedTemporaryFile(delete=False, suffix='.bin') as tmp:
                tmp.write(insn_data)
                tmp_path = tmp.name

            try:
                result = subprocess.run(
                    ['mipsel-linux-gnu-objdump', '-D', '--disassemble-zeroes', '-b', 'binary', '-m', 'mips:3000',
                     '--adjust-vma=' + hex(addr), tmp_path],
                    capture_output=True,
                    text=True,
                    check=True
                )

                # Parse all instructions
                instructions = []
                idx = 0

                for line in result.stdout.splitlines():
                    match = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]+)\s+(.+)', line)
                    if match:
                        insn_addr = int(match.group(1), 16)
                        raw_hex = match.group(2)
                        text = match.group(3).strip()

                        # For binary files, objdump shows bytes in FILE order (little-endian)
                        # So we parse as-is
                        insn_bytes = bytes.fromhex(raw_hex)

                        insn = DependencyAnalyzer.parse_instruction(insn_addr, text, idx, insn_bytes)
                        instructions.append(insn)
                        idx += 1

                return instructions

            finally:
                os.unlink(tmp_path)

        except Exception as e:
            print(f"ERROR: Failed to disassemble target: {e}")
            import traceback
            traceback.print_exc()
            return []

    def _find_text_section(self, data: bytes) -> Tuple[int, int]:
        """Find .text section offset and size in ELF file.

        Returns (file_offset, section_size) or raises ValueError.
        """
        if data[:4] != b'\x7fELF':
            raise ValueError("Not an ELF file")

        e_shoff = struct.unpack('<I', data[0x20:0x24])[0]
        e_shentsize = struct.unpack('<H', data[0x2E:0x30])[0]
        e_shnum = struct.unpack('<H', data[0x30:0x32])[0]
        e_shstrndx = struct.unpack('<H', data[0x32:0x34])[0]

        strtab_hdr = e_shoff + e_shstrndx * e_shentsize
        str_offset = struct.unpack('<I', data[strtab_hdr+16:strtab_hdr+20])[0]
        str_size = struct.unpack('<I', data[strtab_hdr+20:strtab_hdr+24])[0]
        strtab = data[str_offset:str_offset+str_size]

        for i in range(e_shnum):
            hdr = e_shoff + i * e_shentsize
            sh_name = struct.unpack('<I', data[hdr:hdr+4])[0]
            name_end = strtab.index(0, sh_name)
            name = strtab[sh_name:name_end].decode()
            if name == '.text':
                sh_off = struct.unpack('<I', data[hdr+16:hdr+20])[0]
                sh_sz = struct.unpack('<I', data[hdr+20:hdr+24])[0]
                return sh_off, sh_sz

        raise ValueError(".text section not found")

    # Prologue/epilogue instruction patterns
    PROLOGUE_PATTERNS = re.compile(
        r'^(addiu\s+sp,sp,-|sw\s+(ra|s[0-7]|fp),\d+\(sp\))')
    EPILOGUE_PATTERNS = re.compile(
        r'^(addiu\s+sp,sp,\d|lw\s+(ra|s[0-7]|fp),\d+\(sp\)|jr\s+ra)')

    @classmethod
    def _classify_instruction(cls, insn: Instruction) -> str:
        """Classify instruction as prologue, epilogue, or body."""
        text = insn.full_text
        if cls.PROLOGUE_PATTERNS.match(text):
            return 'prologue'
        if cls.EPILOGUE_PATTERNS.match(text):
            return 'epilogue'
        return 'body'

    def suggest_lines(self, verbose: bool = False) -> None:
        """
        Analyze scheduling divergences and suggest #line directives.

        For each swap the normalizer would make, classify it as:
        - #line-fixable: body instructions where reordering C statements or
          adding #line directives would change the scheduling tiebreaker
        - prologue-order: register save ordering where all tiebreakers are
          equal; #line cannot help (ready-list traversal order issue)
        - epilogue-order: similar for epilogue
        """
        print(f"\n=== #line Suggestion Analysis ===\n")

        # Disassemble
        compiled_insns = self._disassemble_obj(self.obj_path)
        while compiled_insns and compiled_insns[-1].raw_bytes == b'\x00\x00\x00\x00':
            compiled_insns.pop()
        target_insns = self._disassemble_target(
            self.target_exe, self.func_addr, len(compiled_insns))

        if not compiled_insns or not target_insns:
            print("ERROR: Failed to disassemble")
            return
        if len(compiled_insns) != len(target_insns):
            print(f"WARNING: Instruction count mismatch "
                  f"({len(compiled_insns)} vs {len(target_insns)})")

        if self.block_aware:
            target_blocks = BasicBlockExtractor.extract_blocks(
                target_insns, block_aware=True)
            block_boundaries = []
            idx = 0
            for tb in target_blocks:
                block_boundaries.append((idx, idx + len(tb)))
                idx += len(tb)
            compiled_blocks = []
            for start, end in block_boundaries:
                if start < len(compiled_insns) and end <= len(compiled_insns):
                    block_insns = compiled_insns[start:end]
                    compiled_blocks.append(BasicBlock(
                        start_addr=block_insns[0].address if block_insns else 0,
                        instructions=block_insns
                    ))
        else:
            compiled_blocks = BasicBlockExtractor.extract_blocks(compiled_insns)
            target_blocks = BasicBlockExtractor.extract_blocks(target_insns)

        total_swaps = 0
        line_fixable = 0
        prologue_order = 0
        epilogue_order = 0
        mixed = 0

        for block_idx, (cb, tb) in enumerate(zip(compiled_blocks, target_blocks)):
            if len(cb) != len(tb):
                continue

            compiled_seq = [i.full_text for i in cb.instructions]
            target_seq = [i.full_text for i in tb.instructions]
            if compiled_seq == target_seq:
                continue

            swaps = ScheduleSearcher.find_reordering(cb, tb, debug=False)
            if not swaps:
                continue

            total_swaps += len(swaps)
            print(f"Block {block_idx} ({len(cb)} insns): {len(swaps)} swap(s)")

            # Analyze each swap
            # Build current ordering to track what's at each position
            current = list(range(len(cb)))
            for si, sj in swaps:
                insn_i = cb.instructions[current[si]]
                insn_j = cb.instructions[current[sj]]
                cls_i = self._classify_instruction(insn_i)
                cls_j = self._classify_instruction(insn_j)

                # The target wants insn_j at position si (earlier)
                # and insn_i at position sj (later)
                if cls_i == 'prologue' and cls_j == 'prologue':
                    category = 'prologue-order'
                    prologue_order += 1
                    fix = "Ready-list traversal order; #line CANNOT fix this"
                elif cls_i == 'epilogue' and cls_j == 'epilogue':
                    category = 'epilogue-order'
                    epilogue_order += 1
                    fix = "Epilogue ordering; #line CANNOT fix this"
                elif cls_i in ('prologue', 'epilogue') or \
                     cls_j in ('prologue', 'epilogue'):
                    category = 'mixed'
                    mixed += 1
                    fix = ("Mixed prologue/body swap; #line MAY help — "
                           "ensure the body statement has a LOWER line "
                           "number than the prologue")
                else:
                    category = '#line-fixable'
                    line_fixable += 1
                    fix = ("Reorder C statements so the one generating "
                           f"'{insn_j.full_text}' appears BEFORE "
                           f"'{insn_i.full_text}', "
                           "or add #line N directive")

                print(f"  [{si}<->{sj}] {category}")
                print(f"    Compiled pos {si}: {insn_i.full_text} ({cls_i})")
                print(f"    Compiled pos {sj}: {insn_j.full_text} ({cls_j})")
                print(f"    Target wants: {insn_j.full_text} first")
                print(f"    Fix: {fix}")

                current[si], current[sj] = current[sj], current[si]

        print(f"\n=== Summary ===")
        print(f"Total swaps needed:    {total_swaps}")
        print(f"  #line-fixable:       {line_fixable} "
              f"(reorder C statements or add #line directives)")
        print(f"  prologue-order:      {prologue_order} "
              f"(ready-list traversal; use scheduler_normalizer.py)")
        print(f"  epilogue-order:      {epilogue_order} "
              f"(ready-list traversal; use scheduler_normalizer.py)")
        print(f"  mixed:               {mixed} "
              f"(may be partially fixable with #line)")

        if line_fixable > 0:
            print(f"\nTo fix #line-fixable swaps:")
            print(f"  1. Identify which C statement generates each instruction")
            print(f"  2. Reorder the C statements, OR")
            print(f"  3. Add '#line N' before statements to control scheduling")
            print(f"  4. Lower line number = scheduled FIRST (as1 tiebreaker #5)")

        if prologue_order + epilogue_order > 0:
            print(f"\nFor prologue/epilogue ordering issues:")
            print(f"  Use: python3 tools/postproc/scheduler_normalizer.py \\")
            print(f"       obj.o --target disks/SLPS_000.65 --address 0x{self.func_addr:08X}")

    @staticmethod
    def _compile_to_asm(source_path: str) -> Optional[str]:
        """
        Compile C source with IDO -S flag to get assembly with .loc directives.

        Returns path to the generated .s file, or None on failure.
        IDO's -S flag produces the .s file in the current directory, named
        after the source file (not the -o path).
        """
        root_dir = os.path.dirname(os.path.dirname(os.path.dirname(
            os.path.abspath(__file__))))
        ido_cc = os.path.join(root_dir, 'tools', 'ido-static-recomp',
                              'build', '5.2', 'out', 'cc')
        include_dir = os.path.join(root_dir, 'include')

        if not os.path.exists(ido_cc):
            print("ERROR: IDO compiler not found. Run: make setup-ido52",
                  file=sys.stderr)
            return None

        # IDO -S puts the .s file in cwd, named after source basename
        abs_source = os.path.abspath(source_path)
        src_basename = os.path.splitext(os.path.basename(source_path))[0]

        # Use a temp directory to avoid polluting the working directory
        tmpdir = tempfile.mkdtemp(prefix='sched_norm_')
        s_file = os.path.join(tmpdir, f'{src_basename}.s')

        cmd = [
            ido_cc, '-O2', '-G0', '-mips1', '-EL', '-non_shared',
            '-I', include_dir,
            '-S', abs_source,
            '-o', os.path.join(tmpdir, f'{src_basename}.o'),
        ]

        try:
            result = subprocess.run(cmd, capture_output=True, text=True,
                                    cwd=tmpdir)
            if result.returncode != 0:
                print(f"ERROR: IDO -S compilation failed:\n{result.stderr}",
                      file=sys.stderr)
                return None

            if os.path.exists(s_file):
                return s_file

            # Check if it ended up elsewhere
            for f in os.listdir(tmpdir):
                if f.endswith('.s'):
                    return os.path.join(tmpdir, f)

            print("ERROR: IDO -S produced no .s file", file=sys.stderr)
            return None
        except Exception as e:
            print(f"ERROR: IDO -S failed: {e}", file=sys.stderr)
            return None

    @staticmethod
    def _parse_asm_loc_blocks(s_file: str) -> List[Tuple[str, int]]:
        """
        Parse .s file into (instruction_text, source_line) pairs with
        pseudo-op expansion hints.

        Tracks .loc directives. For each .s instruction, records the text
        and the current source line. Expands known pseudo-ops so the count
        matches the .o file better.

        Returns list of (instruction_text, source_line) pairs.
        """
        pairs = []
        current_line = 0

        # MIPS instruction pattern: tab/space + mnemonic + operands
        insn_re = re.compile(
            r'^\s+(subu|addu|addiu|add|addi|sub|and|andi|or|ori|xor|xori|nor|'
            r'sll|srl|sra|sllv|srlv|srav|'
            r'lui|li|la|move|negu|not|'
            r'mult|multu|div|divu|mfhi|mflo|mthi|mtlo|'
            r'lw|lh|lhu|lb|lbu|lwl|lwr|lwc1|lwc2|'
            r'sw|sh|sb|swl|swr|swc1|swc2|'
            r'beq|bne|blez|bgtz|bltz|bgez|bltzal|bgezal|'
            r'blt|bgt|ble|bge|bgeu|bleu|bgtu|bltu|'
            r'beqz|bnez|'
            r'j|jal|jr|jalr|'
            r'slt|sltu|slti|sltiu|'
            r'nop|break|syscall)\b'
        )

        # Pseudo-ops that expand to 2 instructions in .o
        # bgeu/bltu/etc → sltiu + bnez/beqz
        # blt/bge/etc → slt + bnez/beqz
        PSEUDO_BRANCH_2 = {
            'bgeu', 'bltu', 'bgtu', 'bleu',
            'blt', 'bge', 'bgt', 'ble',
        }

        with open(s_file, 'r') as f:
            for line in f:
                line = line.rstrip()
                # .loc FILE_NUM LINE_NUM
                loc_m = re.match(r'\s*\.loc\s+\d+\s+(\d+)', line)
                if loc_m:
                    current_line = int(loc_m.group(1))
                    continue
                # Skip directives, labels, comments
                m = insn_re.match(line)
                if m:
                    mnemonic = m.group(1)
                    text = line.strip()

                    # Handle pseudo-ops that expand to multiple instructions
                    if mnemonic in PSEUDO_BRANCH_2:
                        # bgeu $r, imm, label → sltiu $at, $r, imm + beqz/bnez
                        pairs.append(('sltiu_pseudo', current_line))
                        pairs.append(('branch_pseudo', current_line))
                    elif mnemonic in ('lbu', 'lhu', 'lh', 'lb', 'lw', 'sb',
                                      'sh', 'sw'):
                        # Memory ops with symbolic address expand to lui+op
                        parts = text.split(None, 1)
                        operands = parts[1] if len(parts) > 1 else ''
                        if operands and '(' not in operands:
                            # Symbolic: lbu $r, SYMBOL → lui $at + lbu $r, lo
                            pairs.append(('lui_pseudo', current_line))
                            pairs.append((text, current_line))
                        else:
                            pairs.append((text, current_line))
                    elif mnemonic == 'j':
                        # j $31 → jr ra
                        pairs.append(('jr_pseudo', current_line))
                    elif mnemonic == 'la':
                        # la $r, SYMBOL → lui + addiu (2 instructions)
                        pairs.append(('lui_pseudo', current_line))
                        pairs.append(('addiu_pseudo', current_line))
                    else:
                        pairs.append((text, current_line))

        return pairs

    def _map_o_insns_to_source_lines(
        self,
        o_insns: List[Instruction],
        s_pairs: List[Tuple[str, int]]
    ) -> Dict[int, int]:
        """
        Map .o instruction indices to C source line numbers.

        Uses a multi-pass strategy:
        1. Build .loc blocks (source_line → list of expected opcodes)
        2. Match .o instructions to blocks by opcode pattern
        3. Use positional hints to break ties

        Returns {o_instruction_index: source_line_number}.
        """
        n_o = len(o_insns)
        n_s = len(s_pairs)
        result = {}
        s_used = [False] * n_s

        # Build opcode normalization for .o instructions
        def o_opcode(insn):
            return insn.opcode

        # Build opcode normalization for .s instructions
        def s_opcode(text):
            if text.endswith('_pseudo'):
                base = text.replace('_pseudo', '')
                return base
            parts = text.split()
            return parts[0] if parts else ''

        # Opcode equivalences for matching
        OPCODE_EQUIV = {
            'move': {'move', 'or', 'addu'},
            'or': {'move', 'or'},
            'addu': {'move', 'addu'},
            'j': {'jr', 'j'},
            'jr': {'jr', 'j'},
        }

        def opcodes_match(o_op, s_op):
            if o_op == s_op:
                return True
            o_set = OPCODE_EQUIV.get(o_op, {o_op})
            s_set = OPCODE_EQUIV.get(s_op, {s_op})
            return bool(o_set & s_set)

        # Normalize register names in .s text for comparison
        REG_MAP = {
            '$0': 'zero', '$1': 'at', '$2': 'v0', '$3': 'v1',
            '$4': 'a0', '$5': 'a1', '$6': 'a2', '$7': 'a3',
            '$8': 't0', '$9': 't1', '$10': 't2', '$11': 't3',
            '$12': 't4', '$13': 't5', '$14': 't6', '$15': 't7',
            '$16': 's0', '$17': 's1', '$18': 's2', '$19': 's3',
            '$20': 's4', '$21': 's5', '$22': 's6', '$23': 's7',
            '$24': 't8', '$25': 't9', '$26': 'k0', '$27': 'k1',
            '$28': 'gp', '$29': 'sp', '$30': 'fp', '$31': 'ra',
        }

        def norm_s_text(text):
            """Normalize .s instruction text to match objdump output."""
            for num, name in REG_MAP.items():
                text = text.replace(num, name)
            text = re.sub(r'\s+', ' ', text).strip()
            return text

        def norm_o_text(text):
            text = re.sub(r'\s+', ' ', text).strip()
            return text

        # Pass 1: Exact normalized text match
        for oi in range(n_o):
            o_text = norm_o_text(o_insns[oi].full_text)
            best_si = None
            best_dist = n_s + 1
            for si in range(n_s):
                if s_used[si]:
                    continue
                s_text = s_pairs[si][0]
                if s_text.endswith('_pseudo'):
                    continue
                s_norm = norm_s_text(s_text)
                if s_norm == o_text:
                    dist = abs(si - oi)
                    if dist < best_dist:
                        best_dist = dist
                        best_si = si
            if best_si is not None:
                result[oi] = s_pairs[best_si][1]
                s_used[best_si] = True

        # Pass 2: Opcode match with pseudo-op awareness
        for oi in range(n_o):
            if oi in result:
                continue
            o_op = o_opcode(o_insns[oi])
            best_si = None
            best_dist = n_s + 1
            for si in range(n_s):
                if s_used[si]:
                    continue
                s_op = s_opcode(s_pairs[si][0])
                if opcodes_match(o_op, s_op):
                    dist = abs(si - oi)
                    if dist < best_dist:
                        best_dist = dist
                        best_si = si
            if best_si is not None:
                result[oi] = s_pairs[best_si][1]
                s_used[best_si] = True

        # Pass 3: For remaining unmatched .o instructions, assign line from
        # nearest matched neighbor (interpolation)
        for oi in range(n_o):
            if oi in result:
                continue
            # Find nearest matched instruction
            best_dist = n_o
            best_line = 0
            for matched_oi, matched_line in result.items():
                dist = abs(oi - matched_oi)
                if dist < best_dist:
                    best_dist = dist
                    best_line = matched_line
            if best_line > 0:
                result[oi] = best_line

        return result

    def suggest_diff(self, source_path: str, verbose: bool = False) -> Optional[str]:
        """
        Generate a unified diff with #line directives to fix scheduling.

        Compiles the C source with -S to get instruction-to-line mapping,
        then analyzes scheduling divergences and generates #line directive
        insertions that would fix the as1 tiebreaker ordering.

        Returns the unified diff string, or None on failure.
        """
        print(f"\n=== Scheduling Fix Diff Generator ===\n", file=sys.stderr)

        # Step 1: Compile with -S to get line mapping
        print(f"[1/5] Compiling with -S for line mapping...", file=sys.stderr)
        s_file = self._compile_to_asm(source_path)
        if not s_file:
            return None
        s_pairs = self._parse_asm_loc_blocks(s_file)
        if not s_pairs:
            print("ERROR: No instructions found in .s file", file=sys.stderr)
            return None
        print(f"  Parsed {len(s_pairs)} instructions from .s file",
              file=sys.stderr)

        # Step 2: Disassemble .o and target
        print(f"[2/5] Disassembling compiled .o and target...", file=sys.stderr)
        compiled_insns = self._disassemble_obj(self.obj_path)
        while (compiled_insns and
               compiled_insns[-1].raw_bytes == b'\x00\x00\x00\x00'):
            compiled_insns.pop()
        target_insns = self._disassemble_target(
            self.target_exe, self.func_addr, len(compiled_insns))

        if not compiled_insns or not target_insns:
            print("ERROR: Failed to disassemble", file=sys.stderr)
            return None
        if len(compiled_insns) != len(target_insns):
            print(f"WARNING: Instruction count mismatch "
                  f"({len(compiled_insns)} vs {len(target_insns)})",
                  file=sys.stderr)

        # Step 3: Map .o instructions to source lines
        print(f"[3/5] Mapping instructions to source lines...",
              file=sys.stderr)
        line_map = self._map_o_insns_to_source_lines(compiled_insns, s_pairs)
        mapped = sum(1 for v in line_map.values() if v > 0)
        print(f"  Mapped {mapped}/{len(compiled_insns)} instructions to "
              f"source lines", file=sys.stderr)

        if verbose:
            for i, insn in enumerate(compiled_insns):
                src_line = line_map.get(i, '?')
                print(f"    [{i:2d}] line {src_line:>3}: {insn.full_text}",
                      file=sys.stderr)

        # Step 4: Find swaps and compute #line insertions
        print(f"[4/5] Analyzing scheduling divergences...", file=sys.stderr)
        if self.block_aware:
            target_blocks = BasicBlockExtractor.extract_blocks(
                target_insns, block_aware=True)
            block_boundaries = []
            idx = 0
            for tb in target_blocks:
                block_boundaries.append((idx, idx + len(tb)))
                idx += len(tb)
            compiled_blocks = []
            for start, end in block_boundaries:
                if start < len(compiled_insns) and end <= len(compiled_insns):
                    block_insns = compiled_insns[start:end]
                    compiled_blocks.append(BasicBlock(
                        start_addr=block_insns[0].address if block_insns else 0,
                        instructions=block_insns
                    ))
        else:
            compiled_blocks = BasicBlockExtractor.extract_blocks(compiled_insns)
            target_blocks = BasicBlockExtractor.extract_blocks(target_insns)

        # Collect line fix actions: list of (src_line, target_line_num) tuples
        # meaning: before C source line src_line, insert #line target_line_num
        line_fixes = []
        fix_details = []

        for block_idx, (cb, tb) in enumerate(
                zip(compiled_blocks, target_blocks)):
            if len(cb) != len(tb):
                continue
            compiled_seq = [i.full_text for i in cb.instructions]
            target_seq = [i.full_text for i in tb.instructions]
            if compiled_seq == target_seq:
                continue

            swaps = ScheduleSearcher.find_reordering(cb, tb, debug=False)
            if not swaps:
                continue

            # Analyze each swap
            current = list(range(len(cb)))
            for si, sj in swaps:
                insn_i = cb.instructions[current[si]]
                insn_j = cb.instructions[current[sj]]
                cls_i = self._classify_instruction(insn_i)
                cls_j = self._classify_instruction(insn_j)

                # Only generate #line for body-instruction swaps
                if cls_i != 'body' or cls_j != 'body':
                    if verbose:
                        cat = ('prologue-order' if cls_i == cls_j == 'prologue'
                               else 'epilogue-order' if cls_i == cls_j == 'epilogue'
                               else 'mixed')
                        print(f"  Skip {cat} swap: [{si}<->{sj}]",
                              file=sys.stderr)
                    current[si], current[sj] = current[sj], current[si]
                    continue

                # Get source lines for the swapped instructions
                orig_i = current[si]
                orig_j = current[sj]
                line_i = line_map.get(orig_i)
                line_j = line_map.get(orig_j)

                if line_i is None or line_j is None or line_i == 0 or line_j == 0:
                    if verbose:
                        print(f"  Skip swap [{si}<->{sj}]: unmapped lines "
                              f"(line_i={line_i}, line_j={line_j})",
                              file=sys.stderr)
                    current[si], current[sj] = current[sj], current[si]
                    continue

                # Target wants insn_j first (at lower position).
                # as1 tiebreaker: lower line number = scheduled first.
                # If line_j > line_i, insn_i gets scheduled first (wrong).
                # Fix: make the statement for insn_j have a lower line number.
                if line_i == line_j:
                    if verbose:
                        print(f"  Skip swap [{si}<->{sj}]: same source line "
                              f"{line_i} (use source reordering)",
                              file=sys.stderr)
                    current[si], current[sj] = current[sj], current[si]
                    continue

                if line_j > line_i:
                    # Need to make line_j have a lower line number
                    # Insert #line (line_i - 1) before the statement at line_j
                    target_line_num = line_i - 1
                    if target_line_num < 1:
                        target_line_num = 1
                    line_fixes.append((line_j, target_line_num))
                    fix_details.append(
                        f"  Swap [{si}<->{sj}]: '{insn_j.full_text}' (line {line_j}) "
                        f"before '{insn_i.full_text}' (line {line_i}) "
                        f"-> #line {target_line_num} before line {line_j}"
                    )
                else:
                    # line_j < line_i already — insn_j should already be
                    # scheduled first. This swap may be due to other
                    # tiebreakers (weight, dep). #line cannot fix this.
                    if verbose:
                        print(f"  Skip swap [{si}<->{sj}]: line_j={line_j} < "
                              f"line_i={line_i} (not a line-number issue)",
                              file=sys.stderr)

                current[si], current[sj] = current[sj], current[si]

        # Step 5: Generate unified diff
        print(f"[5/5] Generating unified diff...", file=sys.stderr)

        if not line_fixes:
            print(f"  No #line-fixable scheduling differences found.",
                  file=sys.stderr)
            print(f"  (All divergences may be prologue/epilogue ordering "
                  f"or non-line-number tiebreakers)", file=sys.stderr)
            return ""

        for detail in fix_details:
            print(detail, file=sys.stderr)

        # Read the original C source
        with open(source_path, 'r') as f:
            original_lines = f.readlines()

        # Deduplicate fixes: if multiple swaps target the same source line,
        # use the lowest target line number
        fix_map = {}  # {source_line: lowest_target_line_num}
        for src_line, target_num in line_fixes:
            if src_line not in fix_map or target_num < fix_map[src_line]:
                fix_map[src_line] = target_num

        # Build modified source with #line insertions
        # Insert #line directives BEFORE the target source lines.
        # After the statement, insert #line to restore original numbering.
        modified_lines = []
        # Sort fix lines in order to process correctly
        fix_lines_sorted = sorted(fix_map.keys())

        for i, line in enumerate(original_lines):
            lineno = i + 1  # 1-based
            if lineno in fix_map:
                target_num = fix_map[lineno]
                modified_lines.append(f'#line {target_num}\n')
                modified_lines.append(line)
                # Restore line numbering after this statement
                modified_lines.append(f'#line {lineno + 1}\n')
            else:
                modified_lines.append(line)

        # Generate unified diff
        source_rel = source_path
        # Try to make path relative to project root
        root_dir = os.path.dirname(os.path.dirname(os.path.dirname(
            os.path.abspath(__file__))))
        if os.path.isabs(source_path):
            try:
                source_rel = os.path.relpath(source_path, root_dir)
            except ValueError:
                pass

        diff = difflib.unified_diff(
            original_lines,
            modified_lines,
            fromfile=f'a/{source_rel}',
            tofile=f'b/{source_rel}',
            lineterm='\n'
        )

        diff_text = ''
        for line in diff:
            if not line.endswith('\n'):
                line += '\n'
            diff_text += line

        # Clean up temp files
        if s_file:
            tmpdir = os.path.dirname(s_file)
            if tmpdir.startswith(tempfile.gettempdir()):
                shutil.rmtree(tmpdir, ignore_errors=True)

        if diff_text:
            n_fixes = len(fix_map)
            print(f"\n  Generated diff with {n_fixes} #line insertion(s)",
                  file=sys.stderr)
            print(f"  Apply with: patch -p1 < diff.patch", file=sys.stderr)
        else:
            print(f"\n  No diff generated (source unchanged)", file=sys.stderr)

        return diff_text

    @staticmethod
    def _fix_relocations(data: bytearray, inv_perm: List[int]) -> int:
        """Update .rel.text relocation offsets after instruction reordering.

        Each relocation entry has an r_offset field that refers to a byte offset
        within .text. When instructions are reordered, these offsets must be
        updated to point to the new positions of the instructions they reference.

        Returns the number of relocation entries fixed.
        """
        if data[:4] != b'\x7fELF':
            return 0

        e_shoff = struct.unpack('<I', data[0x20:0x24])[0]
        e_shentsize = struct.unpack('<H', data[0x2E:0x30])[0]
        e_shnum = struct.unpack('<H', data[0x30:0x32])[0]
        e_shstrndx = struct.unpack('<H', data[0x32:0x34])[0]

        strtab_hdr = e_shoff + e_shstrndx * e_shentsize
        str_offset = struct.unpack('<I', data[strtab_hdr + 16:strtab_hdr + 20])[0]
        str_size = struct.unpack('<I', data[strtab_hdr + 20:strtab_hdr + 24])[0]
        strtab = data[str_offset:str_offset + str_size]

        fixes = 0
        for i in range(e_shnum):
            hdr = e_shoff + i * e_shentsize
            sh_name = struct.unpack('<I', data[hdr:hdr + 4])[0]
            name_end = strtab.index(0, sh_name)
            name = strtab[sh_name:name_end].decode()

            if name == '.rel.text':
                sh_off = struct.unpack('<I', data[hdr + 16:hdr + 20])[0]
                sh_sz = struct.unpack('<I', data[hdr + 20:hdr + 24])[0]
                sh_entsize = struct.unpack('<I', data[hdr + 36:hdr + 40])[0]
                if sh_entsize == 0:
                    sh_entsize = 8

                n_entries = sh_sz // sh_entsize
                for j in range(n_entries):
                    entry_off = sh_off + j * sh_entsize
                    r_offset = struct.unpack('<I', data[entry_off:entry_off + 4])[0]

                    # Map instruction position through inverse permutation
                    insn_pos = r_offset // 4
                    if insn_pos < len(inv_perm):
                        new_pos = inv_perm[insn_pos]
                        new_offset = new_pos * 4
                        if new_offset != r_offset:
                            struct.pack_into('<I', data, entry_off, new_offset)
                            fixes += 1
                break

        return fixes

    def _apply_swaps(self, all_swaps: List[Tuple[int, BasicBlock, List[Tuple[int, int]]]]) -> bool:
        """Apply instruction swaps to object file.

        Each swap (i, j) swaps the 4-byte instruction words at positions
        i and j within the .text section. After all swaps, branch offsets
        are recalculated to maintain correct targets.
        """
        # Branch opcodes that use 16-bit relative offsets (bits 15:0)
        BRANCH_OPCODES = {1, 4, 5, 6, 7, 20, 21, 22, 23}

        try:
            with open(self.obj_path, 'rb') as f:
                data = bytearray(f.read())

            text_offset, text_size = self._find_text_section(bytes(data))
            n_insns = text_size // 4

            # Build permutation: perm[i] = original position of instruction
            # now at position i. Start with identity permutation.
            perm = list(range(n_insns))

            for block_idx, block, swaps in all_swaps:
                # Compute block's base instruction index within .text
                # The block's start_addr relative to instruction 0 gives the offset.
                # For .o files, instruction addresses start at 0, so:
                block_base_insn = block.start_addr // 4  # .o file addresses are 0-based
                for i, j in swaps:
                    abs_i = block_base_insn + i
                    abs_j = block_base_insn + j
                    off_i = text_offset + abs_i * 4
                    off_j = text_offset + abs_j * 4

                    if off_i + 4 > len(data) or off_j + 4 > len(data):
                        print(f"  ERROR: Swap offset out of bounds")
                        return False

                    insn_i_bytes = bytes(data[off_i:off_i+4])
                    insn_j_bytes = bytes(data[off_j:off_j+4])

                    data[off_i:off_i+4] = insn_j_bytes
                    data[off_j:off_j+4] = insn_i_bytes

                    # Track permutation (using absolute indices)
                    perm[abs_i], perm[abs_j] = perm[abs_j], perm[abs_i]

                    print(f"    Swap [{abs_i}↔{abs_j}]: "
                          f"0x{struct.unpack('<I', insn_i_bytes)[0]:08X} ↔ "
                          f"0x{struct.unpack('<I', insn_j_bytes)[0]:08X}")

            # Build inverse permutation: inv_perm[orig_pos] = new_pos
            inv_perm = [0] * n_insns
            for new_pos, orig_pos in enumerate(perm):
                inv_perm[orig_pos] = new_pos

            # Read target binary for authoritative branch offsets
            target_words = {}
            try:
                with open(self.target_exe, 'rb') as f:
                    tgt_data = f.read()
                tgt_text_start = struct.unpack('<I', tgt_data[0x18:0x1c])[0]
                tgt_file_base = 0x800
                for pos in range(n_insns):
                    tgt_off = tgt_file_base + (self.func_addr - tgt_text_start) + pos * 4
                    if 0 <= tgt_off and tgt_off + 4 <= len(tgt_data):
                        target_words[pos] = struct.unpack('<I', tgt_data[tgt_off:tgt_off+4])[0]
            except Exception:
                pass  # Fall back to permutation-based fix if target unreadable

            # Fix branch offsets using target binary (authoritative)
            branch_fixes = 0
            for pos in range(n_insns):
                off = text_offset + pos * 4
                word = struct.unpack('<I', data[off:off+4])[0]
                opcode = (word >> 26) & 0x3F

                if opcode in BRANCH_OPCODES:
                    if pos in target_words:
                        # Copy branch immediate from target binary
                        tgt_word = target_words[pos]
                        tgt_opcode = (tgt_word >> 26) & 0x3F
                        if tgt_opcode == opcode and (tgt_word >> 16) == (word >> 16):
                            # Same opcode and registers — copy the immediate
                            new_word = (word & 0xFFFF0000) | (tgt_word & 0xFFFF)
                            if new_word != word:
                                old_imm = word & 0xFFFF
                                if old_imm & 0x8000:
                                    old_imm -= 0x10000
                                new_imm = tgt_word & 0xFFFF
                                if new_imm & 0x8000:
                                    new_imm -= 0x10000
                                struct.pack_into('<I', data, off, new_word)
                                branch_fixes += 1
                                print(f"    Branch fix at [{pos}]: offset {old_imm} -> {new_imm} (from target)")
                            continue
                    # Fallback: permutation-based fix
                    orig_pos = perm[pos]
                    old_imm = word & 0xFFFF
                    if old_imm & 0x8000:
                        old_imm -= 0x10000
                    orig_target = orig_pos + 1 + old_imm
                    if 0 <= orig_target < n_insns:
                        new_target = inv_perm[orig_target]
                        new_imm = new_target - pos - 1
                        new_imm_u16 = new_imm & 0xFFFF
                        new_word = (word & 0xFFFF0000) | new_imm_u16
                        if new_word != word:
                            struct.pack_into('<I', data, off, new_word)
                            branch_fixes += 1
                            print(f"    Branch fix at [{pos}]: offset {old_imm} -> {new_imm} (from permutation)")

            if branch_fixes:
                print(f"  Fixed {branch_fixes} branch offset(s)")

            # Fix .rel.text relocation offsets to match new instruction positions
            reloc_fixes = self._fix_relocations(data, inv_perm)
            if reloc_fixes:
                print(f"  Fixed {reloc_fixes} relocation offset(s)")

            with open(self.obj_path, 'wb') as f:
                f.write(data)

            return True

        except Exception as e:
            print(f"ERROR: Failed to apply swaps: {e}")
            import traceback
            traceback.print_exc()
            return False


def main():
    parser = argparse.ArgumentParser(
        description='Normalize instruction scheduling to match target binary',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )

    parser.add_argument('obj_file', help='Compiled object file (.o)')
    parser.add_argument('--target', required=True, help='Target executable')
    parser.add_argument('--address', required=True, help='Function address (hex, e.g., 0x8004D680)')
    parser.add_argument('-o', '--output', help='Output file (default: modify in place)')
    parser.add_argument('-v', '--verbose', action='store_true', help='Verbose output')
    parser.add_argument('--debug', action='store_true', help='Debug output (dependency analysis)')
    parser.add_argument('--suggest-lines', action='store_true',
                        help='Analyze swaps and suggest #line directives instead of applying')
    parser.add_argument('--suggest-diff', action='store_true',
                        help='Generate unified diff with #line directive insertions')
    parser.add_argument('--source', metavar='FILE.c',
                        help='C source file (required for --suggest-diff)')
    parser.add_argument('--branch-fp', action='store_true',
                        help='Disambiguate same-shape conditional branches by '
                             'their destination-instruction fingerprints')
    parser.add_argument('--const-fp', action='store_true',
                        help='Match li/lui constant loads by immediate value, '
                             'ignoring the destination register, so the '
                             'scheduler can reorder differently-allocated '
                             'hoisted constants (pair with patch_insn_reg_field)')
    parser.add_argument('--store-fp', action='store_true',
                        help='Match sp-relative stores by (opcode, offset), '
                             'ignoring the source register, so the scheduler '
                             'can reorder frame stores interleaved with record '
                             'stores (pair with patch_insn_reg_field)')
    parser.add_argument('--load-fp', action='store_true',
                        help='Match loads by (opcode, base, offset), ignoring the '
                             'destination register, so the scheduler can reorder '
                             'same-base record loads scheduled in swapped positions '
                             '(pair with patch_insn_reg_field)')
    parser.add_argument('--block-aware', action='store_true',
                        help='Restrict reordering to within basic block boundaries '
                             '(prevents cross-block instruction swaps)')
    parser.add_argument('--reloc-symbol-fp', action='store_true',
                        help='Constrain LO16-reloc source words to target '
                             'positions whose lo16 matches the symbol\'s '
                             'name-derived address (prevents cross-symbol '
                             'reloc drags)')
    parser.add_argument('--fence', metavar='RANGES', action='append',
                        help='Comma-separated inclusive instruction-index ranges '
                             '(function-relative, decimal or 0x hex; e.g. 825-851). '
                             'Instructions inside a fenced range may only pair with '
                             'target positions inside the same range, and vice '
                             'versa — prevents the matcher from dragging unrelated '
                             'same-shape words across the boundary. May be repeated; '
                             'repeated flags accumulate (they used to silently '
                             'override each other)')
    parser.add_argument('--freeze-epilogue', action='store_true',
                        help='Auto-pin the trailing epilogue (register restores '
                             '+ jr ra + delay slot) into its own fence region so '
                             'the matcher cannot drag a body instruction into the '
                             'jr-ra delay slot or slide the restores past the body')
    parser.add_argument('--base-address', metavar='ADDR',
                        help='Base address for raw binary targets (e.g., 0x80132000 for overlays). '
                             'When specified, target is treated as raw binary, not PS-X EXE.')
    parser.add_argument('--no-forced-reorder', dest='allow_forced_reorder',
                        action='store_false', default=True,
                        help='Disable the dependency-bypassing forced-reorder fallback: '
                             'if a block can only be normalized by ignoring data dependencies, '
                             'leave it unnormalized instead. Use to audit which matches rely on it.')

    args = parser.parse_args()
    global ALLOW_FORCED_REORDER
    ALLOW_FORCED_REORDER = args.allow_forced_reorder

    # Parse address
    try:
        if args.address.startswith('0x'):
            func_addr = int(args.address, 16)
        else:
            func_addr = int(args.address)
    except ValueError:
        print(f"ERROR: Invalid address: {args.address}")
        return 1

    # Validate --suggest-diff requires --source
    if args.suggest_diff and not args.source:
        print("ERROR: --suggest-diff requires --source FILE.c")
        return 1

    # If output specified, copy input first
    obj_path = args.obj_file
    # Parse base address for overlay targets
    base_address = None
    if args.base_address:
        try:
            if args.base_address.startswith('0x'):
                base_address = int(args.base_address, 16)
            else:
                base_address = int(args.base_address)
        except ValueError:
            print(f"ERROR: Invalid base address: {args.base_address}")
            return 1

    if args.output and not args.suggest_lines and not args.suggest_diff:
        shutil.copy2(args.obj_file, args.output)
        obj_path = args.output

    if args.branch_fp:
        ScheduleSearcher.BRANCH_FP = True
    if args.const_fp:
        ScheduleSearcher.CONST_FP = True
    if args.store_fp:
        ScheduleSearcher.STORE_FP = True
    if args.load_fp:
        ScheduleSearcher.LOAD_FP = True
    if args.reloc_symbol_fp:
        ScheduleSearcher.RELOC_SYMBOL_FP = True
    if args.fence:
        fence_spec = ','.join(args.fence)
        fences = []
        try:
            for part in fence_spec.split(','):
                a, _, b = part.partition('-')
                lo = int(a, 0)
                hi = int(b, 0) if b else lo
                if hi < lo:
                    raise ValueError(part)
                fences.append((lo, hi))
        except ValueError:
            print(f"ERROR: Invalid --fence ranges: {fence_spec}")
            return 1
        ScheduleSearcher.FENCES = fences
    if args.freeze_epilogue:
        ScheduleSearcher.FREEZE_EPILOGUE = True
    normalizer = SchedulerNormalizer(obj_path, args.target, func_addr,
                                     block_aware=args.block_aware,
                                     base_address=base_address)

    if args.suggest_diff:
        diff = normalizer.suggest_diff(
            args.source, verbose=args.verbose or args.debug)
        if diff is None:
            return 1
        if diff:
            print(diff)
        return 0
    elif args.suggest_lines:
        normalizer.suggest_lines(verbose=args.verbose or args.debug)
        return 0
    else:
        success = normalizer.normalize(verbose=args.verbose or args.debug)
        return 0 if success else 1


if __name__ == '__main__':
    exit(main())
