#!/usr/bin/env python3
"""
compiler_fingerprint.py - Systematic compiler fingerprinting for GCC region

Tests different PSY-Q GCC versions and flag combinations to find the best
match for functions in the GCC region (0x80041000-0x80085FFF).

Usage:
    python3 tools/analysis/compiler_fingerprint.py
    python3 tools/analysis/compiler_fingerprint.py --verbose
    python3 tools/analysis/compiler_fingerprint.py --function prim_init
"""

import argparse
import json
import os
import struct
import subprocess
import sys
import tempfile
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List, Optional, Tuple

# Project paths
ROOT_DIR = Path(__file__).parent.parent.parent
INCLUDE_DIR = ROOT_DIR / "include"
EXE_PATH = ROOT_DIR / "disks" / "SLPS_000.65"

# Compiler paths
COMPILERS = {
    "gcc-2.6.0": ROOT_DIR / "tools" / "psyq-gcc-260" / "cc1",
    "gcc-2.7.2": ROOT_DIR / "tools" / "psyq-gcc-272" / "cc1",
    "gcc-2.8.1": ROOT_DIR / "tools" / "psyq-gcc" / "cc1",
}

CPP_PATH = "mipsel-linux-gnu-cpp"
AS_PATH = "mipsel-linux-gnu-as"
OBJCOPY_PATH = "mipsel-linux-gnu-objcopy"

# Address mapping
TEXT_SECTION_START = 0x80041000
FILE_OFFSET_BASE = 0x800


@dataclass
class TestFunction:
    """Represents a function to test"""
    name: str
    address: int
    size: int
    source: str  # C source code


@dataclass
class CompileResult:
    """Result of a compilation attempt"""
    success: bool
    binary: bytes = b""
    error: str = ""
    asm_output: str = ""


@dataclass
class MatchResult:
    """Result of comparing compiled code to target"""
    compiler: str
    flags: str
    match_percent: float
    byte_matches: int
    byte_total: int
    instruction_matches: int
    instruction_total: int
    first_diff_offset: int = -1
    details: Dict = field(default_factory=dict)


# Test functions - simple clean functions from the GCC region
TEST_FUNCTIONS = [
    TestFunction(
        name="dummy_func",
        address=0x80044640,
        size=8,
        source="""
void dummy_func(void) {
}
"""
    ),
    TestFunction(
        name="prim_init",
        address=0x80041744,
        size=48,
        source="""
typedef int s32;
extern void sprite_init(s32 arg0, s32 arg1);
extern void bg_init(s32 arg0, s32 arg1);

void prim_init(void) {
    sprite_init(0, 64);
    bg_init(0, 2);
}
"""
    ),
    TestFunction(
        name="step34_init1",
        address=0x8005A638,
        size=40,
        source="""
extern void k_disp_inc(void);
extern void sub_sub_step_up(void);

void step34_init1(void) {
    k_disp_inc();
    sub_sub_step_up();
}
"""
    ),
    TestFunction(
        name="ending_step_check",
        address=0x800795E0,
        size=28,
        source="""
extern unsigned char D_800E8678;

int ending_step_check(void) {
    unsigned char t;
    t = D_800E8678;
    if (t == 144) {
        return 1;
    }
    return 0;
}
"""
    ),
    TestFunction(
        name="byouki_init",
        address=0x80072D28,
        size=56,
        source="""
extern void parameter_show_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void sub_step_up(void);

void byouki_init(void) {
    parameter_show_init();
    hizuke_init();
    message_window_init();
    sub_step_up();
}
"""
    ),
]

# Flag combinations to test
FLAG_COMBINATIONS = [
    # Base combinations with optimization levels
    {"opt": "-O2", "G": "-G0", "extra": ""},
    {"opt": "-O1", "G": "-G0", "extra": ""},
    {"opt": "-O3", "G": "-G0", "extra": ""},
    {"opt": "-O0", "G": "-G0", "extra": ""},

    # G8 variants
    {"opt": "-O2", "G": "-G8", "extra": ""},
    {"opt": "-O1", "G": "-G8", "extra": ""},

    # With peephole variations
    {"opt": "-O2", "G": "-G0", "extra": "-fpeephole"},
    {"opt": "-O2", "G": "-G0", "extra": "-fno-peephole"},

    # With function-cse variations
    {"opt": "-O2", "G": "-G0", "extra": "-ffunction-cse"},
    {"opt": "-O2", "G": "-G0", "extra": "-fno-function-cse"},

    # With struct return and unsigned char
    {"opt": "-O2", "G": "-G0", "extra": "-fpcc-struct-return"},
    {"opt": "-O2", "G": "-G0", "extra": "-funsigned-char"},

    # Combined popular flags
    {"opt": "-O2", "G": "-G0", "extra": "-fpeephole -ffunction-cse -fpcc-struct-return"},
    {"opt": "-O2", "G": "-G0", "extra": "-funsigned-char -fpeephole"},

    # With -msoft-float (common for PS1)
    {"opt": "-O2", "G": "-G0", "extra": "-msoft-float"},
    {"opt": "-O2", "G": "-G0", "extra": "-msoft-float -fpeephole"},
]


def vram_to_file_offset(vram_addr: int) -> int:
    """Convert VRAM address to file offset"""
    return (vram_addr - TEXT_SECTION_START) + FILE_OFFSET_BASE


def extract_target_bytes(address: int, size: int) -> bytes:
    """Extract bytes from the target executable"""
    file_offset = vram_to_file_offset(address)
    with open(EXE_PATH, 'rb') as f:
        f.seek(file_offset)
        return f.read(size)


def preprocess_source(source: str, temp_dir: Path) -> Tuple[bool, str, str]:
    """Preprocess C source with cpp"""
    source_file = temp_dir / "source.c"
    preprocessed_file = temp_dir / "preprocessed.i"

    with open(source_file, 'w') as f:
        f.write(source)

    cmd = [
        CPP_PATH,
        "-I", str(INCLUDE_DIR),
        "-I", str(INCLUDE_DIR / "psyq"),
        "-lang-c",
        "-undef",
        str(source_file)
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        return False, "", result.stderr

    # Remove gcc-specific directives that confuse old GCC
    output = result.stdout
    # Filter out # line directives
    lines = [l for l in output.split('\n') if not l.startswith('#')]
    output = '\n'.join(lines)

    with open(preprocessed_file, 'w') as f:
        f.write(output)

    return True, str(preprocessed_file), ""


def compile_with_cc1(cc1_path: Path, preprocessed: str, flags: str, temp_dir: Path) -> CompileResult:
    """Compile preprocessed source with cc1"""
    asm_file = temp_dir / "output.s"

    # Build cc1 command
    cmd_parts = [str(cc1_path), "-quiet"]
    cmd_parts.extend(flags.split())
    cmd_parts.extend(["-mgas", "-fgnu-linker"])
    cmd_parts.extend(["-o", str(asm_file)])

    # cc1 reads from stdin or file
    with open(preprocessed, 'r') as f:
        source_content = f.read()

    result = subprocess.run(
        cmd_parts,
        input=source_content,
        capture_output=True,
        text=True
    )

    if result.returncode != 0:
        return CompileResult(success=False, error=result.stderr or result.stdout)

    if not asm_file.exists():
        return CompileResult(success=False, error="No assembly output generated")

    with open(asm_file, 'r') as f:
        asm_output = f.read()

    return CompileResult(success=True, asm_output=asm_output)


def assemble(asm_content: str, temp_dir: Path) -> CompileResult:
    """Assemble the output with GNU as"""
    asm_file = temp_dir / "assembled.s"
    obj_file = temp_dir / "assembled.o"
    bin_file = temp_dir / "assembled.bin"

    with open(asm_file, 'w') as f:
        f.write(asm_content)

    # Assemble
    cmd = [
        AS_PATH,
        "-EL",  # Little endian
        "-march=r3000",
        "-mtune=r3000",
        "-no-pad-sections",
        "-o", str(obj_file),
        str(asm_file)
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        return CompileResult(success=False, error=result.stderr or result.stdout)

    # Extract .text section
    cmd = [
        OBJCOPY_PATH,
        "-O", "binary",
        "-j", ".text",
        str(obj_file),
        str(bin_file)
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        return CompileResult(success=False, error=result.stderr)

    if not bin_file.exists():
        return CompileResult(success=False, error="No binary output")

    with open(bin_file, 'rb') as f:
        binary = f.read()

    return CompileResult(success=True, binary=binary)


def compile_function(source: str, compiler: str, flags: Dict) -> CompileResult:
    """Full compilation pipeline for a function"""
    cc1_path = COMPILERS.get(compiler)
    if not cc1_path or not cc1_path.exists():
        return CompileResult(success=False, error=f"Compiler not found: {compiler}")

    # Build flag string
    flag_str = f"{flags['opt']} {flags['G']} -mips1 -mcpu=3000"
    if flags.get('extra'):
        flag_str += f" {flags['extra']}"

    with tempfile.TemporaryDirectory() as temp_dir:
        temp_path = Path(temp_dir)

        # Step 1: Preprocess
        success, preprocessed, error = preprocess_source(source, temp_path)
        if not success:
            return CompileResult(success=False, error=f"Preprocessing failed: {error}")

        # Step 2: Compile with cc1
        result = compile_with_cc1(cc1_path, preprocessed, flag_str, temp_path)
        if not result.success:
            return result

        # Step 3: Assemble
        asm_result = assemble(result.asm_output, temp_path)
        if not asm_result.success:
            return asm_result

        asm_result.asm_output = result.asm_output
        return asm_result


def patch_frame_size(data: bytes, old_frame: int, new_frame: int) -> bytes:
    """Patch stack frame size in compiled bytes"""
    if len(data) < 4:
        return data

    result = bytearray(data)

    # Patch prologue: addiu sp,sp,-old -> addiu sp,sp,-new
    # addiu sp,sp,-N format: 0x27bdXXXX where XXXX is -N as s16
    old_prologue = struct.pack('<I', 0x27bd0000 | ((-old_frame) & 0xFFFF))
    new_prologue = struct.pack('<I', 0x27bd0000 | ((-new_frame) & 0xFFFF))

    # Find and replace prologue
    for i in range(0, len(result) - 3, 4):
        if result[i:i+4] == old_prologue:
            result[i:i+4] = new_prologue
            break

    # Patch epilogue: addiu sp,sp,old -> addiu sp,sp,new
    old_epilogue = struct.pack('<I', 0x27bd0000 | (old_frame & 0xFFFF))
    new_epilogue = struct.pack('<I', 0x27bd0000 | (new_frame & 0xFFFF))

    # Find and replace epilogue
    for i in range(len(result) - 4, -1, -4):
        if result[i:i+4] == old_epilogue:
            result[i:i+4] = new_epilogue
            break

    # Patch ra save/restore offsets: sw/lw ra,old_offset(sp) -> sw/lw ra,new_offset(sp)
    # For frame 24: ra at offset 16, for frame 40: ra at offset 20
    old_ra_offset = old_frame - 8 if old_frame == 24 else old_frame - 4
    new_ra_offset = new_frame - 20 if new_frame == 40 else new_frame - 8

    # Actually, let's figure out RA offset from actual differences
    # GCC uses ra at frame-8, target uses ra at offset 20 for frame 40
    if old_frame == 24 and new_frame == 40:
        # sw ra,16(sp) -> sw ra,20(sp)
        # 0xafbf0010 -> 0xafbf0014
        old_sw_ra = struct.pack('<I', 0xafbf0010)
        new_sw_ra = struct.pack('<I', 0xafbf0014)
        for i in range(0, len(result) - 3, 4):
            if result[i:i+4] == old_sw_ra:
                result[i:i+4] = new_sw_ra

        # lw ra,16(sp) -> lw ra,20(sp)
        # 0x8fbf0010 -> 0x8fbf0014
        old_lw_ra = struct.pack('<I', 0x8fbf0010)
        new_lw_ra = struct.pack('<I', 0x8fbf0014)
        for i in range(0, len(result) - 3, 4):
            if result[i:i+4] == old_lw_ra:
                result[i:i+4] = new_lw_ra

    return bytes(result)


def mask_relocations(data: bytes) -> bytes:
    """Mask out JAL/J targets as they are relocatable"""
    if len(data) < 4:
        return data

    result = bytearray(data)

    for i in range(0, len(result) - 3, 4):
        word = struct.unpack('<I', result[i:i+4])[0]
        opcode = (word >> 26) & 0x3F

        # JAL (opcode 0x03) and J (opcode 0x02)
        if opcode in (0x02, 0x03):
            # Mask the target address (lower 26 bits)
            masked_word = word & 0xFC000000  # Keep only opcode
            result[i:i+4] = struct.pack('<I', masked_word)

    return bytes(result)


def compare_bytes(compiled: bytes, target: bytes, apply_frame_patch: bool = False, mask_relocs: bool = False) -> MatchResult:
    """Compare compiled bytes to target"""
    # Optionally apply frame patching (24 -> 40 bytes)
    if apply_frame_patch:
        compiled = patch_frame_size(compiled, 24, 40)

    # Optionally mask relocations
    if mask_relocs:
        compiled = mask_relocations(compiled)
        target = mask_relocations(target)

    max_len = max(len(compiled), len(target))
    min_len = min(len(compiled), len(target))

    byte_matches = 0
    first_diff = -1

    for i in range(min_len):
        if compiled[i] == target[i]:
            byte_matches += 1
        elif first_diff == -1:
            first_diff = i

    # Count instruction matches (4-byte words)
    instr_matches = 0
    instr_total = max_len // 4

    for i in range(0, min_len - 3, 4):
        if compiled[i:i+4] == target[i:i+4]:
            instr_matches += 1

    match_percent = (byte_matches / max_len * 100) if max_len > 0 else 0

    result = MatchResult(
        compiler="",
        flags="",
        match_percent=match_percent,
        byte_matches=byte_matches,
        byte_total=max_len,
        instruction_matches=instr_matches,
        instruction_total=instr_total,
        first_diff_offset=first_diff
    )

    # Add size comparison
    result.details["compiled_size"] = len(compiled)
    result.details["target_size"] = len(target)
    result.details["frame_patched"] = apply_frame_patch
    result.details["relocs_masked"] = mask_relocs

    return result


def analyze_differences(compiled: bytes, target: bytes, address: int) -> str:
    """Analyze specific differences between compiled and target"""
    lines = []
    max_len = max(len(compiled), len(target))

    for i in range(0, max_len, 4):
        comp_word = struct.unpack('<I', compiled[i:i+4])[0] if i+4 <= len(compiled) else 0
        tgt_word = struct.unpack('<I', target[i:i+4])[0] if i+4 <= len(target) else 0

        if comp_word != tgt_word:
            lines.append(f"  {address + i:08x}: compiled={comp_word:08x} target={tgt_word:08x}")

    return '\n'.join(lines[:10])  # Limit to first 10 differences


def test_function(func: TestFunction, verbose: bool = False, apply_frame_patch: bool = False) -> List[MatchResult]:
    """Test all compiler/flag combinations for a function"""
    results = []
    target_bytes = extract_target_bytes(func.address, func.size)

    print(f"\n{'='*60}")
    print(f"Testing: {func.name} @ 0x{func.address:08x} ({func.size} bytes)")
    if apply_frame_patch:
        print(f"Frame patching enabled: 24 -> 40 bytes")
    print(f"{'='*60}")

    for compiler_name, cc1_path in COMPILERS.items():
        if not cc1_path.exists():
            print(f"  Skipping {compiler_name}: compiler not found")
            continue

        for flags in FLAG_COMBINATIONS:
            flag_str = f"{flags['opt']} {flags['G']}"
            if flags.get('extra'):
                flag_str += f" {flags['extra']}"

            result = compile_function(func.source, compiler_name, flags)

            if not result.success:
                if verbose:
                    print(f"  {compiler_name} {flag_str}: COMPILE FAILED - {result.error[:50]}")
                continue

            # Compare without frame patch first (always mask relocations)
            match_raw = compare_bytes(result.binary, target_bytes, apply_frame_patch=False, mask_relocs=True)

            # Compare with frame patch if enabled
            if apply_frame_patch and match_raw.match_percent < 100:
                match_patched = compare_bytes(result.binary, target_bytes, apply_frame_patch=True, mask_relocs=True)
                if match_patched.match_percent > match_raw.match_percent:
                    match = match_patched
                    match.flags = flag_str + " [+frame patch]"
                else:
                    match = match_raw
            else:
                match = match_raw

            match.compiler = compiler_name
            if "[+frame patch]" not in match.flags:
                match.flags = flag_str

            status = "MATCH!" if match.match_percent == 100 else f"{match.match_percent:.1f}%"
            print(f"  {compiler_name} {flag_str}: {status}")

            if verbose and match.match_percent < 100 and match.match_percent > 50:
                diff = analyze_differences(
                    patch_frame_size(result.binary, 24, 40) if apply_frame_patch else result.binary,
                    target_bytes,
                    func.address
                )
                if diff:
                    print(diff)

            results.append(match)

    return results


def generate_report(all_results: Dict[str, List[MatchResult]], output_path: Path):
    """Generate markdown report of results"""
    lines = [
        "# Compiler Fingerprint Results",
        "",
        "**Date**: Auto-generated",
        "",
        "## Overview",
        "",
        "This report shows the results of testing different PSY-Q GCC versions and flag",
        "combinations against functions in the GCC region (0x80041000-0x80085FFF).",
        "",
        "## Compilers Tested",
        "",
        "| Compiler | Path |",
        "|----------|------|",
    ]

    for name, path in COMPILERS.items():
        status = "Available" if path.exists() else "Not found"
        lines.append(f"| {name} | {status} |")

    lines.extend([
        "",
        "## Results by Function",
        "",
    ])

    best_overall = {}  # Track best results per compiler/flags combo

    for func_name, results in all_results.items():
        lines.append(f"### {func_name}")
        lines.append("")
        lines.append("| Compiler | Flags | Match % | Bytes | Instructions |")
        lines.append("|----------|-------|---------|-------|--------------|")

        # Sort by match percentage
        sorted_results = sorted(results, key=lambda r: -r.match_percent)

        for r in sorted_results:
            lines.append(
                f"| {r.compiler} | {r.flags} | {r.match_percent:.1f}% | "
                f"{r.byte_matches}/{r.byte_total} | {r.instruction_matches}/{r.instruction_total} |"
            )

            # Track best
            key = (r.compiler, r.flags)
            if key not in best_overall:
                best_overall[key] = []
            best_overall[key].append(r.match_percent)

        lines.append("")

    # Summary
    lines.extend([
        "## Summary - Best Compiler/Flag Combinations",
        "",
        "Average match percentage across all test functions:",
        "",
        "| Compiler | Flags | Average Match % |",
        "|----------|-------|-----------------|",
    ])

    averages = []
    for (compiler, flags), percentages in best_overall.items():
        avg = sum(percentages) / len(percentages)
        averages.append((avg, compiler, flags))

    averages.sort(reverse=True)
    for avg, compiler, flags in averages[:20]:  # Top 20
        lines.append(f"| {compiler} | {flags} | {avg:.1f}% |")

    lines.extend([
        "",
        "## Recommendations",
        "",
        "Based on the results above, the recommended compiler and flags for the GCC region:",
        "",
    ])

    if averages:
        best_avg, best_compiler, best_flags = averages[0]
        lines.append(f"**Best match**: {best_compiler} with flags: `{best_flags}`")
        lines.append(f"**Average match rate**: {best_avg:.1f}%")
        lines.append("")

        if best_avg < 50:
            lines.extend([
                "**Note**: Low match rates suggest the original compiler may be:",
                "- A different GCC version than those tested",
                "- A modified/customized PSY-Q compiler",
                "- Using flags not tested in this analysis",
                "",
            ])

    with open(output_path, 'w') as f:
        f.write('\n'.join(lines))

    print(f"\nReport written to: {output_path}")


def main():
    parser = argparse.ArgumentParser(
        description="Systematic compiler fingerprinting for GCC region"
    )
    parser.add_argument(
        '-v', '--verbose',
        action='store_true',
        help='Show detailed output'
    )
    parser.add_argument(
        '-f', '--function',
        help='Test specific function only'
    )
    parser.add_argument(
        '-o', '--output',
        default='docs/COMPILER-FINGERPRINT-RESULTS.md',
        help='Output report path'
    )
    parser.add_argument(
        '-p', '--patch-frame',
        action='store_true',
        help='Apply frame size patching (24 -> 40 bytes)'
    )

    args = parser.parse_args()

    # Check compilers
    print("Checking compilers...")
    for name, path in COMPILERS.items():
        status = "OK" if path.exists() else "NOT FOUND"
        print(f"  {name}: {status}")

    # Select functions to test
    if args.function:
        functions = [f for f in TEST_FUNCTIONS if f.name == args.function]
        if not functions:
            print(f"Function not found: {args.function}")
            sys.exit(1)
    else:
        functions = TEST_FUNCTIONS

    # Run tests
    all_results = {}
    for func in functions:
        results = test_function(func, args.verbose, args.patch_frame)
        all_results[func.name] = results

    # Generate report
    output_path = ROOT_DIR / args.output
    generate_report(all_results, output_path)

    # Print summary
    print("\n" + "="*60)
    print("SUMMARY")
    print("="*60)

    perfect_matches = 0
    high_matches = 0
    total_tests = 0

    for func_name, results in all_results.items():
        for r in results:
            total_tests += 1
            if r.match_percent == 100:
                perfect_matches += 1
            elif r.match_percent >= 80:
                high_matches += 1

    print(f"Total tests run: {total_tests}")
    print(f"Perfect matches (100%): {perfect_matches}")
    print(f"High matches (>=80%): {high_matches}")


if __name__ == "__main__":
    main()
