#!/usr/bin/env python3
"""Decode EN_GAME/KINGYO.NBN's exact 256x256 RGB1555 plane."""
import hashlib
import json
import struct
from pathlib import Path

from PIL import Image


ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / "assets/extracted_iso/CDROM/EN_GAME/KINGYO.NBN"
OUTPUT = ROOT / "extracted_assets/nbn_proven/KINGYO"


def main():
    data = SOURCE.read_bytes()
    if len(data) != 256 * 256 * 2:
        raise ValueError(f"unexpected KINGYO size: {len(data)}")
    pixels = bytearray()
    for offset in range(0, len(data), 2):
        value = struct.unpack_from("<H", data, offset)[0]
        pixels.extend(((value & 31) * 255 // 31,
                       ((value >> 5) & 31) * 255 // 31,
                       ((value >> 10) & 31) * 255 // 31))
    OUTPUT.mkdir(parents=True, exist_ok=True)
    png = OUTPUT / "KINGYO.png"
    Image.frombytes("RGB", (256, 256), bytes(pixels)).save(png, optimize=False)
    archive = {
        "archive": SOURCE.relative_to(ROOT).as_posix(),
        "sha1": hashlib.sha1(data).hexdigest(),
        "size": len(data),
        "family": "kingyo-rgb1555-v1",
        "confidence": "strong",
        "evidence": "entire file is a coherent 256x256 PS1 RGB1555 atlas",
        "image": {"path": png.relative_to(ROOT).as_posix(), "width": 256,
                  "height": 256, "bpp": 16, "pixel_offset": 0},
        "regions": [{"type": "pixel_rgb1555", "offset": 0,
                     "length": len(data)}],
        "accounted_bytes": len(data),
        "semantically_confirmed_bytes": len(data),
    }
    manifest = {"schema": "tokimemo.nbn.family-manifest/v1",
                "family": "kingyo-rgb1555-v1", "decoded_count": 1,
                "archives": [archive]}
    (OUTPUT / "manifest.json").write_text(json.dumps(manifest, indent=1) + "\n")
    print(f"decoded KINGYO RGB1555 atlas -> {OUTPUT}")


if __name__ == "__main__":
    main()
