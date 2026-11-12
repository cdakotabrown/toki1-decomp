#!/usr/bin/env python3
"""Render explicitly tentative NBN previews from same-size proven layouts."""
import argparse
import json
import struct
from pathlib import Path

from PIL import Image, ImageDraw


ROOT = Path(__file__).resolve().parents[2]
REPORT = ROOT / "extracted_assets/nbn_family_report.json"
OUTPUT = ROOT / "extracted_assets/nbn_candidates"


def rgb1555(value):
    return ((value & 31) * 255 // 31,
            ((value >> 5) & 31) * 255 // 31,
            ((value >> 10) & 31) * 255 // 31)


def render(path, layout):
    data = path.read_bytes()
    width, height = layout["width"], layout["height"]
    pixel_offset = layout["pixel_offset"]
    palette_offset = layout["palette_offset"]
    if layout["bpp"] != 8 or palette_offset is None:
        return None
    end = pixel_offset + width * height
    palette_end = palette_offset + 512
    if end > len(data) or palette_end > len(data):
        return None
    palette = []
    for offset in range(palette_offset, palette_end, 2):
        palette.extend(rgb1555(struct.unpack_from("<H", data, offset)[0]))
    image = Image.frombytes("P", (width, height), data[pixel_offset:end])
    image.putpalette(palette)
    return image.convert("RGB")


def edge_score(image):
    """Mean neighboring RGB distance; descriptive, never proof of a layout."""
    pixels = image.load()
    width, height = image.size
    total = count = 0
    for y in range(height):
        for x in range(width):
            here = pixels[x, y]
            if x:
                total += sum(abs(here[c] - pixels[x - 1, y][c]) for c in range(3))
                count += 1
            if y:
                total += sum(abs(here[c] - pixels[x, y - 1][c]) for c in range(3))
                count += 1
    return round(total / count, 3) if count else 0.0


def main():
    parser = argparse.ArgumentParser(description="render tentative NBN layout candidates")
    parser.add_argument("--report", type=Path, default=REPORT)
    parser.add_argument("--output", type=Path, default=OUTPUT)
    args = parser.parse_args()
    report = json.loads(args.report.read_text())
    results = []
    thumbs = []
    for cluster in report["candidate_clusters"]:
        for archive in cluster["members"]:
            path = ROOT / archive
            for layout_index, layout in enumerate(
                    cluster["same_size_known_layout_candidates"]):
                image = render(path, layout)
                if image is None:
                    continue
                relative = Path(path.parent.name) / (
                    f"{path.stem}.layout-{layout_index + 1}.png")
                output_path = args.output / relative
                output_path.parent.mkdir(parents=True, exist_ok=True)
                image.save(output_path, optimize=False)
                score = edge_score(image)
                results.append({
                    "archive": archive,
                    "cluster": cluster["id"],
                    "layout": layout,
                    "preview": output_path.relative_to(ROOT).as_posix(),
                    "edge_score": score,
                    "status": "candidate-not-proven",
                })
                thumb = image.copy()
                thumb.thumbnail((192, 128))
                thumbs.append((thumb, f"{path.parent.name}/{path.name} L{layout_index + 1}"))

    if thumbs:
        cell_w, cell_h, columns = 240, 160, 4
        rows = (len(thumbs) + columns - 1) // columns
        sheet = Image.new("RGB", (columns * cell_w, rows * cell_h), "white")
        draw = ImageDraw.Draw(sheet)
        for index, (thumb, label) in enumerate(thumbs):
            x = (index % columns) * cell_w
            y = (index // columns) * cell_h
            sheet.paste(thumb, (x, y))
            draw.text((x, y + 132), label[:38], fill="black")
        sheet.save(args.output / "contact-sheet.png")

    manifest = {
        "schema": "tokimemo.nbn.candidate-previews/v1",
        "warning": "same-size layout trials; no preview is evidence-backed",
        "count": len(results),
        "previews": results,
    }
    args.output.mkdir(parents=True, exist_ok=True)
    (args.output / "manifest.json").write_text(json.dumps(manifest, indent=1) + "\n")
    print(f"rendered {len(results)} tentative previews -> {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
