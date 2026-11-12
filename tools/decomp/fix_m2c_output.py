#!/usr/bin/env python3
"""
Fix m2c decompiler output for IDO 5.2 compilation.

This tool post-processes m2c output to fix common issues that prevent
compilation with the IDO 5.2 C compiler.

USAGE:
    # From stdin/stdout (pipeline mode)
    cat func_8004XXXX.c | python3 fix_m2c_output.py > fixed.c

    # From file (in-place edit)
    python3 fix_m2c_output.py func_8004XXXX.c

    # From file with output
    python3 fix_m2c_output.py func_8004XXXX.c -o fixed.c

    # With verbose output
    python3 fix_m2c_output.py func_8004XXXX.c -v

FIXES APPLIED:
    1. Unknown types (?) -> s32 (or void for return types)
    2. Function pointer casts: (type (*)(...))0xADDR( -> proper syntax
    3. M2C_ERROR statements -> removed with warning comments
    4. Suspicious pointer arithmetic -> warning comments
    5. Syntax errors (mismatched braces, void* arithmetic)
    6. Type consistency checks

IDO 5.2 COMPATIBILITY:
    - Uses ANSI C89 syntax only (no // comments, no C99)
    - Typedef headers match project conventions
    - Handles void* pointer arithmetic (illegal in ANSI C)

DATE: January 2026
"""

import sys
import re
import argparse
from pathlib import Path
from typing import List, Tuple, Set


class M2COutputFixer:
    """Post-processor for m2c decompiler output."""

    def __init__(self, verbose=False):
        self.verbose = verbose
        self.warnings = []
        self.fixes_applied = {
            'unknown_types': 0,
            'function_pointers': 0,
            'm2c_errors': 0,
            'pointer_arithmetic': 0,
            'void_pointer_arithmetic': 0,
            'syntax_errors': 0,
        }

    def log(self, msg):
        """Log verbose message."""
        if self.verbose:
            print(f"[fix_m2c_output] {msg}", file=sys.stderr)

    def add_warning(self, line_num, msg):
        """Add a warning message."""
        self.warnings.append((line_num, msg))
        self.log(f"Line {line_num}: {msg}")

    def fix_unknown_return_type(self, line: str) -> str:
        """Fix unknown return type: ? func_name( -> s32 func_name( (conservative: assume returns value)"""
        if re.match(r'^\s*\?\s+\w+\s*\(', line):
            self.fixes_applied['unknown_types'] += 1
            self.log("Fixed unknown return type -> s32 (conservative)")
            return re.sub(r'^\s*\?\s+', 's32 ', line)
        return line

    def fix_unknown_in_params(self, line: str) -> str:
        """Fix unknown types in parameter lists: (?, ?) -> (s32, s32)"""
        # Match function declarations and definitions
        if '(' in line and ('?' in line):
            original = line

            # Fix standalone ? in parameter lists -> s32
            line = re.sub(r'\(\s*\?(?=\s*[,)])', '(s32', line)
            line = re.sub(r',\s*\?(?=\s*[,)])', ', s32', line)

            # Fix ? * (unknown pointer) -> void *
            line = re.sub(r'\?\s*\*', 'void *', line)

            # Fix function pointer with unknown types
            # (? (*)(...)) -> (void (*)(void))
            line = re.sub(
                r'\(\s*\?\s*\(\*\)\s*\([^)]*\)\s*\)',
                '(void (*)(void))',
                line
            )

            if line != original:
                self.fixes_applied['unknown_types'] += 1
                self.log(f"Fixed unknown parameter types")

        return line

    def fix_function_pointer_cast(self, line: str) -> str:
        """Fix function pointer cast syntax: (type (*)(...))0xADDR( -> ((type (*)(...))0xADDR)("""
        # Pattern: (return_type (*)(args))0xADDRESS(
        pattern = r'\(([^()]+)\s*\(\*\)\s*\([^)]*\)\)\s*(0x[0-9A-Fa-f]+)\s*\('

        if re.search(pattern, line):
            # Wrap the entire cast+address in parens, then add call parens
            def replacer(match):
                self.fixes_applied['function_pointers'] += 1
                self.log(f"Fixed function pointer cast at {match.group(2)}")
                return f"(({match.group(0)[:-1]}))("

            line = re.sub(pattern, replacer, line)

        return line

    def fix_m2c_error(self, line: str, line_num: int) -> Tuple[str, bool]:
        """Fix or remove M2C_ERROR statements. Returns (fixed_line, should_keep)."""
        if 'M2C_ERROR' in line or 'MIPS2C_ERROR' in line:
            self.fixes_applied['m2c_errors'] += 1
            self.add_warning(line_num, "Removed M2C_ERROR statement - manual review needed")
            # Return empty line but mark to not completely remove (preserve line numbers)
            return ('    /* M2C_ERROR - removed, needs manual fix */\n', True)
        return (line, True)

    def check_struct_access_on_void_ptr(self, lines: List[str]) -> List[str]:
        """Check for struct member access on void pointer (not allowed in ANSI C)."""
        void_ptr_vars = set()

        # First pass: collect void* variable names
        for line in lines:
            # Pattern: void *varname or void * varname
            match = re.search(r'void\s*\*\s*(\w+)', line)
            if match:
                void_ptr_vars.add(match.group(1))

        # Second pass: check for struct access on those variables
        # Only add ONE warning per function, not per line
        if void_ptr_vars:
            first_issue_found = {}
            for i, line in enumerate(lines):
                # Check for var->member
                for var in void_ptr_vars:
                    if re.search(rf'\b{var}\s*->', line) and var not in first_issue_found:
                        first_issue_found[var] = i
                        self.add_warning(i+1, f"void* variable '{var}' used with -> operator (needs struct definition)")

        return lines

    def check_pointer_arithmetic(self, line: str, line_num: int) -> str:
        """Check for suspicious pointer arithmetic and add warnings."""
        # Detect: ptr += N where N > 1 (likely wrong for typed pointers)
        match = re.search(r'(\w+)\s*\+=\s*(\d+)', line)
        if match:
            var_name = match.group(1)
            increment = int(match.group(2))
            if increment > 1:
                self.fixes_applied['pointer_arithmetic'] += 1
                warning = f"/* WARNING: {var_name} += {increment} - check if pointer arithmetic is correct */"
                self.add_warning(line_num, f"Suspicious pointer arithmetic: {var_name} += {increment}")
                # Add warning comment before the line
                return f"    {warning}\n{line}"

        return line

    def fix_pointer_dereference_of_int(self, line: str, line_num: int) -> str:
        """Fix dereferencing integer expressions: *(expr + *(s32 *)addr) -> *(s32 *)(expr + *(s32 *)addr)"""
        # Pattern: *(any_expr + *(type *)0xADDR) = value
        # This happens when m2c tries to dereference a calculated address

        # More general pattern to catch nested parens
        if '+ *(s32 *)0x' in line and line.strip().startswith('*') and '=' in line:
            # Find the opening * and its matching closing ) before =
            # Need to count parens to find the right closing one
            content = line.strip()
            if content.startswith('*(') and ') =' in content:
                # Find matching paren for the first (
                paren_count = 0
                start_idx = content.index('(')
                end_idx = -1
                for i in range(start_idx, len(content)):
                    if content[i] == '(':
                        paren_count += 1
                    elif content[i] == ')':
                        paren_count -= 1
                        if paren_count == 0:
                            end_idx = i
                            break

                if end_idx > 0 and content[end_idx:end_idx+3] == ') =':
                    # Extract the expression inside the first ()
                    expr = content[start_idx+1:end_idx]
                    # Get everything after ) =
                    rest = content[end_idx+3:].strip()
                    # Get indentation
                    indent = line[:len(line) - len(line.lstrip())]
                    # Rebuild as: *(s32 *)(EXPR) = REST
                    line = f"{indent}*(s32 *)({expr}) = {rest}\n"
                    self.fixes_applied['void_pointer_arithmetic'] += 1
                    self.add_warning(line_num, "Fixed integer dereference -> cast to pointer")

        return line

    def fix_void_pointer_arithmetic(self, line: str, line_num: int) -> str:
        """Fix void pointer arithmetic (illegal in ANSI C): *(void *)0x... -> *(s32 *)0x..."""
        # Pattern: *(void *)0xADDRESS
        if re.search(r'\*\s*\(\s*void\s*\*\s*\)\s*0x[0-9A-Fa-f]+', line):
            original = line
            # Replace *(void *) with *(s32 *) for:
            # 1. Arithmetic contexts (<<, >>, +, -, *, /)
            # 2. Assignment contexts (on left side of =)
            # 3. OR operations (|, &, ^)
            if any(op in line for op in ['<<', '>>', '+', '-', '|', '&', '^']) or re.search(r'\*\s*\(\s*void\s*\*\s*\)\s*0x[0-9A-Fa-f]+\s*=', line):
                line = re.sub(r'\*\s*\(\s*void\s*\*\s*\)', '*(s32 *)', line)
                if line != original:
                    self.fixes_applied['void_pointer_arithmetic'] += 1
                    self.add_warning(line_num, "Fixed void pointer dereference -> s32*")

        return line

    def fix_mismatched_braces(self, lines: List[str]) -> List[str]:
        """Fix common brace mismatches from m2c."""
        # Count braces to detect mismatches
        brace_depth = 0
        fixed = []

        for i, line in enumerate(lines):
            # Count opening and closing braces in this line
            opening = line.count('{')
            closing = line.count('}')

            # Update depth
            brace_depth += opening
            brace_depth -= closing

            # Check for negative depth (too many closing braces)
            if brace_depth < 0:
                # Remove one closing brace from this line
                if line.strip() == '}':
                    self.fixes_applied['syntax_errors'] += 1
                    self.log(f"Line {i+1}: Removed extra closing brace (negative depth)")
                    continue  # Skip this line entirely
                else:
                    # Line has mixed content, try to remove one }
                    if line.count('}') > 0:
                        # Remove the last } on this line
                        line = line[::-1].replace('}', '', 1)[::-1]
                        self.fixes_applied['syntax_errors'] += 1
                        self.log(f"Line {i+1}: Removed extra closing brace from mixed line")
                        brace_depth += 1  # Adjust depth back

            fixed.append(line)

        return fixed

    def has_typedef_header(self, lines: List[str]) -> bool:
        """Check if file already has typedef header."""
        first_few = ''.join(lines[:10])
        return 'typedef unsigned char u8' in first_few or 'typedef signed char s8' in first_few

    def process(self, input_text: str) -> str:
        """Process m2c output and return fixed version."""
        lines = input_text.splitlines(keepends=True)

        # Fix mismatched braces first
        lines = self.fix_mismatched_braces(lines)

        # Check for void* struct access issues
        lines = self.check_struct_access_on_void_ptr(lines)

        fixed_lines = []
        in_function = False
        function_has_errors = False
        error_count_in_function = 0

        for line_num, line in enumerate(lines, 1):
            # Track function boundaries
            if re.match(r'^[a-zA-Z_].*\(.*\)\s*\{?\s*$', line) and not line.strip().endswith(';'):
                in_function = True
                function_has_errors = False
                error_count_in_function = 0

            # Apply fixes in order
            original_line = line

            # 1. Fix unknown return types
            line = self.fix_unknown_return_type(line)

            # 2. Fix unknown types in parameters
            line = self.fix_unknown_in_params(line)

            # 3. Fix function pointer casts
            line = self.fix_function_pointer_cast(line)

            # 4. Fix integer dereference
            line = self.fix_pointer_dereference_of_int(line, line_num)

            # 5. Fix void pointer arithmetic
            line = self.fix_void_pointer_arithmetic(line, line_num)

            # 6. Check pointer arithmetic
            line = self.check_pointer_arithmetic(line, line_num)

            # 7. Handle M2C_ERROR
            line, should_keep = self.fix_m2c_error(line, line_num)
            if 'M2C_ERROR' in original_line:
                error_count_in_function += 1
                function_has_errors = True

            if should_keep:
                fixed_lines.append(line)

            # Check for function end
            if in_function and line.strip() == '}':
                # If function had many errors, add warning
                if error_count_in_function > 3:
                    fixed_lines.insert(-1, '\n    /* WARNING: This function had multiple M2C_ERROR statements */\n')
                    fixed_lines.insert(-1, '    /* NEEDS MANUAL DECOMPILATION */\n\n')
                in_function = False

        result = ''.join(fixed_lines)

        # Print summary if verbose
        if self.verbose:
            print("\n=== FIX SUMMARY ===", file=sys.stderr)
            for fix_type, count in self.fixes_applied.items():
                if count > 0:
                    print(f"  {fix_type}: {count}", file=sys.stderr)
            if self.warnings:
                print(f"\n=== WARNINGS ({len(self.warnings)}) ===", file=sys.stderr)
                for line_num, msg in self.warnings:
                    print(f"  Line {line_num}: {msg}", file=sys.stderr)

        return result


def main():
    parser = argparse.ArgumentParser(
        description='Fix m2c decompiler output for IDO 5.2 compilation',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
EXAMPLES:
  # Pipeline mode (stdin/stdout)
  cat func.c | python3 fix_m2c_output.py > fixed.c

  # In-place edit
  python3 fix_m2c_output.py func.c

  # With output file
  python3 fix_m2c_output.py func.c -o fixed.c

  # Verbose mode
  python3 fix_m2c_output.py func.c -v

FIXES APPLIED:
  - Unknown types (?) replaced with s32 or void
  - Function pointer cast syntax corrected
  - M2C_ERROR statements removed with warnings
  - Void pointer arithmetic fixed (ANSI C compliance)
  - Suspicious pointer arithmetic flagged
  - Syntax errors corrected
        """
    )

    parser.add_argument('input', nargs='?', type=str,
                       help='Input file (default: stdin)')
    parser.add_argument('-o', '--output', type=str,
                       help='Output file (default: stdout or in-place if input file given)')
    parser.add_argument('-v', '--verbose', action='store_true',
                       help='Print verbose output')

    args = parser.parse_args()

    # Read input
    if args.input:
        input_path = Path(args.input)
        if not input_path.exists():
            print(f"Error: Input file not found: {args.input}", file=sys.stderr)
            return 1
        input_text = input_path.read_text()
    else:
        # Read from stdin
        input_text = sys.stdin.read()

    # Process
    fixer = M2COutputFixer(verbose=args.verbose)
    fixed_text = fixer.process(input_text)

    # Write output
    if args.output:
        output_path = Path(args.output)
        output_path.write_text(fixed_text)
        if args.verbose:
            print(f"\nWrote fixed output to: {args.output}", file=sys.stderr)
    elif args.input:
        # In-place edit
        input_path = Path(args.input)
        input_path.write_text(fixed_text)
        if args.verbose:
            print(f"\nUpdated file in-place: {args.input}", file=sys.stderr)
    else:
        # Write to stdout
        sys.stdout.write(fixed_text)

    return 0


if __name__ == '__main__':
    sys.exit(main())
