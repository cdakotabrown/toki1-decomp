#!/usr/bin/env python3
"""generate_status_json.py — emit a byte-perfect status JSON for every
overlay function, mirroring the shape of
`config/asm_conversion_status.json` (main binary tracker).

Output: `config/overlay_conversion_status.json`

Schema:
  metadata   description / timestamps / tooling info
  summary    project-wide counts (total / matched / will_match / ...)
  overlays   per-overlay counts
  functions  per-function record keyed by "<OVERLAY>::<func_name>"

Per-function record:
  overlay          overlay name (e.g. "GEKO")
  name             function symbol (e.g. "func_80132000")
  status           one of: matched | will_match | unmatched
                          | no_target | compile_fail | missing_c
  target_insns     instruction count from `asm/overlay/<OVL>/<fn>.s`
  compiled_insns   raw IDO compile count
  insn_diff        compiled - target (pre-PostProc)
  effective_diff   diff after accounting for allowed strip/pad PostProc
  c_file           path (repo-relative) to the C source, or null
  postproc         list of declared @PostProc: directives
  compile_flags    list of declared @CompileFlags: directives
  compiler         "ido5.2" (default) or "gcc260"/"gcc281" if annotated
  last_built       UTC timestamp

This JSON is the canonical tracker for overlay work — the same way
asm_conversion_status.json is for the main binary.
"""

import datetime as _dt
import json
import sys
from pathlib import Path
from typing import Any, Dict, List

# Import helpers from the overlay build pipeline
TOOLS_DIR = Path(__file__).parent
sys.path.insert(0, str(TOOLS_DIR))
sys.path.insert(0, str(TOOLS_DIR.parent / "build"))
from banned_postproc import count_banned_in_source
from build_overlay import (  # type: ignore
    SRC_OVERLAY_DIR,
    ASM_OVERLAY_DIR,
    BUILD_DIR,
    compile_function,
    get_all_overlays,
    get_obj_insn_count,
    get_target_insn_count,
    parse_compile_flags,
    parse_postproc,
)

ROOT = TOOLS_DIR.parent.parent
OUT_JSON = ROOT / "config" / "overlay_conversion_status.json"


def detect_compiler(c_file: Path) -> str:
    try:
        with open(c_file, "r") as f:
            for _ in range(40):
                line = f.readline()
                if not line:
                    break
                m = line.strip().lower()
                if "@compiler:" in m:
                    val = m.split("@compiler:", 1)[1].strip()
                    return val.split()[0] if val else "ido5.2"
    except OSError:
        pass
    return "ido5.2"


def status_for(
    diff: int,
    postproc: List[Dict[str, Any]],
) -> (str, int):
    """Compute (status, effective_diff) using allowed PostProc math only.

    The policy-allowed trailing-nop adjustments (strip/pad) are the only
    ones that change instruction count, so they're the only PostProc we
    model here. Other allowed PostProc (swap_registers, scheduler_*,
    patch_stack_complete, etc.) preserve count; unallowed ones (banned)
    aren't represented in the JSON.
    """
    strip = sum(s["count"] for s in postproc if s["type"] == "strip_trailing")
    pad = sum(s["count"] for s in postproc if s["type"] == "pad_trailing")
    eff = diff
    if diff > 0 and strip:
        eff = diff - strip
    elif diff < 0 and pad:
        eff = diff + pad
    if diff == 0:
        return "matched", 0
    if eff == 0:
        return "will_match", 0
    return "unmatched", eff


def analyze_overlay(name: str) -> Dict[str, Any]:
    src = SRC_OVERLAY_DIR / name
    asm = ASM_OVERLAY_DIR / name
    obj_dir = BUILD_DIR / name / "obj"
    obj_dir.mkdir(parents=True, exist_ok=True)

    c_files = sorted(src.glob("*.c")) if src.exists() else []
    asm_files = sorted(asm.glob("*.s")) if asm.exists() else []

    by_name: Dict[str, Dict[str, Any]] = {}

    # Seed with ASM files (authoritative symbol set)
    for a in asm_files:
        fn = a.stem
        by_name[fn] = {
            "overlay": name,
            "name": fn,
            "status": "missing_c",
            "target_insns": get_target_insn_count(a),
            "compiled_insns": None,
            "insn_diff": None,
            "effective_diff": None,
            "c_file": None,
            "postproc": [],
            "compile_flags": [],
            "compiler": None,
            "banned_postproc_count": 0,
        }

    for c in c_files:
        fn = c.stem
        rec = by_name.setdefault(fn, {
            "overlay": name,
            "name": fn,
            "status": "no_target",
            "target_insns": 0,
            "compiled_insns": None,
            "insn_diff": None,
            "effective_diff": None,
            "c_file": None,
            "postproc": [],
            "compile_flags": [],
            "compiler": None,
            "banned_postproc_count": 0,
        })
        rec["c_file"] = str(c.relative_to(ROOT))
        rec["postproc"] = [
            p.get("raw") or f"{p['type']} --count {p.get('count', '')}".strip()
            for p in parse_postproc(c)
        ]
        rec["compile_flags"] = parse_compile_flags(c)
        rec["compiler"] = detect_compiler(c)

        # Check for banned PostProc directives
        try:
            with open(c, "r", encoding="utf-8") as f:
                src_content = f.read()
            rec["banned_postproc_count"] = count_banned_in_source(src_content)
        except Exception:
            pass

        target = rec["target_insns"]
        if target == 0:
            rec["status"] = "no_target"
            continue

        obj = compile_function(c, obj_dir, apply_pp=False, verbose=False)
        if not obj:
            rec["status"] = "compile_fail"
            continue

        compiled = get_obj_insn_count(obj)
        diff = compiled - target
        postproc = parse_postproc(c)
        status, eff = status_for(diff, postproc)
        rec["compiled_insns"] = compiled
        rec["insn_diff"] = diff
        rec["effective_diff"] = eff
        rec["status"] = status

    # Summarize
    summary = {
        "total": len(by_name),
        "matched": 0,
        "will_match": 0,
        "unmatched": 0,
        "no_target": 0,
        "compile_fail": 0,
        "missing_c": 0,
    }
    for rec in by_name.values():
        summary[rec["status"]] = summary.get(rec["status"], 0) + 1

    return {"summary": summary, "functions": by_name}


def main():
    overlays = get_all_overlays()
    print(f"[status-json] analyzing {len(overlays)} overlays")

    all_functions: Dict[str, Dict[str, Any]] = {}
    per_overlay: Dict[str, Dict[str, int]] = {}
    grand: Dict[str, int] = {
        "total": 0, "matched": 0, "will_match": 0, "unmatched": 0,
        "no_target": 0, "compile_fail": 0, "missing_c": 0,
    }

    for ovl in overlays:
        result = analyze_overlay(ovl)
        per_overlay[ovl] = result["summary"]
        for k, v in result["summary"].items():
            grand[k] = grand.get(k, 0) + v
        for fn, rec in result["functions"].items():
            key = f"{ovl}::{fn}"
            all_functions[key] = rec
        s = result["summary"]
        print(f"  {ovl:<10} total={s['total']:<4} matched={s['matched']:<4} "
              f"will_match={s['will_match']:<4} unmatched={s['unmatched']:<4} "
              f"missing_c={s['missing_c']:<4} compile_fail={s['compile_fail']:<4}")

    grand["effective_match"] = grand["matched"] + grand["will_match"]
    grand["effective_pct"] = (
        round(100.0 * grand["effective_match"] / grand["total"], 2)
        if grand["total"] else 0.0
    )

    now = _dt.datetime.utcnow().isoformat(timespec="seconds") + "Z"
    doc = {
        "metadata": {
            "description": "Per-function byte-perfect status for every EXN overlay "
                           "function. Mirrors config/asm_conversion_status.json "
                           "(main binary) for the overlay tree.",
            "generated": now,
            "schema_version": 1,
            "generator": "tools/overlay/generate_status_json.py",
            "notes": [
                "status values: matched | will_match | unmatched | no_target | "
                "compile_fail | missing_c.",
                "'matched' = raw IDO compile byte-matches target insn count.",
                "'will_match' = needs allowed strip_trailing / pad_trailing nop "
                "PostProc to reach byte-perfect. Other allowed PostProc "
                "(swap_registers, scheduler_*, patch_stack_complete, etc.) "
                "preserve count; they're declared in 'postproc' but don't "
                "change status classification.",
                "'unmatched' = instruction count diverges beyond allowed "
                "PostProc corrections.",
                "'missing_c' = target ASM exists but no C source yet.",
                "BANNED PostProc (patch_insn_word, patch_instruction, "
                "insert_instruction_and_shift, insert_move_and_shift, "
                "hardcoded-byte custom_script) are tracked in "
                "'banned_postproc_count'. Any value > 0 is a policy violation "
                "and will cause test/build tools to abort.",
            ],
        },
        "summary": grand,
        "overlays": per_overlay,
        "functions": all_functions,
    }

    OUT_JSON.parent.mkdir(parents=True, exist_ok=True)
    with open(OUT_JSON, "w") as f:
        json.dump(doc, f, indent=2, sort_keys=False)

    print(
        f"\n[status-json] wrote {OUT_JSON.relative_to(ROOT)} — "
        f"{grand['total']} funcs, {grand['matched']} matched, "
        f"{grand['will_match']} will_match, {grand['unmatched']} unmatched, "
        f"{grand['effective_match']} effective ({grand['effective_pct']}%)"
    )


if __name__ == "__main__":
    main()
