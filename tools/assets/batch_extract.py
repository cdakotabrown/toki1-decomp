#!/usr/bin/env python3
"""
Batch extract all assets from extracted_iso to converted directory.

Extracts:
- TIM files -> PNG
- BIN files -> Scan for embedded TIM -> PNG
- VH/VB pairs -> VAG samples

Usage:
    python3 batch_extract.py                    # Extract all
    python3 batch_extract.py --type tim         # Extract only TIM files
    python3 batch_extract.py --type bin         # Extract only BIN files
    python3 batch_extract.py --type audio       # Extract only VH/VB audio
    python3 batch_extract.py --dir CDROM/EVENT  # Extract specific directory
"""

import argparse
import os
import sys
import struct
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor, as_completed
from typing import Optional, List, Tuple

# Add parent to path for imports
sys.path.insert(0, str(Path(__file__).parent))
from asset_manager import TIMFile, VABFile, scan_for_tim

# Directories
SCRIPT_DIR = Path(__file__).parent
PROJECT_ROOT = SCRIPT_DIR.parent.parent
ASSETS_DIR = PROJECT_ROOT / "assets" / "extracted_iso"
OUTPUT_DIR = PROJECT_ROOT / "assets" / "converted"


def extract_tim(tim_path: Path, output_base: Path) -> Tuple[bool, str]:
    """Extract a single TIM file to PNG."""
    try:
        data = tim_path.read_bytes()
        tim = TIMFile(data)
        if not tim.valid:
            return False, f"Invalid TIM: {tim_path}"

        rel_path = tim_path.relative_to(ASSETS_DIR)
        out_path = output_base / rel_path.with_suffix('.png')
        out_path.parent.mkdir(parents=True, exist_ok=True)

        if tim.to_png(out_path):
            return True, str(out_path)
        return False, f"Failed to convert: {tim_path}"
    except Exception as e:
        return False, f"Error {tim_path}: {e}"


def extract_bin(bin_path: Path, output_base: Path) -> Tuple[int, str]:
    """Extract TIM files embedded in a BIN file."""
    try:
        data = bin_path.read_bytes()
        tims = scan_for_tim(data)

        if not tims:
            return 0, ""

        rel_path = bin_path.relative_to(ASSETS_DIR)
        out_dir = output_base / rel_path.parent / rel_path.stem
        out_dir.mkdir(parents=True, exist_ok=True)

        count = 0
        for i, (offset, tim) in enumerate(tims):
            out_path = out_dir / f"{i:03d}_{offset:06X}.png"
            if tim.to_png(out_path):
                count += 1

        return count, str(out_dir)
    except Exception as e:
        return 0, f"Error {bin_path}: {e}"


def extract_audio(vh_path: Path, output_base: Path) -> Tuple[int, str]:
    """Extract VAB audio samples."""
    try:
        vb_path = vh_path.with_suffix('.VB')
        if not vb_path.exists():
            vb_path = vh_path.with_suffix('.vb')
        if not vb_path.exists():
            return 0, f"No VB file for: {vh_path}"

        vh_data = vh_path.read_bytes()
        vb_data = vb_path.read_bytes()

        vab = VABFile(vh_data, vb_data)
        if not vab.valid:
            return 0, f"Invalid VAB: {vh_path}"

        rel_path = vh_path.relative_to(ASSETS_DIR)
        out_dir = output_base / rel_path.parent / rel_path.stem

        count = vab.extract_samples(out_dir)
        return count, str(out_dir)
    except Exception as e:
        return 0, f"Error {vh_path}: {e}"


def find_files(asset_type: Optional[str], subdir: Optional[str]) -> List[Path]:
    """Find files to extract based on type and directory filters."""
    base = ASSETS_DIR / subdir if subdir else ASSETS_DIR

    if not base.exists():
        print(f"Directory not found: {base}")
        return []

    files = []

    if asset_type in (None, 'tim'):
        files.extend(base.rglob('*.TIM'))
        files.extend(base.rglob('*.tim'))

    if asset_type in (None, 'bin'):
        files.extend(base.rglob('*.BIN'))
        files.extend(base.rglob('*.bin'))

    if asset_type in (None, 'audio'):
        files.extend(base.rglob('*.VH'))
        files.extend(base.rglob('*.vh'))

    return sorted(set(files))


def main():
    parser = argparse.ArgumentParser(description='Batch extract game assets')
    parser.add_argument('--type', '-t', choices=['tim', 'bin', 'audio'],
                        help='Asset type to extract (default: all)')
    parser.add_argument('--dir', '-d', help='Subdirectory to extract (e.g., CDROM/EVENT)')
    parser.add_argument('--output', '-o', type=Path, default=OUTPUT_DIR,
                        help='Output directory')
    parser.add_argument('--workers', '-w', type=int, default=4,
                        help='Number of parallel workers')
    parser.add_argument('--quiet', '-q', action='store_true',
                        help='Suppress per-file output')
    args = parser.parse_args()

    # Check PIL
    try:
        from PIL import Image
    except ImportError:
        print("ERROR: Pillow not installed. Run:")
        print("  .venv/bin/pip install Pillow")
        sys.exit(1)

    # Check assets exist
    if not ASSETS_DIR.exists():
        print(f"ERROR: Assets not extracted. Run: make setup-assets")
        sys.exit(1)

    # Create output dir
    args.output.mkdir(parents=True, exist_ok=True)

    # Find files
    files = find_files(args.type, args.dir)
    if not files:
        print("No files found to extract.")
        return

    print(f"Found {len(files)} files to process")
    print(f"Output: {args.output}")
    print()

    # Stats
    stats = {'tim': 0, 'tim_fail': 0, 'bin': 0, 'bin_images': 0, 'audio': 0, 'audio_samples': 0}

    # Process files
    with ThreadPoolExecutor(max_workers=args.workers) as executor:
        futures = {}

        for f in files:
            suffix = f.suffix.upper()
            if suffix == '.TIM':
                futures[executor.submit(extract_tim, f, args.output)] = ('tim', f)
            elif suffix == '.BIN':
                futures[executor.submit(extract_bin, f, args.output)] = ('bin', f)
            elif suffix == '.VH':
                futures[executor.submit(extract_audio, f, args.output)] = ('audio', f)

        for future in as_completed(futures):
            ftype, fpath = futures[future]
            try:
                result = future.result()

                if ftype == 'tim':
                    success, msg = result
                    if success:
                        stats['tim'] += 1
                        if not args.quiet:
                            print(f"[TIM] {msg}")
                    else:
                        stats['tim_fail'] += 1

                elif ftype == 'bin':
                    count, msg = result
                    if count > 0:
                        stats['bin'] += 1
                        stats['bin_images'] += count
                        if not args.quiet:
                            print(f"[BIN] {count} images -> {msg}")

                elif ftype == 'audio':
                    count, msg = result
                    if count > 0:
                        stats['audio'] += 1
                        stats['audio_samples'] += count
                        if not args.quiet:
                            print(f"[VAB] {count} samples -> {msg}")

            except Exception as e:
                print(f"Error processing {fpath}: {e}")

    # Summary
    print()
    print("=" * 60)
    print("Extraction complete!")
    print("=" * 60)
    print(f"  TIM files:     {stats['tim']} converted ({stats['tim_fail']} failed)")
    print(f"  BIN files:     {stats['bin']} with {stats['bin_images']} embedded images")
    print(f"  Audio files:   {stats['audio']} with {stats['audio_samples']} samples")
    print()
    print(f"Output directory: {args.output}")


if __name__ == '__main__':
    main()
