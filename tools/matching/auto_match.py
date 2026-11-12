#!/usr/bin/env python3
"""
Autonomous Level 3 Function Matching Script

This script attempts to match a single function using IDO 5.2 and the decomp-permuter.
It is safe to run several instances in parallel.

Usage:
    python3 auto_match.py <func_address> [--iterations 5000]
"""

import argparse
import re
import json
import os
import subprocess
import sys
import struct
import tempfile
import shutil
from pathlib import Path


def jal_offset_to_addr(offset):
    """Convert JAL 26-bit offset to full address."""
    return 0x80000000 | (offset << 2)


def fix_m2c_output(code, addr_to_name):
    """Replace address casts with function calls."""
    import re
    # Pattern: (M2C_UNK (*)(args))0x80XXXXXX(call_args)
    pattern = r'\([^)]*\s*\(\*\)\([^)]*\)\)0x([0-9a-fA-F]+)\('
    
    def replace_func_call(match):
        addr = int(match.group(1), 16)
        name = addr_to_name.get(addr, f'func_{addr:08X}')
        return f'{name}('
    
    return re.sub(pattern, replace_func_call, code)


def extract_called_functions(code):
    """Extract function names called in the code."""
    import re
    # Pattern: function_name(
    pattern = r'\b(func_[0-9A-Fa-f]{8})\('
    return set(re.findall(pattern, code))


def generate_extern_declarations(func_names, return_int_funcs=None):
    """Generate extern declarations for functions."""
    if return_int_funcs is None:
        return_int_funcs = set()
    lines = []
    for name in sorted(func_names):
        if name in return_int_funcs:
            lines.append(f'extern s32 {name}();')
        else:
            lines.append(f'extern void {name}();')
    return '\n'.join(lines)


# Project paths
PROJECT_ROOT = Path(__file__).resolve().parents[2]
EXECUTABLE = PROJECT_ROOT / "disks" / "SLPS_000.65"
FUNCTION_MAP = PROJECT_ROOT / "build" / "function_analysis" / "function_map.json"
RESULTS_DIR = PROJECT_ROOT / "build" / "level3_matching" / "results"
PERMUTER_DIR = PROJECT_ROOT / "build" / "decomp-permuter"
IDO_CC = PROJECT_ROOT / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
OBIN_PATH = PROJECT_ROOT / "tmp" / "o.bin"

# Compile flags
IDO_FLAGS = ["-O2", "-Wo,-loopunroll,0", "-G0", "-mips1", "-non_shared", "-c"]


def load_function_map():
    """Load the function map JSON."""
    with open(FUNCTION_MAP) as f:
        return json.load(f)


def get_function_info(func_map, address):
    """Get function info by address."""
    addr_str = f"0x{address:08x}" if isinstance(address, int) else address
    for func in func_map["functions"]:
        if func["vram_start"].lower() == addr_str.lower():
            return func
    return None


def extract_function_bytes(func_info):
    """Extract raw bytes of a function from the executable."""
    file_offset = int(func_info["file_offset"], 16)
    size = func_info["size"]

    with open(EXECUTABLE, "rb") as f:
        f.seek(file_offset)
        return f.read(size)


def create_target_asm(func_info, output_path):
    """Create target assembly file for permuter."""
    func_name = func_info["name"]
    disasm = func_info["disassembly"]
    func_size = func_info["size"]

    asm_lines = [
        ".set noat",
        ".set noreorder",
        "",
        ".text",
        f".globl {func_name}",
        f".type {func_name}, @function",
        f"{func_name}:",
    ]

    import re

    # Instructions that use internal branch targets (relative branches)
    branch_insns = {"beq", "bne", "beqz", "bnez", "bgtz", "bgez", "bltz", "blez", "b"}

    # First pass: collect ONLY internal branch targets (within function)
    branch_targets = set()
    for line in disasm:
        parts = line.split("\t")
        if len(parts) >= 3:
            mnemonic = parts[2]
            operands = parts[3] if len(parts) > 3 else ""
            # Only convert internal branches (not jal/j to external functions)
            if mnemonic in branch_insns and operands:
                # Extract branch target (last operand after comma, or the whole thing)
                target_match = re.search(r'0x([0-9a-f]+)$', operands)
                if target_match:
                    target_offset = int(target_match.group(1), 16)
                    # Only add if target is within function (internal branch)
                    if target_offset < func_size:
                        branch_targets.add(target_offset)

    # Second pass: generate assembly with labels for internal branches only
    for line in disasm:
        parts = line.split("\t")
        if len(parts) >= 3:
            # Get offset from first part: "0:" -> 0
            offset = int(parts[0].rstrip(":"), 16)

            # Add label if this offset is an internal branch target
            if offset in branch_targets:
                asm_lines.append(f".L{func_name}_{offset:x}:")

            mnemonic = parts[2]
            operands = parts[3] if len(parts) > 3 else ""

            # Add $ prefix to register names
            if operands:
                operands = re.sub(r'\b(zero|at|v[0-1]|a[0-3]|t[0-9]|s[0-8]|k[0-1]|gp|sp|fp|ra)\b',
                                  r'$\1', operands)

                # ONLY convert internal branch targets to labels
                # Don't touch jal targets (external functions) or lui/addiu (data addresses)
                if mnemonic in branch_insns:
                    def replace_branch_target(m):
                        target_offset = int(m.group(1), 16)
                        if target_offset < func_size:
                            return f'.L{func_name}_{m.group(1)}'
                        return m.group(0)  # Keep original
                    operands = re.sub(r'0x([0-9a-f]+)$', replace_branch_target, operands)

            # Convert JAL offsets to full addresses
            if mnemonic == 'jal' and operands:
                match = re.search(r'0x([0-9a-f]+)$', operands)
                if match:
                    offset = int(match.group(1), 16)
                    full_addr = jal_offset_to_addr(offset)
                    operands = f'0x{full_addr:08x}'
            
            instruction = f"{mnemonic}\t{operands}" if operands else mnemonic
            asm_lines.append(f"    {instruction}")

    with open(output_path, "w") as f:
        f.write("\n".join(asm_lines) + "\n")


def create_base_c(func_info, output_path, asm_path):
    """Create base C source file using m2c decompiler."""
    func_name = func_info["name"]
    m2c_path = PROJECT_ROOT / "tools" / "m2c" / "m2c.py"

    # Build address to name mapping from function map
    func_map = load_function_map()
    addr_to_name = {}
    for f in func_map["functions"]:
        addr = int(f["vram_start"], 16)
        addr_to_name[addr] = f["name"]

    # Try to decompile with m2c
    try:
        result = subprocess.run(
            [sys.executable, str(m2c_path), "-t", "mips-ido-c", "--valid-syntax", str(asm_path)],
            capture_output=True,
            text=True,
            timeout=30
        )

        if result.returncode == 0 and result.stdout.strip():
            raw_output = result.stdout
            
            # Check for M2C_ERROR but still try to use the output
            if 'M2C_ERROR' in raw_output or 'unset register' in raw_output:
                print(f"  m2c partial: output contains M2C_ERROR (may still work)")
            
            # Post-process to replace address casts with function calls
            processed_code = fix_m2c_output(raw_output, addr_to_name)
            processed_code = fix_void_ptr_deref(processed_code)

            # Extract called functions and detect which return values
            called_funcs = extract_called_functions(processed_code)

            # Detect functions that return values (assigned to something)
            return_int_funcs = set()
            for name in called_funcs:
                if re.search(rf'=\s*{name}\(', processed_code):
                    return_int_funcs.add(name)

            extern_decls = generate_extern_declarations(called_funcs, return_int_funcs)

            # Add type definitions header with m2c macros
            c_code = """typedef unsigned int u32;
typedef signed int s32;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned char u8;
typedef signed char s8;
typedef long long s64;

/* m2c macros for unknown types */
typedef s32 M2C_UNK;
typedef s8  M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))
#define M2C_BITWISE(type, expr) ((type)(expr))
#define M2C_ERROR(desc) (0)
#define M2C_CARRY 0
#define M2C_MEMCPY_ALIGNED memcpy
#define M2C_MEMCPY_UNALIGNED memcpy
#define M2C_UNALIGNED32(expr) (expr)
extern void *memcpy(void *, const void *, u32);

/* Extern function declarations */
""" + extern_decls + """

""" + processed_code
            with open(output_path, "w") as f:
                f.write(c_code)
            return True
    except Exception as e:
        print(f"  m2c failed: {e}")

    # Fallback to simple stub
    is_leaf = func_info.get("num_jal_calls", 0) == 0
    c_code = f"""typedef unsigned int u32;
typedef signed int s32;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned char u8;
typedef signed char s8;

void {func_name}(void) {{
    /* m2c decompilation failed - stub */
}}
"""
    with open(output_path, "w") as f:
        f.write(c_code)
    return False


    """Create base C source file using m2c decompiler."""
    func_name = func_info["name"]
    m2c_path = PROJECT_ROOT / "tools" / "m2c" / "m2c.py"

    # Build address to name mapping from function map
    func_map = load_function_map()
    addr_to_name = {}
    for f in func_map["functions"]:
        addr = int(f["vram_start"], 16)
        addr_to_name[addr] = f["name"]

    # Try to decompile with m2c
    try:
        result = subprocess.run(
            [sys.executable, str(m2c_path), "-t", "mips-ido-c", "--valid-syntax", str(asm_path)],
            capture_output=True,
            text=True,
            timeout=30
        )

        if result.returncode == 0 and result.stdout.strip():
            # Check if m2c output contains errors (M2C_ERROR, unset register, etc.)
            raw_output = result.stdout
            has_errors = 'M2C_ERROR' in raw_output or 'unset register' in raw_output
            if has_errors:
                print(f"  m2c partial: output contains M2C_ERROR (may still work)")
            
            # Post-process to replace address casts with function calls
            processed_code = fix_m2c_output(raw_output, addr_to_name)
            processed_code = fix_void_ptr_deref(processed_code)

                # Extract called functions and detect which return values
            called_funcs = extract_called_functions(processed_code)

                # Detect functions that return values (assigned to something)
            return_int_funcs = set()
            for name in called_funcs:
                    # Pattern: var = func_name() or *(type*) = func_name()
                if re.search(rf'=\s*{name}\(', processed_code):
                    return_int_funcs.add(name)

            extern_decls = generate_extern_declarations(called_funcs, return_int_funcs)

                # Add type definitions header
            c_code = """typedef unsigned int u32;
typedef signed int s32;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned char u8;
typedef signed char s8;
typedef long long s64;

/* m2c macros for unknown types */
typedef s32 M2C_UNK;
typedef s8  M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))
#define M2C_BITWISE(type, expr) ((type)(expr))
#define M2C_ERROR(desc) (0)
#define M2C_CARRY 0

/* Extern function declarations */
""" + extern_decls + """

""" + processed_code
            with open(output_path, "w") as f:
                f.write(c_code)
            return True
    except Exception as e:
        print(f"  m2c failed: {e}")

    # Fallback to simple stub
    is_leaf = func_info.get("num_jal_calls", 0) == 0
    c_code = f"""typedef unsigned int u32;
typedef signed int s32;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned char u8;
typedef signed char s8;

void {func_name}(void) {{
    /* m2c decompilation failed - stub */
}}
"""
    with open(output_path, "w") as f:
        f.write(c_code)
    return False


def create_compile_script(output_path):
    """Create compile.sh for permuter."""
    script = f"""#!/bin/bash
set -e
# Permuter passes: compile.sh source.c -o output.o
{IDO_CC} {' '.join(IDO_FLAGS)} "$1" -o "$3"
"""
    with open(output_path, "w") as f:
        f.write(script)
    os.chmod(output_path, 0o755)


def create_settings_toml(output_path):
    """Create settings.toml for permuter."""
    with open(output_path, "w") as f:
        f.write('compiler = "ido"\n')


def setup_permuter_dir(func_info, permuter_work_dir):
    """Set up a permuter directory for a function.

    Returns: (asm_success, m2c_success)
        - asm_success: True if target assembly was assembled successfully
        - m2c_success: True if m2c decompiled successfully (not a stub)
    """
    permuter_work_dir.mkdir(parents=True, exist_ok=True)

    # Create target assembly first (needed by m2c)
    asm_path = permuter_work_dir / "target.s"
    create_target_asm(func_info, asm_path)

    # Create C source using m2c (returns False if m2c failed and stub was used)
    m2c_success = create_base_c(func_info, permuter_work_dir / "base.c", asm_path)
    create_compile_script(permuter_work_dir / "compile.sh")
    create_settings_toml(permuter_work_dir / "settings.toml")

    # Assemble target
    result = subprocess.run(
        ["mipsel-linux-gnu-as", "-march=r3000", "-o",
         str(permuter_work_dir / "target.o"),
         str(permuter_work_dir / "target.s")],
        capture_output=True, text=True
    )

    return result.returncode == 0, m2c_success


def run_permuter(permuter_work_dir, timeout_seconds=300):
    """Run the permuter and return results."""
    env = os.environ.copy()
    env["PATH"] = f"{PROJECT_ROOT}/bin:" + env.get("PATH", "")

    # Use timeout command to limit permuter runtime
    result = subprocess.run(
        ["timeout", str(timeout_seconds), sys.executable, "permuter.py",
         str(permuter_work_dir), "-j4", "--better-only", "--stop-on-zero"],
        cwd=PERMUTER_DIR,
        capture_output=True,
        text=True,
        timeout=timeout_seconds + 60,  # Extra buffer for timeout command
        env=env
    )

    return result.stdout, result.stderr, result.returncode


def get_best_result(permuter_work_dir):
    """Get the best permuter result."""
    best_score = float('inf')
    best_source = None

    for entry in permuter_work_dir.iterdir():
        if entry.is_dir() and entry.name.startswith("output-"):
            score_file = entry / "score.txt"
            source_file = entry / "source.c"

            if score_file.exists():
                try:
                    score = int(score_file.read_text().strip())
                    if score < best_score:
                        best_score = score
                        if source_file.exists():
                            best_source = source_file.read_text()
                except:
                    pass

    return best_score, best_source


def save_result(func_info, matched, best_score, best_source, base_score, m2c_failed=False):
    """Save matching result."""
    func_name = func_info["name"]
    result_file = RESULTS_DIR / f"{func_name}.json"

    result = {
        "function": func_name,
        "address": func_info["vram_start"],
        "size": func_info["size"],
        "matched": matched,
        "base_score": base_score,
        "best_score": best_score,
        "best_source": best_source,
        "complexity": func_info.get("complexity", "unknown"),
        "is_leaf": func_info.get("num_jal_calls", 0) == 0,
        "m2c_failed": m2c_failed
    }

    with open(result_file, "w") as f:
        json.dump(result, f, indent=2)

    return result_file


def match_function(address, timeout_seconds=300):
    """Main function matching routine."""
    # Load function map
    func_map = load_function_map()

    # Get function info
    func_info = get_function_info(func_map, address)
    if not func_info:
        print(f"ERROR: Function at {address} not found")
        return False, None

    func_name = func_info["name"]
    print(f"=== Matching {func_name} ({func_info['size']} bytes) ===")

    # Create permuter work directory
    permuter_work_dir = PROJECT_ROOT / "build" / "level3_matching" / "permuter_outputs" / func_name

    # Set up permuter directory
    asm_success, m2c_success = setup_permuter_dir(func_info, permuter_work_dir)

    if not asm_success:
        print(f"ERROR: Failed to assemble target for {func_name}")
        save_result(func_info, False, float('inf'), None, None)
        return False, None

    if not m2c_success:
        print(f"SKIPPED: m2c failed to decompile {func_name} (needs manual work)")
        save_result(func_info, False, float('inf'), None, None, m2c_failed=True)
        return False, None

    # Run permuter
    print(f"Running permuter for {timeout_seconds}s (--stop-on-zero)...")
    try:
        stdout, stderr, returncode = run_permuter(permuter_work_dir, timeout_seconds)

        # Parse base score from output
        base_score = None
        for line in stdout.split('\n'):
            if "base score" in line:
                try:
                    base_score = int(line.split('=')[1].strip())
                except:
                    pass
                break

        # Check if matched (score = 0)
        best_score, best_source = get_best_result(permuter_work_dir)
        matched = best_score == 0

        if matched:
            print(f"SUCCESS: {func_name} matched!")
        else:
            print(f"NOT MATCHED: {func_name} - best score: {best_score} (base: {base_score})")

        # Save result
        result_file = save_result(func_info, matched, best_score, best_source, base_score)
        print(f"Result saved to: {result_file}")

        return matched, best_score

    except subprocess.TimeoutExpired:
        print(f"TIMEOUT: {func_name} - permuter took too long")
        save_result(func_info, False, float('inf'), None, None)
        return False, None
    except Exception as e:
        print(f"ERROR: {func_name} - {str(e)}")
        return False, None


def main():
    parser = argparse.ArgumentParser(description="Autonomous function matching")
    parser.add_argument("address", help="Function address (e.g., 0x80041000 or 80041000)")
    parser.add_argument("--timeout", type=int, default=300, help="Permuter timeout in seconds")

    args = parser.parse_args()

    # Parse address
    addr = args.address.lower().replace("0x", "")
    address = int(addr, 16)

    matched, score = match_function(address, args.timeout)

    sys.exit(0 if matched else 1)


if __name__ == "__main__":
    main()


def fix_void_ptr_deref(code):
    """Fix *(void *)addr patterns by replacing with *(u8 *)addr."""
    import re
    # Replace *(void *)addr = with *(u8 *)addr =
    code = re.sub(r'\*\(void \*\)(0x[0-9A-Fa-f]+)\s*=', r'*(u8 *)\1 =', code)
    return code
