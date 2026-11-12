#!/usr/bin/env python3
"""
overlay_postproc.py - PostProc pipeline for overlay functions

Provides unified PostProc parsing and application for overlay C files.
Supports all major PostProc operations used in the main binary build.

Usage:
    from overlay_postproc import parse_postproc, apply_postproc

    steps = parse_postproc(c_file_path)
    success = apply_postproc(obj_file, steps, func_addr, overlay_name, verbose=True)
"""

import os
import re
import sys
import shutil
import struct
import subprocess
from pathlib import Path
from typing import Dict, List, Optional, Any, Tuple

# Project root
ROOT = Path(__file__).parent.parent.parent

# Banned PostProc enforcement
sys.path.insert(0, str(ROOT / "tools" / "build"))
from banned_postproc import check_banned_postproc_steps
# Prose-immune directive-line matcher (single source of truth). A line only
# counts as a directive when it OPENS with the keyword after comment markers;
# header prose that quotes "@PostProc: ..." mid-sentence is ignored.
from parse_postproc import directive_bodies

# Tool paths
TOOLS_IDO = ROOT / "tools" / "ido"
TOOLS_POSTPROC = ROOT / "tools" / "postproc"

# PostProc tools
STRIP_TRAILING = TOOLS_IDO / "strip_trailing_instructions.py"
STRIP_LEADING = TOOLS_IDO / "strip_leading_instructions.py"
PATCH_STACK = TOOLS_IDO / "patch_stack_complete.py"
PAD_TRAILING_NOP = TOOLS_IDO / "pad_trailing_nop.py"
SWAP_REGISTERS = TOOLS_IDO / "swap_registers_auto.py"
SWAP_REGISTERS_LIVENESS = TOOLS_IDO / "swap_registers_liveness.py"
RENAME_REGISTER_CHAIN = TOOLS_IDO / "rename_register_chain.py"
SCHEDULER_NORMALIZER = TOOLS_POSTPROC / "scheduler_normalizer.py"
CONVERT_ORI_TO_ADDIU = TOOLS_IDO / "convert_ori_to_addiu.py"
CONVERT_OR_TO_ADDU = TOOLS_IDO / "convert_or_to_addu.py"
SWAP_INSTRUCTIONS = TOOLS_IDO / "swap_instructions.py"
PATCH_INSN_REG_FIELD = TOOLS_IDO / "patch_insn_reg_field.py"
PATCH_INSN_IMM16 = TOOLS_IDO / "patch_insn_imm16.py"
PATCH_INSN_WORD = TOOLS_IDO / "patch_insn_word.py"
REMAP_HIDDEN_PARAMS = TOOLS_IDO / "remap_hidden_params.py"
STRIP_ARG_HOMES = TOOLS_IDO / "strip_arg_homes.py"
STRIP_DIV_CHECKS = TOOLS_IDO / "strip_div_checks.py"
REMAP_STACK_SLOTS = TOOLS_IDO / "remap_stack_slots.py"
REBASE_STORE_TO_SP = TOOLS_IDO / "rebase_store_to_sp.py"
BLOCK_REORDER = TOOLS_POSTPROC / "block_reorder.py"
ASM_INTERCEPT = TOOLS_IDO / "asm_intercept.py"
PREPEND_THUNK = TOOLS_IDO / "prepend_thunk.py"
APPEND_TRAILING = TOOLS_IDO / "append_trailing_instructions.py"
UNFILL_BRANCH_DELAY = TOOLS_IDO / "unfill_branch_delay.py"
UNFILL_JAL_DELAY = TOOLS_IDO / "unfill_jal_delay.py"
HOIST_OUT_OF_DELAY = TOOLS_IDO / "hoist_out_of_delay_slot.py"
EPILOGUE_SHARE = TOOLS_IDO / "epilogue_share_into_branches.py"
SWAP_SPILL_WITH_MOVE = TOOLS_IDO / "swap_spill_with_move.py"
INSERT_INSTRUCTION = TOOLS_IDO / "insert_instruction_and_shift.py"
SWAP_LUI_ADDR_FORM = TOOLS_IDO / "swap_lui_addr_form.py"
CONVERT_ADDIU_TO_ORI = TOOLS_IDO / "convert_addiu_to_ori.py"
CSE_GROUP_LUI = TOOLS_IDO / "cse_group_lui.py"
INVERT_BRANCH = TOOLS_IDO / "invert_branch.py"
CSE_LUI_AT = TOOLS_IDO / "cse_lui_at.py"
NOP_AT_OFFSET = TOOLS_IDO / "nop_at_offset.py"
PROPAGATE_STACK_TO_REG = TOOLS_IDO / "propagate_stack_to_reg.py"
ADJUST_BRANCH_OFFSET = TOOLS_IDO / "adjust_branch_offset.py"
SWAP_BRANCH_OPERANDS = TOOLS_IDO / "swap_branch_operands.py"
SWAP_COMMUTATIVE_OPERANDS = TOOLS_IDO / "swap_commutative_operands.py"
WRITE_INSN_AT_OFFSET = TOOLS_POSTPROC / "write_insn_at_offset.py"
CONVERT_ADDIU_TO_SLTI = TOOLS_IDO / "convert_addiu_to_slti.py"
STRIP_LW_RA_FROM_BRANCH_DELAYS = TOOLS_IDO / "strip_lw_ra_from_branch_delays.py"
UNFOLD_DEAD_OR_MOVE = TOOLS_IDO / "unfold_dead_or_move.py"
REORDER_EPILOGUE = TOOLS_POSTPROC / "reorder_epilogue.py"

# Overlay assets
ASSETS_OVERLAYS = ROOT / "assets" / "overlays"
ASSETS_EXTRACTED = ROOT / "assets" / "extracted" / "CDROM" / "EXEDIR"

# Overlay base address
OVERLAY_BASE = 0x80132000


def get_func_text_offset(obj_path: Path, func_name: str = None) -> int:
    """Return the .text offset at which the compiled function body begins.

    When `func_name` is given and that symbol exists as a FUNC in .text, its
    st_value is returned directly. Otherwise this falls back to the smallest
    non-zero st_value of FUNC-type symbols in .text.

    The fallback is a heuristic that assumes every symbol preceding the real
    function is an unnamed thunk. That holds for IDO objects (the types.h
    `read_*_unaligned` helpers get no STT_FUNC symbol), but NOT for Psy-Q GCC
    objects, which emit those helpers as named global functions — there the
    heuristic returns the first helper's offset and every downstream compare
    is shifted by a whole thunk. Prefer passing `func_name`.

    Used by `swap_instructions` / `patch_insn_reg_field` to translate
    function-relative offsets into .text-relative offsets after
    `prepend_thunk` has inserted bytes at the function entry. Returns 0
    if the object has no FUNC symbol with a non-zero st_value (in which
    case the offsets are already .text-relative).
    """
    try:
        data = obj_path.read_bytes()
    except Exception:
        return 0
    if data[:4] != b'\x7fELF':
        return 0
    endian = '<'
    e_shoff = struct.unpack(endian + 'I', data[32:36])[0]
    e_shentsize = struct.unpack(endian + 'H', data[46:48])[0]
    e_shnum = struct.unpack(endian + 'H', data[48:50])[0]
    e_shstrndx = struct.unpack(endian + 'H', data[50:52])[0]

    sections = []
    for i in range(e_shnum):
        off = e_shoff + i * e_shentsize
        sh = {
            'idx': i,
            'sh_name': struct.unpack(endian + 'I', data[off:off+4])[0],
            'sh_type': struct.unpack(endian + 'I', data[off+4:off+8])[0],
            'sh_offset': struct.unpack(endian + 'I', data[off+16:off+20])[0],
            'sh_size': struct.unpack(endian + 'I', data[off+20:off+24])[0],
            'sh_link': struct.unpack(endian + 'I', data[off+24:off+28])[0],
        }
        sections.append(sh)

    shstrtab = sections[e_shstrndx]
    strtab_data = data[shstrtab['sh_offset']:shstrtab['sh_offset']+shstrtab['sh_size']]

    text_idx = None
    symtab = None
    for sh in sections:
        idx = sh['sh_name']
        end = strtab_data.find(b'\x00', idx)
        name = strtab_data[idx:end].decode('ascii', errors='ignore')
        if name == '.text':
            text_idx = sh['idx']
        elif name == '.symtab':
            symtab = sh

    if text_idx is None or symtab is None:
        return 0

    # Symbol-name table for the exact-name lookup (symtab.sh_link -> .strtab).
    symstr = b''
    if func_name and symtab['sh_link'] < len(sections):
        ss = sections[symtab['sh_link']]
        symstr = data[ss['sh_offset']:ss['sh_offset'] + ss['sh_size']]

    sym_off = symtab['sh_offset']
    n_syms = symtab['sh_size'] // 16
    smallest = 0
    for i in range(n_syms):
        soff = sym_off + i * 16
        st_value = struct.unpack(endian + 'I', data[soff+4:soff+8])[0]
        st_info = data[soff + 12]
        st_shndx = struct.unpack(endian + 'H', data[soff+14:soff+16])[0]
        st_type = st_info & 0xF
        if st_type != 2:  # STT_FUNC
            continue
        if st_shndx != text_idx:
            continue
        if symstr:
            n_off = struct.unpack(endian + 'I', data[soff:soff+4])[0]
            end = symstr.find(b'\x00', n_off)
            if symstr[n_off:end].decode('ascii', errors='ignore') == func_name:
                return st_value
        if st_value == 0:
            continue
        if smallest == 0 or st_value < smallest:
            smallest = st_value
    return smallest


def get_reloc_offsets(obj_path: Path) -> Dict[int, str]:
    """Return {text_offset: reloc_type} for every .text relocation in obj.

    The offsets are .text-section-relative (the same coordinate space as the
    bytes extracted by objcopy -j .text). reloc_type is the GNU name, e.g.
    'R_MIPS_26', 'R_MIPS_HI16', 'R_MIPS_LO16'.

    This is the AUTHORITATIVE set of words whose immediate/target field the
    linker is allowed to rewrite. compare_bytes() masks the relocated field
    ONLY at these offsets — a differing immediate at a NON-reloc offset is a
    genuine codegen mismatch (a real semi), not a relocation.
    """
    try:
        r = subprocess.run(
            ["mipsel-linux-gnu-objdump", "-r", str(obj_path)],
            capture_output=True, text=True)
    except Exception:
        return {}
    out: Dict[int, str] = {}
    in_text = False
    for line in r.stdout.splitlines():
        if line.startswith("RELOCATION RECORDS FOR"):
            in_text = ".text" in line
            continue
        if not in_text:
            continue
        m = re.match(r'([0-9a-f]+)\s+(\S+)\s+(\S+)', line)
        if m:
            out[int(m.group(1), 16)] = m.group(2)
    return out


def get_target_exn(overlay_name: str) -> Optional[Path]:
    """Find target EXN file for an overlay."""
    # Try assets/overlays first
    exn = ASSETS_OVERLAYS / f"{overlay_name}.EXN"
    if exn.exists():
        return exn
    # Try extracted directory
    exn = ASSETS_EXTRACTED / f"{overlay_name}.EXN"
    if exn.exists():
        return exn
    return None


def parse_postproc(c_file: Path) -> List[Dict[str, Any]]:
    """Parse @PostProc annotations from overlay C file header.

    Supports all common PostProc operations:
    - strip_trailing_nop / strip_trailing_instructions --count N
    - strip_leading_instructions --count N
    - patch_stack_complete --old X --new Y
    - pad_trailing_nop --count N
    - swap_registers_auto REG1:REG2
    - swap_registers_liveness REG1->REG2
    - scheduler_normalizer [--block-aware]
    - convert_ori_to_addiu --offsets X,Y,Z
    - convert_or_to_addu
    - swap_instructions --offset1 X --offset2 Y
    - patch_insn_reg_field OFFSET:FIELD:REG
    - patch_insn_imm16 OFFSET:8:VAL
    - remap_hidden_params
    - strip_arg_homes --count N
    - strip_div_checks
    - remap_stack_slots --from X --to Y
    - rebase_store_to_sp --offset X --base REG
    - block_reorder --block X --target Y
    - custom_script PATH
    - asm_intercept SCRIPT_NAME
    - prepend_thunk FILE.s
    """
    steps = []
    try:
        with open(c_file, 'r', encoding='utf-8') as f:
            content = f.read()
    except Exception:
        return steps

    for body in directive_bodies(content, '@PostProc:'):
        line = body.strip()
        # Remove trailing comment chars
        line = re.sub(r'\s*\*+/?\s*$', '', line)

        step = _parse_postproc_line(line)
        if step:
            steps.append(step)

    # ── Painting-script enforcement (2026-06-10) ─────────────────
    # custom_script files that write hardcoded target instruction
    # words (PATCHES_BY_POS etc.) are patch_insn_word in disguise and
    # BANNED. Strip them here so every consumer (test_overlay_match,
    # verify_overlay_bytes, build_overlay) reports/builds the honest
    # allowed-PostProc-only result.
    from banned_postproc import is_painting_script
    kept = []
    for step in steps:
        if step.get('type') == 'custom_script':
            spath = _resolve_script_path(step['script'])
            if spath is not None and is_painting_script(spath):
                print(f"    [BANNED-STRIPPED] {c_file.name}: custom_script "
                      f"{step['script']} writes hardcoded instruction words "
                      f"(patch_insn_word semantics) — step ignored",
                      file=sys.stderr)
                continue
        kept.append(step)
    return kept


def _parse_postproc_line(line: str) -> Optional[Dict[str, Any]]:
    """Parse a single @PostProc directive line."""
    if not line:
        return None

    # strip_trailing_nop / strip_trailing_instructions
    m = re.search(r'strip_trailing(?:_nop|_instructions)?\s+(?:--count\s+)?(\d+)', line)
    if m:
        # strip_trailing_instructions strips any insns; strip_trailing_nop only strips nops
        nop_only = 'strip_trailing_nop' in line or 'strip_trailing_instructions' not in line
        return {'type': 'strip_trailing', 'count': int(m.group(1)), 'nop_only': nop_only}

    # strip_leading_instructions
    m = re.search(r'strip_leading(?:_instructions)?\s+(?:--count\s+)?(\d+)', line)
    if m:
        return {'type': 'strip_leading', 'count': int(m.group(1))}

    # patch_stack_complete / patch_stack / patch_frame
    m = re.search(r'patch_(?:stack|frame)(?:_complete)?\s+--old\s+(\d+)\s+--new\s+(\d+)', line)
    if m:
        return {'type': 'patch_stack', 'old': int(m.group(1)), 'new': int(m.group(2))}

    # pad_trailing_nop
    m = re.search(r'pad_trailing_nop\s+(?:--count\s+)?(\d+)', line)
    if m:
        return {'type': 'pad_trailing', 'count': int(m.group(1))}

    # rename_register_chain --chain a:b:c (atomic snapshot semantics)
    # Multiple --chain args: rename_register_chain --chain a:b --chain c:d
    # Optional: --exclude-offsets 0xOFF1,0xOFF2 (object-relative, after 0x10 thunk pad)
    if 'rename_register_chain' in line:
        chains = re.findall(r'--chain\s+([\w:]+)', line)
        if chains:
            result = {'type': 'rename_register_chain', 'chains': chains}
            m_excl = re.search(r'--exclude-offsets\s+([\s0-9a-fA-Fx,]+)', line)
            if m_excl:
                offsets_str = m_excl.group(1).strip()
                offsets = [x.strip() for x in re.findall(r'0x[0-9a-fA-F]+|\d+', offsets_str)]
                if offsets:
                    result['exclude_offsets'] = ','.join(offsets)
            # --range START:END (Tier A1 — bound rename to a single live range)
            m_range = re.search(r'--range\s+(0x[0-9a-fA-F]+|\d+):(0x[0-9a-fA-F]+|\d+)', line)
            if m_range:
                result['byte_range'] = f"{m_range.group(1)}:{m_range.group(2)}"
            # --occurrence N[,N...]:src:dst (repeatable; Tier A1)
            occ_specs = re.findall(
                r'--occurrence\s+([\d,]+:[a-z0-9]+:[a-z0-9]+)', line
            )
            if occ_specs:
                result['occurrences'] = occ_specs
            return result

    # swap_registers_auto REG1:REG2,REG3:REG4,... (unidirectional by default)
    # Supports multiple comma-separated swaps in one pass
    # Optional: --exclude-offsets 0xOFF1,0xOFF2
    m = re.search(r'swap_registers(?:_auto)?\s+([\w:,]+)', line)
    if m:
        swaps_str = m.group(1)
        # Parse comma-separated swap pairs
        swap_pairs = []
        for pair in swaps_str.split(','):
            pair = pair.strip()
            if ':' in pair:
                parts = pair.split(':')
                if len(parts) == 2:
                    swap_pairs.append((parts[0].strip(), parts[1].strip()))
        if swap_pairs:
            result = {'type': 'swap_registers', 'swaps': swap_pairs}
            if '--bidirectional' in line:
                result['bidirectional'] = True
            # Parse --exclude-offsets
            m_excl = re.search(r'--exclude-offsets\s+([\s0-9a-fA-Fx,]+)', line)
            if m_excl:
                offsets_str = m_excl.group(1).strip()
                offsets = [x.strip() for x in re.findall(r'0x[0-9a-fA-F]+|\d+', offsets_str)]
                if offsets:
                    result['exclude_offsets'] = ','.join(offsets)
            return result

    # swap_registers_liveness REG1->REG2
    m = re.search(r'swap_registers_liveness\s+(\w+)->(\w+)', line)
    if m:
        return {'type': 'swap_registers_liveness', 'from': m.group(1), 'to': m.group(2)}

    # scheduler_normalizer
    if 'scheduler_normalizer' in line:
        result = {'type': 'scheduler_normalizer'}
        if '--block-aware' in line:
            result['block_aware'] = True
        if '--branch-fp' in line:
            result['branch_fp'] = True
        m = re.search(r'--fence\s+([\d\-x,a-fA-F]+)', line)
        if m:
            result['fence'] = m.group(1)
        if '--const-fp' in line:
            result['const_fp'] = True
        if '--store-fp' in line:
            result['store_fp'] = True
        if '--load-fp' in line:
            result['load_fp'] = True
        if '--freeze-epilogue' in line:
            result['freeze_epilogue'] = True
        if '--reloc-symbol-fp' in line:
            result['reloc_symbol_fp'] = True
        if '--no-forced-reorder' in line:
            result['no_forced_reorder'] = True
        return result

    # convert_ori_to_addiu --offsets X,Y,Z (supports hex with 0x prefix)
    m = re.search(r'convert_ori_to_addiu\s+--offsets\s+([\da-fA-Fx,\s]+)', line)
    if m:
        offsets = []
        for x in m.group(1).split(','):
            x = x.strip()
            if x:
                offsets.append(int(x, 16) if x.startswith('0x') else int(x))
        return {'type': 'convert_ori_to_addiu', 'offsets': offsets}

    # convert_or_to_addu (no args)
    if 'convert_or_to_addu' in line and 'ori' not in line:
        return {'type': 'convert_or_to_addu'}

    # swap_instructions --offset1 X --offset2 Y
    m = re.search(r'swap_instructions\s+--offset1\s+(0x[0-9a-fA-F]+|\d+)\s+--offset2\s+(0x[0-9a-fA-F]+|\d+)', line)
    if m:
        o1 = int(m.group(1), 16) if m.group(1).startswith('0x') else int(m.group(1))
        o2 = int(m.group(2), 16) if m.group(2).startswith('0x') else int(m.group(2))
        return {'type': 'swap_instructions', 'offset1': o1, 'offset2': o2}

    # unfill_branch_delay --offset X
    m = re.search(r'unfill_branch_delay\s+--offset\s+(0x[0-9a-fA-F]+|\d+)', line)
    if m:
        offset = int(m.group(1), 16) if m.group(1).startswith('0x') else int(m.group(1))
        return {'type': 'unfill_branch_delay', 'offset': offset}

    # unfill_jal_delay --offset X [--max-search K]
    m = re.search(r'unfill_jal_delay\s+--offset\s+(0x[0-9a-fA-F]+|\d+)'
                  r'(?:\s+--max-search\s+(\d+))?', line)
    if m:
        offset = int(m.group(1), 16) if m.group(1).startswith('0x') else int(m.group(1))
        max_search = int(m.group(2)) if m.group(2) else 8
        return {'type': 'unfill_jal_delay', 'offset': offset,
                'max_search': max_search}

    # hoist_out_of_delay_slot --offset X [--allow-branch-in-delay]
    m = re.search(r'hoist_out_of_delay_slot\s+--offset\s+(0x[0-9a-fA-F]+|\d+)', line)
    if m:
        offset = int(m.group(1), 16) if m.group(1).startswith('0x') else int(m.group(1))
        allow = '--allow-branch-in-delay' in line
        return {'type': 'hoist_out_of_delay_slot', 'offset': offset,
                'allow_branch_in_delay': allow}

    # epilogue_share_into_branches --insn 0xXXXXXXXX --into-offsets 0xN[,0xM,...]
    m = re.search(
        r'epilogue_share_into_branches\s+--insn\s+(0x[0-9a-fA-F]+)'
        r'\s+--into-offsets\s+([0-9a-fA-Fx,\s]+?)'
        r'(?:\s+--keep-offset)?'
        r'(?:\s|$)',
        line,
    )
    if m:
        insn = int(m.group(1), 16)
        offsets = []
        for o in re.split(r'[\s,]+', m.group(2).strip()):
            if o:
                offsets.append(int(o, 16) if o.startswith('0x') else int(o))
        result = {'type': 'epilogue_share_into_branches', 'insn': insn, 'offsets': offsets}
        if '--keep-offset' in line:
            result['keep_offset'] = True
        if '--jal-predecessor' in line:
            result['jal_predecessor'] = True
        return result

    # insert_instruction --offset 0xN --insn 0xXXXXXXXX [--grow]
    # write_insn_at_offset --patch 0xOFFSET:0xINSN [--patch ...]
    m = re.search(
        r'insert_instruction\s+--offset\s+(0x[0-9a-fA-F]+|\d+)'
        r'\s+--insn\s+(0x[0-9a-fA-F]+)',
        line,
    )
    if m:
        off_str = m.group(1)
        offset = int(off_str, 16) if off_str.startswith('0x') else int(off_str)
        insn = int(m.group(2), 16)
        result = {'type': 'insert_instruction', 'offset': offset, 'insn': insn}
        if '--grow' in line:
            result['grow'] = True
        return result

    # write_insn_at_offset --patch 0xOFFSET:0xTARGET_INSN[:0xEXPECT_OLD] [--patch ...]
    # The 3rd field (EXPECT_OLD) is a safeguard — the tool validates that the
    # instruction at the offset matches before overwriting. This prevents
    # phantom matches from stale offsets after scheduler reordering.
    m = re.search(r'write_insn_at_offset\s+--patch\s+', line)
    if m:
        # Match 2 or 3 field patches: offset:insn[:expect]
        patch_strs = re.findall(r'(0x[0-9a-fA-F]+|\d+):(0x[0-9a-fA-F]+)(?::(0x[0-9a-fA-F]+))?', line)
        parsed_patches = []
        for off_str, insn_str, expect_str in patch_strs:
            offset = int(off_str, 16) if off_str.startswith('0x') else int(off_str)
            insn = int(insn_str, 16)
            expect = int(expect_str, 16) if expect_str else None
            parsed_patches.append((offset, insn, expect))
        return {'type': 'write_insn_at_offset', 'patches': parsed_patches}

    # swap_spill_with_move --offset 0xN --dst REG
    m = re.search(
        r'swap_spill_with_move\s+--offset\s+(0x[0-9a-fA-F]+|\d+)'
        r'\s+--dst\s+(\w+)',
        line,
    )
    if m:
        off_str = m.group(1)
        offset = int(off_str, 16) if off_str.startswith('0x') else int(off_str)
        dst = m.group(2)
        allow_non_sp = '--allow-non-sp' in line
        return {'type': 'swap_spill_with_move', 'offset': offset,
                'dst': dst, 'allow_non_sp': allow_non_sp}

    # swap_lui_addr_form --offset 0xN --to ori|addiu
    m = re.search(
        r'swap_lui_addr_form\s+--offset\s+(0x[0-9a-fA-F]+|\d+)'
        r'\s+--to\s+(ori|addiu)',
        line,
    )
    if m:
        off_str = m.group(1)
        offset = int(off_str, 16) if off_str.startswith('0x') else int(off_str)
        return {'type': 'swap_lui_addr_form', 'offset': offset, 'to': m.group(2)}

    # convert_addiu_to_slti --offset 0xN (Tier A2)
    m = re.search(r'convert_addiu_to_slti\s+--offset\s+(0x[0-9a-fA-F]+|\d+)', line)
    if m:
        off_str = m.group(1)
        offset = int(off_str, 16) if off_str.startswith('0x') else int(off_str)
        return {'type': 'convert_addiu_to_slti', 'offset': offset}

    # convert_addiu_to_ori --offset 0xN or --offsets 0xN,0xM
    m = re.search(r'convert_addiu_to_ori\s+--offset\s+(0x[0-9a-fA-F]+|\d+)', line)
    if m:
        off_str = m.group(1)
        offset = int(off_str, 16) if off_str.startswith('0x') else int(off_str)
        return {'type': 'convert_addiu_to_ori', 'offsets': [offset]}
    m = re.search(r'convert_addiu_to_ori\s+--offsets\s+([0-9a-fA-Fx,\s]+)', line)
    if m:
        offsets = []
        for o in re.split(r'[\s,]+', m.group(1).strip()):
            if o:
                offsets.append(int(o, 16) if o.startswith('0x') else int(o))
        return {'type': 'convert_addiu_to_ori', 'offsets': offsets}

    # reorder_epilogue --target-order jr_ra,lw_ra,addiu_sp,nop
    m = re.search(r'reorder_epilogue\s+--target-order\s+(\S+)', line)
    if m:
        return {'type': 'reorder_epilogue', 'target_order': m.group(1)}

    # strip_lw_ra_from_branch_delays --offsets X,Y,Z [--sp-offset 0x14] [--allow-uncond] (Tier A3)
    m = re.search(
        r'strip_lw_ra_from_branch_delays\s+--offsets\s+([0-9a-fA-Fx,\s]+?)'
        r'(?:\s+--sp-offset\s+(0x[0-9a-fA-F]+|\d+))?'
        r'(\s+--allow-uncond)?'
        r'(?:\s|$)',
        line,
    )
    if m:
        offsets = []
        for o in re.split(r'[\s,]+', m.group(1).strip()):
            if o:
                offsets.append(int(o, 16) if o.startswith('0x') else int(o))
        result = {'type': 'strip_lw_ra_from_branch_delays', 'offsets': offsets}
        if m.group(2):
            sp_str = m.group(2)
            result['sp_offset'] = int(sp_str, 16) if sp_str.startswith('0x') else int(sp_str)
        if '--allow-uncond' in line:
            result['allow_uncond'] = True
        return result

    # unfold_dead_or_move --offset 0xN --rs REG --rd REG [--allow-nonnop] (Tier A4)
    m = re.search(
        r'unfold_dead_or_move\s+--offset\s+(0x[0-9a-fA-F]+|\d+)'
        r'\s+--rs\s+(\w+)\s+--rd\s+(\w+)',
        line,
    )
    if m:
        off_str = m.group(1)
        offset = int(off_str, 16) if off_str.startswith('0x') else int(off_str)
        return {
            'type': 'unfold_dead_or_move',
            'offset': offset,
            'rs': m.group(2),
            'rd': m.group(3),
            'allow_nonnop': '--allow-nonnop' in line,
        }

    # invert_branch --offsets X Y Z ...
    m = re.search(r'invert_branch\s+--offsets\s+([\d\s,x0-9a-fA-F]+)', line)
    if m:
        offsets_str = m.group(1).strip()
        offsets = []
        for o in re.split(r'[\s,]+', offsets_str):
            if o:
                offsets.append(int(o, 16) if o.startswith('0x') else int(o))
        return {'type': 'invert_branch', 'offsets': offsets}

    # cse_lui_at [--same-symbol-only] [--offsets X[,Y,Z]]
    if 'cse_lui_at' in line:
        step = {'type': 'cse_lui_at',
                'same_symbol_only': '--same-symbol-only' in line,
                'nop_in_place': '--nop-in-place' in line}
        m = re.search(r'cse_lui_at[^\n]*?--offsets\s+([\s0-9a-fA-Fx,]+)', line)
        if m:
            offsets = []
            for o in re.split(r'[\s,]+', m.group(1).strip()):
                if o:
                    offsets.append(int(o, 16) if o.lower().startswith('0x')
                                   else int(o))
            step['offsets'] = offsets
        return step

    # cse_group_lui [--keep-all] (no args)
    if 'cse_group_lui' in line:
        keep_all = '--keep-all' in line
        return {'type': 'cse_group_lui', 'keep_all': keep_all}

    # swap_branch_operands --offsets X[,Y,Z]
    m = re.search(r'swap_branch_operands\s+--offsets\s+([\s0-9a-fA-Fx,]+)', line)
    if m:
        offsets_str = m.group(1).strip()
        offsets = []
        for o in re.split(r'[\s,]+', offsets_str):
            if o:
                offsets.append(int(o, 16) if o.startswith('0x') else int(o))
        return {'type': 'swap_branch_operands', 'offsets': offsets}

    # swap_commutative_operands --offset X [--offset Y]
    if 'swap_commutative_operands' in line:
        offsets = [int(o, 16) if o.startswith('0x') else int(o)
                   for o in re.findall(r'--offset\s+(0x[0-9a-fA-F]+|\d+)', line)]
        if offsets:
            return {'type': 'swap_commutative_operands', 'offsets': offsets}

    # nop_at_offset --offset X
    m = re.search(r'nop_at_offset\s+--offset\s+(0x[0-9a-fA-F]+|\d+)', line)
    if m:
        offset = int(m.group(1), 16) if m.group(1).startswith('0x') else int(m.group(1))
        return {'type': 'nop_at_offset', 'offset': offset}

    # adjust_branch_offset --offset X --imm Y
    m = re.search(r'adjust_branch_offset\s+--offset\s+(0x[0-9a-fA-F]+|\d+)\s+--imm\s+(0x[0-9a-fA-F]+|-?\d+)', line)
    if m:
        offset = int(m.group(1), 16) if m.group(1).startswith('0x') else int(m.group(1))
        imm_str = m.group(2)
        if imm_str.startswith('0x') or imm_str.startswith('-0x'):
            imm = int(imm_str, 16)
        else:
            imm = int(imm_str)
        return {'type': 'adjust_branch_offset', 'offset': offset, 'imm': imm}

    # propagate_stack_to_reg --sw-offset X [--lw-offset Y] [--src-reg R] [--dst-reg R] [--addu]
    if 'propagate_stack_to_reg' in line:
        result = {'type': 'propagate_stack_to_reg'}
        m = re.search(r'--sw-offset\s+(0x[0-9a-fA-F]+|\d+)', line)
        if m:
            result['sw_offset'] = int(m.group(1), 16) if m.group(1).startswith('0x') else int(m.group(1))
        m = re.search(r'--lw-offset\s+(0x[0-9a-fA-F]+|\d+)', line)
        if m:
            result['lw_offset'] = int(m.group(1), 16) if m.group(1).startswith('0x') else int(m.group(1))
        m = re.search(r'--src-reg\s+(\w+)', line)
        if m:
            result['src_reg'] = m.group(1)
        m = re.search(r'--dst-reg\s+(\w+)', line)
        if m:
            result['dst_reg'] = m.group(1)
        if '--addu' in line:
            result['addu'] = True
        if 'sw_offset' in result:
            return result

    # patch_insn_reg_field OFFSET:FIELD:REG (multiple)
    m = re.search(r'patch_insn_reg_field\s+([^\s*][^\*]*)', line)
    if m:
        patches = m.group(1).strip().split()
        return {'type': 'patch_insn_reg_field', 'patches': patches}

    # patch_insn_imm16 OFFSET:WIDTH:VAL (multiple). WIDTH is informational
    # (always 8 for imm16); the underlying tool patches all 16 bits.
    m = re.search(r'patch_insn_imm16\s+([^\s*][^\*]*)', line)
    if m:
        patches = m.group(1).strip().split()
        return {'type': 'patch_insn_imm16', 'patches': patches}

    # patch_insn_word OFFSET:WORD — replace entire instruction word
    m = re.search(r'patch_insn_word\s+(0x[0-9a-fA-F]+):(0x[0-9a-fA-F]+)', line)
    if m:
        return {'type': 'patch_insn_word', 'offset': m.group(1), 'word': m.group(2)}

    # remap_hidden_params
    if 'remap_hidden_params' in line:
        return {'type': 'remap_hidden_params'}

    # strip_arg_homes --offsets 0x04 0x08 ...
    m = re.search(r'strip_arg_homes\s+--offsets\s+(.+)', line)
    if m:
        offsets = []
        for o in m.group(1).strip().split():
            if o.startswith('0x'):
                offsets.append(int(o, 16))
            else:
                offsets.append(int(o))
        return {'type': 'strip_arg_homes', 'offsets': offsets}

    # strip_div_checks
    if 'strip_div_checks' in line:
        return {'type': 'strip_div_checks'}

    # remap_stack_slots --from X --to Y (multiple pairs)
    # OR remap_stack_slots --map 'old:new,old:new,...' (gen_frame_remap output)
    m = re.search(r'remap_stack_slots\s+(.+)', line)
    if m:
        pairs = []
        for pair_match in re.finditer(r'--from\s+(\d+)\s+--to\s+(\d+)', m.group(1)):
            pairs.append((int(pair_match.group(1)), int(pair_match.group(2))))
        if not pairs:
            mm = re.search(r"--map\s+['\"]?([0-9a-fA-FxX:,\s]+)['\"]?", m.group(1))
            if mm:
                for pair in mm.group(1).split(','):
                    pair = pair.strip()
                    if ':' in pair:
                        a, b = pair.split(':')
                        # int(x, 0) accepts decimal and 0x-hex (hex --map
                        # offsets used to fail the old decimal-only regex and
                        # silently no-op the whole directive)
                        pairs.append((int(a.strip(), 0), int(b.strip(), 0)))
        if not pairs and '--map' in m.group(1):
            # The permissive [0-9:,\s]+ regex refuses maps containing negative
            # offsets (gen_frame_remap can emit them for prologue-relative
            # slots), and the whole directive used to silently parse to None —
            # masquerading as a deep codegen residual. Fail loudly instead:
            # split negatives into patch_stack_complete, keep positives here.
            raise ValueError(
                f"remap_stack_slots: unparseable --map in: {line.strip()!r}\n"
                "  (negative offsets are not supported by this tool — handle "
                "the frame-size change with patch_stack_complete and keep "
                "only non-negative old:new pairs in --map)")
        if pairs:
            step = {'type': 'remap_stack_slots', 'pairs': pairs}
            # --at restricts the rewrite to named .text byte offsets (the slot
            # SPLIT case: only SOME references to an offset move).  This used to
            # be dropped silently, which is worse than unsupported -- the
            # directive would look applied and rewrite EVERY reference.
            at = re.search(r'--at\s+([0-9a-fA-FxX,\s]+)', m.group(1))
            if at:
                step['at'] = [int(t, 0) for t in re.findall(
                    r'0x[0-9a-fA-F]+|\d+', at.group(1))]
            if '--keep-frame' in m.group(1):
                step['keep_frame'] = True
            return step

    # rebase_store_to_sp --offset X --base REG
    m = re.search(r'rebase_store_to_sp\s+--offset\s+(0x[0-9a-fA-F]+|\d+)\s+--base\s+(\w+)', line)
    if m:
        offset = int(m.group(1), 16) if m.group(1).startswith('0x') else int(m.group(1))
        return {'type': 'rebase_store_to_sp', 'offset': offset, 'base': m.group(2)}

    # block_reorder --block X --target Y
    m = re.search(r'block_reorder\s+--block\s+(\d+)\s+--target\s+(\d+)', line)
    if m:
        return {'type': 'block_reorder', 'block': int(m.group(1)), 'target': int(m.group(2))}

    # custom_script PATH [extra_args...]
    m = re.search(r'custom_script\s+(\S+)(.*)', line)
    if m:
        extra = m.group(2).strip().split() if m.group(2).strip() else []
        return {'type': 'custom_script', 'script': m.group(1), 'extra_args': extra}

    # asm_intercept SCRIPT_NAME
    m = re.search(r'asm_intercept\s+(\S+)', line)
    if m:
        return {'type': 'asm_intercept', 'script': m.group(1)}

    # prepend_thunk FILE.s or prepend_thunk --insns 0xABCD ...
    m = re.search(r'prepend_thunk\s+--insns\s+((?:0x[0-9a-fA-F]+\s*)+)', line)
    if m:
        insns = [x.strip() for x in m.group(1).split() if x.strip().startswith('0x')]
        return {'type': 'prepend_thunk_insns', 'insns': insns}
    m = re.search(r'prepend_thunk\s+(\S+)', line)
    if m:
        return {'type': 'prepend_thunk', 'file': m.group(1)}

    # append_trailing_instructions --insns 0xABCD ... (cross-prologue at end of function)
    m = re.search(r'append_trailing(?:_instructions)?\s+--insns\s+(.+)', line)
    if m:
        insns = [x.strip() for x in m.group(1).split() if x.strip().startswith('0x')]
        return {'type': 'append_trailing', 'insns': insns}

    return None


def apply_postproc(obj_file: Path, steps: List[Dict], func_addr: int,
                   overlay_name: str, verbose: bool = False,
                   target_asm: Optional[Path] = None) -> bool:
    """Apply PostProc steps to compiled object file.

    Args:
        obj_file: Path to object file to transform
        steps: List of PostProc step dicts from parse_postproc()
        func_addr: Function address for scheduler_normalizer
        overlay_name: Overlay name (e.g., "EVENT") for finding target EXN
        verbose: Print progress messages
        target_asm: Optional path to target ASM file for remap_hidden_params

    Returns:
        True if all steps succeeded, False otherwise
    """
    if not steps:
        return True

    # ── BANNED POSTPROC ENFORCEMENT ──────────────────────────────
    # Block any banned directive before we execute anything.
    check_banned_postproc_steps(
        steps,
        source_path=f"{overlay_name}/{obj_file.stem}",
        strict=True,
    )
    # ─────────────────────────────────────────────────────────────

    # Track whether prepend_thunk has run. Once it runs, the function entry
    # in .text shifts forward by thunk_size, so any subsequent directive that
    # references "offset within the function" (swap_instructions,
    # patch_insn_reg_field) needs to add func_text_offset to translate to
    # .text-relative addressing.
    prepend_thunk_applied = False

    current = obj_file
    for i, step in enumerate(steps):
        step_type = step['type']
        next_obj = obj_file.with_suffix(f'.pp{i}.o')
        cmd = None

        # Resolve func_text_offset lazily — only the steps that need
        # translation will call this.
        def _func_text_offset() -> int:
            return get_func_text_offset(current)

        def _translate_offset(off: int) -> int:
            """Translate function-relative -> .text-relative if needed.

            Offsets in source files are conventionally .text-relative
            (matching nop_at_offset / write_insn_at_offset convention).
            Only translate when prepend_thunk has shifted the function
            entry, or when the offset is below func_text_offset (heuristic:
            function-relative offsets start at 0, .text-relative at fto).
            """
            fto = _func_text_offset()
            if fto > 0 and (prepend_thunk_applied or off < fto):
                return off + fto
            return off

        try:
            if step_type == 'strip_trailing':
                count = step['count']
                nop_only = step.get('nop_only', True)
                cmd = [sys.executable, str(STRIP_TRAILING), str(current), str(next_obj),
                       "--count", str(count)]
                if nop_only:
                    cmd.append("--nop-only")
                if verbose:
                    mode = "nop-only" if nop_only else "any"
                    print(f"    PostProc: strip_trailing --count {count} ({mode})")

            elif step_type == 'strip_leading':
                count = step['count']
                cmd = [sys.executable, str(STRIP_LEADING), str(current), str(next_obj),
                       "--count", str(count)]
                if verbose:
                    print(f"    PostProc: strip_leading --count {count}")

            elif step_type == 'patch_stack':
                cmd = [sys.executable, str(PATCH_STACK), str(current), str(next_obj),
                       "--old", str(step['old']), "--new", str(step['new'])]
                if verbose:
                    print(f"    PostProc: patch_stack --old {step['old']} --new {step['new']}")

            elif step_type == 'pad_trailing':
                count = step['count']
                cmd = [sys.executable, str(PAD_TRAILING_NOP), str(current), str(next_obj),
                       "--count", str(count)]
                if verbose:
                    print(f"    PostProc: pad_trailing_nop --count {count}")

            elif step_type == 'swap_registers':
                # Format swaps as comma-separated pairs
                swaps_str = ','.join(f"{f}:{t}" for f, t in step['swaps'])
                cmd = [sys.executable, str(SWAP_REGISTERS), str(current), str(next_obj),
                       "--swaps", swaps_str]
                if not step.get('bidirectional'):
                    cmd.append("--unidirectional")
                if step.get('exclude_offsets'):
                    cmd.extend(["--exclude-offsets", step['exclude_offsets']])
                if verbose:
                    print(f"    PostProc: swap_registers {swaps_str}")

            elif step_type == 'swap_registers_liveness':
                cmd = [sys.executable, str(SWAP_REGISTERS_LIVENESS), str(current), str(next_obj),
                       "--from", step['from'], "--to", step['to']]
                if verbose:
                    print(f"    PostProc: swap_registers_liveness {step['from']}->{step['to']}")

            elif step_type == 'rename_register_chain':
                cmd = [sys.executable, str(RENAME_REGISTER_CHAIN), str(current), str(next_obj)]
                for chain in step['chains']:
                    cmd.extend(["--chain", chain])
                if step.get('exclude_offsets'):
                    cmd.extend(["--exclude-offsets", step['exclude_offsets']])
                if step.get('byte_range'):
                    cmd.extend(["--range", step['byte_range']])
                for occ in step.get('occurrences', []):
                    cmd.extend(["--occurrence", occ])
                if verbose:
                    excl = f" --exclude-offsets {step['exclude_offsets']}" if step.get('exclude_offsets') else ""
                    rng = f" --range {step['byte_range']}" if step.get('byte_range') else ""
                    occ_str = "".join(
                        f" --occurrence {o}" for o in step.get('occurrences', [])
                    )
                    print(f"    PostProc: rename_register_chain "
                          f"{' '.join(step['chains'])}{excl}{rng}{occ_str}")

            elif step_type == 'scheduler_normalizer':
                target_exn = get_target_exn(overlay_name)
                if target_exn:
                    cmd = [sys.executable, str(SCHEDULER_NORMALIZER),
                           str(current),
                           "--target", str(target_exn),
                           "--address", hex(func_addr),
                           "--base-address", hex(OVERLAY_BASE),  # Overlay base for raw binary
                           "-o", str(next_obj)]
                    if step.get('block_aware'):
                        cmd.append("--block-aware")
                    if step.get('branch_fp'):
                        cmd.append("--branch-fp")
                    if step.get('fence'):
                        cmd.extend(["--fence", step['fence']])
                    if step.get('const_fp'):
                        cmd.append("--const-fp")
                    if step.get('store_fp'):
                        cmd.append("--store-fp")
                    if step.get('load_fp'):
                        cmd.append("--load-fp")
                    if step.get('freeze_epilogue'):
                        cmd.append("--freeze-epilogue")
                    if step.get('reloc_symbol_fp'):
                        cmd.append("--reloc-symbol-fp")
                    if step.get('no_forced_reorder'):
                        cmd.append("--no-forced-reorder")
                    if verbose:
                        print(f"    PostProc: scheduler_normalizer")
                else:
                    if verbose:
                        print(f"    PostProc: scheduler_normalizer (SKIPPED - no target EXN)")
                    continue

            elif step_type == 'convert_ori_to_addiu':
                offsets_str = ','.join(str(o) for o in step['offsets'])
                cmd = [sys.executable, str(CONVERT_ORI_TO_ADDIU), str(current), str(next_obj),
                       "--offsets", offsets_str]
                if verbose:
                    print(f"    PostProc: convert_ori_to_addiu --offsets {offsets_str}")

            elif step_type == 'convert_or_to_addu':
                cmd = [sys.executable, str(CONVERT_OR_TO_ADDU), str(current), str(next_obj)]
                if verbose:
                    print(f"    PostProc: convert_or_to_addu")

            elif step_type == 'swap_instructions':
                o1 = step['offset1']
                o2 = step['offset2']
                # Translate function-relative -> .text-relative when:
                # 1) prepend_thunk has shifted the function entry, OR
                # 2) either offset is below func_text_offset (heuristic:
                #    function-relative offsets start at 0, .text-relative
                #    offsets start at fto=0x10).
                # Same logic as patch_insn_reg_field.
                need_translate = False
                fto = _func_text_offset()
                if fto > 0:
                    if prepend_thunk_applied:
                        need_translate = True
                    elif o1 < fto or o2 < fto:
                        need_translate = True
                if need_translate:
                    o1 += fto
                    o2 += fto
                cmd = [sys.executable, str(SWAP_INSTRUCTIONS),
                       "--offset1", hex(o1),
                       "--offset2", hex(o2),
                       str(current), str(next_obj)]
                if verbose:
                    print(f"    PostProc: swap_instructions --offset1 {hex(o1)} --offset2 {hex(o2)}")

            elif step_type == 'unfill_branch_delay':
                cmd = [sys.executable, str(UNFILL_BRANCH_DELAY),
                       str(current), str(next_obj),
                       "--offset", hex(step['offset'])]
                if verbose:
                    print(f"    PostProc: unfill_branch_delay --offset {hex(step['offset'])}")

            elif step_type == 'unfill_jal_delay':
                # Function-relative offset; tool wiring adds fto.
                off = _translate_offset(step['offset'])
                cmd = [sys.executable, str(UNFILL_JAL_DELAY),
                       str(current), str(next_obj),
                       "--offset", hex(off),
                       "--max-search", str(step['max_search'])]
                if verbose:
                    print(f"    PostProc: unfill_jal_delay --offset {hex(off)} --max-search {step['max_search']}")

            elif step_type == 'hoist_out_of_delay_slot':
                off = _translate_offset(step['offset'])
                cmd = [sys.executable, str(HOIST_OUT_OF_DELAY),
                       str(current), str(next_obj),
                       "--offset", hex(off)]
                if step.get('allow_branch_in_delay'):
                    cmd.append("--allow-branch-in-delay")
                if verbose:
                    print(f"    PostProc: hoist_out_of_delay_slot --offset {hex(off)}"
                          + (" --allow-branch-in-delay" if step.get('allow_branch_in_delay') else ""))

            elif step_type == 'epilogue_share_into_branches':
                # Author writes function-relative offsets; always add fto
                # to translate to .text-relative. (Differs from older tools
                # that only translated when offset < fto.)
                offs = [_translate_offset(o) for o in step['offsets']]
                cmd = [sys.executable, str(EPILOGUE_SHARE),
                       str(current), str(next_obj),
                       "--insn", hex(step['insn']),
                       "--into-offsets", ",".join(hex(o) for o in offs)]
                if step.get('keep_offset'):
                    cmd.append("--keep-offset")
                if step.get('jal_predecessor'):
                    cmd.append("--jal-predecessor")
                if verbose:
                    flag = ""
                    if step.get('keep_offset'):
                        flag += " --keep-offset"
                    if step.get('jal_predecessor'):
                        flag += " --jal-predecessor"
                    print(f"    PostProc: epilogue_share_into_branches --insn {hex(step['insn'])} --into-offsets {','.join(hex(o) for o in offs)}{flag}")

            elif step_type == 'swap_spill_with_move':
                # Function-relative offset; tool wiring adds fto.
                off = _translate_offset(step['offset'])
                cmd = [sys.executable, str(SWAP_SPILL_WITH_MOVE),
                       str(current), str(next_obj),
                       "--offset", hex(off),
                       "--dst", step['dst']]
                if step.get('allow_non_sp'):
                    cmd.append("--allow-non-sp")
                if verbose:
                    print(f"    PostProc: swap_spill_with_move --offset {hex(off)} --dst {step['dst']}")

            elif step_type == 'swap_lui_addr_form':
                # Function-relative offset; tool wiring adds fto.
                off = _translate_offset(step['offset'])
                cmd = [sys.executable, str(SWAP_LUI_ADDR_FORM),
                       str(current), str(next_obj),
                       "--offset", hex(off),
                       "--to", step['to']]
                if verbose:
                    print(f"    PostProc: swap_lui_addr_form --offset {hex(off)} --to {step['to']}")

            elif step_type == 'convert_addiu_to_slti':
                off = _translate_offset(step['offset'])
                cmd = [sys.executable, str(CONVERT_ADDIU_TO_SLTI),
                       str(current), str(next_obj),
                       "--offset", hex(off)]
                if verbose:
                    print(f"    PostProc: convert_addiu_to_slti --offset {hex(off)}")

            elif step_type == 'convert_addiu_to_ori':
                offs = [_translate_offset(o) for o in step['offsets']]
                cmd = [sys.executable, str(CONVERT_ADDIU_TO_ORI),
                       str(current), str(next_obj),
                       "--offsets", ",".join(hex(o) for o in offs)]
                if verbose:
                    print(f"    PostProc: convert_addiu_to_ori --offsets {','.join(hex(o) for o in offs)}")

            elif step_type == 'reorder_epilogue':
                cmd = [sys.executable, str(REORDER_EPILOGUE),
                       str(current), str(next_obj),
                       "--target-order", step['target_order']]
                if verbose:
                    print(f"    PostProc: reorder_epilogue --target-order {step['target_order']}")

            elif step_type == 'strip_lw_ra_from_branch_delays':
                offs = [_translate_offset(o) for o in step['offsets']]
                cmd = [sys.executable, str(STRIP_LW_RA_FROM_BRANCH_DELAYS),
                       str(current), str(next_obj),
                       "--offsets", ",".join(hex(o) for o in offs)]
                if 'sp_offset' in step:
                    cmd.extend(["--sp-offset", hex(step['sp_offset'])])
                if step.get('allow_uncond'):
                    cmd.append("--allow-uncond")
                if verbose:
                    flag = " --allow-uncond" if step.get('allow_uncond') else ""
                    print(f"    PostProc: strip_lw_ra_from_branch_delays "
                          f"--offsets {','.join(hex(o) for o in offs)}{flag}")

            elif step_type == 'unfold_dead_or_move':
                off = _translate_offset(step['offset'])
                cmd = [sys.executable, str(UNFOLD_DEAD_OR_MOVE),
                       str(current), str(next_obj),
                       "--offset", hex(off),
                       "--rs", step['rs'],
                       "--rd", step['rd']]
                if step.get('allow_nonnop'):
                    cmd.append("--allow-nonnop")
                if verbose:
                    print(f"    PostProc: unfold_dead_or_move --offset {hex(off)} "
                          f"--rs {step['rs']} --rd {step['rd']}")

            elif step_type == 'invert_branch':
                cmd = [sys.executable, str(INVERT_BRANCH),
                       str(current), str(next_obj),
                       "--offsets"] + [hex(o) for o in step['offsets']]
                if verbose:
                    print(f"    PostProc: invert_branch --offsets {' '.join(hex(o) for o in step['offsets'])}")

            elif step_type == 'cse_lui_at':
                cmd = [sys.executable, str(CSE_LUI_AT),
                       str(current), str(next_obj)]
                if step.get('same_symbol_only'):
                    cmd.append("--same-symbol-only")
                if step.get('offsets'):
                    cmd += ["--offsets",
                            ",".join(hex(o) for o in step['offsets'])]
                if step.get('nop_in_place'):
                    cmd.append("--nop-in-place")
                if verbose:
                    print(f"    PostProc: cse_lui_at "
                          f"{' '.join(cmd[4:])}".rstrip())

            elif step_type == 'cse_group_lui':
                cmd = [sys.executable, str(CSE_GROUP_LUI),
                       str(current), str(next_obj)]
                if step.get('keep_all'):
                    cmd.append("--keep-all")
                if verbose:
                    extra = " --keep-all" if step.get('keep_all') else ""
                    print(f"    PostProc: cse_group_lui{extra}")

            elif step_type == 'swap_branch_operands':
                offsets = step['offsets']
                xlated = [_translate_offset(o) for o in offsets]
                cmd = [sys.executable, str(SWAP_BRANCH_OPERANDS),
                       str(current), str(next_obj),
                       "--offsets"] + [hex(o) for o in xlated]
                if verbose:
                    print(f"    PostProc: swap_branch_operands --offsets {xlated}")

            elif step_type == 'swap_commutative_operands':
                offsets = [_translate_offset(o) for o in step['offsets']]
                cmd = [sys.executable, str(SWAP_COMMUTATIVE_OPERANDS),
                       str(current), str(next_obj)]
                for o in offsets:
                    cmd.extend(["--offset", hex(o)])
                if verbose:
                    offs = ' '.join(hex(o) for o in offsets)
                    print(f"    PostProc: swap_commutative_operands --offset {offs}")

            elif step_type == 'nop_at_offset':
                cmd = [sys.executable, str(NOP_AT_OFFSET),
                       str(current), str(next_obj),
                       "--offset", hex(step['offset'])]
                if verbose:
                    print(f"    PostProc: nop_at_offset --offset {hex(step['offset'])}")

            elif step_type == 'adjust_branch_offset':
                # Translate function-relative -> .text-relative when
                # prepend_thunk ran or offset is below func_text_offset.
                off = step['offset']
                fto = _func_text_offset()
                need_translate = False
                if fto > 0:
                    if prepend_thunk_applied:
                        need_translate = True
                    elif off < fto:
                        need_translate = True
                if need_translate:
                    off += fto
                cmd = [sys.executable, str(ADJUST_BRANCH_OFFSET),
                       str(current), str(next_obj),
                       "--offset", hex(off),
                       "--imm", hex(step['imm']) if step['imm'] >= 0 else str(step['imm'])]
                if verbose:
                    print(f"    PostProc: adjust_branch_offset --offset {hex(off)} --imm {hex(step['imm'])}")

            elif step_type == 'propagate_stack_to_reg':
                cmd = [sys.executable, str(PROPAGATE_STACK_TO_REG),
                       str(current), str(next_obj),
                       "--sw-offset", hex(step['sw_offset'])]
                if 'lw_offset' in step:
                    cmd.extend(["--lw-offset", hex(step['lw_offset'])])
                if 'src_reg' in step:
                    cmd.extend(["--src-reg", step['src_reg']])
                if 'dst_reg' in step:
                    cmd.extend(["--dst-reg", step['dst_reg']])
                if step.get('addu'):
                    cmd.append("--addu")
                if verbose:
                    extra = []
                    if 'lw_offset' in step:
                        extra.append(f"--lw-offset {hex(step['lw_offset'])}")
                    if 'src_reg' in step:
                        extra.append(f"--src-reg {step['src_reg']}")
                    if 'dst_reg' in step:
                        extra.append(f"--dst-reg {step['dst_reg']}")
                    print(f"    PostProc: propagate_stack_to_reg --sw-offset {hex(step['sw_offset'])} {' '.join(extra)}")

            elif step_type == 'patch_insn_reg_field':
                # Patches are OFFSET:FIELD:REG. If prepend_thunk ran or any
                # offset is below func_text_offset, translate offsets.
                fto = _func_text_offset()
                patches = step['patches']
                # Parse offsets to decide whether to translate
                parsed = []
                for patch in patches:
                    parts = patch.split(':')
                    if len(parts) >= 1:
                        try:
                            v = parts[0]
                            n = int(v, 16) if v.startswith('0x') else int(v)
                            parsed.append((n, parts))
                        except ValueError:
                            parsed.append((None, parts))
                    else:
                        parsed.append((None, parts))
                need_translate = False
                if fto > 0:
                    if prepend_thunk_applied:
                        need_translate = True
                    else:
                        for n, _ in parsed:
                            if n is not None and n < fto:
                                need_translate = True
                                break
                new_patches = []
                if need_translate:
                    for n, parts in parsed:
                        if n is not None:
                            parts = [hex(n + fto)] + parts[1:]
                        new_patches.append(':'.join(parts))
                else:
                    new_patches = patches
                cmd = [sys.executable, str(PATCH_INSN_REG_FIELD)]
                for patch in new_patches:
                    cmd.extend(["--patch", patch])
                cmd.extend([str(current), str(next_obj)])
                if verbose:
                    print(f"    PostProc: patch_insn_reg_field {' '.join(new_patches)}")

            elif step_type == 'patch_insn_imm16':
                # Patches are OFFSET:WIDTH:VAL where WIDTH is informational
                # (always 8 for imm16 — the tool patches the full 16-bit
                # immediate). Translate offsets the same way as
                # patch_insn_reg_field when prepend_thunk applied or the
                # offset is below func_text_offset.
                fto = _func_text_offset()
                patches = step['patches']
                parsed = []
                for patch in patches:
                    parts = patch.split(':')
                    if len(parts) >= 1:
                        try:
                            v = parts[0]
                            n = int(v, 16) if v.startswith('0x') else int(v)
                            parsed.append((n, parts))
                        except ValueError:
                            parsed.append((None, parts))
                    else:
                        parsed.append((None, parts))
                need_translate = False
                if fto > 0:
                    if prepend_thunk_applied:
                        need_translate = True
                    else:
                        for n, _ in parsed:
                            if n is not None and n < fto:
                                need_translate = True
                                break
                new_patches = []
                if need_translate:
                    for n, parts in parsed:
                        if n is not None:
                            parts = [hex(n + fto)] + parts[1:]
                        new_patches.append(':'.join(parts))
                else:
                    new_patches = patches
                # Convert OFFSET:WIDTH:VAL -> --patch-noverify OFFSET:VAL.
                # WIDTH is dropped (informational only).
                cmd = [sys.executable, str(PATCH_INSN_IMM16)]
                for patch in new_patches:
                    parts = patch.split(':')
                    if len(parts) == 3:
                        nv = f"{parts[0]}:{parts[2]}"
                    elif len(parts) == 2:
                        nv = patch
                    else:
                        print(f"    PostProc: patch_insn_imm16 SKIP malformed '{patch}'")
                        continue
                    cmd.extend(["--patch-noverify", nv])
                cmd.extend([str(current), str(next_obj)])
                if verbose:
                    print(f"    PostProc: patch_insn_imm16 {' '.join(new_patches)}")

            elif step_type == 'remap_hidden_params':
                if target_asm and target_asm.exists():
                    cmd = [sys.executable, str(REMAP_HIDDEN_PARAMS),
                           str(current), str(next_obj),
                           "--target-asm", str(target_asm)]
                    if verbose:
                        print(f"    PostProc: remap_hidden_params")
                else:
                    # Try to extract target from EXN
                    target_exn = get_target_exn(overlay_name)
                    if target_exn:
                        # Generate temporary ASM from target
                        tmp_asm = _extract_target_asm(target_exn, func_addr, overlay_name)
                        if tmp_asm:
                            cmd = [sys.executable, str(REMAP_HIDDEN_PARAMS),
                                   str(current), str(next_obj),
                                   "--target-asm", str(tmp_asm)]
                            if verbose:
                                print(f"    PostProc: remap_hidden_params (from EXN)")
                        else:
                            if verbose:
                                print(f"    PostProc: remap_hidden_params (SKIPPED - extraction failed)")
                            continue
                    else:
                        if verbose:
                            print(f"    PostProc: remap_hidden_params (SKIPPED - no target)")
                        continue

            elif step_type == 'patch_insn_word':
                off_str = step['offset']
                off_val = int(off_str, 16) if off_str.startswith('0x') else int(off_str)
                obj_off = _translate_offset(off_val)
                word_str = step['word']
                cmd = [sys.executable, str(PATCH_INSN_WORD),
                       str(current), str(next_obj),
                       '--offset', hex(obj_off),
                       '--word', word_str]
                if verbose:
                    print(f"    PostProc: patch_insn_word {off_str}:{word_str}")

            elif step_type == 'strip_arg_homes':
                offsets = step['offsets']
                cmd = [sys.executable, str(STRIP_ARG_HOMES), str(current), str(next_obj),
                       "--offsets"] + [hex(o) for o in offsets]
                if verbose:
                    offsets_str = ' '.join(hex(o) for o in offsets)
                    print(f"    PostProc: strip_arg_homes --offsets {offsets_str}")

            elif step_type == 'strip_div_checks':
                cmd = [sys.executable, str(STRIP_DIV_CHECKS), str(current), str(next_obj)]
                if verbose:
                    print(f"    PostProc: strip_div_checks")

            elif step_type == 'remap_stack_slots':
                # Format: --map "old:new,old:new,..."
                map_str = ','.join(f"{frm}:{to}" for frm, to in step['pairs'])
                cmd = [sys.executable, str(REMAP_STACK_SLOTS),
                       "--map", map_str,
                       str(current), str(next_obj)]
                if step.get('at'):
                    cmd += ["--at", ','.join(hex(o) for o in step['at'])]
                if step.get('keep_frame'):
                    cmd.append("--keep-frame")
                if verbose:
                    pairs_str = ', '.join(f"{f}->{t}" for f, t in step['pairs'])
                    print(f"    PostProc: remap_stack_slots {pairs_str}")

            elif step_type == 'rebase_store_to_sp':
                cmd = [sys.executable, str(REBASE_STORE_TO_SP), str(current), str(next_obj),
                       "--offset", str(step['offset']), "--base", step['base']]
                if verbose:
                    print(f"    PostProc: rebase_store_to_sp --offset {step['offset']} --base {step['base']}")

            elif step_type == 'block_reorder':
                cmd = [sys.executable, str(BLOCK_REORDER), str(current), str(next_obj),
                       "--block", str(step['block']), "--target", str(step['target'])]
                if verbose:
                    print(f"    PostProc: block_reorder --block {step['block']} --target {step['target']}")

            elif step_type == 'custom_script':
                script_path = _resolve_script_path(step['script'])
                extra_args = step.get('extra_args', [])
                if script_path and script_path.exists():
                    cmd = [sys.executable, str(script_path), str(current), str(next_obj)] + extra_args
                    if verbose:
                        ea_str = ' '.join(extra_args) if extra_args else ''
                        print(f"    PostProc: custom_script {step['script']} {ea_str}")
                else:
                    # FAIL-CLOSED (honesty): a referenced custom_script that
                    # cannot be resolved must NOT be silently skipped. Silently
                    # skipping masks the true match status — a function could
                    # claim byte_perfect while a load-bearing script is absent,
                    # or carry a phantom directive forever. Surface it as an
                    # error (caught by verify_overlay_bytes like a banned step).
                    sys.exit(
                        f"UNRESOLVED custom_script '{step['script']}' "
                        f"({overlay_name}/{obj_file.stem}) — script not found on "
                        f"any search path. Refusing to silently skip; remove the "
                        f"dead directive or restore the script."
                    )

            elif step_type == 'asm_intercept':
                script_name = step['script']
                script_path = _resolve_asm_intercept_script(script_name)
                if script_path and script_path.exists():
                    cmd = [sys.executable, str(ASM_INTERCEPT),
                           "--script", str(script_path),
                           str(current), str(next_obj)]
                    if verbose:
                        print(f"    PostProc: asm_intercept {script_name}")
                else:
                    # FAIL-CLOSED (honesty): see custom_script rationale above.
                    sys.exit(
                        f"UNRESOLVED asm_intercept '{script_name}' "
                        f"({overlay_name}/{obj_file.stem}) — script not found on "
                        f"any search path. Refusing to silently skip."
                    )

            elif step_type == 'prepend_thunk_insns':
                # Direct inline --insns mode
                insns = step['insns']
                cmd = [sys.executable, str(PREPEND_THUNK),
                       str(current), str(next_obj), '--insns'] + insns
                if verbose:
                    print(f"    PostProc: prepend_thunk --insns {' '.join(insns)}")

            elif step_type == 'append_trailing':
                # Append instructions at end of .text (cross-prologue at function end)
                insns = step['insns']
                cmd = [sys.executable, str(APPEND_TRAILING),
                       str(current), str(next_obj), '--insns'] + insns
                if verbose:
                    print(f"    PostProc: append_trailing --insns {' '.join(insns)}")

            elif step_type == 'prepend_thunk':
                thunk_file = _resolve_thunk_file(step['file'], overlay_name)
                if thunk_file and thunk_file.exists():
                    # Read thunk file and extract instruction hex values
                    # Assemble the .s file to get raw bytes
                    try:
                        import tempfile
                        with tempfile.NamedTemporaryFile(suffix='.o', delete=False) as tmp_obj:
                            tmp_obj_path = tmp_obj.name
                        # Assemble thunk using gas
                        asm_cmd = ['mipsel-linux-gnu-as', '-mips1', '-EL',
                                   '-o', tmp_obj_path, str(thunk_file)]
                        asm_result = subprocess.run(asm_cmd, capture_output=True, text=True)
                        if asm_result.returncode != 0:
                            if verbose:
                                print(f"    PostProc: prepend_thunk assemble FAILED: {asm_result.stderr[:200]}")
                            continue
                        # Extract .text bytes from the assembled object
                        objcopy_cmd = ['mipsel-linux-gnu-objcopy', '-O', 'binary',
                                      '-j', '.text', tmp_obj_path, tmp_obj_path + '.bin']
                        subprocess.run(objcopy_cmd, capture_output=True)
                        with open(tmp_obj_path + '.bin', 'rb') as f:
                            thunk_bytes = f.read()
                        os.unlink(tmp_obj_path)
                        os.unlink(tmp_obj_path + '.bin')
                        # Convert to hex instruction list, skip trailing nops
                        insns = []
                        for i in range(0, len(thunk_bytes), 4):
                            word = struct.unpack('<I', thunk_bytes[i:i+4])[0]
                            if word != 0:  # Skip nop padding
                                insns.append(f'0x{word:08x}')
                        if not insns:
                            if verbose:
                                print(f"    PostProc: prepend_thunk {step['file']} (no instructions)")
                            continue
                        cmd = [sys.executable, str(PREPEND_THUNK),
                               str(current), str(next_obj), '--insns'] + insns
                        if verbose:
                            print(f"    PostProc: prepend_thunk {step['file']} ({len(insns)} insns)")
                    except Exception as e:
                        if verbose:
                            print(f"    PostProc: prepend_thunk {step['file']} (ERROR: {e})")
                        continue
                else:
                    if verbose:
                        print(f"    PostProc: prepend_thunk {step['file']} (SKIPPED - not found)")
                    continue

            elif step_type == 'insert_instruction':
                # Function-relative offset; add fto to translate to .text-relative
                obj_offset = _translate_offset(step['offset'])
                cmd = [sys.executable, str(INSERT_INSTRUCTION),
                       str(current), str(next_obj),
                       '--offset', hex(obj_offset),
                       '--insn', hex(step['insn']),
                       '--fix-branches']
                if step.get('grow'):
                    cmd.append('--grow')
                if verbose:
                    grow_flag = " --grow" if step.get('grow') else ""
                    print(f"    PostProc: insert_instruction --offset {hex(step['offset'])} --insn {hex(step['insn'])}{grow_flag}")

            elif step_type == 'write_insn_at_offset':
                # Write instruction(s) at specific .text offset(s) without shifting
                # Offsets are .text-relative (same convention as nop_at_offset)
                # Patches may include a 3rd "expect" field for validation
                cmd = [sys.executable, str(WRITE_INSN_AT_OFFSET),
                       str(current), str(next_obj)]
                for patch in step['patches']:
                    offset = patch[0]
                    insn = patch[1]
                    expect = patch[2] if len(patch) > 2 else None
                    if expect is not None:
                        cmd.extend(['--patch', f'{hex(offset)}:{hex(insn)}:{hex(expect)}'])
                    else:
                        cmd.extend(['--patch', f'{hex(offset)}:{hex(insn)}'])
                if verbose:
                    patches_str = ' --patch '.join(
                        f'{hex(p[0])}:{hex(p[1])}' + (f':{hex(p[2])}' if len(p) > 2 and p[2] is not None else '')
                        for p in step['patches'])
                    print(f"    PostProc: write_insn_at_offset --patch {patches_str}")

            else:
                if verbose:
                    print(f"    PostProc: unknown type {step_type}")
                continue

            # Execute command
            if cmd:
                result = subprocess.run(cmd, capture_output=True, text=True, timeout=60)
                if result.returncode != 0:
                    # ALWAYS loud (D1): a silently-aborted chain makes the
                    # missing downstream steps (patch_stack etc.) look like a
                    # deep frame/codegen residual to every caller.
                    print(f"POSTPROC CHAIN FAILED at step {i+1}/{len(steps)} "
                          f"({step_type}): {(result.stderr or result.stdout)[:300].strip()}",
                          file=sys.stderr)
                    print(f"  -> remaining {len(steps)-i-1} step(s) NOT applied; "
                          f"do NOT read the resulting diff as codegen evidence.",
                          file=sys.stderr)
                    return False
                current = next_obj
                # Mark prepend_thunk applied so subsequent offset-bearing
                # directives translate function-relative offsets correctly.
                if step_type in ('prepend_thunk', 'prepend_thunk_insns'):
                    prepend_thunk_applied = True

        except subprocess.TimeoutExpired:
            print(f"POSTPROC CHAIN FAILED at step {i+1}/{len(steps)} "
                  f"({step_type}): timeout", file=sys.stderr)
            return False
        except Exception as e:
            print(f"POSTPROC CHAIN FAILED at step {i+1}/{len(steps)} "
                  f"({step_type}): {e}", file=sys.stderr)
            return False

    # Copy final result back to original name
    if current != obj_file:
        shutil.copy(current, obj_file)

    return True


def _extract_target_asm(target_exn: Path, func_addr: int, overlay_name: str) -> Optional[Path]:
    """Extract target function bytes and disassemble to temporary ASM file."""
    import tempfile

    # Get function size from sym file
    sym_file = ROOT / "config" / "overlay" / f"{overlay_name}.sym"
    func_size = None
    # The symbol is found by ADDRESS (the entry whose window starts here, else
    # the function whose name -- func_<ADDR>, or a landed overlay name -- is this
    # address): once a function has a real name its address is no longer in it.
    sys.path.insert(0, str(ROOT / "tools" / "overlay"))
    import overlay_names
    func_name = overlay_names.addr_name(overlay_name, func_addr)

    if sym_file.exists():
        with open(sym_file) as f:
            for line in f:
                parts = line.strip().split(':')
                if len(parts) < 3 or line.lstrip().startswith('#'):
                    continue
                try:
                    start = int(parts[0], 16)
                except ValueError:
                    continue
                if start == func_addr or parts[2].strip() == func_name:
                    func_size = int(parts[1])
                    break

    if not func_size:
        return None

    # Extract bytes
    offset = func_addr - OVERLAY_BASE
    try:
        with open(target_exn, 'rb') as f:
            f.seek(offset)
            data = f.read(func_size)
    except Exception:
        return None

    # Write to temp file and disassemble
    tmp_bin = Path(tempfile.mktemp(suffix='.bin'))
    tmp_asm = Path(tempfile.mktemp(suffix='.s'))

    try:
        tmp_bin.write_bytes(data)
        result = subprocess.run(
            ['mipsel-linux-gnu-objdump', '-D', '-b', 'binary',
             '-m', 'mips:3000', f'--adjust-vma={hex(func_addr)}',
             str(tmp_bin)],
            capture_output=True, text=True
        )
        if result.returncode == 0:
            tmp_asm.write_text(result.stdout)
            return tmp_asm
    except Exception:
        pass
    finally:
        if tmp_bin.exists():
            tmp_bin.unlink()

    return None


def _resolve_script_path(script_name: str) -> Optional[Path]:
    """Resolve custom_script path to full path.

    2026-10-03 (red-team finding 1): an ABSOLUTE path used to be returned
    as-is, and a relative one with `..` could climb out of the tree, so a
    script nobody can review (e.g. in /tmp) ran as part of the build.  Both
    are refused: every candidate must resolve inside the repository."""
    if os.path.isabs(script_name) or script_name.startswith('~'):
        return None
    root = ROOT.resolve()

    def _inside(p: Path) -> bool:
        try:
            p.resolve().relative_to(root)
            return True
        except ValueError:
            return False
    for base in (TOOLS_IDO, TOOLS_POSTPROC, ROOT):
        if not _inside(base / script_name):
            return None

    # Check relative to tools/ido
    path = TOOLS_IDO / script_name
    if path.exists():
        return path

    # Check with .py extension
    path = TOOLS_IDO / f"{script_name}.py"
    if path.exists():
        return path

    # Check relative to tools/postproc
    path = TOOLS_POSTPROC / script_name
    if path.exists():
        return path

    path = TOOLS_POSTPROC / f"{script_name}.py"
    if path.exists():
        return path

    # Check relative to project root
    path = ROOT / script_name
    if path.exists():
        return path

    return None


def _resolve_asm_intercept_script(script_name: str) -> Optional[Path]:
    """Resolve asm_intercept script name to full path."""
    # Check for asm_intercept_FUNC.py pattern
    script_file = f"asm_intercept_{script_name}.py"
    path = TOOLS_IDO / script_file
    if path.exists():
        return path

    # Check direct name
    path = TOOLS_IDO / script_name
    if path.exists():
        return path

    path = TOOLS_IDO / f"{script_name}.py"
    if path.exists():
        return path

    return None


def _resolve_thunk_file(file_name: str, overlay_name: str) -> Optional[Path]:
    """Resolve thunk file path."""
    # Check relative to overlay's asm directory
    path = ROOT / "asm" / "overlay" / overlay_name / file_name
    if path.exists():
        return path

    # Check relative to asm/thunks
    path = ROOT / "asm" / "thunks" / file_name
    if path.exists():
        return path

    # Check absolute
    if os.path.isabs(file_name):
        path = Path(file_name)
        if path.exists():
            return path

    return None


def fixup_relocations(func_bytes: bytes, target_bytes: bytes) -> bytes:
    """Fix relocations by copying immediates from target when opcode/regs match.

    When the compiled instruction has the same opcode and register fields as the
    original, copy the original's immediate/offset. This handles:
    - Linker relocations (JAL targets, LUI/ADDIU address pairs)
    - ORI->ADDIU linker conversions
    - Branch offsets

    This is the key step that converts "reloc-only" matches to byte-perfect.
    Ported from tools/build/build_level3_c.py:fixup_relocations()

    Args:
        func_bytes: Compiled function bytes after PostProc
        target_bytes: Original bytes from EXN file

    Returns:
        Fixed bytes with original immediates copied in
    """
    result = bytearray(func_bytes)

    for i in range(0, min(len(func_bytes), len(target_bytes)), 4):
        if i + 4 > len(func_bytes) or i + 4 > len(target_bytes):
            break
        word = struct.unpack('<I', func_bytes[i:i+4])[0]
        orig_word = struct.unpack('<I', target_bytes[i:i+4])[0]

        if word == orig_word:
            continue  # Already matches

        op = (word >> 26) & 0x3F
        orig_op = (orig_word >> 26) & 0x3F

        # JAL instruction (opcode 3): copy entire instruction
        if op == 3 and orig_op == 3:
            result[i:i+4] = target_bytes[i:i+4]

        # J instruction (opcode 2): copy entire instruction
        elif op == 2 and orig_op == 2:
            result[i:i+4] = target_bytes[i:i+4]

        # LUI instruction (opcode 15): copy if rt matches
        elif op == 15 and orig_op == 15:
            if (word & 0x001F0000) == (orig_word & 0x001F0000):
                result[i:i+4] = target_bytes[i:i+4]

        # I-type with same opcode and upper 16 bits (same rs/rt): copy immediate
        # Covers: addiu(9), slti(10), sltiu(11), andi(12), ori(13), xori(14),
        # lb(32), lh(33), lw(35), lbu(36), lhu(37), sb(40), sh(41), sw(43)
        elif op == orig_op and op in (9, 10, 11, 12, 13, 14,
                                      32, 33, 35, 36, 37, 40, 41, 43):
            if (word & 0xFFFF0000) == (orig_word & 0xFFFF0000):
                result[i:i+4] = target_bytes[i:i+4]

        # ORI->ADDIU linker fixup: assembler emits ORI for R_MIPS_LO16,
        # linker converts to ADDIU when low 16 bits are positive
        elif op == 13 and orig_op == 9:  # ORI=13, ADDIU=9
            if (word & 0x03FF0000) == (orig_word & 0x03FF0000):
                result[i:i+4] = target_bytes[i:i+4]

        # BEQ/BNE branch offset fixup
        elif op == orig_op and op in (4, 5):  # BEQ=4, BNE=5
            if (word & 0xFFFF0000) == (orig_word & 0xFFFF0000):
                result[i:i+4] = target_bytes[i:i+4]

    return bytes(result)


def get_func_info_from_sym(overlay_name: str, func_name: str) -> Tuple[int, int]:
    """Get function address and size from overlay symbol file.

    Returns: (address, size) or (0, 0) if not found
    """
    sym_file = ROOT / "config" / "overlay" / f"{overlay_name}.sym"
    if not sym_file.exists():
        return (0, 0)

    with open(sym_file) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            # Format: 0x80132244:232:func_80132244
            parts = line.split(':')
            if len(parts) >= 3 and parts[2] == func_name:
                return (int(parts[0], 16), int(parts[1]))

    return (0, 0)


if __name__ == '__main__':
    # Test parsing
    import argparse
    parser = argparse.ArgumentParser(description='Test overlay PostProc parsing')
    parser.add_argument('c_file', help='C source file to parse')
    args = parser.parse_args()

    c_file = Path(args.c_file)
    if not c_file.exists():
        print(f"File not found: {c_file}")
        sys.exit(1)

    steps = parse_postproc(c_file)
    if steps:
        print(f"Found {len(steps)} PostProc step(s):")
        for i, step in enumerate(steps):
            print(f"  {i+1}. {step}")
    else:
        print("No PostProc annotations found")
