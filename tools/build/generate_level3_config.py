#!/usr/bin/env python3
"""Generate level3_functions.json from matched C and ASM files."""

import json
import os
import re
import sys
from pathlib import Path

def extract_address_from_filename(filename):
    """Extract function address from filename like device_cmd_send.c or FUN_8004523C.c"""
    # Match func_XXXXXXXX or FUN_XXXXXXXX (case insensitive)
    match = re.search(r'(?:func|FUN)_([0-9A-Fa-f]{8})', filename, re.IGNORECASE)
    if match:
        return int(match.group(1), 16)
    return None

def extract_address_from_file_header(filepath):
    """Extract function address from file header comments (for symbol-named files)."""
    try:
        with open(filepath, 'r') as f:
            header = f.read(2000)
        # Pattern: "Address: 0x80072BE0" or "Address: 0x80072BE0 - 0x80072C18"
        match = re.search(r'Address:\s*(0x[0-9A-Fa-f]{8})', header)
        if match:
            return int(match.group(1), 16)
        # Pattern: ".globl device_load_wait" (ASM files)
        match = re.search(r'\.globl\s+func_([0-9A-Fa-f]{8})', header)
        if match:
            return int(match.group(1), 16)
    except Exception:
        pass
    return None

def determine_region(address):
    """Determine GCC or IDO region based on address (boundary at 0x80086000)."""
    if 0x80041000 <= address <= 0x80085FFF:
        return "GCC"
    elif 0x80086000 <= address <= 0x800BFFFF:
        return "IDO"
    return "UNKNOWN"

def parse_size_from_c_file(filepath):
    """Parse size from C file header comments."""
    try:
        with open(filepath, 'r') as f:
            content = f.read(2000)  # Only read first 2KB for header

        # Pattern 1: "Address: 0x80044980 - 0x800449A4 (36 bytes)"
        match = re.search(r'Address:\s*0x[0-9A-Fa-f]+\s*-\s*0x[0-9A-Fa-f]+\s*\((\d+)\s*bytes?\)', content)
        if match:
            return int(match.group(1))

        # Pattern 2: "Size: 64 bytes"
        match = re.search(r'Size:\s*(\d+)\s*bytes?', content)
        if match:
            return int(match.group(1))

        # Pattern 3: "(N bytes)" anywhere
        match = re.search(r'\((\d+)\s*bytes?\)', content)
        if match:
            return int(match.group(1))

        # Pattern 4: "N instructions" -> N * 4
        match = re.search(r'(\d+)\s*instructions?\)', content)
        if match:
            return int(match.group(1)) * 4

        # Pattern 5: "- N bytes" (like "tpage_buf_clear - 88 bytes")
        match = re.search(r'-\s*(\d+)\s*bytes?', content)
        if match:
            return int(match.group(1))

    except Exception as e:
        print(f"Error parsing {filepath}: {e}", file=sys.stderr)

    return None

def parse_size_from_asm_file(filepath):
    """Parse size from ASM file by counting instructions or header comments."""
    try:
        with open(filepath, 'r') as f:
            content = f.read()
        
        # First try header patterns
        # Pattern 1: "Size: 84 bytes (21 instructions)"
        match = re.search(r'Size:\s*(\d+)\s*bytes?', content)
        if match:
            return int(match.group(1))
        
        # Pattern 2: "(N bytes)"
        match = re.search(r'\((\d+)\s*bytes?\)', content[:1000])
        if match:
            return int(match.group(1))
        
        # Count actual instructions as fallback
        # MIPS instructions are 4 bytes each
        instruction_count = 0
        in_text = False
        for line in content.split('\n'):
            stripped = line.strip()
            
            # Skip empty lines and comments
            if not stripped or stripped.startswith('#') or stripped.startswith('/*') or stripped.startswith('*'):
                continue
            
            # Track .text section
            if stripped.startswith('.text'):
                in_text = True
                continue
            
            # Skip directives
            if stripped.startswith('.'):
                continue
            
            # Skip labels
            if stripped.endswith(':'):
                continue
            
            # Count instruction lines (have actual opcodes)
            if in_text:
                # Match instruction patterns
                if re.match(r'\s*(addiu|addu|add|sub|subu|and|andi|or|ori|xor|xori|nor|sll|srl|sra|sllv|srlv|srav|lui|lw|sw|lb|sb|lh|sh|lbu|lhu|beq|bne|bgez|bgtz|blez|bltz|bnez|beqz|b\b|bnezl|beqzl|j|jal|jr|jalr|move|negu|neg|not|nop|li|la|mflo|mfhi|mtlo|mthi|mult|multu|div|divu|slt|slti|sltu|sltiu|sync|break|syscall|tge|tgeu|tlt|tltu|teq|tne|lwl|lwr|swl|swr|ll|sc|cop0|cop1|cop2|mfc0|mtc0|mfc1|mtc1|cvt|bc1f|bc1t|abs|floor|ceil|round|trunc|sqrt)\s+', stripped, re.IGNORECASE):
                    instruction_count += 1
        
        if instruction_count > 0:
            return instruction_count * 4
            
    except Exception as e:
        print(f"Error parsing {filepath}: {e}", file=sys.stderr)
    
    return None

def main():
    project_root = Path('.')
    c_dir = project_root / 'src/main/matched'
    asm_dir = project_root / 'asm/main/matched'
    output_file = project_root / 'config/level3_functions.json'
    
    functions = []
    seen_addresses = set()
    
    # Process C files
    c_files = sorted(c_dir.glob('*.c'))
    print(f"Found {len(c_files)} C files")
    
    for cfile in c_files:
        # Prefer header address over filename (cross-prologue functions
        # have thunk address in header that differs from filename)
        addr = extract_address_from_file_header(cfile)
        if addr is None:
            addr = extract_address_from_filename(cfile.name)
        if addr is None:
            print(f"Warning: Could not extract address from {cfile.name}", file=sys.stderr)
            continue
        
        if addr in seen_addresses:
            print(f"Warning: Duplicate address 0x{addr:08X} from {cfile.name}", file=sys.stderr)
            continue
        
        size = parse_size_from_c_file(cfile)
        if size is None:
            print(f"Warning: Could not extract size from {cfile.name}", file=sys.stderr)
            size = 0  # Use 0 as placeholder
        
        region = determine_region(addr)
        func_name = cfile.stem
        
        functions.append({
            "name": func_name,
            "address": f"0x{addr:08X}",
            "size": size,
            "source": f"src/main/matched/{cfile.name}",
            "region": region,
            "match_status": "verified"
        })
        seen_addresses.add(addr)
    
    # Process ASM files
    asm_files = sorted(asm_dir.glob('*.s'))
    print(f"Found {len(asm_files)} ASM files")
    
    for asmfile in asm_files:
        addr = extract_address_from_filename(asmfile.name)
        if addr is None:
            addr = extract_address_from_file_header(asmfile)
        if addr is None:
            print(f"Warning: Could not extract address from {asmfile.name}", file=sys.stderr)
            continue
        
        if addr in seen_addresses:
            # C file takes precedence
            continue
        
        size = parse_size_from_asm_file(asmfile)
        if size is None:
            print(f"Warning: Could not extract size from {asmfile.name}", file=sys.stderr)
            size = 0  # Use 0 as placeholder
        
        region = determine_region(addr)
        func_name = asmfile.stem
        
        functions.append({
            "name": func_name,
            "address": f"0x{addr:08X}",
            "size": size,
            "source": f"asm/main/matched/{asmfile.name}",
            "region": region,
            "match_status": "verified"
        })
        seen_addresses.add(addr)
    
    # Sort by address
    functions.sort(key=lambda f: int(f["address"], 16))
    
    # Create output structure
    output = {
        "version": "3.0",
        "description": "Level 3 verified matched functions",
        "generated_by": "tools/build/generate_level3_config.py",
        "total_functions": len(functions),
        "c_functions": len([f for f in functions if f["source"].endswith(".c")]),
        "asm_functions": len([f for f in functions if f["source"].endswith(".s")]),
        "functions": functions
    }
    
    # Write output
    with open(output_file, 'w') as f:
        json.dump(output, f, indent=2)
    
    print(f"Generated {output_file}")
    print(f"  Total: {output['total_functions']} functions")
    print(f"  C files: {output['c_functions']}")
    print(f"  ASM files: {output['asm_functions']}")

if __name__ == '__main__':
    main()
