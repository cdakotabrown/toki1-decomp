#!/usr/bin/env python3
"""
Audit all ASM files in asm/main/matched/ and classify conversion difficulty.
Outputs JSON with per-function analysis.
"""

import os
import re
import json
from pathlib import Path

ASM_DIR = Path("asm/main/matched")
OUTPUT_FILE = Path("build/asm_audit_report.json")

def analyze_function(filepath):
    """Analyze a single ASM file and return classification."""
    with open(filepath, 'r') as f:
        content = f.read()
        lines = content.split('\n')

    filename = os.path.basename(filepath)
    addr_match = re.search(r'func_([0-9a-fA-F]+)', filename)
    address = f"0x{addr_match.group(1)}" if addr_match else "unknown"

    # Determine region
    addr_int = int(addr_match.group(1), 16) if addr_match else 0
    if 0x80041000 <= addr_int <= 0x80085FFF:
        region = "GCC"
    elif 0x80086000 <= addr_int <= 0x800BFFFF:
        region = "IDO"
    else:
        region = "unknown"

    # Count lines (excluding empty and comments)
    code_lines = len([l for l in lines if l.strip() and not l.strip().startswith('#')])

    # Initialize classification
    issues = []
    convertible = True

    # Check for thunk pattern (code before prologue)
    has_thunk = bool(re.search(r'[Tt]hunk|pre-prologue', content))
    if has_thunk:
        issues.append("thunk_pre_prologue")
        convertible = False

    # Find prologue position
    prologue_idx = None
    for i, line in enumerate(lines):
        if re.search(r'addiu\s+\$?sp\s*,\s*\$?sp\s*,\s*-', line):
            prologue_idx = i
            break

    # Check for code before prologue (more precise check)
    if prologue_idx:
        for i, line in enumerate(lines[:prologue_idx]):
            # Skip directives, labels, comments
            stripped = line.strip()
            if stripped and not stripped.startswith('.') and not stripped.startswith('#'):
                if not stripped.endswith(':') and not 'globl' in stripped.lower():
                    # This is an instruction before prologue
                    if re.search(r'\b(sw|sb|sh|lw|lb|lh|lui|addiu|sll|or|move|bnez|beqz|j\s)', stripped):
                        if "thunk_pre_prologue" not in issues:
                            issues.append("code_before_prologue")
                            convertible = False
                        break

    # Check for hidden register patterns
    # Look for t6, t7, t8 usage in first few instructions after prologue
    if prologue_idx:
        first_instrs = '\n'.join(lines[prologue_idx:prologue_idx+10])

        # t6 used before being set - check first few instructions
        t6_written = False
        t6_read_before_write = False
        for line in lines[prologue_idx:prologue_idx+10]:
            if '$t6' in line or 't6' in line:
                # Check if t6 is destination (write)
                if re.search(r'^\s*(li|lui|lw|lbu|lhu|lb|lh|move|addiu|sll|srl|or|and)\s+\$?t6\s*,', line):
                    t6_written = True
                    break
                # Check if t6 is source (read)
                elif re.search(r',\s*\$?t6|bnez\s+\$?t6|beqz\s+\$?t6|addu.*\$?t6', line):
                    if not t6_written:
                        t6_read_before_write = True
                        break
        if t6_read_before_write:
            issues.append("hidden_t6")
            convertible = False

        # v1 used as dispatcher
        if re.search(r'bnez\s+\$v1|beqz\s+\$v1|beq\s+\$v1|bne\s+\$v1', first_instrs):
            if not re.search(r'move\s+\$v1|li\s+\$v1|lw\s+\$v1', first_instrs[:first_instrs.find('$v1')]):
                issues.append("hidden_v1_dispatcher")
                convertible = False

    # Check for hand-written opcodes
    if '.word' in content:
        issues.append("hand_written_opcodes")
        convertible = False

    # Check for indirect calls (jalr)
    if re.search(r'jalr\s+\$t', content):
        issues.append("indirect_jalr")
        # Not necessarily unconvertible

    # Check for jump tables
    if re.search(r'jr\s+\$t[0-9]', content):
        issues.append("jump_table")
        convertible = False

    # Check for leaf function (no prologue)
    if not prologue_idx:
        if re.search(r'jr\s+\$ra', content):
            issues.append("leaf_no_stack")
            # May be convertible with right flags

    # Determine final status
    if not issues:
        status = "potentially_convertible"
    elif convertible:
        status = "may_need_postprocessing"
    else:
        status = "requires_asm"

    return {
        "filename": filename,
        "address": address,
        "region": region,
        "lines": code_lines,
        "status": status,
        "issues": issues,
        "convertible": convertible
    }

def main():
    print(f"Auditing ASM files in {ASM_DIR}...")

    results = []
    files = sorted(ASM_DIR.glob("func_*.s"))

    for i, filepath in enumerate(files):
        if i % 100 == 0:
            print(f"  Processed {i}/{len(files)}...")
        try:
            result = analyze_function(filepath)
            results.append(result)
        except Exception as e:
            results.append({
                "filename": filepath.name,
                "address": "error",
                "status": "error",
                "error": str(e)
            })

    # Generate summary
    summary = {
        "total": len(results),
        "by_status": {},
        "by_issue": {},
        "by_region": {}
    }

    for r in results:
        status = r.get("status", "unknown")
        summary["by_status"][status] = summary["by_status"].get(status, 0) + 1

        region = r.get("region", "unknown")
        summary["by_region"][region] = summary["by_region"].get(region, 0) + 1

        for issue in r.get("issues", []):
            summary["by_issue"][issue] = summary["by_issue"].get(issue, 0) + 1

    output = {
        "summary": summary,
        "functions": results
    }

    # Ensure output directory exists
    OUTPUT_FILE.parent.mkdir(parents=True, exist_ok=True)

    with open(OUTPUT_FILE, 'w') as f:
        json.dump(output, f, indent=2)

    print(f"\nAudit complete!")
    print(f"Results saved to: {OUTPUT_FILE}")
    print(f"\nSummary:")
    print(f"  Total files: {summary['total']}")
    print(f"\n  By status:")
    for status, count in sorted(summary["by_status"].items()):
        print(f"    {status}: {count}")
    print(f"\n  By issue:")
    for issue, count in sorted(summary["by_issue"].items(), key=lambda x: -x[1]):
        print(f"    {issue}: {count}")

if __name__ == "__main__":
    main()
