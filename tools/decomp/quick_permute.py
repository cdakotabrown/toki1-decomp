#!/usr/bin/env python3
"""
quick_permute.py - Token-efficient permuter wrapper for endless runs

Usage:
    python3 quick_permute.py func_XXXXXXXX [--iterations N] [--stop-on-zero]

Creates minimal permuter candidate dir and runs until match or limit.
"""

import argparse
import os
import subprocess
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).parent.parent.parent
PERMUTER_DIR = PROJECT_ROOT / "build" / "decomp-permuter"
EXE_PATH = PROJECT_ROOT / "disks" / "SLPS_000.65"


def get_function_info(func_name: str) -> tuple:
    """Get function address and size from function_map.json"""
    import json
    map_path = PROJECT_ROOT / "build" / "function_analysis" / "function_map.json"
    if not map_path.exists():
        return None, None

    with open(map_path) as f:
        data = json.load(f)

    for func in data.get("functions", []):
        if func.get("name") == func_name:
            addr = int(func.get("vram_start", "0"), 16)
            end = int(func.get("vram_end", "0"), 16)
            return addr, end - addr
    return None, None


def extract_target_asm(func_name: str, addr: int, size: int) -> str:
    """Extract target assembly from binary"""
    import struct

    file_offset = addr - 0x80041000 + 0x800  # PSX-EXE load address
    asm_lines = [".set noreorder", ".set noat", "", f"glabel {func_name}"]

    with open(EXE_PATH, "rb") as f:
        f.seek(file_offset)
        for i in range(size // 4):
            word = struct.unpack("<I", f.read(4))[0]
            # Basic disassembly - just output hex for now
            asm_lines.append(f"    .word 0x{word:08X}")

    return "\n".join(asm_lines)


def get_wip_source(func_name: str) -> str:
    """Get WIP C source if exists"""
    wip_path = PROJECT_ROOT / "src" / "main" / "wip" / f"{func_name}.c"
    if wip_path.exists():
        with open(wip_path) as f:
            return f.read()
    return None


def setup_candidate_dir(func_name: str) -> Path:
    """Create minimal permuter candidate directory"""
    cand_dir = PERMUTER_DIR / func_name
    cand_dir.mkdir(exist_ok=True)

    addr, size = get_function_info(func_name)
    if not addr:
        print(f"Error: Function {func_name} not found in function_map.json")
        sys.exit(1)

    # Get or create base.c
    source = get_wip_source(func_name)
    if source:
        # Strip comments for token efficiency
        lines = []
        in_block = False
        for line in source.split("\n"):
            if "/*" in line and "*/" not in line:
                in_block = True
                continue
            if in_block:
                if "*/" in line:
                    in_block = False
                continue
            if line.strip().startswith("//"):
                continue
            if line.strip().startswith("*"):
                continue
            lines.append(line)
        source = "\n".join(lines)
        (cand_dir / "base.c").write_text(source)
    else:
        print(f"Warning: No WIP source for {func_name}")
        return None

    # Create target.s using objdump
    result = subprocess.run(
        ["python3", str(PROJECT_ROOT / "tools" / "analysis" / "extract_function_asm.py"),
         "--exe", str(EXE_PATH), "--addr", hex(addr), "--size", str(size)],
        capture_output=True, text=True
    )
    if result.returncode == 0:
        (cand_dir / "target.s").write_text(result.stdout)
    else:
        # Fallback: create basic target
        asm = extract_target_asm(func_name, addr, size)
        (cand_dir / "target.s").write_text(asm)

    # Create compile.sh (required by permuter)
    compile_sh = f'''#!/bin/bash
DIR="$(dirname "$0")"
ROOT="$DIR/../.."
"$ROOT/tools/ido-static-recomp/build/5.2/out/cc" -O2 -G0 -mips1 -EL -non_shared -I"$ROOT/include" "$@" 2>/dev/null
'''
    (cand_dir / "compile.sh").write_text(compile_sh)
    os.chmod(cand_dir / "compile.sh", 0o755)

    # Create settings.toml
    settings = '''[default]
compiler_path = "./compile.sh"
assembler_path = "mipsel-linux-gnu-as"
objdump_path = "mipsel-linux-gnu-objdump"
arch = "mips"
'''
    (cand_dir / "settings.toml").write_text(settings)

    return cand_dir


def run_permuter(func_name: str, iterations: int = 0, stop_on_zero: bool = True):
    """Run permuter on function"""
    cand_dir = setup_candidate_dir(func_name)
    if not cand_dir:
        return

    os.chdir(PERMUTER_DIR)

    cmd = ["python3", "permuter.py", "-j4", str(cand_dir)]
    if stop_on_zero:
        cmd.append("--stop-on-zero")
    if iterations > 0:
        cmd.extend(["--iterations", str(iterations)])

    print(f"Running: {' '.join(cmd)}")
    subprocess.run(cmd)


def main():
    parser = argparse.ArgumentParser(description="Quick permuter for Tokimeki")
    parser.add_argument("function", help="Function name (e.g., GsSetLsMatrix)")
    parser.add_argument("--iterations", "-n", type=int, default=0,
                        help="Max iterations (0=unlimited)")
    parser.add_argument("--stop-on-zero", "-z", action="store_true", default=True,
                        help="Stop when score reaches 0")
    parser.add_argument("--setup-only", action="store_true",
                        help="Only setup candidate dir, don't run")

    args = parser.parse_args()

    if args.setup_only:
        setup_candidate_dir(args.function)
        print(f"Created: {PERMUTER_DIR / args.function}")
    else:
        run_permuter(args.function, args.iterations, args.stop_on_zero)


if __name__ == "__main__":
    main()
