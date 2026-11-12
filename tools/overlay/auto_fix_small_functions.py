#!/usr/bin/env python3
"""auto_fix_small_functions.py — Batch-fix small functions by rewriting C from ASM.

For functions with ≤20 instructions, reads the target ASM, classifies the
pattern (leaf init, accessor, switch dispatch, etc.), and generates C source
that matches the target's code generation pattern.

Strategy:
1. Find small semi-matched functions (≤20 insns, 20-60% exact)
2. Read the target ASM for each
3. Classify the pattern
4. Generate C source that produces the target instruction sequence
5. Run auto_infer_postproc to add PostProc directives
6. Keep if improvement >= 5%

Common patterns for small functions:
- Leaf init: Write constants to globals (CSE $at pattern)
- Accessor: Read a global and return it
- Indexed load: Read global[A] and return (like GEKO::func_80134590)
- Conditional set: if (cond) global = value
- Simple math: return a + b, a * b, etc.
- Function stub: Just calls another function

Usage:
    python3 auto_fix_small_functions.py BUNKAKEN --apply --max-funcs 10
    python3 auto_fix_small_functions.py --batch-all --apply --max-funcs 50
"""

import argparse
import json
import os
import re
import subprocess
import sys
from pathlib import Path
from typing import Dict, List, Optional, Tuple

ROOT = Path(__file__).parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

AUTO_INFER = ROOT / "tools" / "overlay" / "auto_infer_postproc.py"
TEST_TOOL = ROOT / "tools" / "overlay" / "test_overlay_match.py"
SRC_DIR = ROOT / "src" / "overlay"
ASM_DIR = ROOT / "asm" / "overlay"
STATUS_FILE = ROOT / "config" / "overlay_match_status.json"


def get_match_pct(overlay: str, func_name: str) -> Optional[float]:
    result = subprocess.run(
        ["python3", str(TEST_TOOL), overlay, func_name, "--fix-relocs"],
        capture_output=True, text=True, timeout=30
    )
    m = re.search(r'(\d+\.\d+)%', result.stdout)
    return float(m.group(1)) if m else None


def read_asm(overlay: str, func_name: str) -> Optional[str]:
    path = ASM_DIR / overlay / f"{func_name}.s"
    if path.exists():
        return path.read_text()
    return None


def classify_asm(asm: str) -> str:
    """Classify the function pattern from its ASM."""
    lines = asm.split('\n')
    insns = [l.strip() for l in lines if l.strip() and not l.strip().startswith(('.', '/*', '.set', '.text', '.globl'))]
    
    has_jal = any('jal' in i for i in insns)
    has_jr_ra = any('jr' in i for i in insns)
    has_addiu_sp = any('addiu' in i and '$sp' in i for i in insns)
    has_sw = any('sw' in i for i in insns)
    has_lw = any('lw' in i for i in insns)
    has_lbu = any('lbu' in i for i in insns)
    has_ori = any('ori' in i for i in insns)
    has_slti = any('slti' in i for i in insns)
    has_bne = any('bne' in i for i in insns)
    has_beq = any('beq' in i for i in insns)
    
    # Leaf: no stack frame, no jal, just jr $ra
    if not has_addiu_sp and not has_jal and has_jr_ra:
        if has_sw and has_ori:
            return "leaf_init"  # Writes constants to globals
        elif has_lbu and not has_sw:
            return "leaf_accessor"  # Reads and returns
        elif has_sw and not has_ori:
            return "leaf_store"  # Simple stores
        else:
            return "leaf_other"
    
    # Has function call
    if has_jal:
        return "caller"
    
    # Has stack frame
    if has_addiu_sp:
        if has_bne or has_beq:
            return "branch_func"
        return "frame_func"
    
    return "unknown"


def parse_leaf_init(asm: str) -> Optional[str]:
    """Parse a leaf init function and generate C source.
    
    Pattern: Construct values with lui+ori, store with CSE'd lui $at.
    """
    # Extract all sw instructions and the values being stored
    lines = asm.split('\n')
    
    # Find lui+ori pairs (value construction) and lui+sw pairs (stores)
    lui_values = {}  # reg -> hi_value
    ori_values = {}  # reg -> lo_value
    stores = []  # (value_reg, offset, base_reg)
    store_bases = {}  # base_reg -> hi_value
    
    for line in lines:
        line = line.strip()
        if line.startswith('.') or line.startswith('/*') or not line:
            continue
        
        # Parse lui $reg, value
        m = re.match(r'lui\s+\$(\w+),\s*(0x[0-9a-fA-F]+|\d+)', line)
        if m:
            reg = m.group(1)
            val = int(m.group(2), 16) if m.group(2).startswith('0x') else int(m.group(2))
            if reg == 'at':
                store_bases[reg] = val
            elif reg.startswith('t'):
                lui_values[reg] = val
            continue
        
        # Parse ori $reg, $reg, value
        m = re.match(r'ori\s+\$(\w+),\s*\$(\w+),\s*(0x[0-9a-fA-F]+|\d+)', line)
        if m:
            reg = m.group(1)
            val = int(m.group(3), 16) if m.group(3).startswith('0x') else int(m.group(3))
            ori_values[reg] = val
            continue
        
        # Parse sw $value_reg, offset($base_reg)
        m = re.match(r'sw\s+\$(\w+),\s*([-\d]+|0x[0-9a-fA-F]+)\(\$(\w+)\)', line)
        if m:
            val_reg = m.group(1)
            offset = int(m.group(2), 16) if m.group(2).startswith('0x') else int(m.group(2))
            base_reg = m.group(3)
            stores.append((val_reg, offset, base_reg))
            continue
    
    if not stores:
        return None
    
    # Reconstruct the full values and addresses
    assigns = []
    for val_reg, offset, base_reg in stores:
        # Get the full 32-bit value
        hi = lui_values.get(val_reg, 0)
        lo = ori_values.get(val_reg, 0)
        full_value = (hi << 16) | lo
        
        # Get the full 32-bit address
        base_hi = store_bases.get(base_reg, lui_values.get(base_reg, 0))
        full_addr = (base_hi << 16) + (offset & 0xFFFF)
        if offset < 0:
            full_addr = (base_hi << 16) + offset
        
        assigns.append((full_addr, full_value))
    
    # Generate C source
    c_lines = []
    c_lines.append('/* @SymbolSize: 52 */')
    c_lines.append('/* @PostProc: scheduler_normalizer */')
    c_lines.append('#include "types.h"')
    c_lines.append('')
    
    # Declare globals
    for addr, val in assigns:
        c_lines.append(f'#define ADDR_0x{addr:08X} (*(volatile s32 *)0x{addr:08X}U)')
    c_lines.append('')
    c_lines.append('void func_NAME(void) {')
    for addr, val in assigns:
        c_lines.append(f'    ADDR_0x{addr:08X} = 0x{val:08X};')
    c_lines.append('}')
    
    return '\n'.join(c_lines)


def fix_function(overlay: str, func_name: str, apply: bool = False, verbose: bool = False) -> Dict:
    """Try to fix a single small function."""
    baseline_pct = get_match_pct(overlay, func_name)
    if baseline_pct is None:
        return {"func": func_name, "error": "can't compile"}
    
    if baseline_pct >= 99:
        return {"func": func_name, "result": "already matched", "pct": baseline_pct}
    
    asm = read_asm(overlay, func_name)
    if asm is None:
        return {"func": func_name, "error": "no ASM"}
    
    pattern = classify_asm(asm)
    if verbose:
        print(f"  Pattern: {pattern}")
    
    if pattern == "leaf_init":
        new_c = parse_leaf_init(asm)
        if new_c is None:
            return {"func": func_name, "error": "couldn't parse ASM", "pattern": pattern}
        
        # Replace function name
        new_c = new_c.replace('func_NAME', func_name)
        
        # Calculate correct symbol size
        src_path = SRC_DIR / overlay / f"{func_name}.c"
        if src_path.exists():
            with open(src_path) as f:
                old_c = f.read()
            m = re.search(r'@SymbolSize:\s*(\d+)', old_c)
            if m:
                new_c = new_c.replace('/* @SymbolSize: 52 */', f'/* @SymbolSize: {m.group(1)} */')
        
        if apply:
            with open(src_path, 'w') as f:
                f.write(new_c)
            
            # Run auto_infer
            subprocess.run(
                ["python3", str(AUTO_INFER), overlay, func_name, "--apply"],
                capture_output=True, timeout=60
            )
            
            new_pct = get_match_pct(overlay, func_name)
            if new_pct and new_pct > baseline_pct:
                return {"func": func_name, "result": "improved", "before": baseline_pct, "after": new_pct}
            else:
                # Revert
                with open(src_path, 'w') as f:
                    f.write(old_c)
                return {"func": func_name, "result": "no improvement", "pct": baseline_pct}
        else:
            return {"func": func_name, "result": "would rewrite", "pattern": pattern}
    
    return {"func": func_name, "result": f"unsupported pattern: {pattern}"}


def main():
    parser = argparse.ArgumentParser(description="Auto-fix small functions by rewriting C from ASM")
    parser.add_argument("overlay", help="Overlay name (or 'all' for --batch-all)")
    parser.add_argument("--apply", action="store_true", help="Apply changes (default: dry-run)")
    parser.add_argument("--max-funcs", type=int, default=10, help="Max functions to process")
    parser.add_argument("--min-pct", type=float, default=10, help="Minimum baseline %")
    parser.add_argument("--max-pct", type=float, default=70, help="Maximum baseline %")
    parser.add_argument("-v", "--verbose", action="store_true")
    args = parser.parse_args()
    
    d = json.load(open(STATUS_FILE))
    
    # Find target functions
    targets = []
    for ovl, funcs in d.items():
        if args.overlay != "all" and ovl != args.overlay:
            continue
        for fname, info in funcs.items():
            if info.get('status') not in ('semi_matched', 'semi'):
                continue
            pct = info.get('exact_pct', 0)
            if not isinstance(pct, (int, float)):
                continue
            if pct < args.min_pct or pct > args.max_pct:
                continue
            
            src_path = SRC_DIR / ovl / f"{fname}.c"
            if not src_path.exists():
                continue
            
            with open(src_path) as f:
                content = f.read()
            m = re.search(r'@SymbolSize:\s*(\d+)', content)
            if m:
                size = int(m.group(1))
                insns = size // 4
                if insns <= 20 and insns >= 3:
                    targets.append((ovl, fname, float(pct), insns))
    
    targets.sort(key=lambda x: -x[2])
    if args.max_funcs:
        targets = targets[:args.max_funcs]
    
    print(f"Processing {len(targets)} small functions")
    
    improved = 0
    for ovl, fn, pct, insns in targets:
        print(f"\n{ovl}::{fn}: {pct:.1f}% ({insns} insns)")
        result = fix_function(ovl, fn, args.apply, args.verbose)
        r = result.get('result', 'unknown')
        if 'improved' in r:
            improved += 1
            print(f"  ✓ {result['before']:.1f}% → {result['after']:.1f}%")
        elif 'error' in result:
            print(f"  ✗ {result.get('error', 'unknown error')}")
        elif 'unsupported' in r:
            print(f"  ⊘ {r}")
        else:
            print(f"  - {r}")
    
    print(f"\nResults: {improved} improved out of {len(targets)}")


if __name__ == "__main__":
    main()
