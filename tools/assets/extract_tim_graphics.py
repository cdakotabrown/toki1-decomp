#!/usr/bin/env python3
"""
TIM Graphics Extractor for Tokimeki Memorial: Forever with You

This script scans PS1 disc images for TIM (PlayStation graphics) files and extracts them.

FINDINGS FOR TOKIMEKI MEMORIAL:
===============================
After thorough scanning of the entire disc (702 MB, 298,500 sectors), we found:
- Only 1 valid TIM file exists on the disc
- The TIM is a 256x256 4bpp font atlas with 16 palettes
- Located at sector 40087 (offset 0x059EAB68)

The ~9,400 TIM estimate was based on false positives - the `10 00 00 00` magic
appears frequently in data but isn't followed by valid TIM structures.

WHERE ARE THE GRAPHICS?
=======================
Tokimeki Memorial uses alternative graphics storage:
1. Video streams (STR files): MDEC-compressed frames
2. Custom BIN archives: Raw pixel data or proprietary format
3. XA interleaved data: Audio with some video
4. Possibly compressed formats specific to Konami

The CDROM directory contains many .BIN files (VS_*.BIN, etc.) that likely
contain graphics, but NOT in standard TIM format.

USAGE:
    python3 extract_tim_graphics.py [--disc PATH] [--output DIR]

OPTIONS:
    --disc PATH     Path to disc image (default: auto-detect in disks/)
    --output DIR    Output directory (default: assets/graphics)
    --verbose       Show detailed progress
    --scan-only     Just scan, don't extract
"""

import argparse
import struct
import mmap
import sys
from pathlib import Path
from collections import defaultdict

# Constants
SECTOR_SIZE = 2352
SYNC_SIZE = 12
HEADER_SIZE = 4
SUBHEADER_SIZE = 8
USER_DATA_OFFSET = 24  # sync + header + subheader
USER_DATA_SIZE = 2048
ECC_SIZE = 280  # EDC + ECC


class TIMFile:
    """Represents a parsed TIM graphics file."""

    def __init__(self, data: bytes, offset: int = 0):
        self.valid = False
        self.offset = offset
        self.parse(data, offset)

    def parse(self, data: bytes, offset: int):
        """Parse TIM header and validate structure."""
        if offset + 20 > len(data):
            return

        # Check magic
        if data[offset:offset+4] != b'\x10\x00\x00\x00':
            return

        flags = struct.unpack('<I', data[offset+4:offset+8])[0]

        # Valid flag values: 0x00-0x03 (with CLUT implied), 0x08-0x0B (explicit CLUT flag)
        if flags > 0x0B:
            return

        self.flags = flags
        self.bpp_mode = flags & 0x03
        self.has_clut = ((flags >> 3) & 0x01) == 1 or self.bpp_mode < 2

        ptr = offset + 8
        self.clut = None

        if self.has_clut:
            if ptr + 12 > len(data):
                return

            clut_size = struct.unpack('<I', data[ptr:ptr+4])[0]
            if clut_size < 12 or clut_size > 200000:
                return

            clut_x = struct.unpack('<H', data[ptr+4:ptr+6])[0]
            clut_y = struct.unpack('<H', data[ptr+6:ptr+8])[0]
            clut_w = struct.unpack('<H', data[ptr+8:ptr+10])[0]
            clut_h = struct.unpack('<H', data[ptr+10:ptr+12])[0]

            # VRAM constraints
            if clut_x > 1023 or clut_y > 511:
                return
            if clut_w == 0 or clut_h == 0 or clut_w > 512 or clut_h > 512:
                return

            # Verify size
            expected_size = 12 + clut_w * clut_h * 2
            if clut_size != expected_size:
                return

            self.clut = {
                'size': clut_size,
                'x': clut_x,
                'y': clut_y,
                'width': clut_w,
                'height': clut_h,
                'colors': clut_w * clut_h,
                'offset': ptr
            }
            ptr += clut_size

        # Image block
        if ptr + 12 > len(data):
            return

        img_size = struct.unpack('<I', data[ptr:ptr+4])[0]
        if img_size < 12 or img_size > 500000:
            return

        img_x = struct.unpack('<H', data[ptr+4:ptr+6])[0]
        img_y = struct.unpack('<H', data[ptr+6:ptr+8])[0]
        img_w = struct.unpack('<H', data[ptr+8:ptr+10])[0]
        img_h = struct.unpack('<H', data[ptr+10:ptr+12])[0]

        # VRAM constraints
        if img_x > 1023 or img_y > 511:
            return
        if img_w == 0 or img_h == 0 or img_w > 1024 or img_h > 512:
            return

        # Verify size
        expected_size = 12 + img_w * img_h * 2
        if img_size != expected_size:
            return

        # Calculate pixel dimensions
        bpp_map = {0: 4, 1: 8, 2: 16, 3: 24}
        self.bpp = bpp_map.get(self.bpp_mode, 16)

        if self.bpp == 4:
            pixel_w = img_w * 4
        elif self.bpp == 8:
            pixel_w = img_w * 2
        elif self.bpp == 16:
            pixel_w = img_w
        else:
            pixel_w = img_w * 2 // 3

        self.image = {
            'size': img_size,
            'x': img_x,
            'y': img_y,
            'raw_width': img_w,
            'raw_height': img_h,
            'pixel_width': pixel_w,
            'pixel_height': img_h,
            'offset': ptr
        }

        self.total_size = ptr + img_size - offset
        self.valid = True

    def get_tim_data(self, data: bytes) -> bytes:
        """Extract the complete TIM file data."""
        if not self.valid:
            return b''
        return data[self.offset:self.offset + self.total_size]

    def __str__(self):
        if not self.valid:
            return "Invalid TIM"

        clut_str = f"CLUT {self.clut['width']}x{self.clut['height']} ({self.clut['colors']} colors)" if self.clut else "no CLUT"
        return f"{self.bpp}bpp {self.image['pixel_width']}x{self.image['pixel_height']}px, {clut_str}, {self.total_size} bytes"


def read_continuous_sectors(f, start_sector: int, num_sectors: int) -> bytes:
    """Read user data from consecutive Mode 2 sectors."""
    data = bytearray()
    for i in range(num_sectors):
        f.seek((start_sector + i) * SECTOR_SIZE + USER_DATA_OFFSET)
        data.extend(f.read(USER_DATA_SIZE))
    return bytes(data)


def scan_disc_for_tim(disc_path: str, verbose: bool = False) -> list:
    """Scan entire disc image for valid TIM files."""
    tims = []

    with open(disc_path, 'rb') as f:
        mm = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)

        total_sectors = len(mm) // SECTOR_SIZE
        if verbose:
            print(f"Scanning {total_sectors:,} sectors ({len(mm):,} bytes)...")

        magic = b'\x10\x00\x00\x00'
        pos = 0
        checked = 0

        while pos < len(mm) - 100:
            pos = mm.find(magic, pos)
            if pos == -1:
                break

            checked += 1

            # Read enough data to validate (up to 500KB)
            end = min(pos + 500000, len(mm))
            chunk = mm[pos:end]

            tim = TIMFile(chunk, 0)
            if tim.valid:
                # Calculate sector location
                sector = pos // SECTOR_SIZE
                sector_offset = pos % SECTOR_SIZE
                in_user_data = USER_DATA_OFFSET <= sector_offset < (USER_DATA_OFFSET + USER_DATA_SIZE)

                tims.append({
                    'tim': tim,
                    'raw_offset': pos,
                    'sector': sector,
                    'sector_offset': sector_offset,
                    'in_user_data': in_user_data
                })

                if verbose:
                    print(f"  Found TIM at sector {sector}: {tim}")

            pos += 1

        mm.close()

    if verbose:
        print(f"Checked {checked:,} magic sequences")
        print(f"Found {len(tims)} valid TIM file(s)")

    return tims


def extract_tim(disc_path: str, tim_info: dict, output_dir: Path, index: int) -> Path:
    """Extract a TIM file from the disc."""
    tim = tim_info['tim']

    # Read the TIM data
    # Need to read multiple sectors for multi-sector TIMs
    with open(disc_path, 'rb') as f:
        sector = tim_info['sector']
        sector_offset = tim_info['sector_offset']

        # Calculate how many sectors we need
        offset_in_first_sector = sector_offset - USER_DATA_OFFSET
        if offset_in_first_sector < 0:
            offset_in_first_sector = 0

        bytes_needed = tim.total_size + offset_in_first_sector
        sectors_needed = (bytes_needed + USER_DATA_SIZE - 1) // USER_DATA_SIZE

        # Read user data from sectors
        data = read_continuous_sectors(f, sector, sectors_needed + 1)

        # Extract TIM from the data
        tim_data = data[offset_in_first_sector:offset_in_first_sector + tim.total_size]

    # Save TIM file
    output_dir.mkdir(parents=True, exist_ok=True)
    tim_path = output_dir / f"tim_{index:05d}.tim"

    with open(tim_path, 'wb') as f:
        f.write(tim_data)

    return tim_path


def convert_tim_to_png(tim_path: Path, output_dir: Path) -> Path:
    """Convert TIM file to PNG using PIL if available."""
    try:
        from PIL import Image
    except ImportError:
        print("Warning: PIL not installed, skipping PNG conversion")
        print("Install with: pip install Pillow")
        return None

    with open(tim_path, 'rb') as f:
        data = f.read()

    tim = TIMFile(data, 0)
    if not tim.valid:
        return None

    # Create image based on BPP
    width = tim.image['pixel_width']
    height = tim.image['pixel_height']

    img_data_start = tim.image['offset'] + 12
    img_data = data[img_data_start:img_data_start + tim.image['raw_width'] * height * 2]

    if tim.bpp == 4 and tim.clut:
        # 4bpp indexed color
        clut_data_start = tim.clut['offset'] + 12
        clut_data = data[clut_data_start:clut_data_start + tim.clut['width'] * tim.clut['height'] * 2]

        # Parse CLUT (16-bit colors: ABBBBBGGGGGRRRRR)
        colors = []
        for i in range(0, len(clut_data), 2):
            if i + 2 > len(clut_data):
                break
            word = struct.unpack('<H', clut_data[i:i+2])[0]
            r = ((word >> 0) & 0x1F) * 255 // 31
            g = ((word >> 5) & 0x1F) * 255 // 31
            b = ((word >> 10) & 0x1F) * 255 // 31
            a = 255 if (word >> 15) == 0 else 128  # Semi-transparent if STP bit set
            colors.append((r, g, b, a))

        # Create indexed image
        img = Image.new('RGBA', (width, height))
        pixels = img.load()

        for y in range(height):
            for x in range(width):
                byte_idx = y * tim.image['raw_width'] * 2 + (x // 2)
                if byte_idx < len(img_data):
                    byte = img_data[byte_idx]
                    if x % 2 == 0:
                        idx = byte & 0x0F
                    else:
                        idx = (byte >> 4) & 0x0F

                    if idx < len(colors):
                        pixels[x, y] = colors[idx]

        # Save first palette version
        output_dir.mkdir(parents=True, exist_ok=True)
        png_path = output_dir / f"{tim_path.stem}_p00.png"
        img.save(png_path)

        # For multi-palette TIMs, save additional versions
        if tim.clut['height'] > 1:
            for pal_idx in range(1, min(tim.clut['height'], 16)):
                pal_offset = pal_idx * tim.clut['width'] * 2
                pal_colors = []
                for i in range(0, tim.clut['width'] * 2, 2):
                    if clut_data_start + pal_offset + i + 2 > len(data):
                        break
                    word = struct.unpack('<H', data[clut_data_start + pal_offset + i:clut_data_start + pal_offset + i + 2])[0]
                    r = ((word >> 0) & 0x1F) * 255 // 31
                    g = ((word >> 5) & 0x1F) * 255 // 31
                    b = ((word >> 10) & 0x1F) * 255 // 31
                    a = 255 if (word >> 15) == 0 else 128
                    pal_colors.append((r, g, b, a))

                img_pal = Image.new('RGBA', (width, height))
                pixels_pal = img_pal.load()

                for y in range(height):
                    for x in range(width):
                        byte_idx = y * tim.image['raw_width'] * 2 + (x // 2)
                        if byte_idx < len(img_data):
                            byte = img_data[byte_idx]
                            if x % 2 == 0:
                                idx = byte & 0x0F
                            else:
                                idx = (byte >> 4) & 0x0F

                            if idx < len(pal_colors):
                                pixels_pal[x, y] = pal_colors[idx]

                pal_path = output_dir / f"{tim_path.stem}_p{pal_idx:02d}.png"
                img_pal.save(pal_path)

        return png_path

    elif tim.bpp == 8 and tim.clut:
        # 8bpp indexed color
        clut_data_start = tim.clut['offset'] + 12
        clut_data = data[clut_data_start:clut_data_start + tim.clut['width'] * tim.clut['height'] * 2]

        colors = []
        for i in range(0, min(256 * 2, len(clut_data)), 2):
            word = struct.unpack('<H', clut_data[i:i+2])[0]
            r = ((word >> 0) & 0x1F) * 255 // 31
            g = ((word >> 5) & 0x1F) * 255 // 31
            b = ((word >> 10) & 0x1F) * 255 // 31
            a = 255 if (word >> 15) == 0 else 128
            colors.append((r, g, b, a))

        img = Image.new('RGBA', (width, height))
        pixels = img.load()

        for y in range(height):
            for x in range(width):
                byte_idx = y * tim.image['raw_width'] * 2 + x
                if byte_idx < len(img_data):
                    idx = img_data[byte_idx]
                    if idx < len(colors):
                        pixels[x, y] = colors[idx]

        output_dir.mkdir(parents=True, exist_ok=True)
        png_path = output_dir / f"{tim_path.stem}.png"
        img.save(png_path)
        return png_path

    elif tim.bpp == 16:
        # 16bpp direct color
        img = Image.new('RGBA', (width, height))
        pixels = img.load()

        for y in range(height):
            for x in range(width):
                word_idx = (y * width + x) * 2
                if word_idx + 2 <= len(img_data):
                    word = struct.unpack('<H', img_data[word_idx:word_idx+2])[0]
                    r = ((word >> 0) & 0x1F) * 255 // 31
                    g = ((word >> 5) & 0x1F) * 255 // 31
                    b = ((word >> 10) & 0x1F) * 255 // 31
                    a = 255 if (word >> 15) == 0 else 128
                    pixels[x, y] = (r, g, b, a)

        output_dir.mkdir(parents=True, exist_ok=True)
        png_path = output_dir / f"{tim_path.stem}.png"
        img.save(png_path)
        return png_path

    return None


def find_disc_image(project_root: Path) -> Path:
    """Find the disc image file."""
    disks_dir = project_root / "disks"

    # Look for Track 1 specifically (data track, not audio)
    for pattern in ["*.bin", "*.BIN"]:
        for path in disks_dir.glob(pattern):
            if "Track 1" in path.name:
                return path

    # Look for main game disc (Tokimeki Memorial)
    for pattern in ["*.bin", "*.BIN"]:
        for path in disks_dir.glob(pattern):
            name_lower = path.name.lower()
            if "tokimeki" in name_lower and "track 2" not in name_lower:
                return path

    # Fallback to any .bin that's large enough to be data (>100MB)
    for path in disks_dir.glob("*.bin"):
        if path.stat().st_size > 100_000_000:  # >100MB = likely data track
            return path

    return None


def main():
    parser = argparse.ArgumentParser(
        description="Extract TIM graphics from Tokimeki Memorial disc",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )
    parser.add_argument("--disc", type=str, help="Path to disc image")
    parser.add_argument("--output", type=str, default="assets/graphics", help="Output directory")
    parser.add_argument("--verbose", "-v", action="store_true", help="Verbose output")
    parser.add_argument("--scan-only", action="store_true", help="Only scan, don't extract")

    args = parser.parse_args()

    # Find project root
    script_path = Path(__file__).resolve()
    project_root = script_path.parent.parent

    # Find disc image
    if args.disc:
        disc_path = Path(args.disc)
    else:
        disc_path = find_disc_image(project_root)

    if not disc_path or not disc_path.exists():
        print("Error: Could not find disc image")
        print("Please specify with --disc PATH")
        sys.exit(1)

    print(f"Disc image: {disc_path}")
    print(f"Disc size: {disc_path.stat().st_size:,} bytes")
    print()

    # Scan for TIMs
    print("Scanning for TIM files...")
    tims = scan_disc_for_tim(str(disc_path), verbose=args.verbose)

    print()
    print("=" * 60)
    print("SCAN RESULTS")
    print("=" * 60)
    print(f"Total TIM files found: {len(tims)}")

    if not tims:
        print()
        print("No TIM files found on disc.")
        print()
        print("This is expected for Tokimeki Memorial.")
        print("The game uses alternative graphics formats (see docstring for details).")
        return

    print()
    for i, tim_info in enumerate(tims):
        tim = tim_info['tim']
        print(f"{i+1}. Sector {tim_info['sector']}: {tim}")

    if args.scan_only:
        return

    # Extract TIMs
    print()
    print("Extracting TIM files...")

    output_dir = project_root / args.output
    tim_dir = output_dir / "tim"
    png_dir = output_dir / "png"

    extracted_tims = []
    converted_pngs = []

    for i, tim_info in enumerate(tims):
        tim_path = extract_tim(str(disc_path), tim_info, tim_dir, i)
        extracted_tims.append(tim_path)
        print(f"  Extracted: {tim_path}")

        # Convert to PNG
        png_path = convert_tim_to_png(tim_path, png_dir)
        if png_path:
            converted_pngs.append(png_path)
            print(f"  Converted: {png_path}")

    # Summary
    print()
    print("=" * 60)
    print("EXTRACTION SUMMARY")
    print("=" * 60)
    print(f"TIM files extracted: {len(extracted_tims)}")
    print(f"PNG files created: {len(converted_pngs)}")
    print(f"Output directory: {output_dir}")
    print()
    print("Note: Tokimeki Memorial only has 1 TIM file (a font atlas).")
    print("Other graphics are stored in proprietary formats within .BIN files.")


if __name__ == "__main__":
    main()
