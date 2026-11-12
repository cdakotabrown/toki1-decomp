#!/usr/bin/env python3
"""Re-audit ASM-ONLY verdicts by ACTUALLY compiling their retained C.

An `asm_only` label is a *judgment* ("unmatchable from C"), not a fact -- and
judgments made with older tooling go stale as the compiler/harness improve.
This tool operationalises "keep an open mind about asm_only": for every
asm_only function that still carries a retained C body, it compiles that C and
checks whether it matches TODAY. Any hit is a stale verdict = a free win.

CRITICAL: the match harness SHORT-CIRCUITS when it sees "Status: ASM-ONLY" /
"@AsmOnly" in the source (it reports byte-perfect because the ROM ships the raw
ASM). A naive re-run therefore reports 100% "byte-perfect" and is meaningless.
This tool defeats that by writing a marker-STRIPPED copy to a temp file and
pointing the harness at it via --src / --source, so the retained C is genuinely
compiled. Tracked files are never mutated.

Scope: overlays (status==asm_only in overlay_match_status.json) and main
(src/main/**/*.c carrying an ASM-ONLY marker).

Outputs: config/asm_only_reaudit.json + stdout summary. Re-run after any
compiler/harness/hook change; a non-empty `stale` list is actionable.

Usage: .venv/bin/python3 tools/analysis/reaudit_asm_only.py [--limit N]
"""
import json
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OV_STATUS = ROOT / "config/overlay_match_status.json"
OUT = ROOT / "config/asm_only_reaudit.json"
OV_TESTER = ROOT / "tools/overlay/test_overlay_match.py"
MAIN_TESTER = ROOT / "tools/analysis/test_single_match.py"

MARKER = re.compile(r"Status:\s*ASM-ONLY|@AsmOnly|ASM_FINAL|ASM-only|"
                    r"CONFIRMED UNMATCHABLE|will not match from C|"
                    r"unmatchable", re.IGNORECASE)


def has_real_body(text):
    return (text.count(";") > 3 and "return" in text) or text.count("{") > 1


def stripped_temp(src_path):
    """Write a copy with the ASM-ONLY marker neutralised; return temp path."""
    text = src_path.read_text(errors="ignore")
    # Neutralise every known short-circuit marker so the harness compiles the
    # retained C instead of trusting the ASM label.
    text = text.replace("Status: ASM-ONLY", "Status: (reaudit)")
    text = text.replace("ASM-ONLY", "(reaudit)").replace("@AsmOnly", "")
    text = text.replace("ASM_FINAL", "(reaudit)")
    tf = tempfile.NamedTemporaryFile("w", suffix=".c", delete=False,
                                     dir=src_path.parent)
    tf.write(text)
    tf.close()
    return Path(tf.name)


def verdict_of(output):
    up = output.upper()
    if "BYTE-PERFECT" in up:
        return "byte_perfect"
    if "RELOCATION-ONLY" in up or "RELOC-ONLY" in up:
        return "reloc_only"
    return "mismatch"


def retest_overlay(overlay, fn, src):
    tmp = stripped_temp(src)
    try:
        p = subprocess.run(
            [sys.executable, str(OV_TESTER), overlay, fn, "--src", str(tmp)],
            capture_output=True, text=True, timeout=60)
        return verdict_of(p.stdout + p.stderr)
    except subprocess.TimeoutExpired:
        return "timeout"
    finally:
        tmp.unlink(missing_ok=True)


def retest_main(fn, src):
    tmp = stripped_temp(src)
    try:
        p = subprocess.run(
            [sys.executable, str(MAIN_TESTER), fn, "--source", str(tmp)],
            capture_output=True, text=True, timeout=60)
        return verdict_of(p.stdout + p.stderr)
    except subprocess.TimeoutExpired:
        return "timeout"
    finally:
        tmp.unlink(missing_ok=True)


def overlay_candidates():
    ov = json.loads(OV_STATUS.read_text())
    out = []
    for overlay, fns in ov.items():
        if not isinstance(fns, dict):
            continue
        for fn, meta in fns.items():
            if not isinstance(meta, dict) or meta.get("status") != "asm_only":
                continue
            src = ROOT / f"src/overlay/{overlay}/{fn}.c"
            if src.exists() and has_real_body(src.read_text(errors="ignore")):
                out.append((overlay, fn, src))
    return sorted(out)


def _matched_main():
    """Functions already verified-matched in level3 -- a historical ASM-ONLY
    mention in their comment must NOT re-flag them as stale."""
    try:
        d = json.loads((ROOT / "config/level3_functions.json").read_text())
        return {e["name"] for e in d["functions"]
                if e.get("match_status") == "verified"}
    except Exception:
        return set()


def main_candidates():
    # Skip pc-port (PC re-implementation, not a byte-match target) and anything
    # already matched (stale comment mentions of ASM-ONLY are not verdicts).
    matched = _matched_main()
    out = []
    for src in sorted((ROOT / "src/main").rglob("*.c")):
        if "pc-port" in src.parts or src.stem in matched:
            continue
        text = src.read_text(errors="ignore")
        if MARKER.search(text) and has_real_body(text):
            out.append((src.stem, src))
    return out


def main():
    limit = int(sys.argv[sys.argv.index("--limit") + 1]) \
        if "--limit" in sys.argv else None
    results = {"overlay": {}, "main": {}}
    stale = []

    ovc = overlay_candidates()
    if limit:
        ovc = ovc[:limit]
    for i, (overlay, fn, src) in enumerate(ovc, 1):
        v = retest_overlay(overlay, fn, src)
        results["overlay"][f"{overlay}/{fn}"] = v
        if v in ("byte_perfect", "reloc_only"):
            stale.append(("overlay", f"{overlay}/{fn}", v))
        if i % 25 == 0:
            print(f"  [overlay {i}/{len(ovc)}]", flush=True)

    mc = main_candidates()
    if limit:
        mc = mc[:limit]
    for i, (fn, src) in enumerate(mc, 1):
        v = retest_main(fn, src)
        results["main"][fn] = v
        if v in ("byte_perfect", "reloc_only"):
            stale.append(("main", fn, v))
        if i % 25 == 0:
            print(f"  [main {i}/{len(mc)}]", flush=True)

    def tally(d):
        t = {}
        for v in d.values():
            t[v] = t.get(v, 0) + 1
        return t

    manifest = {
        "generated_by": "tools/analysis/reaudit_asm_only.py",
        "overlay_tested": len(results["overlay"]),
        "main_tested": len(results["main"]),
        "overlay_tally": tally(results["overlay"]),
        "main_tally": tally(results["main"]),
        "stale": [{"scope": s, "name": n, "verdict": v} for s, n, v in stale],
        "results": results,
    }
    OUT.write_text(json.dumps(manifest, indent=1))

    print(f"\nRe-audited asm_only retained-C: "
          f"{len(results['overlay'])} overlay + {len(results['main'])} main")
    print(f"  overlay: {tally(results['overlay'])}")
    print(f"  main:    {tally(results['main'])}")
    if stale:
        print(f"\n⭐ {len(stale)} GENUINELY STALE (retained C matches now -- "
              f"promote!):")
        for s, n, v in stale:
            print(f"   {s} {n}: {v}")
    else:
        print("\nNo stale verdicts today -- all asm_only retained C still "
              "mismatches. Re-run after tooling changes.")
    print(f"-> {OUT.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
