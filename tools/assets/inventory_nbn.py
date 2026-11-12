#!/usr/bin/env python3
"""Freeze a deterministic inventory of the untouched NBN corpus.

This deliberately makes no format claims. It records paths, sizes, sector
counts, and cryptographic hashes so later parser/trace results always identify
their exact source bytes.
"""
import argparse
import hashlib
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
DEFAULT_SOURCE = ROOT / "assets/extracted_iso/CDROM"
DEFAULT_OUTPUT = ROOT / "extracted_assets/nbn_inventory.json"


def digest(path, name):
    h = hashlib.new(name)
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()


def main():
    parser = argparse.ArgumentParser(
        description="inventory the original Tokimeki Memorial NBN corpus")
    parser.add_argument("--source", type=Path, default=DEFAULT_SOURCE)
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    args = parser.parse_args()

    files = []
    for path in sorted(args.source.rglob("*.NBN")):
        size = path.stat().st_size
        files.append({
            "path": path.relative_to(ROOT).as_posix(),
            "size": size,
            "sectors_2048": (size + 2047) // 2048,
            "sector_aligned": size % 2048 == 0,
            "sha1": digest(path, "sha1"),
            "sha256": digest(path, "sha256"),
        })

    result = {
        "schema": "tokimemo.nbn.inventory/v1",
        "source": args.source.relative_to(ROOT).as_posix(),
        "count": len(files),
        "total_bytes": sum(item["size"] for item in files),
        "files": files,
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=1) + "\n")
    print(f"{len(files)} NBN files -> {args.output}")
    return 0 if files else 1


if __name__ == "__main__":
    raise SystemExit(main())
