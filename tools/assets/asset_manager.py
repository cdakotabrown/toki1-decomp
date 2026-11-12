#!/usr/bin/env python3
"""
Asset Manager for Tokimeki Memorial PS1

Extract, view, and edit game assets (TIM textures, VAB audio, etc.)

Usage:
    python3 asset_manager.py list                    # List all assets
    python3 asset_manager.py info FILE               # Show file info
    python3 asset_manager.py extract FILE [--out DIR] # Extract/convert asset
    python3 asset_manager.py replace FILE NEW_FILE   # Replace asset (same size)
    python3 asset_manager.py pack FILE NEW_FILE      # Pack new asset (any size)
"""

import argparse
import struct
import os
import sys
from pathlib import Path
from typing import Optional, Tuple, List, Dict, Any

# Asset directory
ASSETS_DIR = Path(__file__).parent.parent.parent / "assets" / "extracted_iso"
OUTPUT_DIR = Path(__file__).parent.parent.parent / "assets" / "converted"


class TIMFile:
    """PlayStation TIM texture format handler."""

    MAGIC = 0x10  # TIM magic number

    def __init__(self, data: bytes):
        self.data = data
        self.valid = False
        self.bpp = 0
        self.width = 0
        self.height = 0
        self.clut_colors = 0
        self.clut_data = None
        self.pixel_data = None
        self.parse()

    def parse(self):
        if len(self.data) < 8:
            return

        magic, flags = struct.unpack_from('<II', self.data, 0)
        if magic != self.MAGIC:
            return

        # Bits per pixel from flags
        pmode = flags & 0x7
        has_clut = (flags >> 3) & 1

        bpp_map = {0: 4, 1: 8, 2: 16, 3: 24}
        self.bpp = bpp_map.get(pmode, 0)

        offset = 8

        # Read CLUT if present
        if has_clut:
            clut_size = struct.unpack_from('<I', self.data, offset)[0]
            clut_x, clut_y, clut_w, clut_h = struct.unpack_from('<HHHH', self.data, offset + 4)
            self.clut_colors = clut_w * clut_h
            self.clut_data = self.data[offset + 12:offset + clut_size]
            offset += clut_size

        # Read pixel data
        if offset + 12 <= len(self.data):
            pix_size = struct.unpack_from('<I', self.data, offset)[0]
            pix_x, pix_y, pix_w, pix_h = struct.unpack_from('<HHHH', self.data, offset + 4)

            # Calculate actual dimensions based on BPP
            if self.bpp == 4:
                self.width = pix_w * 4
            elif self.bpp == 8:
                self.width = pix_w * 2
            elif self.bpp == 16:
                self.width = pix_w
            elif self.bpp == 24:
                self.width = (pix_w * 2) // 3
            else:
                self.width = pix_w

            self.height = pix_h
            self.pixel_data = self.data[offset + 12:offset + pix_size]
            self.valid = True

    def to_png(self, output_path: Path) -> bool:
        """Convert TIM to PNG (requires PIL)."""
        try:
            from PIL import Image
        except ImportError:
            print("PIL not installed. Install with: pip install Pillow")
            return False

        if not self.valid:
            return False

        img = Image.new('RGBA', (self.width, self.height))
        pixels = img.load()

        if self.bpp == 4 and self.clut_data:
            # 4-bit indexed
            clut = self._parse_clut_16bit()
            for y in range(self.height):
                for x in range(0, self.width, 2):
                    idx = (y * (self.width // 2) + x // 2)
                    if idx < len(self.pixel_data):
                        byte = self.pixel_data[idx]
                        c1 = clut[byte & 0xF] if (byte & 0xF) < len(clut) else (0, 0, 0, 255)
                        c2 = clut[(byte >> 4) & 0xF] if ((byte >> 4) & 0xF) < len(clut) else (0, 0, 0, 255)
                        if x < self.width:
                            pixels[x, y] = c1
                        if x + 1 < self.width:
                            pixels[x + 1, y] = c2

        elif self.bpp == 8 and self.clut_data:
            # 8-bit indexed
            clut = self._parse_clut_16bit()
            for y in range(self.height):
                for x in range(self.width):
                    idx = y * self.width + x
                    if idx < len(self.pixel_data):
                        cidx = self.pixel_data[idx]
                        if cidx < len(clut):
                            pixels[x, y] = clut[cidx]

        elif self.bpp == 16:
            # 16-bit direct color
            for y in range(self.height):
                for x in range(self.width):
                    idx = (y * self.width + x) * 2
                    if idx + 1 < len(self.pixel_data):
                        val = struct.unpack_from('<H', self.pixel_data, idx)[0]
                        r = ((val & 0x1F) * 255) // 31
                        g = (((val >> 5) & 0x1F) * 255) // 31
                        b = (((val >> 10) & 0x1F) * 255) // 31
                        a = 255 if (val & 0x8000) == 0 else 0
                        if val == 0:
                            a = 0  # Transparent black
                        pixels[x, y] = (r, g, b, a if a else 255)

        elif self.bpp == 24:
            # 24-bit direct color
            for y in range(self.height):
                for x in range(self.width):
                    idx = (y * self.width + x) * 3
                    if idx + 2 < len(self.pixel_data):
                        r, g, b = self.pixel_data[idx:idx + 3]
                        pixels[x, y] = (r, g, b, 255)

        output_path.parent.mkdir(parents=True, exist_ok=True)
        img.save(output_path)
        return True

    def _parse_clut_16bit(self) -> List[Tuple[int, int, int, int]]:
        """Parse 16-bit CLUT to RGBA tuples."""
        colors = []
        for i in range(0, len(self.clut_data), 2):
            if i + 1 < len(self.clut_data):
                val = struct.unpack_from('<H', self.clut_data, i)[0]
                r = ((val & 0x1F) * 255) // 31
                g = (((val >> 5) & 0x1F) * 255) // 31
                b = (((val >> 10) & 0x1F) * 255) // 31
                a = 0 if val == 0 else 255
                colors.append((r, g, b, a))
        return colors

    @staticmethod
    def from_png(png_path: Path, bpp: int = 8) -> Optional['TIMFile']:
        """Create TIM from PNG (requires PIL)."""
        try:
            from PIL import Image
        except ImportError:
            print("PIL not installed. Install with: pip install Pillow")
            return None

        img = Image.open(png_path).convert('RGBA')
        width, height = img.size

        # Build TIM data
        data = bytearray()

        if bpp == 16:
            # 16-bit direct color, no CLUT
            flags = 0x02  # 16bpp, no CLUT
            data.extend(struct.pack('<II', 0x10, flags))

            # Pixel data header
            pix_w = width
            pix_size = 12 + width * height * 2
            data.extend(struct.pack('<I', pix_size))
            data.extend(struct.pack('<HHHH', 0, 0, pix_w, height))

            # Pixel data
            pixels = img.load()
            for y in range(height):
                for x in range(width):
                    r, g, b, a = pixels[x, y]
                    r5 = (r * 31) // 255
                    g5 = (g * 31) // 255
                    b5 = (b * 31) // 255
                    stp = 0 if a < 128 else 0x8000
                    val = r5 | (g5 << 5) | (b5 << 10) | stp
                    data.extend(struct.pack('<H', val))

        elif bpp == 8:
            # 8-bit indexed with CLUT
            flags = 0x09  # 8bpp with CLUT

            # Quantize to 256 colors
            img_p = img.convert('P', palette=Image.ADAPTIVE, colors=256)
            palette = img_p.getpalette()

            # Build CLUT
            clut_data = bytearray()
            for i in range(256):
                r = palette[i * 3] if i * 3 < len(palette) else 0
                g = palette[i * 3 + 1] if i * 3 + 1 < len(palette) else 0
                b = palette[i * 3 + 2] if i * 3 + 2 < len(palette) else 0
                r5 = (r * 31) // 255
                g5 = (g * 31) // 255
                b5 = (b * 31) // 255
                val = r5 | (g5 << 5) | (b5 << 10)
                if r == 0 and g == 0 and b == 0:
                    val = 0  # Transparent
                clut_data.extend(struct.pack('<H', val))

            data.extend(struct.pack('<II', 0x10, flags))

            # CLUT header
            clut_size = 12 + len(clut_data)
            data.extend(struct.pack('<I', clut_size))
            data.extend(struct.pack('<HHHH', 0, 0, 256, 1))
            data.extend(clut_data)

            # Pixel data header
            pix_w = width // 2
            pix_size = 12 + width * height
            data.extend(struct.pack('<I', pix_size))
            data.extend(struct.pack('<HHHH', 0, 0, pix_w, height))

            # Pixel data
            pixels = img_p.load()
            for y in range(height):
                for x in range(width):
                    data.append(pixels[x, y])

        tim = TIMFile(bytes(data))
        return tim


class VABFile:
    """PlayStation VAB audio format handler."""

    def __init__(self, vh_data: bytes, vb_data: bytes):
        self.vh_data = vh_data  # Header
        self.vb_data = vb_data  # Body (samples)
        self.valid = False
        self.version = 0
        self.num_programs = 0
        self.num_tones = 0
        self.num_vags = 0
        self.sample_rate = 0
        self.parse()

    def parse(self):
        if len(self.vh_data) < 32:
            return

        magic = self.vh_data[0:4]
        if magic != b'pBAV' and magic != b'VABp':
            return

        self.version = struct.unpack_from('<I', self.vh_data, 4)[0]
        self.num_programs = struct.unpack_from('<H', self.vh_data, 18)[0]
        self.num_tones = struct.unpack_from('<H', self.vh_data, 20)[0]
        self.num_vags = struct.unpack_from('<H', self.vh_data, 22)[0]
        self.valid = True

    def extract_samples(self, output_dir: Path) -> int:
        """Extract VAG samples to individual files."""
        if not self.valid:
            return 0

        output_dir.mkdir(parents=True, exist_ok=True)

        # Parse VAG offsets from VH header (after program/tone data)
        # This is simplified - real implementation needs full VH parsing
        count = 0
        offset = 0

        # Each VAG starts with a size field in VH
        vh_offset = 2592  # Typical offset after headers

        for i in range(min(self.num_vags, 256)):
            if vh_offset + 2 > len(self.vh_data):
                break

            size = struct.unpack_from('<H', self.vh_data, vh_offset)[0]
            size = (size << 3)  # Size is in 8-byte units
            vh_offset += 2

            if size > 0 and offset + size <= len(self.vb_data):
                vag_data = self.vb_data[offset:offset + size]
                vag_path = output_dir / f"sample_{i:03d}.vag"

                # Add VAG header
                with open(vag_path, 'wb') as f:
                    f.write(b'VAGp')  # Magic
                    f.write(struct.pack('>I', 3))  # Version
                    f.write(struct.pack('>I', 0))  # Reserved
                    f.write(struct.pack('>I', size))  # Data size
                    f.write(struct.pack('>I', 22050))  # Sample rate
                    f.write(b'\x00' * 12)  # Reserved
                    f.write(f"sample_{i:03d}".encode().ljust(16, b'\x00'))
                    f.write(vag_data)

                count += 1
                offset += size

        return count


def scan_for_tim(data: bytes) -> List[Tuple[int, TIMFile]]:
    """Scan binary data for embedded TIM files."""
    tims = []
    i = 0
    while i < len(data) - 8:
        if data[i] == 0x10 and data[i + 1] == 0x00 and data[i + 2] == 0x00 and data[i + 3] == 0x00:
            # Potential TIM magic
            flags = struct.unpack_from('<I', data, i + 4)[0]
            if (flags & 0x7) <= 3:  # Valid BPP
                try:
                    tim = TIMFile(data[i:i + 0x80000])  # Max reasonable size
                    if tim.valid and tim.width > 0 and tim.height > 0:
                        tims.append((i, tim))
                        # Skip past this TIM
                        i += len(tim.data) if hasattr(tim, 'data') else 0x100
                        continue
                except:
                    pass
        i += 4
    return tims


def list_assets():
    """List all extractable assets."""
    if not ASSETS_DIR.exists():
        print(f"Assets not extracted. Run: make setup-assets")
        return

    print(f"Assets directory: {ASSETS_DIR}\n")

    # Count by type
    counts: Dict[str, int] = {}
    for f in ASSETS_DIR.rglob('*'):
        if f.is_file():
            ext = f.suffix.upper()
            counts[ext] = counts.get(ext, 0) + 1

    print("Asset types:")
    print("-" * 40)
    for ext, count in sorted(counts.items(), key=lambda x: -x[1]):
        desc = {
            '.BIN': 'Packed game data (may contain TIM/TMD)',
            '.VH': 'VAB audio header',
            '.VB': 'VAB audio body (samples)',
            '.SEQ': 'Music sequence (MIDI-like)',
            '.TMD': '3D model data',
            '.TIM': 'Texture image',
            '.STR': 'Video stream',
            '.XA': 'CD-XA audio',
            '.BS': 'Bitstream data',
            '.NBN': 'Game-specific data',
        }.get(ext, 'Unknown')
        print(f"  {ext:6} {count:4} files  - {desc}")

    print(f"\nTotal: {sum(counts.values())} files")
    print("\nKey directories:")
    for d in sorted(ASSETS_DIR.glob('CDROM/*')):
        if d.is_dir():
            count = sum(1 for _ in d.rglob('*') if _.is_file())
            print(f"  {d.name:12} ({count} files)")


def show_info(file_path: str):
    """Show information about an asset file."""
    path = Path(file_path)
    if not path.exists():
        # Try relative to assets dir
        path = ASSETS_DIR / file_path
    if not path.exists():
        print(f"File not found: {file_path}")
        return

    print(f"File: {path}")
    print(f"Size: {path.stat().st_size:,} bytes")
    print(f"Type: {path.suffix.upper()}")

    data = path.read_bytes()

    if path.suffix.upper() == '.TIM':
        tim = TIMFile(data)
        if tim.valid:
            print(f"\nTIM Image:")
            print(f"  Dimensions: {tim.width}x{tim.height}")
            print(f"  BPP: {tim.bpp}")
            print(f"  CLUT colors: {tim.clut_colors}")

    elif path.suffix.upper() == '.VH':
        vb_path = path.with_suffix('.VB')
        if vb_path.exists():
            vab = VABFile(data, vb_path.read_bytes())
            if vab.valid:
                print(f"\nVAB Audio:")
                print(f"  Version: {vab.version}")
                print(f"  Programs: {vab.num_programs}")
                print(f"  Tones: {vab.num_tones}")
                print(f"  Samples: {vab.num_vags}")

    elif path.suffix.upper() == '.BIN':
        # Scan for embedded TIM files
        tims = scan_for_tim(data)
        if tims:
            print(f"\nEmbedded TIM images found: {len(tims)}")
            for i, (offset, tim) in enumerate(tims[:10]):
                print(f"  [{i}] Offset 0x{offset:X}: {tim.width}x{tim.height} {tim.bpp}bpp")
            if len(tims) > 10:
                print(f"  ... and {len(tims) - 10} more")

    # Show hex dump of header
    print(f"\nHeader (first 64 bytes):")
    for i in range(0, min(64, len(data)), 16):
        hex_str = ' '.join(f'{b:02X}' for b in data[i:i+16])
        ascii_str = ''.join(chr(b) if 32 <= b < 127 else '.' for b in data[i:i+16])
        print(f"  {i:04X}: {hex_str:48} {ascii_str}")


def extract_asset(file_path: str, output_dir: Optional[str] = None):
    """Extract/convert an asset file."""
    path = Path(file_path)
    if not path.exists():
        path = ASSETS_DIR / file_path
    if not path.exists():
        print(f"File not found: {file_path}")
        return

    out_dir = Path(output_dir) if output_dir else OUTPUT_DIR
    out_dir.mkdir(parents=True, exist_ok=True)

    data = path.read_bytes()

    if path.suffix.upper() == '.TIM':
        tim = TIMFile(data)
        if tim.valid:
            png_path = out_dir / (path.stem + '.png')
            if tim.to_png(png_path):
                print(f"Converted to: {png_path}")
            else:
                print("Failed to convert TIM to PNG")

    elif path.suffix.upper() == '.VH':
        vb_path = path.with_suffix('.VB')
        if vb_path.exists():
            vab = VABFile(data, vb_path.read_bytes())
            if vab.valid:
                sample_dir = out_dir / path.stem
                count = vab.extract_samples(sample_dir)
                print(f"Extracted {count} samples to: {sample_dir}")

    elif path.suffix.upper() == '.BIN':
        # Extract embedded TIM files
        tims = scan_for_tim(data)
        if tims:
            tim_dir = out_dir / path.stem
            tim_dir.mkdir(parents=True, exist_ok=True)
            for i, (offset, tim) in enumerate(tims):
                png_path = tim_dir / f"{i:03d}_{offset:06X}.png"
                tim.to_png(png_path)
            print(f"Extracted {len(tims)} TIM images to: {tim_dir}")
        else:
            print("No extractable content found in BIN file")

    else:
        # Just copy the file
        out_path = out_dir / path.name
        out_path.write_bytes(data)
        print(f"Copied to: {out_path}")


def replace_asset(original_path: str, new_path: str):
    """Replace an asset with a new file (must be same size for in-place replacement)."""
    orig = Path(original_path)
    if not orig.exists():
        orig = ASSETS_DIR / original_path
    if not orig.exists():
        print(f"Original file not found: {original_path}")
        return

    new = Path(new_path)
    if not new.exists():
        print(f"New file not found: {new_path}")
        return

    orig_size = orig.stat().st_size
    new_size = new.stat().st_size

    if orig_size != new_size:
        print(f"WARNING: Size mismatch!")
        print(f"  Original: {orig_size:,} bytes")
        print(f"  New:      {new_size:,} bytes")
        print()
        print("For in-place disc replacement, sizes must match exactly.")
        print("Options:")
        print("  1. Resize your image to match original dimensions")
        print("  2. Use 'pack' command to rebuild the asset with new size")
        print("     (requires ISO rebuild with mkpsxiso)")
        return

    # Create backup
    backup = orig.with_suffix(orig.suffix + '.bak')
    if not backup.exists():
        orig.rename(backup)
        print(f"Backed up original to: {backup}")

    # Replace
    new_data = new.read_bytes()
    orig.write_bytes(new_data)
    print(f"Replaced: {orig}")
    print(f"Size: {new_size:,} bytes")


def pack_asset(source_path: str, output_path: str, asset_type: str = 'auto'):
    """Pack a new asset (e.g., PNG to TIM)."""
    src = Path(source_path)
    if not src.exists():
        print(f"Source file not found: {source_path}")
        return

    out = Path(output_path)

    if asset_type == 'auto':
        asset_type = out.suffix.upper().lstrip('.')

    if asset_type == 'TIM' and src.suffix.lower() == '.png':
        tim = TIMFile.from_png(src, bpp=8)
        if tim and tim.valid:
            out.parent.mkdir(parents=True, exist_ok=True)
            out.write_bytes(tim.data)
            print(f"Created TIM: {out}")
            print(f"  Size: {len(tim.data):,} bytes")
            print(f"  Dimensions: {tim.width}x{tim.height}")
            print(f"  BPP: {tim.bpp}")
        else:
            print("Failed to create TIM from PNG")
    else:
        print(f"Unsupported conversion: {src.suffix} -> {asset_type}")


def main():
    parser = argparse.ArgumentParser(description='Tokimeki Memorial Asset Manager')
    subparsers = parser.add_subparsers(dest='command', help='Commands')

    # list command
    subparsers.add_parser('list', help='List all assets')

    # info command
    info_parser = subparsers.add_parser('info', help='Show asset info')
    info_parser.add_argument('file', help='Asset file path')

    # extract command
    extract_parser = subparsers.add_parser('extract', help='Extract/convert asset')
    extract_parser.add_argument('file', help='Asset file path')
    extract_parser.add_argument('--out', '-o', help='Output directory')

    # replace command
    replace_parser = subparsers.add_parser('replace', help='Replace asset (same size)')
    replace_parser.add_argument('original', help='Original asset path')
    replace_parser.add_argument('new', help='New file path')

    # pack command
    pack_parser = subparsers.add_parser('pack', help='Pack new asset')
    pack_parser.add_argument('source', help='Source file (e.g., PNG)')
    pack_parser.add_argument('output', help='Output file (e.g., TIM)')
    pack_parser.add_argument('--type', '-t', default='auto', help='Asset type')

    args = parser.parse_args()

    if args.command == 'list':
        list_assets()
    elif args.command == 'info':
        show_info(args.file)
    elif args.command == 'extract':
        extract_asset(args.file, args.out)
    elif args.command == 'replace':
        replace_asset(args.original, args.new)
    elif args.command == 'pack':
        pack_asset(args.source, args.output, args.type)
    else:
        parser.print_help()


if __name__ == '__main__':
    main()
