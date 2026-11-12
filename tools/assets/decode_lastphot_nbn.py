#!/usr/bin/env python3
"""Decode the historically documented LASTPHOT NBN family.

The dimensions and offsets come from the PSTM resources embedded in Tadanori
Tezuka's 1996 freeware Photoshop plug-in "TM Formats", not from image
heuristics. The original plug-in declares:

  ALL_C.NBN     384x256, 8bpp pixels at 0, palette at 0x18000
  LASTP_*.NBN   256x256, 8bpp pixels at 0, palette at 0x10000

The palette is 256 little-endian PS1 RGB1555 words. Bytes outside the pixel and
palette regions are retained in the manifest as unknown; this decoder never
modifies the source archive.
"""
import argparse
import hashlib
import json
import struct
from pathlib import Path

from PIL import Image


ROOT = Path(__file__).resolve().parents[2]
DEFAULT_SOURCE = ROOT / "assets/extracted_iso/CDROM/LASTPHOT"
DEFAULT_OUTPUT = ROOT / "extracted_assets/nbn_proven/LASTPHOT"


def rgb1555(value):
    r = (value & 0x1f) * 255 // 31
    g = ((value >> 5) & 0x1f) * 255 // 31
    b = ((value >> 10) & 0x1f) * 255 // 31
    return r, g, b


def layout(path):
    if path.name == "ALL_C.NBN":
        return 384, 256, 0, 0x18000
    if path.name.startswith("LASTP_"):
        return 256, 256, 0, 0x10000
    raise ValueError(f"not a documented LASTPHOT archive: {path.name}")


def decode(path, output_dir, layout_values=None):
    data = path.read_bytes()
    width, height, pixel_offset, palette_offset = (
        layout_values if layout_values is not None else layout(path))
    pixel_size = width * height
    palette_size = 256 * 2
    if pixel_offset + pixel_size > len(data):
        raise ValueError(f"{path.name}: pixel region exceeds file")
    if palette_offset + palette_size > len(data):
        raise ValueError(f"{path.name}: palette region exceeds file")

    palette = []
    for offset in range(palette_offset, palette_offset + palette_size, 2):
        palette.extend(rgb1555(struct.unpack_from("<H", data, offset)[0]))
    image = Image.frombytes("P", (width, height),
                            data[pixel_offset:pixel_offset + pixel_size])
    image.putpalette(palette)

    output_dir.mkdir(parents=True, exist_ok=True)
    png = output_dir / (path.stem + ".png")
    image.save(png, optimize=False)

    covered = sorted([
        (pixel_offset, pixel_offset + pixel_size, "pixel_8bpp"),
        (palette_offset, palette_offset + palette_size, "palette_rgb1555"),
    ])
    regions = []
    cursor = 0
    for start, end, kind in covered:
        if cursor < start:
            regions.append({"type": "unknown", "offset": cursor,
                            "length": start - cursor})
        regions.append({"type": kind, "offset": start,
                        "length": end - start})
        cursor = end
    if cursor < len(data):
        regions.append({"type": "unknown", "offset": cursor,
                        "length": len(data) - cursor})

    return {
        "archive": path.relative_to(ROOT).as_posix(),
        "sha1": hashlib.sha1(data).hexdigest(),
        "size": len(data),
        "family": "lastphot-tm-formats-v1",
        "evidence": "TM Formats PSTM resource 134 (LASTP), 1996",
        "image": {
            "path": png.relative_to(ROOT).as_posix(),
            "width": width,
            "height": height,
            "bpp": 8,
            "pixel_offset": pixel_offset,
            "palette_offset": palette_offset,
        },
        "regions": regions,
        "accounted_bytes": len(data),
        "semantically_confirmed_bytes": pixel_size + palette_size,
    }


def main():
    parser = argparse.ArgumentParser(
        description="decode evidence-backed CDROM/LASTPHOT NBN images")
    parser.add_argument("--source", type=Path, default=DEFAULT_SOURCE)
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    args = parser.parse_args()

    manifests = []
    for path in sorted(args.source.glob("*.NBN")):
        manifests.append(decode(path, args.output))
    result = {
        "schema": "tokimemo.nbn.family-manifest/v1",
        "family": "lastphot-tm-formats-v1",
        "count": len(manifests),
        "archives": manifests,
    }
    manifest_path = args.output / "manifest.json"
    manifest_path.write_text(json.dumps(result, indent=1) + "\n")
    print(f"decoded {len(manifests)} LASTPHOT archives -> {args.output}")
    return 0 if manifests else 1


if __name__ == "__main__":
    raise SystemExit(main())
