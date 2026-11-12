#!/usr/bin/env python3
"""
Create target.o from extracted assembly for permuter
Converts assembly to .word format and assembles it
"""

import sys
import subprocess
import re
from pathlib import Path

def asm_to_word_format(asm_file, output_s):
    """Convert assembly to .word format"""
    with open(asm_file, 'r') as f:
        lines = f.readlines()
    
    # Find function name
    func_name = None
    for line in lines:
        if '.globl' in line or 'func_' in line:
            match = re.search(r'func_[0-9A-Fa-f]+', line)
            if match:
                func_name = match.group(0)
                break
    
    if not func_name:
        print("ERROR: Could not find function name in assembly")
        return False
    
    # Extract hex instructions
    instructions = []
    for line in lines:
        # Look for hex addresses and instructions
        # Format: "addiu sp,sp,-40 # 8008669c"
        parts = line.strip().split('#')
        if len(parts) >= 1:
            instr = parts[0].strip()
            if instr and not instr.startswith('.') and not instr.startswith(func_name + ':'):
                # Extract hex from comment if present
                if len(parts) >= 2:
                    hex_match = re.search(r'[0-9a-fA-F]{8}', parts[1])
                    if hex_match:
                        instructions.append(hex_match.group(0).upper())
    
    # If no hex found, try to assemble the instructions
    if not instructions:
        print(f"No hex instructions found, will use original assembly")
        return False
    
    # Write .word format
    with open(output_s, 'w') as f:
        f.write('.set noreorder\n')
        f.write('.set noat\n\n')
        f.write(f'.globl {func_name}\n')
        f.write(f'{func_name}:\n')
        for hex_instr in instructions:
            f.write(f'    .word 0x{hex_instr}\n')
    
    print(f"Created {output_s} with {len(instructions)} instructions")
    return True

def assemble_target(input_s, output_o):
    """Assemble target.s to target.o"""
    cmd = ['mipsel-linux-gnu-as', '-march=mips1', '-mabi=32', input_s, '-o', output_o]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"ERROR assembling: {result.stderr}")
        return False
    return True

def main():
    if len(sys.argv) < 3:
        print("Usage: create_target_obj.py input.txt output.o")
        print("  Converts extracted assembly to target.o for permuter")
        sys.exit(1)
    
    input_file = sys.argv[1]
    output_o = sys.argv[2]
    
    # Create intermediate .word format
    temp_s = output_o.replace('.o', '_temp.s')
    
    if not asm_to_word_format(input_file, temp_s):
        print("Failed to convert to .word format")
        sys.exit(1)
    
    if not assemble_target(temp_s, output_o):
        print("Failed to assemble target")
        sys.exit(1)
    
    print(f"Successfully created {output_o}")

if __name__ == '__main__':
    main()
