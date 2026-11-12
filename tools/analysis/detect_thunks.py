#!/usr/bin/env python3
"""
Thunk Pattern Detector for Tokimeki Memorial PS1

Detects "thunk" patterns where code BEFORE a function prologue sets up
t-registers (t6, t7, t8, etc.) that the function uses without initialization.

The thunk pattern:
1. Small code block (8-32 bytes) that loads from globals or pre-processes args
2. Sets up t-registers (t6, t7, t8, t1, etc.)
3. Falls through directly to function prologue (addiu sp, sp, -N)
4. Function uses t-registers without setting them

This is a calling convention optimization where:
- Callers call the THUNK address, not the function address
- The thunk sets up context registers before the function body

Usage:
    python3 tools/analysis/detect_thunks.py [--output build/thunks.json] [--verbose]
"""

import argparse
import json
import struct
import sys
from pathlib import Path
from dataclasses import dataclass, asdict
from typing import List, Optional, Tuple, Set

# MIPS instruction decoding helpers
def decode_instruction(word: int) -> dict:
    """Decode a MIPS instruction into components."""
    opcode = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    shamt = (word >> 6) & 0x1F
    funct = word & 0x3F
    imm = word & 0xFFFF
    imm_signed = imm if imm < 0x8000 else imm - 0x10000
    target = word & 0x3FFFFFF

    return {
        'word': word,
        'opcode': opcode,
        'rs': rs,
        'rt': rt,
        'rd': rd,
        'shamt': shamt,
        'funct': funct,
        'imm': imm,
        'imm_signed': imm_signed,
        'target': target
    }

def get_reg_name(reg: int) -> str:
    """Get register name from number."""
    names = [
        'zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
        't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
        's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
        't8', 't9', 'k0', 'k1', 'gp', 'sp', 's8', 'ra'
    ]
    return names[reg] if 0 <= reg < 32 else f'r{reg}'

def is_prologue(word: int) -> bool:
    """Check if instruction is a stack allocation (addiu sp, sp, -N)."""
    instr = decode_instruction(word)
    # addiu opcode = 9, rs = sp (29), rt = sp (29), negative immediate
    return (instr['opcode'] == 9 and
            instr['rs'] == 29 and
            instr['rt'] == 29 and
            instr['imm_signed'] < 0)

def is_jr_ra(word: int) -> bool:
    """Check if instruction is jr ra."""
    instr = decode_instruction(word)
    # jr is opcode=0, funct=8, rs=ra(31)
    return instr['opcode'] == 0 and instr['funct'] == 8 and instr['rs'] == 31

def is_nop(word: int) -> bool:
    """Check if instruction is a NOP."""
    return word == 0

def is_branch_or_jump(word: int) -> bool:
    """Check if instruction is a branch or jump."""
    instr = decode_instruction(word)
    opcode = instr['opcode']
    # J, JAL
    if opcode in [2, 3]:
        return True
    # Branch opcodes: BEQ, BNE, BLEZ, BGTZ, REGIMM (BLTZ, BGEZ, etc.)
    if opcode in [1, 4, 5, 6, 7]:
        return True
    # JR, JALR
    if opcode == 0 and instr['funct'] in [8, 9]:
        return True
    return False

def get_written_registers(word: int) -> Set[int]:
    """Get set of registers written by this instruction."""
    instr = decode_instruction(word)
    opcode = instr['opcode']
    funct = instr['funct']
    written = set()

    # R-type instructions (opcode 0)
    if opcode == 0:
        # Most R-type write to rd
        if funct not in [8, 9, 12, 13]:  # jr, jalr, syscall, break
            written.add(instr['rd'])
        if funct == 9:  # jalr writes rd (usually ra)
            written.add(instr['rd'])
    # I-type that write to rt
    elif opcode in [8, 9, 10, 11, 12, 13, 14, 15]:  # ADDI, ADDIU, SLTI, SLTIU, ANDI, ORI, XORI, LUI
        written.add(instr['rt'])
    # Load instructions write to rt
    elif opcode in [32, 33, 34, 35, 36, 37, 38]:  # LB, LH, LWL, LW, LBU, LHU, LWR
        written.add(instr['rt'])
    # JAL writes ra
    elif opcode == 3:
        written.add(31)  # ra

    return written

def get_read_registers(word: int) -> Set[int]:
    """Get set of registers read by this instruction."""
    instr = decode_instruction(word)
    opcode = instr['opcode']
    funct = instr['funct']
    read = set()

    # R-type instructions read rs and rt (usually)
    if opcode == 0:
        read.add(instr['rs'])
        if funct not in [0, 2, 3]:  # SLL, SRL, SRA use shamt, not rs
            pass  # Already added rs
        if funct in [0, 2, 3, 4, 6, 7, 24, 25, 26, 27]:  # Shift/mult/div
            read.add(instr['rt'])
        elif funct not in [8, 16, 17, 18, 19]:  # Not jr, mfhi, mflo, mthi, mtlo
            read.add(instr['rt'])
    # I-type reads rs
    elif opcode in [8, 9, 10, 11, 12, 13, 14]:  # ADDI through XORI
        read.add(instr['rs'])
    # LUI reads nothing extra
    elif opcode == 15:
        pass
    # Load instructions read rs (base)
    elif opcode in [32, 33, 34, 35, 36, 37, 38]:
        read.add(instr['rs'])
    # Store instructions read rs and rt
    elif opcode in [40, 41, 42, 43, 46]:  # SB, SH, SWL, SW, SWR
        read.add(instr['rs'])
        read.add(instr['rt'])
    # Branches read rs (and sometimes rt)
    elif opcode in [4, 5]:  # BEQ, BNE
        read.add(instr['rs'])
        read.add(instr['rt'])
    elif opcode in [1, 6, 7]:  # REGIMM, BLEZ, BGTZ
        read.add(instr['rs'])

    return read

def disassemble_instr(addr: int, word: int) -> str:
    """Simple disassembly for display."""
    instr = decode_instruction(word)
    opcode = instr['opcode']

    if word == 0:
        return "nop"

    if opcode == 0:  # R-type
        funct = instr['funct']
        rd = get_reg_name(instr['rd'])
        rs = get_reg_name(instr['rs'])
        rt = get_reg_name(instr['rt'])
        shamt = instr['shamt']

        if funct == 0:  # SLL
            return f"sll {rd}, {rt}, {shamt}"
        elif funct == 2:  # SRL
            return f"srl {rd}, {rt}, {shamt}"
        elif funct == 3:  # SRA
            return f"sra {rd}, {rt}, {shamt}"
        elif funct == 8:  # JR
            return f"jr {rs}"
        elif funct == 9:  # JALR
            return f"jalr {rd}, {rs}"
        elif funct == 33:  # ADDU
            return f"addu {rd}, {rs}, {rt}"
        elif funct == 37:  # OR
            if instr['rt'] == 0:
                return f"move {rd}, {rs}"
            return f"or {rd}, {rs}, {rt}"
        elif funct == 36:  # AND
            return f"and {rd}, {rs}, {rt}"
        elif funct == 42:  # SLT
            return f"slt {rd}, {rs}, {rt}"
        else:
            return f"r-type funct={funct}"

    elif opcode == 2:  # J
        target = (addr & 0xF0000000) | (instr['target'] << 2)
        return f"j 0x{target:08x}"

    elif opcode == 3:  # JAL
        target = (addr & 0xF0000000) | (instr['target'] << 2)
        return f"jal 0x{target:08x}"

    elif opcode == 4:  # BEQ
        rs = get_reg_name(instr['rs'])
        rt = get_reg_name(instr['rt'])
        offset = instr['imm_signed'] * 4 + 4
        target = addr + offset
        return f"beq {rs}, {rt}, 0x{target:08x}"

    elif opcode == 5:  # BNE
        rs = get_reg_name(instr['rs'])
        rt = get_reg_name(instr['rt'])
        offset = instr['imm_signed'] * 4 + 4
        target = addr + offset
        return f"bne {rs}, {rt}, 0x{target:08x}"

    elif opcode == 9:  # ADDIU
        rt = get_reg_name(instr['rt'])
        rs = get_reg_name(instr['rs'])
        imm = instr['imm_signed']
        return f"addiu {rt}, {rs}, {imm}"

    elif opcode == 12:  # ANDI
        rt = get_reg_name(instr['rt'])
        rs = get_reg_name(instr['rs'])
        imm = instr['imm']
        return f"andi {rt}, {rs}, 0x{imm:x}"

    elif opcode == 15:  # LUI
        rt = get_reg_name(instr['rt'])
        imm = instr['imm']
        return f"lui {rt}, 0x{imm:x}"

    elif opcode == 32:  # LB
        rt = get_reg_name(instr['rt'])
        rs = get_reg_name(instr['rs'])
        imm = instr['imm_signed']
        return f"lb {rt}, {imm}({rs})"

    elif opcode == 35:  # LW
        rt = get_reg_name(instr['rt'])
        rs = get_reg_name(instr['rs'])
        imm = instr['imm_signed']
        return f"lw {rt}, {imm}({rs})"

    elif opcode == 36:  # LBU
        rt = get_reg_name(instr['rt'])
        rs = get_reg_name(instr['rs'])
        imm = instr['imm_signed']
        return f"lbu {rt}, {imm}({rs})"

    elif opcode == 37:  # LHU
        rt = get_reg_name(instr['rt'])
        rs = get_reg_name(instr['rs'])
        imm = instr['imm_signed']
        return f"lhu {rt}, {imm}({rs})"

    elif opcode == 43:  # SW
        rt = get_reg_name(instr['rt'])
        rs = get_reg_name(instr['rs'])
        imm = instr['imm_signed']
        return f"sw {rt}, {imm}({rs})"

    else:
        return f"op={opcode} 0x{word:08x}"


@dataclass
class ThunkInfo:
    """Information about a detected thunk."""
    thunk_address: int
    function_address: int
    thunk_size: int
    thunk_instructions: int
    registers_set: List[str]
    globals_loaded: List[str]
    arg_preprocessing: bool
    thunk_disasm: List[str]
    first_func_instrs: List[str]
    registers_used_early: List[str]
    has_prologue_at_func: bool
    pattern_type: str  # 'global_load', 'arg_preprocess', 'combined', 'complex'


class ThunkDetector:
    def __init__(self, exe_path: str, verbose: bool = False):
        self.exe_path = exe_path
        self.verbose = verbose
        self.exe_data = None
        self.text_start = 0x80041000
        self.text_end = 0x800B6260
        self.file_offset = 0x800

    def load_exe(self):
        """Load the executable."""
        with open(self.exe_path, 'rb') as f:
            self.exe_data = f.read()

    def vram_to_offset(self, vram: int) -> int:
        """Convert VRAM address to file offset."""
        return vram - self.text_start + self.file_offset

    def read_word(self, vram: int) -> int:
        """Read a 32-bit word from VRAM address."""
        offset = self.vram_to_offset(vram)
        if 0 <= offset < len(self.exe_data) - 3:
            return struct.unpack('<I', self.exe_data[offset:offset+4])[0]
        return 0

    def find_all_prologues(self) -> List[int]:
        """Find all function prologues (addiu sp, sp, -N)."""
        prologues = []
        for addr in range(self.text_start, self.text_end, 4):
            word = self.read_word(addr)
            if is_prologue(word):
                prologues.append(addr)
        return prologues

    def analyze_pre_prologue(self, prologue_addr: int, max_instrs: int = 16) -> Optional[ThunkInfo]:
        """
        Analyze instructions BEFORE a prologue to detect thunk patterns.
        Returns ThunkInfo if a thunk is detected, None otherwise.
        """
        # Look at up to max_instrs instructions before the prologue
        thunk_instrs = []
        thunk_start = None
        registers_set = set()
        globals_loaded = []
        arg_preprocessing = False

        # First, find the actual thunk start (skip trailing NOPs and delay slots)
        # Check if immediately before prologue is a NOP (delay slot of jr ra)
        prev_word = self.read_word(prologue_addr - 4)
        if is_nop(prev_word):
            # Check if it's delay slot of jr ra
            prev_prev_word = self.read_word(prologue_addr - 8)
            if is_jr_ra(prev_prev_word):
                # Previous function ends at prologue_addr - 4, no thunk
                return None

        # Scan backwards from prologue, skipping leading NOPs
        scan_start = 1
        while scan_start <= max_instrs:
            addr = prologue_addr - (scan_start * 4)
            if addr < self.text_start:
                break
            word = self.read_word(addr)
            if not is_nop(word):
                break
            scan_start += 1

        # Scan backwards from prologue
        for i in range(1, max_instrs + 1):
            addr = prologue_addr - (i * 4)
            if addr < self.text_start:
                break

            word = self.read_word(addr)

            # Stop if we hit a previous function's epilogue (jr ra)
            if is_jr_ra(word):
                break

            # Stop if we hit a NOP that's a delay slot
            if is_nop(word):
                # Check if previous instruction is jr ra
                prev_word = self.read_word(addr - 4)
                if is_jr_ra(prev_word):
                    break
                # Skip NOPs that are just padding
                continue

            # Stop if we hit another prologue
            if is_prologue(word):
                break

            # Stop if we hit a branch that jumps elsewhere (not conditional)
            if is_branch_or_jump(word):
                instr = decode_instruction(word)
                # J, JAL, JR, JALR are hard stops
                if instr['opcode'] in [2, 3] or (instr['opcode'] == 0 and instr['funct'] in [8, 9]):
                    break
                # Conditional branches might be part of complex thunk
                # But stop if they branch backwards (loop)
                if instr['opcode'] in [4, 5, 6, 7]:
                    offset = instr['imm_signed'] * 4 + 4
                    if offset < 0:  # Backwards branch
                        break

            thunk_instrs.insert(0, (addr, word))
            thunk_start = addr

            # Track registers written
            written = get_written_registers(word)
            for reg in written:
                if reg not in [0, 29, 31]:  # Exclude zero, sp, ra
                    registers_set.add(reg)

            # Check for global loads (LUI + load pattern)
            instr = decode_instruction(word)
            if instr['opcode'] == 15:  # LUI
                globals_loaded.append(f"0x{instr['imm']:04X}0000")

            # Check for arg preprocessing (using a0-a3)
            read = get_read_registers(word)
            if any(4 <= r <= 7 for r in read):  # a0-a3
                arg_preprocessing = True

        # No thunk found
        if not thunk_instrs:
            return None

        # Filter out thunks that are just stores to globals (not setting up t-regs for function)
        # A real thunk should either:
        # 1. Set t-registers that the function uses
        # 2. Load from globals into registers the function uses
        # 3. Preprocess arguments for the function

        # Check for t-registers being set
        t_regs_set = {r for r in registers_set if (8 <= r <= 15) or (24 <= r <= 25)}

        # Check what registers the function actually uses in first few instructions
        regs_used_early = set()
        for j in range(1, 8):  # Skip prologue instruction
            func_word = self.read_word(prologue_addr + (j * 4))
            if is_jr_ra(func_word):
                break
            read = get_read_registers(func_word)
            regs_used_early.update(read)

        # Thunk is only valid if it sets registers the function uses
        registers_passed = registers_set & regs_used_early

        if not registers_passed and not arg_preprocessing:
            return None

        # Filter out thunks that just store to globals without setting up function input
        has_load = any(decode_instruction(w)['opcode'] in [32, 33, 35, 36, 37]
                      for _, w in thunk_instrs)
        has_alu = any(decode_instruction(w)['opcode'] in [8, 9, 12, 13, 14] or
                     (decode_instruction(w)['opcode'] == 0 and decode_instruction(w)['funct'] not in [8, 9])
                     for _, w in thunk_instrs)

        if not has_load and not has_alu and not arg_preprocessing:
            return None

        # regs_used_early already computed above

        # Build thunk info
        thunk_disasm = [
            f"0x{addr:08X}: {disassemble_instr(addr, word)}"
            for addr, word in thunk_instrs
        ]

        first_func_disasm = []
        for j in range(8):
            func_addr = prologue_addr + (j * 4)
            func_word = self.read_word(func_addr)
            first_func_disasm.append(f"0x{func_addr:08X}: {disassemble_instr(func_addr, func_word)}")
            if is_jr_ra(func_word):
                break

        # Determine pattern type
        if globals_loaded and arg_preprocessing:
            pattern_type = 'combined'
        elif globals_loaded:
            pattern_type = 'global_load'
        elif arg_preprocessing:
            pattern_type = 'arg_preprocess'
        else:
            pattern_type = 'complex'

        return ThunkInfo(
            thunk_address=thunk_start,
            function_address=prologue_addr,
            thunk_size=prologue_addr - thunk_start,
            thunk_instructions=len(thunk_instrs),
            registers_set=[get_reg_name(r) for r in sorted(registers_set)],
            globals_loaded=globals_loaded,
            arg_preprocessing=arg_preprocessing,
            thunk_disasm=thunk_disasm,
            first_func_instrs=first_func_disasm,
            registers_used_early=[get_reg_name(r) for r in sorted(registers_passed)],
            has_prologue_at_func=True,
            pattern_type=pattern_type
        )

    def find_jal_calls_to(self, target: int) -> List[int]:
        """Find all JAL calls to a specific address."""
        calls = []
        for addr in range(self.text_start, self.text_end, 4):
            word = self.read_word(addr)
            instr = decode_instruction(word)
            if instr['opcode'] == 3:  # JAL
                jal_target = (addr & 0xF0000000) | (instr['target'] << 2)
                if jal_target == target:
                    calls.append(addr)
        return calls

    def detect_all_thunks(self) -> List[ThunkInfo]:
        """Detect all thunk patterns in the executable."""
        self.load_exe()

        if self.verbose:
            print("Finding all prologues...")

        prologues = self.find_all_prologues()

        if self.verbose:
            print(f"Found {len(prologues)} prologues")
            print("Analyzing pre-prologue code...")

        thunks = []
        for prologue_addr in prologues:
            thunk = self.analyze_pre_prologue(prologue_addr)
            if thunk:
                thunks.append(thunk)

        if self.verbose:
            print(f"Found {len(thunks)} potential thunks")

        return thunks

    def verify_thunk_calls(self, thunks: List[ThunkInfo]) -> dict:
        """
        Verify calling patterns for thunks.
        Check if callers call the thunk or the function address.
        """
        verified = {
            'thunk_called': [],  # Thunks where callers call thunk address
            'func_called': [],   # Thunks where callers call function address
            'both_called': [],   # Both addresses called
            'neither_called': [] # Neither called (internal/unused)
        }

        if self.verbose:
            print("Verifying call patterns...")

        for thunk in thunks:
            thunk_calls = self.find_jal_calls_to(thunk.thunk_address)
            func_calls = self.find_jal_calls_to(thunk.function_address)

            thunk_dict = asdict(thunk)
            thunk_dict['thunk_callers'] = [f"0x{c:08X}" for c in thunk_calls]
            thunk_dict['function_callers'] = [f"0x{c:08X}" for c in func_calls]
            thunk_dict['thunk_address'] = f"0x{thunk.thunk_address:08X}"
            thunk_dict['function_address'] = f"0x{thunk.function_address:08X}"

            if thunk_calls and func_calls:
                verified['both_called'].append(thunk_dict)
            elif thunk_calls:
                verified['thunk_called'].append(thunk_dict)
            elif func_calls:
                verified['func_called'].append(thunk_dict)
            else:
                verified['neither_called'].append(thunk_dict)

        return verified


def main():
    parser = argparse.ArgumentParser(
        description='Detect thunk patterns in Tokimeki Memorial executable'
    )
    parser.add_argument('--exe', default='disks/SLPS_000.65',
                        help='Path to executable')
    parser.add_argument('--output', '-o', default='build/thunks.json',
                        help='Output JSON file')
    parser.add_argument('--verbose', '-v', action='store_true',
                        help='Verbose output')
    parser.add_argument('--verify-calls', action='store_true',
                        help='Verify calling patterns (slower)')

    args = parser.parse_args()

    if not Path(args.exe).exists():
        print(f"Error: Executable not found: {args.exe}")
        sys.exit(1)

    detector = ThunkDetector(args.exe, args.verbose)
    thunks = detector.detect_all_thunks()

    print(f"\n=== Thunk Detection Results ===")
    print(f"Total thunks detected: {len(thunks)}")

    # Categorize by pattern type
    by_type = {}
    for t in thunks:
        by_type.setdefault(t.pattern_type, []).append(t)

    print("\nBy pattern type:")
    for ptype, items in sorted(by_type.items()):
        print(f"  {ptype}: {len(items)}")

    # Categorize by registers set
    t_reg_thunks = [t for t in thunks if any('t' in r for r in t.registers_set)]
    print(f"\nThunks setting t-registers: {len(t_reg_thunks)}")

    # Show some examples
    print("\n=== Example Thunks ===")
    for i, thunk in enumerate(thunks[:5]):
        print(f"\n[{i+1}] Thunk at 0x{thunk.thunk_address:08X} -> Func at 0x{thunk.function_address:08X}")
        print(f"    Size: {thunk.thunk_size} bytes ({thunk.thunk_instructions} instructions)")
        print(f"    Pattern: {thunk.pattern_type}")
        print(f"    Registers set: {', '.join(thunk.registers_set)}")
        print(f"    Globals loaded: {', '.join(thunk.globals_loaded) if thunk.globals_loaded else 'none'}")
        print(f"    Thunk code:")
        for line in thunk.thunk_disasm:
            print(f"      {line}")
        print(f"    Function start:")
        for line in thunk.first_func_instrs[:4]:
            print(f"      {line}")

    # Verify calls if requested
    if args.verify_calls:
        verified = detector.verify_thunk_calls(thunks)
        print(f"\n=== Call Pattern Verification ===")
        print(f"  Thunk address called: {len(verified['thunk_called'])}")
        print(f"  Function address called: {len(verified['func_called'])}")
        print(f"  Both called: {len(verified['both_called'])}")
        print(f"  Neither called: {len(verified['neither_called'])}")

        # Output includes verification
        output_data = {
            'metadata': {
                'executable': args.exe,
                'total_thunks': len(thunks),
                'by_type': {k: len(v) for k, v in by_type.items()},
                't_register_thunks': len(t_reg_thunks),
                'verified': True
            },
            'call_verification': {
                'thunk_called': len(verified['thunk_called']),
                'func_called': len(verified['func_called']),
                'both_called': len(verified['both_called']),
                'neither_called': len(verified['neither_called'])
            },
            'thunks': verified
        }
    else:
        # Convert to dict for JSON
        thunk_dicts = []
        for t in thunks:
            d = asdict(t)
            d['thunk_address'] = f"0x{t.thunk_address:08X}"
            d['function_address'] = f"0x{t.function_address:08X}"
            thunk_dicts.append(d)

        output_data = {
            'metadata': {
                'executable': args.exe,
                'total_thunks': len(thunks),
                'by_type': {k: len(v) for k, v in by_type.items()},
                't_register_thunks': len(t_reg_thunks),
                'verified': False
            },
            'thunks': thunk_dicts
        }

    # Write output
    output_path = Path(args.output)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    with open(output_path, 'w') as f:
        json.dump(output_data, f, indent=2)

    print(f"\nResults written to: {args.output}")


if __name__ == '__main__':
    main()
