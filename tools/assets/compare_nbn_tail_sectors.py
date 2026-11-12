#!/usr/bin/env python3
"""Compare packed 2 KiB tail sectors without assuming they are padding."""
import argparse
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / "assets/extracted_iso/CDROM"
OUTPUT = ROOT / "extracted_assets/nbn_tail_sectors.json"
SECTOR = 2048
MARKER = b"\x12\x10"


def used_end(tail):
    for offset in range(len(tail) - 5, -1, -1):
        if tail[offset]:
            return offset + 1
    return 0


def main():
    parser = argparse.ArgumentParser(description="analyze NBN final sectors")
    parser.add_argument("--source", type=Path, default=SOURCE)
    parser.add_argument("--output", type=Path, default=OUTPUT)
    args = parser.parse_args()
    records = []
    for path in sorted(args.source.rglob("*.NBN")):
        data = path.read_bytes()
        if len(data) < SECTOR or data[-2:] != MARKER:
            continue
        tail = data[-SECTOR:]
        raw_checksum = data[-4:-2]
        byte_sum = sum(data[:-4]) & 0xffff
        records.append({
            "archive": path.relative_to(ROOT).as_posix(),
            "size": len(data),
            "tail_offset": len(data) - SECTOR,
            "tail_payload_bytes": used_end(tail),
            "zero_padding_bytes": SECTOR - 4 - used_end(tail),
            "trailer_hex": data[-4:].hex(),
            "byte_sum_16": f"{byte_sum:04x}",
            "checksum_matches_byte_sum_bytes": raw_checksum == byte_sum.to_bytes(2, "big"),
        })
    result = {
        "schema": "tokimemo.nbn.tail-sector-analysis/v1",
        "marker_hex": MARKER.hex(),
        "warning": "tail payload boundary is measured; its semantics are not inferred",
        "summary": {
            "archives_with_marker": len(records),
            "empty_tail_payloads": sum(not r["tail_payload_bytes"] for r in records),
            "packed_tail_payloads": sum(bool(r["tail_payload_bytes"]) for r in records),
            "byte_sum_matches": sum(r["checksum_matches_byte_sum_bytes"] for r in records),
        },
        "archives": records,
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=1) + "\n")
    print(json.dumps(result["summary"]))


if __name__ == "__main__":
    main()
