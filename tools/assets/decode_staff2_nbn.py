#!/usr/bin/env python3
"""Decode the structurally uniform STAFF2 portrait/palette NBN family."""
import hashlib
import json
import struct
from pathlib import Path

from PIL import Image

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / "assets/extracted_iso/CDROM/STAFF2"
OUTPUT = ROOT / "extracted_assets/nbn_proven/STAFF2"
WIDTH, HEIGHT = 128, 400
PIXEL_BYTES = WIDTH * HEIGHT // 2


def rgb1555(value):
    return ((value & 31) * 255 // 31,
            ((value >> 5) & 31) * 255 // 31,
            ((value >> 10) & 31) * 255 // 31)


def palettes(data):
    result = []
    for bank_offset in range(0, len(data), 32):
        bank = []
        for offset in range(bank_offset, bank_offset + 32, 2):
            bank.extend(rgb1555(struct.unpack_from("<H", data, offset)[0]))
        result.append(bank)
    return result


def indexed_pixels(data):
    pixels = bytearray()
    for value in data:
        pixels.extend((value & 15, value >> 4))
    return bytes(pixels)


def main():
    OUTPUT.mkdir(parents=True, exist_ok=True)
    palette_path = SOURCE / "STAF2COL.NBN"
    palette_data = palette_path.read_bytes()
    banks = palettes(palette_data)
    if len(banks) != 64:
        raise ValueError(f"expected 64 palette banks, found {len(banks)}")

    swatch = Image.new("RGB", (16 * 8, len(banks) * 4))
    for bank_index, bank in enumerate(banks):
        for color_index in range(16):
            color = tuple(bank[color_index * 3:color_index * 3 + 3])
            for y in range(bank_index * 4, bank_index * 4 + 4):
                for x in range(color_index * 8, color_index * 8 + 8):
                    swatch.putpixel((x, y), color)
    swatch.save(OUTPUT / "STAF2COL.palettes.png")

    archives = [{
        "archive": palette_path.relative_to(ROOT).as_posix(),
        "sha1": hashlib.sha1(palette_data).hexdigest(),
        "size": len(palette_data),
        "family": "staff2-v1",
        "confidence": "strong",
        "evidence": "exactly 64 contiguous PS1 16-color RGB1555 banks",
        "palette_bank_count": 64,
        "regions": [{"type": "palette_rgb1555", "offset": 0,
                     "length": len(palette_data)}],
        "accounted_bytes": len(palette_data),
        "semantically_confirmed_bytes": len(palette_data),
    }]
    for path in sorted(SOURCE.glob("STAF2*.NBN")):
        if path == palette_path:
            continue
        data = path.read_bytes()
        if len(data) != 26624:
            raise ValueError(f"unexpected STAFF2 size: {path} ({len(data)})")
        image = Image.frombytes("P", (WIDTH, HEIGHT),
                                indexed_pixels(data[:PIXEL_BYTES]))
        image.putpalette(banks[0] + [0] * (768 - len(banks[0])))
        png = OUTPUT / (path.stem + ".png")
        image.save(png, optimize=False)
        archives.append({
            "archive": path.relative_to(ROOT).as_posix(),
            "sha1": hashlib.sha1(data).hexdigest(),
            "size": len(data),
            "family": "staff2-v1",
            "confidence": "strong",
            "evidence": "uniform 128x400 4bpp payload produces three coherent portraits; exact 0x400-byte footer retained",
            "image": {"path": png.relative_to(ROOT).as_posix(),
                      "width": WIDTH, "height": HEIGHT, "bpp": 4,
                      "pixel_offset": 0,
                      "palette_archive": palette_path.relative_to(ROOT).as_posix(),
                      "preview_palette_bank": 0,
                      "palette_binding": "runtime-selection-unresolved"},
            "regions": [
                {"type": "pixel_4bpp", "offset": 0,
                 "length": PIXEL_BYTES},
                {"type": "unknown", "offset": PIXEL_BYTES,
                 "length": len(data) - PIXEL_BYTES}],
            "accounted_bytes": len(data),
            "semantically_confirmed_bytes": PIXEL_BYTES,
        })
    manifest = {"schema": "tokimemo.nbn.family-manifest/v1",
                "family": "staff2-v1", "decoded_count": len(archives),
                "archives": archives}
    (OUTPUT / "manifest.json").write_text(json.dumps(manifest, indent=1) + "\n")
    print(f"decoded {len(archives) - 1} portrait sheets + 64 palettes -> {OUTPUT}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
