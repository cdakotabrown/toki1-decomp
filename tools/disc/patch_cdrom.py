#!/usr/bin/env python3
"""
patch_cdrom.py - Patch executable into CD-ROM track while preserving sector structure

This tool correctly patches a PS1 executable into a raw CD-ROM track by:
1. Preserving the 24-byte sector header (sync + header + subheader)
2. Writing only to the 2048-byte user data area
3. Preserving the 280-byte EDC/ECC area (most emulators don't verify these)

CD-ROM Mode 2 Form 1 sector structure (2352 bytes total):
- Bytes 0-11:   Sync pattern (12 bytes)
- Bytes 12-15:  Header (4 bytes: minute, second, sector, mode)
- Bytes 16-23:  Subheader (8 bytes, repeated twice)
- Bytes 24-2071: User data (2048 bytes)
- Bytes 2072-2075: EDC (4 bytes)
- Bytes 2076-2351: ECC (276 bytes)

Usage:
    python3 tools/patch_cdrom.py --track track1.bin --exe new_exe.bin --output patched.bin --start-sector 24
"""

import argparse
import os
import sys
import struct

SECTOR_SIZE = 2352
SYNC_SIZE = 12
HEADER_SIZE = 4
SUBHEADER_SIZE = 8
USER_DATA_OFFSET = SYNC_SIZE + HEADER_SIZE + SUBHEADER_SIZE  # 24 bytes
USER_DATA_SIZE = 2048
EDC_ECC_SIZE = 280

# CD-ROM sync pattern
SYNC_PATTERN = bytes([0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00])


def read_sector(track_data: bytes, sector_num: int) -> bytes:
    """Read a single sector from track data."""
    offset = sector_num * SECTOR_SIZE
    if offset + SECTOR_SIZE > len(track_data):
        raise ValueError(f"Sector {sector_num} is beyond end of track data")
    return track_data[offset:offset + SECTOR_SIZE]


def patch_sector_user_data(sector: bytes, user_data: bytes) -> bytes:
    """Replace the user data portion of a sector, preserving header and trailer."""
    if len(user_data) > USER_DATA_SIZE:
        raise ValueError(f"User data too large: {len(user_data)} > {USER_DATA_SIZE}")

    # Pad user data to exactly 2048 bytes
    if len(user_data) < USER_DATA_SIZE:
        user_data = user_data + bytes(USER_DATA_SIZE - len(user_data))

    # Construct new sector: preserve header, replace user data, preserve trailer
    new_sector = (
        sector[:USER_DATA_OFFSET] +       # Keep sync + header + subheader (24 bytes)
        user_data +                        # New user data (2048 bytes)
        sector[USER_DATA_OFFSET + USER_DATA_SIZE:]  # Keep EDC/ECC (280 bytes)
    )

    assert len(new_sector) == SECTOR_SIZE, f"Sector size mismatch: {len(new_sector)}"
    return new_sector


def patch_executable(track_path: str, exe_path: str, output_path: str, start_sector: int) -> None:
    """Patch an executable into the CD-ROM track at the specified sector."""

    print(f"Reading track: {track_path}")
    with open(track_path, 'rb') as f:
        track_data = bytearray(f.read())

    track_size = len(track_data)
    total_sectors = track_size // SECTOR_SIZE
    print(f"Track size: {track_size:,} bytes ({total_sectors} sectors)")

    print(f"Reading executable: {exe_path}")
    with open(exe_path, 'rb') as f:
        exe_data = f.read()

    exe_size = len(exe_data)
    sectors_needed = (exe_size + USER_DATA_SIZE - 1) // USER_DATA_SIZE
    print(f"Executable size: {exe_size:,} bytes ({sectors_needed} sectors needed)")

    # Verify start sector is valid
    if start_sector < 0 or start_sector >= total_sectors:
        raise ValueError(f"Start sector {start_sector} is invalid (track has {total_sectors} sectors)")

    end_sector = start_sector + sectors_needed
    if end_sector > total_sectors:
        raise ValueError(f"Executable would extend beyond track end (needs sector {end_sector-1}, track has {total_sectors})")

    # Verify we're overwriting at the right place (check for PS-X EXE header)
    first_sector = read_sector(track_data, start_sector)
    existing_header = first_sector[USER_DATA_OFFSET:USER_DATA_OFFSET + 8]

    if existing_header != b'PS-X EXE':
        print(f"WARNING: Sector {start_sector} doesn't contain PS-X EXE header!")
        print(f"  Found: {existing_header}")
        print(f"  Expected: PS-X EXE")

        # Try to find where the executable actually is
        print("\nSearching for PS-X EXE header in track...")
        for sector in range(total_sectors):
            s = read_sector(track_data, sector)
            if s[USER_DATA_OFFSET:USER_DATA_OFFSET + 8] == b'PS-X EXE':
                print(f"  Found at sector {sector} (offset 0x{sector * SECTOR_SIZE:x})")

        response = input("\nProceed anyway? (y/N): ")
        if response.lower() != 'y':
            print("Aborted.")
            sys.exit(1)
    else:
        print(f"Verified: Sector {start_sector} contains PS-X EXE header")

    # Patch each sector
    print(f"\nPatching {sectors_needed} sectors starting at sector {start_sector}...")
    for i in range(sectors_needed):
        sector_num = start_sector + i

        # Get the chunk of executable data for this sector
        chunk_start = i * USER_DATA_SIZE
        chunk_end = min(chunk_start + USER_DATA_SIZE, exe_size)
        chunk = exe_data[chunk_start:chunk_end]

        # Read existing sector
        sector_offset = sector_num * SECTOR_SIZE
        existing_sector = bytes(track_data[sector_offset:sector_offset + SECTOR_SIZE])

        # Patch it
        new_sector = patch_sector_user_data(existing_sector, chunk)

        # Write it back
        track_data[sector_offset:sector_offset + SECTOR_SIZE] = new_sector

        if i % 50 == 0 or i == sectors_needed - 1:
            print(f"  Patched sector {sector_num} ({i + 1}/{sectors_needed})")

    # Write output
    print(f"\nWriting patched track: {output_path}")
    with open(output_path, 'wb') as f:
        f.write(track_data)

    print(f"\nPatching complete!")
    print(f"  Sectors modified: {start_sector} to {end_sector - 1} ({sectors_needed} sectors)")
    print(f"  Bytes patched: {exe_size:,}")


def verify_patch(track_path: str, exe_path: str, start_sector: int) -> bool:
    """Verify that a patched track contains the correct executable."""

    print(f"Verifying patch...")

    with open(track_path, 'rb') as f:
        track_data = f.read()

    with open(exe_path, 'rb') as f:
        exe_data = f.read()

    exe_size = len(exe_data)
    sectors_needed = (exe_size + USER_DATA_SIZE - 1) // USER_DATA_SIZE

    # Extract user data from patched sectors
    extracted = bytearray()
    for i in range(sectors_needed):
        sector_num = start_sector + i
        sector_offset = sector_num * SECTOR_SIZE + USER_DATA_OFFSET
        extracted.extend(track_data[sector_offset:sector_offset + USER_DATA_SIZE])

    # Compare (truncate to exe size)
    extracted = bytes(extracted[:exe_size])

    if extracted == exe_data:
        print(f"  PASS: Extracted data matches executable")
        return True
    else:
        print(f"  FAIL: Extracted data does not match executable")
        # Find first difference
        for i, (a, b) in enumerate(zip(extracted, exe_data)):
            if a != b:
                print(f"  First difference at offset 0x{i:x}: got 0x{a:02x}, expected 0x{b:02x}")
                break
        return False


def main():
    parser = argparse.ArgumentParser(
        description='Patch PS1 executable into CD-ROM track',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
    # Patch executable at sector 24 (typical location)
    python3 tools/patch_cdrom.py \\
        --track "disks/track1.bin" \\
        --exe "build/jp/raw/SLPS_000.65.new" \\
        --output "build/jp/patched_track1.bin" \\
        --start-sector 24

    # Verify a patched track
    python3 tools/patch_cdrom.py \\
        --track "build/jp/patched_track1.bin" \\
        --exe "build/jp/raw/SLPS_000.65.new" \\
        --start-sector 24 \\
        --verify-only
        """
    )

    parser.add_argument('--track', required=True, help='Input track file (raw CD-ROM)')
    parser.add_argument('--exe', required=True, help='Executable file to patch in')
    parser.add_argument('--output', help='Output track file (default: overwrite input)')
    parser.add_argument('--start-sector', type=int, required=True,
                        help='Starting sector for executable (e.g., 24)')
    parser.add_argument('--verify-only', action='store_true',
                        help='Only verify, do not patch')

    args = parser.parse_args()

    if args.verify_only:
        success = verify_patch(args.track, args.exe, args.start_sector)
        sys.exit(0 if success else 1)

    output = args.output if args.output else args.track
    patch_executable(args.track, args.exe, output, args.start_sector)

    # Verify the patch
    verify_patch(output, args.exe, args.start_sector)


if __name__ == '__main__':
    main()
