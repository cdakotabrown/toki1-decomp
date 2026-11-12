#!/usr/bin/env python3
"""Safely repack an evidence-backed indexed NBN image in place.

Unknown bytes, descriptor data, padding, and palette words are copied from the
original archive. An optional indexed PNG may replace only the confirmed pixel
region and must retain identical geometry and palette. Relocation is forbidden.
"""
import argparse
import hashlib
import json
from pathlib import Path

from PIL import Image


ROOT = Path(__file__).resolve().parents[2]


def find_archive(document, name):
    matches = [item for item in document.get("archives", [])
               if item["archive"] == name or Path(item["archive"]).name == name]
    if len(matches) != 1:
        raise ValueError(f"archive selector matched {len(matches)} entries: {name}")
    return matches[0]


def ps1_palette(data, offset, color_count):
    palette = []
    for index in range(color_count):
        value = int.from_bytes(data[offset + index * 2:offset + index * 2 + 2],
                               "little")
        palette.extend([
            (value & 0x1f) * 255 // 31,
            ((value >> 5) & 0x1f) * 255 // 31,
            ((value >> 10) & 0x1f) * 255 // 31,
        ])
    return palette


def expected_palette(original, image_info):
    bpp = image_info["bpp"]
    color_count = 1 << bpp
    if "palette_offset" in image_info:
        palette_data = original
        palette_offset = image_info["palette_offset"]
    else:
        palette_path = ROOT / image_info["palette_archive"]
        palette_data = palette_path.read_bytes()
        palette_offset = image_info.get("preview_palette_bank", 0) * color_count * 2
    palette = ps1_palette(palette_data, palette_offset, color_count)
    return palette + [0] * (768 - len(palette))


def encode_pixels(image, bpp):
    pixels = image.tobytes()
    if bpp == 8:
        return pixels
    if bpp != 4:
        raise ValueError(f"unsupported indexed depth: {bpp}bpp")
    if len(pixels) % 2:
        raise ValueError("4bpp image has an odd pixel count")
    if any(pixel > 15 for pixel in pixels):
        raise ValueError("4bpp replacement uses a palette index above 15")
    packed = bytearray(len(pixels) // 2)
    for offset in range(0, len(pixels), 2):
        packed[offset // 2] = pixels[offset] | (pixels[offset + 1] << 4)
    return bytes(packed)


def main():
    parser = argparse.ArgumentParser(
        description="fixed-layout repacker for manifested 4/8bpp NBN images")
    parser.add_argument("manifest", type=Path)
    parser.add_argument("archive", help="manifest archive path or basename")
    parser.add_argument("output", type=Path)
    parser.add_argument("--png", type=Path,
                        help="optional indexed replacement PNG")
    args = parser.parse_args()

    document = json.loads(args.manifest.read_text())
    entry = find_archive(document, args.archive)
    source = ROOT / entry["archive"]
    original = source.read_bytes()
    if hashlib.sha1(original).hexdigest() != entry["sha1"]:
        raise ValueError("original archive SHA-1 does not match manifest")
    image_info = entry["image"]
    if image_info["bpp"] not in (4, 8):
        raise ValueError("only manifested 4/8bpp indexed images are supported")
    output = bytearray(original)

    changed_start = None
    changed_end = None
    if args.png:
        image = Image.open(args.png)
        if image.mode != "P":
            raise ValueError("replacement PNG must remain indexed (mode P)")
        expected_size = (image_info["width"], image_info["height"])
        if image.size != expected_size:
            raise ValueError(
                f"replacement is {image.size}, expected {expected_size}")
        source_palette = expected_palette(original, image_info)
        replacement_palette = (image.getpalette() or [])[:768]
        if replacement_palette != source_palette:
            raise ValueError("replacement palette differs; palette edits are forbidden")
        pixels = encode_pixels(image, image_info["bpp"])
        expected_bytes = image_info["width"] * image_info["height"] * image_info["bpp"] // 8
        if len(pixels) != expected_bytes:
            raise ValueError(
                f"encoded payload is {len(pixels)} bytes, expected {expected_bytes}")
        changed_start = image_info["pixel_offset"]
        changed_end = changed_start + len(pixels)
        if changed_end > len(output):
            raise ValueError("replacement pixel range exceeds archive")
        output[changed_start:changed_end] = pixels

    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(output)
    if args.png is None:
        if hashlib.sha1(output).hexdigest() != entry["sha1"]:
            raise ValueError("no-edit round trip is not byte-identical")
        verdict = "byte-identical no-edit round trip"
    else:
        for index, (before, after) in enumerate(zip(original, output)):
            if before != after and not (changed_start <= index < changed_end):
                raise ValueError(f"unauthorized byte changed at {index:#x}")
        verdict = f"pixel-only patch {changed_start:#x}-{changed_end:#x}"
    print(f"{verdict}: {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
