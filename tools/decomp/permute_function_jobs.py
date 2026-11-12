#!/usr/bin/env python3
"""
permute_function_jobs.py - decomp-permuter runner with configurable --jobs.

This is a thin wrapper around tools/decomp/permute_function.py that:
- Reuses its helper functions (DRY)
- Adds a --jobs option for permuter threads
"""

from __future__ import annotations

import argparse
import importlib.util
import shutil
import subprocess
import sys
from pathlib import Path
from typing import Optional

ROOT_DIR = Path(__file__).resolve().parents[2]
BASE_SCRIPT = ROOT_DIR / "tools" / "decomp" / "permute_function.py"


def configure_base_paths(module) -> None:
    module.ROOT_DIR = ROOT_DIR
    module.ORIGINAL_EXE = ROOT_DIR / "disks" / "SLPM_86.053"
    module.FUNCTION_MAP = ROOT_DIR / "build" / "function_analysis" / "function_map.json"
    module.DECOMP_CONFIG = ROOT_DIR / "config" / "decompiled_functions.json"
    module.BUILD_DIR = ROOT_DIR / "build" / "jp"
    module.PERMUTER_DIR = ROOT_DIR / "build" / "permuter"


def load_base_module():
    spec = importlib.util.spec_from_file_location("permute_function_base", BASE_SCRIPT)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"Failed to load {BASE_SCRIPT}")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    configure_base_paths(module)
    return module


def run_permuter(func_dir: Path, func_name: str, iterations: int, jobs: int, verbose: bool) -> Optional[str]:
    base = load_base_module()
    installed, permuter_cmd = base.check_permuter_installed()

    if not installed:
        print("ERROR: decomp-permuter not installed!")
        print("Install: pip install decomp-permuter")
        return None

    cmd_parts = permuter_cmd.split()
    cmd = cmd_parts + [
        str(func_dir),
        "-j", str(jobs),
        "--iterations", str(iterations),
    ]

    if verbose:
        cmd.append("--debug")

    print(f"Running: {' '.join(cmd)}")

    try:
        result = subprocess.run(
            cmd,
            cwd=func_dir,
            capture_output=not verbose,
            text=True,
        )

        if verbose:
            print(result.stdout)
            print(result.stderr)

        output_dir = func_dir / "output"
        if output_dir.exists():
            outputs = list(output_dir.glob("*.c"))
            if outputs:
                with open(outputs[0], "r") as handle:
                    return handle.read()

        return None
    except subprocess.CalledProcessError as exc:
        print(f"Permuter failed: {exc}")
        return None


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Run decomp-permuter with configurable --jobs",
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )

    parser.add_argument("function", help="Function name (e.g., func_8004B338)")
    parser.add_argument("-i", "--iterations", type=int, default=1000)
    parser.add_argument("--jobs", type=int, default=2, help="Permuter worker threads")
    parser.add_argument("-v", "--verbose", action="store_true")
    parser.add_argument("--save-match", action="store_true")
    parser.add_argument("--show-base", action="store_true")
    parser.add_argument("--setup-only", action="store_true")
    parser.add_argument("--clean", action="store_true")

    args = parser.parse_args()

    base = load_base_module()

    function_map = base.load_function_map()
    decomp_config = base.load_decomp_config()

    func_info = base.get_function_info(args.function, function_map)
    if not func_info:
        print(f"Error: Function '{args.function}' not found in function map")
        sys.exit(1)

    decomp_func = next(
        (func for func in decomp_config.get("functions", []) if func["name"] == args.function),
        None,
    )

    if not decomp_func:
        print(f"Error: Function '{args.function}' not in decompiled_functions.json")
        sys.exit(1)

    if "source_file" not in decomp_func:
        print(
            f"Error: '{args.function}' missing source_file in config/decompiled_functions.json. "
            "Add it before permuting."
        )
        sys.exit(2)

    func_dir = base.PERMUTER_DIR / args.function
    if args.clean and func_dir.exists():
        print(f"Cleaning {func_dir}...")
        shutil.rmtree(func_dir)

    func_dir = base.setup_permuter_env(args.function, func_info, decomp_func)

    if args.show_base:
        base_c = func_dir / "base.c"
        print(base_c.read_text())

    if args.setup_only:
        print("Setup complete. Run without --setup-only to start permuting.")
        return

    match_code = run_permuter(
        func_dir=func_dir,
        func_name=args.function,
        iterations=args.iterations,
        jobs=args.jobs,
        verbose=args.verbose,
    )

    if match_code:
        print("MATCH FOUND!")
        if args.save_match:
            base.save_match_to_source(args.function, decomp_func["source_file"], match_code)
        return

    sys.exit(1)


if __name__ == "__main__":
    main()
