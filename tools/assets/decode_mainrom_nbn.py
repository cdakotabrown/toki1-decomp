#!/usr/bin/env python3
"""Decode FUKUTEST/MAINROM.NBN's traced 4bpp page bank."""
import hashlib
import json
import struct
from pathlib import Path

from PIL import Image, ImageDraw


ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / "assets/extracted_iso/CDROM/FUKUTEST/MAINROM.NBN"
OUTPUT = ROOT / "extracted_assets/nbn_proven/MAINROM"
PAGE_BYTES = 8192
PAGE_COUNT = 13
TAIL_BYTES = 2048


def rgb1555(value):
    return ((value & 31) * 255 // 31,
            ((value >> 5) & 31) * 255 // 31,
            ((value >> 10) & 31) * 255 // 31)


def main():
    data = SOURCE.read_bytes()
    plane_bytes = PAGE_BYTES * PAGE_COUNT
    if len(data) != plane_bytes + TAIL_BYTES:
        raise ValueError(f"unexpected MAINROM size: {len(data)}")
    palette = []
    for offset in range(plane_bytes, plane_bytes + 32, 2):
        palette.extend(rgb1555(struct.unpack_from("<H", data, offset)[0]))
    palette += [0] * (768 - len(palette))
    OUTPUT.mkdir(parents=True, exist_ok=True)
    sheet = Image.new("RGB", (4 * 160, 4 * 156), "white")
    draw = ImageDraw.Draw(sheet)
    pages = []
    for index in range(PAGE_COUNT):
        packed = data[index * PAGE_BYTES:(index + 1) * PAGE_BYTES]
        pixels = bytearray()
        for value in packed:
            pixels.extend((value & 15, value >> 4))
        image = Image.frombytes("P", (128, 128), bytes(pixels))
        image.putpalette(palette)
        png = OUTPUT / f"page-{index:02d}.png"
        image.save(png, optimize=False)
        x, y = (index % 4) * 160, (index // 4) * 156
        sheet.paste(image.convert("RGB"), (x, y))
        draw.text((x, y + 130), f"page {index:02d} @ {index * PAGE_BYTES:#x}", fill="black")
        pages.append({"index": index, "offset": index * PAGE_BYTES,
                      "length": PAGE_BYTES,
                      "preview": png.relative_to(ROOT).as_posix()})
    sheet.save(OUTPUT / "contact-sheet.png")
    archive = {
        "archive": SOURCE.relative_to(ROOT).as_posix(),
        "sha1": hashlib.sha1(data).hexdigest(), "size": len(data),
        "family": "mainrom-page-bank-v1", "confidence": "confirmed",
        "evidence": "retail GPU trace observed 32-word x 128-row uploads at nine exact 8192-byte page offsets; file size contains thirteen pages plus tail",
        "pages": pages,
        "image_bank": {"width": 128, "height": 128, "bpp": 4,
                       "palette_offset": plane_bytes,
                       "palette_binding": "embedded"},
        "regions": [
            {"type": "pixel_4bpp_page_bank", "offset": 0,
             "length": plane_bytes},
            {"type": "palette_rgb1555", "offset": plane_bytes, "length": 32},
            {"type": "unknown", "offset": plane_bytes + 32,
             "length": TAIL_BYTES - 32},
        ],
        "accounted_bytes": len(data),
        "semantically_confirmed_bytes": plane_bytes + 32,
    }
    manifest = {"schema": "tokimemo.nbn.family-manifest/v1",
                "family": "mainrom-page-bank-v1", "decoded_count": 1,
                "archives": [archive]}
    (OUTPUT / "manifest.json").write_text(json.dumps(manifest, indent=1) + "\n")
    print(f"decoded {PAGE_COUNT} MAINROM 4bpp pages -> {OUTPUT}")


if __name__ == "__main__":
    main()
