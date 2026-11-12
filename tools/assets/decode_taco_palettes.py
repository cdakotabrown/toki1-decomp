#!/usr/bin/env python3
"""Extract TACO's RGB1555 palette banks while retaining its footer."""
import hashlib
import json
import struct
from pathlib import Path

from PIL import Image


ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / "assets/extracted_iso/CDROM/TACO"
OUTPUT = ROOT / "extracted_assets/nbn_proven/TACO_PALETTES"
FILES = ("SYS_COL.NBN", "ENE_COL.NBN", "C_JIKI.NBN")
FOOTER_BYTES = 2048


def color(value):
    return ((value & 31) * 255 // 31,
            ((value >> 5) & 31) * 255 // 31,
            ((value >> 10) & 31) * 255 // 31)


def main():
    OUTPUT.mkdir(parents=True, exist_ok=True)
    archives = []
    for filename in FILES:
        path = SOURCE / filename
        data = path.read_bytes()
        tail = data[-FOOTER_BYTES:]
        tail_used = 0
        for offset in range(FOOTER_BYTES - 5, -1, -1):
            if tail[offset]:
                tail_used = offset + 1
                break
        # Palette banks are 32-byte aligned; packed tail data occupies whole
        # banks even when the final colors happen to be zero.
        tail_palette_bytes = (tail_used + 31) // 32 * 32
        palette_bytes = len(data) - FOOTER_BYTES + tail_palette_bytes
        if palette_bytes <= 0 or palette_bytes % 32:
            raise ValueError(f"{filename}: invalid 16-color bank region")
        banks = palette_bytes // 32
        swatch = Image.new("RGB", (16 * 8, banks * 2))
        for bank in range(banks):
            for index in range(16):
                value = struct.unpack_from("<H", data, bank * 32 + index * 2)[0]
                rgb = color(value)
                for y in range(bank * 2, bank * 2 + 2):
                    for x in range(index * 8, index * 8 + 8):
                        swatch.putpixel((x, y), rgb)
        png = OUTPUT / f"{path.stem}.palettes.png"
        swatch.save(png, optimize=False)
        archives.append({
            "archive": path.relative_to(ROOT).as_posix(),
            "sha1": hashlib.sha1(data).hexdigest(), "size": len(data),
            "family": "taco-palette-banks-v1", "confidence": "strong",
            "evidence": "contiguous coherent 16-entry PS1 RGB1555 banks plus shared TACO footer",
            "palette_bank_count": banks,
            "preview": png.relative_to(ROOT).as_posix(),
            "regions": [
                {"type": "palette_rgb1555", "offset": 0,
                 "length": palette_bytes},
                {"type": "unknown", "offset": palette_bytes,
                 "length": len(data) - palette_bytes},
            ],
            "accounted_bytes": len(data),
            "semantically_confirmed_bytes": palette_bytes,
        })
    manifest = {"schema": "tokimemo.nbn.family-manifest/v1",
                "family": "taco-palette-banks-v1",
                "decoded_count": len(archives), "archives": archives,
                "warning": "palette banks proven; texture-to-bank bindings unresolved"}
    (OUTPUT / "manifest.json").write_text(json.dumps(manifest, indent=1) + "\n")
    print(f"decoded {len(archives)} TACO palette archives -> {OUTPUT}")


if __name__ == "__main__":
    main()
