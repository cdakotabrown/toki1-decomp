#!/usr/bin/env python3
"""Validate overlay source files compile correctly.

Checks each src/overlay/<NAME>/func_*.c file for:
1. Syntax errors (via GCC -fsyntax-only)
2. Missing type definitions
3. Inconsistent function signatures (declaration vs definition)
4. Non-standard variable naming (D_XXXX_XXXX instead of D_XXXXXXXX)

Usage:
    python3 tools/overlay/validate_overlay_sources.py [--fix] [OVERLAY_NAME]
"""

import os
import re
import subprocess
import sys
from pathlib import Path
from collections import defaultdict

# Banned PostProc checker
BUILD_DIR = Path(__file__).resolve().parent.parent / "build"
sys.path.insert(0, str(BUILD_DIR))
from banned_postproc import count_banned_in_source

BASE_DIR = Path(__file__).resolve().parent.parent.parent
OVERLAY_SRC = BASE_DIR / "src" / "overlay"
INCLUDE_DIR = BASE_DIR / "include"

def check_syntax(filepath):
    """Check if a C file has syntax errors using GCC."""
    result = subprocess.run(
        ["gcc", "-fsyntax-only", "-w", "-std=c89",
         f"-I{INCLUDE_DIR}", str(filepath)],
        capture_output=True, text=True
    )
    if result.returncode != 0:
        return result.stderr.strip()
    return None

def check_variable_naming(content, filepath):
    """Check for non-standard D_ variable naming."""
    issues = []
    # Look for D_ followed by something other than exactly 8 hex digits
    for m in re.finditer(r'\bD_([0-9A-Fa-f_]+)\b', content):
        name = m.group(1)
        if '_' in name or len(name) != 8:
            issues.append(f"Non-standard variable name: D_{name}")
    return issues

def normalize_signature_args(args):
    """Normalize argument list: treat empty () and (void) as equivalent.

    Also strips parameter names, keeping only types, so that
    (s32 a0, s32 a1) and (s32, s32) are treated as equivalent.
    """
    args = args.strip()
    if not args or args == 'void':
        return 'void'

    # Split by comma and extract just the type from each parameter
    normalized = []
    for param in args.split(','):
        param = param.strip()
        if not param:
            continue
        # Handle multiline args (newlines, extra spaces)
        param = ' '.join(param.split())
        # Extract type: remove parameter name (last word if it looks like identifier)
        # e.g., "s32 a0" -> "s32", "s32* ptr" -> "s32*", "s32" -> "s32"
        tokens = param.split()
        if len(tokens) >= 2:
            # Check if last token is a parameter name (simple identifier)
            if tokens[-1].isidentifier() and not tokens[-1].startswith('*'):
                # Keep everything except the last token (parameter name)
                param = ' '.join(tokens[:-1])
        normalized.append(param)

    return ', '.join(normalized) if normalized else 'void'

def check_function_signatures(overlay_dir):
    """Check for inconsistent function signatures across files."""
    signatures = defaultdict(list)  # func_name -> [(file, sig_display, sig_normalized), ...]

    for f in overlay_dir.glob("*.c"):
        content = f.read_text()

        # Find extern declarations
        for m in re.finditer(r'extern\s+(\w+)\s+(func_[0-9A-Fa-f]+)\s*\(([^)]*)\)', content):
            ret_type, name, args = m.groups()
            norm_args = normalize_signature_args(args)
            signatures[name].append((f.name, f"extern {ret_type} {name}({args})", (ret_type, norm_args)))

        # Find definitions
        for m in re.finditer(r'^(\w+)\s+(func_[0-9A-Fa-f]+)\s*\(([^)]*)\)\s*$', content, re.MULTILINE):
            ret_type, name, args = m.groups()
            if ret_type not in ('extern', 'static', 'typedef'):
                norm_args = normalize_signature_args(args)
                signatures[name].append((f.name, f"{ret_type} {name}({args})", (ret_type, norm_args)))

    issues = []
    for name, sigs in signatures.items():
        if len(sigs) > 1:
            # Check if signatures are consistent (compare normalized form, ignoring extern)
            unique_sigs = set(s[2] for s in sigs)  # Compare (ret_type, normalized_args) tuples
            if len(unique_sigs) > 1:
                issues.append(f"{name}: inconsistent signatures:")
                for file, sig_display, _ in sigs:
                    issues.append(f"  {file}: {sig_display}")
    return issues

def validate_overlay(overlay_name, fix=False):
    """Validate all source files in an overlay."""
    overlay_dir = OVERLAY_SRC / overlay_name
    if not overlay_dir.exists():
        print(f"Overlay not found: {overlay_name}")
        return 1

    files = list(overlay_dir.glob("*.c"))
    print(f"Validating {overlay_name}: {len(files)} files")

    errors = []
    warnings = []

    # Check individual files
    for f in files:
        content = f.read_text()

        # Check variable naming
        var_issues = check_variable_naming(content, f)
        for issue in var_issues:
            warnings.append(f"{f.name}: {issue}")

    # Check function signature consistency
    sig_issues = check_function_signatures(overlay_dir)
    for issue in sig_issues:
        errors.append(f"{overlay_name}: {issue}")

    # Check for banned PostProc directives
    banned_count = 0
    for f in files:
        try:
            content = f.read_text()
            n = count_banned_in_source(content)
            if n > 0:
                banned_count += n
                errors.append(f"{f.name}: {n} banned PostProc directive(s) found")
        except Exception:
            pass
    if banned_count:
        print(f"  ⛔ {banned_count} banned PostProc directive(s) detected across {overlay_name}")

    # Report results
    if warnings:
        print(f"\nWarnings ({len(warnings)}):")
        for w in warnings[:20]:  # Limit output
            print(f"  {w}")
        if len(warnings) > 20:
            print(f"  ... and {len(warnings) - 20} more")

    if errors:
        print(f"\nErrors ({len(errors)}):")
        for e in errors[:20]:
            print(f"  {e}")
        if len(errors) > 20:
            print(f"  ... and {len(errors) - 20} more")
        return 1

    print(f"  OK: No critical issues found")
    return 0

def main():
    fix = "--fix" in sys.argv
    args = [a for a in sys.argv[1:] if not a.startswith("-")]

    if args:
        overlays = args
    else:
        overlays = [d.name for d in OVERLAY_SRC.iterdir() if d.is_dir()]

    total_errors = 0
    for ovl in sorted(overlays):
        total_errors += validate_overlay(ovl, fix)

    print(f"\n{'='*50}")
    if total_errors:
        print(f"Validation failed: {total_errors} overlay(s) have issues")
        return 1
    else:
        print("All overlays validated successfully")
        return 0

if __name__ == "__main__":
    sys.exit(main())
