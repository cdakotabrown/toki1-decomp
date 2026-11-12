#!/usr/bin/env python3
"""Advanced symbol matching using drift zones, call graph, and data xrefs.

Implements the playbook: piecewise drift modeling -> candidate generation ->
multi-signal scoring -> bipartite assignment -> confidence-tiered output.

Usage:
    python3 tools/analysis/symbol_match_advanced.py [--output-dir build/symbol_match]
"""

import json
import math
import os
import re
import sys
from collections import defaultdict
from pathlib import Path

PROJECT_ROOT = Path('.')

# ---------------------------------------------------------------------------
# Phase 0: Load all data
# ---------------------------------------------------------------------------

def load_retail_functions():
    """Load all retail functions from level3_functions.json."""
    path = PROJECT_ROOT / 'config/level3_functions.json'
    with open(path) as f:
        data = json.load(f)
    funcs = {}
    for entry in data['functions']:
        addr = int(entry['address'], 16)
        funcs[addr] = {
            'name': entry['name'],
            'address': addr,
            'size': entry.get('size', 0),
            'source': entry['source'],
            'region': entry['region'],
            'type': 'c' if entry['source'].endswith('.c') else 'asm',
        }
    return funcs


def load_debug_symbols():
    """Load debug symbols from o.bin symbol_map.json."""
    path = PROJECT_ROOT / 'build/slps00065/symbols/symbol_map.json'
    with open(path) as f:
        data = json.load(f)
    syms = {}
    for entry in data['symbols']:
        addr = int(entry['addr'], 16)
        syms[addr] = entry['name']
    return syms


def load_proximity_report():
    """Load proximity match report for anchor extraction."""
    path = PROJECT_ROOT / 'build/symbol_audit/proximity_match_report.json'
    with open(path) as f:
        return json.load(f)


def load_function_map():
    """Load function_map.json for call graph and data xrefs."""
    path = PROJECT_ROOT / 'build/function_analysis/function_map.json'
    with open(path) as f:
        data = json.load(f)
    return data.get('functions', [])


def load_already_renamed():
    """Load files that have already been renamed to symbol names.
    These are verified correct assignments and serve as strong anchors."""
    renamed = {}
    for pattern, base_dir in [
        ('src/main/matched/*.c', PROJECT_ROOT / 'src/main/matched'),
        ('asm/main/matched/*.s', PROJECT_ROOT / 'asm/main/matched'),
    ]:
        for filepath in sorted(Path(base_dir).glob('*')):
            name = filepath.stem
            if not re.match(r'(?:func|FUN)_[0-9A-Fa-f]{8}$', name, re.IGNORECASE):
                # This is a symbol-named file - read its address from header
                try:
                    with open(filepath) as f:
                        header = f.read(2000)
                    match = re.search(r'Address:\s*(0x[0-9A-Fa-f]{8})', header)
                    if match:
                        addr = int(match.group(1), 16)
                        renamed[addr] = name
                except Exception:
                    pass
    return renamed


# ---------------------------------------------------------------------------
# Phase 1: Drift zone modeling
# ---------------------------------------------------------------------------

def extract_anchors(proximity_report, already_renamed, debug_symbols):
    """Extract deduplicated anchors from proximity report + renamed files.

    Priority: already-renamed files (verified) > EXACT matches > HIGH matches.
    Dedup: each retail addr gets one symbol, each symbol gets one retail addr.
    """
    # Start with already-renamed files as strongest anchors
    anchors = {}
    used_symbols = set()

    for addr, sym_name in already_renamed.items():
        # Find the debug symbol address for this name
        for sym_addr, sname in debug_symbols.items():
            if sname == sym_name:
                anchors[addr] = {
                    'retail_addr': addr,
                    'sym_addr': sym_addr,
                    'sym_name': sym_name,
                    'drift': sym_addr - addr,
                    'confidence': 'VERIFIED',
                }
                used_symbols.add(sym_name)
                break

    # Add EXACT matches
    for m in proximity_report.get('exact_matches', []):
        fa = int(m['func_addr'], 16)
        sa = int(m['sym_addr'], 16)
        sn = m['sym_name']
        if fa not in anchors and sn not in used_symbols:
            anchors[fa] = {
                'retail_addr': fa,
                'sym_addr': sa,
                'sym_name': sn,
                'drift': sa - fa,
                'confidence': 'EXACT',
            }
            used_symbols.add(sn)

    # Add HIGH (close) matches - pick closest per symbol
    close_by_sym = defaultdict(list)
    for m in proximity_report.get('close_matches', []):
        sn = m['sym_name']
        if sn not in used_symbols:
            close_by_sym[sn].append(m)

    for sn, matches in close_by_sym.items():
        # Pick the match with smallest distance
        best = min(matches, key=lambda m: m['distance'])
        fa = int(best['func_addr'], 16)
        sa = int(best['sym_addr'], 16)
        if fa not in anchors:
            anchors[fa] = {
                'retail_addr': fa,
                'sym_addr': sa,
                'sym_name': sn,
                'drift': sa - fa,
                'confidence': 'HIGH',
            }
            used_symbols.add(sn)

    return dict(sorted(anchors.items()))


def build_drift_model(anchors, window_size=12):
    """Build piecewise drift model from anchors.

    Returns a function that predicts debug address from retail address,
    plus an uncertainty estimate.
    """
    anchor_list = sorted(anchors.values(), key=lambda a: a['retail_addr'])

    if not anchor_list:
        return None

    # Build segments: sliding window of anchors
    segments = []
    n = len(anchor_list)

    for i in range(n):
        # Window: anchors[max(0, i-w//2) : min(n, i+w//2)]
        half = window_size // 2
        lo = max(0, i - half)
        hi = min(n, i + half + 1)
        window = anchor_list[lo:hi]

        drifts = [a['drift'] for a in window]
        mean_drift = sum(drifts) / len(drifts)
        max_residual = max(abs(d - mean_drift) for d in drifts)

        segments.append({
            'center_addr': anchor_list[i]['retail_addr'],
            'mean_drift': mean_drift,
            'uncertainty': max(max_residual, 16),  # min 16 bytes
            'n_anchors': len(window),
        })

    class DriftModel:
        def __init__(self, segments, anchor_list):
            self.segments = segments
            self.anchor_list = anchor_list
            self.min_addr = anchor_list[0]['retail_addr']
            self.max_addr = anchor_list[-1]['retail_addr']

        def predict(self, retail_addr):
            """Return (predicted_debug_addr, uncertainty)."""
            # Before first anchor
            if retail_addr <= self.segments[0]['center_addr']:
                s = self.segments[0]
                # Increase uncertainty the further we are from first anchor
                dist = self.segments[0]['center_addr'] - retail_addr
                extra_unc = dist // 256  # +1 byte uncertainty per 256 bytes
                return (
                    retail_addr + s['mean_drift'],
                    s['uncertainty'] + extra_unc
                )

            # After last anchor
            if retail_addr >= self.segments[-1]['center_addr']:
                s = self.segments[-1]
                dist = retail_addr - self.segments[-1]['center_addr']
                extra_unc = dist // 256
                return (
                    retail_addr + s['mean_drift'],
                    s['uncertainty'] + extra_unc
                )

            # Between anchors: interpolate between nearest segments
            for i in range(len(self.segments) - 1):
                if (self.segments[i]['center_addr'] <= retail_addr <=
                        self.segments[i + 1]['center_addr']):
                    s0 = self.segments[i]
                    s1 = self.segments[i + 1]
                    span = s1['center_addr'] - s0['center_addr']
                    if span == 0:
                        t = 0.5
                    else:
                        t = (retail_addr - s0['center_addr']) / span
                    drift = s0['mean_drift'] * (1 - t) + s1['mean_drift'] * t
                    unc = max(s0['uncertainty'], s1['uncertainty'])
                    return (retail_addr + drift, unc)

            # Fallback
            s = self.segments[-1]
            return (retail_addr + s['mean_drift'], s['uncertainty'] + 32)

    return DriftModel(segments, anchor_list)


# ---------------------------------------------------------------------------
# Phase 2: Call graph and data xref extraction
# ---------------------------------------------------------------------------

def extract_call_graph(function_map_entries):
    """Extract caller->callee edges from function_map disassembly.

    JAL format: 'jal\\t0xNNNNN' where real addr = 0x80000000 | target.
    """
    callees = defaultdict(set)  # retail_addr -> set of callee addrs
    callers = defaultdict(set)  # retail_addr -> set of caller addrs

    for func in function_map_entries:
        caller_addr = int(func['vram_start'], 16)
        for line in func.get('disassembly', []):
            match = re.search(r'jal\t0x([0-9a-fA-F]+)', line)
            if match:
                raw_target = int(match.group(1), 16)
                callee_addr = 0x80000000 | raw_target
                callees[caller_addr].add(callee_addr)
                callers[callee_addr].add(caller_addr)

    return dict(callees), dict(callers)


def extract_call_graph_from_sources(retail_funcs):
    """Extract caller->callee edges by parsing JAL targets from ASM/C source files.

    This supplements extract_call_graph() which only covers functions in
    function_map.json (~927 functions). By parsing source files directly,
    we get call graph coverage for all ~1189 functions.

    ASM patterns:
        jal     func_800XXXXX       -> callee = address from name
        jal     0x800XXXXX          -> callee = literal address
        jal\t0x800XXXXX            -> callee = literal address (tab-separated)

    C patterns:
        func_800XXXXX(              -> callee = address from name
    """
    callees = defaultdict(set)
    callers = defaultdict(set)

    # Regex patterns for ASM files
    # Match: jal  func_800XXXXX  or  jal  0x800XXXXX  (with spaces or tabs)
    asm_jal_sym = re.compile(r'jal\s+func_([0-9a-fA-F]{8})', re.IGNORECASE)
    asm_jal_addr = re.compile(r'jal\s+0x(8[0-9a-fA-F]{7})', re.IGNORECASE)

    # Regex for C files: function calls like func_800XXXXX(
    c_call = re.compile(r'\bfunc_(8[0-9a-fA-F]{7})\s*\(', re.IGNORECASE)

    # Also match named function calls in C files for domain analysis
    # e.g., SsVabOpen(, CdInit(, etc.
    c_named_call = re.compile(r'\b([A-Z][a-zA-Z0-9_]+)\s*\(')

    for addr, func in retail_funcs.items():
        source = func['source']
        if source.endswith('.s'):
            filepath = PROJECT_ROOT / source
        elif source.endswith('.c'):
            filepath = PROJECT_ROOT / source
        else:
            continue

        if not filepath.exists():
            continue

        try:
            with open(filepath) as f:
                text = f.read()
        except Exception:
            continue

        caller_addr = addr
        found_callees = set()

        if source.endswith('.s'):
            # Parse ASM JAL instructions
            for m in asm_jal_sym.finditer(text):
                callee = int(m.group(1), 16)
                found_callees.add(callee)
            for m in asm_jal_addr.finditer(text):
                callee = int(m.group(1), 16)
                found_callees.add(callee)
        else:
            # Parse C function calls
            for m in c_call.finditer(text):
                callee = int(m.group(1), 16)
                found_callees.add(callee)

        # Remove self-references
        found_callees.discard(caller_addr)

        if found_callees:
            callees[caller_addr] = found_callees
            for callee in found_callees:
                callers[callee].add(caller_addr)

    return dict(callees), dict(callers)


def merge_call_graphs(cg1_callees, cg1_callers, cg2_callees, cg2_callers):
    """Merge two call graphs, taking the union of edges."""
    merged_callees = defaultdict(set)
    merged_callers = defaultdict(set)

    for addr, targets in cg1_callees.items():
        merged_callees[addr].update(targets)
    for addr, targets in cg2_callees.items():
        merged_callees[addr].update(targets)
    for addr, sources in cg1_callers.items():
        merged_callers[addr].update(sources)
    for addr, sources in cg2_callers.items():
        merged_callers[addr].update(sources)

    return dict(merged_callees), dict(merged_callers)


def extract_data_xrefs(function_map_entries):
    """Extract data references from function_map."""
    xrefs = {}
    for func in function_map_entries:
        addr = int(func['vram_start'], 16)
        refs = set()
        for r in func.get('data_refs', []):
            refs.add(r)
        for r in func.get('undeclared_data_refs', []):
            refs.add(r)
        if refs:
            xrefs[addr] = refs
    return xrefs


def extract_function_metrics(function_map_entries):
    """Extract per-function metrics for micro-structure comparison."""
    metrics = {}
    for func in function_map_entries:
        addr = int(func['vram_start'], 16)
        metrics[addr] = {
            'size': func.get('size', 0),
            'num_jal': func.get('num_jal_calls', 0),
            'num_branches': func.get('num_branches', 0),
            'num_loops': func.get('num_loops', 0),
            'complexity': func.get('complexity', 'unknown'),
            'category': func.get('category', 'unknown'),
        }
    return metrics


# ---------------------------------------------------------------------------
# Phase 2b: Build symbol name semantic clusters
# ---------------------------------------------------------------------------

# Known name prefixes -> functional domain
DOMAIN_PATTERNS = {
    'sound': [
        r'^Sd_', r'^SD_', r'^Ss', r'^SS', r'^Se[qQ]', r'^Note',
        r'^Cont[A-Z]', r'^Set(Program|Control)', r'^spu_', r'^Spu',
        r'^_Ss', r'^_spu', r'^vab_', r'^midi_', r'^Snd_',
        r'^addr_init.*_sd$',
    ],
    'cd': [
        r'^Cd[A-Z]', r'^_Cd', r'^cd_', r'^CD_',
        r'^CdInit', r'^CdRead', r'^CdControl', r'^Control_CD',
        r'^xa_',
    ],
    'gpu': [
        r'^Gs[A-Z]', r'^_Gs', r'^GPU_', r'^gpu_', r'^Draw',
        r'^GsTMD', r'^GsSort', r'^GsMap',
        r'^tpage_', r'^search_tpage', r'^goto_tpage',
        r'^load_tpage', r'^mod_set_gpu', r'^mod_trans_vram',
        r'^load_csr', r'^show_packet',
    ],
    'pad': [
        r'^Pad', r'^pad_', r'^MyPad', r'^_pad',
    ],
    'calendar': [
        r'^cal_', r'^calendar', r'^date_', r'^month_',
        r'^week_', r'^holiday_', r'^vacation_',
    ],
    'girl': [
        r'^girl_', r'^tokimeki', r'^yuukou', r'^h_',
        r'^bust_up_', r'^bustup_', r'^fujisaki',
        r'^touzyou_', r'^event_face', r'^event_char',
    ],
    'menu': [
        r'^menu_', r'^sel_', r'^select_', r'^icon_',
        r'^cursor_', r'^x_taku',
    ],
    'screen': [
        r'^srn_', r'^screen_', r'^disp_', r'^k_disp',
        r'^k_sub_', r'^bg_', r'^back_scroll',
        r'^show_string', r'^system_moji',
    ],
    'sprite': [
        r'^sprite_', r'^spr_', r'^_sprite',
    ],
    'card': [
        r'^card_', r'^Card', r'^mem_card', r'^save_', r'^load_',
        r'^data_save', r'^data_load', r'^make_file', r'^delete_file',
    ],
    'math': [
        r'^SquareRoot', r'^sin_', r'^cos_', r'^ratan',
        r'^rcos', r'^rsin', r'^csin', r'^ccos',
    ],
    'system': [
        r'^sys_', r'^system_', r'^init_', r'^_init',
        r'^warm_reset', r'^game_', r'^game_mode',
        r'^Hw_', r'^Sw_',
    ],
    'file_select': [
        r'^file_select', r'^file_sel',
    ],
    'title': [
        r'^title_',
    ],
    'normal_event': [
        r'^normal_', r'^pre_',
    ],
    'parameter': [
        r'^parameter_', r'^class_', r'^set_.*_mode',
    ],
    'magazine': [
        r'^magazine_',
    ],
    'mascot': [
        r'^mascot_',
    ],
    'basyo': [
        r'^basyo_',
    ],
}

# Data ref patterns -> domain
DATA_REF_DOMAINS = {
    'sound': ['spu_', 'vab_', 'SD_', 'seq_', 'midi_', 'sound_'],
    'calendar': ['cal_dat', 'sum_month', 'cal_color', 'calendar'],
    'girl': ['tokimeki', 'yuukou', 'girl_', 'h_para'],
    'menu': ['menu_', 'sel_', 'select_'],
    'card': ['card_', 'file_name', 'save_', 'load_'],
    'gpu': ['GPU_', 'prim_', 'ot_', 'draw_'],
    'screen': ['icon_image', 'telephone', 'date_name'],
}


def classify_symbol_domain(sym_name):
    """Classify a symbol name into a functional domain."""
    for domain, patterns in DOMAIN_PATTERNS.items():
        for pat in patterns:
            if re.match(pat, sym_name, re.IGNORECASE):
                return domain
    return None


def classify_data_domain(data_refs):
    """Classify a function's data references into a domain."""
    domains = set()
    for ref in data_refs:
        ref_lower = ref.lower()
        for domain, patterns in DATA_REF_DOMAINS.items():
            for pat in patterns:
                if pat.lower() in ref_lower:
                    domains.add(domain)
    return domains


# ---------------------------------------------------------------------------
# Phase 3: Scoring
# ---------------------------------------------------------------------------

def score_address(retail_addr, sym_addr, predicted_addr, window):
    """Score based on address consistency under local drift."""
    delta = abs(sym_addr - predicted_addr)
    if delta == 0:
        return 1.0
    return max(0.0, 1.0 - delta / window)


def score_neighborhood(retail_addr, sym_addr, anchors_sorted, debug_symbols):
    """Score based on whether symbol is consistent with neighbors."""
    # Find previous and next anchors
    prev_anchor = None
    next_anchor = None
    for a in anchors_sorted:
        if a['retail_addr'] < retail_addr:
            prev_anchor = a
        elif a['retail_addr'] > retail_addr:
            next_anchor = a
            break

    if prev_anchor is None and next_anchor is None:
        return 0.5  # neutral

    score = 0.5
    checks = 0

    if prev_anchor is not None:
        # Symbol should be after prev anchor's symbol
        if sym_addr > prev_anchor['sym_addr']:
            score += 0.25
        else:
            score -= 0.25
        checks += 1

    if next_anchor is not None:
        # Symbol should be before next anchor's symbol
        if sym_addr < next_anchor['sym_addr']:
            score += 0.25
        else:
            score -= 0.25
        checks += 1

    return max(0.0, min(1.0, score))


def score_callgraph(retail_addr, sym_name, callees_map, callers_map,
                    assigned_names, sdk_calls):
    """Score based on call graph similarity."""
    score = 0.0
    signals = 0

    sym_domain = classify_symbol_domain(sym_name)
    if sym_domain is None:
        return 0.5  # neutral

    # Check if callees are in the same domain
    func_callees = callees_map.get(retail_addr, set())

    if func_callees:
        domain_callees = 0
        named_callees = 0
        for callee in func_callees:
            if callee in assigned_names:
                callee_domain = classify_symbol_domain(assigned_names[callee])
                if callee_domain == sym_domain:
                    domain_callees += 1
                named_callees += 1
        if named_callees > 0:
            score = domain_callees / named_callees
            signals += 1

    # Check SDK call patterns
    if sym_domain == 'sound':
        sound_calls = sum(1 for c in func_callees
                          if 0x80095000 <= c <= 0x800A5000)
        if sound_calls > 0:
            score = max(score, 0.7)
            signals += 1
    elif sym_domain == 'gpu':
        gpu_calls = sum(1 for c in func_callees
                        if 0x800A5000 <= c <= 0x800C0000)
        if gpu_calls > 0:
            score = max(score, 0.7)
            signals += 1

    if signals == 0:
        return 0.5
    return score


def score_data_xrefs(retail_addr, sym_name, data_xrefs_map):
    """Score based on data reference domain matching."""
    sym_domain = classify_symbol_domain(sym_name)
    func_refs = data_xrefs_map.get(retail_addr, set())

    if not func_refs or sym_domain is None:
        return 0.5  # neutral

    func_domains = classify_data_domain(func_refs)
    if sym_domain in func_domains:
        return 1.0
    if func_domains and sym_domain not in func_domains:
        return 0.2  # weak negative signal
    return 0.5


def score_callee_propagation(retail_addr, sym_name, callees_map,
                            assigned_names):
    """Score based on callee name propagation: if a retail function calls 2+
    already-named functions in the same domain as the candidate symbol,
    boost candidates from that domain.

    This captures the pattern where e.g. a sound function calls multiple
    Ss*/Spu* functions, strongly suggesting it is itself a sound function.
    """
    sym_domain = classify_symbol_domain(sym_name)
    if sym_domain is None:
        return 0.5  # neutral - can't classify candidate

    func_callees = callees_map.get(retail_addr, set())
    if not func_callees:
        return 0.5  # neutral - no call data

    # Count how many callees are named and in the same domain
    domain_matches = 0
    named_callees = 0
    for callee_addr in func_callees:
        callee_name = assigned_names.get(callee_addr)
        if callee_name is None:
            continue
        named_callees += 1
        callee_domain = classify_symbol_domain(callee_name)
        if callee_domain == sym_domain:
            domain_matches += 1

    if named_callees == 0:
        return 0.5  # neutral

    # Need 2+ domain matches for a meaningful signal
    if domain_matches >= 3:
        return 1.0  # strong positive
    elif domain_matches >= 2:
        return 0.85  # good positive
    elif domain_matches >= 1 and named_callees <= 2:
        return 0.65  # mild positive (single match but few callees)
    elif domain_matches == 0 and named_callees >= 2:
        return 0.25  # negative signal: calls named functions but none in domain
    return 0.5


def compute_total_score(addr_score, neighbor_score, data_score, cg_score,
                        callee_prop_score=0.5):
    """Weighted combination of score components.

    The base score uses the original weights for address, neighborhood,
    data xref, and call graph signals. Callee name propagation acts as
    an additive bonus/penalty (up to +/-0.06) rather than redistributing
    weight away from the primary address signal.
    """
    base = (0.45 * addr_score +
            0.20 * neighbor_score +
            0.20 * data_score +
            0.15 * cg_score)
    # Callee propagation: bonus/penalty centered at 0.5 (neutral)
    # Score range [0, 1] -> adjustment range [-0.06, +0.06]
    cp_adjustment = (callee_prop_score - 0.5) * 0.12
    return max(0.0, min(1.0, base + cp_adjustment))


# ---------------------------------------------------------------------------
# Phase 4: Candidate generation and assignment
# ---------------------------------------------------------------------------

PRE_SYMBOL_START = 0x80041000
PRE_SYMBOL_END = 0x8004BFFF


def generate_candidates(retail_funcs, debug_symbols, drift_model,
                        anchors, initial_window=1024, max_window=2048,
                        sparse_max_window=4096):
    """Generate candidate symbol matches for each unassigned retail function.

    For functions far from the nearest anchor (> 0x2000 bytes), the search
    window is expanded up to sparse_max_window to improve coverage in
    sparsely-anchored regions.
    """
    # Already-assigned addresses (from anchors)
    assigned_addrs = set(anchors.keys())
    assigned_syms = {a['sym_name'] for a in anchors.values()}

    # Build sorted anchor address list for distance computation
    anchor_addrs = sorted(anchors.keys())

    # Build sorted symbol list for binary search
    sym_list = sorted(debug_symbols.items())  # (addr, name)

    candidates = {}

    for retail_addr, func in sorted(retail_funcs.items()):
        if retail_addr in assigned_addrs:
            continue

        # Skip pre-symbol range
        if PRE_SYMBOL_START <= retail_addr <= PRE_SYMBOL_END:
            continue

        pred_addr, uncertainty = drift_model.predict(retail_addr)
        window = max(initial_window, int(uncertainty * 6))

        # Enhancement 2: Widen window for functions far from nearest anchor
        dist_to_anchor = _distance_to_nearest_anchor(retail_addr, anchor_addrs)
        if dist_to_anchor > 0x2000:
            # Scale window: base max_window + extra proportional to distance
            # At 0x2000 distance: max_window (768)
            # At 0x8000+ distance: sparse_max_window (1536)
            scale = min(1.0, (dist_to_anchor - 0x2000) / 0x6000)
            effective_max = max_window + int(scale * (sparse_max_window - max_window))
            window = min(window, effective_max)
        else:
            window = min(window, max_window)

        # Find candidate symbols within window
        lo = pred_addr - window
        hi = pred_addr + window

        func_candidates = []
        for sym_addr, sym_name in sym_list:
            if sym_addr < lo:
                continue
            if sym_addr > hi:
                break
            if sym_name in assigned_syms:
                continue
            func_candidates.append({
                'sym_addr': sym_addr,
                'sym_name': sym_name,
                'pred_delta': abs(sym_addr - pred_addr),
            })

        if func_candidates:
            # Sort by distance from predicted address
            func_candidates.sort(key=lambda c: c['pred_delta'])
            candidates[retail_addr] = func_candidates[:15]  # top 15 (was 10)

    return candidates


def _distance_to_nearest_anchor(addr, sorted_anchor_addrs):
    """Return the distance from addr to the nearest anchor address.
    Uses binary search for efficiency."""
    import bisect
    if not sorted_anchor_addrs:
        return 0xFFFFFFFF
    idx = bisect.bisect_left(sorted_anchor_addrs, addr)
    dist = 0xFFFFFFFF
    if idx < len(sorted_anchor_addrs):
        dist = min(dist, abs(sorted_anchor_addrs[idx] - addr))
    if idx > 0:
        dist = min(dist, abs(sorted_anchor_addrs[idx - 1] - addr))
    return dist


def run_assignment(retail_funcs, candidates, drift_model, anchors,
                   callees_map, callers_map, data_xrefs_map,
                   debug_symbols):
    """Run two-pass greedy bipartite assignment with scoring.

    Pass 1: Score and assign high-confidence matches using anchor names only.
    Pass 2: Re-score remaining candidates using pass-1 assignments for
            callee name propagation, then assign remaining matches.
    """
    anchors_sorted = sorted(anchors.values(), key=lambda a: a['retail_addr'])

    # Build map of already-assigned names (from anchors)
    assigned_names = {}
    for addr, a in anchors.items():
        assigned_names[addr] = a['sym_name']

    # Score all candidates
    scored = []
    for retail_addr, cands in candidates.items():
        pred_addr, uncertainty = drift_model.predict(retail_addr)
        window = max(1024, int(uncertainty * 6))
        window = min(window, 2048)

        for cand in cands:
            a_score = score_address(
                retail_addr, cand['sym_addr'], pred_addr, window)
            n_score = score_neighborhood(
                retail_addr, cand['sym_addr'], anchors_sorted, debug_symbols)
            d_score = score_data_xrefs(
                retail_addr, cand['sym_name'], data_xrefs_map)
            c_score = score_callgraph(
                retail_addr, cand['sym_name'], callees_map, callers_map,
                assigned_names, set())
            cp_score = score_callee_propagation(
                retail_addr, cand['sym_name'], callees_map, assigned_names)

            total = compute_total_score(
                a_score, n_score, d_score, c_score, cp_score)

            scored.append({
                'retail_addr': retail_addr,
                'sym_addr': cand['sym_addr'],
                'sym_name': cand['sym_name'],
                'total_score': total,
                'addr_score': a_score,
                'neighbor_score': n_score,
                'data_score': d_score,
                'callgraph_score': c_score,
                'callee_prop_score': cp_score,
            })

    # Sort by total score descending
    scored.sort(key=lambda s: s['total_score'], reverse=True)

    # Greedy assignment
    assigned_retail = set(anchors.keys())
    assigned_sym = {a['sym_name'] for a in anchors.values()}
    results = []

    for s in scored:
        if s['retail_addr'] in assigned_retail:
            continue
        if s['sym_name'] in assigned_sym:
            continue

        # Determine confidence
        # Find second-best for this retail addr
        same_retail = [x for x in scored
                       if x['retail_addr'] == s['retail_addr']
                       and x['sym_name'] != s['sym_name']
                       and x['sym_name'] not in assigned_sym]
        second_best = same_retail[0]['total_score'] if same_retail else 0.0
        gap = s['total_score'] - second_best

        if s['total_score'] >= 0.78 and gap >= 0.10:
            confidence = 'HIGH'
        elif s['total_score'] >= 0.70 and gap >= 0.05:
            confidence = 'MEDIUM'
        elif s['total_score'] >= 0.55:
            confidence = 'LOW'
        else:
            confidence = 'VERY_LOW'

        results.append({
            'retail_addr': s['retail_addr'],
            'retail_name': retail_funcs[s['retail_addr']]['name'],
            'sym_addr': s['sym_addr'],
            'sym_name': s['sym_name'],
            'total_score': round(s['total_score'], 4),
            'addr_score': round(s['addr_score'], 4),
            'neighbor_score': round(s['neighbor_score'], 4),
            'data_score': round(s['data_score'], 4),
            'callgraph_score': round(s['callgraph_score'], 4),
            'callee_prop_score': round(s.get('callee_prop_score', 0.5), 4),
            'gap': round(gap, 4),
            'confidence': confidence,
            'region': retail_funcs[s['retail_addr']]['region'],
        })

        assigned_retail.add(s['retail_addr'])
        assigned_sym.add(s['sym_name'])

    # Pass 2: Use pass-1 results to enrich assigned_names for callee propagation
    pass1_count = len(results)
    for r in results:
        assigned_names[r['retail_addr']] = r['sym_name']

    # Re-score remaining candidates using enriched assigned_names
    remaining_cands = {addr: cands for addr, cands in candidates.items()
                       if addr not in assigned_retail}
    if remaining_cands:
        scored2 = []
        for retail_addr, cands in remaining_cands.items():
            pred_addr, uncertainty = drift_model.predict(retail_addr)
            window = max(1024, int(uncertainty * 6))
            window = min(window, 2048)

            for cand in cands:
                if cand['sym_name'] in assigned_sym:
                    continue
                a_score = score_address(
                    retail_addr, cand['sym_addr'], pred_addr, window)
                n_score = score_neighborhood(
                    retail_addr, cand['sym_addr'], anchors_sorted,
                    debug_symbols)
                d_score = score_data_xrefs(
                    retail_addr, cand['sym_name'], data_xrefs_map)
                c_score = score_callgraph(
                    retail_addr, cand['sym_name'], callees_map, callers_map,
                    assigned_names, set())
                cp_score = score_callee_propagation(
                    retail_addr, cand['sym_name'], callees_map,
                    assigned_names)

                total = compute_total_score(
                    a_score, n_score, d_score, c_score, cp_score)

                scored2.append({
                    'retail_addr': retail_addr,
                    'sym_addr': cand['sym_addr'],
                    'sym_name': cand['sym_name'],
                    'total_score': total,
                    'addr_score': a_score,
                    'neighbor_score': n_score,
                    'data_score': d_score,
                    'callgraph_score': c_score,
                    'callee_prop_score': cp_score,
                })

        scored2.sort(key=lambda s: s['total_score'], reverse=True)

        for s in scored2:
            if s['retail_addr'] in assigned_retail:
                continue
            if s['sym_name'] in assigned_sym:
                continue

            same_retail = [x for x in scored2
                           if x['retail_addr'] == s['retail_addr']
                           and x['sym_name'] != s['sym_name']
                           and x['sym_name'] not in assigned_sym]
            second_best = same_retail[0]['total_score'] if same_retail else 0.0
            gap = s['total_score'] - second_best

            if s['total_score'] >= 0.78 and gap >= 0.10:
                confidence = 'HIGH'
            elif s['total_score'] >= 0.70 and gap >= 0.05:
                confidence = 'MEDIUM'
            elif s['total_score'] >= 0.55:
                confidence = 'LOW'
            else:
                confidence = 'VERY_LOW'

            results.append({
                'retail_addr': s['retail_addr'],
                'retail_name': retail_funcs[s['retail_addr']]['name'],
                'sym_addr': s['sym_addr'],
                'sym_name': s['sym_name'],
                'total_score': round(s['total_score'], 4),
                'addr_score': round(s['addr_score'], 4),
                'neighbor_score': round(s['neighbor_score'], 4),
                'data_score': round(s['data_score'], 4),
                'callgraph_score': round(s['callgraph_score'], 4),
                'callee_prop_score': round(s.get('callee_prop_score', 0.5), 4),
                'gap': round(gap, 4),
                'confidence': confidence,
                'region': retail_funcs[s['retail_addr']]['region'],
            })

            assigned_retail.add(s['retail_addr'])
            assigned_sym.add(s['sym_name'])

    return results


# ---------------------------------------------------------------------------
# Phase 5: Collect unresolved functions and pre-symbol range
# ---------------------------------------------------------------------------

def collect_unresolved(retail_funcs, anchors, assignment_results):
    """Collect functions that couldn't be matched."""
    assigned = set(anchors.keys())
    assigned.update(r['retail_addr'] for r in assignment_results)

    unresolved = []
    pre_symbol = []

    for addr, func in sorted(retail_funcs.items()):
        if addr in assigned:
            continue
        entry = {
            'address': f'0x{addr:08X}',
            'name': func['name'],
            'region': func['region'],
            'type': func['type'],
            'reason': 'no_candidates',
        }
        if PRE_SYMBOL_START <= addr <= PRE_SYMBOL_END:
            entry['reason'] = 'pre_symbol_range'
            pre_symbol.append(entry)
        else:
            unresolved.append(entry)

    return unresolved, pre_symbol


# ---------------------------------------------------------------------------
# Output
# ---------------------------------------------------------------------------

def write_outputs(output_dir, anchors, results, unresolved, pre_symbol,
                  drift_model, retail_funcs, debug_symbols):
    """Write all output files."""
    os.makedirs(output_dir, exist_ok=True)

    # 1. Full results JSON
    full_output = {
        'metadata': {
            'total_retail_functions': len(retail_funcs),
            'total_debug_symbols': len(debug_symbols),
            'total_anchors': len(anchors),
            'total_new_assignments': len(results),
            'by_confidence': {},
            'unresolved_count': len(unresolved),
            'pre_symbol_count': len(pre_symbol),
        },
        'anchors': [
            {
                'retail_addr': f'0x{a["retail_addr"]:08X}',
                'sym_addr': f'0x{a["sym_addr"]:08X}',
                'sym_name': a['sym_name'],
                'drift': a['drift'],
                'confidence': a['confidence'],
            }
            for a in sorted(anchors.values(), key=lambda x: x['retail_addr'])
        ],
        'assignments': [],
        'unresolved': unresolved,
        'pre_symbol_range': pre_symbol,
    }

    # Count by confidence
    conf_counts = defaultdict(int)
    for r in results:
        conf_counts[r['confidence']] += 1
    full_output['metadata']['by_confidence'] = dict(conf_counts)

    # Format assignments
    for r in results:
        full_output['assignments'].append({
            'retail_addr': f'0x{r["retail_addr"]:08X}',
            'retail_name': r['retail_name'],
            'sym_addr': f'0x{r["sym_addr"]:08X}',
            'sym_name': r['sym_name'],
            'confidence': r['confidence'],
            'total_score': r['total_score'],
            'scores': {
                'address': r['addr_score'],
                'neighborhood': r['neighbor_score'],
                'data_xref': r['data_score'],
                'call_graph': r['callgraph_score'],
                'callee_propagation': r.get('callee_prop_score', 0.5),
            },
            'gap': r['gap'],
            'region': r['region'],
        })

    with open(output_dir / 'symbol_match_results.json', 'w') as f:
        json.dump(full_output, f, indent=2)

    # 2. Drift model visualization
    drift_data = []
    for seg in drift_model.segments:
        drift_data.append({
            'center_addr': f'0x{seg["center_addr"]:08X}',
            'mean_drift': round(seg['mean_drift'], 2),
            'uncertainty': round(seg['uncertainty'], 2),
            'n_anchors': seg['n_anchors'],
        })
    with open(output_dir / 'drift_model.json', 'w') as f:
        json.dump(drift_data, f, indent=2)

    # 3. Human-readable summary
    with open(output_dir / 'SUMMARY.md', 'w') as f:
        f.write('# Advanced Symbol Matching Results\n\n')
        f.write(f'- Retail functions: {len(retail_funcs)}\n')
        f.write(f'- Debug symbols: {len(debug_symbols)}\n')
        f.write(f'- Anchors (verified+exact+high): {len(anchors)}\n')
        f.write(f'- New assignments: {len(results)}\n')
        for conf in ['HIGH', 'MEDIUM', 'LOW', 'VERY_LOW']:
            c = conf_counts.get(conf, 0)
            f.write(f'  - {conf}: {c}\n')
        f.write(f'- Unresolved: {len(unresolved)}\n')
        f.write(f'- Pre-symbol range: {len(pre_symbol)}\n\n')

        # Drift model summary
        f.write('## Drift Model\n\n')
        f.write('| Region | Anchor Count | Drift Range |\n')
        f.write('|--------|-------------|-------------|\n')
        gcc_a = [a for a in anchors.values() if a['retail_addr'] < 0x80086000]
        ido_a = [a for a in anchors.values() if a['retail_addr'] >= 0x80086000]
        if gcc_a:
            gd = [a['drift'] for a in gcc_a]
            f.write(f'| GCC | {len(gcc_a)} | {min(gd):+d} to {max(gd):+d} |\n')
        if ido_a:
            id_ = [a['drift'] for a in ido_a]
            f.write(f'| IDO | {len(ido_a)} | {min(id_):+d} to {max(id_):+d} |\n')

        # HIGH confidence matches
        high = [r for r in results if r['confidence'] == 'HIGH']
        if high:
            f.write(f'\n## HIGH Confidence Matches ({len(high)})\n\n')
            f.write('| Address | Current Name | Symbol | Score | Gap |\n')
            f.write('|---------|-------------|--------|-------|-----|\n')
            for r in sorted(high, key=lambda x: x['retail_addr']):
                f.write(f'| 0x{r["retail_addr"]:08X} | {r["retail_name"]} '
                        f'| {r["sym_name"]} | {r["total_score"]:.3f} '
                        f'| {r["gap"]:.3f} |\n')

        # MEDIUM confidence matches
        med = [r for r in results if r['confidence'] == 'MEDIUM']
        if med:
            f.write(f'\n## MEDIUM Confidence Matches ({len(med)})\n\n')
            f.write('| Address | Current Name | Symbol | Score | Gap |\n')
            f.write('|---------|-------------|--------|-------|-----|\n')
            for r in sorted(med, key=lambda x: x['retail_addr']):
                f.write(f'| 0x{r["retail_addr"]:08X} | {r["retail_name"]} '
                        f'| {r["sym_name"]} | {r["total_score"]:.3f} '
                        f'| {r["gap"]:.3f} |\n')

        # LOW confidence (just count)
        low = [r for r in results if r['confidence'] in ('LOW', 'VERY_LOW')]
        if low:
            f.write(f'\n## LOW/VERY_LOW Confidence ({len(low)} matches)\n\n')
            f.write('See `symbol_match_results.json` for full details.\n')

    # 4. Triage report for manual review
    with open(output_dir / 'triage_report.txt', 'w') as f:
        f.write('# Symbol Matching Triage Report\n')
        f.write('# Functions needing manual review\n\n')

        # MEDIUM matches first (quick wins)
        med = [r for r in results if r['confidence'] == 'MEDIUM']
        if med:
            f.write(f'=== MEDIUM CONFIDENCE ({len(med)}) ===\n')
            f.write('(Review these first - likely correct but need verification)\n\n')
            for r in sorted(med, key=lambda x: x['retail_addr']):
                f.write(f'0x{r["retail_addr"]:08X} size=?? '
                        f'file={r["retail_name"]} region={r["region"]}\n')
                f.write(f'  -> {r["sym_name"]} '
                        f'(score={r["total_score"]:.3f}, gap={r["gap"]:.3f})\n')
                f.write(f'  scores: addr={r["addr_score"]:.2f} '
                        f'neigh={r["neighbor_score"]:.2f} '
                        f'data={r["data_score"]:.2f} '
                        f'cg={r["callgraph_score"]:.2f}\n\n')

        # LOW matches
        low = [r for r in results if r['confidence'] == 'LOW']
        if low:
            f.write(f'\n=== LOW CONFIDENCE ({len(low)}) ===\n\n')
            for r in sorted(low, key=lambda x: x['retail_addr']):
                f.write(f'0x{r["retail_addr"]:08X} '
                        f'file={r["retail_name"]} region={r["region"]}\n')
                f.write(f'  -> {r["sym_name"]} '
                        f'(score={r["total_score"]:.3f}, gap={r["gap"]:.3f})\n\n')

        # Unresolved
        if unresolved:
            f.write(f'\n=== UNRESOLVED ({len(unresolved)}) ===\n\n')
            for u in unresolved:
                f.write(f'{u["address"]} {u["name"]} '
                        f'region={u["region"]} reason={u["reason"]}\n')

    print(f'Results written to {output_dir}/')


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    import argparse
    parser = argparse.ArgumentParser(
        description='Advanced symbol matching with drift zones and scoring')
    parser.add_argument('--output-dir', default='build/symbol_match',
                        help='Output directory')
    args = parser.parse_args()

    output_dir = Path(args.output_dir)

    print('Phase 0: Loading data...')
    retail_funcs = load_retail_functions()
    debug_symbols = load_debug_symbols()
    proximity_report = load_proximity_report()
    function_map = load_function_map()
    already_renamed = load_already_renamed()

    print(f'  Retail functions: {len(retail_funcs)}')
    print(f'  Debug symbols: {len(debug_symbols)}')
    print(f'  Already renamed: {len(already_renamed)}')

    print('\nPhase 1: Building drift model...')
    anchors = extract_anchors(proximity_report, already_renamed, debug_symbols)
    print(f'  Deduplicated anchors: {len(anchors)}')

    drift_model = build_drift_model(anchors, window_size=12)
    if drift_model is None:
        print('ERROR: No anchors found, cannot build drift model')
        sys.exit(1)

    # Show drift model stats
    gcc_a = [a for a in anchors.values() if a['retail_addr'] < 0x80086000]
    ido_a = [a for a in anchors.values() if a['retail_addr'] >= 0x80086000]
    print(f'  GCC anchors: {len(gcc_a)}')
    print(f'  IDO anchors: {len(ido_a)}')

    print('\nPhase 2: Extracting call graph and data xrefs...')
    callees_map, callers_map = extract_call_graph(function_map)
    data_xrefs_map = extract_data_xrefs(function_map)
    metrics = extract_function_metrics(function_map)
    print(f'  Functions with call data (function_map): {len(callees_map)}')
    print(f'  Functions with data xrefs: {len(data_xrefs_map)}')

    print('\nPhase 2b: Extracting call graph from source files...')
    src_callees, src_callers = extract_call_graph_from_sources(retail_funcs)
    print(f'  Functions with call data (sources): {len(src_callees)}')
    new_from_sources = len(set(src_callees.keys()) - set(callees_map.keys()))
    print(f'  New functions covered (not in function_map): {new_from_sources}')

    # Merge call graphs
    callees_map, callers_map = merge_call_graphs(
        callees_map, callers_map, src_callees, src_callers)
    print(f'  Merged call graph: {len(callees_map)} functions with call data')

    print('\nPhase 3: Generating candidates...')
    candidates = generate_candidates(
        retail_funcs, debug_symbols, drift_model, anchors)
    print(f'  Functions with candidates: {len(candidates)}')
    total_cands = sum(len(c) for c in candidates.values())
    print(f'  Total candidate pairs: {total_cands}')

    print('\nPhase 4: Scoring and assignment...')
    results = run_assignment(
        retail_funcs, candidates, drift_model, anchors,
        callees_map, callers_map, data_xrefs_map, debug_symbols)

    conf_counts = defaultdict(int)
    for r in results:
        conf_counts[r['confidence']] += 1
    print(f'  Assignments: {len(results)}')
    for conf in ['HIGH', 'MEDIUM', 'LOW', 'VERY_LOW']:
        print(f'    {conf}: {conf_counts.get(conf, 0)}')

    print('\nPhase 5: Collecting unresolved...')
    unresolved, pre_symbol = collect_unresolved(
        retail_funcs, anchors, results)
    print(f'  Unresolved: {len(unresolved)}')
    print(f'  Pre-symbol range: {len(pre_symbol)}')

    print('\nWriting outputs...')
    write_outputs(output_dir, anchors, results, unresolved, pre_symbol,
                  drift_model, retail_funcs, debug_symbols)

    # Final summary
    total_named = len(anchors) + len(results)
    print(f'\n=== SUMMARY ===')
    print(f'Total functions with symbols: {total_named} / {len(retail_funcs)} '
          f'({100*total_named/len(retail_funcs):.1f}%)')
    print(f'  Anchors (verified): {len(anchors)}')
    print(f'  New HIGH: {conf_counts.get("HIGH", 0)}')
    print(f'  New MEDIUM: {conf_counts.get("MEDIUM", 0)}')
    print(f'  New LOW: {conf_counts.get("LOW", 0)}')
    print(f'  New VERY_LOW: {conf_counts.get("VERY_LOW", 0)}')
    print(f'  Unresolved: {len(unresolved)}')
    print(f'  Pre-symbol range (no coverage): {len(pre_symbol)}')


if __name__ == '__main__':
    main()
