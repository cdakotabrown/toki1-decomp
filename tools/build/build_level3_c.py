#!/usr/bin/env python3
"""
Build game binary with Level 3 (byte-perfect) matched C functions.

This script:
1. Reads the original executable
2. Compiles each Level 3 C function with IDO 5.2
3. Applies documented post-processing (frame patches, register swaps, etc.)
4. Patches the compiled code into the binary
5. Verifies match against original

Usage:
    python3 tools/build/build_level3_c.py [--verbose]
"""

import json
import struct
import hashlib
import argparse
import subprocess
import tempfile
import os
import sys
import shutil
import time
from pathlib import Path
from concurrent.futures import ProcessPoolExecutor, as_completed

# Import post-processing parser
sys.path.insert(0, str(Path(__file__).parent))
from parse_postproc import parse_postproc_header, parse_compile_flags, parse_compiler, parse_compile_env
from banned_postproc import check_banned_postproc, check_banned_postproc_steps

# Constants
SCRIPT_DIR = Path(__file__).parent
ROOT_DIR = SCRIPT_DIR.parent.parent
EXE_HEADER_SIZE = 0x800
TEXT_LOAD_ADDR = 0x80041000
EXPECTED_SHA1 = "2d8d474816e4bf937b09dc3b017f550f7ce4e14b"

# Tool paths
IDO_CC = ROOT_DIR / "tools/ido-static-recomp/build/5.2/out/cc"
IDO_CC_53 = ROOT_DIR / "tools/ido-static-recomp/build/5.3/out/cc"
GCCWRAP = ROOT_DIR / "tools/gcc/gccwrap.py"
FRAME_PATCHER = ROOT_DIR / "tools/ido/patch_frame_size.py"
STACK_PATCHER = ROOT_DIR / "tools/ido/patch_stack_complete.py"
OR_TO_ADDU_PATCHER = ROOT_DIR / "tools/ido/convert_or_to_addu.py"
SWAP_REGISTERS = ROOT_DIR / "tools/ido/swap_registers.py"
SWAP_REGISTERS_LIVENESS = ROOT_DIR / "tools/ido/swap_registers_liveness.py"
SWAP_INSTRUCTIONS = ROOT_DIR / "tools/ido/swap_instructions.py"
PATCH_RA_OFFSET = ROOT_DIR / "tools/ido/patch_ra_offset.py"
FIX_SLL_SRA_PATTERN = ROOT_DIR / "tools/ido/fix_sll_sra_cross_reg.py"
FIX_ANDI_PATTERN = ROOT_DIR / "tools/ido/fix_andi_pattern.py"
STRIP_DIV_CHECKS = ROOT_DIR / "tools/ido/strip_div_checks.py"
STRIP_INTERIOR_NOPS = ROOT_DIR / "tools/ido/strip_interior_nops.py"
STRIP_LEADING_INSTRUCTIONS = ROOT_DIR / "tools/ido/strip_leading_instructions.py"
STRIP_TRAILING_INSTRUCTIONS = ROOT_DIR / "tools/ido/strip_trailing_instructions.py"
PATCH_NOR_OPERANDS = ROOT_DIR / "tools/ido/patch_nor_operands.py"
CONVERT_ORI_TO_ADDIU = ROOT_DIR / "tools/ido/convert_ori_to_addiu.py"
SCHEDULER_NORMALIZER = ROOT_DIR / "tools/postproc/scheduler_normalizer.py"
INSERT_MOVE_AND_SHIFT = ROOT_DIR / "tools/ido/insert_move_and_shift.py"
ADD_LEAF_FRAME = ROOT_DIR / "tools/ido/add_leaf_frame.py"
PATCH_STORE_ZERO = ROOT_DIR / "tools/ido/patch_store_zero.py"
INSERT_INSTRUCTION_AND_SHIFT = ROOT_DIR / "tools/ido/insert_instruction_and_shift.py"
FILL_DELAY_SLOTS = ROOT_DIR / "tools/postproc/fill_delay_slots.py"
REMAP_STACK_SLOTS = ROOT_DIR / "tools/ido/remap_stack_slots.py"
RENAME_REGISTER_CHAIN = ROOT_DIR / "tools/ido/rename_register_chain.py"
SHIFT_STACK_OFFSETS = ROOT_DIR / "tools/ido/shift_stack_offsets.py"
UNFILL_BRANCH_DELAY = ROOT_DIR / "tools/ido/unfill_branch_delay.py"
PATCH_LOAD_SIGNEDNESS = ROOT_DIR / "tools/ido/patch_load_signedness.py"
STRIP_ARG_HOMES = ROOT_DIR / "tools/ido/strip_arg_homes.py"
PROPAGATE_STACK_TO_REG = ROOT_DIR / "tools/ido/propagate_stack_to_reg.py"
SWAP_BRANCH_OPERANDS = ROOT_DIR / "tools/ido/swap_branch_operands.py"
INVERT_BRANCH = ROOT_DIR / "tools/ido/invert_branch.py"
FIX_PROLOGUE_AND_DELAYS = ROOT_DIR / "tools/ido/fix_prologue_and_delays.py"
COLLAPSE_SHIFT_MOVE = ROOT_DIR / "tools/ido/collapse_shift_move.py"
STRIP_INSTRUCTIONS = ROOT_DIR / "tools/ido/strip_instructions.py"
SWAP_COMMUTATIVE_OPERANDS = ROOT_DIR / "tools/ido/swap_commutative_operands.py"
ADJUST_BRANCH_OFFSET = ROOT_DIR / "tools/ido/adjust_branch_offset.py"
PATCH_INSN_REG_FIELD = ROOT_DIR / "tools/ido/patch_insn_reg_field.py"
PATCH_FRAME_FULL = ROOT_DIR / "tools/ido/patch_frame_full.py"
ROTATE_DISPATCH = ROOT_DIR / "tools/ido/rotate_dispatch.py"
REBASE_STORE_TO_SP = ROOT_DIR / "tools/ido/rebase_store_to_sp.py"
CSE_LUI_AT = ROOT_DIR / "tools/ido/cse_lui_at.py"
FIX_SIGN_TEST_PATTERN = ROOT_DIR / "tools/ido/fix_sign_test_pattern.py"
FILL_GCC_DELAY_SLOTS = ROOT_DIR / "tools/ido/fill_gcc_delay_slots.py"
SWAP_LUI_ADDR_FORM = ROOT_DIR / "tools/ido/swap_lui_addr_form.py"


def addr_to_file_offset(vram_addr):
    """Convert VRAM address to file offset."""
    return EXE_HEADER_SIZE + (vram_addr - TEXT_LOAD_ADDR)


def apply_postprocessing(obj_path, postproc_steps, func_name, verbose=False, address=0):
    """Apply post-processing steps to object file.

    Returns path to final processed object file.
    """
    # ── BANNED POSTPROC ENFORCEMENT ──────────────────────────
    # Block any banned directive before we execute anything.
    check_banned_postproc_steps(
        postproc_steps,
        source_path=func_name,
        strict=True,
    )
    # ─────────────────────────────────────────────────────────

    current_obj = obj_path

    for i, step in enumerate(postproc_steps):
        step_type = step['type']
        next_obj = obj_path.replace('.o', f'_step{i}.o')

        if step_type == 'patch_frame':
            old_size = step['old_size']
            new_size = step['new_size']
            if step.get('shift_ra'):
                cmd = [
                    sys.executable, str(STACK_PATCHER),
                    current_obj, next_obj,
                    "--old", str(old_size), "--new", str(new_size),
                    "--shift-ra"
                ]
            else:
                cmd = [
                    sys.executable, str(FRAME_PATCHER),
                    current_obj, next_obj,
                    "--old", str(old_size), "--new", str(new_size)
                ]
            if verbose:
                shift_ra_str = " +shift-ra" if step.get('shift_ra') else ""
                print(f"    Post-proc: patch_frame {old_size}->{new_size}{shift_ra_str}")

        elif step_type == 'swap_registers':
            from_reg = step['from_reg']
            to_reg = step['to_reg']
            cmd = [
                sys.executable, str(SWAP_REGISTERS),
                current_obj, next_obj,
                "--from", from_reg, "--to", to_reg
            ]
            if 'exclude_offsets' in step:
                cmd.extend(["--exclude-offsets", step['exclude_offsets']])
            if 'only_offsets' in step:
                cmd.extend(["--only", step['only_offsets']])
            if step.get('bidirectional'):
                cmd.append("--bidirectional")
            excl_str = f" (exclude {step['exclude_offsets']})" if 'exclude_offsets' in step else ""
            only_str = f" (only {step['only_offsets']})" if 'only_offsets' in step else ""
            bidir_str = " (bidirectional)" if step.get('bidirectional') else ""
            if verbose:
                print(f"    Post-proc: swap_registers {from_reg}<->{to_reg}{excl_str}{only_str}{bidir_str}")

        elif step_type == 'swap_registers_liveness':
            address = step['address']
            exe_path = str(ROOT_DIR / "disks" / "SLPS_000.65")
            cmd = [
                sys.executable, str(SWAP_REGISTERS_LIVENESS),
                current_obj, next_obj,
                "--target-exe", exe_path,
                "--address", f"0x{address:08X}"
            ]
            if verbose:
                print(f"    Post-proc: swap_registers_liveness --address 0x{address:08X}")

        elif step_type == 'convert_or_to_addu':
            cmd = [
                sys.executable, str(OR_TO_ADDU_PATCHER),
                current_obj, next_obj
            ]
            if 'exclude_offsets' in step:
                cmd.extend(["--exclude-offsets", step['exclude_offsets']])
            excl_str = f" (exclude {step['exclude_offsets']})" if 'exclude_offsets' in step else ""
            if verbose:
                print(f"    Post-proc: convert_or_to_addu{excl_str}")

        elif step_type == 'swap_instructions':
            offset1 = step['offset1']
            offset2 = step['offset2']
            cmd = [
                sys.executable, str(SWAP_INSTRUCTIONS),
                current_obj, next_obj,
                "--offset1", hex(offset1), "--offset2", hex(offset2)
            ]
            if verbose:
                print(f"    Post-proc: swap_instructions 0x{offset1:x}<->0x{offset2:x}")

        elif step_type == 'patch_ra_offset':
            old_offset = step['old_offset']
            new_offset = step['new_offset']
            cmd = [
                sys.executable, str(PATCH_RA_OFFSET),
                current_obj, next_obj,
                "--old", str(old_offset), "--new", str(new_offset)
            ]
            if verbose:
                print(f"    Post-proc: patch_ra_offset {old_offset}->{new_offset}")

        elif step_type == 'fix_sll_sra_pattern':
            cmd = [
                sys.executable, str(FIX_SLL_SRA_PATTERN),
                current_obj, next_obj
            ]
            if verbose:
                print(f"    Post-proc: fix_sll_sra_pattern")

        elif step_type == 'fix_andi_pattern':
            cmd = [
                sys.executable, str(FIX_ANDI_PATTERN),
                current_obj, next_obj
            ]
            if verbose:
                print(f"    Post-proc: fix_andi_pattern")

        elif step_type == 'strip_div_checks':
            cmd = [
                sys.executable, str(STRIP_DIV_CHECKS),
                current_obj, next_obj
            ]
            if verbose:
                print(f"    Post-proc: strip_div_checks")

        elif step_type == 'strip_interior_nops':
            cmd = [
                sys.executable, str(STRIP_INTERIOR_NOPS),
                current_obj, next_obj
            ]
            if verbose:
                print(f"    Post-proc: strip_interior_nops")

        elif step_type == 'strip_leading_instructions':
            count = step['count']
            cmd = [
                sys.executable, str(STRIP_LEADING_INSTRUCTIONS),
                current_obj, next_obj,
                "--count", str(count)
            ]
            if func_name:
                cmd.extend(["--func", func_name])
            if verbose:
                print(f"    Post-proc: strip_leading_instructions --count {count}")

        elif step_type == 'strip_trailing_instructions':
            count = step['count']
            nop_only = step.get('nop_only', False)
            cmd = [
                sys.executable, str(STRIP_TRAILING_INSTRUCTIONS),
                current_obj, next_obj,
                "--count", str(count)
            ]
            if nop_only:
                cmd.append("--nop-only")
            if verbose:
                nop_str = " --nop-only" if nop_only else ""
                print(f"    Post-proc: strip_trailing_instructions --count {count}{nop_str}")

        elif step_type == 'patch_nor_operands':
            cmd = [
                sys.executable, str(PATCH_NOR_OPERANDS),
                current_obj, next_obj
            ]
            if verbose:
                print(f"    Post-proc: patch_nor_operands")

        elif step_type == 'convert_ori_to_addiu':
            offsets = step.get('offsets', [])
            cmd = [
                sys.executable, str(CONVERT_ORI_TO_ADDIU),
                current_obj, next_obj
            ]
            if offsets:
                cmd.extend(["--offsets"] + [hex(o) for o in offsets])
            if verbose:
                off_str = ' '.join(hex(o) for o in offsets) if offsets else 'all LO16'
                print(f"    Post-proc: convert_ori_to_addiu ({off_str})")

        elif step_type == 'scheduler_normalizer':
            exe_path = str(ROOT_DIR / "disks" / "SLPS_000.65")
            sched_addr = step.get('address', address)
            cmd = [
                sys.executable, str(SCHEDULER_NORMALIZER),
                current_obj,
                "--target", exe_path,
                "--address", f"0x{sched_addr:08X}",
                "-o", next_obj
            ]
            if step.get('block_aware'):
                cmd.append("--block-aware")
            if verbose:
                ba_tag = " --block-aware" if step.get('block_aware') else ""
                print(f"    Post-proc: scheduler_normalizer --address 0x{sched_addr:08X}{ba_tag}")

        elif step_type == 'fill_delay_slots':
            exe_path = str(ROOT_DIR / "disks" / "SLPS_000.65")
            cmd = [
                sys.executable, str(FILL_DELAY_SLOTS),
                current_obj, next_obj,
                "--target-exe", exe_path,
                "--address", f"0x{address:08X}"
            ]
            if verbose:
                print(f"    Post-proc: fill_delay_slots --address 0x{address:08X}")

        elif step_type == 'fill_gcc_delay_slots':
            cmd = [
                sys.executable, str(FILL_GCC_DELAY_SLOTS),
                current_obj, next_obj,
            ]
            for flag in ('fill_jalr', 'fill_epilogue', 'fill_j',
                         'fill_jal_store', 'fill_branch_store', 'fill_prev', 'fill_earliest', 'fill_load_delay', 'fill_all'):
                if step.get(flag):
                    cmd.append('--' + flag.replace('_', '-'))
            if verbose:
                flags = ' '.join('--' + f.replace('_', '-') for f in
                                 ('fill_jalr', 'fill_epilogue', 'fill_j',
                                  'fill_jal_store', 'fill_all')
                                 if step.get(f))
                print(f"    Post-proc: fill_gcc_delay_slots {flags}")

        elif step_type == 'swap_lui_addr_form':
            cmd = [
                sys.executable, str(SWAP_LUI_ADDR_FORM),
                current_obj, next_obj,
                "--offset", str(step['offset']),
                "--to", step['to'],
            ]
            if verbose:
                print(f"    Post-proc: swap_lui_addr_form --offset {step['offset']} --to {step['to']}")

        elif step_type == 'swap_registers_liveness_arrow':
            # Arrow format (a0->v1) - use liveness tool with function address
            exe_path = str(ROOT_DIR / "disks" / "SLPS_000.65")
            cmd = [
                sys.executable, str(SWAP_REGISTERS_LIVENESS),
                current_obj, next_obj,
                "--target-exe", exe_path,
                "--address", f"0x{address:08X}"
            ]
            if verbose:
                print(f"    Post-proc: swap_registers_liveness --address 0x{address:08X}")

        elif step_type == 'remap_stack_slots':
            cmd = [
                sys.executable, str(REMAP_STACK_SLOTS),
                current_obj, next_obj,
                "--map", step['map']
            ]
            if step.get('keep_frame'):
                cmd.append("--keep-frame")
            if verbose:
                print(f"    Post-proc: remap_stack_slots --map '{step['map']}'"
                      f"{' --keep-frame' if step.get('keep_frame') else ''}")

        elif step_type == 'rename_register_chain':
            chains = step['chains']
            cmd = [
                sys.executable, str(RENAME_REGISTER_CHAIN),
                current_obj, next_obj
            ]
            for chain in chains:
                cmd.extend(["--chain", chain])
            if verbose:
                print(f"    Post-proc: rename_register_chain {' '.join(chains)}")

        elif step_type == 'patch_store_zero':
            offset = step['offset']
            register = step['register']
            cmd = [
                sys.executable, str(PATCH_STORE_ZERO),
                current_obj, next_obj,
                "--offset", hex(offset), "--register", register
            ]
            if verbose:
                print(f"    Post-proc: patch_store_zero at 0x{offset:x} reg={register}")

        elif step_type == 'add_leaf_frame':
            frame_size = step.get('frame_size', 8)
            cmd = [
                sys.executable, str(ADD_LEAF_FRAME),
                current_obj, next_obj,
                "--frame-size", str(frame_size)
            ]
            if verbose:
                print(f"    Post-proc: add_leaf_frame --frame-size {frame_size}")

        elif step_type == 'insert_move_and_shift':
            offset = step.get('offset', 0)
            move_reg = step.get('move_reg', 'v1')
            src_reg = step.get('src_reg', 'a0')
            cmd = [
                sys.executable, str(INSERT_MOVE_AND_SHIFT),
                current_obj, next_obj,
                "--offset", hex(offset),
                "--move-reg", move_reg,
                "--src-reg", src_reg
            ]
            if 'dst_reg' in step:
                cmd.extend(["--dst-reg", step['dst_reg']])
            if verbose:
                print(f"    Post-proc: insert_move_and_shift at 0x{offset:x} move={move_reg} src={src_reg}")

        elif step_type == 'insert_instruction_and_shift':
            offset = step.get('offset', 0)
            insn = step.get('insn', 0)
            grow = step.get('grow', False)
            cmd = [
                sys.executable, str(INSERT_INSTRUCTION_AND_SHIFT),
                current_obj, next_obj,
                "--offset", hex(offset),
                "--insn", hex(insn)
            ]
            if grow:
                cmd.append("--grow")
            fix_branches = step.get('fix_branches', False)
            if fix_branches:
                cmd.append("--fix-branches")
            if verbose:
                grow_str = " --grow" if grow else ""
                fb_str = " --fix-branches" if fix_branches else ""
                print(f"    Post-proc: insert_instruction_and_shift at 0x{offset:x} insn=0x{insn:08x}{grow_str}{fb_str}")

        elif step_type == 'strip_arg_homes':
            offsets = step['offsets']
            shrink = step.get('shrink', False)
            cmd = [
                sys.executable, str(STRIP_ARG_HOMES),
                current_obj, next_obj,
                "--offsets"
            ] + [hex(o) for o in offsets]
            if shrink:
                cmd.append("--shrink")
            if verbose:
                off_str = ' '.join(hex(o) for o in offsets)
                shrink_str = " --shrink" if shrink else ""
                print(f"    Post-proc: strip_arg_homes --offsets {off_str}{shrink_str}")

        elif step_type == 'propagate_stack_to_reg':
            sw_off = step['sw_offset']
            cmd = [
                sys.executable, str(PROPAGATE_STACK_TO_REG),
                current_obj, next_obj,
                "--sw-offset", hex(sw_off)
            ]
            if 'lw_offset' in step:
                cmd.extend(["--lw-offset", hex(step['lw_offset'])])
            if 'dst_reg' in step:
                cmd.extend(["--dst-reg", step['dst_reg']])
            if 'src_reg' in step:
                cmd.extend(["--src-reg", step['src_reg']])
            if step.get('addu'):
                cmd.append("--addu")
            if verbose:
                lw_str = f" lw@0x{step['lw_offset']:x}" if 'lw_offset' in step else ""
                dst_str = f" dst={step['dst_reg']}" if 'dst_reg' in step else ""
                src_str = f" src={step['src_reg']}" if 'src_reg' in step else ""
                print(f"    Post-proc: propagate_stack_to_reg sw@0x{sw_off:x}{lw_str}{dst_str}{src_str}")

        elif step_type == 'patch_load_signedness':
            offsets = step['offsets']
            cmd = [
                sys.executable, str(PATCH_LOAD_SIGNEDNESS),
                current_obj, next_obj,
                "--offsets"
            ] + [hex(o) for o in offsets]
            if verbose:
                off_str = ' '.join(hex(o) for o in offsets)
                print(f"    Post-proc: patch_load_signedness --offsets {off_str}")

        elif step_type == 'shift_stack_offsets':
            old_size = step['old_size']
            new_size = step['new_size']
            cmd = [
                sys.executable, str(SHIFT_STACK_OFFSETS),
                current_obj, next_obj,
                "--old", str(old_size), "--new", str(new_size)
            ]
            if verbose:
                print(f"    Post-proc: shift_stack_offsets {old_size}->{new_size}")

        elif step_type == 'unfill_branch_delay':
            offset = step['offset']
            cmd = [
                sys.executable, str(UNFILL_BRANCH_DELAY),
                current_obj, next_obj,
                "--offset", hex(offset)
            ]
            if verbose:
                print(f"    Post-proc: unfill_branch_delay --offset 0x{offset:x}")

        elif step_type == 'rebase_store_to_sp':
            offset = step['offset']
            base_reg = step['base_reg']
            base_delta = step['base_delta']
            cmd = [
                sys.executable, str(REBASE_STORE_TO_SP),
                current_obj, next_obj,
                "--offset", hex(offset),
                "--base-reg", base_reg,
                "--base-delta", str(base_delta)
            ]
            if verbose:
                print(f"    Post-proc: rebase_store_to_sp --offset 0x{offset:x} --base-reg {base_reg} --base-delta {base_delta}")

        elif step_type == 'swap_branch_operands':
            offsets = step['offsets']
            cmd = [
                sys.executable, str(SWAP_BRANCH_OPERANDS),
                current_obj, next_obj,
                "--offsets"
            ] + [hex(o) for o in offsets]
            if verbose:
                off_str = ' '.join(hex(o) for o in offsets)
                print(f"    Post-proc: swap_branch_operands --offsets {off_str}")

        elif step_type == 'invert_branch':
            offsets = step['offsets']
            cmd = [
                sys.executable, str(INVERT_BRANCH),
                current_obj, next_obj,
                "--offsets"
            ] + [hex(o) for o in offsets]
            if verbose:
                off_str = ' '.join(hex(o) for o in offsets)
                print(f"    Post-proc: invert_branch --offsets {off_str}")

        elif step_type == 'fix_prologue_and_delays':
            cmd = [
                sys.executable, str(FIX_PROLOGUE_AND_DELAYS),
                current_obj, next_obj
            ]
            if verbose:
                print(f"    Post-proc: fix_prologue_and_delays")

        elif step_type == 'collapse_shift_move':
            cmd = [sys.executable, str(COLLAPSE_SHIFT_MOVE), current_obj, next_obj]
            if 'old_ra_offset' in step and 'new_ra_offset' in step:
                cmd.extend(["--ra-offset", str(step['old_ra_offset']), str(step['new_ra_offset'])])
            if verbose:
                ra_str = f" --ra-offset {step.get('old_ra_offset')} {step.get('new_ra_offset')}" if 'old_ra_offset' in step else ""
                print(f"    Post-proc: collapse_shift_move{ra_str}")

        elif step_type == 'strip_instructions':
            nop_offsets = step.get('nop_offsets', [])
            remove_offsets = step.get('remove_offsets', [])
            cmd = [sys.executable, str(STRIP_INSTRUCTIONS), current_obj, next_obj]
            if nop_offsets:
                cmd.extend(["--nop", ','.join(hex(o) for o in nop_offsets)])
            if remove_offsets:
                cmd.extend(["--remove", ','.join(hex(o) for o in remove_offsets)])
            if verbose:
                nop_str = f" --nop {','.join(hex(o) for o in nop_offsets)}" if nop_offsets else ""
                rem_str = f" --remove {','.join(hex(o) for o in remove_offsets)}" if remove_offsets else ""
                print(f"    Post-proc: strip_instructions{nop_str}{rem_str}")

        elif step_type == 'swap_commutative_operands':
            offsets = step['offsets']
            cmd = [
                sys.executable, str(SWAP_COMMUTATIVE_OPERANDS),
                current_obj, next_obj
            ]
            for off in offsets:
                cmd.extend(["--offset", hex(off)])
            if verbose:
                off_str = ' '.join(hex(o) for o in offsets)
                print(f"    Post-proc: swap_commutative_operands --offset {off_str}")

        elif step_type == 'adjust_branch_offset':
            offset = step['offset']
            imm = step['imm']
            cmd = [
                sys.executable, str(ADJUST_BRANCH_OFFSET),
                current_obj, next_obj,
                "--offset", hex(offset), "--imm", hex(imm)
            ]
            if verbose:
                print(f"    Post-proc: adjust_branch_offset --offset 0x{offset:x} --imm 0x{imm:x}")

        elif step_type == 'rotate_dispatch':
            move_from = step['move_from']
            move_to = step['move_to']
            cmd = [
                sys.executable, str(ROTATE_DISPATCH),
                current_obj, next_obj,
                "--move-from", hex(move_from), "--move-to", hex(move_to)
            ]
            if 'fix_beq' in step:
                cmd.extend(["--fix-beq", hex(step['fix_beq'])])
            if 'add_nop' in step:
                cmd.extend(["--add-nop", hex(step['add_nop'])])
            if verbose:
                fix_str = f" --fix-beq 0x{step['fix_beq']:x}" if 'fix_beq' in step else ""
                nop_str = f" --add-nop 0x{step['add_nop']:x}" if 'add_nop' in step else ""
                print(f"    Post-proc: rotate_dispatch 0x{move_from:x}->0x{move_to:x}{fix_str}{nop_str}")

        elif step_type == 'patch_frame_full':
            old_size = step['old_size']
            new_size = step['new_size']
            cmd = [
                sys.executable, str(PATCH_FRAME_FULL),
                current_obj, next_obj,
                "--old", str(old_size), "--new", str(new_size)
            ]
            if verbose:
                print(f"    Post-proc: patch_frame_full {old_size}->{new_size}")

        elif step_type == 'patch_insn_reg_field':
            patches = step['patches']
            cmd = [
                sys.executable, str(PATCH_INSN_REG_FIELD),
                current_obj, next_obj
            ]
            for patch in patches:
                cmd.extend(["--patch", patch])
            if verbose:
                print(f"    Post-proc: patch_insn_reg_field {' '.join(patches)}")

        elif step_type == 'cse_lui_at':
            cmd = [
                sys.executable, str(CSE_LUI_AT),
                current_obj, next_obj
            ]
            if step.get('same_symbol_only'):
                cmd.append("--same-symbol-only")
            if step.get('offsets'):
                cmd += ["--offsets", ",".join(hex(o) for o in step['offsets'])]
            if verbose:
                print(f"    Post-proc: cse_lui_at {' '.join(cmd[4:])}".rstrip())

        elif step_type == 'maspsx_noop':
            # maspsx is already handled by gccwrap.py pipeline
            if verbose:
                print(f"    Post-proc: maspsx (handled by gccwrap, skipping)")
            continue

        elif step_type == 'fix_sign_test_pattern':
            dest_reg = step.get('dest_reg', 't8')
            cmd = [
                sys.executable, str(FIX_SIGN_TEST_PATTERN),
                current_obj, next_obj,
                "--dest-reg", dest_reg
            ]
            if verbose:
                print(f"    Post-proc: fix_sign_test_pattern --dest-reg {dest_reg}")

        elif step_type == 'prepend_thunk':
            prepend_tool = ROOT_DIR / "tools" / "ido" / "prepend_thunk.py"
            cmd = [
                sys.executable, str(prepend_tool),
                current_obj, next_obj,
                "--insns"
            ] + step['insns']
            if verbose:
                print(f"    Post-proc: prepend_thunk --insns {' '.join(step['insns'])}")

        elif step_type == 'custom_script':
            # 2026-10-03 (red-team finding 1): `ROOT_DIR / "tools/ido" / X`
            # IS X when X is absolute, so an out-of-tree script ran.  The
            # banned-step check at the top of this function refuses it; this
            # is the belt to that brace.
            from banned_postproc import script_path_violation
            _why = script_path_violation(step)
            if _why:
                print(f"⛔ BANNED: custom_script {step['script']} for {func_name}: {_why}",
                      file=sys.stderr)
                sys.exit(1)
            script_path = ROOT_DIR / "tools" / "ido" / step['script']
            # A custom_script that writes hardcoded target instruction words is
            # patch_insn_word in disguise — refuse it (project policy).
            from banned_postproc import is_painting_script
            if is_painting_script(script_path):
                print(f"⛔ BANNED: custom_script {step['script']} writes hardcoded "
                      f"instruction words (patch_insn_word in disguise) for {func_name}",
                      file=sys.stderr)
                sys.exit(1)
            extra_args = step.get('extra_args', [])
            cmd = [
                sys.executable, str(script_path),
                current_obj, next_obj
            ] + extra_args
            if verbose:
                print(f"    Post-proc: custom_script {step['script']} {' '.join(extra_args)}")

        elif step_type == 'manual':
            if verbose:
                print(f"    WARNING: Manual post-processing required: {step.get('description', 'unknown')}")
            # Skip manual steps - these need manual handling
            continue
        else:
            if verbose:
                print(f"    WARNING: Unknown post-processing type: {step_type}")
            continue

        # Run the command
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode != 0:
            print(f"    WARNING: Post-processing step failed: {step_type}")
            print(f"    {result.stderr}")
            # Don't fail completely, just skip this step
            continue

        current_obj = next_obj

    return current_obj


def compile_c_function(c_path, func_name, func_addr, verbose=False,
                       postproc_steps=None, extra_flags=None, compiler="ido52",
                       compile_env=None):
    """Compile a C function with IDO 5.2 or PSY-Q GCC and apply post-processing.

    Args:
        c_path: Path to C source file
        func_name: Function name for logging
        func_addr: Function address (for determining region)
        verbose: Enable verbose output
        postproc_steps: List of post-processing steps from parse_postproc_header
        extra_flags: List of extra compiler flags from parse_compile_flags
        compiler: Compiler identifier ("ido52", "gcc260", "gcc263", "gcc263p",
                  "gcc272", "gcc281",
                  "ido52-aspsx", "ido52-gas")
        compile_env: Dict of extra environment variables for compilation
    """
    if not os.path.exists(c_path):
        raise FileNotFoundError(f"C source not found: {c_path}")

    # Determine region
    is_gcc_region = func_addr < 0x80086000

    with tempfile.TemporaryDirectory() as tmpdir:
        obj_path = os.path.join(tmpdir, "func.o")
        bin_path = os.path.join(tmpdir, "func.bin")

        if compiler.startswith("gcc") or compiler in ("ido52-aspsx", "ido52-gas"):
            # GCC compilation via gccwrap.py -- or the hybrid: IDO 5.2 codegen
            # assembled PSY-Q style (maspsx+as, or GNU as reorder mode).
            cmd = [
                sys.executable, str(GCCWRAP),
                str(c_path),
                "-o", obj_path,
            ]
            if compiler.startswith("gcc"):
                version_number = compiler[3:]  # "gcc260" -> "260"
                cmd += ["--gcc-version", version_number]
            else:
                version_number = compiler
                cmd += ["--codegen", "ido52",
                        "--assembler", "gas" if compiler == "ido52-gas" else "maspsx"]
            # Separate gccwrap flags from cc1 flags
            gccwrap_flags = {"--ido-fill-load-delay", "--ido-b-store-macro-slot", "--ido-fill-macro-lui", "--ido-branch-zero-imm", "--ido-jr-store-slot", "--ido-fill-at-next", "--ido-copy-dead-ft", "--ido-b-store-slot", "--ido-fill-ptr-bump", "--ido-fill-branch-hop", "--ido-copy-and-bump", "--ido-div-macro-residue", "--ido-hoist-load-above", "--ido-label-load-nop", "--fill-jalr", "--fill-epilogue", "--fill-j", "--mflo-nops-first", "--div-result-no-nop", "--load-base-nop", "--delay-slot-load-nop", "--label-load-nop", "--loc-load-nop", "--konami-lib-sched", "--macro-lui-fill", "--macro-lui-fill-far", "--macro-lui-fill-cross", "--load-dest-temp", "--jal-load-nop", "--store-macro-split", "--fill-branch-store", "--fill-prev", "--fill-earliest", "--fill-load-delay",
                             "--expand-div", "--defer-div", "--fill-sp-adjust", "--div-mf-with-check", "--mflo-div-writer-slot", "--sink-sp-adjust", "--mflo-noreorder-nops", "--mflo-consumer-fill", "--macro-lui-fill-split",
                             "--fill-jal-store", "--fill-all", "--fill-ptr-bump-down", "--reuse-at-same-hi",
                             "--defer-div-check-first-on-slot-read", "--fill-load-delay-macro", "--fill-load-delay-no-div",
                             "--no-delay-slots", "--no-cross-jumping"}
            cc1_flags = []
            if extra_flags:
                for flag in extra_flags:
                    if flag in gccwrap_flags or flag.startswith("--mflo-hoist-skip=") or flag.startswith("--store-macro-split-window="):
                        cmd.append(flag)
                    else:
                        cc1_flags.append(flag)
                if cc1_flags:
                    cmd.append("--extra-flags=" + " ".join(cc1_flags))
            if verbose:
                print(f"    Compiling (GCC {version_number}): {' '.join(cmd)}")
        else:
            # IDO 5.2 or 5.3 compilation (default: 5.2)
            # Base flags: -O2 [extra_flags] -G0 -mips1 -EL -non_shared
            cc_bin = IDO_CC_53 if compiler == "ido53" else IDO_CC
            cmd = [
                str(cc_bin),
                "-O2",
            ]
            if extra_flags:
                cmd.extend(extra_flags)
            cmd.extend([
                "-G0", "-mips1", "-EL", "-non_shared",
                "-c", "-I", str(ROOT_DIR / "include"),
                str(c_path),
                "-o", obj_path
            ])
            if verbose:
                print(f"    Compiling: {' '.join(cmd)}")

        # Build environment with any per-function env vars
        run_env = None
        if compile_env:
            run_env = os.environ.copy()
            run_env.update(compile_env)
            if verbose:
                for k, v in compile_env.items():
                    print(f"    Env: {k}={v}")

        result = subprocess.run(cmd, capture_output=True, text=True, cwd=str(ROOT_DIR),
                                env=run_env)
        if result.returncode != 0:
            raise RuntimeError(f"Compilation failed for {func_name}:\n{result.stderr}")

        current_obj = obj_path

        # Apply default region-specific processing if no custom post-processing
        if not postproc_steps:
            default_step = os.path.join(tmpdir, "func_default.o")

            if is_gcc_region:
                # GCC region: Default frame patch 24->40
                cmd = [
                    sys.executable, str(FRAME_PATCHER),
                    current_obj, default_step,
                    "--old", "24", "--new", "40"
                ]
                if verbose:
                    print(f"    Default: patch_frame 24->40 (GCC region)")

                result = subprocess.run(cmd, capture_output=True, text=True)
                if result.returncode != 0:
                    raise RuntimeError(f"Frame patching failed for {func_name}:\n{result.stderr}")
                current_obj = default_step
            else:
                # IDO region: Convert OR->ADDU
                cmd = [
                    sys.executable, str(OR_TO_ADDU_PATCHER),
                    current_obj, default_step
                ]
                if verbose:
                    print(f"    Default: convert_or_to_addu (IDO region)")

                result = subprocess.run(cmd, capture_output=True, text=True)
                if result.returncode != 0:
                    raise RuntimeError(f"OR->ADDU conversion failed for {func_name}:\n{result.stderr}")

                if verbose and "Converted" in result.stdout:
                    print(f"    {result.stdout.strip()}")

                current_obj = default_step
        else:
            # Apply custom post-processing steps
            current_obj = apply_postprocessing(current_obj, postproc_steps, func_name, verbose, address=func_addr)

        # Keep the FINAL (post-PostProc) object for positional relocation audits
        if os.environ.get('L3_KEEP_OBJ'):
            shutil.copy(current_obj, os.environ['L3_KEEP_OBJ'])

        # The REAL relocation records of the final object (and of the raw
        # compile, to measure how many leading words PostProc stripped).
        # fixup_relocations rewrites ONLY these fields -- see its docstring.
        global LAST_OBJ_INFO
        LAST_OBJ_INFO = read_object_info(current_obj, raw_obj=obj_path)
        LAST_OBJ_INFO['func_addr'] = func_addr

        # Extract .text section
        cmd = [
            "mipsel-linux-gnu-objcopy",
            "-O", "binary",
            "-j", ".text",
            current_obj,
            bin_path
        ]
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode != 0:
            raise RuntimeError(f"objcopy failed for {func_name}:\n{result.stderr}")

        # Read binary
        with open(bin_path, "rb") as f:
            func_bytes = f.read()

        return func_bytes


# ── FIXUP OVER-MASKING AUDIT ──────────────────────────────────────────────
# fixup_relocations copies the TARGET's immediate into any compiled I-type word
# whose opcode + top-16 bits match. At a genuine relocation site that is
# correct (the linker, not the compiler, writes those bits). Everywhere else it
# silently CORRECTS the compiler: a wrong `sw $ra,0x18($sp)` frame offset or a
# wrong branch displacement is repaired on the way into the build, so a real
# codegen bug never shows up as a mismatch. These accumulate here so the
# masking is visible; `--strict-fixup-audit` prints them per word.
NONRELOC_FIXUPS = []          # list of dicts, appended by fixup_relocations

# I-type opcodes whose imm16 can legitimately be an R_MIPS_LO16 field.
_LO16_OPS = frozenset((9, 13,                      # addiu, ori
                       32, 33, 34, 35, 36, 37, 38,  # lb lh lwl lw lbu lhu lwr
                       40, 41, 42, 43, 46,          # sb sh swl sw swr
                       49, 53, 57, 61))             # lwc1 ldc1 swc1 sdc1

_OPNAMES = {2: 'j', 3: 'jal', 4: 'beq', 5: 'bne', 9: 'addiu', 10: 'slti',
            11: 'sltiu', 12: 'andi', 13: 'ori', 14: 'xori', 15: 'lui',
            32: 'lb', 33: 'lh', 35: 'lw', 36: 'lbu', 37: 'lhu',
            40: 'sb', 41: 'sh', 43: 'sw'}

_REGNAMES = ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
             't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
             's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
             't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra']


def _itype_dest(word):
    """Register written by an I-type/R-type word, or None (audit helper)."""
    op = (word >> 26) & 0x3f
    if word == 0:
        return None
    if op == 0:
        funct = word & 0x3f
        if funct in (0x08, 0x0c, 0x0d, 0x11, 0x13, 0x18, 0x19, 0x1a, 0x1b):
            return None
        return (word >> 11) & 0x1f
    if op == 3:
        return 31
    if op in (2, 4, 5, 6, 7, 20, 21, 22, 23) or op == 1:
        return None
    if op in _LO16_OPS or op in (10, 11, 12, 14, 15):
        if op in (40, 41, 42, 43, 46, 57, 61):      # stores write no register
            return None
        return (word >> 16) & 0x1f
    return None


def _branch_target(i, w):
    """Word index a branch/jump at i transfers to, or None (jr/jalr/none)."""
    op = (w >> 26) & 0x3f
    if op in (4, 5, 6, 7, 20, 21, 22, 23) or (op == 1 and ((w >> 16) & 0x1f) in (0, 1, 16, 17)):
        disp = w & 0xffff
        if disp & 0x8000:
            disp -= 0x10000
        return i + 1 + disp
    return None


def _is_unconditional(w):
    op = (w >> 26) & 0x3f
    if op == 2:                                   # j
        return True
    if op == 4 and ((w >> 21) & 0x1f) == ((w >> 16) & 0x1f):   # beq r,r == b
        return True
    if op == 0 and (w & 0x3f) == 0x08:            # jr
        return True
    return False


def reloc_site_indices(words):
    """Word indices whose immediate field is a genuine LINKER relocation.

    R_MIPS_26   -- j / jal target
    R_MIPS_HI16 -- lui of an address
    R_MIPS_LO16 -- the addiu/ori/load/store that CONSUMES a %hi register

    Everything else is compiler output: frame offsets off $sp/$fp, branch
    displacements, ALU immediates on non-address registers. If one of those
    differs from the target, that is a real codegen difference — copying the
    target's bits over it is masking, not relocation.

    2026-09-16: the %hi-register state is propagated along the CFG (fall-
    through AND branch edges, delay slots included) to a fixpoint, not just
    linearly. as1 puts the `lui` for a block's first global access in the
    branch delay slot that reaches it (`beq ..; lui $a0,%hi(S)` at the top of
    a dispatcher, consumed by `lw $a0,%lo(S)($a0)` at the case body after
    another case has overwritten $a0 in between) -- the linear scan called
    that consumer a non-relocation site and the fixup was "copied".
    """
    n = len(words)
    branch_of = {}                       # slot index -> (target, fallthrough?)
    for i, w in enumerate(words):
        t = _branch_target(i, w)
        if t is not None:
            branch_of[i + 1] = (t, True)

    def step(state, i, w, sites):
        """Apply word i to a copy of state (reg -> lui index); record sites."""
        st = dict(state)
        op = (w >> 26) & 0x3f
        rs = (w >> 21) & 0x1f
        rt = (w >> 16) & 0x1f
        if (op in _LO16_OPS and (w & 0xffff) == 0
                and rs not in (0, 28, 29, 30)):
            sites.add(i)
        if op in (2, 3):                     # j / jal
            sites.add(i)
        elif op == 15:                       # lui
            sites.add(i)
            st[rt] = i
            return st
        elif op in _LO16_OPS and rs in st:   # the %lo partner
            sites.add(i)
            if rs != 1:
                del st[rs]                   # pair consumed ($at shared: 2026-09-03)
        elif op == 0 and (w & 0x3f) in (0x20, 0x21, 0x23, 0x25) and (
                rs in st or rt in st):
            src = rs if rs in st else rt
            st[(w >> 11) & 0x1f] = st[src]
            return st
        d = _itype_dest(w)
        if d is not None and d in st:
            del st[d]
        return st

    IN = [None] * (n + 1)
    IN[0] = {}
    sites = set()
    changed = True
    while changed:
        changed = False
        for i in range(n):
            if IN[i] is None:
                continue
            out = step(IN[i], i, words[i], sites)
            # The fall-through edge is ALWAYS kept (even after b/j/jr): jr-
            # table case bodies have no static predecessor, and the linear
            # order is what the pre-2026-09-16 scan followed, so the result
            # stays a superset of it. Branch edges only ADD %hi state.
            succs = [i + 1]
            if i in branch_of:
                t, fall = branch_of[i]
                if t is not None and 0 <= t < n:
                    succs.append(t)
            for s_ in succs:
                if s_ > n:
                    continue
                if IN[s_] is None:
                    IN[s_] = dict(out); changed = True
                else:
                    merged = dict(IN[s_])
                    for k, v in out.items():
                        if k not in merged:
                            merged[k] = v
                    if merged != IN[s_]:
                        IN[s_] = merged; changed = True
    return sites


def _fmt_word(word):
    op = (word >> 26) & 0x3f
    name = _OPNAMES.get(op, f'op{op}')
    rs = _REGNAMES[(word >> 21) & 0x1f]
    rt = _REGNAMES[(word >> 16) & 0x1f]
    imm = word & 0xffff
    if imm & 0x8000:
        imm -= 0x10000
    return f'{name} ${rt},{imm}(${rs})'


# ── RELOCATION-RECORD FIXUP (2026-10-03) ──────────────────────────────────
# Until 2026-10-03 fixup_relocations copied the retail word over ANY compiled
# j/jal and ANY lui with the same rt -- raw constants included -- and the
# imm16 of any same-opcode I-type word, reporting only some of it.  So a raw
# cast `*(u8 *)0x800EE760` that compiled to the wrong lui/addiu pair
# (main icon_disp_switch) still printed MATCH, a misnamed jal target printed
# MATCH, and a jump table in .rodata was never looked at.
#
# Now the fixup is driven by the compiled object's OWN relocation records
# (read_object_info): it rewrites a field only where the object carries an
# R_MIPS_26 / R_MIPS_HI16 / R_MIPS_LO16 record, only when opcode and register
# fields already agree, and only after checking that the symbol the C NAMED
# resolves to the address the retail word encodes (the audit_main_relocs.py
# name tables).  Any other difference is left in place -- a mismatch.  A
# reference into the TU's own .rodata/.data is checked by CONTENT against
# retail at the address the retail hi/lo pair encodes, jump tables entry by
# entry against the function's own labels.  Every refusal is recorded in
# RELOC_ERRORS (or the caller's `errors` list) with its reason.
LAST_OBJ_INFO = None          # set by compile_c_function (this process)
RELOC_ERRORS = []             # appended by fixup_relocations
_NAME_TABLE = None

R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16 = 2, 4, 5, 6


def _name_table():
    global _NAME_TABLE
    if _NAME_TABLE is None:
        sys.path.insert(0, str(ROOT_DIR / 'tools' / 'analysis'))
        import audit_main_relocs as _amr
        _NAME_TABLE = (_amr, _amr.load_symtab())
    return _NAME_TABLE


def resolve_symbol_name(name):
    """Absolute address a symbol NAME denotes (audit_main_relocs tables)."""
    amr, tab = _name_table()
    return amr.sym_addr(name, tab)


_MDEBUG_SC_SECTION = {2: '.data', 3: '.bss', 13: '.sdata', 14: '.sbss',
                      15: '.rodata'}


def _mdebug_static_symbols(elf):
    """File-static data symbols from an IDO .mdebug (ECOFF symbolic header).

    IDO keeps `static` objects out of the ELF .symtab -- their relocations
    are against the section symbol -- but records them, with their section
    offset, as stStatic local symbols in .mdebug.  Returns
    {section: [[value, 0, name], ...]}.
    """
    out = {}
    sec = elf.get_section_by_name('.mdebug')
    if sec is None:
        return out
    stream = elf.stream
    hoff = sec['sh_offset']
    stream.seek(hoff)
    h = stream.read(96)
    if len(h) < 96:
        return out
    # IDO writes the symbolic header BIG-endian even for -EL objects
    if struct.unpack_from('<H', h, 0)[0] == 0x7009:
        e = '<'
    elif struct.unpack_from('>H', h, 0)[0] == 0x7009:
        e = '>'
    else:
        return out
    f = struct.unpack_from(e + 'HH23I', h, 0)
    isym_max, cb_sym = f[9], f[10]
    cb_ss = f[16]
    ifd_max, cb_fd = f[19], f[20]
    stream.seek(cb_fd)
    fds = stream.read(72 * ifd_max)
    stream.seek(cb_sym)
    syms = stream.read(12 * isym_max)
    for k in range(ifd_max):
        fd = struct.unpack_from(e + '6I', fds, 72 * k)
        iss_base, isym_base, csym = fd[2], fd[4], fd[5]
        for j in range(isym_base, isym_base + csym):
            if 12 * j + 12 > len(syms):
                break
            iss, value, bits = struct.unpack_from(e + '3I', syms, 12 * j)
            if e == '>':
                st, sc = bits >> 26, (bits >> 21) & 0x1f
            else:
                st, sc = bits & 0x3f, (bits >> 6) & 0x1f
            if st != 2 or sc not in _MDEBUG_SC_SECTION:
                continue
            stream.seek(cb_ss + iss_base + iss)
            name = stream.read(128).split(b'\0', 1)[0].decode('latin-1')
            if name:
                out.setdefault(_MDEBUG_SC_SECTION[sc], []).append([value, 0, name])
    return out


def read_object_info(obj_path, raw_obj=None):
    """Relocation records + data sections of a compiled object (JSON-safe).

    reloc = [offset, type, symname, kind, secname, symvalue]
      kind: 'undef' (external, resolved by NAME), 'section' (STT_SECTION),
            'local' (a symbol defined in this object: secname + symvalue)
    """
    from elftools.elf.elffile import ELFFile

    def _read(path):
        with open(path, 'rb') as fh:
            elf = ELFFile(fh)
            symtab = elf.get_section_by_name('.symtab')
            secnames = {i: s.name for i, s in enumerate(elf.iter_sections())}
            out = {'relocs': {}, 'secs': {}, 'text_len': 0, 'funcs': {},
                   'datasyms': {}}
            for sym in (symtab.iter_symbols() if symtab is not None else []):
                shn = sym['st_shndx']
                if not isinstance(shn, int) or shn == 0:
                    continue
                sn = secnames.get(shn, '?')
                if sym['st_info']['type'] == 'STT_FUNC' and sn == '.text':
                    out['funcs'][sym.name] = sym['st_value']
                elif sym['st_info']['type'] in ('STT_OBJECT', 'STT_NOTYPE') \
                        and sn in ('.data', '.bss', '.rodata', '.rdata',
                                   '.sdata', '.sbss') and sym.name:
                    out['datasyms'].setdefault(sn, []).append(
                        [sym['st_value'], sym['st_size'], sym.name])
            try:
                for sn, lst in _mdebug_static_symbols(elf).items():
                    have = {x[0] for x in out['datasyms'].get(sn, [])}
                    out['datasyms'].setdefault(sn, []).extend(
                        x for x in lst if x[0] not in have)
            except Exception:
                pass
            text = elf.get_section_by_name('.text')
            if text is not None:
                out['text_len'] = text['sh_size']
            for sec in elf.iter_sections():
                if sec['sh_type'] not in ('SHT_REL', 'SHT_RELA'):
                    continue
                target = secnames.get(sec['sh_info'], '?')
                lst = []
                for r in sec.iter_relocations():
                    sym = symtab.get_symbol(r['r_info_sym'])
                    shn = sym['st_shndx']
                    if shn == 'SHN_UNDEF' or shn == 0:
                        kind, sname = 'undef', None
                    elif isinstance(shn, int):
                        sname = secnames.get(shn, '?')
                        kind = 'section' if sym['st_info']['type'] == 'STT_SECTION' else 'local'
                    else:
                        kind, sname = 'abs', None
                    lst.append([r['r_offset'], r['r_info_type'], sym.name,
                                kind, sname, sym['st_value']])
                lst.sort(key=lambda x: x[0])
                out['relocs'][target] = lst
            for name in ('.rodata', '.data', '.rdata', '.sdata'):
                s = elf.get_section_by_name(name)
                if s is not None and s['sh_type'] != 'SHT_NOBITS':
                    out['secs'][name] = s.data().hex()
            return out

    info = _read(obj_path)
    info['strip'] = 0
    if raw_obj and os.path.exists(raw_obj) and raw_obj != obj_path:
        try:
            raw = _read(raw_obj)
        except Exception:
            raw = None
        if raw is not None:
            # Leading bytes PostProc stripped: strip_leading_instructions
            # shifts .rel.text offsets and symbol values but NOT the in-place
            # addends of section-relative .text references (jump-table
            # entries, GCC local `j` targets), which stay raw .text offsets.
            # Measure it from the function symbols both objects carry (reloc
            # offsets are useless here: swap_instructions moves them).
            common = [(n, raw['funcs'][n] - v) for n, v in info['funcs'].items()
                      if n in raw['funcs']]
            at0 = [d for n, d in common if info['funcs'][n] == 0]
            if at0:
                info['strip'] = max(at0)
            elif common:
                info['strip'] = max(common, key=lambda nd: raw['funcs'][nd[0]])[1]
            else:
                info['strip'] = max(0, raw['text_len'] - info['text_len'])
            info['raw_text_len'] = raw['text_len']
    return info


def _sext16(v):
    v &= 0xffff
    return v - 0x10000 if v & 0x8000 else v


def _rodata_extent_ok(info, secname, off, retail_addr, exe_data, func_addr,
                      size, errors, why, checked):
    """Compare a TU-local data object with retail bytes at retail_addr."""
    key = (secname, off, retail_addr)
    if key in checked:
        return checked[key]
    # A NAMED object defined in this TU (`static s32 D_800EBD10;`): its name
    # must denote the retail address, exactly like an extern.
    # the nearest named object at or below the offset (sizes are unreliable:
    # IDO records `u32 D[2]` as size 4 and some objects as size 0)
    named = None
    for val, sz, nm in sorted(info.get('datasyms', {}).get(secname, [])):
        if val <= off and (secname not in ('.rodata', '.rdata') or sz == 0
                           or off < val + sz):
            named = (val, nm)
    fo = retail_addr - TEXT_LOAD_ADDR + EXE_HEADER_SIZE
    in_image = EXE_HEADER_SIZE <= fo < len(exe_data)
    hexdata = info.get('secs', {}).get(secname)
    if named is not None:
        a = resolve_symbol_name(named[1])
        if a is None or a + (off - named[0]) != retail_addr:
            errors.append(f'{why}: {secname} object {named[1]}+0x{off - named[0]:x}'
                          f' denotes 0x{(a or 0) + off - named[0]:08X} but retail '
                          f'addresses 0x{retail_addr:08X}'
                          + ('' if a is not None else ' (name not address-derived)'))
            checked[key] = False
            return False
        if hexdata is None or not in_image:
            # .bss (or retail .bss): the name check is the whole check, but an
            # initialiser the retail image cannot hold is a real difference
            if hexdata is not None and any(bytes.fromhex(hexdata)[named[0]:named[0] + 4]):
                errors.append(f'{why}: {named[1]} is initialised in the C but '
                              f'retail 0x{retail_addr:08X} is .bss')
                checked[key] = False
                return False
            checked[key] = True
            return True
    if hexdata is None:
        # anonymous .bss: no name and no content to compare
        errors.append(f'{why}: reference into anonymous {secname}+0x{off:x} '
                      f'(retail 0x{retail_addr:08X}) -- no name, no content: '
                      f'unverifiable; name the object D_XXXXXXXX')
        checked[key] = False
        return False
    data = bytes.fromhex(hexdata)
    rel32 = {r[0]: r for r in info.get('relocs', {}).get(secname, [])}
    # extent: up to the next offset any .text reloc references in this section
    refs = sorted({(r[5] if r[3] == 'local' else 0) + r[6]
                   for r in info.get('_secrefs', []) if r[4] == secname})
    end = len(data)
    for o in refs:
        if o > off:
            end = o
            break
    span = data[off:end]
    # trailing zero bytes are usually alignment padding between objects
    n = len(span)
    def _in_reloc(pos):
        return any(ro <= pos < ro + 4 for ro in rel32)
    while n > 0 and span[n - 1] == 0 and not _in_reloc(off + n - 1):
        n -= 1
    if fo < EXE_HEADER_SIZE or fo + n > len(exe_data):
        errors.append(f'{why}: {secname}+0x{off:x} -> retail 0x{retail_addr:08X} '
                      f'is outside the executable image')
        checked[key] = False
        return False
    ok = True
    i = 0
    strip = info.get('strip', 0)
    while i < n:
        o = off + i
        if o in rel32 and i + 4 <= len(span):
            r = rel32[o]
            word = struct.unpack_from('<I', data, o)[0]
            rword = struct.unpack_from('<I', exe_data, fo + i)[0]
            if r[1] != R_MIPS_32:
                errors.append(f'{why}: {secname}+0x{o:x} reloc type {r[1]} not checked')
                ok = False
            elif r[3] == 'undef':
                a = resolve_symbol_name(r[2])
                if a is None or ((a + word) & 0xffffffff) != rword:
                    errors.append(f'{why}: {secname}+0x{o:x} names {r[2]} '
                                  f'(0x{(a or 0) + word:08X}) but retail holds 0x{rword:08X}')
                    ok = False
            elif r[4] == '.text':
                want = (func_addr + r[5] + word if r[3] == 'local'
                        else func_addr - strip + word)
                if want != rword:
                    errors.append(f'{why}: JUMP TABLE {secname}+0x{o:x} -> '
                                  f'0x{want:08X} but retail entry is 0x{rword:08X}')
                    ok = False
            else:
                errors.append(f'{why}: {secname}+0x{o:x} relocated against '
                              f'{r[4]} not checked')
                ok = False
            i += 4
            continue
        if exe_data[fo + i] != span[i]:
            errors.append(f'{why}: {secname}+0x{o:x} byte 0x{span[i]:02x} but '
                          f'retail 0x{retail_addr + i:08X} holds 0x{exe_data[fo + i]:02x}')
            ok = False
            break
        i += 1
    checked[key] = ok
    return ok


def fixup_relocations(func_bytes, func_addr, exe_data, file_offset,
                      func_name=None, audit=None, obj_info=None, errors=None):
    """Write link-time fields into the compiled body -- at REAL relocations only.

    For every R_MIPS_26 / R_MIPS_HI16 / R_MIPS_LO16 record of the compiled
    object that lands in the window, when the compiled and retail words agree
    in opcode and register fields, the retail field is copied IF the symbol
    the C named resolves to the address the retail word encodes:
      * external symbol -> its NAME (D_XXXXXXXX / func_XXXXXXXX / registry /
        main_retail_symbols / landed symbol_names) + addend;
      * this TU's .rodata/.data -> retail bytes at the retail address must
        equal the compiled contents (jump tables: each entry must be the
        function's own label);
      * this TU's .text -> func_addr + offset.
    Nothing else is ever copied: a raw constant `lui`, a branch displacement,
    a frame offset or a non-address immediate that differs stays different
    and the function is a MISMATCH.  Refusals go to `errors` (default
    RELOC_ERRORS) as human-readable strings.

    obj_info: read_object_info() of the final object; defaults to
    LAST_OBJ_INFO when that was produced for this func_addr.  With no
    relocation information at all nothing is copied.
    `audit` is kept for API compatibility: non-relocation words are never
    copied any more, so it stays empty.
    """
    if errors is None:
        errors = RELOC_ERRORS
    if obj_info is None and LAST_OBJ_INFO is not None \
            and LAST_OBJ_INFO.get('func_addr') == func_addr:
        obj_info = LAST_OBJ_INFO
    result = bytearray(func_bytes)
    if obj_info is None:
        errors.append(f'{func_name}: no relocation records available -- '
                      f'nothing copied from retail')
        return bytes(result)
    tag = func_name or f'0x{func_addr:08X}'
    nwords = len(func_bytes) // 4
    cw = [struct.unpack_from('<I', func_bytes, i * 4)[0] for i in range(nwords)]
    tw = [struct.unpack_from('<I', exe_data, file_offset + i * 4)[0]
          for i in range(nwords)]
    trel = [r for r in obj_info.get('relocs', {}).get('.text', [])
            if r[0] // 4 < nwords]
    strip = obj_info.get('strip', 0)
    # section references, with their in-place addend, for extent computation
    secrefs = []
    for r in trel:
        if r[3] in ('section', 'local') and r[4] not in ('.text',) \
                and r[1] == R_MIPS_LO16:
            secrefs.append(r + [_sext16(cw[r[0] // 4])])
    obj_info = dict(obj_info)
    obj_info['_secrefs'] = secrefs
    checked = {}

    def _put(wi, word):
        struct.pack_into('<I', result, wi * 4, word)

    def _target_ok(r, retail_addr, why):
        """Does the symbol reference r (plus addend) denote retail_addr?"""
        kind, sec = r[3], r[4]
        if kind == 'undef':
            a = resolve_symbol_name(r[2])
            if a is None:
                errors.append(f'{tag} [{r[0] // 4}] {why}: UNRESOLVED name {r[2]} '
                              f'(no trusted table maps it; retail 0x{retail_addr:08X})')
                return False
            return a
        if sec == '.text':
            # a defined symbol's value was moved by the strip; a section
            # symbol's in-place addend was not
            return func_addr + r[5] if kind == 'local' else func_addr - strip
        return ('data', sec, r[5] if kind == 'local' else 0)

    for r in trel:
        off, ty = r[0], r[1]
        wi = off // 4
        c, t = cw[wi], tw[wi]
        cop, top = c >> 26, t >> 26
        if ty == R_MIPS_26:
            if cop != top or cop not in (2, 3):
                continue                       # opcode differs: a mismatch
            retail = ((func_addr + wi * 4 + 4) & 0xf0000000) | ((t & 0x3ffffff) << 2)
            base = _target_ok(r, retail, 'jal/j')
            if base is False:
                continue
            want = base + ((c & 0x3ffffff) << 2) if not isinstance(base, tuple) else None
            if want is None or (want & 0x0fffffff) != (retail & 0x0fffffff):
                errors.append(f'{tag} [{wi}] {"jal" if cop == 3 else "j"} names '
                              f'{r[2]} -> 0x{(want or 0):08X} but retail targets '
                              f'0x{retail:08X}')
                continue
            _put(wi, t)
        elif ty in (R_MIPS_HI16, R_MIPS_LO16):
            if (c & 0xffff0000) != (t & 0xffff0000):
                continue                       # opcode/register differ
            same_sym = [x for x in trel if x[2] == r[2] and x[3] == r[3]
                        and x[4] == r[4]]
            if ty == R_MIPS_HI16:
                rt = (c >> 16) & 0x1f
                cands = [x for x in same_sym if x[1] == R_MIPS_LO16
                         and ((cw[x[0] // 4] >> 21) & 0x1f) == rt] or \
                        [x for x in same_sym if x[1] == R_MIPS_LO16]
                pairs = [(((c & 0xffff) << 16) + _sext16(cw[x[0] // 4]),
                          (((t & 0xffff) << 16) + _sext16(tw[x[0] // 4])) & 0xffffffff)
                         for x in cands]
                what = '%hi'
            else:
                rs = (c >> 21) & 0x1f
                cands = [x for x in same_sym if x[1] == R_MIPS_HI16
                         and ((cw[x[0] // 4] >> 16) & 0x1f) == rs]
                pairs = [(((cw[x[0] // 4] & 0xffff) << 16) + _sext16(c),
                          (((tw[x[0] // 4] & 0xffff) << 16) + _sext16(t)) & 0xffffffff)
                         for x in cands]
                what = '%lo'
            if not pairs:
                if ty == R_MIPS_LO16 and r[3] == 'undef':
                    # no visible %hi (shared $at base): the low half alone
                    a = resolve_symbol_name(r[2])
                    if a is None:
                        errors.append(f'{tag} [{wi}] %lo: UNRESOLVED name {r[2]}')
                        continue
                    if ((a + _sext16(c)) & 0xffff) != (t & 0xffff):
                        errors.append(f'{tag} [{wi}] %lo({r[2]}{_sext16(c):+#x}) = '
                                      f'..{(a + _sext16(c)) & 0xffff:04X} but retail '
                                      f'low half is {t & 0xffff:04X}')
                        continue
                    _put(wi, t)
                    continue
                errors.append(f'{tag} [{wi}] {what}({r[2] or r[4]}) has no partner '
                              f'relocation -- cannot verify')
                continue
            base = _target_ok(r, pairs[0][1], what)
            if base is False:
                continue
            ok = False
            msgs = []
            for addend, retail_addr in pairs:
                if isinstance(base, tuple):
                    _, sec, sval = base
                    trial = []
                    if _rodata_extent_ok(obj_info, sec, sval + addend, retail_addr,
                                         exe_data, func_addr, len(func_bytes),
                                         trial, f'{tag} [{wi}]', checked):
                        ok = True
                        break
                    msgs.extend(trial)
                else:
                    want = (base + addend) & 0xffffffff
                    if want == retail_addr:
                        ok = True
                        break
                    msgs.append(f'{tag} [{wi}] {what}({r[2] or r[4]}'
                                f'{addend:+#x}) = 0x{want:08X} but retail '
                                f'addresses 0x{retail_addr:08X}')
            if not ok:
                errors.extend(msgs[:1] if not isinstance(base, tuple) else msgs)
                continue
            _put(wi, t)
        elif c != t:
            errors.append(f'{tag} [{wi}] relocation type {ty} against {r[2]} '
                          f'is not checked -- left as compiled')
    return bytes(result)


def report_nonreloc_fixups(strict=False, out=print, audit=None):
    """Retained for API compatibility; non-reloc words are never copied now."""
    audit = NONRELOC_FIXUPS if audit is None else audit
    if not audit:
        return 0
    for r in audit:
        out(f"      {r}")
    return len(audit)


CACHE_DIR = ROOT_DIR / "build" / "cache"
TOOLCHAIN_FP_FILE = CACHE_DIR / "toolchain.fingerprint"


def _compute_toolchain_fingerprint():
    """Compute a content hash of the entire toolchain: compiler binaries,
    PostProc scripts, GCC binaries, maspsx, and include headers.

    This is computed ONCE per build. If it differs from the stored fingerprint,
    the entire cache is invalidated. Uses file CONTENT hashes (not mtime) so
    git operations, rsync, and touch don't cause false invalidation.
    """
    h = hashlib.sha256()

    # IDO compiler binaries (content hash — catches hook rebuilds)
    for binary_name in ["cc", "uopt", "ugen", "as1", "cfe"]:
        p = ROOT_DIR / f"tools/ido-static-recomp/build/5.2/out/{binary_name}"
        if p.exists():
            with open(p, 'rb') as f:
                h.update(f.read())

    # ALL PostProc tool scripts (content hash)
    for tool_dir in [ROOT_DIR / "tools" / "ido",
                     ROOT_DIR / "tools" / "postproc",
                     ROOT_DIR / "tools" / "build",
                     ROOT_DIR / "tools" / "maspsx",
                     # gccwrap.py (the GCC and ido52-gas front end) lives
                     # here; a rem/mul expansion change went unnoticed by the
                     # cache until --no-cache (2026-09-24)
                     ROOT_DIR / "tools" / "gcc"]:
        if tool_dir.exists():
            # rglob: tools/maspsx/maspsx/__init__.py holds the assembler
            # rules; a top-level glob missed it (stale cache hit, 2026-09-17)
            for py_file in sorted(tool_dir.rglob("*.py")):
                with open(py_file, 'rb') as f:
                    h.update(py_file.name.encode())
                    h.update(f.read())

    # GCC compiler binaries
    # (every cc1 variant: cc1, cc1-nocj, cc1-psx-26, cc1-psx-26-patched)
    for gcc_dir in ["psyq-gcc-260", "psyq-gcc-272", "psyq-gcc"]:
        for cc1 in sorted((ROOT_DIR / "tools" / gcc_dir).glob("cc1*")):
            with open(cc1, 'rb') as f:
                h.update(cc1.name.encode())
                h.update(f.read())

    # Include headers (content hash)
    include_dir = ROOT_DIR / "include"
    if include_dir.exists():
        for hdr in sorted(include_dir.rglob("*.h")):
            with open(hdr, 'rb') as f:
                h.update(hdr.name.encode())
                h.update(f.read())

    return h.hexdigest()


def _validate_cache():
    """Check if toolchain has changed since last build. If so, wipe cache.

    Returns True if cache is valid, False if it was invalidated.
    """
    current_fp = _compute_toolchain_fingerprint()

    if TOOLCHAIN_FP_FILE.exists():
        stored_fp = TOOLCHAIN_FP_FILE.read_text().strip()
        if stored_fp == current_fp:
            return True
        # Toolchain changed — wipe entire cache
        print(f"  Toolchain changed (tools/scripts/headers modified), clearing cache...")
        import shutil
        for f in CACHE_DIR.glob("*.bin"):
            f.unlink()

    # Store new fingerprint
    CACHE_DIR.mkdir(parents=True, exist_ok=True)
    TOOLCHAIN_FP_FILE.write_text(current_fp + "\n")
    return False


def _compute_cache_key(source_path, name, addr, size):
    """Compute per-function cache key from source content and identity.

    Toolchain validation is done separately (once per build) by _validate_cache().
    This only needs to hash the function-specific inputs.
    """
    h = hashlib.sha256()
    with open(source_path, 'rb') as f:
        h.update(f.read())
    h.update(f"{name}:{addr}:{size}".encode())
    return h.hexdigest()[:32]


def _cache_get(cache_key):
    """Try to load compiled bytes (+ relocation info) from cache.

    Returns (bytes, obj_info) or None.  An entry without its relocation
    sidecar is a miss: the fixup refuses to copy anything without it.
    """
    cache_file = CACHE_DIR / f"{cache_key}.bin"
    info_file = CACHE_DIR / f"{cache_key}.relocs.json"
    if cache_file.exists() and info_file.exists():
        with open(cache_file, 'rb') as f:
            data = f.read()
        with open(info_file) as f:
            return data, json.load(f)
    return None


def _cache_put(cache_key, func_bytes, obj_info=None):
    """Store compiled bytes (+ relocation info sidecar) in cache."""
    CACHE_DIR.mkdir(parents=True, exist_ok=True)
    cache_file = CACHE_DIR / f"{cache_key}.bin"
    with open(cache_file, 'wb') as f:
        f.write(func_bytes)
    if obj_info is not None:
        with open(CACHE_DIR / f"{cache_key}.relocs.json", 'w') as f:
            json.dump(obj_info, f)


def _compile_worker(task):
    """Worker function for parallel compilation.

    Runs in a subprocess via ProcessPoolExecutor. Compiles a single C function,
    applies post-processing, and returns the compiled bytes or an error.
    Uses content-hash cache to skip recompilation of unchanged functions.

    Args:
        task: dict with keys: index, total, name, addr, size, source, verbose

    Returns:
        dict with keys: index, name, addr, size, func_bytes, error, log_lines
    """
    idx = task['index']
    total = task['total']
    name = task['name']
    addr = task['addr']
    size = task['size']
    source = task['source']
    verbose = task['verbose']
    use_cache = task.get('use_cache', True)

    log_lines = []

    source_path = ROOT_DIR / source
    if not source_path.exists():
        return {'index': idx, 'name': name, 'addr': addr, 'size': size,
                'func_bytes': None, 'error': f"Source not found: {source}",
                'log_lines': log_lines}

    if not source.endswith('.c'):
        return {'index': idx, 'name': name, 'addr': addr, 'size': size,
                'func_bytes': None, 'error': None, 'skip': True,
                'log_lines': [f"    SKIP: Not a C file: {source}"]}

    # Check cache
    cache_key = None
    if use_cache:
        try:
            cache_key = _compute_cache_key(source_path, name, addr, size)
            cached = _cache_get(cache_key)
            if cached is not None and len(cached[0]) == size:
                return {'index': idx, 'name': name, 'addr': addr, 'size': size,
                        'func_bytes': cached[0], 'obj_info': cached[1],
                        'error': None, 'cached': True,
                        'log_lines': log_lines}
        except Exception:
            pass  # Cache miss, compile normally

    # Parse headers
    postproc_steps = parse_postproc_header(source_path)
    if postproc_steps and verbose:
        log_lines.append(f"    Found {len(postproc_steps)} post-processing step(s)")

    extra_flags = parse_compile_flags(source_path)
    if extra_flags and verbose:
        log_lines.append(f"    Extra compile flags: {' '.join(extra_flags)}")

    compiler = parse_compiler(source_path)
    if compiler != "ido52" and verbose:
        log_lines.append(f"    Compiler: {compiler}")

    compile_env = parse_compile_env(source_path)
    if compile_env and verbose:
        log_lines.append(f"    Compile env: {compile_env}")

    # Compile (verbose output from compile_c_function goes to stdout directly;
    # in parallel mode we suppress it to avoid interleaved output)
    worker_verbose = verbose and task.get('sequential', False)
    try:
        func_bytes = compile_c_function(source_path, name, addr, worker_verbose,
                                        postproc_steps, extra_flags, compiler,
                                        compile_env)
    except Exception as e:
        return {'index': idx, 'name': name, 'addr': addr, 'size': size,
                'func_bytes': None, 'error': str(e), 'log_lines': log_lines}

    # Truncate to expected size
    if len(func_bytes) > size:
        func_bytes = func_bytes[:size]
    elif len(func_bytes) < size:
        return {'index': idx, 'name': name, 'addr': addr, 'size': size,
                'func_bytes': None,
                'error': f"Compiled size {len(func_bytes)} < expected {size}",
                'log_lines': log_lines}

    # Store in cache (before relocation fixup — cache stores pre-reloc bytes)
    obj_info = LAST_OBJ_INFO
    if cache_key:
        try:
            _cache_put(cache_key, func_bytes, obj_info)
        except Exception:
            pass  # Non-fatal

    return {'index': idx, 'name': name, 'addr': addr, 'size': size,
            'func_bytes': func_bytes, 'obj_info': obj_info, 'error': None,
            'log_lines': log_lines}


def main():
    parser = argparse.ArgumentParser(description='Build with Level 3 matched C functions')
    parser.add_argument('--verbose', '-v', action='store_true', help='Verbose output')
    parser.add_argument('--jobs', '-j', type=int, default=None,
                        help='Number of parallel compilation jobs (default: cpu_count)')
    parser.add_argument('--exe', type=str, default='disks/SLPS_000.65', help='Original executable')
    parser.add_argument('--config', type=str, default='config/level3_functions.json', help='Level 3 config')
    parser.add_argument('--output', type=str, default='build/jp/level3/SLPS_000.65', help='Output path')
    parser.add_argument('--no-cache', action='store_true', help='Disable compilation cache')
    parser.add_argument('--clear-cache', action='store_true', help='Clear cache before building')
    parser.add_argument('--strict-fixup-audit', action='store_true',
                        help='List every word fixup_relocations masked at a '
                             'NON-relocation site (frame offsets, branch '
                             'displacements, non-address immediates)')
    args = parser.parse_args()

    num_jobs = args.jobs if args.jobs is not None else (os.cpu_count() or 4)
    if num_jobs < 1:
        num_jobs = 1

    # Resolve paths relative to ROOT_DIR
    exe_path = ROOT_DIR / args.exe
    config_path = ROOT_DIR / args.config
    output_path = ROOT_DIR / args.output

    print("=" * 70)
    print("LEVEL 3 BUILD - Byte-Perfect Matched C Functions")
    print("=" * 70)

    # Check IDO compiler
    if not IDO_CC.exists():
        print(f"ERROR: IDO compiler not found at {IDO_CC}")
        print("Run: make setup-ido52")
        return 1

    # Load original executable
    print(f"\nLoading original: {exe_path}")
    with open(exe_path, 'rb') as f:
        exe_data = bytearray(f.read())

    orig_sha1 = hashlib.sha1(exe_data).hexdigest()
    print(f"Original SHA1: {orig_sha1}")

    if orig_sha1 != EXPECTED_SHA1:
        print(f"WARNING: Original SHA1 doesn't match expected!")
        print(f"Expected: {EXPECTED_SHA1}")

    # Cache management
    if args.clear_cache:
        import shutil
        if CACHE_DIR.exists():
            shutil.rmtree(CACHE_DIR)
            print("Cache cleared.")
    if not args.no_cache:
        print("\nValidating toolchain cache...")
        cache_valid = _validate_cache()
        if cache_valid:
            cached_count = len(list(CACHE_DIR.glob("*.bin")))
            print(f"  Cache valid ({cached_count} entries)")
        else:
            print(f"  Cache rebuilt (toolchain fingerprint updated)")

    # Load Level 3 config
    print(f"\nLoading Level 3 config: {config_path}")
    with open(config_path, 'r') as f:
        config = json.load(f)

    functions = config.get('functions', [])
    print(f"Level 3 functions: {len(functions)}")

    if not functions:
        print("\nNo Level 3 functions configured. Output will match original.")

    # Build task list, skipping ASM files early
    tasks = []
    skipped_asm = 0
    for i, func in enumerate(functions):
        source = func['source']
        if not source.endswith('.c'):
            skipped_asm += 1
            continue
        tasks.append({
            'index': i,
            'total': len(functions),
            'name': func['name'],
            'addr': int(func['address'], 16),
            'size': func['size'],
            'source': source,
            'verbose': args.verbose,
            'use_cache': not args.no_cache,
        })

    c_total = len(tasks)
    if skipped_asm:
        print(f"Skipped {skipped_asm} non-C entries, compiling {c_total} C functions")

    print(f"Using {num_jobs} parallel job(s)\n")

    start_time = time.time()

    # Process functions
    matched_count = 0
    mismatch_count = 0

    if num_jobs == 1:
        # Sequential mode: compile and patch one at a time (original behavior)
        for task_idx, task in enumerate(tasks):
            name = task['name']
            addr = task['addr']
            size = task['size']
            file_offset = addr_to_file_offset(addr)

            print(f"  [{task_idx + 1}/{c_total}] {name}:")
            if args.verbose:
                print(f"    Address: 0x{addr:08X}, Size: {size}B")

            task['sequential'] = True
            result = _compile_worker(task)

            # Print collected log lines (verbose output from worker)
            for line in result.get('log_lines', []):
                print(line)

            if result.get('skip'):
                continue

            if result['error']:
                print(f"    ERROR: {result['error']}")
                mismatch_count += 1
                continue

            func_bytes = result['func_bytes']

            # Get original bytes
            orig_bytes = bytes(exe_data[file_offset:file_offset + size])

            # Fix relocations (needs exe_data)
            rel_errs = []
            func_bytes = fixup_relocations(func_bytes, addr, exe_data, file_offset,
                                           func_name=name,
                                           obj_info=result.get('obj_info'),
                                           errors=rel_errs)
            RELOC_ERRORS.extend(rel_errs)
            for e in rel_errs:
                print(f"    RELOC CHECK: {e}")

            # Compare with original
            if func_bytes == orig_bytes and not rel_errs:
                print(f"    MATCH!  [honesty audit: no painting custom_script / no banned PostProc / faithful C — see CLAUDE.md]")
                matched_count += 1
            else:
                print(f"    MISMATCH:")
                for i in range(0, size, 4):
                    orig_word = struct.unpack('<I', orig_bytes[i:i+4])[0]
                    new_word = struct.unpack('<I', func_bytes[i:i+4])[0]
                    if orig_word != new_word:
                        print(f"      +0x{i:02X}: orig={orig_word:08x} new={new_word:08x}")
                mismatch_count += 1

            # Patch into executable
            exe_data[file_offset:file_offset + size] = func_bytes
    else:
        # Parallel mode: compile all functions, then patch sequentially
        results_by_index = {}
        completed = 0

        with ProcessPoolExecutor(max_workers=num_jobs) as executor:
            future_to_task = {executor.submit(_compile_worker, t): t for t in tasks}

            for future in as_completed(future_to_task):
                completed += 1
                result = future.result()
                results_by_index[result['index']] = result
                # Print progress counter (not verbose, just progress)
                name = result['name']
                status = "ok" if result['error'] is None and not result.get('skip') else (
                    "skip" if result.get('skip') else "FAIL")
                cached_str = " (cached)" if result.get('cached') else ""
                print(f"  [{completed}/{c_total}] compiled {name} ... {status}{cached_str}")

        # Patch sequentially in original order
        mismatch_details = []  # (name, diffs_list) for log
        print(f"\nPatching {len(results_by_index)} compiled functions into binary...")
        for task in tasks:
            orig_index = task['index']
            result = results_by_index.get(orig_index)
            if result is None:
                continue

            name = result['name']
            addr = result['addr']
            size = result['size']
            file_offset = addr_to_file_offset(addr)

            # Print verbose log lines
            if args.verbose:
                print(f"\n  {name}:")
                print(f"    Address: 0x{addr:08X}, Size: {size}B")
                for line in result.get('log_lines', []):
                    print(line)

            if result.get('skip'):
                continue

            if result['error']:
                if args.verbose:
                    print(f"    ERROR: {result['error']}")
                mismatch_details.append((name, [f"COMPILE ERROR: {result['error']}"]))
                mismatch_count += 1
                continue

            func_bytes = result['func_bytes']

            # Get original bytes
            orig_bytes = bytes(exe_data[file_offset:file_offset + size])

            # Fix relocations (needs exe_data, must be sequential)
            rel_errs = []
            func_bytes = fixup_relocations(func_bytes, addr, exe_data, file_offset,
                                           func_name=name,
                                           obj_info=result.get('obj_info'),
                                           errors=rel_errs)
            RELOC_ERRORS.extend(rel_errs)

            # Compare with original
            if func_bytes == orig_bytes and not rel_errs:
                if args.verbose:
                    print(f"    MATCH!  [honesty audit: no painting custom_script / no banned PostProc / faithful C — see CLAUDE.md]")
                matched_count += 1
            else:
                print(f"  {name}: MISMATCH")
                diffs = []
                for i in range(0, size, 4):
                    orig_word = struct.unpack('<I', orig_bytes[i:i+4])[0]
                    new_word = struct.unpack('<I', func_bytes[i:i+4])[0]
                    if orig_word != new_word:
                        diff_line = f"+0x{i:02X}: orig={orig_word:08x} new={new_word:08x}"
                        diffs.append(diff_line)
                        if args.verbose:
                            print(f"      {diff_line}")
                diffs = [f"RELOC CHECK: {e}" for e in rel_errs] + diffs
                for e in rel_errs:
                    print(f"      RELOC CHECK: {e}")
                mismatch_details.append((name, diffs))
                mismatch_count += 1

            # Patch into executable
            exe_data[file_offset:file_offset + size] = func_bytes

    elapsed = time.time() - start_time

    # Write output
    output_path.parent.mkdir(parents=True, exist_ok=True)

    print(f"\nWriting output: {output_path}")
    with open(output_path, 'wb') as f:
        f.write(exe_data)

    # Verify final SHA1
    final_sha1 = hashlib.sha1(exe_data).hexdigest()

    summary_lines = []
    summary_lines.append("=" * 70)
    summary_lines.append("BUILD SUMMARY")
    summary_lines.append("=" * 70)
    # results_by_index only exists in parallel mode; sequential (-j1) skips it
    _results = locals().get('results_by_index', {})
    cache_hits = sum(1 for r in _results.values() if r.get('cached'))
    cache_misses = len(_results) - cache_hits
    summary_lines.append(f"C functions:       {c_total}")
    summary_lines.append(f"Matched:           {matched_count}")
    summary_lines.append(f"Mismatched:        {mismatch_count}")
    summary_lines.append(f"Cache hits:        {cache_hits}/{c_total}")
    summary_lines.append(f"Jobs:              {num_jobs}")
    summary_lines.append(f"Elapsed:           {elapsed:.1f}s")
    summary_lines.append(f"Original SHA1:     {orig_sha1}")
    summary_lines.append(f"Output SHA1:       {final_sha1}")
    # Make the fixup's over-masking visible: words it copied from the target at
    # sites that are NOT linker relocations are genuine codegen diffs corrected
    # on the way into the build.
    # Since 2026-10-03 the fixup copies ONLY at the object's own relocation
    # records after a name/content check; a refused check is a mismatch.
    summary_lines.append(f"Reloc refusals:    {len(RELOC_ERRORS)}"
                         + ("   (listed in build/build.log)" if RELOC_ERRORS else ""))

    if final_sha1 == orig_sha1 and mismatch_count:
        # 2026-10-03 (red-team finding 5): a registered C entry that does not
        # match is patched with... the retail bytes it failed to reproduce
        # (the ASM fallback), so the SHA1 still matches and this printed
        # SUCCESS / exit 0 while a C file in the registry was broken.
        summary_lines.append("")
        summary_lines.append(f" FAILURE: {mismatch_count} registered C function(s) did NOT "
                             "match and fell back to retail bytes")
        summary_lines.append("  (the SHA1 matches only because of that fallback; see "
                             "build/build.log for the list)")
        rc = 1
    elif final_sha1 == orig_sha1:
        summary_lines.append("")
        summary_lines.append(" SUCCESS! Output matches original SHA1!")
        summary_lines.append("The game should run identically to the original.")
        rc = 0
    else:
        summary_lines.append("")
        summary_lines.append(f" Output differs from original ({mismatch_count} functions)")
        summary_lines.append("  The game should still be playable if matches are correct.")
        rc = 1

    for line in summary_lines:
        print(line)

    if args.strict_fixup_audit:
        report_nonreloc_fixups(strict=True)

    # Always write build log
    log_path = ROOT_DIR / "build" / "build.log"
    log_path.parent.mkdir(parents=True, exist_ok=True)
    with open(log_path, 'w') as f:
        f.write(f"Build completed: {time.strftime('%Y-%m-%d %H:%M:%S')}\n\n")
        for line in summary_lines:
            f.write(line + "\n")
        _md = locals().get('mismatch_details', [])
        if _md:
            f.write("\nMismatched functions:\n")
            for name, diffs in _md:
                f.write(f"\n  {name}:\n")
                for diff_line in diffs:
                    f.write(f"    {diff_line}\n")
    print(f"\nBuild log written to: {log_path}")

    return rc


if __name__ == '__main__':
    sys.exit(main())
