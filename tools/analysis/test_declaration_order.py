#!/usr/bin/env python3
"""
Test declaration order effects on IDO register allocation

Tests whether reordering variable declarations or duplicating expressions
affects which registers IDO assigns to temporaries.

Usage:
    python3 tools/analysis/test_declaration_order.py icon_schedule_set
    python3 tools/analysis/test_declaration_order.py --all
"""

import argparse
import subprocess
import tempfile
import re
from pathlib import Path
from typing import List, Tuple, Dict

REPO_ROOT = Path(__file__).parent.parent.parent

def parse_instructions(asm: str) -> List[Tuple[str, str]]:
    """Parse assembly into list of (opcode, operands) tuples"""
    instructions = []
    in_text = False
    for line in asm.split('\n'):
        # Check if we're in the .text section
        if '.text' in line:
            in_text = True
            continue
        if not in_text:
            continue

        # Strip address and hex bytes
        line = line.strip()
        if not line or line.startswith('.') or line.startswith('#'):
            continue

        # Match objdump format: address: hex bytes \t instruction
        match = re.match(r'^[0-9a-f]+:\s+[0-9a-f\s]+\s+([a-z][a-z0-9]*)\s+(.*)$', line, re.IGNORECASE)
        if match:
            opcode, operands = match.groups()
            instructions.append((opcode.strip(), operands.strip()))
    return instructions

def get_register_usage(instructions: List[Tuple[str, str]]) -> Dict[str, int]:
    """Count register usage in assembly"""
    reg_counts = {}
    for opcode, operands in instructions:
        # Extract all register references (handle both $reg and reg format)
        regs = re.findall(r'\$?(zero|at|v[01]|a[0-3]|t[0-9]|s[0-7]|k[01]|gp|sp|fp|ra)\b', operands)
        for reg in regs:
            # Normalize register names (remove $ if present)
            reg = reg.lstrip('$')
            reg_counts[reg] = reg_counts.get(reg, 0) + 1
    return reg_counts

def compile_with_ido(source_code: str, address: int, loopunroll0: bool = False) -> Tuple[str, int]:
    """Compile C code with IDO and return disassembly and match percentage"""
    # Determine region based on address
    is_game_region = address < 0x80086000

    with tempfile.NamedTemporaryFile(mode='w', suffix='.c', delete=False) as f:
        f.write(source_code)
        source_path = f.name

    try:
        # Compile with IDO
        cmd = [
            str(REPO_ROOT / 'tools/ido-static-recomp/build/5.2/out/cc'),
            '-O2', '-G0', '-mips1', '-EL', '-non_shared',
            '-I', str(REPO_ROOT / 'include'),
            '-c', source_path,
            '-o', '/tmp/test_decl_order.o'
        ]

        if loopunroll0:
            cmd.insert(2, '-Wo,-loopunroll,0')

        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode != 0:
            return f"COMPILE ERROR:\n{result.stderr}", 0

        # Just use the raw .o file for disassembly analysis
        # (We'll get match % from ido_diff.py which handles post-processing)
        obj_path = '/tmp/test_decl_order.o'

        # Disassemble
        disasm_result = subprocess.run(
            ['mipsel-linux-gnu-objdump', '-d', obj_path],
            capture_output=True,
            text=True
        )

        # Get match percentage by comparing with target
        diff_cmd = [
            'python3',
            str(REPO_ROOT / 'tools/decomp/ido_diff.py'),
            f'func_{address:08X}',
            '-c', source_path
        ]

        diff_result = subprocess.run(diff_cmd, capture_output=True, text=True)

        # Extract match percentage
        match = re.search(r'Match:\s+(\d+)/(\d+)\s+instructions\s+\((\d+\.?\d*)%\)', diff_result.stdout)
        if match:
            match_pct = float(match.group(3))
        else:
            match_pct = 0.0

        return disasm_result.stdout, match_pct

    finally:
        Path(source_path).unlink(missing_ok=True)

def test_func_8006CF20_variants():
    """Test declaration order variants for icon_schedule_set"""

    print("=" * 80)
    print("TESTING: icon_schedule_set (s1 ↔ s2 swap issue)")
    print("=" * 80)

    base_code = '''/* icon_schedule_set - BASELINE */
#include "types.h"

extern void icon_can_use_set(s32, s32);
extern u8 D_800FE87C;
extern u8 D_800FF6CC;
extern u32 D_800FF6C8;
extern u8 D_800FE740[];

void icon_schedule_set(void)
{
    s32 i;
    u8 *ptr;
    s32 limit;

    i = 0;
    ptr = D_800FE740;
    limit = 14;

    do {
        u8 value;
        value = (ptr + (D_800FE87C << 2))[1693];

        if (i != value) {
            icon_can_use_set(i, 0);
        } else {
            icon_can_use_set(i, 1);
        }
        i++;
    } while (i != limit);

    if (D_800FF6CC == 2) {
        if ((D_800FF6C8 & 0xF) != 14) {
            icon_can_use_set(14, 0);
        }
    }
}
'''

    # Variant 1: Reverse declaration order
    variant1 = base_code.replace(
        '    s32 i;\n    u8 *ptr;\n    s32 limit;',
        '    s32 limit;\n    u8 *ptr;\n    s32 i;'
    )

    # Variant 2: Duplicate ptr initialization
    variant2 = base_code.replace(
        '    ptr = D_800FE740;',
        '    ptr = D_800FE740;\n    ptr = D_800FE740;  /* duplicated */'
    )

    # Variant 3: Duplicate limit initialization
    variant3 = base_code.replace(
        '    limit = 14;',
        '    limit = 14;\n    limit = 14;  /* duplicated */'
    )

    # Variant 4: All declarations in reverse
    variant4 = variant1

    variants = [
        ("BASELINE", base_code),
        ("VARIANT 1: Reverse declaration order (limit, ptr, i)", variant1),
        ("VARIANT 2: Duplicate ptr initialization", variant2),
        ("VARIANT 3: Duplicate limit initialization", variant3),
    ]

    results = []
    for name, code in variants:
        print(f"\n{name}")
        print("-" * 80)
        asm, match_pct = compile_with_ido(code, 0x8006CF20)

        # Extract key register assignments
        instructions = parse_instructions(asm)
        reg_usage = get_register_usage(instructions)

        print(f"Match: {match_pct}%")
        print(f"Total instructions parsed: {len(instructions)}")

        if len(instructions) > 0:
            print(f"First 10 instructions:")
            for i, (op, operands) in enumerate(instructions[:10]):
                print(f"  {i}: {op:6s} {operands}")

        print(f"Register usage: {dict(sorted(reg_usage.items()))}")

        # Look for s1/s2 usage specifically
        s1_count = reg_usage.get('s1', 0)
        s2_count = reg_usage.get('s2', 0)
        print(f"s1 usage: {s1_count}, s2 usage: {s2_count}")

        # Look for sw s1 and sw s2 instructions specifically
        s1_stores = []
        s2_stores = []
        for i, (opcode, operands) in enumerate(instructions):
            if opcode == 'sw' and 's1' in operands:
                s1_stores.append(f"  {i}: sw {operands}")
            if opcode == 'sw' and 's2' in operands:
                s2_stores.append(f"  {i}: sw {operands}")

        if s1_stores:
            print(f"s1 store instructions:")
            for s in s1_stores:
                print(s)
        if s2_stores:
            print(f"s2 store instructions:")
            for s in s2_stores:
                print(s)

        results.append((name, match_pct, reg_usage))

    return results

def test_func_8004D680_variants():
    """Test declaration order variants for move_255_line"""

    print("\n" + "=" * 80)
    print("TESTING: move_255_line (instruction scheduling issue)")
    print("=" * 80)

    base_code = '''/* move_255_line - BASELINE */
#include "types.h"

extern void image_rect_move(s32, s32, s32, s32, s32, s32);

void move_255_line(s32 a0, s32 a1, s32 a2) {
    s32 s0;
    s32 s1;
    s32 s2;
    s32 s3;
    s32 s4;
    s32 s5;
    s32 t6;
    s32 t7;

    s2 = a0;
    s0 = a0;

    if (a1 < a0) {
        return;
    }

    s3 = a2 & 0xF;
    s4 = a2 & 0x10;
    t7 = s4 << 4;
    t6 = s3 << 6;
    s1 = (a0 << 6) + 63;
    s3 = t6;
    s4 = t7 & 0xFFFF;
    s5 = a1 + 1;

    do {
        image_rect_move(s1 & 0xFFFF, 0, 1, 255, s3 + s0 - s2, s4);
        s0++;
        s1 += 64;
    } while (s5 != s0);
}
'''

    # Variant 1: Declare t6/t7 before s-registers
    variant1 = base_code.replace(
        '    s32 s0;\n    s32 s1;\n    s32 s2;\n    s32 s3;\n    s32 s4;\n    s32 s5;\n    s32 t6;\n    s32 t7;',
        '    s32 t6;\n    s32 t7;\n    s32 s0;\n    s32 s1;\n    s32 s2;\n    s32 s3;\n    s32 s4;\n    s32 s5;'
    )

    # Variant 2: Reverse all declarations
    variant2 = base_code.replace(
        '    s32 s0;\n    s32 s1;\n    s32 s2;\n    s32 s3;\n    s32 s4;\n    s32 s5;\n    s32 t6;\n    s32 t7;',
        '    s32 t7;\n    s32 t6;\n    s32 s5;\n    s32 s4;\n    s32 s3;\n    s32 s2;\n    s32 s1;\n    s32 s0;'
    )

    # Variant 3: Duplicate shift operations
    variant3 = base_code.replace(
        '    t7 = s4 << 4;\n    t6 = s3 << 6;',
        '    t7 = s4 << 4;\n    t7 = s4 << 4;  /* duplicate */\n    t6 = s3 << 6;\n    t6 = s3 << 6;  /* duplicate */'
    )

    variants = [
        ("BASELINE", base_code),
        ("VARIANT 1: t6/t7 declared first", variant1),
        ("VARIANT 2: All declarations reversed", variant2),
        ("VARIANT 3: Duplicate shift operations", variant3),
    ]

    results = []
    for name, code in variants:
        print(f"\n{name}")
        print("-" * 80)
        asm, match_pct = compile_with_ido(code, 0x8004D680)

        instructions = parse_instructions(asm)
        reg_usage = get_register_usage(instructions)

        print(f"Match: {match_pct}%")
        print(f"Register usage: {dict(sorted(reg_usage.items()))}")

        # Look for shift instruction order in assembly
        shift_pattern = []
        for i, (opcode, operands) in enumerate(instructions):
            if opcode == 'sll':
                shift_pattern.append(f"{i}: sll {operands}")

        print(f"Shift instruction order:")
        for s in shift_pattern[:5]:  # First 5 shifts
            print(f"  {s}")

        results.append((name, match_pct, reg_usage))

    return results

def test_func_800A3274_variants():
    """Test declaration order variants for DumpDispEnv"""

    print("\n" + "=" * 80)
    print("TESTING: DumpDispEnv (v0 reuse vs distinct t-registers)")
    print("=" * 80)

    base_code = '''/* DumpDispEnv - BASELINE */
#include "types.h"

typedef void (*FuncPtr)(char*, ...);

void DumpDispEnv(void* arg0) {
    FuncPtr func1;
    FuncPtr func2;
    FuncPtr func3;
    FuncPtr func4;
    s16* s0;

    s0 = (s16*)arg0;

    func1 = *(FuncPtr*)0x800E3C88;
    func1((char*)0x800E3D60, s0[0], s0[1], s0[2], s0[3]);

    func2 = *(FuncPtr*)0x800E3C88;
    func2((char*)0x800E3D7C, s0[4], s0[5], s0[6], s0[7]);

    func3 = *(FuncPtr*)0x800E3C88;
    func3((char*)0x800E3D98, ((u8*)s0)[16]);

    func4 = *(FuncPtr*)0x800E3C88;
    func4((char*)0x800E3DA4, ((u8*)s0)[17]);
}
'''

    # Variant 1: Reverse func pointer declaration order
    variant1 = base_code.replace(
        '    FuncPtr func1;\n    FuncPtr func2;\n    FuncPtr func3;\n    FuncPtr func4;\n    s16* s0;',
        '    s16* s0;\n    FuncPtr func4;\n    FuncPtr func3;\n    FuncPtr func2;\n    FuncPtr func1;'
    )

    # Variant 2: Duplicate first function pointer load
    variant2 = base_code.replace(
        '    func1 = *(FuncPtr*)0x800E3C88;',
        '    func1 = *(FuncPtr*)0x800E3C88;\n    func1 = *(FuncPtr*)0x800E3C88;  /* duplicate */'
    )

    # Variant 3: Use volatile keyword
    variant3 = base_code.replace(
        'typedef void (*FuncPtr)(char*, ...);',
        'typedef void (*FuncPtr)(char*, ...);\ntypedef volatile FuncPtr VolatileFuncPtr;'
    ).replace(
        '    FuncPtr func1;\n    FuncPtr func2;\n    FuncPtr func3;\n    FuncPtr func4;',
        '    VolatileFuncPtr func1;\n    VolatileFuncPtr func2;\n    VolatileFuncPtr func3;\n    VolatileFuncPtr func4;'
    )

    # Variant 4: Interleave declarations with loads
    variant4 = '''/* DumpDispEnv - VARIANT 4 */
#include "types.h"

typedef void (*FuncPtr)(char*, ...);

void DumpDispEnv(void* arg0) {
    s16* s0;
    s0 = (s16*)arg0;

    {
        FuncPtr func1;
        func1 = *(FuncPtr*)0x800E3C88;
        func1((char*)0x800E3D60, s0[0], s0[1], s0[2], s0[3]);
    }

    {
        FuncPtr func2;
        func2 = *(FuncPtr*)0x800E3C88;
        func2((char*)0x800E3D7C, s0[4], s0[5], s0[6], s0[7]);
    }

    {
        FuncPtr func3;
        func3 = *(FuncPtr*)0x800E3C88;
        func3((char*)0x800E3D98, ((u8*)s0)[16]);
    }

    {
        FuncPtr func4;
        func4 = *(FuncPtr*)0x800E3C88;
        func4((char*)0x800E3DA4, ((u8*)s0)[17]);
    }
}
'''

    variants = [
        ("BASELINE", base_code),
        ("VARIANT 1: Reverse func pointer declarations", variant1),
        ("VARIANT 2: Duplicate first load", variant2),
        ("VARIANT 3: Volatile keyword", variant3),
        ("VARIANT 4: Block-scoped declarations", variant4),
    ]

    results = []
    for name, code in variants:
        print(f"\n{name}")
        print("-" * 80)
        asm, match_pct = compile_with_ido(code, 0x800A3274)

        instructions = parse_instructions(asm)
        reg_usage = get_register_usage(instructions)

        print(f"Match: {match_pct}%")
        print(f"Register usage: {dict(sorted(reg_usage.items()))}")

        # Look for jalr patterns
        jalr_regs = []
        for opcode, operands in instructions:
            if opcode == 'jalr':
                jalr_regs.append(operands.split(',')[0].strip() if ',' in operands else operands.strip())

        print(f"jalr registers: {jalr_regs}")

        # Count v0 vs t-register usage for function pointers
        v0_jalr = sum(1 for r in jalr_regs if r in ['v0', '$v0'])
        t_jalr = sum(1 for r in jalr_regs if r in ['t6', 't7', 't8', 't9', 't0', 't1', '$t6', '$t7', '$t8', '$t9', '$t0', '$t1'])
        print(f"v0 jalr: {v0_jalr}, t-reg jalr: {t_jalr}")

        results.append((name, match_pct, reg_usage))

    return results

def main():
    parser = argparse.ArgumentParser(
        description='Test declaration order effects on IDO register allocation'
    )
    parser.add_argument('function', nargs='?', help='Function to test (icon_schedule_set, move_255_line, DumpDispEnv)')
    parser.add_argument('--all', action='store_true', help='Test all functions')

    args = parser.parse_args()

    if args.all or not args.function:
        print("Testing all functions with declaration order variants\n")
        results_8006 = test_func_8006CF20_variants()
        results_8004 = test_func_8004D680_variants()
        results_800A = test_func_800A3274_variants()

        print("\n" + "=" * 80)
        print("SUMMARY")
        print("=" * 80)

        print("\nfunc_8006CF20 results:")
        for name, match_pct, _ in results_8006:
            print(f"  {match_pct:5.1f}% - {name}")

        print("\nfunc_8004D680 results:")
        for name, match_pct, _ in results_8004:
            print(f"  {match_pct:5.1f}% - {name}")

        print("\nfunc_800A3274 results:")
        for name, match_pct, _ in results_800A:
            print(f"  {match_pct:5.1f}% - {name}")

    elif args.function == 'icon_schedule_set':
        test_func_8006CF20_variants()
    elif args.function == 'move_255_line':
        test_func_8004D680_variants()
    elif args.function == 'DumpDispEnv':
        test_func_800A3274_variants()
    else:
        print(f"Unknown function: {args.function}")
        print("Available: icon_schedule_set, move_255_line, DumpDispEnv")

if __name__ == '__main__':
    main()
