#!/usr/bin/env python3
"""
Post-Link Rewriter Pipeline - Per tokimeki_docs/04_post_link_rewriters.md

Chains all rewrite passes in the correct order:
1. Frame size normalization
2. Register renaming (allocation matching)
3. Delay-slot filling normalization
4. Move-encoding normalization (or <-> addu)

Usage:
    python3 rewriter_pipeline.py input.o output.o \
        --target-exe disks/SLPS_000.65 \
        --address 0x80041234 \
        --region gcc|ido
"""

import argparse
import json
import hashlib
import shutil
import subprocess
import sys
from pathlib import Path
from dataclasses import dataclass, asdict
from typing import List, Dict, Optional

@dataclass
class RewritePass:
    name: str
    applied: bool
    changes: List[Dict]
    before_hash: str
    after_hash: str

@dataclass
class PipelineResult:
    input_file: str
    output_file: str
    address: str
    region: str
    passes: List[RewritePass]
    final_match: bool
    match_percentage: float

def file_hash(filepath: str) -> str:
    """Compute SHA256 of file."""
    h = hashlib.sha256()
    with open(filepath, 'rb') as f:
        h.update(f.read())
    return h.hexdigest()[:16]

def run_pass(pass_name: str, input_file: str, output_file: str,
             target_exe: str, address: str, region: str, verbose: bool) -> RewritePass:
    """Run a single rewrite pass."""
    before_hash = file_hash(input_file)
    changes = []
    applied = False

    tools_dir = Path(__file__).parent.parent

    if pass_name == "frame_size":
        # Frame size normalization - GCC uses 24, target uses 40+
        script = tools_dir / "ido" / "patch_stack_complete.py"
        if script.exists():
            # Try common frame size adjustments
            for old_size, new_size in [(24, 40), (24, 48), (24, 56), (32, 40)]:
                try:
                    result = subprocess.run([
                        sys.executable, str(script),
                        input_file, output_file,
                        "--old", str(old_size),
                        "--new", str(new_size)
                    ], capture_output=True, text=True)
                    if result.returncode == 0 and "patched" in result.stdout.lower():
                        applied = True
                        changes.append({
                            "type": "frame_resize",
                            "old_size": old_size,
                            "new_size": new_size
                        })
                        break
                except Exception as e:
                    pass
        if not applied:
            shutil.copy(input_file, output_file)

    elif pass_name == "register_rename":
        # Register renaming pass
        script = tools_dir / "ido" / "swap_registers_auto.py"
        if script.exists():
            try:
                result = subprocess.run([
                    sys.executable, str(script),
                    input_file, output_file,
                    "--target-exe", target_exe,
                    "--address", address,
                    "--unidirectional"
                ], capture_output=True, text=True)
                if result.returncode == 0:
                    applied = True
                    # Parse changes from output if available
                    if "swapped" in result.stdout.lower():
                        changes.append({"type": "register_swap", "details": result.stdout[:200]})
            except Exception as e:
                pass
        if not applied:
            shutil.copy(input_file, output_file)

    elif pass_name == "delay_slot":
        # Delay slot rewriting
        script = tools_dir / "postproc" / "delay_slot_rewriter.py"
        if script.exists():
            try:
                result = subprocess.run([
                    sys.executable, str(script),
                    input_file, output_file,
                    "--target-exe", target_exe,
                    "--address", address
                ], capture_output=True, text=True)
                if result.returncode == 0:
                    applied = True
            except Exception as e:
                pass
        if not applied:
            shutil.copy(input_file, output_file)

    elif pass_name == "move_encoding":
        # Move encoding normalization (or <-> addu)
        if region == "ido":
            # IDO region needs or->addu conversion
            script = tools_dir / "ido" / "convert_or_to_addu.py"
            if script.exists():
                try:
                    result = subprocess.run([
                        sys.executable, str(script),
                        input_file, output_file
                    ], capture_output=True, text=True)
                    if result.returncode == 0:
                        applied = True
                        changes.append({"type": "move_encoding", "conversion": "or_to_addu"})
                except Exception as e:
                    pass
        # GCC region uses 'or' natively, no conversion needed
        if not applied:
            shutil.copy(input_file, output_file)
    else:
        shutil.copy(input_file, output_file)

    after_hash = file_hash(output_file)

    return RewritePass(
        name=pass_name,
        applied=applied or (before_hash != after_hash),
        changes=changes,
        before_hash=before_hash,
        after_hash=after_hash
    )

def compare_with_target(obj_file: str, target_exe: str, address: str) -> tuple:
    """Compare compiled object with target executable."""
    tools_dir = Path(__file__).parent.parent
    script = tools_dir / "decomp" / "quick_diff.py"

    if not script.exists():
        return False, 0.0

    try:
        result = subprocess.run([
            sys.executable, str(script),
            obj_file,
            "--address", address,
            "--exe", target_exe
        ], capture_output=True, text=True)

        output = result.stdout + result.stderr

        # Parse match percentage from output
        if "100%" in output or "exact match" in output.lower():
            return True, 100.0

        # Try to extract percentage
        import re
        match = re.search(r'(\d+(?:\.\d+)?)\s*%', output)
        if match:
            return False, float(match.group(1))

        return False, 0.0
    except Exception as e:
        return False, 0.0

def run_pipeline(input_file: str, output_file: str, target_exe: str,
                 address: str, region: str, verbose: bool = False) -> PipelineResult:
    """Run the complete rewriter pipeline."""

    # Pass order per tokimeki_docs/04_post_link_rewriters.md
    pass_order = [
        "frame_size",      # Pass 1: Frame size normalization
        "register_rename", # Pass 2: Register renaming
        "delay_slot",      # Pass 3: Delay-slot filling
        "move_encoding",   # Pass 4: Move encoding normalization
    ]

    passes = []
    current_input = input_file

    # Create temp directory for intermediate files
    temp_dir = Path("/tmp/rewriter_pipeline")
    temp_dir.mkdir(exist_ok=True)

    for i, pass_name in enumerate(pass_order):
        if i == len(pass_order) - 1:
            current_output = output_file
        else:
            current_output = str(temp_dir / f"pass_{i}_{pass_name}.o")

        if verbose:
            print(f"Running pass {i+1}/{len(pass_order)}: {pass_name}")

        result = run_pass(
            pass_name, current_input, current_output,
            target_exe, address, region, verbose
        )
        passes.append(result)

        if verbose and result.applied:
            print(f"  Applied: {result.changes}")

        current_input = current_output

    # Compare final output with target
    final_match, match_pct = compare_with_target(output_file, target_exe, address)

    return PipelineResult(
        input_file=input_file,
        output_file=output_file,
        address=address,
        region=region,
        passes=passes,
        final_match=final_match,
        match_percentage=match_pct
    )

def main():
    parser = argparse.ArgumentParser(description='Post-link rewriter pipeline')
    parser.add_argument('input', help='Input object file')
    parser.add_argument('output', help='Output object file')
    parser.add_argument('--target-exe', required=True, help='Target executable')
    parser.add_argument('--address', required=True, help='Function address (hex)')
    parser.add_argument('--region', choices=['gcc', 'ido'], required=True,
                        help='Code region (gcc or ido)')
    parser.add_argument('--verbose', '-v', action='store_true')
    parser.add_argument('--json-report', help='Output JSON report file')

    args = parser.parse_args()

    result = run_pipeline(
        args.input, args.output,
        args.target_exe, args.address,
        args.region, args.verbose
    )

    # Print summary
    print(f"\n{'='*60}")
    print(f"Rewriter Pipeline Result: {args.input}")
    print(f"{'='*60}")
    print(f"Address: {args.address}")
    print(f"Region: {args.region}")
    print(f"\nPasses:")
    for p in result.passes:
        status = "✓ Applied" if p.applied else "○ No change"
        print(f"  {p.name}: {status}")
        if p.changes:
            for c in p.changes:
                print(f"    {c}")

    print(f"\nFinal match: {result.match_percentage:.1f}%")
    if result.final_match:
        print("✓ BYTE-PERFECT MATCH!")

    # Write JSON report if requested
    if args.json_report:
        report = {
            'input': result.input_file,
            'output': result.output_file,
            'address': result.address,
            'region': result.region,
            'passes': [asdict(p) for p in result.passes],
            'final_match': result.final_match,
            'match_percentage': result.match_percentage
        }
        with open(args.json_report, 'w') as f:
            json.dump(report, f, indent=2)
        print(f"\nReport written to: {args.json_report}")

    return 0 if result.final_match else 1

if __name__ == '__main__':
    sys.exit(main())
