#!/usr/bin/env python3
"""
Parse post-processing requirements from C file headers.

Extracts structured post-processing steps from comments like:
/* Post-processing required:
 *   1. patch_stack_complete.py --old 24 --new 40
 *   2. swap_registers_auto.py --swaps v0:v1
 */
"""

import re
from pathlib import Path
from typing import Iterator, List, Dict, Optional


# A directive counts ONLY when its line OPENS with the directive keyword,
# after nothing but C/C++ comment markers ( "/*", "*", "**", "//" ) and
# whitespace. Header PROSE routinely quotes a directive keyword inline --
# e.g. "disabling loop unrolling (the @CompileFlags: directive above)" or
# " *   - @PostProc: ..." in a bullet list -- and the old naive whole-file
# `re.search` grabbed the FIRST such occurrence anywhere, feeding prose words
# to the compiler ("uget: input file not initialized").
#
# The prefix allows ANY run of comment markers/whitespace (so a malformed but
# genuine "/*  * @PostProc: ..." double-marker line still parses) but rejects
# any line with real prose words before the keyword (a leading "-", "cracked
# by", a quote char, etc.). This mirrors the audit's directive-line discipline
# (audit_fabrication.directive_body), widened only for repeated markers.
_COMMENT_PREFIX = r'^\s*(?:(?:/\*+|\*+|//)\s*)*'


def directive_bodies(content: str, directive: str) -> Iterator[str]:
    """Yield the text following `directive` on every line that OPENS with it.

    `directive` is the full keyword token including its trailing colon, e.g.
    "@CompileFlags:". Lines that merely mention the keyword mid-sentence are
    ignored. Shared by every directive-parsing entry point in the build/test
    path (parse_postproc, overlay_postproc, build_overlay) so prose-immunity is
    guaranteed in one place.
    """
    pat = re.compile(_COMMENT_PREFIX + re.escape(directive) + r'\s*(.*)$')
    for line in content.splitlines():
        m = pat.match(line)
        if m is not None:
            yield m.group(1)


# Backwards-compatible private alias.
_directive_bodies = directive_bodies


def _first_directive_body(content: str, directive: str) -> Optional[str]:
    """Return the body of the first real directive line, or None."""
    for body in _directive_bodies(content, directive):
        return body
    return None


def parse_symbol_start_text(content: str) -> Optional[int]:
    """@SymbolStart from already-read text, ANCHORED to a directive line.

    The compare-window directives are the one class where a prose mention is
    actively dangerous: a header that merely EXPLAINS the fix ("Correct fix:
    ONE C function windowed with @SymbolStart: 0xADDR / @SymbolSize: N")
    silently re-windows the byte compare for a file that never adopted it.
    Use this (or parse_symbol_start) everywhere a window is derived.
    """
    body = _first_directive_body(content, '@SymbolStart:')
    if body is None:
        return None
    m = re.search(r'(0x[0-9A-Fa-f]+)', body)
    return int(m.group(1), 16) if m else None


def parse_symbol_size_text(content: str) -> Optional[int]:
    """@SymbolSize from already-read text, ANCHORED to a directive line."""
    body = _first_directive_body(content, '@SymbolSize:')
    if body is None:
        return None
    m = re.search(r'(\d+)', body)
    return int(m.group(1)) if m else None


def parse_compiler(c_file_path: Path) -> str:
    """Parse compiler annotation from C file header.

    Looks for a line like:
        * @Compiler: gcc260
        * @Compiler: ido52

    Returns:
        Compiler identifier string: "ido52" (default), "gcc260", "gcc272",
        "gcc281", or the hybrids "ido52-aspsx" / "ido52-gas" (IDO codegen
        assembled PSY-Q style through gccwrap --codegen ido52).  If no @Compiler annotation is found, returns "ido52"
        for backward compatibility.
    """
    VALID_COMPILERS = {"ido52", "ido53", "gcc260", "gcc263", "gcc263p", "gcc272", "gcc281",
                       "ido52-aspsx", "ido52-gas"}

    try:
        with open(c_file_path, 'r', encoding='utf-8') as f:
            content = f.read()
    except Exception as e:
        print(f"Warning: Could not read {c_file_path}: {e}")
        return "ido52"

    body = _first_directive_body(content, '@Compiler:')
    if body is None:
        return "ido52"
    match = re.search(r'(\S+)', body)
    if not match:
        return "ido52"

    compiler = match.group(1).strip().rstrip('*').rstrip('/').strip()
    if compiler not in VALID_COMPILERS:
        print(f"Warning: Unknown @Compiler value '{compiler}' in {c_file_path}, defaulting to ido52")
        return "ido52"

    return compiler


def parse_symbol_start(c_file_path: Path) -> Optional[int]:
    """Parse @SymbolStart override from C file header.

    Looks for a line like:
        * @SymbolStart: 0x80139ECC

    SANCTIONED boundary recovery (NOT painting): the .sym file draws this
    function's start 1-2 instructions LATE because IDO parked its leading
    setup (li/lui for a hoisted global store) in the prior function's dead
    epilogue space. @SymbolStart sets the function's TRUE start so the compare
    window covers the hoisted setup the compiler genuinely emits from
    globals-first C. The producer (prior function) trims its tail with
    @SymbolSize so no bytes are double-counted.

    Returns the address as an int, or None if no @SymbolStart annotation.
    """
    try:
        with open(c_file_path, 'r', encoding='utf-8') as f:
            content = f.read()
    except Exception as e:
        print(f"Warning: Could not read {c_file_path}: {e}")
        return None

    body = _first_directive_body(content, '@SymbolStart:')
    if body is None:
        return None
    match = re.search(r'(0x[0-9A-Fa-f]+)', body)
    if not match:
        return None
    return int(match.group(1), 16)


def parse_symbol_size(c_file_path: Path) -> Optional[int]:
    """Parse @SymbolSize override from C file header.

    Looks for a line like:
        * @SymbolSize: 72

    Overrides the function's byte size for the compare window. Used to (a)
    trim trailing foreign words an over-counting .sym pulled in, or (b)
    extend the window when paired with @SymbolStart to cover hoisted leading
    setup. Self-validating: it can only window the symbol's true extent, it
    cannot fabricate bytes.

    Returns the size in bytes as an int, or None if no @SymbolSize annotation.
    """
    try:
        with open(c_file_path, 'r', encoding='utf-8') as f:
            content = f.read()
    except Exception as e:
        print(f"Warning: Could not read {c_file_path}: {e}")
        return None

    body = _first_directive_body(content, '@SymbolSize:')
    if body is None:
        return None
    match = re.search(r'(\d+)', body)
    if not match:
        return None
    return int(match.group(1))


def parse_compile_flags(c_file_path: Path) -> List[str]:
    """Parse per-function compiler flags from C file header.

    Looks for a line like:
        * @CompileFlags: -Wo,-loopunroll,0

    Returns:
        List of extra compiler flags (e.g., ['-Wo,-loopunroll,0']),
        or empty list if none specified.
    """
    try:
        with open(c_file_path, 'r', encoding='utf-8') as f:
            # Read enough bytes to cover large @PostProc blocks.
            # Kept bounded to avoid loading entire files unnecessarily.
            content = f.read()
    except Exception as e:
        print(f"Warning: Could not read {c_file_path}: {e}")
        return []

    # Look for a real @CompileFlags: directive line (not a prose mention).
    body = _first_directive_body(content, '@CompileFlags:')
    if body is None:
        return []

    flags_str = body.strip()
    # Remove trailing comment artifacts like */ or *
    flags_str = re.sub(r'\s*\*+/?\s*$', '', flags_str)
    if not flags_str:
        return []

    # Split on whitespace to get individual flags
    return flags_str.split()


def parse_compile_env(c_file_path: Path) -> Dict[str, str]:
    """Parse per-function environment variables from C file header.

    Looks for a line like:
        * @CompileEnv: IDO52_CONST_NOINREG_VALUES=32 IDO52_LDA_NOINREG_CLASSES=0

    Returns:
        Dict of environment variable name -> value pairs,
        or empty dict if none specified.
    """
    try:
        with open(c_file_path, 'r', encoding='utf-8') as f:
            content = f.read()
    except Exception as e:
        print(f"Warning: Could not read {c_file_path}: {e}")
        return {}

    # Look for ALL real @CompileEnv: directive lines (not prose mentions).
    # Multiple lines are merged so callers can split env vars across separate
    # annotations.
    matches = list(_directive_bodies(content, '@CompileEnv:'))
    if not matches:
        return {}

    env_vars = {}
    for raw in matches:
        env_str = re.sub(r'\s*\*+/?\s*$', '', raw.strip())
        if not env_str:
            continue
        for token in env_str.split():
            if '=' in token:
                key, value = token.split('=', 1)
                env_vars[key] = value

    # 2026-10-03 (red-team finding 5): IDO52_SKIP_PASSES=...codemotion
    # MISCOMPILES (deletes loads; the shrink is a count compensator --
    # CLAUDE.md, UOPT-HOOKS-REFERENCE.md Hook 15) and nothing refused it.
    if 'codemotion' in env_vars.get('IDO52_SKIP_PASSES', ''):
        raise SystemExit(
            "BANNED @CompileEnv in {}: IDO52_SKIP_PASSES={} -- codemotion "
            "miscompiles (Hook 15); remove it.".format(c_file_path, env_vars['IDO52_SKIP_PASSES']))
    return env_vars


def _parse_postproc_line(line: str) -> Optional[Dict]:
    """Parse a single post-processing directive line.

    Handles both long format (from Post-processing block) and
    shorthand format (from @PostProc: annotations).
    """
    if not line:
        return None

    # rotate_dispatch
    if 'rotate_dispatch' in line:
        result = {'type': 'rotate_dispatch'}
        m = re.search(r'--move-from\s+(0x[0-9a-fA-F]+|\d+)', line)
        if m:
            result['move_from'] = int(m.group(1), 0)
        m = re.search(r'--move-to\s+(0x[0-9a-fA-F]+|\d+)', line)
        if m:
            result['move_to'] = int(m.group(1), 0)
        m = re.search(r'--fix-beq\s+(0x[0-9a-fA-F]+|\d+)', line)
        if m:
            result['fix_beq'] = int(m.group(1), 0)
        m = re.search(r'--add-nop\s+(0x[0-9a-fA-F]+|\d+)', line)
        if m:
            result['add_nop'] = int(m.group(1), 0)
        if 'move_from' in result and 'move_to' in result:
            return result
        return None

    # patch_frame_full (must check before patch_frame)
    if 'patch_frame_full' in line:
        m = re.search(r'--old[_-]?(?:size)?\s+(\d+)\s+--new[_-]?(?:size)?\s+(\d+)', line)
        if m:
            return {'type': 'patch_frame_full', 'old_size': int(m.group(1)), 'new_size': int(m.group(2))}
        return None

    # patch_stack_complete / patch_stack / patch_frame
    if 'patch_stack' in line or 'patch_frame' in line:
        # Long: patch_stack_complete --old 24 --new 40 [--shift-ra]
        m = re.search(r'--old[_-]?(?:size)?\s+(\d+)\s+--new[_-]?(?:size)?\s+(\d+)', line)
        if m:
            result = {'type': 'patch_frame', 'old_size': int(m.group(1)), 'new_size': int(m.group(2))}
            if '--shift-ra' in line:
                result['shift_ra'] = True
            return result
        # Short: patch_stack 24 40
        m = re.search(r'patch_stack(?:_complete)?\s+(\d+)\s+(\d+)', line)
        if m:
            result = {'type': 'patch_frame', 'old_size': int(m.group(1)), 'new_size': int(m.group(2))}
            if '--shift-ra' in line:
                result['shift_ra'] = True
            return result
        return None

    # swap_registers_liveness (must check before swap_registers)
    if 'swap_registers_liveness' in line:
        # Long: swap_registers_liveness --target-exe X --address 0xY
        m_addr = re.search(r'--address\s+(0x[0-9a-fA-F]+)', line)
        if m_addr:
            return {'type': 'swap_registers_liveness', 'address': int(m_addr.group(1), 16)}
        # Short: swap_registers_liveness a0->v1
        m = re.search(r'swap_registers_liveness\s+(\w+)\s*->\s*(\w+)', line)
        if m:
            return {'type': 'swap_registers_liveness_arrow', 'from_reg': m.group(1), 'to_reg': m.group(2)}
        return None

    # swap_registers / swap_registers_auto
    if 'swap_registers' in line:
        # Parse optional --exclude offsets
        exclude_offsets = None
        m_excl = re.search(r'--exclude(?:-offsets)?\s+([\s0-9a-fA-Fx,]+)', line)
        if m_excl:
            offsets_str = m_excl.group(1).strip()
            exclude_offsets = [x.strip() for x in re.findall(r'0x[0-9a-fA-F]+|\d+', offsets_str)]
            exclude_offsets = ','.join(exclude_offsets) if exclude_offsets else None

        # Parse optional --only offsets
        only_offsets = None
        m_only = re.search(r'--only\s+([\s0-9a-fA-Fx,]+)', line)
        if m_only:
            offsets_str = m_only.group(1).strip()
            only_offsets = [x.strip() for x in re.findall(r'0x[0-9a-fA-F]+|\d+', offsets_str)]
            only_offsets = ','.join(only_offsets) if only_offsets else None

        bidirectional = '--bidirectional' in line

        def _make_swap(from_reg, to_reg):
            result = {'type': 'swap_registers', 'from_reg': from_reg, 'to_reg': to_reg}
            if exclude_offsets:
                result['exclude_offsets'] = exclude_offsets
            if only_offsets:
                result['only_offsets'] = only_offsets
            if bidirectional:
                result['bidirectional'] = True
            return result

        # Format: --swaps v0:v1
        m = re.search(r'--swaps?\s+(\w+):(\w+)', line)
        if m:
            return _make_swap(m.group(1), m.group(2))
        # Format: --from X --to Y
        m_from = re.search(r'--from\s+(\w+)', line)
        m_to = re.search(r'--to\s+(\w+)', line)
        if m_from and m_to:
            return _make_swap(m_from.group(1), m_to.group(1))
        # Format with colon: swap_registers a0:v0 (may have --unidirectional or --exclude)
        m = re.search(r'swap_registers(?:_auto)?\s+(\w+):(\w+)', line)
        if m:
            return _make_swap(m.group(1), m.group(2))
        # Short format: swap_registers a0 t6
        m = re.search(r'swap_registers(?:_auto)?\s+(\w+)\s+(\w+)', line)
        if m and m.group(1) not in ('--swaps', '--from'):
            return _make_swap(m.group(1), m.group(2))
        return None

    # collapse_shift_move
    if 'collapse_shift_move' in line:
        result = {'type': 'collapse_shift_move'}
        m = re.search(r'--ra-offset\s+(\d+)\s+(\d+)', line)
        if m:
            result['old_ra_offset'] = int(m.group(1))
            result['new_ra_offset'] = int(m.group(2))
        return result

    # fix_sll_sra_pattern
    if 'fix_sll_sra_pattern' in line:
        return {'type': 'fix_sll_sra_pattern'}

    # fix_andi_pattern
    if 'fix_andi_pattern' in line:
        return {'type': 'fix_andi_pattern'}

    # patch_nor_operands
    if 'patch_nor_operands' in line:
        return {'type': 'patch_nor_operands'}

    # adjust_branch_offset --offset 0x28 --imm 0x0f
    if 'adjust_branch_offset' in line:
        import re as _re
        m_off = _re.search(r'--offset\s+(0x[0-9a-fA-F]+|\d+)', line)
        m_imm = _re.search(r'--imm\s+(0x[0-9a-fA-F]+|\d+)', line)
        if m_off and m_imm:
            return {
                'type': 'adjust_branch_offset',
                'offset': int(m_off.group(1), 0),
                'imm': int(m_imm.group(1), 0)
            }

    # swap_commutative_operands --offset 0x54 [--offset 0x68]
    if 'swap_commutative_operands' in line:
        import re as _re
        offsets = [int(x, 0) for x in _re.findall(r'--offset\s+(0x[0-9a-fA-F]+|\d+)', line)]
        if not offsets:
            # Shorthand: swap_commutative_operands 0x54
            args_str = line.split('swap_commutative_operands')[-1].strip()
            offsets = [int(x, 0) for x in _re.findall(r'0x[0-9a-fA-F]+|\d+', args_str)]
        return {'type': 'swap_commutative_operands', 'offsets': offsets}

    # strip_instructions (--nop and/or --remove at specific offsets)
    if 'strip_instructions' in line and 'strip_instructions.' not in line and 'leading' not in line:
        result = {'type': 'strip_instructions'}
        m_nop = re.search(r'--nop\s+([\s0-9a-fA-Fx,]+)', line)
        if m_nop:
            offsets_str = m_nop.group(1).strip()
            result['nop_offsets'] = [int(x.strip(), 0) for x in re.findall(r'0x[0-9a-fA-F]+|\d+', offsets_str)]
        m_rem = re.search(r'--remove\s+([\s0-9a-fA-Fx,]+)', line)
        if m_rem:
            offsets_str = m_rem.group(1).strip()
            result['remove_offsets'] = [int(x.strip(), 0) for x in re.findall(r'0x[0-9a-fA-F]+|\d+', offsets_str)]
        return result

    # strip_div_checks
    if 'strip_div_checks' in line:
        return {'type': 'strip_div_checks'}

    # strip_interior_nops
    if 'strip_interior_nops' in line:
        return {'type': 'strip_interior_nops'}

    # strip_leading_instructions
    if 'strip_leading_instructions' in line:
        # Long: strip_leading_instructions --count 2 (or --count auto)
        m = re.search(r'--count\s+(\d+|auto)', line)
        if m:
            c = m.group(1)
            return {'type': 'strip_leading_instructions',
                    'count': c if c == 'auto' else int(c)}
        # Short: strip_leading_instructions 2 (or ... auto)
        m = re.search(r'strip_leading_instructions\s+(\d+|auto)', line)
        if m:
            c = m.group(1)
            return {'type': 'strip_leading_instructions',
                    'count': c if c == 'auto' else int(c)}
        return None

    # strip_trailing_instructions
    if 'strip_trailing_instructions' in line:
        m = re.search(r'--count\s+(\d+)', line)
        count = int(m.group(1)) if m else 1
        nop_only = '--nop-only' in line
        return {'type': 'strip_trailing_instructions', 'count': count, 'nop_only': nop_only}

    # convert_ori_to_addiu
    if 'convert_ori_to_addiu' in line:
        m = re.search(r'--offsets\s+([\s0-9a-fA-Fx,]+)', line)
        if m:
            offsets_str = m.group(1).strip()
            offsets = [int(x.strip(), 0) for x in re.findall(r'0x[0-9a-fA-F]+|\d+', offsets_str)]
            return {'type': 'convert_ori_to_addiu', 'offsets': offsets}
        return {'type': 'convert_ori_to_addiu', 'offsets': []}

    # convert_or_to_addu
    if 'convert_or_to_addu' in line or 'or to addu' in line.lower():
        result = {'type': 'convert_or_to_addu'}
        m_excl = re.search(r'--exclude(?:-offsets)?\s+([\s0-9a-fA-Fx,]+)', line)
        if m_excl:
            offsets_str = m_excl.group(1).strip()
            offsets = [x.strip() for x in re.findall(r'0x[0-9a-fA-F]+|\d+', offsets_str)]
            if offsets:
                result['exclude_offsets'] = ','.join(offsets)
        return result

    # swap_instructions
    if 'swap_instructions' in line or 'swap instructions' in line.lower():
        # Long: --offset1 0x10 --offset2 0x14
        m = re.search(r'--offset1\s+(0x[0-9a-fA-F]+|\d+)\s+--offset2\s+(0x[0-9a-fA-F]+|\d+)', line)
        if m:
            return {'type': 'swap_instructions', 'offset1': int(m.group(1), 0), 'offset2': int(m.group(2), 0)}
        # Short: swap_instructions 0x4 0x8
        m = re.search(r'swap_instructions\s+(0x[0-9a-fA-F]+|\d+)\s+(0x[0-9a-fA-F]+|\d+)', line)
        if m:
            return {'type': 'swap_instructions', 'offset1': int(m.group(1), 0), 'offset2': int(m.group(2), 0)}
        # Legacy: offset 0x10 and 0x14
        m = re.search(r'offset(?:s)?\s+0x([0-9a-fA-F]+)\s+and\s+0x([0-9a-fA-F]+)', line)
        if m:
            return {'type': 'swap_instructions', 'offset1': int(m.group(1), 16), 'offset2': int(m.group(2), 16)}
        return None

    # patch_ra_offset
    if 'patch_ra_offset' in line:
        # Long: --old 20 --new 16
        m = re.search(r'--old\s+(\d+)\s+--new\s+(\d+)', line)
        if m:
            return {'type': 'patch_ra_offset', 'old_offset': int(m.group(1)), 'new_offset': int(m.group(2))}
        # Short: patch_ra_offset 20 16
        m = re.search(r'patch_ra_offset\s+(\d+)\s+(\d+)', line)
        if m:
            return {'type': 'patch_ra_offset', 'old_offset': int(m.group(1)), 'new_offset': int(m.group(2))}
        return None

    # cse_lui_at [--same-symbol-only] [--offsets X[,Y,Z]]
    if 'cse_lui_at' in line:
        step = {'type': 'cse_lui_at',
                'same_symbol_only': '--same-symbol-only' in line}
        m = re.search(r'cse_lui_at[^\n]*?--offsets\s+([\s0-9a-fA-Fx,]+)', line)
        if m:
            offsets = []
            for o in re.split(r'[\s,]+', m.group(1).strip()):
                if o:
                    offsets.append(int(o, 16) if o.lower().startswith('0x')
                                   else int(o))
            step['offsets'] = offsets
        return step

    # scheduler_normalizer [--address 0xXXXXXXXX] [--block-aware]
    if 'scheduler_normalizer' in line:
        result = {'type': 'scheduler_normalizer'}
        addr_match = re.search(r'--address\s+(0x[0-9a-fA-F]+)', line)
        if addr_match:
            result['address'] = int(addr_match.group(1), 16)
        if '--block-aware' in line:
            result['block_aware'] = True
        return result

    # rename_register_chain
    if 'rename_register_chain' in line:
        chains = re.findall(r'--chain\s+([\w:]+)', line)
        if chains:
            return {'type': 'rename_register_chain', 'chains': chains}
        # Short: rename_register_chain v0:t8:t9
        m = re.search(r'rename_register_chain\s+([\w:]+)', line)
        if m:
            return {'type': 'rename_register_chain', 'chains': [m.group(1)]}
        return None

    # remap_stack_slots
    if 'remap_stack_slots' in line:
        # Format: remap_stack_slots --map '24:40' or remap_stack_slots 24:40
        # The unquoted value is ONLY slot-map characters.  It used to be
        # `[^'"]+`, which is greedy and swallowed any following flag into the
        # map -- `--map 84:76 --keep-frame` parsed as the map "84:76
        # --keep-frame", parse_slot_map then threw, and test_single_match
        # reported that as a WARNING while still printing a score.  That is
        # indistinguishable from "applied, did nothing", so a directive could
        # look live and do nothing at all (found 2026-09-20).
        m = re.search(r"--map\s+'([^']+)'", line) or re.search(r'--map\s+"([^"]+)"', line)
        if not m:
            m = re.search(r'--map\s+([\d:,]+)', line)
        if not m:
            m = re.search(r'remap_stack_slots\s+([\d:,]+)', line)
        if m:
            return {'type': 'remap_stack_slots', 'map': m.group(1).strip(),
                    'keep_frame': '--keep-frame' in line}
        return None

    # patch_load_signedness
    if 'patch_load_signedness' in line:
        m = re.search(r'--offsets\s+([\s0-9a-fA-Fx,]+)', line)
        if m:
            offsets_str = m.group(1).strip()
            offsets = [int(x.strip(), 0) for x in re.findall(r'0x[0-9a-fA-F]+|\d+', offsets_str)]
            return {'type': 'patch_load_signedness', 'offsets': offsets}
        m = re.search(r'patch_load_signedness\s+([\s0-9a-fA-Fx,]+)', line)
        if m:
            offsets_str = m.group(1).strip()
            offsets = [int(x.strip(), 0) for x in re.findall(r'0x[0-9a-fA-F]+|\d+', offsets_str)]
            return {'type': 'patch_load_signedness', 'offsets': offsets}
        return None

    # shift_stack_offsets
    if 'shift_stack_offsets' in line:
        m = re.search(r'--old\s+(\d+)\s+--new\s+(\d+)', line)
        if m:
            return {'type': 'shift_stack_offsets', 'old_size': int(m.group(1)), 'new_size': int(m.group(2))}
        m = re.search(r'shift_stack_offsets\s+(\d+)\s+(\d+)', line)
        if m:
            return {'type': 'shift_stack_offsets', 'old_size': int(m.group(1)), 'new_size': int(m.group(2))}
        return None

    # unfill_branch_delay
    if 'unfill_branch_delay' in line:
        m = re.search(r'--offset\s+(0x[0-9a-fA-F]+|\d+)', line)
        if m:
            return {'type': 'unfill_branch_delay', 'offset': int(m.group(1), 0)}
        m = re.search(r'unfill_branch_delay\s+(0x[0-9a-fA-F]+|\d+)', line)
        if m:
            return {'type': 'unfill_branch_delay', 'offset': int(m.group(1), 0)}
        return None

    # fill_delay_slots (target-aware delay slot filler)
    if 'fill_delay_slots' in line and 'fill_gcc_delay_slots' not in line:
        return {'type': 'fill_delay_slots'}

    # fill_gcc_delay_slots (GCC object delay-slot filler with selectable scopes)
    if 'fill_gcc_delay_slots' in line:
        result = {'type': 'fill_gcc_delay_slots'}
        for flag in ('fill-jalr', 'fill-epilogue', 'fill-j',
                     'fill-jal-store', 'fill-branch-store', 'fill-prev', 'fill-earliest', 'fill-load-delay', 'fill-all'):
            if re.search(r'--' + flag + r'(?![\w-])', line):
                result[flag.replace('-', '_')] = True
        return result

    # swap_lui_addr_form (toggle %lo folded-load <-> separate addiu address form)
    if 'swap_lui_addr_form' in line:
        m = re.search(r'--offset\s+(0x[0-9a-fA-F]+|\d+)', line)
        m_to = re.search(r'--to\s+(ori|addiu)', line)
        if m and m_to:
            return {'type': 'swap_lui_addr_form',
                    'offset': int(m.group(1), 0), 'to': m_to.group(1)}
        return None

    # insert_move_and_shift
    if 'insert_move_and_shift' in line:
        m = re.search(r'--offset\s+(0x[0-9a-fA-F]+|\d+)', line)
        m_move = re.search(r'--move-reg\s+(\w+)', line)
        m_src = re.search(r'--src-reg\s+(\w+)', line)
        m_dst = re.search(r'--dst-reg\s+(\w+)', line)
        m_insn = re.search(r'--insn\s+(0x[0-9a-fA-F]+|\d+)', line)
        result = {'type': 'insert_move_and_shift'}
        if m:
            result['offset'] = int(m.group(1), 0)
        if m_move:
            result['move_reg'] = m_move.group(1)
        if m_src:
            result['src_reg'] = m_src.group(1)
        if m_dst:
            result['dst_reg'] = m_dst.group(1)
        if m_insn:
            result['insn'] = int(m_insn.group(1), 0)
        return result

    # propagate_stack_to_reg
    if 'propagate_stack_to_reg' in line:
        result = {'type': 'propagate_stack_to_reg'}
        m_sw = re.search(r'--sw-offset\s+(0x[0-9a-fA-F]+|\d+)', line)
        if m_sw:
            result['sw_offset'] = int(m_sw.group(1), 0)
        m_lw = re.search(r'--lw-offset\s+(0x[0-9a-fA-F]+|\d+)', line)
        if m_lw:
            result['lw_offset'] = int(m_lw.group(1), 0)
        m_dst = re.search(r'--dst-reg\s+(\w+)', line)
        if m_dst:
            result['dst_reg'] = m_dst.group(1)
        m_src = re.search(r'--src-reg\s+(\w+)', line)
        if m_src:
            result['src_reg'] = m_src.group(1)
        if '--addu' in line:
            result['addu'] = True
        if 'sw_offset' not in result:
            return None
        return result

    # strip_arg_homes
    if 'strip_arg_homes' in line:
        m = re.search(r'--offsets\s+([\s0-9a-fA-Fx,]+)', line)
        if m:
            offsets_str = m.group(1).strip()
            offsets = [int(x.strip(), 0) for x in re.findall(r'0x[0-9a-fA-F]+|\d+', offsets_str)]
            result = {'type': 'strip_arg_homes', 'offsets': offsets}
            if '--shrink' in line:
                result['shrink'] = True
            return result
        m = re.search(r'strip_arg_homes\s+([\s0-9a-fA-Fx,]+)', line)
        if m:
            offsets_str = m.group(1).strip()
            offsets = [int(x.strip(), 0) for x in re.findall(r'0x[0-9a-fA-F]+|\d+', offsets_str)]
            result = {'type': 'strip_arg_homes', 'offsets': offsets}
            if '--shrink' in line:
                result['shrink'] = True
            return result
        return None

    # insert_instruction_and_shift
    if 'insert_instruction_and_shift' in line:
        m_off = re.search(r'--offset\s+(0x[0-9a-fA-F]+|\d+)', line)
        m_insn = re.search(r'--insn\s+(0x[0-9a-fA-F]+|\d+)', line)
        result = {'type': 'insert_instruction_and_shift'}
        if m_off:
            result['offset'] = int(m_off.group(1), 0)
        if m_insn:
            result['insn'] = int(m_insn.group(1), 0)
        if '--grow' in line:
            result['grow'] = True
        if '--fix-branches' in line:
            result['fix_branches'] = True
        # Shorthand: insert_instruction_and_shift 0x48 0x24a50001 [--grow]
        if not m_off and not m_insn:
            args_str = line.split('insert_instruction_and_shift')[-1].strip()
            parts = args_str.replace('--grow', '').replace('--fix-branches', '').split()
            if len(parts) >= 2:
                try:
                    result['offset'] = int(parts[0], 0)
                    result['insn'] = int(parts[1], 0)
                except ValueError:
                    pass
        return result

    # swap_branch_operands
    if 'swap_branch_operands' in line:
        m = re.search(r'--offsets\s+([\s0-9a-fA-Fx,]+)', line)
        if m:
            offsets_str = m.group(1).strip()
            offsets = [int(x.strip(), 0) for x in re.findall(r'0x[0-9a-fA-F]+|\d+', offsets_str)]
            return {'type': 'swap_branch_operands', 'offsets': offsets}
        m = re.search(r'swap_branch_operands\s+([\s0-9a-fA-Fx,]+)', line)
        if m:
            offsets_str = m.group(1).strip()
            offsets = [int(x.strip(), 0) for x in re.findall(r'0x[0-9a-fA-F]+|\d+', offsets_str)]
            return {'type': 'swap_branch_operands', 'offsets': offsets}
        return None

    # fix_prologue_and_delays (GCC post-processing)
    if 'fix_prologue_and_delays' in line:
        return {'type': 'fix_prologue_and_delays'}

    # maspsx (handled by gccwrap, no-op in build system)
    if 'maspsx' in line and 'fix' not in line:
        return {'type': 'maspsx_noop'}

    # invert_branch
    if 'invert_branch' in line:
        m = re.search(r'--offsets\s+([\s0-9a-fA-Fx,]+)', line)
        if m:
            offsets_str = m.group(1).strip()
            offsets = [int(x.strip(), 0) for x in re.findall(r'0x[0-9a-fA-F]+|\d+', offsets_str)]
            return {'type': 'invert_branch', 'offsets': offsets}
        m = re.search(r'invert_branch\s+([\s0-9a-fA-Fx,]+)', line)
        if m:
            offsets_str = m.group(1).strip()
            offsets = [int(x.strip(), 0) for x in re.findall(r'0x[0-9a-fA-F]+|\d+', offsets_str)]
            return {'type': 'invert_branch', 'offsets': offsets}
        return None

    # patch_store_zero
    if 'patch_store_zero' in line:
        m_off = re.search(r'--offset\s+(0x[0-9a-fA-F]+|\d+)', line)
        m_reg = re.search(r'--register\s+(\w+)', line)
        if m_off and m_reg:
            return {'type': 'patch_store_zero', 'offset': int(m_off.group(1), 0), 'register': m_reg.group(1)}
        return None

    # patch_insn_reg_field
    if 'patch_insn_reg_field' in line:
        patches = re.findall(r'--patch\s+([\w:x]+)', line)
        if patches:
            return {'type': 'patch_insn_reg_field', 'patches': patches}
        # Short format: patch_insn_reg_field 0x34:rt:v0:v1 0x3c:rs:v0:v1
        m = re.findall(r'(0x[0-9a-fA-F]+:\w+:\w+:\w+)', line)
        if m:
            return {'type': 'patch_insn_reg_field', 'patches': m}
        return None

    # add_leaf_frame
    if 'add_leaf_frame' in line:
        m = re.search(r'--frame-size\s+(\d+)', line)
        result = {'type': 'add_leaf_frame'}
        if m:
            result['frame_size'] = int(m.group(1))
        return result

    # rebase_store_to_sp
    if 'rebase_store_to_sp' in line:
        m_off = re.search(r'--offset\s+(0x[0-9a-fA-F]+|\d+)', line)
        m_reg = re.search(r'--base-reg\s+(\w+)', line)
        m_delta = re.search(r'--base-delta\s+(\d+)', line)
        if m_off and m_reg and m_delta:
            return {
                'type': 'rebase_store_to_sp',
                'offset': int(m_off.group(1), 0),
                'base_reg': m_reg.group(1),
                'base_delta': int(m_delta.group(1))
            }
        return None

    # fix_sign_test_pattern
    if 'fix_sign_test_pattern' in line:
        m_reg = re.search(r'--dest-reg\s+(\w+)', line)
        result = {'type': 'fix_sign_test_pattern'}
        if m_reg:
            result['dest_reg'] = m_reg.group(1)
        return result

    # prepend_thunk --insns 0xAABBCCDD 0xEEFF0011
    if 'prepend_thunk' in line:
        m = re.search(r'prepend_thunk\s+--insns\s+(.+)', line)
        if m:
            insns = re.findall(r'0x[0-9a-fA-F]+', m.group(1))
            if insns:
                return {'type': 'prepend_thunk', 'insns': insns}
        return None

    # custom_script (per-function postproc script)
    # Supports: custom_script script.py [extra_args...]
    if 'custom_script' in line:
        m = re.search(r'custom_script\s+(\S+)(.*)', line)
        if m:
            extra = m.group(2).strip().split() if m.group(2).strip() else []
            return {'type': 'custom_script', 'script': m.group(1), 'extra_args': extra}
        return None

    # Frame size patching with arrow: 24 -> 40
    if 'frame size' in line.lower() and '->' in line:
        m = re.search(r'(\d+)\s*->\s*(\d+)', line)
        if m:
            return {'type': 'patch_frame', 'old_size': int(m.group(1)), 'new_size': int(m.group(2))}

    return None


def parse_postproc_header(c_file_path: Path) -> Optional[List[Dict]]:
    """Parse post-processing requirements from C file header.

    Returns:
        List of post-processing step dicts, or None if no requirements found.
        Each dict has:
        - 'type': 'patch_frame', 'swap_registers', 'convert_or_to_addu', 'fix_sll_sra_pattern',
                  'fix_andi_pattern', 'strip_interior_nops', 'strip_leading_instructions',
                  'strip_trailing_instructions', 'swap_instructions', 'patch_nor_operands', 'manual'
        - type-specific fields
    """
    try:
        with open(c_file_path, 'r', encoding='utf-8') as f:
            # Read enough bytes to cover large @PostProc blocks.
            # Kept bounded to avoid loading entire files unnecessarily.
            content = f.read()
    except Exception as e:
        print(f"Warning: Could not read {c_file_path}: {e}")
        return None

    steps = []

    # Method 1: Parse @PostProc: annotations (preferred format).
    # Only real directive lines -- a header that PROSE-quotes a
    # "@PostProc: pad_trailing_nop ..." fake stub must not be parsed as live.
    postproc_lines = list(_directive_bodies(content, '@PostProc:'))
    if postproc_lines:
        for line in postproc_lines:
            # Clean trailing comment artifacts
            line = re.sub(r'\s*\*+/?\s*$', '', line).strip()
            step = _parse_postproc_line(line)
            if step:
                steps.append(step)
        return steps if steps else None

    # Method 2: Parse "Post-processing required:" block (legacy format)
    match = re.search(r'Post-processing(?:\s+required|\s+pipeline[^:]*)?:(.*?)(?:\*\s*\n\s*\*\s*(?:Match status|Compilation|Original|Disassembly)|\*/)',
                      content, re.DOTALL | re.IGNORECASE)
    if not match:
        return None

    postproc_text = match.group(1)

    # Extract each numbered step or line
    lines = [line.strip() for line in postproc_text.split('\n') if line.strip()]

    for line in lines:
        # Remove leading * and numbering
        line = re.sub(r'^\*\s*(?:\d+\.)?\s*', '', line)
        if not line or line.startswith('Match status') or line.startswith('Example'):
            continue

        # Pattern: patch_stack_complete.py --old X --new Y
        if 'patch_stack_complete' in line or 'patch_frame' in line:
            m = re.search(r'--old[_-]?(?:size)?\s+(\d+)\s+--new[_-]?(?:size)?\s+(\d+)', line)
            if m:
                step = {
                    'type': 'patch_frame',
                    'old_size': int(m.group(1)),
                    'new_size': int(m.group(2))
                }
                if '--shift-ra' in line:
                    step['shift_ra'] = True
                steps.append(step)

        # Pattern: swap_registers_liveness.py --target-exe X --address 0xY
        elif 'swap_registers_liveness' in line:
            m_addr = re.search(r'--address\s+(0x[0-9a-fA-F]+)', line)
            if m_addr:
                steps.append({
                    'type': 'swap_registers_liveness',
                    'address': int(m_addr.group(1), 16)
                })

        # Pattern: swap_registers_auto.py --swaps v0:v1 or swap_registers.py --from X --to Y
        elif 'swap_registers' in line:
            # Try --swaps format first
            m = re.search(r'--swaps?\s+(\w+):(\w+)', line)
            if m:
                steps.append({
                    'type': 'swap_registers',
                    'from_reg': m.group(1),
                    'to_reg': m.group(2)
                })
            else:
                # Try --from/--to format
                m_from = re.search(r'--from\s+(\w+)', line)
                m_to = re.search(r'--to\s+(\w+)', line)
                if m_from and m_to:
                    steps.append({
                        'type': 'swap_registers',
                        'from_reg': m_from.group(1),
                        'to_reg': m_to.group(1)
                    })

        # Pattern: fix_sll_sra_pattern.py
        elif 'fix_sll_sra_pattern' in line:
            steps.append({
                'type': 'fix_sll_sra_pattern'
            })

        # Pattern: fix_andi_pattern.py
        elif 'fix_andi_pattern' in line:
            steps.append({
                'type': 'fix_andi_pattern'
            })

        # Pattern: patch_nor_operands.py
        elif 'patch_nor_operands' in line:
            steps.append({
                'type': 'patch_nor_operands'
            })

        # Pattern: custom_script <script_name.py> [extra_args...]
        elif 'custom_script' in line:
            m = re.search(r'custom_script\s+(\S+)(.*)', line)
            if m:
                extra = m.group(2).strip().split() if m.group(2).strip() else []
                steps.append({
                    'type': 'custom_script',
                    'script': m.group(1),
                    'extra_args': extra
                })

        # Pattern: strip_interior_nops.py
        elif 'strip_interior_nops' in line:
            steps.append({
                'type': 'strip_interior_nops'
            })

        # Pattern: strip_leading_instructions.py --count N
        elif 'strip_leading_instructions' in line:
            m = re.search(r'--count\s+(\d+)', line)
            if m:
                steps.append({
                    'type': 'strip_leading_instructions',
                    'count': int(m.group(1))
                })
            else:
                m = re.search(r'strip_leading_instructions\s+(\d+)', line)
                if m:
                    steps.append({
                        'type': 'strip_leading_instructions',
                        'count': int(m.group(1))
                    })

        # Pattern: convert_ori_to_addiu.py --offsets 0x18 0x24 0x98
        elif 'convert_ori_to_addiu' in line:
            m = re.search(r'--offsets\s+([\s0-9a-fA-Fx,]+)', line)
            if m:
                offsets_str = m.group(1).strip()
                offsets = [int(x.strip(), 0) for x in re.findall(r'0x[0-9a-fA-F]+|\d+', offsets_str)]
                steps.append({
                    'type': 'convert_ori_to_addiu',
                    'offsets': offsets
                })
            else:
                steps.append({
                    'type': 'convert_ori_to_addiu',
                    'offsets': []
                })

        # Pattern: convert_or_to_addu.py
        elif 'convert_or_to_addu' in line or 'or to addu' in line.lower():
            steps.append({
                'type': 'convert_or_to_addu'
            })

        # Pattern: Swap instructions at offset 0x10 and 0x14
        elif 'swap instructions' in line.lower() or 'instruction reordering' in line.lower():
            m = re.search(r'offset(?:s)?\s+0x([0-9a-fA-F]+)\s+and\s+0x([0-9a-fA-F]+)', line)
            if m:
                steps.append({
                    'type': 'swap_instructions',
                    'offset1': int(m.group(1), 16),
                    'offset2': int(m.group(2), 16)
                })
            else:
                # Complex reordering - mark as manual
                steps.append({
                    'type': 'manual',
                    'description': line
                })

        # Pattern: patch_ra_offset.py --old X --new Y
        elif 'patch_ra_offset' in line:
            m = re.search(r'--old\s+(\d+)\s+--new\s+(\d+)', line)
            if m:
                steps.append({
                    'type': 'patch_ra_offset',
                    'old_offset': int(m.group(1)),
                    'new_offset': int(m.group(2))
                })

        # Pattern: Patch offsets X: change Y to Z
        elif 'patch offset' in line.lower() and 'change' in line.lower():
            steps.append({
                'type': 'manual',
                'description': line
            })

        # Frame size patching with offset changes
        elif 'frame size' in line.lower() and '->' in line:
            m = re.search(r'(\d+)\s*->\s*(\d+)', line)
            if m:
                steps.append({
                    'type': 'patch_frame',
                    'old_size': int(m.group(1)),
                    'new_size': int(m.group(2))
                })

    return steps if steps else None


def main():
    """Test the parser on all matched C files."""
    import sys
    from pathlib import Path

    root_dir = Path(__file__).parent.parent.parent
    matched_dir = root_dir / 'src' / 'main' / 'matched'

    files_with_postproc = []

    for c_file in sorted(matched_dir.glob('*.c')):
        steps = parse_postproc_header(c_file)
        if steps:
            files_with_postproc.append((c_file.name, steps))

    print(f"Found {len(files_with_postproc)} C files with post-processing requirements:\n")

    for filename, steps in files_with_postproc:
        print(f"{filename}:")
        for i, step in enumerate(steps, 1):
            print(f"  {i}. {step['type']}: {step}")
        print()

    print(f"\nTotal: {len(files_with_postproc)} files need post-processing")


if __name__ == '__main__':
    main()
