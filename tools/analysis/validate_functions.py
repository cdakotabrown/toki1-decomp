#!/usr/bin/env python3
"""
Validate Functions - Check decompiled functions against known checksums

This tool validates that decompiled functions:
1. Compile successfully
2. Match their expected SHA1 checksums (for matching functions)
3. Don't regress (accidentally change from matching to non-matching)

Usage:
    python3 tools/validate_functions.py                    # Validate all functions
    python3 tools/validate_functions.py func_800AACFC      # Validate specific function
    python3 tools/validate_functions.py --changed          # Validate only changed files
    python3 tools/validate_functions.py --update           # Update checksums for matching functions
"""

import argparse
import hashlib
import json
import os
import re
import subprocess
import sys
from pathlib import Path
from typing import Dict, List, Optional, Tuple


class Color:
    """ANSI color codes"""
    RED = '\033[0;31m'
    GREEN = '\033[0;32m'
    YELLOW = '\033[1;33m'
    BLUE = '\033[0;34m'
    NC = '\033[0m'  # No Color


class FunctionValidator:
    def __init__(self, project_root: Path):
        self.project_root = project_root
        self.manifest_path = project_root / "config" / "function-checksums.json"
        self.manifest = self.load_manifest()

    def load_manifest(self) -> Dict:
        """Load function checksum manifest"""
        if not self.manifest_path.exists():
            return {"functions": {}, "version": "1.0"}

        with open(self.manifest_path, 'r') as f:
            return json.load(f)

    def save_manifest(self):
        """Save function checksum manifest"""
        self.manifest_path.parent.mkdir(parents=True, exist_ok=True)
        with open(self.manifest_path, 'w') as f:
            json.dump(self.manifest, f, indent=2, sort_keys=True)
            f.write('\n')  # Trailing newline

    def get_function_sha1(self, func_name: str, obj_file: Path) -> Optional[str]:
        """Extract SHA1 of a specific function from compiled object file"""
        if not obj_file.exists():
            return None

        try:
            # Use objdump to disassemble just this function
            result = subprocess.run(
                ['mipsel-linux-gnu-objdump', '-d', str(obj_file)],
                capture_output=True,
                text=True,
                check=True
            )

            # Extract the function's assembly
            in_function = False
            func_asm = []

            for line in result.stdout.split('\n'):
                # Start of function
                if f'<{func_name}>:' in line:
                    in_function = True
                    continue

                # End of function (next function or end of section)
                if in_function and (line.startswith('Disassembly') or
                                   (line.strip() and not line.startswith(' '))):
                    break

                # Collect function assembly (skip address and hex, keep instruction)
                if in_function and line.strip():
                    # Format: "  400080:	3c020001 	lui	v0,0x1"
                    # We want: "lui v0,0x1" (instruction + operands)
                    parts = line.split('\t')
                    if len(parts) >= 3:
                        func_asm.append(parts[2].strip())

            if func_asm:
                # Hash the normalized assembly
                asm_text = '\n'.join(func_asm)
                return hashlib.sha1(asm_text.encode()).hexdigest()

            return None

        except subprocess.CalledProcessError:
            return None

    def compile_function(self, c_file: Path, func_name: str) -> Optional[Path]:
        """Compile a C file containing a function and return the object file path"""
        # This is a simplified version - in practice you'd use the actual build system
        build_dir = self.project_root / "build" / "jp" / "validation"
        build_dir.mkdir(parents=True, exist_ok=True)

        obj_file = build_dir / f"{c_file.stem}.o"

        # Use same flags as main build
        cmd = [
            'mipsel-linux-gnu-gcc',
            '-c',
            '-Iinclude',
            '-O2',
            '-G0',
            '-funsigned-char',
            '-fpeephole',
            '-ffunction-cse',
            '-fpcc-struct-return',
            '-fcommon',
            '-msoft-float',
            str(c_file),
            '-o', str(obj_file)
        ]

        try:
            subprocess.run(cmd, capture_output=True, check=True)
            return obj_file
        except subprocess.CalledProcessError as e:
            print(f"{Color.RED}✗{Color.NC} Failed to compile {c_file}: {e.stderr.decode()}")
            return None

    def validate_function(self, func_name: str, c_file: Path, address: str) -> Tuple[bool, str]:
        """
        Validate a single function

        Returns: (success, message)
        """
        # Check if function is in manifest
        func_info = self.manifest["functions"].get(func_name)

        # Compile the function
        obj_file = self.compile_function(c_file, func_name)
        if not obj_file:
            return False, "Compilation failed"

        # Get current SHA1
        current_sha1 = self.get_function_sha1(func_name, obj_file)
        if not current_sha1:
            return False, "Could not extract SHA1 from compiled output"

        # If not in manifest, this is a new function
        if not func_info:
            return True, f"New function (SHA1: {current_sha1[:8]}...)"

        # Check if it's supposed to match
        expected_sha1 = func_info.get("sha1")
        status = func_info.get("status", "unknown")

        if status == "matching":
            # Must match expected SHA1
            if current_sha1 == expected_sha1:
                return True, f"Matching (SHA1: {current_sha1[:8]}...)"
            else:
                return False, f"REGRESSION: Was matching, now differs!\n" \
                            f"  Expected: {expected_sha1[:8]}...\n" \
                            f"  Got:      {current_sha1[:8]}..."

        elif status == "non-matching":
            # Just needs to compile (which it did)
            return True, f"Non-matching (compiles OK, SHA1: {current_sha1[:8]}...)"

        else:
            return True, f"Unknown status '{status}' (SHA1: {current_sha1[:8]}...)"

    def find_functions_in_file(self, c_file: Path) -> List[Tuple[str, str]]:
        """Find all function definitions in a C file

        Returns: List of (function_name, address) tuples
        """
        functions = []

        with open(c_file, 'r') as f:
            content = f.read()

        # Match function definitions
        # Pattern: return_type func_name(...) { or INCLUDE_ASM(..., func_name)

        # C function definitions
        func_pattern = r'^(?:static\s+)?(?:inline\s+)?(?:\w+\s+)+(\w+)\s*\([^)]*\)\s*\{'
        for match in re.finditer(func_pattern, content, re.MULTILINE):
            func_name = match.group(1)
            # Try to find address from comment or name
            address = self.guess_address(func_name)
            functions.append((func_name, address))

        # INCLUDE_ASM macro
        asm_pattern = r'INCLUDE_ASM\s*\([^,]+,\s*(\w+)\s*\)'
        for match in re.finditer(asm_pattern, content):
            func_name = match.group(1)
            address = self.guess_address(func_name)
            functions.append((func_name, address))

        return functions

    def guess_address(self, func_name: str) -> str:
        """Try to extract address from function name like func_800AACFC"""
        match = re.match(r'func_([0-9A-Fa-f]{8})', func_name)
        if match:
            return f"0x{match.group(1)}"
        return "unknown"

    def get_changed_files(self) -> List[Path]:
        """Get list of C files changed in git staging area"""
        try:
            result = subprocess.run(
                ['git', 'diff', '--cached', '--name-only', '--diff-filter=AM'],
                capture_output=True,
                text=True,
                check=True,
                cwd=self.project_root
            )

            changed = []
            for line in result.stdout.strip().split('\n'):
                if line.endswith('.c'):
                    path = self.project_root / line
                    if path.exists():
                        changed.append(path)

            return changed

        except subprocess.CalledProcessError:
            return []


def main():
    parser = argparse.ArgumentParser(
        description="Validate decompiled functions against checksums"
    )
    parser.add_argument(
        'functions',
        nargs='*',
        help='Specific functions to validate (default: all)'
    )
    parser.add_argument(
        '--changed',
        action='store_true',
        help='Validate only functions in changed files (git staging area)'
    )
    parser.add_argument(
        '--update',
        action='store_true',
        help='Update checksums for matching functions'
    )
    parser.add_argument(
        '--file',
        type=Path,
        help='Validate functions in a specific file'
    )

    args = parser.parse_args()

    # Find project root
    project_root = Path(__file__).resolve().parent.parent

    validator = FunctionValidator(project_root)

    # Determine which files to check
    files_to_check = []

    if args.file:
        files_to_check = [args.file]
    elif args.changed:
        files_to_check = validator.get_changed_files()
        if not files_to_check:
            print(f"{Color.GREEN}✓{Color.NC} No C files changed")
            return 0
    else:
        # Check all C files in src/
        src_dir = project_root / "src"
        files_to_check = list(src_dir.rglob("*.c"))

    if not files_to_check:
        print(f"{Color.RED}✗{Color.NC} No files to validate")
        return 1

    # Validate functions
    print(f"{Color.BLUE}Validating functions...{Color.NC}\n")

    total = 0
    passed = 0
    failed = 0

    for c_file in files_to_check:
        functions = validator.find_functions_in_file(c_file)

        if not functions:
            continue

        print(f"{Color.BLUE}{c_file.relative_to(project_root)}:{Color.NC}")

        for func_name, address in functions:
            # Skip if specific functions requested and this isn't one
            if args.functions and func_name not in args.functions:
                continue

            total += 1
            success, message = validator.validate_function(func_name, c_file, address)

            if success:
                passed += 1
                print(f"  {Color.GREEN}✓{Color.NC} {func_name} - {message}")
            else:
                failed += 1
                print(f"  {Color.RED}✗{Color.NC} {func_name} - {message}")

    print()
    print("=" * 70)
    print(f"Total: {total} | Passed: {Color.GREEN}{passed}{Color.NC} | "
          f"Failed: {Color.RED}{failed}{Color.NC}")
    print("=" * 70)

    if failed > 0:
        print()
        print(f"{Color.RED}Validation failed!{Color.NC}")
        print("Some functions have regressed or failed to compile.")
        return 1

    print()
    print(f"{Color.GREEN}All functions validated successfully!{Color.NC}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
