#!/usr/bin/env python3
"""Decode configured raw 8bpp planes while preserving packed-tail ambiguity."""
import hashlib
import json
import struct
from pathlib import Path

from PIL import Image


ROOT = Path(__file__).resolve().parents[2]
CONFIG = ROOT / "config/nbn_raw_planes.json"
SOURCE = ROOT / "assets/extracted_iso/CDROM"
OUTPUT = ROOT / "extracted_assets/nbn_proven/RAW_PLANES"
FOOTER_BYTES = 2048


def rgb1555(value):
    return ((value & 31) * 255 // 31,
            ((value >> 5) & 31) * 255 // 31,
            ((value >> 10) & 31) * 255 // 31)


def main():
    config = json.loads(CONFIG.read_text())
    archives = []
    for record in config["records"]:
        path = SOURCE / record["family"] / record["filename"]
        data = path.read_bytes()
        plane_length = record["width"] * record["height"]
        if len(data) != plane_length + FOOTER_BYTES:
            raise ValueError(
                f"{path.name}: {len(data)} bytes, expected {plane_length + FOOTER_BYTES}")
        tail = data[plane_length:]
        has_embedded_palette = any(tail[:-4])
        image = Image.frombytes("P", (record["width"], record["height"]),
                                data[:plane_length])
        if has_embedded_palette:
            palette = []
            for offset in range(plane_length, plane_length + 512, 2):
                palette.extend(rgb1555(struct.unpack_from("<H", data, offset)[0]))
            image.putpalette(palette)
            rendering = "embedded RGB1555 palette"
            palette_binding = "embedded"
        else:
            image.putpalette([component for value in range(256)
                              for component in (value, value, value)])
            rendering = "palette-index grayscale"
            palette_binding = "external-unresolved"
        output_dir = OUTPUT / record["family"]
        output_dir.mkdir(parents=True, exist_ok=True)
        png = output_dir / f"{path.stem}.indices.png"
        image.save(png, optimize=False)
        archives.append({
            "archive": path.relative_to(ROOT).as_posix(),
            "sha1": hashlib.sha1(data).hexdigest(),
            "size": len(data),
            "family": "raw-index-plane-footer-v1",
            "confidence": "strong",
            "evidence": config["evidence"],
            "preview": {
                "path": png.relative_to(ROOT).as_posix(),
                "width": record["width"], "height": record["height"],
                "bpp": 8, "pixel_offset": 0,
                "geometry_confidence": record.get("geometry_confidence", "strong"),
                "palette_binding": palette_binding,
                "palette_offset": plane_length if has_embedded_palette else None,
                "rendering": rendering,
            },
            "regions": ([
                {"type": "pixel_indices_8bpp", "offset": 0,
                 "length": plane_length},
                {"type": "palette_rgb1555", "offset": plane_length,
                 "length": 512},
                {"type": "unknown", "offset": plane_length + 512,
                 "length": FOOTER_BYTES - 512},
            ] if has_embedded_palette else [
                {"type": "pixel_indices_8bpp", "offset": 0,
                 "length": plane_length},
                {"type": "unknown", "offset": plane_length,
                 "length": FOOTER_BYTES},
            ]),
            "accounted_bytes": len(data),
            "semantically_confirmed_bytes": plane_length + (512 if has_embedded_palette else 0),
        })
    OUTPUT.mkdir(parents=True, exist_ok=True)
    manifest = {
        "schema": "tokimemo.nbn.family-manifest/v1",
        "family": "raw-index-plane-footer-v1",
        "decoded_count": len(archives),
        "warning": "index planes proven; packed tail-sector contents, external palettes, and tentative preview orientations unresolved",
        "archives": archives,
    }
    (OUTPUT / "manifest.json").write_text(json.dumps(manifest, indent=1) + "\n")
    print(f"decoded {len(archives)} raw NBN planes -> {OUTPUT}")


if __name__ == "__main__":
    main()
