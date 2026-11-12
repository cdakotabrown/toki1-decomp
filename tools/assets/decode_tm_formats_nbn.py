#!/usr/bin/env python3
"""Decode every local NBN described by historical TM Formats metadata."""
import argparse
import json
from pathlib import Path

from decode_lastphot_nbn import decode


ROOT = Path(__file__).resolve().parents[2]
CDROM = ROOT / "assets/extracted_iso/CDROM"
LAYOUTS = ROOT / "config/tm_formats_graphics_layouts.json"
OUTPUT = ROOT / "extracted_assets/nbn_proven/TM_FORMATS"
SUPPLEMENTAL = ROOT / "config/nbn_supplemental_layouts.json"
INFERRED = ROOT / "config/nbn_inferred_layouts.json"
FAMILY_DIR = {
    "CLUB_SD": "CLUB_SD",
    "HOLI_SD": "HOLI_SD",
    "MAIN_SD": "MAIN_SD",
    "LASTP": "LASTPHOT",
}


def decode_record(path, record, output_dir):
    layout_values = (
        record["width"], record["height"], record["pixel_offset"],
        record["palette_offset"])
    result = decode(path, output_dir, layout_values)
    result["family"] = "tm-formats-pstm-v1"
    if "evidence" in record:
        result["evidence"] = record["evidence"]
        result["confidence"] = record.get("confidence", "strong")
    else:
        result["evidence"] = (
            f"TM Formats PSTM resource {record['resource_id']} "
            f"({record['family']}), 1996")
        result["confidence"] = "confirmed"
    return result


def main():
    parser = argparse.ArgumentParser(
        description="decode NBN files documented by TM Formats")
    parser.add_argument("--layouts", type=Path, default=LAYOUTS)
    parser.add_argument("--supplemental", type=Path, default=SUPPLEMENTAL)
    parser.add_argument("--inferred", type=Path, default=INFERRED)
    parser.add_argument("--output", type=Path, default=OUTPUT)
    args = parser.parse_args()
    metadata = json.loads(args.layouts.read_text())
    records = list(metadata["records"])
    if args.supplemental.exists():
        records.extend(json.loads(args.supplemental.read_text())["records"])
    if args.inferred.exists():
        inferred = json.loads(args.inferred.read_text())
        for source_record in inferred["records"]:
            record = dict(source_record)
            record["confidence"] = inferred.get("confidence", "strong")
            record["evidence"] = inferred["method"]
            records.append(record)
    manifests = []
    missing = []
    for record in records:
        if not record["filename"].upper().endswith(".NBN"):
            continue
        family_dir = FAMILY_DIR.get(record["family"], record["family"])
        if family_dir:
            path = CDROM / family_dir / record["filename"]
        else:
            matches = sorted(CDROM.rglob(record["filename"]))
            path = matches[0] if len(matches) == 1 else Path()
        if not path.is_file():
            # ALBUM is classified with LASTP by the historical plug-in but
            # resides in FUKUTEST on this disc.
            matches = sorted(CDROM.rglob(record["filename"]))
            if len(matches) != 1:
                missing.append({"record": record, "matches": len(matches)})
                continue
            path = matches[0]
        out = args.output / record["family"]
        manifests.append(decode_record(path, record, out))

    args.output.mkdir(parents=True, exist_ok=True)
    result = {
        "schema": "tokimemo.nbn.family-manifest/v1",
        "evidence": metadata["provenance"],
        "decoded_count": len(manifests),
        "missing_count": len(missing),
        "archives": manifests,
        "missing": missing,
    }
    manifest = args.output / "manifest.json"
    manifest.write_text(json.dumps(result, indent=1) + "\n")
    print(f"decoded {len(manifests)} NBNs; {len(missing)} missing -> {args.output}")
    return 0 if manifests and not missing else 1


if __name__ == "__main__":
    raise SystemExit(main())
