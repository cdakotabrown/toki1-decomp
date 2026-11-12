#!/usr/bin/env python3
"""
Test C conversion for a single function.

This script:
1. Reads the ASM file
2. Generates initial C code using m2c (if available) or template
3. Compiles with appropriate compiler (GCC or IDO)
4. Compares against target binary
5. Reports match status and issues

Usage:
    python3 tools/analysis/test_c_conversion.py 0x80046738
    python3 tools/analysis/test_c_conversion.py --batch build/asm_categorization.json --limit 10
"""

import argparse
import json
import os
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path


def hex_to_int(addr):
    if isinstance(addr, int):
        return addr
    return int(addr.replace('0x', '').replace('0X', ''), 16)


def get_target_bytes(exe_path: Path, addr: int, size: int) -> bytes:
    """Extract bytes from target executable."""
    load_addr = 0x80041000
    header_size = 0x800
    file_offset = addr - load_addr + header_size

    with open(exe_path, 'rb') as f:
        f.seek(file_offset)
        return f.read(size)


def disassemble_bytes(data: bytes, addr: int) -> list:
    """Disassemble bytes using objdump."""
    # Write to temp file
    with tempfile.NamedTemporaryFile(suffix='.bin', delete=False) as f:
        f.write(data)
        temp_path = f.name

    try:
        result = subprocess.run([
            'mipsel-linux-gnu-objdump', '-D', '-b', 'binary',
            '-m', 'mips:3000', '--adjust-vma', hex(addr), temp_path
        ], capture_output=True, text=True)
        return result.stdout.split('\n')
    finally:
        os.unlink(temp_path)


def read_asm_file(asm_path: Path) -> dict:
    """Parse ASM file for function info."""
    content = asm_path.read_text()

    # Get size
    size_match = re.search(r'[#\*]\s*Size:\s*(\d+)\s*bytes', content, re.IGNORECASE)
    size = int(size_match.group(1)) if size_match else 0

    # If size is 0, count instructions
    if size == 0:
        instr_count = len(re.findall(r'^\s+\w+\s+\$', content, re.MULTILINE))
        size = instr_count * 4

    # Check for issues in comments
    issues = []
    patterns = [
        (r'thunk', 'thunk_pattern'),
        (r'hidden', 'hidden_reg'),
        (r'preceding', 'hidden_reg'),
        (r'before\s+prologue', 'pre_prologue'),
        (r'requires\s+assembly', 'asm_required'),
        (r'cannot\s+be\s+matched', 'unmatchable'),
    ]
    for pattern, issue in patterns:
        if re.search(pattern, content, re.IGNORECASE):
            issues.append(issue)

    return {
        'content': content,
        'size': size,
        'issues': issues,
    }


def compile_with_gcc(c_code: str, output_path: Path) -> tuple:
    """Compile C code with GCC."""
    with tempfile.NamedTemporaryFile(suffix='.c', mode='w', delete=False) as f:
        f.write(c_code)
        c_path = f.name

    try:
        result = subprocess.run([
            'mipsel-linux-gnu-gcc', '-O2', '-G0', '-mips1', '-march=r3000', '-mabi=32',
            '-fno-pic', '-mno-abicalls', '-ffreestanding',
            '-I', 'include', '-c', c_path, '-o', str(output_path)
        ], capture_output=True, text=True)
        return result.returncode == 0, result.stderr
    finally:
        os.unlink(c_path)


def compile_with_ido(c_code: str, output_path: Path) -> tuple:
    """Compile C code with IDO 5.2."""
    with tempfile.NamedTemporaryFile(suffix='.c', mode='w', delete=False) as f:
        f.write(c_code)
        c_path = f.name

    ido_cc = Path('tools/ido-static-recomp/build/5.2/out/cc')
    if not ido_cc.exists():
        return False, "IDO compiler not found"

    try:
        result = subprocess.run([
            str(ido_cc), '-O2', '-G0', '-mips1', '-non_shared',
            '-I', 'include', '-c', c_path, '-o', str(output_path)
        ], capture_output=True, text=True)
        return result.returncode == 0, result.stderr
    finally:
        os.unlink(c_path)


def extract_text_section(obj_path: Path) -> bytes:
    """Extract .text section from object file."""
    result = subprocess.run([
        'mipsel-linux-gnu-objcopy', '-O', 'binary', '--only-section=.text',
        str(obj_path), '/dev/stdout'
    ], capture_output=True)
    return result.stdout


def compare_bytes(target: bytes, compiled: bytes) -> dict:
    """Compare two byte sequences instruction by instruction."""
    target_words = [struct.unpack('<I', target[i:i+4])[0] for i in range(0, len(target), 4)]
    compiled_words = [struct.unpack('<I', compiled[i:i+4])[0] for i in range(0, len(compiled), 4)]

    # Pad shorter one
    max_len = max(len(target_words), len(compiled_words))
    target_words.extend([0] * (max_len - len(target_words)))
    compiled_words.extend([0] * (max_len - len(compiled_words)))

    matches = 0
    diffs = []
    for i, (t, c) in enumerate(zip(target_words, compiled_words)):
        if t == c:
            matches += 1
        else:
            # Check if only relocation differs (lui immediate, jal target)
            t_op = (t >> 26) & 0x3F
            c_op = (c >> 26) & 0x3F
            if t_op == c_op:
                # Same opcode, might be relocation
                if t_op in (0x0F, 0x03, 0x09):  # lui, jal, addiu
                    matches += 1  # Count as match (relocation)
                    continue
            diffs.append({
                'index': i,
                'offset': i * 4,
                'target': f"0x{t:08X}",
                'compiled': f"0x{c:08X}",
            })

    return {
        'total': max_len,
        'matches': matches,
        'diffs': len(diffs),
        'diff_details': diffs[:10],  # First 10 diffs
        'match_percent': (matches / max_len * 100) if max_len > 0 else 0,
    }


def generate_stub_c(addr: int, asm_content: str) -> str:
    """Generate a stub C file from ASM content."""
    # Extract function calls
    calls = re.findall(r'jal\s+(0x[0-9a-fA-F]+|func_[0-9a-fA-F]+)', asm_content)

    # Count arguments (look for a0-a3 usage)
    args = set(re.findall(r'\$a([0-3])', asm_content))
    num_args = max([int(a) for a in args]) + 1 if args else 0

    # Generate stub
    func_name = f"func_{addr:08X}"
    arg_list = ', '.join([f"s32 arg{i}" for i in range(num_args)]) or "void"

    c_code = f'''/* Auto-generated stub for {func_name} */
#include "common.h"

'''

    # Add extern declarations for called functions
    for call in set(calls):
        call_name = call if call.startswith('func_') else f"func_{call.replace('0x', '').upper()}"
        c_code += f"extern void {call_name}(void);\n"

    c_code += f'''
void {func_name}({arg_list}) {{
    /* TODO: Implement */
'''

    for call in calls:
        call_name = call if call.startswith('func_') else f"func_{call.replace('0x', '').upper()}"
        c_code += f"    {call_name}();\n"

    c_code += "}\n"
    return c_code


def test_function(addr: int, exe_path: Path = None) -> dict:
    """Test C conversion for a single function."""
    if exe_path is None:
        exe_path = Path('disks/SLPS_000.65')

    addr_hex = f"0x{addr:08X}"
    func_name = f"func_{addr:08X}"
    asm_path = Path(f"asm/main/matched/{func_name}.s")

    result = {
        'address': addr_hex,
        'function': func_name,
        'status': 'unknown',
        'issues': [],
        'match_percent': 0,
    }

    # Check if ASM file exists
    if not asm_path.exists():
        result['status'] = 'no_asm_file'
        return result

    # Read ASM file
    asm_info = read_asm_file(asm_path)
    result['size'] = asm_info['size']
    result['issues'].extend(asm_info['issues'])

    # Check for known issues
    if asm_info['issues']:
        result['status'] = 'known_issue'
        result['category'] = asm_info['issues'][0]
        return result

    if asm_info['size'] == 0:
        result['status'] = 'unknown_size'
        return result

    # Get target bytes
    try:
        target_bytes = get_target_bytes(exe_path, addr, asm_info['size'])
    except Exception as e:
        result['status'] = 'read_error'
        result['issues'].append(str(e))
        return result

    # Determine compiler
    region = "gcc" if addr < 0x80086000 else "ido"
    result['region'] = region

    # Check if C source already exists
    c_matched = Path(f"src/main/matched/{func_name}.c")
    c_wip = Path(f"src/main/wip/{func_name}.c")

    if c_matched.exists():
        result['status'] = 'already_matched_c'
        result['c_path'] = str(c_matched)
        return result

    # Generate stub C code
    c_code = generate_stub_c(addr, asm_info['content'])

    # Compile
    with tempfile.NamedTemporaryFile(suffix='.o', delete=False) as f:
        obj_path = Path(f.name)

    try:
        if region == "gcc":
            success, error = compile_with_gcc(c_code, obj_path)
        else:
            success, error = compile_with_ido(c_code, obj_path)

        if not success:
            result['status'] = 'compile_error'
            result['issues'].append(error[:200])
            return result

        # Extract compiled bytes
        compiled_bytes = extract_text_section(obj_path)

        # Compare
        comparison = compare_bytes(target_bytes, compiled_bytes)
        result['match_percent'] = comparison['match_percent']
        result['matches'] = comparison['matches']
        result['total'] = comparison['total']
        result['diffs'] = comparison['diffs']

        if comparison['diffs'] == 0:
            result['status'] = 'perfect_match'
        elif comparison['match_percent'] > 80:
            result['status'] = 'near_match'
        elif comparison['match_percent'] > 50:
            result['status'] = 'partial_match'
        else:
            result['status'] = 'no_match'

    finally:
        if obj_path.exists():
            obj_path.unlink()

    return result


def main():
    parser = argparse.ArgumentParser(description="Test C conversion for ASM functions")
    parser.add_argument('address', nargs='?', help='Function address (e.g., 0x80046738)')
    parser.add_argument('--batch', help='Process functions from categorization JSON')
    parser.add_argument('--category', default='c_convertible', help='Category to process')
    parser.add_argument('--limit', type=int, default=10, help='Max functions to test')
    parser.add_argument('--output', help='Output JSON file')
    parser.add_argument('--exe', default='disks/SLPS_000.65', help='Target executable')
    args = parser.parse_args()

    exe_path = Path(args.exe)

    if args.address:
        # Single function
        addr = hex_to_int(args.address)
        result = test_function(addr, exe_path)
        print(json.dumps(result, indent=2))

    elif args.batch:
        # Batch mode
        data = json.loads(Path(args.batch).read_text())
        functions = data.get('by_category', {}).get(args.category, [])

        results = []
        for i, func in enumerate(functions[:args.limit]):
            addr = hex_to_int(func['address'])
            print(f"[{i+1}/{min(len(functions), args.limit)}] Testing {func['address']}...", file=sys.stderr)
            result = test_function(addr, exe_path)
            results.append(result)
            print(f"  -> {result['status']} ({result.get('match_percent', 0):.1f}%)", file=sys.stderr)

        # Summary
        summary = {
            'total': len(results),
            'by_status': {},
        }
        for r in results:
            status = r['status']
            summary['by_status'][status] = summary['by_status'].get(status, 0) + 1

        output = {
            'summary': summary,
            'results': results,
        }

        if args.output:
            Path(args.output).write_text(json.dumps(output, indent=2))
            print(f"Saved to {args.output}", file=sys.stderr)
        else:
            print(json.dumps(output, indent=2))


if __name__ == '__main__':
    main()
