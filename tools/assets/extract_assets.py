#!/usr/bin/env python3
"""
extract_assets.py - Extract game assets from disc image

Usage:
    python3 tools/assets/extract_assets.py [disc_image]

If no disc image specified, looks for:
    disks/Tokimeki Memorial*.bin (Track 1)

Extracts to:
    assets/extracted/    - Raw dumpsxiso output
    assets/overlays/     - EXN overlays (for reference)
"""

import os
import sys
import shutil
import subprocess
from pathlib import Path

# Project root
ROOT = Path(__file__).parent.parent.parent

# Output directories
EXTRACTED_DIR = ROOT / "assets" / "extracted"
OVERLAYS_DIR = ROOT / "assets" / "overlays"

# Tools
DUMPSXISO = ROOT / "tools" / "psxiso" / "dumpsxiso"


def find_disc_image():
    """Find the disc image in disks/"""
    disks_dir = ROOT / "disks"

    # Look for Track 1 bin file
    for pattern in ["*Track 1*.bin", "*track1*.bin", "*.bin"]:
        matches = list(disks_dir.glob(pattern))
        # Filter out o.bin and Track 2
        matches = [m for m in matches if "Track 2" not in str(m) and m.name != "o.bin"]
        if matches:
            return matches[0]

    return None


def extract_disc(disc_image: Path):
    """Extract disc contents using dumpsxiso"""
    print(f"[EXTRACT] Extracting {disc_image.name}...")

    # Create output directory
    EXTRACTED_DIR.mkdir(parents=True, exist_ok=True)

    # Run dumpsxiso
    cmd = [str(DUMPSXISO), str(disc_image), "-x", str(EXTRACTED_DIR), "-s"]
    print(f"[EXTRACT] Running: {' '.join(cmd)}")

    result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode != 0:
        print(f"[ERROR] dumpsxiso failed:")
        print(result.stderr)
        return False

    print(f"[EXTRACT] Extraction complete")
    return True


def copy_overlays():
    """Copy EXN overlays to separate directory for easy access"""
    exedir = EXTRACTED_DIR / "CDROM" / "EXEDIR"

    if not exedir.exists():
        print(f"[WARN] EXEDIR not found at {exedir}")
        return False

    OVERLAYS_DIR.mkdir(parents=True, exist_ok=True)

    count = 0
    for exn in exedir.glob("*.EXN"):
        dest = OVERLAYS_DIR / exn.name
        shutil.copy2(exn, dest)
        count += 1
        print(f"[OVERLAY] Copied {exn.name}")

    # Also copy o.bin if it exists
    obin = exedir / "O.BIN"
    if obin.exists():
        shutil.copy2(obin, OVERLAYS_DIR / "O.BIN")
        count += 1
        print(f"[OVERLAY] Copied O.BIN")

    print(f"[OVERLAY] Copied {count} overlay files")
    return True


def normalize_executable():
    """Handle different executable naming (PSX.EXE vs SLPS_000.65)"""
    psx_exe = EXTRACTED_DIR / "PSX.EXE"
    slps = EXTRACTED_DIR / "SLPS_000.65"

    if psx_exe.exists() and not slps.exists():
        # Create symlink with expected name
        print(f"[NORMALIZE] Linking PSX.EXE -> SLPS_000.65")
        slps.symlink_to("PSX.EXE")
        return True
    elif slps.exists():
        return True
    return False


def verify_extraction():
    """Verify key files were extracted"""
    # Normalize executable name first
    normalize_executable()

    # Check for either naming convention
    exe_exists = (EXTRACTED_DIR / "SLPS_000.65").exists() or (EXTRACTED_DIR / "PSX.EXE").exists()

    required = [
        EXTRACTED_DIR / "SYSTEM.CNF",
        EXTRACTED_DIR / "CDROM" / "EXEDIR" / "OPTION.EXN",
    ]

    missing = []
    if not exe_exists:
        missing.append("SLPS_000.65 or PSX.EXE")

    for f in required:
        if not f.exists():
            missing.append(str(f))

    if missing:
        print(f"[ERROR] Missing required files:")
        for f in missing:
            print(f"  - {f}")
        return False

    print(f"[VERIFY] All required files present")
    return True


def print_summary():
    """Print extraction summary"""
    print("\n" + "="*60)
    print("EXTRACTION SUMMARY")
    print("="*60)

    # Count files by type
    if EXTRACTED_DIR.exists():
        cdrom = EXTRACTED_DIR / "CDROM"
        if cdrom.exists():
            for subdir in sorted(cdrom.iterdir()):
                if subdir.is_dir():
                    count = len(list(subdir.glob("*")))
                    print(f"  {subdir.name}/: {count} files")

    # Overlay count
    if OVERLAYS_DIR.exists():
        overlay_count = len(list(OVERLAYS_DIR.glob("*.EXN")))
        print(f"  Overlays: {overlay_count} EXN files")

    print("="*60)
    print(f"Output: {EXTRACTED_DIR}")
    print(f"Overlays: {OVERLAYS_DIR}")
    print("="*60 + "\n")


def main():
    # Find or use provided disc image
    if len(sys.argv) > 1:
        disc_image = Path(sys.argv[1])
        if not disc_image.exists():
            print(f"[ERROR] Disc image not found: {disc_image}")
            sys.exit(1)
    else:
        disc_image = find_disc_image()
        if not disc_image:
            print("[ERROR] No disc image found in disks/")
            print("Usage: python3 tools/assets/extract_assets.py <disc_image.bin>")
            sys.exit(1)

    print(f"[SETUP] Using disc image: {disc_image}")

    # Check if already extracted
    if EXTRACTED_DIR.exists() and (EXTRACTED_DIR / "SLPS_000.65").exists():
        print(f"[INFO] Assets already extracted at {EXTRACTED_DIR}")
        response = input("Re-extract? [y/N] ").strip().lower()
        if response != 'y':
            print("[INFO] Skipping extraction")
            return 0
        shutil.rmtree(EXTRACTED_DIR)

    # Extract
    if not extract_disc(disc_image):
        return 1

    # Copy overlays
    copy_overlays()

    # Verify
    if not verify_extraction():
        return 1

    # Summary
    print_summary()

    print("[DONE] Asset extraction complete!")
    print("Next: make build-c  (build main binary)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
