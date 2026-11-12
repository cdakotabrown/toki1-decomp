#!/usr/bin/env python3
"""
asm_to_m2c.py - Convert our ASM format to m2c-compatible format

Our ASM files have:
- Block /* */ comments at the top (m2c chokes on these)
- .global instead of glabel
- Branch targets as raw hex (0x8004e998)
- Address comments at end of lines (# 8004e930)
- No labels at branch targets

m2c expects:
- No leading block comments
- glabel instead of .global
- Branch targets as labels (.L8004e998)
- Labels at branch target addresses
"""

import argparse
import re
import sys
from pathlib import Path


def convert_asm_to_m2c(content: str) -> str:
    """Convert our ASM format to m2c-compatible format"""
    lines = content.split('\n')

    # First pass: collect addresses and branch targets
    addr_to_idx = {}  # Map address -> line index
    branch_targets = set()

    # Pattern for address comment at end of line: # 8004e930 or # 0x8004e930
    addr_comment_pattern = re.compile(r'#\s*(?:0x)?([0-9a-fA-F]{8})\s*$')

    # Pattern for branch instructions with hex targets
    branch_pattern = re.compile(
        r'\b(b|beq|bne|bgez|bgtz|blez|bltz|beqz|bnez|bnel|beql|bgezl|bltzl|bgtzl|blezl|bge|blt|bgt|ble)\s+(.+?),?\s*0x([0-9a-fA-F]+)',
        re.IGNORECASE
    )
    # Pattern for unconditional branch
    uncond_branch_pattern = re.compile(r'\bb\s+0x([0-9a-fA-F]+)', re.IGNORECASE)

    for i, line in enumerate(lines):
        # Extract address from comment
        addr_match = addr_comment_pattern.search(line)
        if addr_match:
            addr = addr_match.group(1).lower()
            addr_to_idx[addr] = i

        # Find branch targets
        match = branch_pattern.search(line)
        if match:
            target = match.group(3).lower()
            branch_targets.add(target)
        else:
            match = uncond_branch_pattern.search(line)
            if match:
                target = match.group(1).lower()
                branch_targets.add(target)

    # Second pass: convert and insert labels
    output_lines = []
    in_block_comment = False
    found_first_directive = False

    for i, line in enumerate(lines):
        # Skip leading block comments
        if not found_first_directive:
            if '/*' in line and '*/' not in line:
                in_block_comment = True
                continue
            if in_block_comment:
                if '*/' in line:
                    in_block_comment = False
                continue
            if line.strip().startswith('/*') and line.strip().endswith('*/'):
                continue
            if line.strip().startswith('#') and not line.strip().startswith('#include'):
                # Skip header comments but check if this line has an address
                addr_match = addr_comment_pattern.search(line)
                if not addr_match:
                    continue
            if not line.strip():
                continue

        if line.strip().startswith('.'):
            found_first_directive = True

        # Check if we need to insert a label before this line
        addr_match = addr_comment_pattern.search(line)
        if addr_match:
            addr = addr_match.group(1).lower()
            if addr in branch_targets:
                output_lines.append(f'.L{addr}:')

        # Convert .global/.globl to glabel
        if re.match(r'\s*\.globa?l\s+', line):
            match = re.match(r'\s*\.globa?l\s+(\S+)', line)
            if match:
                func_name = match.group(1)
                output_lines.append(f'glabel {func_name}')
                continue

        # Skip function entry label (glabel handles it)
        if ':' in line and not line.strip().startswith('.L'):
            match = re.match(r'^(\w+):\s*(.*)', line.strip())
            if match:
                if match.group(2):
                    output_lines.append(f'  {match.group(2)}')
                continue

        # Convert branch targets from 0xADDR to .LADDR
        new_line = line

        # Handle conditional branches: bXX reg,0xADDR or bXX reg,reg,0xADDR
        def replace_cond_branch(m):
            instr = m.group(1)
            args = m.group(2)
            target = m.group(3).lower()
            return f'{instr} {args},.L{target}'

        new_line = branch_pattern.sub(replace_cond_branch, new_line)

        # Handle unconditional branch: b 0xADDR
        def replace_uncond_branch(m):
            target = m.group(1).lower()
            return f'b .L{target}'

        new_line = uncond_branch_pattern.sub(replace_uncond_branch, new_line)

        # Remove address comments (they confuse m2c sometimes)
        new_line = addr_comment_pattern.sub('', new_line).rstrip()

        output_lines.append(new_line)

    # Join and clean up
    result = '\n'.join(output_lines)
    result = re.sub(r'\n{3,}', '\n\n', result)

    return result


def main():
    parser = argparse.ArgumentParser(description='Convert ASM to m2c format')
    parser.add_argument('input', help='Input ASM file')
    parser.add_argument('-o', '--output', help='Output file (default: stdout)')
    args = parser.parse_args()

    input_path = Path(args.input)
    if not input_path.exists():
        print(f"Error: {input_path} not found", file=sys.stderr)
        sys.exit(1)

    content = input_path.read_text()
    converted = convert_asm_to_m2c(content)

    if args.output:
        Path(args.output).write_text(converted)
    else:
        print(converted)


if __name__ == '__main__':
    main()
