#!/usr/bin/env python3
"""
Build a machine-readable blocker database from:
  - config/c_unmatchable.txt (free-text blocker annotations)
  - src/main/semi-matched/*.c (header comments)
  - src/main/wip/*.c (header comments)

Outputs JSON to config/blocker_database.json.
"""

import json
import os
import re
import sys
from collections import Counter
from pathlib import Path

# Blocker categories and their detection patterns (order matters for priority)
BLOCKER_PATTERNS = [
    # (category, list of regex patterns to match in blocker text)
    ("HIDDEN_REGS", [
        r"hidden.*(?:t\d|v[01]|a[0-3]|at|\$at|HI|LO|mflo|mfhi)",
        r"hidden.*reg",
        r"hidden.*param",
        r"hidden.*live.?in",
        r"(?:t[0-9]|v[01]) (?:used |read )before write",
        r"non.?standard ABI",
        r"hidden_[vt]reg",
        r"hidden_at",
        r"hidden_hilo",
        r"\$?ra used as data",
        r"ra.as.data",
        r"cross.prologue.*mflo",
    ]),
    ("DUAL_ENTRY", [
        r"dual.?entry",
        r"dual_entry_thunk",
        r"two entry",
        r"thunk.*falls? through",
        r"no prologue.*uses.*parent",
        r"code fragment inside",
        r"overlapping code",
        r"internal callable subroutine",
    ]),
    ("BANNED", [
        r"^BANNED\b",
        r"BANNED \(",
        r"all \d+ instructions were patched",
        r"^Previously used insert_instruction_and_shift",
        r"^Wholesale instruction replacement",
        r"previously.*BANNED",
    ]),
    ("STRENGTH_REDUCTION", [
        r"strength.?reduc",
        r"voice\*168",
        r"voice.*168",
        r"i\*168",
        r"\*168 offset",
        r"eliminduct",
        r"ugen.*stride",
        r"negu.*stride",
        r"induction variable",
        r"loop strength reduction",
    ]),
    ("IDO_CSE", [
        r"IDO.*CSE",
        r"CSE.?merge",
        r"CSE.?break",
        r"CSE instruction count",
        r"CSE.*offset.*gap",
        r"constant.?fold",
        r"const.?fold",
        r"IDO.*constant.fold",
        r"cfe constant.fold",
        r"CSE_MAX_REUSE",
        r"CSE.*reuse",
        r"recompute.*offset.*times",
    ]),
    ("LICM", [
        r"LICM",
        r"loop.?invariant",
        r"address.of.*hoist",
        r"hoist.*out of.*loop",
        r"loop.*address.*setup",
    ]),
    ("ANDI_ELIM", [
        r"andi.*0x[Ff]{4}",
        r"andi.*ffff",
        r"zero.?extension.*eliminat",
        r"redundant andi",
        r"IDO.*eliminat.*andi",
        r"lhu.*zero.?ext",
    ]),
    ("SREG_ALLOC", [
        r"s.?reg.*alloc",
        r"register alloc",
        r"s.?register.*promot",
        r"IDO.*promotes.*s\d",
        r"s.?reg.*structure",
        r"s.?reg.*mismatch",
        r"frame.*mismatch.*s.reg",
        r"callee.saved.*mismatch",
        r"too few s.?reg",
        r"too many s.?reg",
        r"cannot prevent.*s.reg",
        r"s\d.*vs.*s\d.*allocation",
        r"different.*s.reg.*count",
    ]),
    ("INSN_COUNT_GAP", [
        r"instruction count.*gap",
        r"instruction count.*cannot",
        r"\d+ compiled vs \d+ target",
        r"\d+ extra instruction",
        r"\d+.insn gap",
        r"\d+.instruction gap",
        r"insn.*gap cannot be bridged",
        r"cannot be bridged by any allowed PostProc",
    ]),
    ("DEAD_STORE", [
        r"dead.?store",
        r"dead code.*artifact",
        r"unreachable.*instruction",
        r"dead.*sll",
        r"dead.*insn",
        r"intermediate dead store",
        r"IDO.*eliminat.*dead",
        r"nordstore",
    ]),
    ("DELAY_SLOT", [
        r"delay.?slot.*(?:fill|hoist|unhoist|duplicat)",
        r"unfilled.*(?:nop|delay)",
        r"load.?delay",
        r"branch delay slot",
        r"IDO fills.*delay",
        r"delay slot scheduling",
    ]),
    ("JUMP_TABLE", [
        r"jump.?table",
        r"switch.*table.*(?:wrong|library|rodata)",
        r"IDO.*requires \d\+.*cases",
        r"backward.dispatch",
    ]),
    ("CROSS_PROLOGUE", [
        r"cross.?prologue(?!.*hidden)",
        r"pre.?prologue(?!.*hidden)",
        r"before.*prologue.*addiu sp",
        r"cross.prologue.*lui.*s\d",
        r"base.pointer.*global.*pattern",
    ]),
    ("LAZY_SAVE", [
        r"lazy.?save",
        r"deferred.*save",
        r"saves.*AFTER.*branch",
        r"separate epilogue path",
        r"dual.epilogue",
        r"duplicate epilogue",
    ]),
    ("GCC_ASPSX", [
        r"GCC.*(?:blocker|mismatch|cannot)",
        r"ASPSX.*(?:hazard|delay|scheduling)",
        r"maspsx.*(?:mismatch|cannot)",
        r"GCC.*instruction count",
        r"GCC.*wrong.*(?:structure|frame|count)",
        r"confirmed GCC.*unmatchable",
    ]),
    ("UNALIGNED_ACCESS", [
        r"lwl|lwr|swl|swr",
        r"unaligned.*(?:memory|access|copy)",
    ]),
    ("OVERLAP_MISATTR", [
        r"MISATTRIBUT",
        r"OVERLAP",
        r"wrong.*address",
        r"falls? inside.*matched",
    ]),
]

# Known function families
FAMILY_PATTERNS = {
    "voice168": [
        r"voice\*168", r"voice.*168", r"i\*168",
        r"func_8008FFF[48]", r"func_8008FF0[8C]", r"func_800901C[48C]",
        r"func_800902[CD]0", r"SsSeqClose", r"Snd_nextseq",
        r"func_8009465[C0]", r"GetMetaEvent", r"func_800908F[8C]",
        r"SsSeqCalledTbyT",
    ],
    "cd_hw": [
        r"Cd(?:ControlB|Mix|Reset)", r"CD_(?:init|sync)",
    ],
    "andi_ffff": [
        r"andi.*0xffff.*eliminat", r"redundant andi",
        r"CdControlB", r"_spu_r", r"_spu_writeByIO",
    ],
    "dual_entry_game": [
        r"dec_bg_show", r"vram_full_move", r"func_80067490",
        r"func_8005DF34", r"func_80066700", r"pad_scan",
        r"cdq_command_process",
    ],
    "sound_sequencer": [
        r"StCdInterrupt", r"Snd_crescendo", r"change_yoshio",
        r"SD_BranchCTRL", r"func_80087208", r"MDEC_vlc2",
    ],
    "sprite_tile": [
        r"srn_set", r"mod_set_gpu", r"mod_set",
        r"mod_trans_vram",
    ],
    "heap_alloc": [
        r"_gcSPU", r"SpuMalloc", r"func_80097D7[4C]",
    ],
    "licm_address": [
        r"parameter_show_init", r"config_screen_init",
        r"schedule_mark_on",
    ],
}

GAME_REGION_END = 0x80086000

def extract_address(func_name, text=""):
    """Extract hex address from function name or text."""
    m = re.search(r'func_(8[0-9a-fA-F]{7})', func_name)
    if m:
        return f"0x{m.group(1).upper()}"

    m = re.search(r'Address:\s*(0x[0-9a-fA-F]+)', text)
    if m:
        return m.group(1)

    return None


def determine_region(address_str):
    """Determine game vs library region from address."""
    if not address_str:
        return "unknown"
    try:
        addr = int(address_str, 16)
        return "game" if addr < GAME_REGION_END else "library"
    except ValueError:
        return "unknown"


def classify_blocker(text):
    """Classify blocker text into primary and secondary categories."""
    text_lower = text.lower()
    categories = []

    for category, patterns in BLOCKER_PATTERNS:
        for pattern in patterns:
            if re.search(pattern, text, re.IGNORECASE):
                if category not in categories:
                    categories.append(category)
                break

    if not categories:
        categories = ["OTHER"]

    return categories[0], categories[1:]


def detect_family(func_name, text):
    """Detect function family based on name and blocker text."""
    combined = func_name + " " + text
    for family, patterns in FAMILY_PATTERNS.items():
        for pattern in patterns:
            if re.search(pattern, combined, re.IGNORECASE):
                return family
    return None


def parse_unmatchable_txt(filepath):
    """Parse config/c_unmatchable.txt into structured entries."""
    entries = {}

    with open(filepath, 'r') as f:
        for line in f:
            line = line.strip()

            # Skip empty lines, pure comments, section headers
            if not line or line.startswith('# ---') or line.startswith('# func_') and line.startswith('# '):
                # Check for commented-out MATCHED entries
                if line.startswith('#') and 'MATCHED' in line:
                    continue
                if line.startswith('#'):
                    continue
                continue

            # Skip commented-out (matched) entries
            if line.startswith('#'):
                continue

            # Parse: func_name    # comment
            m = re.match(r'^(\S+)\s*#\s*(.*)', line)
            if not m:
                # Bare function name with no comment
                m = re.match(r'^(\S+)\s*$', line)
                if m:
                    func_name = m.group(1)
                    comment = ""
                else:
                    continue
            else:
                func_name = m.group(1)
                comment = m.group(2).strip()

            address = extract_address(func_name, comment)

            # Try to find address in comment if not in name
            if not address:
                addr_m = re.search(r'0x(8[0-9a-fA-F]{7})', comment)
                if addr_m:
                    address = f"0x{addr_m.group(1).upper()}"

            region = determine_region(address)
            primary, secondary = classify_blocker(comment)
            family = detect_family(func_name, comment)

            # Check for source files
            source_file = None
            for d in ['src/main/semi-matched', 'src/main/wip']:
                candidate = os.path.join(d, f"{func_name}.c")
                if os.path.exists(os.path.join(os.path.dirname(filepath), '..', candidate)):
                    source_file = candidate
                    break

            entries[func_name] = {
                "func_name": func_name,
                "address": address,
                "status": "unmatchable",
                "primary_blocker": primary,
                "secondary_blockers": secondary,
                "blocker_detail": comment,
                "family": family,
                "region": region,
                "source_file": source_file,
            }

    return entries


def parse_source_headers(directory, status, project_root):
    """Parse C file headers for blocker information."""
    entries = {}
    dir_path = os.path.join(project_root, directory)

    if not os.path.isdir(dir_path):
        return entries

    for filename in sorted(os.listdir(dir_path)):
        if not filename.endswith('.c'):
            continue

        func_name = filename[:-2]  # strip .c
        filepath = os.path.join(dir_path, filename)

        # Read first 60 lines for header comment
        header_lines = []
        with open(filepath, 'r') as f:
            for i, line in enumerate(f):
                if i >= 60:
                    break
                header_lines.append(line)

        header_text = ''.join(header_lines)

        # Extract address
        address = extract_address(func_name, header_text)
        if not address:
            addr_m = re.search(r'Address:\s*(0x[0-9a-fA-F]+)', header_text)
            if addr_m:
                address = addr_m.group(1)

        region = determine_region(address)

        # Extract blocker detail from header
        blocker_detail = ""
        blocker_lines = []

        for line in header_lines:
            line_s = line.strip().lstrip('* ').strip()
            if re.search(r'BLOCK|UNMATCH|OBSTACLE|STATUS.*(?:WIP|BLOCKED|SEMI)', line_s, re.IGNORECASE):
                blocker_lines.append(line_s)
            elif blocker_lines and line_s and not line_s.startswith('@') and not line_s.startswith('#include'):
                # Continue collecting blocker description
                if re.match(r'^[A-Z0-9]', line_s) or line_s.startswith('-') or line_s.startswith('('):
                    blocker_lines.append(line_s)
                elif len(blocker_lines) < 10:
                    blocker_lines.append(line_s)

        blocker_detail = ' '.join(blocker_lines).strip()

        # If no explicit blocker lines found, use entire header as context
        if not blocker_detail:
            blocker_detail = ' '.join(
                l.strip().lstrip('* ').strip()
                for l in header_lines
                if l.strip().lstrip('* ').strip()
                and not l.strip().startswith('#include')
                and not l.strip().startswith('@')
            )[:500]

        primary, secondary = classify_blocker(blocker_detail + " " + header_text)
        family = detect_family(func_name, blocker_detail + " " + header_text)

        source_file = os.path.join(directory, filename)

        entries[func_name] = {
            "func_name": func_name,
            "address": address,
            "status": status,
            "primary_blocker": primary,
            "secondary_blockers": secondary,
            "blocker_detail": blocker_detail[:800],
            "family": family,
            "region": region,
            "source_file": source_file,
        }

    return entries


def main():
    project_root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

    unmatchable_path = os.path.join(project_root, 'config', 'c_unmatchable.txt')

    # Parse all sources
    print("Parsing config/c_unmatchable.txt...")
    unmatchable = parse_unmatchable_txt(unmatchable_path)
    print(f"  Found {len(unmatchable)} unmatchable entries")

    print("Scanning src/main/semi-matched/...")
    semi = parse_source_headers('src/main/semi-matched', 'semi-matched', project_root)
    print(f"  Found {len(semi)} semi-matched entries")

    print("Scanning src/main/wip/...")
    wip = parse_source_headers('src/main/wip', 'wip', project_root)
    print(f"  Found {len(wip)} WIP entries")

    # Merge: unmatchable entries take priority, then semi-matched, then WIP
    database = {}

    # Add WIP first (lowest priority)
    for name, entry in wip.items():
        database[name] = entry

    # Semi-matched overrides WIP
    for name, entry in semi.items():
        if name in database:
            # If already in database (from WIP), update status but keep WIP data
            # that semi-matched might lack
            existing = database[name]
            entry["blocker_detail"] = entry["blocker_detail"] or existing.get("blocker_detail", "")
        database[name] = entry

    # Unmatchable entries override everything, but merge source_file info
    for name, entry in unmatchable.items():
        if name in database:
            existing = database[name]
            # Keep source file from semi-matched/WIP if unmatchable doesn't have one
            if not entry["source_file"] and existing.get("source_file"):
                entry["source_file"] = existing["source_file"]
            # Keep address from source header if unmatchable doesn't have one
            if not entry["address"] and existing.get("address"):
                entry["address"] = existing["address"]
                entry["region"] = existing["region"]
            # Merge blocker detail
            if existing.get("blocker_detail") and entry["blocker_detail"]:
                # Keep unmatchable's detail as primary
                pass
            elif existing.get("blocker_detail") and not entry["blocker_detail"]:
                entry["blocker_detail"] = existing["blocker_detail"]
            # Track if there's a semi-matched C file
            if existing.get("status") == "semi-matched":
                entry["has_semi_matched_source"] = True
            elif existing.get("status") == "wip":
                entry["has_wip_source"] = True
        database[name] = entry

    # Post-process: fix source_file paths by checking existence
    for name, entry in database.items():
        if not entry["source_file"]:
            for d in ['src/main/semi-matched', 'src/main/wip', 'src/main/matched']:
                candidate = os.path.join(d, f"{name}.c")
                if os.path.exists(os.path.join(project_root, candidate)):
                    entry["source_file"] = candidate
                    break

    # Output
    output_path = os.path.join(project_root, 'config', 'blocker_database.json')
    with open(output_path, 'w') as f:
        json.dump(database, f, indent=2, sort_keys=False)

    print(f"\nWrote {len(database)} entries to {output_path}")

    # Summary statistics
    print("\n=== Summary Statistics ===")
    print(f"Total entries: {len(database)}")

    status_counts = Counter(e['status'] for e in database.values())
    print(f"\nBy status:")
    for status, count in status_counts.most_common():
        print(f"  {status}: {count}")

    blocker_counts = Counter(e['primary_blocker'] for e in database.values())
    print(f"\nBy primary blocker:")
    for blocker, count in blocker_counts.most_common():
        print(f"  {blocker}: {count}")

    region_counts = Counter(e['region'] for e in database.values())
    print(f"\nBy region:")
    for region, count in region_counts.most_common():
        print(f"  {region}: {count}")

    family_counts = Counter(e['family'] for e in database.values() if e['family'])
    if family_counts:
        print(f"\nBy family:")
        for family, count in family_counts.most_common():
            print(f"  {family}: {count}")

    # Show entries with multiple blockers
    multi = [(n, e) for n, e in database.items() if len(e['secondary_blockers']) >= 2]
    if multi:
        print(f"\nFunctions with 3+ blocker categories: {len(multi)}")
        for name, entry in sorted(multi, key=lambda x: -len(x[1]['secondary_blockers']))[:10]:
            all_blockers = [entry['primary_blocker']] + entry['secondary_blockers']
            print(f"  {name}: {', '.join(all_blockers)}")


if __name__ == '__main__':
    main()
