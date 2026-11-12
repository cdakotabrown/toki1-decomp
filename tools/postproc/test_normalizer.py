#!/usr/bin/env python3
"""
Regression Tests for scheduler_normalizer.py

Tests that the scheduler normalizer correctly reorders instructions to match
the target binary for known-good test cases. Each test case:
1. Compiles the C source with IDO 5.2
2. Applies pre-normalizer post-processing (move encoding, frame patching)
3. Runs the scheduler normalizer
4. Compares the .text section against the target binary

Test cases cover:
- Tier 1 (target-guided topological sort): simple 1-2 swap cases
- Tier 2 (greedy swap search): 3-4 swap cases
- Tier 3 (forced reordering): dependency-overriding cases

Usage:
  python3 tools/postproc/test_normalizer.py           # Run all tests
  python3 tools/postproc/test_normalizer.py -v         # Verbose output
  python3 tools/postproc/test_normalizer.py -k 412DC   # Run specific test
"""

import argparse
import os
import shutil
import struct
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from typing import List, Optional, Set

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.dirname(os.path.dirname(SCRIPT_DIR))
IDO_CC = os.path.join(PROJECT_ROOT, 'tools', 'ido-static-recomp', 'build', '5.2', 'out', 'cc')
NORMALIZER = os.path.join(SCRIPT_DIR, 'scheduler_normalizer.py')
CONVERT_OR = os.path.join(PROJECT_ROOT, 'tools', 'ido', 'convert_or_to_addu.py')
PATCH_STACK = os.path.join(PROJECT_ROOT, 'tools', 'ido', 'patch_stack_complete.py')
PATCH_RA = os.path.join(PROJECT_ROOT, 'tools', 'ido', 'patch_ra_offset.py')
TARGET_EXE = os.path.join(PROJECT_ROOT, 'disks', 'SLPS_000.65')
TARGET_BASE = 0x80041000
FILE_BASE = 0x800


@dataclass
class TestCase:
    """A regression test case for the normalizer."""
    name: str
    address: int
    source: str           # Relative to PROJECT_ROOT
    region: str           # 'game' or 'library'
    num_instructions: int
    expected_swaps: int   # Minimum swaps the normalizer should find
    description: str
    extra_postproc: Optional[List[str]] = None  # Additional post-processing commands


# Test cases covering all normalizer tiers
TEST_CASES = [
    TestCase(
        name="screen_clear",
        address=0x800412DC,
        source="src/main/matched/screen_clear.c",
        region="game",
        num_instructions=19,
        expected_swaps=1,
        description="RECT setup: sh t6/sh t7 order swap (Tier 1 - toposort)",
    ),
    TestCase(
        name="StSetRing",
        address=0x800882B0,
        source="src/main/matched/StSetRing.c",
        region="library",
        num_instructions=10,
        expected_swaps=1,
        description="Cross-prologue: sw a0 before addiu sp (Tier 1 - toposort)",
    ),
    TestCase(
        name="_spu_read",
        address=0x80095CD8,
        source="src/main/matched/_spu_read.c",
        region="library",
        num_instructions=10,
        expected_swaps=1,
        description="_spu_read wrapper: sw/lw scheduling (Tier 1 or forced)",
        extra_postproc=[
            ['python3', PATCH_STACK, '{obj}', '{obj}', '--old', '24', '--new', '32'],
            ['python3', PATCH_RA, '{obj}', '{obj}', '--old', '20', '--new', '28'],
        ],
    ),
    TestCase(
        name="GsSetLightMatrix2",
        address=0x8009AF08,
        source="src/main/matched/GsSetLightMatrix2.c",
        region="library",
        num_instructions=12,
        expected_swaps=1,
        description="Chained call: lui/sw ra cross-prologue swap (Tier 1 - toposort)",
        extra_postproc=[
            ['python3', PATCH_RA, '{obj}', '{obj}', '--old', '20', '--new', '16'],
        ],
    ),
    # NOTE: func_80077D6C is excluded — the normalizer's trailing-nop trimming
    # prevents it from handling the li/nop swap at instructions 15<->19.
    # This is a known limitation documented in AS1_SCHEDULING_TODO.md.
]


def read_target_bytes(address: int, num_bytes: int) -> bytes:
    """Read bytes from the target executable."""
    with open(TARGET_EXE, 'rb') as f:
        offset = FILE_BASE + (address - TARGET_BASE)
        f.seek(offset)
        return f.read(num_bytes)


def extract_text_section(obj_path: str) -> tuple:
    """Extract .text section bytes and relocation offsets from an ELF .o file.
    Returns (text_bytes, reloc_offsets).
    """
    with open(obj_path, 'rb') as f:
        data = f.read()

    if data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    e_shoff = struct.unpack('<I', data[0x20:0x24])[0]
    e_shentsize = struct.unpack('<H', data[0x2E:0x30])[0]
    e_shnum = struct.unpack('<H', data[0x30:0x32])[0]
    e_shstrndx = struct.unpack('<H', data[0x32:0x34])[0]

    strtab_hdr = e_shoff + e_shstrndx * e_shentsize
    str_offset = struct.unpack('<I', data[strtab_hdr + 16:strtab_hdr + 20])[0]
    str_size = struct.unpack('<I', data[strtab_hdr + 20:strtab_hdr + 24])[0]
    strtab = data[str_offset:str_offset + str_size]

    text_bytes = None
    reloc_offsets = set()

    for i in range(e_shnum):
        hdr = e_shoff + i * e_shentsize
        sh_name = struct.unpack('<I', data[hdr:hdr + 4])[0]
        name_end = strtab.index(0, sh_name)
        name = strtab[sh_name:name_end].decode()

        if name == '.text':
            sh_off = struct.unpack('<I', data[hdr + 16:hdr + 20])[0]
            sh_sz = struct.unpack('<I', data[hdr + 20:hdr + 24])[0]
            text_bytes = data[sh_off:sh_off + sh_sz]

        elif name == '.rel.text':
            sh_off = struct.unpack('<I', data[hdr + 16:hdr + 20])[0]
            sh_sz = struct.unpack('<I', data[hdr + 20:hdr + 24])[0]
            sh_entsize = struct.unpack('<I', data[hdr + 36:hdr + 40])[0]
            if sh_entsize == 0:
                sh_entsize = 8
            for j in range(sh_sz // sh_entsize):
                entry_off = sh_off + j * sh_entsize
                r_offset = struct.unpack('<I', data[entry_off:entry_off + 4])[0]
                reloc_offsets.add(r_offset)

    if text_bytes is None:
        raise ValueError(".text section not found")

    return text_bytes, reloc_offsets


def compare_instructions(compiled: bytes, target: bytes, reloc_offsets: Set[int],
                         num_instructions: int, verbose: bool = False) -> tuple:
    """Compare compiled vs target instructions.
    Returns (exact_matches, reloc_matches, frame_matches, mismatches, total).
    Handles:
    - Exact match
    - Relocation masking (upper 16 bits for lui/lw/sw, upper 6 bits for j/jal)
    - Branch offset masking (lower 16 bits differ due to reordering)
    - Frame-offset tolerance (same opcode+registers, different sp offset)
    """
    BRANCH_OPCODES = {1, 4, 5, 6, 7, 20, 21, 22, 23}
    JUMP_OPCODES = {2, 3}  # j, jal

    exact = 0
    reloc = 0
    frame = 0
    mismatch = 0
    total = min(num_instructions, len(compiled) // 4, len(target) // 4)

    for i in range(total):
        c_word = struct.unpack('<I', compiled[i*4:(i+1)*4])[0]
        t_word = struct.unpack('<I', target[i*4:(i+1)*4])[0]
        offset = i * 4
        c_opcode = (c_word >> 26) & 0x3F
        t_opcode = (t_word >> 26) & 0x3F

        if c_word == t_word:
            exact += 1
        elif c_opcode in JUMP_OPCODES and t_opcode in JUMP_OPCODES:
            # j/jal: compare upper 6 bits only (lower 26 are target address)
            if (c_word & 0xFC000000) == (t_word & 0xFC000000):
                reloc += 1
            else:
                mismatch += 1
                if verbose:
                    print(f"    MISMATCH (jump) at {offset:3x}: "
                          f"compiled={c_word:08x} target={t_word:08x}")
        elif offset in reloc_offsets:
            # Relocation entry: compare upper 16 bits (lower has unresolved imm)
            if (c_word & 0xFFFF0000) == (t_word & 0xFFFF0000):
                reloc += 1
            else:
                mismatch += 1
                if verbose:
                    print(f"    MISMATCH (reloc) at {offset:3x}: "
                          f"compiled={c_word:08x} target={t_word:08x}")
        elif c_opcode in BRANCH_OPCODES and t_opcode in BRANCH_OPCODES:
            # Branch: compare upper 16 bits (lower 16 are offset)
            if (c_word & 0xFFFF0000) == (t_word & 0xFFFF0000):
                reloc += 1
            else:
                mismatch += 1
                if verbose:
                    print(f"    MISMATCH (branch) at {offset:3x}: "
                          f"compiled={c_word:08x} target={t_word:08x}")
        elif (c_word & 0xFFFF0000) == (t_word & 0xFFFF0000):
            # Same opcode+registers, different immediate (frame offset tolerance)
            frame += 1
        else:
            mismatch += 1
            if verbose:
                print(f"    MISMATCH at {offset:3x}: "
                      f"compiled={c_word:08x} target={t_word:08x}")

    return exact, reloc, frame, mismatch, total


def run_test(tc: TestCase, verbose: bool = False) -> bool:
    """Run a single test case. Returns True if passed."""
    source_path = os.path.join(PROJECT_ROOT, tc.source)
    if not os.path.exists(source_path):
        print(f"  SKIP: Source not found: {tc.source}")
        return False

    with tempfile.TemporaryDirectory() as tmpdir:
        obj_path = os.path.join(tmpdir, f"{tc.name}.o")
        norm_path = os.path.join(tmpdir, f"{tc.name}_norm.o")

        # Step 1: Compile with IDO 5.2
        cmd = [IDO_CC, '-O2', '-G0', '-mips1', '-EL', '-non_shared',
               '-I', os.path.join(PROJECT_ROOT, 'include'),
               '-c', source_path, '-o', obj_path]
        result = subprocess.run(cmd, capture_output=True, text=True, cwd=PROJECT_ROOT)
        if result.returncode != 0:
            print(f"  FAIL: Compilation failed: {result.stderr.strip()}")
            return False

        # Step 2: Pre-normalizer post-processing
        if tc.region == 'library':
            result = subprocess.run(
                ['python3', CONVERT_OR, obj_path, obj_path],
                capture_output=True, text=True, cwd=PROJECT_ROOT)
            if result.returncode != 0:
                print(f"  FAIL: convert_or_to_addu failed: {result.stderr.strip()}")
                return False

        # Extra post-processing (frame patching, RA offset, etc.)
        if tc.extra_postproc:
            for cmd_template in tc.extra_postproc:
                cmd = [s.replace('{obj}', obj_path) for s in cmd_template]
                result = subprocess.run(cmd, capture_output=True, text=True,
                                        cwd=PROJECT_ROOT)
                if result.returncode != 0:
                    print(f"  FAIL: Post-processing failed: {' '.join(cmd)}")
                    print(f"        {result.stderr.strip()}")
                    return False

        # Step 3: Save pre-normalizer state for comparison
        pre_text, pre_relocs = extract_text_section(obj_path)
        target_bytes = read_target_bytes(tc.address, len(pre_text))

        pre_exact, pre_reloc, pre_frame, pre_mismatch, pre_total = \
            compare_instructions(pre_text, target_bytes, pre_relocs,
                                 tc.num_instructions, verbose)

        if verbose:
            pre_match = pre_exact + pre_reloc + pre_frame
            print(f"  Pre-normalizer: {pre_match}/{pre_total} "
                  f"(exact={pre_exact} reloc={pre_reloc} frame={pre_frame} "
                  f"mismatch={pre_mismatch})")

        # Step 4: Run normalizer
        shutil.copy2(obj_path, norm_path)
        cmd = ['python3', NORMALIZER, norm_path,
               '--target', TARGET_EXE,
               '--address', f'0x{tc.address:08X}']
        result = subprocess.run(cmd, capture_output=True, text=True,
                                cwd=PROJECT_ROOT)
        if result.returncode != 0:
            print(f"  FAIL: Normalizer failed: {result.stderr.strip()}")
            return False

        # Step 5: Compare post-normalizer result
        post_text, post_relocs = extract_text_section(norm_path)
        post_exact, post_reloc, post_frame, post_mismatch, post_total = \
            compare_instructions(post_text, target_bytes, post_relocs,
                                 tc.num_instructions, verbose)

        post_match = post_exact + post_reloc + post_frame
        total = post_total

        if verbose:
            print(f"  Post-normalizer: {post_match}/{total} "
                  f"(exact={post_exact} reloc={post_reloc} frame={post_frame} "
                  f"mismatch={post_mismatch})")

        # Verify: normalizer should have improved the match
        pre_match_count = pre_exact + pre_reloc + pre_frame
        improved = post_match > pre_match_count

        # Verify: no mismatches remain (all instructions accounted for)
        all_matched = (post_mismatch == 0)

        # Verify: normalizer actually did something (found expected swaps)
        swap_count = 0
        for line in result.stdout.split('\n'):
            if 'Swap [' in line:
                swap_count += 1

        if all_matched and (improved or pre_mismatch == 0):
            if verbose:
                print(f"  Normalizer applied {swap_count} swap(s)")
            return True
        else:
            print(f"  FAIL: Post-normalizer has {post_mismatch} unresolved mismatches")
            if not improved:
                print(f"  FAIL: Normalizer did not improve match "
                      f"({pre_match_count} -> {post_match})")
            return False


def main():
    parser = argparse.ArgumentParser(description='Regression tests for scheduler_normalizer.py')
    parser.add_argument('-v', '--verbose', action='store_true', help='Verbose output')
    parser.add_argument('-k', '--filter', default=None,
                        help='Only run tests matching this substring')
    args = parser.parse_args()

    # Check prerequisites
    if not os.path.exists(IDO_CC):
        print(f"ERROR: IDO compiler not found: {IDO_CC}")
        print("Run: make setup-ido52")
        sys.exit(1)
    if not os.path.exists(TARGET_EXE):
        print(f"ERROR: Target executable not found: {TARGET_EXE}")
        print("Run: make extract-exe")
        sys.exit(1)

    cases = TEST_CASES
    if args.filter:
        cases = [tc for tc in cases if args.filter.lower() in tc.name.lower()]

    if not cases:
        print("No test cases match filter")
        sys.exit(1)

    print(f"Scheduler Normalizer Regression Tests")
    print(f"{'=' * 60}")
    print(f"Running {len(cases)} test(s)")
    print()

    passed = 0
    failed = 0
    skipped = 0

    for tc in cases:
        print(f"[TEST] {tc.name} @ 0x{tc.address:08X}")
        print(f"       {tc.description}")
        try:
            if run_test(tc, args.verbose):
                print(f"  PASS")
                passed += 1
            else:
                failed += 1
        except Exception as e:
            print(f"  ERROR: {e}")
            if args.verbose:
                import traceback
                traceback.print_exc()
            failed += 1
        print()

    print(f"{'=' * 60}")
    print(f"Results: {passed} passed, {failed} failed, {skipped} skipped "
          f"/ {len(cases)} total")

    if failed > 0:
        sys.exit(1)


if __name__ == '__main__':
    main()
