#!/usr/bin/env python3
"""Extract TACO's exact 64 KiB indexed texture planes without guessing CLUTs."""
import hashlib
import json
import struct
from pathlib import Path

from PIL import Image, ImageDraw


ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / "assets/extracted_iso/CDROM/TACO"
OUTPUT = ROOT / "extracted_assets/nbn_proven/TACO_TEXTURES"
FILES = {
    "BS00.NBN", "BS01.NBN", "JIMEN.NBN", "TACOJIKI.NBN", "T_DRA.NBN",
    "ZK_0.NBN", "ZK_1.NBN", "ZK_2.NBN", "TACOTEX.NBN", "T_EX0.NBN",
    "T_JIKI.NBN", "T_NICHE.NBN", "T_SKIN.NBN", "T_TAKO1.NBN",
    "T_TAKO2.NBN", "T_ZA00.NBN",
}
DIMENSIONS = {filename: (256, 256) for filename in FILES}
DIMENSIONS["IWA.NBN"] = (128, 128)


def rgb1555(value):
    return ((value & 31) * 255 // 31,
            ((value >> 5) & 31) * 255 // 31,
            ((value >> 10) & 31) * 255 // 31)


def main():
    OUTPUT.mkdir(parents=True, exist_ok=True)
    archives = []
    thumbs = []
    for filename, dimensions in sorted(DIMENSIONS.items()):
        path = SOURCE / filename
        data = path.read_bytes()
        plane_bytes = dimensions[0] * dimensions[1]
        if len(data) not in (plane_bytes, plane_bytes + 2048):
            raise ValueError(f"{filename}: unexpected size {len(data)}")
        tail = data[plane_bytes:]
        has_embedded_palette = len(tail) >= 512 and any(tail[:-4])
        image = Image.frombytes("P", dimensions, data[:plane_bytes])
        if has_embedded_palette:
            palette = []
            for offset in range(plane_bytes, plane_bytes + 512, 2):
                palette.extend(rgb1555(struct.unpack_from("<H", data, offset)[0]))
            image.putpalette(palette)
        else:
            image.putpalette([component for value in range(256)
                              for component in (value, value, value)])
        png = OUTPUT / f"{path.stem}.indices.png"
        image.save(png, optimize=False)
        thumb = image.convert("RGB")
        thumb.thumbnail((128, 128))
        thumbs.append((thumb, filename))
        regions = [{"type": "pixel_indices_8bpp", "offset": 0,
                    "length": plane_bytes}]
        if has_embedded_palette:
            regions.append({"type": "palette_rgb1555", "offset": plane_bytes,
                            "length": 512})
            regions.append({"type": "unknown", "offset": plane_bytes + 512,
                            "length": len(data) - plane_bytes - 512})
        elif len(data) > plane_bytes:
            regions.append({"type": "unknown", "offset": plane_bytes,
                            "length": len(data) - plane_bytes})
        archives.append({
            "archive": path.relative_to(ROOT).as_posix(),
            "sha1": hashlib.sha1(data).hexdigest(),
            "size": len(data),
            "family": "taco-texture-plane-v1",
            "confidence": "strong",
            "evidence": "exact 256x256 byte plane shared across named TACO texture family",
            "preview": {
                "path": png.relative_to(ROOT).as_posix(),
                "width": dimensions[0], "height": dimensions[1], "bpp": 8,
                "pixel_offset": 0,
                "palette_binding": "embedded" if has_embedded_palette else "external-unresolved",
                "palette_offset": plane_bytes if has_embedded_palette else None,
                "rendering": "embedded RGB1555 palette" if has_embedded_palette else "palette-index grayscale",
            },
            "regions": regions,
            "accounted_bytes": len(data),
            "semantically_confirmed_bytes": plane_bytes + (512 if has_embedded_palette else 0),
        })
    sheet = Image.new("RGB", (4 * 160, 4 * 156), "white")
    draw = ImageDraw.Draw(sheet)
    for index, (thumb, label) in enumerate(thumbs):
        x, y = (index % 4) * 160, (index // 4) * 156
        sheet.paste(thumb, (x, y))
        draw.text((x, y + 130), label, fill="black")
    sheet.save(OUTPUT / "contact-sheet.indices.png")
    manifest = {
        "schema": "tokimemo.nbn.family-manifest/v1",
        "family": "taco-texture-plane-v1",
        "decoded_count": len(archives),
        "warning": "index planes proven; color palette bindings unresolved",
        "archives": archives,
    }
    (OUTPUT / "manifest.json").write_text(json.dumps(manifest, indent=1) + "\n")
    print(f"decoded {len(archives)} TACO index planes -> {OUTPUT}")


if __name__ == "__main__":
    main()
