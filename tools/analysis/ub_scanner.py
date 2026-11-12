#!/usr/bin/env python3
"""
ub_scanner.py — Scan toki1-decomp decomp C code for IDO→GCC undefined behavior.

Detects patterns that compile on IDO 5.2 but are UB or produce wrong results
on GCC/Clang (PC port). Each checker emits severity + explanation.

Adapted from BattleShip (SSB64 PC port) project.

Usage:
    python tools/analysis/ub_scanner.py [--dir src/] [--exclude psyq]
    python tools/analysis/ub_scanner.py --file src/main/matched/func_80041234.c
    python tools/analysis/ub_scanner.py --check signed-shift   # run one checker only
    python tools/analysis/ub_scanner.py --summary              # counts only
"""

import argparse
import os
import re
import sys
from dataclasses import dataclass
from pathlib import Path
from collections import Counter


@dataclass
class Finding:
    file: str
    line: int
    severity: str        # CRITICAL, HIGH, MEDIUM, LOW
    category: str        # short tag
    message: str         # what's wrong
    code: str            # the offending line
    suggestion: str = "" # how to fix


# ---------------------------------------------------------------------------
# Checkers
# ---------------------------------------------------------------------------

def check_signed_left_shift(filepath: str, lines: list[str]) -> list[Finding]:
    """Left-shifting signed values is UB in C99/C11 when result overflows
    or the value is negative. IDO treats it as a logical shift; GCC may
    optimize assuming no UB."""
    findings = []
    # Pattern: (expr << N) where expr could be signed s32/int/s16/long
    shift_re = re.compile(
        r'\b(val|value|temp|result|ret|i|j|k)\s*<<\s*(\d+)'
    )
    for i, line in enumerate(lines):
        stripped = line.lstrip()
        if stripped.startswith('//') or stripped.startswith('/*'):
            continue
        m = shift_re.search(line)
        if m:
            shift_amt = int(m.group(2))
            if shift_amt >= 16:  # Higher risk shifts
                findings.append(Finding(
                    file=filepath, line=i+1, severity="HIGH",
                    category="signed-shift",
                    message=f"Left-shift of potentially signed value by {shift_amt}. "
                            "UB if value is negative or result overflows.",
                    code=line.rstrip(),
                    suggestion="Cast to unsigned before shifting: ((u32)val << N)"
                ))
    return findings


def check_type_pun_deref(filepath: str, lines: list[str]) -> list[Finding]:
    """Detect *(Type*)&var patterns — strict aliasing violation.
    GCC with -O2 may optimize assuming strict aliasing."""
    findings = []
    pun_re = re.compile(
        r'\*\s*\(\s*'
        r'(u32|s32|u16|s16|u8|s8|int|float|unsigned\s+int|long)'
        r'\s*\*\s*\)\s*[&(]'
    )
    # Also catch struct pun: *(StructName*)&var
    struct_pun_re = re.compile(
        r'\*\s*\(\s*([A-Z][A-Za-z0-9_]+)\s*\*\s*\)\s*&'
    )
    for i, line in enumerate(lines):
        stripped = line.lstrip()
        if stripped.startswith('//'):
            continue
        m = pun_re.search(line)
        if m:
            findings.append(Finding(
                file=filepath, line=i+1, severity="MEDIUM",
                category="strict-alias",
                message=f"Type-pun dereference *(({m.group(1)}*)&...). "
                        f"Strict aliasing violation.",
                code=line.rstrip(),
                suggestion="Use memcpy() or a union for type punning."
            ))
        m2 = struct_pun_re.search(line)
        if m2 and m2.group(1) not in ('void',):
            findings.append(Finding(
                file=filepath, line=i+1, severity="MEDIUM",
                category="strict-alias",
                message=f"Struct type-pun dereference *({m2.group(1)}*)&var. "
                        f"Reads memory as incompatible struct type.",
                code=line.rstrip(),
                suggestion="Use memcpy() or restructure to avoid aliasing."
            ))
    return findings


def check_ternary_side_effect(filepath: str, lines: list[str]) -> list[Finding]:
    """Detect var = (cond) ? val : var++ — reads and modifies var
    without a sequence point. Clear UB."""
    findings = []
    # Pattern: varname++ or varname-- in ternary where same var is on LHS
    ternary_re = re.compile(
        r'(\w+)\s*=\s*.*\?\s*.*:\s*\1\s*(\+\+|--)'
        r'|'
        r'(\w+)\s*=\s*.*\?\s*\3\s*(\+\+|--).*:'
    )
    for i, line in enumerate(lines):
        m = ternary_re.search(line)
        if m:
            var = m.group(1) or m.group(3)
            findings.append(Finding(
                file=filepath, line=i+1, severity="CRITICAL",
                category="sequence-point",
                message=f"Variable '{var}' is read and modified in ternary "
                        f"expression without intervening sequence point. "
                        f"Undefined behavior.",
                code=line.rstrip(),
                suggestion=f"Split into separate statements."
            ))
    return findings


def check_union_type_punning(filepath: str, lines: list[str]) -> list[Finding]:
    """Detect union { int i; float f; } patterns where .i is written
    and .f is read (or vice versa). UB in C99, implementation-defined
    in C11."""
    findings = []
    union_write_re = re.compile(r'(\w+)\.(i|integer|word|u)\s*=')
    for i, line in enumerate(lines):
        m = union_write_re.search(line)
        if m:
            var = m.group(1)
            written_field = m.group(2)
            # Look ahead for read of different field
            for j in range(i+1, min(i+5, len(lines))):
                if re.search(rf'{var}\.(f|float|flt)\b', lines[j]):
                    findings.append(Finding(
                        file=filepath, line=i+1, severity="MEDIUM",
                        category="union-pun",
                        message=f"Union '{var}' written as .{written_field}, "
                                f"read as float member. Type punning through "
                                f"union is implementation-defined.",
                        code=line.rstrip(),
                        suggestion="Use memcpy() for portable type punning."
                    ))
                    break
    return findings


def check_ps1_ptr_arithmetic(filepath: str, lines: list[str]) -> list[Finding]:
    """Detect pointer arithmetic on PS1 physical addresses (0x80xxxxxx, 0x1Fxxxxxx)
    that would be invalid on PC."""
    findings = []
    phys_addr_re = re.compile(r'0x(80|1F)[0-9a-fA-F]{5,6}')
    uintptr_cmp_re = re.compile(r'\(u32\)\s*\w+\s*[<>]=?\s*0x80')
    for i, line in enumerate(lines):
        if uintptr_cmp_re.search(line):
            findings.append(Finding(
                file=filepath, line=i+1, severity="HIGH",
                category="phys-addr",
                message="Comparing pointer cast to PS1 physical address "
                        "(0x80xxxxxx). Invalid on PC.",
                code=line.rstrip(),
                suggestion="Guard with #ifdef PC_PORT or replace with "
                           "PC-compatible memory access."
            ))
    return findings


def check_macro_side_effects(filepath: str, lines: list[str]) -> list[Finding]:
    """Detect calls to multi-eval macros (ABS, MAX, MIN)
    with side-effecting arguments (++, --, function calls)."""
    findings = []
    # These macros evaluate their arg 2+ times
    multi_eval = re.compile(
        r'\b(ABS|MAX|MIN|CLAMP)\s*\('
    )
    for i, line in enumerate(lines):
        m = multi_eval.search(line)
        if m:
            macro = m.group(1)
            # Extract the argument (rough: from open paren to matching close)
            start = m.end()
            depth = 1
            j = start
            while j < len(line) and depth > 0:
                if line[j] == '(':
                    depth += 1
                elif line[j] == ')':
                    depth -= 1
                j += 1
            arg = line[start:j-1] if j <= len(line) else ""
            # Check for side effects in arg
            if re.search(r'\+\+|--|[a-zA-Z_]\w*\s*\(', arg):
                findings.append(Finding(
                    file=filepath, line=i+1, severity="HIGH",
                    category="macro-side-effect",
                    message=f"Macro {macro}() evaluates its argument multiple "
                            f"times. Argument '{arg.strip()}' has side effects.",
                    code=line.rstrip(),
                    suggestion=f"Assign argument to a temp variable before "
                               f"passing to {macro}()."
                ))
    return findings


def check_uninitialized_switch(filepath: str, lines: list[str]) -> list[Finding]:
    """Detect variables declared before switch/if that are only assigned
    in some branches but used after."""
    findings = []
    decl_no_init = re.compile(
        r'^\s+(s32|u32|s16|u16|void\s*\*|[A-Z]\w+\s*\*)\s+(\w+)\s*;'
    )
    for i, line in enumerate(lines):
        m = decl_no_init.search(line)
        if m:
            type_name = m.group(1).strip()
            var_name = m.group(2)
            # Only flag pointers and indices — highest risk
            if '*' in type_name or var_name in ('index', 'idx', 'id', 'type'):
                # Look ahead for switch within 10 lines
                for j in range(i+1, min(i+10, len(lines))):
                    if re.search(r'\bswitch\b', lines[j]):
                        findings.append(Finding(
                            file=filepath, line=i+1, severity="MEDIUM",
                            category="uninit-var",
                            message=f"'{type_name} {var_name}' declared "
                                    f"without init before switch statement. "
                                    f"May be uninitialized on some paths.",
                            code=line.rstrip(),
                            suggestion=f"Initialize: {type_name} {var_name} = "
                                       f"{'NULL' if '*' in type_name else '0'};"
                        ))
                        break
    return findings


def check_s32_min_constants(filepath: str, lines: list[str]) -> list[Finding]:
    """Negating INT_MIN or similar is undefined behavior."""
    findings = []
    for i, line in enumerate(lines):
        # -S32_MIN or similar is UB (negation of most-negative)
        if re.search(r'-\s*0x80000000\b', line):
            findings.append(Finding(
                file=filepath, line=i+1, severity="HIGH",
                category="int-negate-min",
                message="Negating 0x80000000 (most-negative s32) is signed "
                        "integer overflow — undefined behavior.",
                code=line.rstrip(),
                suggestion="Use unsigned arithmetic or special-case."
            ))
    return findings


def check_bitfield_signed_width(filepath: str, lines: list[str]) -> list[Finding]:
    """Detect signed bitfields with widths that don't include sign bit properly."""
    findings = []
    bitfield_re = re.compile(r'\bs32\s+\w+\s*:\s*(\d+)\s*;')
    for i, line in enumerate(lines):
        m = bitfield_re.search(line)
        if m:
            width = int(m.group(1))
            if width < 32:
                findings.append(Finding(
                    file=filepath, line=i+1, severity="LOW",
                    category="signed-bitfield",
                    message=f"Signed bitfield with width {width}. Sign extension "
                            f"behavior may differ between compilers.",
                    code=line.rstrip(),
                    suggestion="Consider using unsigned bitfield with explicit "
                               "sign extension at read site."
                ))
    return findings


def check_implicit_int_return(filepath: str, lines: list[str]) -> list[Finding]:
    """Detect function calls without prototypes — implicit int truncates
    64-bit returns on LP64."""
    findings = []
    # Look for extern declarations without includes
    extern_no_proto = re.compile(r'extern\s+\w+\s*\(')
    for i, line in enumerate(lines):
        # This is a heuristic — hard to detect without full parsing
        if 'extern' in line and '(' in line and ')' not in line.split('(')[0]:
            if re.search(r'extern\s+\w+\s*;', line):
                findings.append(Finding(
                    file=filepath, line=i+1, severity="HIGH",
                    category="implicit-proto",
                    message="External variable or function declared without "
                            "full prototype. On LP64, implicit int return "
                            "truncates 64-bit pointers.",
                    code=line.rstrip(),
                    suggestion="Add full function prototype with return type."
                ))
    return findings


def check_psyq_sdk_patterns(filepath: str, lines: list[str]) -> list[Finding]:
    """Detect PsyQ SDK patterns that need PC port attention."""
    findings = []
    # Direct hardware register access
    hw_reg_re = re.compile(r'\*\s*\(\s*volatile\s+u32\s*\*\s*\)\s*0x1F')
    for i, line in enumerate(lines):
        if hw_reg_re.search(line):
            findings.append(Finding(
                file=filepath, line=i+1, severity="HIGH",
                category="hw-register",
                message="Direct PS1 hardware register access (0x1Fxxxxxx). "
                        "Must be stubbed or emulated for PC port.",
                code=line.rstrip(),
                suggestion="Add #ifdef PC_PORT guard with HAL function call."
            ))
    return findings


# ---------------------------------------------------------------------------
# Runner
# ---------------------------------------------------------------------------

ALL_CHECKERS = [
    ("signed-shift",      check_signed_left_shift),
    ("strict-alias",      check_type_pun_deref),
    ("sequence-point",    check_ternary_side_effect),
    ("union-pun",         check_union_type_punning),
    ("phys-addr",         check_ps1_ptr_arithmetic),
    ("macro-side-effect", check_macro_side_effects),
    ("uninit-var",        check_uninitialized_switch),
    ("min-constants",     check_s32_min_constants),
    ("signed-bitfield",   check_bitfield_signed_width),
    ("implicit-proto",    check_implicit_int_return),
    ("hw-register",       check_psyq_sdk_patterns),
]


def scan_file(filepath: str, checkers: list) -> list[Finding]:
    try:
        with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
            lines = f.readlines()
    except (OSError, IOError):
        return []

    findings = []
    for _name, checker in checkers:
        findings.extend(checker(filepath, lines))
    return findings


def main():
    parser = argparse.ArgumentParser(
        description="Scan toki1-decomp decomp code for IDO→GCC undefined behavior"
    )
    parser.add_argument('--dir', default='src/',
                        help="Directory to scan (default: src/)")
    parser.add_argument('--file', default=None,
                        help="Scan a single file")
    parser.add_argument('--exclude', default='',
                        help="Comma-separated subdirs to exclude")
    parser.add_argument('--include-all', action='store_true',
                        help="Don't exclude any directories")
    parser.add_argument('--check', default=None,
                        help="Run only this checker")
    parser.add_argument('--severity', default=None,
                        help="Only show this severity or above "
                             "(CRITICAL,HIGH,MEDIUM,LOW)")
    parser.add_argument('--summary', action='store_true',
                        help="Only show category counts")
    args = parser.parse_args()

    severity_order = {"CRITICAL": 0, "HIGH": 1, "MEDIUM": 2, "LOW": 3}
    min_severity = severity_order.get(args.severity, 3) if args.severity else 3

    # Select checkers
    if args.check:
        checkers = [(n, c) for n, c in ALL_CHECKERS if n == args.check]
        if not checkers:
            print(f"Unknown checker: {args.check}")
            print(f"Available: {', '.join(n for n, _ in ALL_CHECKERS)}")
            sys.exit(1)
    else:
        checkers = ALL_CHECKERS

    # Collect files
    if args.file:
        files = [args.file]
    else:
        exclude = set() if args.include_all else set(args.exclude.split(',')) if args.exclude else set()
        files = []
        for root, dirs, filenames in os.walk(args.dir):
            # Filter excluded subdirs
            rel = os.path.relpath(root, args.dir)
            parts = Path(rel).parts
            if parts and parts[0] in exclude:
                continue
            for fn in filenames:
                if fn.endswith(('.c', '.h')):
                    files.append(os.path.join(root, fn))

    # Also scan include/ for header issues
    if not args.file and os.path.isdir('include'):
        for root, dirs, filenames in os.walk('include'):
            for fn in filenames:
                if fn.endswith('.h'):
                    files.append(os.path.join(root, fn))

    # Also scan pc/ for PC port issues
    if not args.file and os.path.isdir('pc'):
        for root, dirs, filenames in os.walk('pc'):
            for fn in filenames:
                if fn.endswith(('.c', '.h')):
                    files.append(os.path.join(root, fn))

    # Scan
    all_findings: list[Finding] = []
    for f in sorted(files):
        all_findings.extend(scan_file(f, checkers))

    # Filter by severity
    all_findings = [f for f in all_findings
                    if severity_order.get(f.severity, 3) <= min_severity]

    # Sort: CRITICAL first, then HIGH, etc.
    all_findings.sort(key=lambda f: (severity_order.get(f.severity, 3),
                                      f.category, f.file, f.line))

    if args.summary:
        by_cat = Counter()
        by_sev = Counter()
        for f in all_findings:
            by_cat[f.category] += 1
            by_sev[f.severity] += 1
        print(f"\n{'='*60}")
        print(f"UB Scanner Summary — {len(all_findings)} findings "
              f"in {len(files)} files")
        print(f"{'='*60}")
        print(f"\nBy severity:")
        for sev in ["CRITICAL", "HIGH", "MEDIUM", "LOW"]:
            if by_sev[sev]:
                print(f"  {sev:10s}: {by_sev[sev]}")
        print(f"\nBy category:")
        for cat, count in by_cat.most_common():
            print(f"  {cat:25s}: {count}")
        return

    # Full output
    print(f"\n{'='*70}")
    print(f"UB Scanner — {len(all_findings)} findings in {len(files)} files")
    print(f"{'='*70}")

    current_cat = None
    for f in all_findings:
        if f.category != current_cat:
            current_cat = f.category
            print(f"\n--- [{f.category}] ---\n")
        sev_marker = {"CRITICAL": "!!!", "HIGH": "!!", "MEDIUM": "!",
                      "LOW": "."}
        print(f"  {sev_marker.get(f.severity, '?')} [{f.severity}] "
              f"{f.file}:{f.line}")
        print(f"      {f.message}")
        print(f"      > {f.code.strip()}")
        if f.suggestion:
            print(f"      FIX: {f.suggestion}")
        print()

    # Summary footer
    by_sev = Counter(f.severity for f in all_findings)
    print(f"{'='*70}")
    print(f"Totals: ", end="")
    print(" | ".join(f"{s}: {by_sev[s]}" for s in
                     ["CRITICAL", "HIGH", "MEDIUM", "LOW"] if by_sev[s]))
    print(f"{'='*70}")


if __name__ == '__main__':
    main()
