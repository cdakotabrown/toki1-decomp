#!/usr/bin/env python3
"""Audit count-mismatch functions to find actual status."""

import subprocess
import re
import sys
from pathlib import Path

def find_overlay(func_name):
    """Find which overlay contains this function."""
    result = subprocess.run(
        ['find', 'src/overlay', '-name', f'{func_name}.c'],
        capture_output=True, text=True
    )
    if result.stdout.strip():
        path = Path(result.stdout.strip().split('\n')[0])
        return path.parent.name
    return None

def test_function(overlay, func_name):
    """Test a single function and return its actual status."""
    result = subprocess.run(
        ['make', 'test-overlay-match', f'OVERLAY={overlay}', f'FUNC={func_name}'],
        capture_output=True, text=True, timeout=60
    )
    output = result.stdout + result.stderr

    # Parse results
    target_match = re.search(r'Target:\s+(\d+)', output)
    compiled_match = re.search(r'Compiled:\s+(\d+)', output)
    masked_match = re.search(r'Masked:\s+(\d+)/(\d+)', output)

    if not target_match or not compiled_match:
        return None, None, None, "PARSE_ERROR"

    target = int(target_match.group(1))
    compiled = int(compiled_match.group(1))

    if target != compiled:
        return target, compiled, 0, "COUNT_MISMATCH"

    if masked_match:
        masked = int(masked_match.group(1))
        total = int(masked_match.group(2))
        if masked == total:
            return target, compiled, 100, "RELOC_ONLY"
        else:
            pct = (masked * 100) // total
            return target, compiled, pct, "SEMI_MATCHED"

    return target, compiled, 0, "UNKNOWN"

def main():
    mismatch_file = '/tmp/mismatch_list.txt'

    with open(mismatch_file) as f:
        lines = f.readlines()

    results = {
        'RELOC_ONLY': [],
        'SEMI_MATCHED': [],
        'COUNT_MISMATCH': [],
        'PARSE_ERROR': [],
        'NOT_FOUND': []
    }

    for i, line in enumerate(lines):
        match = re.search(r'(func_[0-9A-Fa-f]+)', line)
        if not match:
            continue

        func_name = match.group(1)
        overlay = find_overlay(func_name)

        if not overlay:
            results['NOT_FOUND'].append(func_name)
            print(f"[{i+1}/{len(lines)}] {func_name}: NOT_FOUND")
            continue

        try:
            target, compiled, masked_pct, status = test_function(overlay, func_name)
            results[status].append((func_name, overlay, target, compiled, masked_pct))

            if status == 'RELOC_ONLY':
                print(f"[{i+1}/{len(lines)}] {overlay}/{func_name}: RELOC_ONLY (100% masked)")
            elif status == 'SEMI_MATCHED':
                print(f"[{i+1}/{len(lines)}] {overlay}/{func_name}: SEMI_MATCHED ({target}/{compiled}, {masked_pct}% masked)")
            else:
                print(f"[{i+1}/{len(lines)}] {overlay}/{func_name}: {status} ({target} vs {compiled})")
        except subprocess.TimeoutExpired:
            results['PARSE_ERROR'].append(func_name)
            print(f"[{i+1}/{len(lines)}] {func_name}: TIMEOUT")
        except Exception as e:
            results['PARSE_ERROR'].append(func_name)
            print(f"[{i+1}/{len(lines)}] {func_name}: ERROR {e}")

    print("\n" + "="*60)
    print("AUDIT SUMMARY")
    print("="*60)
    print(f"RELOC_ONLY (actually matched):  {len(results['RELOC_ONLY'])}")
    print(f"SEMI_MATCHED (count matched):   {len(results['SEMI_MATCHED'])}")
    print(f"COUNT_MISMATCH (truly broken):  {len(results['COUNT_MISMATCH'])}")
    print(f"NOT_FOUND:                      {len(results['NOT_FOUND'])}")
    print(f"PARSE_ERROR:                    {len(results['PARSE_ERROR'])}")

    if results['RELOC_ONLY']:
        print("\n--- Functions that are actually RELOC_ONLY ---")
        for func, overlay, t, c, m in results['RELOC_ONLY']:
            print(f"  {overlay}/{func}")

    if results['SEMI_MATCHED']:
        print("\n--- Functions that are SEMI_MATCHED (count OK) ---")
        for func, overlay, t, c, m in results['SEMI_MATCHED']:
            print(f"  {overlay}/{func}: {t} insns, {m}% masked")

if __name__ == '__main__':
    main()
