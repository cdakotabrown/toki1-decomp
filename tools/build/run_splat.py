#!/usr/bin/env python3
"""
Wrapper script for running splat with MIPS I architecture level.

This ensures splat generates assembly compatible with the PS1's R3000 CPU.
"""

import subprocess
import sys


def main():
    if len(sys.argv) < 2:
        print("Usage: run_splat.py <splat_config.yaml>")
        sys.exit(1)

    config_file = sys.argv[1]

    # Run splat with the config file
    cmd = ['python3', '-m', 'splat', 'split', config_file]

    print(f"Running: {' '.join(cmd)}")
    result = subprocess.run(cmd)

    sys.exit(result.returncode)


if __name__ == '__main__':
    main()
