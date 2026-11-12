#!/usr/bin/env python3
"""
regression_pipeline.py - Automated regression testing for uopt hooks and PostProc changes

Re-tests all semi-matched and WIP functions with specified environment variables
(uopt hooks). Useful for detecting when a new hook or tool change improves (or
regresses) function matching.

Outputs a sorted CSV with instruction counts, deltas, and match percentages.
Highlights functions where compiled instruction count exactly matches the target
(delta == 0), since those are candidates for byte-perfect matching.

Usage:
    # Baseline run (no hooks)
    python3 tools/analysis/regression_pipeline.py

    # Single hook setting
    python3 tools/analysis/regression_pipeline.py --env "IDO52_CSE_MAX_REUSE=5"

    # Multiple hooks
    python3 tools/analysis/regression_pipeline.py --env "IDO52_CSE_MAX_REUSE=5" --env "IDO52_NO_LICM=all"

    # Sweep a hook across a range of values
    python3 tools/analysis/regression_pipeline.py --sweep-hook "IDO52_CSE_MAX_REUSE=1:10"

    # Test only WIP files
    python3 tools/analysis/regression_pipeline.py --wip-only

    # Test only semi-matched files
    python3 tools/analysis/regression_pipeline.py --semi-only

    # Limit to N functions (for quick testing)
    python3 tools/analysis/regression_pipeline.py --limit 5

    # Parallel execution
    python3 tools/analysis/regression_pipeline.py --parallel 8

    # Compare two runs
    python3 tools/analysis/regression_pipeline.py --compare build/regression_baseline.csv build/regression_hook.csv
"""

import argparse
import csv
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor, as_completed
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path
from typing import Dict, List, Optional, Tuple

# Project paths
ROOT_DIR = Path(__file__).parent.parent.parent
SEMI_DIR = ROOT_DIR / "src" / "main" / "semi-matched"
WIP_DIR = ROOT_DIR / "src" / "main" / "wip"
BUILD_DIR = ROOT_DIR / "build"
LEVEL3_CONFIG = ROOT_DIR / "config" / "level3_functions.json"
ORIGINAL_EXE = ROOT_DIR / "disks" / "SLPS_000.65"
IDO_COMPILER = ROOT_DIR / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
PATCH_SCRIPT = ROOT_DIR / "tools" / "ido" / "patch_stack_complete.py"
CONVERT_SCRIPT = ROOT_DIR / "tools" / "ido" / "convert_or_to_addu.py"

# Constants
TEXT_SECTION_START = 0x80041000
FILE_OFFSET_BASE = 0x800
GAME_REGION_END = 0x80086000


@dataclass
class FunctionResult:
    """Result of testing a single function."""
    function: str
    source_dir: str  # "semi-matched" or "wip"
    address: int
    target_insns: int
    compiled_insns: int
    delta: int  # compiled - target (0 = exact count match)
    match_pct: float
    matching_insns: int
    total_insns: int
    status: str  # "ok", "compile_error", "no_info", "no_text", "skipped"
    error: str = ""


def load_level3_config() -> Dict:
    """Load the level3 functions configuration."""
    if not LEVEL3_CONFIG.exists():
        print(f"Error: Level3 config not found at {LEVEL3_CONFIG}", file=sys.stderr)
        sys.exit(1)
    with open(LEVEL3_CONFIG, 'r') as f:
        return json.load(f)


def get_function_info(func_name: str, config: Dict) -> Optional[Dict]:
    """Get function metadata from level3_functions.json."""
    for func in config.get('functions', []):
        if func['name'] == func_name:
            if func.get('size', 0) == 0:
                func['size'] = get_size_from_asm(func_name)
            return func
    return None


def get_size_from_asm(func_name: str) -> int:
    """Extract function size from ASM file header comment."""
    asm_path = ROOT_DIR / "asm" / "main" / "matched" / f"{func_name}.s"
    if not asm_path.exists():
        return 0
    try:
        with open(asm_path, 'r') as f:
            for line in f:
                match = re.search(r'Size:\s*(\d+)\s*bytes', line)
                if match:
                    return int(match.group(1))
                if not line.startswith(('#', '/', '*', ' ')):
                    break
        return 0
    except Exception:
        return 0


def vram_to_file_offset(vram_addr: int) -> int:
    return (vram_addr - TEXT_SECTION_START) + FILE_OFFSET_BASE


def extract_original_bytes(vram_start: int, size: int) -> bytes:
    file_offset = vram_to_file_offset(vram_start)
    with open(ORIGINAL_EXE, 'rb') as f:
        f.seek(file_offset)
        return f.read(size)


# Directive lines only OPEN with the keyword (after comment markers). Prose
# that quotes "@CompileFlags:" mid-sentence must not be parsed as a directive.
_COMPILEFLAGS_LINE = re.compile(
    r'^\s*(?:(?:/\*+|\*+|//)\s*)*@CompileFlags:\s*(.*)$')


def parse_compile_flags_from_source(source_file: Path) -> List[str]:
    """Parse @CompileFlags from source file header."""
    flags = []
    try:
        with open(source_file) as f:
            for line in f:
                if line.strip() == '*/':
                    break
                m = _COMPILEFLAGS_LINE.match(line)
                if m:
                    flag_str = m.group(1).strip().rstrip('*').strip()
                    flags.extend(flag_str.split())
    except Exception:
        pass
    return flags


def compile_with_ido(source_file: Path, output_obj: Path,
                     extra_env: Dict[str, str] = None) -> Tuple[bool, str]:
    """Compile C source with IDO 5.2. Returns (success, error_msg)."""
    cmd = [
        str(IDO_COMPILER),
        "-O2", "-G0", "-mips1", "-EL", "-non_shared",
    ]

    # Parse @CompileFlags from source
    extra_flags = parse_compile_flags_from_source(source_file)
    if extra_flags:
        cmd.extend(extra_flags)

    cmd.extend([
        "-I", str(ROOT_DIR / "include"),
        "-c", str(source_file),
        "-o", str(output_obj)
    ])

    env = os.environ.copy()
    if extra_env:
        env.update(extra_env)

    try:
        result = subprocess.run(cmd, capture_output=True, text=True,
                                timeout=60, env=env)
        if result.returncode != 0:
            return False, result.stderr.strip()[:200]
        return True, ""
    except subprocess.TimeoutExpired:
        return False, "compilation timeout"
    except Exception as e:
        return False, str(e)[:200]


def detect_frame_size(obj_file: Path) -> Optional[int]:
    cmd = ["mipsel-linux-gnu-objdump", "-d", str(obj_file)]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        return None
    for line in result.stdout.split('\n'):
        if 'addiu' in line and 'sp,sp,' in line and '-' in line:
            match = re.search(r'sp,sp,(-\d+)', line)
            if match:
                return abs(int(match.group(1)))
    return None


def post_process(input_obj: Path, output_obj: Path, is_game_region: bool) -> bool:
    """Apply region-appropriate post-processing."""
    if is_game_region:
        detected = detect_frame_size(input_obj)
        if detected:
            frame_pairs = [(detected, detected + 16)]
            if detected != 24:
                frame_pairs.append((24, 40))
        else:
            frame_pairs = [(s, s + 16) for s in range(24, 256, 8)]

        for old, new in frame_pairs:
            cmd = [
                "python3", str(PATCH_SCRIPT),
                str(input_obj), str(output_obj),
                "--old", str(old), "--new", str(new)
            ]
            result = subprocess.run(cmd, capture_output=True, text=True)
            if result.returncode == 0:
                return True
        shutil.copy(input_obj, output_obj)
        return True
    else:
        cmd = [
            "python3", str(CONVERT_SCRIPT),
            str(input_obj), str(output_obj)
        ]
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode != 0:
            shutil.copy(input_obj, output_obj)
        return True


def extract_text_section(obj_file: Path) -> Optional[bytes]:
    bin_file = obj_file.with_suffix('.bin')
    cmd = [
        "mipsel-linux-gnu-objcopy",
        "-O", "binary", "-j", ".text",
        str(obj_file), str(bin_file)
    ]
    result = subprocess.run(cmd, capture_output=True)
    if result.returncode != 0 or not bin_file.exists():
        return None
    with open(bin_file, 'rb') as f:
        data = f.read()
    bin_file.unlink()
    return data


def disassemble_bytes(data: bytes, base_addr: int = 0) -> List[str]:
    """Disassemble raw bytes, return list of instruction strings."""
    with tempfile.NamedTemporaryFile(suffix='.bin', delete=False) as tmp:
        tmp.write(data)
        tmp_path = tmp.name
    try:
        cmd = [
            "mipsel-linux-gnu-objdump",
            "-D", "-b", "binary", "-m", "mips:3000",
            "--adjust-vma", hex(base_addr), tmp_path
        ]
        result = subprocess.run(cmd, capture_output=True, text=True)
        instructions = []
        for line in result.stdout.split('\n'):
            if ':' in line and '\t' in line:
                parts = line.strip().split('\t')
                if len(parts) >= 3:
                    instructions.append('\t'.join(parts[2:]).strip())
        return instructions
    finally:
        os.unlink(tmp_path)


def disassemble_obj(obj_file: Path, base_addr: int = 0) -> List[str]:
    """Disassemble object file, return list of instructions (skip relocations)."""
    cmd = [
        "mipsel-linux-gnu-objdump", "-d", "-r",
        "--adjust-vma", hex(base_addr), str(obj_file)
    ]
    result = subprocess.run(cmd, capture_output=True, text=True)
    instructions = []
    for line in result.stdout.split('\n'):
        if 'R_MIPS' in line:
            continue
        if ':' in line and '\t' in line:
            parts = line.strip().split('\t')
            if len(parts) >= 3:
                instructions.append('\t'.join(parts[2:]).strip())
    return instructions


def normalize_instruction(instr: str) -> str:
    instr = re.sub(r'<[^>]+>', '', instr)
    return ' '.join(instr.split()).strip()


def compare_instruction_lists(orig: List[str], comp: List[str]) -> Tuple[int, int]:
    """Compare instruction lists. Returns (matching_count, total_count)."""
    max_len = max(len(orig), len(comp))
    matching = 0

    for i in range(max_len):
        o = normalize_instruction(orig[i]) if i < len(orig) else ""
        c = normalize_instruction(comp[i]) if i < len(comp) else ""

        if not o and not c:
            continue

        if o == c:
            matching += 1
            continue

        opcode_o = o.split()[0] if o else ""
        opcode_c = c.split()[0] if c else ""

        if opcode_o == opcode_c:
            if opcode_o in ('jal', 'b', 'beq', 'bne', 'beqz', 'bnez',
                            'bgez', 'bltz', 'blez', 'bgtz'):
                matching += 1
                continue
            if opcode_o in ('lui', 'addiu', 'ori', 'lw', 'sw', 'lh', 'sh',
                            'lb', 'sb', 'lbu', 'lhu'):
                if '0x0' in c:
                    matching += 1
                    continue

    return matching, max_len


def extract_func_name(source_path: Path) -> str:
    """Extract canonical function name from source file path."""
    stem = source_path.stem
    # Handle variant files like func_XXX_v1
    match = re.match(r'(func_[0-9a-fA-F]+)(?:_v\d+|_body.*|_param|_reread)?$', stem)
    if match:
        return match.group(1)
    return stem


def test_function(source_path: Path, config: Dict,
                  extra_env: Dict[str, str]) -> FunctionResult:
    """Test a single function file. Returns a FunctionResult."""
    func_name = extract_func_name(source_path)
    source_dir = source_path.parent.name  # "semi-matched" or "wip"

    func_info = get_function_info(func_name, config)
    if not func_info:
        return FunctionResult(
            function=func_name, source_dir=source_dir, address=0,
            target_insns=0, compiled_insns=0, delta=0, match_pct=0.0,
            matching_insns=0, total_insns=0, status="no_info",
            error=f"not in level3_functions.json"
        )

    vram_addr = int(func_info['address'], 16)
    size = func_info['size']

    if size == 0:
        return FunctionResult(
            function=func_name, source_dir=source_dir, address=vram_addr,
            target_insns=0, compiled_insns=0, delta=0, match_pct=0.0,
            matching_insns=0, total_insns=0, status="no_info",
            error="size is 0"
        )

    is_game_region = vram_addr < GAME_REGION_END
    target_insns = size // 4

    temp_dir = Path(tempfile.mkdtemp(prefix="regr_"))
    try:
        compiled_obj = temp_dir / f"{func_name}.o"
        success, error = compile_with_ido(source_path, compiled_obj, extra_env)
        if not success:
            return FunctionResult(
                function=func_name, source_dir=source_dir, address=vram_addr,
                target_insns=target_insns, compiled_insns=0, delta=0,
                match_pct=0.0, matching_insns=0, total_insns=0,
                status="compile_error", error=error
            )

        processed_obj = temp_dir / f"{func_name}_pp.o"
        post_process(compiled_obj, processed_obj, is_game_region)

        compiled_bytes = extract_text_section(processed_obj)
        if not compiled_bytes:
            return FunctionResult(
                function=func_name, source_dir=source_dir, address=vram_addr,
                target_insns=target_insns, compiled_insns=0, delta=0,
                match_pct=0.0, matching_insns=0, total_insns=0,
                status="no_text", error="failed to extract .text"
            )

        compiled_insns = len(compiled_bytes) // 4

        # Extract original bytes and disassemble both
        original_bytes = extract_original_bytes(vram_addr, size)
        orig_disasm = disassemble_bytes(original_bytes, vram_addr)
        comp_disasm = disassemble_obj(processed_obj, vram_addr)

        # Check for perfect byte match first
        if original_bytes == compiled_bytes:
            return FunctionResult(
                function=func_name, source_dir=source_dir, address=vram_addr,
                target_insns=target_insns, compiled_insns=compiled_insns,
                delta=compiled_insns - target_insns,
                match_pct=100.0,
                matching_insns=target_insns, total_insns=target_insns,
                status="ok"
            )

        matching, total = compare_instruction_lists(orig_disasm, comp_disasm)
        match_pct = (matching / total * 100) if total > 0 else 0.0

        return FunctionResult(
            function=func_name, source_dir=source_dir, address=vram_addr,
            target_insns=target_insns, compiled_insns=compiled_insns,
            delta=compiled_insns - target_insns,
            match_pct=round(match_pct, 1),
            matching_insns=matching, total_insns=total,
            status="ok"
        )
    except Exception as e:
        return FunctionResult(
            function=func_name, source_dir=source_dir, address=vram_addr,
            target_insns=target_insns, compiled_insns=0, delta=0,
            match_pct=0.0, matching_insns=0, total_insns=0,
            status="compile_error", error=str(e)[:200]
        )
    finally:
        shutil.rmtree(temp_dir, ignore_errors=True)


def gather_source_files(wip_only: bool = False, semi_only: bool = False) -> List[Path]:
    """Collect all .c files from semi-matched and wip directories."""
    files = []
    if not semi_only and WIP_DIR.exists():
        files.extend(sorted(p for p in WIP_DIR.glob("*.c")))
    if not wip_only and SEMI_DIR.exists():
        files.extend(sorted(p for p in SEMI_DIR.glob("*.c")))
    return files


def parse_env_args(env_list: List[str]) -> Dict[str, str]:
    """Parse --env arguments into a dict."""
    env = {}
    for item in (env_list or []):
        if '=' in item:
            key, val = item.split('=', 1)
            env[key] = val
        else:
            env[item] = "1"
    return env


def run_test_pass(source_files: List[Path], config: Dict,
                  extra_env: Dict[str, str], parallel: int,
                  label: str = "") -> List[FunctionResult]:
    """Run a single test pass over all source files."""
    total = len(source_files)
    if label:
        print(f"\n--- {label} ({total} functions) ---")
    else:
        print(f"\nTesting {total} functions...")

    env_str = " ".join(f"{k}={v}" for k, v in extra_env.items()) if extra_env else "(none)"
    print(f"  Env: {env_str}")

    results = []
    completed = 0

    with ThreadPoolExecutor(max_workers=parallel) as executor:
        future_map = {
            executor.submit(test_function, f, config, extra_env): f
            for f in source_files
        }

        for future in as_completed(future_map):
            completed += 1
            try:
                result = future.result(timeout=120)
            except Exception as e:
                src = future_map[future]
                result = FunctionResult(
                    function=extract_func_name(src),
                    source_dir=src.parent.name,
                    address=0, target_insns=0, compiled_insns=0,
                    delta=0, match_pct=0.0, matching_insns=0,
                    total_insns=0, status="compile_error",
                    error=str(e)[:200]
                )
            results.append(result)

            # Progress
            if completed % 10 == 0 or completed == total:
                print(f"\r  [{completed}/{total}]", end="", flush=True)

    print()  # newline after progress
    return results


def write_csv(results: List[FunctionResult], output_path: Path,
              extra_env: Dict[str, str] = None):
    """Write results to CSV."""
    output_path.parent.mkdir(parents=True, exist_ok=True)
    with open(output_path, 'w', newline='') as f:
        writer = csv.writer(f)
        # Header comment with env settings
        env_str = " ".join(f"{k}={v}" for k, v in (extra_env or {}).items())
        writer.writerow([
            "function", "source_dir", "address", "target_insns",
            "compiled_insns", "delta", "match_pct", "matching_insns",
            "total_insns", "status", "error"
        ])
        for r in results:
            writer.writerow([
                r.function, r.source_dir,
                f"0x{r.address:08x}" if r.address else "",
                r.target_insns, r.compiled_insns, r.delta,
                f"{r.match_pct:.1f}", r.matching_insns, r.total_insns,
                r.status, r.error
            ])
    print(f"  CSV written: {output_path}")


def print_summary(results: List[FunctionResult], extra_env: Dict[str, str] = None):
    """Print a console summary of results."""
    ok = [r for r in results if r.status == "ok"]
    errors = [r for r in results if r.status != "ok"]
    perfect = [r for r in ok if r.match_pct == 100.0]
    delta_zero = [r for r in ok if r.delta == 0 and r.match_pct < 100.0]
    high = [r for r in ok if 90.0 <= r.match_pct < 100.0]

    env_str = " ".join(f"{k}={v}" for k, v in (extra_env or {}).items()) or "(baseline)"
    print(f"\n{'=' * 78}")
    print(f"REGRESSION RESULTS  |  Env: {env_str}")
    print(f"{'=' * 78}")
    print(f"  Total tested:       {len(results)}")
    print(f"  Compiled OK:        {len(ok)}")
    print(f"  Compile errors:     {len(errors)}")
    print(f"  Perfect (100%):     {len(perfect)}")
    print(f"  Delta==0 (not 100%): {len(delta_zero)}")
    print(f"  High (90-99%):      {len(high)}")

    if perfect:
        print(f"\n  PERFECT MATCHES:")
        for r in sorted(perfect, key=lambda x: x.function):
            print(f"    {r.function:<40} {r.source_dir}")

    if delta_zero:
        print(f"\n  DELTA==0 (exact insn count, not byte-perfect):")
        for r in sorted(delta_zero, key=lambda x: -x.match_pct):
            print(f"    {r.function:<40} {r.match_pct:5.1f}%  ({r.target_insns} insns)")

    # Top 10 by match percentage (excluding perfect)
    near = sorted([r for r in ok if r.match_pct < 100.0],
                  key=lambda x: -x.match_pct)[:10]
    if near:
        print(f"\n  TOP 10 NEAR-MATCHES:")
        for r in near:
            delta_str = f"{r.delta:+d}" if r.delta != 0 else " 0"
            print(f"    {r.function:<40} {r.match_pct:5.1f}%  "
                  f"target={r.target_insns} compiled={r.compiled_insns} "
                  f"delta={delta_str}")

    if errors[:5]:
        print(f"\n  ERRORS (first 5):")
        for r in errors[:5]:
            print(f"    {r.function:<40} {r.status}: {r.error[:60]}")

    print(f"{'=' * 78}")


def compare_csv_files(baseline_path: str, hook_path: str):
    """Compare two CSV result files and show improvements/regressions."""
    def load_csv(path):
        data = {}
        with open(path, 'r') as f:
            reader = csv.DictReader(f)
            for row in reader:
                data[row['function']] = row
        return data

    baseline = load_csv(baseline_path)
    hook = load_csv(hook_path)

    common = set(baseline.keys()) & set(hook.keys())
    if not common:
        print("No common functions found between the two files.")
        return

    improvements = []
    regressions = []

    for func in sorted(common):
        b_pct = float(baseline[func]['match_pct'])
        h_pct = float(hook[func]['match_pct'])
        diff = h_pct - b_pct

        if diff > 0.05:
            improvements.append((func, b_pct, h_pct, diff))
        elif diff < -0.05:
            regressions.append((func, b_pct, h_pct, diff))

    print(f"\nComparing: {baseline_path} vs {hook_path}")
    print(f"Common functions: {len(common)}")

    if improvements:
        improvements.sort(key=lambda x: -x[3])
        print(f"\nIMPROVEMENTS ({len(improvements)}):")
        for func, b, h, d in improvements:
            print(f"  {func:<40} {b:5.1f}% -> {h:5.1f}%  ({d:+.1f}%)")
    else:
        print("\nNo improvements found.")

    if regressions:
        regressions.sort(key=lambda x: x[3])
        print(f"\nREGRESSIONS ({len(regressions)}):")
        for func, b, h, d in regressions:
            print(f"  {func:<40} {b:5.1f}% -> {h:5.1f}%  ({d:+.1f}%)")
    else:
        print("\nNo regressions found.")


def main():
    parser = argparse.ArgumentParser(
        description="Regression testing pipeline for uopt hooks and PostProc tools"
    )
    parser.add_argument(
        '--env', action='append', default=[],
        help='Environment variable to set (e.g., "IDO52_CSE_MAX_REUSE=5"). Repeatable.'
    )
    parser.add_argument(
        '--sweep-hook',
        help='Sweep a hook over a range: "NAME=MIN:MAX" (e.g., "IDO52_CSE_MAX_REUSE=1:10")'
    )
    parser.add_argument(
        '--wip-only', action='store_true',
        help='Only test WIP files'
    )
    parser.add_argument(
        '--semi-only', action='store_true',
        help='Only test semi-matched files'
    )
    parser.add_argument(
        '--limit', type=int, default=0,
        help='Limit to first N functions (for quick testing)'
    )
    parser.add_argument(
        '-p', '--parallel', type=int, default=4,
        help='Number of parallel workers (default: 4)'
    )
    parser.add_argument(
        '-o', '--output',
        help='Output CSV path (default: build/regression_<timestamp>.csv)'
    )
    parser.add_argument(
        '--compare', nargs=2, metavar=('BASELINE', 'HOOK'),
        help='Compare two CSV result files instead of running tests'
    )
    args = parser.parse_args()

    # Compare mode
    if args.compare:
        compare_csv_files(args.compare[0], args.compare[1])
        return 0

    # Prerequisite checks
    if not IDO_COMPILER.exists():
        print(f"Error: IDO compiler not found at {IDO_COMPILER}", file=sys.stderr)
        print("Run: make setup-ido52", file=sys.stderr)
        return 1

    if not ORIGINAL_EXE.exists():
        print(f"Error: Original executable not found at {ORIGINAL_EXE}", file=sys.stderr)
        print("Run: make extract-exe", file=sys.stderr)
        return 1

    config = load_level3_config()

    source_files = gather_source_files(
        wip_only=args.wip_only,
        semi_only=args.semi_only
    )
    if not source_files:
        print("No source files found.", file=sys.stderr)
        return 1

    if args.limit > 0:
        source_files = source_files[:args.limit]

    print(f"Regression pipeline: {len(source_files)} functions")
    print(f"  Semi-matched: {sum(1 for f in source_files if 'semi-matched' in str(f))}")
    print(f"  WIP:          {sum(1 for f in source_files if '/wip/' in str(f))}")

    # Sweep mode
    if args.sweep_hook:
        match = re.match(r'(\w+)=(\d+):(\d+)', args.sweep_hook)
        if not match:
            print(f"Error: Invalid --sweep-hook format. Use NAME=MIN:MAX", file=sys.stderr)
            return 1

        hook_name = match.group(1)
        val_min = int(match.group(2))
        val_max = int(match.group(3))

        print(f"\nSweeping {hook_name} from {val_min} to {val_max}")

        all_sweep_results = {}
        base_env = parse_env_args(args.env)

        for val in range(val_min, val_max + 1):
            sweep_env = dict(base_env)
            sweep_env[hook_name] = str(val)
            label = f"{hook_name}={val}"

            results = run_test_pass(
                source_files, config, sweep_env, args.parallel, label=label
            )
            # Sort by match_pct descending, then function name
            results.sort(key=lambda r: (-r.match_pct, r.function))
            all_sweep_results[val] = results

            csv_path = BUILD_DIR / f"regression_sweep_{hook_name}_{val}.csv"
            write_csv(results, csv_path, sweep_env)

        # Sweep summary: for each function, show best value
        print(f"\n{'=' * 78}")
        print(f"SWEEP SUMMARY: {hook_name}={val_min}..{val_max}")
        print(f"{'=' * 78}")

        func_names = sorted(set(r.function for r in all_sweep_results[val_min]))
        for func in func_names:
            best_val = val_min
            best_pct = 0.0
            baseline_pct = 0.0
            for val, results in all_sweep_results.items():
                for r in results:
                    if r.function == func and r.status == "ok":
                        if r.match_pct > best_pct:
                            best_pct = r.match_pct
                            best_val = val
                        if val == val_min:
                            baseline_pct = r.match_pct
                        break

            if best_pct > baseline_pct + 0.05:
                print(f"  {func:<40} best={best_pct:.1f}% at {hook_name}={best_val} "
                      f"(baseline={baseline_pct:.1f}%)")

        return 0

    # Single run mode
    extra_env = parse_env_args(args.env)
    results = run_test_pass(
        source_files, config, extra_env, args.parallel
    )

    # Sort by match_pct descending, then delta (prefer 0), then function name
    results.sort(key=lambda r: (-r.match_pct, abs(r.delta), r.function))

    # Output CSV
    if args.output:
        csv_path = Path(args.output)
    else:
        env_tag = "_".join(f"{k}{v}" for k, v in extra_env.items()) if extra_env else "baseline"
        env_tag = re.sub(r'[^a-zA-Z0-9_]', '', env_tag)[:40]
        timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
        csv_path = BUILD_DIR / f"regression_{env_tag}_{timestamp}.csv"

    write_csv(results, csv_path, extra_env)
    print_summary(results, extra_env)

    return 0


if __name__ == "__main__":
    sys.exit(main())
