#!/usr/bin/env python3
"""
Extract IDO compiler files from SGI IRIX ISO images.

SGI IRIX ISOs use the EFS (Extent File System) or XFS filesystem.
This script handles extraction using various methods.

Usage:
    python3 extract_ido_iso.py /path/to/ido.iso /output/dir
"""

import sys
import os
import subprocess
import shutil
import tempfile
from pathlib import Path

def run_cmd(cmd, check=True):
    """Run a command and return output."""
    print(f"  Running: {' '.join(cmd)}")
    result = subprocess.run(cmd, capture_output=True, text=True)
    if check and result.returncode != 0:
        print(f"  Error: {result.stderr}")
        return None
    return result.stdout

def check_tools():
    """Check which extraction tools are available."""
    tools = {}

    # Check for isoinfo (from genisoimage/cdrtools)
    if shutil.which("isoinfo"):
        tools['isoinfo'] = True

    # Check for 7z
    if shutil.which("7z"):
        tools['7z'] = True

    # Check for xorriso
    if shutil.which("xorriso"):
        tools['xorriso'] = True

    # Check for bsdtar
    if shutil.which("bsdtar"):
        tools['bsdtar'] = True

    # Check for mount (requires root)
    if shutil.which("mount") and os.geteuid() == 0:
        tools['mount'] = True

    return tools

def extract_with_7z(iso_path, output_dir):
    """Extract using 7z (handles most formats)."""
    print("Attempting extraction with 7z...")
    cmd = ["7z", "x", "-y", f"-o{output_dir}", str(iso_path)]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode == 0:
        print("  7z extraction successful!")
        return True
    print(f"  7z failed: {result.stderr[:200]}")
    return False

def extract_with_isoinfo(iso_path, output_dir):
    """Extract using isoinfo (ISO9660 only)."""
    print("Attempting extraction with isoinfo...")

    # List files first
    cmd = ["isoinfo", "-i", str(iso_path), "-l", "-R"]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"  isoinfo list failed: {result.stderr[:200]}")
        return False

    # Parse file list and extract each file
    lines = result.stdout.split('\n')
    current_dir = ""
    files_extracted = 0

    for line in lines:
        if line.startswith("Directory listing of "):
            current_dir = line.replace("Directory listing of ", "").strip()
        elif line.strip() and not line.startswith("-") and not line.startswith("d"):
            continue
        elif line.startswith("-"):
            # Parse file entry
            parts = line.split()
            if len(parts) >= 12:
                filename = parts[-1]
                if filename in ['.', '..']:
                    continue
                iso_path_file = os.path.join(current_dir, filename)
                local_path = os.path.join(output_dir, iso_path_file.lstrip('/'))

                os.makedirs(os.path.dirname(local_path), exist_ok=True)

                # Extract file
                extract_cmd = ["isoinfo", "-i", str(iso_path), "-R", "-x", iso_path_file]
                with open(local_path, 'wb') as f:
                    proc = subprocess.run(extract_cmd, stdout=f, stderr=subprocess.PIPE)
                    if proc.returncode == 0:
                        files_extracted += 1

    if files_extracted > 0:
        print(f"  Extracted {files_extracted} files with isoinfo")
        return True
    return False

def extract_with_bsdtar(iso_path, output_dir):
    """Extract using bsdtar."""
    print("Attempting extraction with bsdtar...")
    cmd = ["bsdtar", "-xf", str(iso_path), "-C", output_dir]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode == 0:
        print("  bsdtar extraction successful!")
        return True
    print(f"  bsdtar failed: {result.stderr[:200]}")
    return False

def find_compiler_files(extract_dir):
    """Find and list all compiler-related files."""
    compiler_files = []
    patterns = ['cc', 'cfe', 'ugen', 'uopt', 'uld', 'as0', 'as1', 'ld', 'ucode']

    for root, dirs, files in os.walk(extract_dir):
        for f in files:
            if f in patterns or any(f.startswith(p) for p in patterns):
                full_path = os.path.join(root, f)
                compiler_files.append(full_path)

    return compiler_files

def extract_sgi_inst(extract_dir, output_dir):
    """
    SGI software is often distributed in 'inst' format.
    Look for .tardist or .sw files and extract them.
    """
    print("Looking for SGI inst packages...")

    for root, dirs, files in os.walk(extract_dir):
        for f in files:
            full_path = os.path.join(root, f)

            # .tardist files are tar archives
            if f.endswith('.tardist') or f.endswith('.tar'):
                print(f"  Found tardist: {f}")
                cmd = ["tar", "-xf", full_path, "-C", output_dir]
                subprocess.run(cmd, capture_output=True)

            # Some SGI packages use cpio
            elif f.endswith('.cpio'):
                print(f"  Found cpio: {f}")
                with open(full_path, 'rb') as cpio_file:
                    cmd = ["cpio", "-idm"]
                    subprocess.run(cmd, stdin=cpio_file, cwd=output_dir, capture_output=True)

def main():
    if len(sys.argv) < 2:
        print("Usage: extract_ido_iso.py <iso_file> [output_dir]")
        print()
        print("Example:")
        print("  python3 extract_ido_iso.py ido51.iso ./ido51_extracted")
        sys.exit(1)

    iso_path = Path(sys.argv[1]).resolve()

    if len(sys.argv) >= 3:
        output_dir = Path(sys.argv[2]).resolve()
    else:
        output_dir = Path(f"{iso_path.stem}_extracted").resolve()

    if not iso_path.exists():
        print(f"Error: ISO file not found: {iso_path}")
        sys.exit(1)

    print(f"=== IDO ISO Extractor ===")
    print(f"Input:  {iso_path}")
    print(f"Output: {output_dir}")
    print()

    # Check available tools
    tools = check_tools()
    print(f"Available extraction tools: {list(tools.keys())}")
    print()

    if not tools:
        print("Error: No extraction tools found!")
        print("Please install one of: p7zip-full, genisoimage, bsdtar")
        print("  Ubuntu/Debian: sudo apt install p7zip-full genisoimage")
        sys.exit(1)

    # Create output directory
    output_dir.mkdir(parents=True, exist_ok=True)

    # Try extraction methods in order of preference
    extracted = False

    if 'bsdtar' in tools and not extracted:
        extracted = extract_with_bsdtar(iso_path, output_dir)

    if '7z' in tools and not extracted:
        extracted = extract_with_7z(iso_path, output_dir)

    if 'isoinfo' in tools and not extracted:
        extracted = extract_with_isoinfo(iso_path, output_dir)

    if not extracted:
        print("Warning: Standard extraction failed. The ISO might use EFS filesystem.")
        print("Try mounting it on an IRIX system or using specialized EFS tools.")
        sys.exit(1)

    # Look for and extract SGI inst packages
    extract_sgi_inst(output_dir, output_dir)

    # Find compiler files
    print()
    print("=== Searching for compiler files ===")
    compiler_files = find_compiler_files(output_dir)

    if compiler_files:
        print(f"Found {len(compiler_files)} compiler-related files:")
        for f in compiler_files:
            rel_path = os.path.relpath(f, output_dir)
            size = os.path.getsize(f)
            print(f"  {rel_path} ({size:,} bytes)")
    else:
        print("No compiler files found directly.")
        print("The ISO may contain package files that need further extraction.")
        print()
        print("Looking for package files...")
        for root, dirs, files in os.walk(output_dir):
            for f in files:
                if any(ext in f.lower() for ext in ['.sw', '.idb', 'inst', 'tardist']):
                    print(f"  {os.path.relpath(os.path.join(root, f), output_dir)}")

    print()
    print(f"Extraction complete! Files in: {output_dir}")
    print()
    print("Next steps:")
    print(f"  1. Check version: ../check_ido_version.sh {output_dir}")
    print(f"  2. If compiler found, test with: ./test_ido_compiler.py {output_dir}")

if __name__ == "__main__":
    main()
