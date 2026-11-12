#!/usr/bin/env python3
"""Fast single-function byte-match verifier.

Compiles ONE C file with its PostProc chain, extracts .text,
compares against the target bytes in SLPS_000.65.
Does NOT rebuild the entire project.

Usage:
    python3 tools/analysis/verify_single_func.py func_XXXXXXXX
    python3 tools/analysis/verify_single_func.py func_XXXXXXXX -c path/to/source.c
"""
import sys, os, json, struct, subprocess, hashlib, tempfile, re

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} FUNC_NAME [-c source.c]")
        sys.exit(1)

    func_name = sys.argv[1]
    source = None
    if '-c' in sys.argv:
        idx = sys.argv.index('-c')
        source = sys.argv[idx + 1]

    # Find source file
    if source is None:
        for d in ['src/main/matched', 'src/main/semi-matched', 'src/main/wip']:
            p = os.path.join(d, f'{func_name}.c')
            if os.path.exists(p):
                source = p
                break

    if source is None or not os.path.exists(source):
        print(f"Error: No source file found for {func_name}")
        sys.exit(1)

    # Get function info from level3_functions.json
    config_path = 'config/level3_functions.json'
    if os.path.exists(config_path):
        with open(config_path) as f:
            funcs = json.load(f)
        if func_name in funcs:
            info = funcs[func_name]
            addr = info.get('address', 0)
            if isinstance(addr, str): addr = int(addr, 16)
            size = info.get('size', 0)
            if isinstance(size, str): size = int(size, 0)
        else:
            print(f"Warning: {func_name} not in level3_functions.json, parsing from source")
            addr, size = parse_header(source)
    else:
        addr, size = parse_header(source)

    if addr == 0 or size == 0:
        print(f"Error: Could not determine address/size for {func_name}")
        sys.exit(1)

    print(f"Function: {func_name}")
    print(f"Source:   {source}")
    print(f"Address:  {hex(addr)}")
    print(f"Size:     {size} bytes ({size//4} instructions)")

    # Extract target bytes
    exe_path = 'disks/SLPS_000.65'
    if not os.path.exists(exe_path):
        print(f"Error: {exe_path} not found")
        sys.exit(1)

    offset = (addr - 0x80041000) + 0x800
    with open(exe_path, 'rb') as f:
        f.seek(offset)
        target_bytes = f.read(size)

    # Compile using ido_diff.py infrastructure
    print(f"\nCompiling {source}...")
    result = subprocess.run(
        ['python3', 'tools/decomp/ido_diff.py', func_name, '-c', source, '--raw-bytes'],
        capture_output=True, text=True, timeout=60
    )

    if result.returncode != 0:
        # Fallback: just report the diff
        result2 = subprocess.run(
            ['python3', 'tools/decomp/ido_diff.py', func_name, '-c', source],
            capture_output=True, text=True, timeout=60
        )
        print(result2.stdout[-500:] if len(result2.stdout) > 500 else result2.stdout)
        print(result2.stderr[-500:] if len(result2.stderr) > 500 else result2.stderr)
        sys.exit(1)

    print(result.stdout[-500:] if len(result.stdout) > 500 else result.stdout)


def parse_header(source):
    """Extract address and size from C file header comments."""
    addr = 0
    size = 0
    with open(source) as f:
        for line in f:
            if line.startswith(' */') or line.startswith('*/'):
                break
            m = re.search(r'Address:\s*(0x[0-9a-fA-F]+)', line)
            if m: addr = int(m.group(1), 16)
            m = re.search(r'Size:\s*(\d+)\s*bytes', line)
            if m: size = int(m.group(1))
    return addr, size


if __name__ == '__main__':
    main()
