#!/usr/bin/env python3
"""
Triage MEDIUM confidence symbol matches by cross-referencing
symbol names against source file content patterns.

Reads: build/symbol_match/symbol_match_results.json
       config/level3_functions.json
       asm/main/matched/*.s, src/main/matched/*.c
Writes: build/symbol_match/medium_triage.md
"""

import json
import os
import re
import sys
from collections import defaultdict

BASE_DIR = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def load_data():
    """Load symbol match results and function config."""
    with open(os.path.join(BASE_DIR, "build/symbol_match/symbol_match_results.json")) as f:
        results = json.load(f)

    with open(os.path.join(BASE_DIR, "config/level3_functions.json")) as f:
        funcs_data = json.load(f)

    # Build address -> function info map
    addr_map = {}
    for func in funcs_data["functions"]:
        addr_map[func["address"]] = func

    # Build anchor lookup for neighborhood checking
    anchors = {}
    for a in results.get("anchors", []):
        anchors[a["retail_addr"]] = a

    return results, addr_map, anchors


def read_source_file(filepath, max_lines=80):
    """Read first N lines of a source file."""
    full_path = os.path.join(BASE_DIR, filepath)
    if not os.path.exists(full_path):
        return None, []
    try:
        with open(full_path, "r", errors="replace") as f:
            lines = []
            for i, line in enumerate(f):
                if i >= max_lines:
                    break
                lines.append(line.rstrip())
        return "\n".join(lines), lines
    except Exception:
        return None, []


def extract_jal_targets(lines):
    """Extract JAL (function call) target addresses from ASM."""
    targets = []
    for line in lines:
        m = re.search(r'\bjal\s+(?:0x)?([0-9a-fA-F]+)', line)
        if m:
            targets.append(int(m.group(1), 16))
        # Also match jal with func_ names
        m = re.search(r'\bjal\s+func_([0-9a-fA-F]+)', line)
        if m:
            targets.append(int(m.group(1), 16))
    return targets


def extract_jal_targets_c(lines):
    """Extract function call targets from C source."""
    targets = []
    for line in lines:
        # Match func_XXXXXXXX( calls
        for m in re.finditer(r'func_([0-9a-fA-F]+)\s*\(', line):
            targets.append(int(m.group(1), 16))
    return targets


def extract_global_refs(lines):
    """Extract global memory references (lui + load/store patterns)."""
    refs = []
    for line in lines:
        # Match lui with hex constant like 0x800E, 0x800F, 0x8013 etc.
        m = re.search(r'lui\s+\$?\w+,\s*(?:0x)?([0-9a-fA-F]+)', line)
        if m:
            val = int(m.group(1), 16)
            if 0x8000 <= val <= 0x801F:
                refs.append(val)
        # Match D_800XXXXX or similar data labels
        for m2 in re.finditer(r'D_([0-9a-fA-F]{8})', line):
            refs.append(int(m2.group(1), 16) >> 16)
    return refs


def extract_comment_keywords(lines):
    """Extract useful keywords from file comments/headers."""
    keywords = []
    for line in lines[:20]:
        # Get comment content
        line_lower = line.lower()
        keywords.extend(re.findall(r'[a-z_]+', line_lower))
    return keywords


def classify_symbol_domain(sym_name):
    """Classify a symbol name into a domain category."""
    name_lower = sym_name.lower()

    domains = []

    # Sound/audio domain
    if any(x in name_lower for x in ["sd_", "ss", "spu", "snd_", "sound",
                                       "noise", "reverb", "keyoff", "keyon",
                                       "music", "pcm", "cd_vol", "fade",
                                       "xa", "seq", "sep", "vab", "vol"]):
        domains.append("SOUND")

    # Girl/character domain
    if any(x in name_lower for x in ["girl", "fujisaki", "fuji", "bust_up",
                                       "tokimeki", "yuukou", "zyotai"]):
        domains.append("CHARACTER")

    # Title screen domain
    if "title" in name_lower:
        domains.append("TITLE")

    # Calendar/date domain
    if any(x in name_lower for x in ["cal_", "calendar", "date", "girl_menu_select",
                                       "bustup_face_step0", "biorhythm"]):
        domains.append("CALENDAR")

    # Card/file/save domain
    if any(x in name_lower for x in ["card", "file_select", "savefile",
                                       "memory_card", "data_save", "data_load"]):
        domains.append("SAVE_LOAD")

    # Menu/UI domain
    if any(x in name_lower for x in ["menu", "select", "cursor", "icon",
                                       "disp", "show", "parameter"]):
        domains.append("UI")

    # Graphics domain
    if any(x in name_lower for x in ["vram", "prim", "3d", "rot_", "mdec",
                                       "gs", "tmd", "bg_", "sprite"]):
        domains.append("GRAPHICS")

    # System domain
    if any(x in name_lower for x in ["system", "config", "init", "control"]):
        domains.append("SYSTEM")

    # String/text domain
    if any(x in name_lower for x in ["string", "str", "kanji", "moji",
                                       "load_string", "name"]):
        domains.append("TEXT")

    # Ending domain
    if "ending" in name_lower:
        domains.append("ENDING")

    # Event domain
    if "event" in name_lower:
        domains.append("EVENT")

    # Sick/status domain
    if "sick" in name_lower:
        domains.append("STATUS")

    # Tarao (character NPC) domain
    if "tarao" in name_lower:
        domains.append("TARAO_NPC")

    # Library functions (PsyQ)
    if any(x in name_lower for x in ["gstmd", "rot_2d"]):
        domains.append("PSYQ_GFX")

    if not domains:
        domains.append("UNKNOWN")

    return domains


def check_address_range_match(retail_addr_int, sym_name):
    """Check if function address is in expected range for symbol domain."""
    name_lower = sym_name.lower()
    signals = []

    # Title functions should be near 0x80073000-0x80075000
    if "title" in name_lower:
        if 0x80073000 <= retail_addr_int <= 0x80075000:
            signals.append(("ADDR_MATCH", "title range 0x80073xxx"))
        else:
            signals.append(("ADDR_MISMATCH", f"title expected 0x80073xxx, got 0x{retail_addr_int:08X}"))

    # Sound library functions (Ss*, Spu*, Snd_*) should be in IDO region 0x80088000+
    if any(name_lower.startswith(p) for p in ["ss", "spu", "snd_"]):
        if retail_addr_int >= 0x80088000:
            signals.append(("ADDR_MATCH", "sound library range"))
        else:
            signals.append(("ADDR_MISMATCH", f"sound lib expected 0x80088xxx+, got 0x{retail_addr_int:08X}"))

    # SD_ functions (game sound driver) typically in 0x80088000-0x8008B000
    if name_lower.startswith("sd_"):
        if 0x80088000 <= retail_addr_int <= 0x8008C000:
            signals.append(("ADDR_MATCH", "SD_ driver range"))
        else:
            signals.append(("ADDR_MISMATCH", f"SD_ expected 0x80088xxx-0x8008Bxxx, got 0x{retail_addr_int:08X}"))

    # GsTMD* functions should be in late IDO region
    if name_lower.startswith("gstmd"):
        if retail_addr_int >= 0x800B0000:
            signals.append(("ADDR_MATCH", "GsTMD late IDO range"))
        else:
            signals.append(("ADDR_MISMATCH", f"GsTMD expected 0x800B0xxx+, got 0x{retail_addr_int:08X}"))

    # Holiday functions should cluster together
    if "girl_menu_select" in name_lower:
        if 0x80084000 <= retail_addr_int <= 0x80087000:
            signals.append(("ADDR_MATCH", "girl_menu_select cluster range"))
        else:
            signals.append(("ADDR_MISMATCH", f"girl_menu_select expected 0x80084xxx-0x80087xxx, got 0x{retail_addr_int:08X}"))

    # Calendar functions
    if name_lower.startswith("cal_"):
        if 0x80079000 <= retail_addr_int <= 0x8007C000:
            signals.append(("ADDR_MATCH", "calendar range"))
        else:
            signals.append(("ADDR_MISMATCH", f"cal expected 0x80079xxx-0x8007Bxxx, got 0x{retail_addr_int:08X}"))

    return signals


def check_neighbor_anchors(retail_addr_int, sym_name, all_anchors, all_assignments):
    """Check if neighboring verified anchors have related names."""
    signals = []
    name_lower = sym_name.lower()

    # Collect all known assignments (anchors + other assignments) within +-0x1000
    nearby = []
    for a in all_anchors:
        a_addr = int(a["retail_addr"], 16)
        if abs(a_addr - retail_addr_int) <= 0x2000 and a_addr != retail_addr_int:
            nearby.append((a_addr, a["sym_name"], a.get("confidence", "ANCHOR")))

    if not nearby:
        return signals

    nearby.sort(key=lambda x: abs(x[0] - retail_addr_int))
    neighbor_names = [n[1].lower() for n in nearby[:8]]

    # Check if neighbors share domain keywords
    sym_domains = classify_symbol_domain(sym_name)
    neighbor_domain_matches = 0
    for nn in neighbor_names:
        n_domains = classify_symbol_domain(nn)
        if set(sym_domains) & set(n_domains):
            neighbor_domain_matches += 1

    if neighbor_domain_matches >= 2:
        signals.append(("NEIGHBOR_MATCH", f"{neighbor_domain_matches} neighbors share domain"))
    elif neighbor_domain_matches == 0 and len(nearby) >= 3:
        signals.append(("NEIGHBOR_MISMATCH", "no domain overlap with nearby anchors"))

    # Store neighbor names for reporting
    signals.append(("NEIGHBORS", ", ".join(f"{n[1]}@0x{n[0]:08X}" for n in nearby[:5])))

    return signals


def check_jal_domain(jal_targets, sym_name, all_anchors_map):
    """Check if JAL targets match the symbol's expected domain."""
    signals = []
    name_lower = sym_name.lower()

    if not jal_targets:
        return signals

    # Check sound domain: calls into 0x80088000-0x800A5000
    sound_calls = sum(1 for t in jal_targets if 0x80088000 <= t <= 0x800A5000)
    sound_domain = any(d in classify_symbol_domain(sym_name) for d in ["SOUND"])

    if sound_domain and sound_calls > 0:
        signals.append(("JAL_MATCH", f"{sound_calls} calls to sound range"))
    elif sound_domain and sound_calls == 0 and len(jal_targets) > 0:
        signals.append(("JAL_MISMATCH", "sound symbol but no calls to sound range"))

    # Check if JAL targets hit known anchors with matching domains
    matched_anchor_calls = 0
    for t in jal_targets:
        t_hex = f"0x{t:08X}"
        if t_hex in all_anchors_map:
            anchor_name = all_anchors_map[t_hex]
            anchor_domains = classify_symbol_domain(anchor_name)
            sym_domains = classify_symbol_domain(sym_name)
            if set(anchor_domains) & set(sym_domains):
                matched_anchor_calls += 1

    if matched_anchor_calls > 0:
        signals.append(("JAL_ANCHOR_MATCH", f"{matched_anchor_calls} calls to domain-related anchors"))

    # Check for calls to known specific functions
    for t in jal_targets:
        t_hex = f"0x{t:08X}"
        if t_hex in all_anchors_map:
            aname = all_anchors_map[t_hex].lower()
            # Card/save functions calling card-related functions
            if "card" in name_lower and "card" in aname:
                signals.append(("JAL_MATCH", f"calls card function {all_anchors_map[t_hex]}"))
            # Title functions calling title functions
            if "title" in name_lower and "title" in aname:
                signals.append(("JAL_MATCH", f"calls title function {all_anchors_map[t_hex]}"))

    return signals


def check_global_refs(global_refs, sym_name):
    """Check if global references match expected data regions for domain."""
    signals = []
    name_lower = sym_name.lower()

    if not global_refs:
        return signals

    # Girl/character data typically in 0x800E-0x800F region
    girl_refs = sum(1 for r in global_refs if r in (0x800E, 0x800F))
    if any(d in classify_symbol_domain(sym_name) for d in ["CHARACTER"]) and girl_refs > 0:
        signals.append(("DATA_MATCH", f"{girl_refs} refs to girl data region (0x800E/0x800F)"))

    # Sound data in 0x8012-0x8013 or hardware regs
    spu_refs = sum(1 for r in global_refs if r in (0x8012, 0x8013, 0x1F80))
    if any(d in classify_symbol_domain(sym_name) for d in ["SOUND"]) and spu_refs > 0:
        signals.append(("DATA_MATCH", f"{spu_refs} refs to sound data regions"))

    return signals


def check_file_header_hints(lines, sym_name):
    """Check if file header/comments already mention the symbol or related terms."""
    signals = []
    header_text = " ".join(lines[:15]).lower()

    # Check if the symbol name is already mentioned as "Likely Symbol"
    if "likely symbol:" in header_text:
        m = re.search(r'likely symbol:\s*(\S+)', header_text)
        if m:
            existing = m.group(1)
            if existing.lower() == sym_name.lower():
                signals.append(("HEADER_MATCH", "file header already suggests this symbol"))
            else:
                signals.append(("HEADER_CONFLICT", f"file header suggests '{existing}' instead"))

    # Check for function type hints
    if "wrapper" in header_text:
        signals.append(("FUNC_TYPE", "wrapper function"))
    if "switch dispatcher" in header_text:
        signals.append(("FUNC_TYPE", "switch dispatcher"))
    if "loop" in header_text and "memcpy" not in header_text:
        signals.append(("FUNC_TYPE", "contains loop"))

    return signals


def check_function_size_plausibility(func_info, sym_name):
    """Check if function size is plausible for the symbol name."""
    signals = []
    size = func_info.get("size", 0)
    name_lower = sym_name.lower()

    # Init functions are typically small-medium
    if "init" in name_lower and size > 2000:
        signals.append(("SIZE_NOTE", f"init function but large ({size} bytes)"))

    # Wrapper functions should be tiny
    if size <= 48:
        signals.append(("SIZE_NOTE", f"very small ({size} bytes) - likely wrapper"))

    return signals


def determine_verdict(signals, match):
    """Determine LIKELY_CORRECT, PLAUSIBLE, UNCERTAIN, or LIKELY_WRONG."""
    score = match["total_score"]
    positive = 0
    negative = 0

    for sig_type, sig_detail in signals:
        if sig_type in ("ADDR_MATCH", "JAL_MATCH", "JAL_ANCHOR_MATCH",
                        "DATA_MATCH", "HEADER_MATCH", "NEIGHBOR_MATCH"):
            positive += 1
        elif sig_type in ("ADDR_MISMATCH", "JAL_MISMATCH", "HEADER_CONFLICT",
                          "NEIGHBOR_MISMATCH"):
            negative += 1

    # High address score + positive signals = likely correct
    addr_score = match["scores"].get("address", 0)

    if negative >= 2:
        return "LIKELY_WRONG"
    elif negative >= 1 and positive == 0:
        return "UNCERTAIN"
    elif positive >= 2:
        return "LIKELY_CORRECT"
    elif positive >= 1:
        return "PLAUSIBLE"
    elif addr_score >= 0.99 and score >= 0.80:
        return "PLAUSIBLE"
    elif score >= 0.80:
        return "PLAUSIBLE"
    else:
        return "UNCERTAIN"


def main():
    results, addr_map, raw_anchors = load_data()

    anchors_list = results.get("anchors", [])
    assignments = results.get("assignments", [])

    # Build anchor name lookup by address
    anchor_name_map = {}
    for a in anchors_list:
        anchor_name_map[a["retail_addr"]] = a["sym_name"]
    # Also add HIGH/MEDIUM assignments for neighborhood checking
    for a in assignments:
        if a["confidence"] in ("HIGH",):
            anchor_name_map[a["retail_addr"]] = a["sym_name"]

    medium = [a for a in assignments if a["confidence"] == "MEDIUM"]
    medium.sort(key=lambda x: x["retail_addr"])

    print(f"Processing {len(medium)} MEDIUM confidence matches...")

    triage_results = []

    for match in medium:
        retail_addr = match["retail_addr"]
        retail_addr_int = int(retail_addr, 16)
        sym_name = match["sym_name"]
        region = match["region"]
        score = match["total_score"]

        # Find source file
        func_info = addr_map.get(retail_addr)
        if not func_info:
            triage_results.append({
                "match": match,
                "verdict": "UNCERTAIN",
                "signals": [("ERROR", "no source file found")],
                "source": None,
                "domains": classify_symbol_domain(sym_name),
            })
            continue

        source_path = func_info["source"]
        content, lines = read_source_file(source_path)

        if content is None:
            triage_results.append({
                "match": match,
                "verdict": "UNCERTAIN",
                "signals": [("ERROR", f"cannot read {source_path}")],
                "source": source_path,
                "domains": classify_symbol_domain(sym_name),
            })
            continue

        # Collect signals
        signals = []

        # 1. Check address range
        signals.extend(check_address_range_match(retail_addr_int, sym_name))

        # 2. Check JAL targets
        if source_path.endswith(".s"):
            jal_targets = extract_jal_targets(lines)
        else:
            jal_targets = extract_jal_targets_c(lines)
        signals.extend(check_jal_domain(jal_targets, sym_name, anchor_name_map))

        # 3. Check global refs
        global_refs = extract_global_refs(lines)
        signals.extend(check_global_refs(global_refs, sym_name))

        # 4. Check neighbor anchors
        signals.extend(check_neighbor_anchors(retail_addr_int, sym_name,
                                               anchors_list, assignments))

        # 5. Check file header
        signals.extend(check_file_header_hints(lines, sym_name))

        # 6. Function size
        signals.extend(check_function_size_plausibility(func_info, sym_name))

        # Determine verdict
        verdict = determine_verdict(signals, match)

        triage_results.append({
            "match": match,
            "verdict": verdict,
            "signals": signals,
            "source": source_path,
            "domains": classify_symbol_domain(sym_name),
            "jal_count": len(jal_targets),
            "func_size": func_info.get("size", 0),
        })

    # Generate report
    generate_report(triage_results, anchors_list)


def generate_report(triage_results, anchors_list):
    """Generate markdown triage report."""
    out_path = os.path.join(BASE_DIR, "build/symbol_match/medium_triage.md")

    # Count verdicts
    verdict_counts = defaultdict(int)
    for r in triage_results:
        verdict_counts[r["verdict"]] += 1

    lines = []
    lines.append("# Medium Confidence Symbol Match Triage Report")
    lines.append("")
    lines.append(f"Generated by `tools/analysis/triage_medium_matches.py`")
    lines.append("")
    lines.append("## Summary")
    lines.append("")
    lines.append(f"| Verdict | Count |")
    lines.append(f"|---------|-------|")
    for v in ["LIKELY_CORRECT", "PLAUSIBLE", "UNCERTAIN", "LIKELY_WRONG"]:
        lines.append(f"| {v} | {verdict_counts.get(v, 0)} |")
    lines.append(f"| **Total** | **{len(triage_results)}** |")
    lines.append("")

    lines.append("## Methodology")
    lines.append("")
    lines.append("Each MEDIUM match is cross-referenced against its source file using:")
    lines.append("1. **Address range**: Does the function address fall in the expected range for its domain?")
    lines.append("2. **JAL targets**: Do function calls go to addresses consistent with the symbol's domain?")
    lines.append("3. **Global refs**: Do memory references match expected data regions?")
    lines.append("4. **Neighbor anchors**: Do verified neighbors share the same domain?")
    lines.append("5. **File header**: Does the file header already reference this symbol?")
    lines.append("6. **Function size**: Is the size plausible for this type of function?")
    lines.append("")

    # Group by verdict
    for verdict_label in ["LIKELY_CORRECT", "PLAUSIBLE", "UNCERTAIN", "LIKELY_WRONG"]:
        group = [r for r in triage_results if r["verdict"] == verdict_label]
        if not group:
            continue

        lines.append(f"## {verdict_label} ({len(group)})")
        lines.append("")

        for r in group:
            m = r["match"]
            lines.append(f"### `{m['sym_name']}` -> `{m['retail_name']}` ({m['retail_addr']})")
            lines.append("")
            lines.append(f"- **Region**: {m['region']}")
            lines.append(f"- **Score**: {m['total_score']:.3f} (addr={m['scores']['address']:.3f}, "
                         f"neighbor={m['scores']['neighborhood']:.3f}, "
                         f"call_graph={m['scores']['call_graph']:.3f})")
            if r.get("source"):
                lines.append(f"- **Source**: `{r['source']}`")
            lines.append(f"- **Domain**: {', '.join(r['domains'])}")
            if r.get("func_size"):
                lines.append(f"- **Size**: {r['func_size']} bytes")

            # Signals
            sig_lines = []
            neighbor_line = None
            for sig_type, sig_detail in r["signals"]:
                if sig_type == "NEIGHBORS":
                    neighbor_line = sig_detail
                elif sig_type.endswith("_MATCH"):
                    sig_lines.append(f"  - :white_check_mark: **{sig_type}**: {sig_detail}")
                elif sig_type.endswith("_MISMATCH") or sig_type.endswith("_CONFLICT"):
                    sig_lines.append(f"  - :x: **{sig_type}**: {sig_detail}")
                else:
                    sig_lines.append(f"  - **{sig_type}**: {sig_detail}")

            if sig_lines:
                lines.append("- **Signals**:")
                lines.extend(sig_lines)
            if neighbor_line:
                lines.append(f"- **Nearby anchors**: {neighbor_line}")

            lines.append("")

    # Write per-region summary tables
    lines.append("## Per-Region Summary")
    lines.append("")

    for region in ["GCC", "IDO"]:
        region_results = [r for r in triage_results if r["match"]["region"] == region]
        if not region_results:
            continue

        lines.append(f"### {region} Region")
        lines.append("")
        lines.append("| Address | Symbol | Verdict | Score | Domains |")
        lines.append("|---------|--------|---------|-------|---------|")
        for r in region_results:
            m = r["match"]
            lines.append(f"| {m['retail_addr']} | `{m['sym_name']}` | {r['verdict']} | "
                         f"{m['total_score']:.3f} | {', '.join(r['domains'])} |")
        lines.append("")

    # Actionable summary
    lines.append("## Recommended Actions")
    lines.append("")

    likely_correct = [r for r in triage_results if r["verdict"] == "LIKELY_CORRECT"]
    plausible = [r for r in triage_results if r["verdict"] == "PLAUSIBLE"]
    uncertain = [r for r in triage_results if r["verdict"] == "UNCERTAIN"]
    likely_wrong = [r for r in triage_results if r["verdict"] == "LIKELY_WRONG"]

    lines.append(f"1. **Promote to HIGH** ({len(likely_correct)} functions): "
                 "These have strong cross-referencing evidence. Consider applying as file renames.")
    lines.append(f"2. **Keep as MEDIUM** ({len(plausible)} functions): "
                 "Plausible matches but need more evidence before renaming.")
    lines.append(f"3. **Review manually** ({len(uncertain)} functions): "
                 "Insufficient automated evidence to judge. Manual inspection recommended.")
    lines.append(f"4. **Demote or reject** ({len(likely_wrong)} functions): "
                 "Cross-referencing suggests these may be incorrect assignments.")
    lines.append("")

    if likely_correct:
        lines.append("### Functions ready to promote:")
        lines.append("```")
        for r in likely_correct:
            m = r["match"]
            lines.append(f"{m['retail_addr']}  {m['sym_name']}")
        lines.append("```")
        lines.append("")

    if likely_wrong:
        lines.append("### Functions to investigate/demote:")
        lines.append("```")
        for r in likely_wrong:
            m = r["match"]
            reasons = [s[1] for s in r["signals"]
                       if s[0].endswith("_MISMATCH") or s[0].endswith("_CONFLICT")]
            lines.append(f"{m['retail_addr']}  {m['sym_name']}  -- {'; '.join(reasons)}")
        lines.append("```")
        lines.append("")

    report = "\n".join(lines)
    with open(out_path, "w") as f:
        f.write(report)

    print(f"\nTriage report written to: {out_path}")
    print(f"\nVerdict Summary:")
    verdict_counts = defaultdict(int)
    for r in triage_results:
        verdict_counts[r["verdict"]] += 1
    for v in ["LIKELY_CORRECT", "PLAUSIBLE", "UNCERTAIN", "LIKELY_WRONG"]:
        print(f"  {v}: {verdict_counts.get(v, 0)}")
    print(f"  Total: {len(triage_results)}")


if __name__ == "__main__":
    main()
