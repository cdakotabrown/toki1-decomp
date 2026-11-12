#!/usr/bin/env python3
"""
build_all_c.py - Build binary using ALL C sources (matched + WIP) plus ASM-required files.

This is the "C-only" build mode. It compiles every function from C source,
falling back to the original binary bytes only for functions that are
categorically impossible to express in C (unaligned access, hi/lo register).

The output binary will NOT match the original SHA1 (WIP functions produce
different code), but it builds entirely from C source — the goal being to
track decompilation progress by eliminating ASM files.

Usage:
    python3 tools/build/build_all_c.py
    python3 tools/build/build_all_c.py --verbose
    python3 tools/build/build_all_c.py --stats  # just show stats, don't build
"""

import argparse
import hashlib
import json
import os
import shutil
import struct
import subprocess
import sys
import tempfile
from concurrent.futures import ProcessPoolExecutor, as_completed
from pathlib import Path

ROOT_DIR = Path(__file__).parent.parent.parent
EXE_HEADER_SIZE = 0x800
TEXT_LOAD_ADDR = 0x80041000
GAME_REGION_END = 0x80086000
EXPECTED_SHA1 = "2d8d474816e4bf937b09dc3b017f550f7ce4e14b"

IDO_CC = ROOT_DIR / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
ORIGINAL_EXE = ROOT_DIR / "disks" / "SLPS_000.65"
LEVEL3_CONFIG = ROOT_DIR / "config" / "level3_functions.json"
CLASSIFICATION = ROOT_DIR / "config" / "function_classification.json"

MATCHED_C_DIR = ROOT_DIR / "src" / "main" / "matched"
WIP_C_DIR = ROOT_DIR / "src" / "main" / "wip"
ASM_DIR = ROOT_DIR / "asm" / "main" / "matched"

OUTPUT_DIR = ROOT_DIR / "build" / "jp" / "c_only"

# Post-processing tools
FRAME_PATCHER = ROOT_DIR / "tools" / "ido" / "patch_stack_complete.py"
OR_TO_ADDU = ROOT_DIR / "tools" / "ido" / "convert_or_to_addu.py"


def addr_to_offset(addr):
    return EXE_HEADER_SIZE + (addr - TEXT_LOAD_ADDR)


def load_functions():
    """Load function list from level3 config."""
    with open(LEVEL3_CONFIG) as f:
        data = json.load(f)
    return data['functions']


def find_c_source(func_name):
    """Find C source for a function. Matched takes priority over WIP."""
    matched = MATCHED_C_DIR / f"{func_name}.c"
    if matched.exists():
        return matched, "matched"
    wip = WIP_C_DIR / f"{func_name}.c"
    if wip.exists():
        return wip, "wip"
    return None, None


def load_compiler_overrides(path):
    """Load per-function compiler overrides from JSON.

    Format:
    {
      "iris_show": {
        "cc": "/abs/or/rel/path/to/cc",
        "env": {"KEY": "VALUE"},
        "extra_flags": ["-Wo,-loopunroll,0"]
      }
    }
    """
    if not path:
        return {}

    with open(path, "r", encoding="utf-8") as f:
        data = json.load(f)

    if not isinstance(data, dict):
        raise ValueError("compiler override JSON must be an object keyed by function name")

    overrides = {}
    for func_name, cfg in data.items():
        if not isinstance(cfg, dict):
            raise ValueError(f"override for {func_name} must be an object")

        cc = cfg.get("cc")
        env = cfg.get("env", {})
        extra_flags = cfg.get("extra_flags", [])

        if cc is not None and not isinstance(cc, str):
            raise ValueError(f"override {func_name}.cc must be a string path")
        if not isinstance(env, dict) or not all(isinstance(k, str) and isinstance(v, str) for k, v in env.items()):
            raise ValueError(f"override {func_name}.env must be an object with string keys/values")
        if not isinstance(extra_flags, list) or not all(isinstance(x, str) for x in extra_flags):
            raise ValueError(f"override {func_name}.extra_flags must be a list of strings")

        overrides[func_name] = {
            "cc": cc,
            "env": env,
            "extra_flags": extra_flags,
        }

    return overrides


def resolve_compiler_settings(name, compiler_overrides):
    """Resolve compiler path/env/flags for a function."""
    cfg = (compiler_overrides or {}).get(name, {})

    cc = Path(cfg.get("cc")) if cfg.get("cc") else IDO_CC
    if not cc.is_absolute():
        cc = (ROOT_DIR / cc).resolve()

    env = os.environ.copy()
    env.update(cfg.get("env", {}))
    extra_flags = cfg.get("extra_flags", [])

    return cc, env, extra_flags


def compile_function(func, tmp_dir, compiler_overrides=None):
    """
    Compile a single function and return its .text bytes.
    Returns (func_name, bytes_or_None, source_type, error_msg).
    """
    name = func['name']
    addr = int(func['address'], 16)
    size = func['size']
    is_game = addr < GAME_REGION_END

    src_path, src_type = find_c_source(name)
    if src_path is None:
        return name, None, "asm_fallback", "no C source"

    cc, cc_env, extra_flags = resolve_compiler_settings(name, compiler_overrides)
    if not cc.exists():
        return name, None, "compile_error", f"compiler not found: {cc}"

    obj_path = os.path.join(tmp_dir, f"{name}.o")
    pp_path = os.path.join(tmp_dir, f"{name}_pp.o")

    # Compile
    cmd = [
        str(cc), "-O2",
    ]
    if extra_flags:
        cmd.extend(extra_flags)
    cmd.extend([
        "-G0", "-mips1", "-EL", "-non_shared",
        "-I", str(ROOT_DIR / "include"),
        "-c", str(src_path), "-o", obj_path
    ])
    r = subprocess.run(cmd, capture_output=True, text=True, timeout=60, env=cc_env)
    if r.returncode != 0:
        err = r.stderr[:200] if r.stderr else "compile failed"
        return name, None, "compile_error", err

    # Post-process based on region
    if is_game:
        # Patch frame size
        cmd = [
            "python3", str(FRAME_PATCHER),
            obj_path, pp_path, "--old", "24", "--new", "40"
        ]
        r = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
        if r.returncode != 0:
            shutil.copy(obj_path, pp_path)
    else:
        # Convert or to addu
        cmd = [
            "python3", str(OR_TO_ADDU),
            obj_path, pp_path
        ]
        r = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
        if r.returncode != 0:
            shutil.copy(obj_path, pp_path)

    # Extract .text section
    bin_path = os.path.join(tmp_dir, f"{name}.bin")
    cmd = [
        "mipsel-linux-gnu-objcopy", "-O", "binary", "-j", ".text",
        pp_path, bin_path
    ]
    r = subprocess.run(cmd, capture_output=True, timeout=30)
    if r.returncode != 0 or not os.path.exists(bin_path):
        return name, None, "extract_error", "objcopy failed"

    with open(bin_path, 'rb') as f:
        code = f.read()

    return name, code, src_type, ""


def main():
    parser = argparse.ArgumentParser(
        description="Build binary using all C sources (matched + WIP)"
    )
    parser.add_argument('-v', '--verbose', action='store_true')
    parser.add_argument('--stats', action='store_true',
                        help='Just show stats, do not build')
    parser.add_argument('-p', '--parallel', type=int, default=8)
    parser.add_argument('--cc-overrides', type=str, default=None,
                        help='JSON file mapping function names to compiler/env overrides')
    args = parser.parse_args()

    compiler_overrides = {}
    if args.cc_overrides:
        try:
            compiler_overrides = load_compiler_overrides(args.cc_overrides)
        except Exception as e:
            print(f"ERROR: failed to load --cc-overrides: {e}")
            return 1

        print(f"Compiler overrides loaded: {len(compiler_overrides)} function(s)")
        if args.verbose:
            for func_name, cfg in sorted(compiler_overrides.items()):
                cc = cfg.get("cc") or str(IDO_CC)
                env_keys = ",".join(sorted(cfg.get("env", {}).keys())) or "-"
                flags = " ".join(cfg.get("extra_flags", [])) or "-"
                print(f"  {func_name}: cc={cc} env={env_keys} flags={flags}")

    # Load functions
    functions = load_functions()
    print(f"Total functions: {len(functions)}")

    # Categorize
    has_matched_c = 0
    has_wip_c = 0
    asm_only = 0
    for func in functions:
        src, stype = find_c_source(func['name'])
        if stype == "matched":
            has_matched_c += 1
        elif stype == "wip":
            has_wip_c += 1
        else:
            asm_only += 1

    print(f"  Matched C:  {has_matched_c}")
    print(f"  WIP C:      {has_wip_c}")
    print(f"  ASM-only:   {asm_only} (will use original bytes)")
    print(f"  Total C:    {has_matched_c + has_wip_c} ({100*(has_matched_c+has_wip_c)/len(functions):.1f}%)")

    if args.stats:
        return

    # Load original binary
    with open(ORIGINAL_EXE, 'rb') as f:
        exe_data = bytearray(f.read())

    output_data = bytearray(exe_data)  # start from original

    # Create output directory
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

    # Compile all C functions
    print(f"\nCompiling {has_matched_c + has_wip_c} C functions...")

    stats = {"matched_ok": 0, "wip_ok": 0, "compile_error": 0,
             "asm_fallback": 0, "patched": 0}

    with tempfile.TemporaryDirectory(prefix="build_all_c_") as tmp_dir:
        # Process functions
        for i, func in enumerate(functions, 1):
            name = func['name']
            addr = int(func['address'], 16)
            size = func['size']

            if size == 0:
                stats['asm_fallback'] += 1
                continue

            src_path, src_type = find_c_source(name)
            if src_path is None:
                stats['asm_fallback'] += 1
                continue

            fname, code, stype, err = compile_function(func, tmp_dir, compiler_overrides)

            if code is not None:
                # Patch into binary (truncate or pad to match original size)
                offset = addr_to_offset(addr)
                if len(code) <= size:
                    output_data[offset:offset + len(code)] = code
                    # Pad remainder with nops if shorter
                    for j in range(len(code), size, 4):
                        output_data[offset + j:offset + j + 4] = b'\x00\x00\x00\x00'
                else:
                    # Compiled code is larger — just use what fits
                    output_data[offset:offset + size] = code[:size]

                if stype == "matched":
                    stats['matched_ok'] += 1
                else:
                    stats['wip_ok'] += 1
                stats['patched'] += 1
            else:
                if stype == "compile_error":
                    stats['compile_error'] += 1
                else:
                    stats['asm_fallback'] += 1

                if args.verbose and err:
                    print(f"  {fname}: {err[:80]}")

            if i % 100 == 0:
                print(f"  [{i}/{len(functions)}] compiled...")

    # Write output
    output_bin = OUTPUT_DIR / "SLPS_000.65.new"
    with open(output_bin, 'wb') as f:
        f.write(output_data)

    # Check SHA1
    sha1 = hashlib.sha1(output_data).hexdigest()

    print(f"\n{'=' * 60}")
    print(f"C-ONLY BUILD COMPLETE")
    print(f"{'=' * 60}")
    print(f"  Output:         {output_bin}")
    print(f"  Size:           {len(output_data)} bytes")
    print(f"  SHA1:           {sha1}")
    print(f"  Expected SHA1:  {EXPECTED_SHA1}")
    print(f"  Match:          {'YES' if sha1 == EXPECTED_SHA1 else 'NO'}")
    print(f"")
    print(f"  Matched C OK:   {stats['matched_ok']}")
    print(f"  WIP C OK:       {stats['wip_ok']}")
    print(f"  Compile errors: {stats['compile_error']}")
    print(f"  ASM fallback:   {stats['asm_fallback']}")
    print(f"  Total patched:  {stats['patched']}")
    print(f"")

    # Compare against original
    orig_sha1 = hashlib.sha1(exe_data).hexdigest()
    diff_bytes = sum(1 for a, b in zip(exe_data, output_data) if a != b)
    print(f"  Bytes changed:  {diff_bytes:,} / {len(exe_data):,} ({100*diff_bytes/len(exe_data):.2f}%)")


if __name__ == "__main__":
    main()
