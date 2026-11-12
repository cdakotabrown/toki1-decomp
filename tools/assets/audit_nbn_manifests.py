#!/usr/bin/env python3
"""Verify source hashes and byte accounting in generated NBN manifests."""
import argparse
import hashlib
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
DEFAULT_ROOT = ROOT / "extracted_assets/nbn_proven"


def audit_archive(archive):
    problems = []
    path = ROOT / archive["archive"]
    if not path.is_file():
        return [f"missing source: {path}"]
    data = path.read_bytes()
    if len(data) != archive["size"]:
        problems.append(f"size: manifest={archive['size']} actual={len(data)}")
    if hashlib.sha1(data).hexdigest() != archive["sha1"]:
        problems.append("SHA-1 mismatch")
    cursor = 0
    for region in sorted(archive["regions"], key=lambda item: item["offset"]):
        if region["offset"] != cursor:
            problems.append(
                f"gap/overlap at {cursor:#x}, next={region['offset']:#x}")
        if region["length"] < 0:
            problems.append(f"negative region length at {region['offset']:#x}")
        cursor = region["offset"] + region["length"]
    if cursor != len(data):
        problems.append(f"coverage ends at {cursor:#x}, file ends at {len(data):#x}")
    if archive.get("accounted_bytes") != len(data):
        problems.append("accounted_bytes does not equal file size")
    return problems


def main():
    parser = argparse.ArgumentParser(
        description="audit generated evidence-backed NBN manifests")
    parser.add_argument("root", nargs="?", type=Path, default=DEFAULT_ROOT)
    args = parser.parse_args()
    archives = {}
    for path in sorted(args.root.rglob("manifest.json")):
        document = json.loads(path.read_text())
        for archive in document.get("archives", []):
            archives[archive["archive"]] = archive
    failures = {}
    for name, archive in sorted(archives.items()):
        problems = audit_archive(archive)
        if problems:
            failures[name] = problems
    result = {
        "manifests": len(list(args.root.rglob("manifest.json"))),
        "archives": len(archives),
        "passed": len(archives) - len(failures),
        "failed": failures,
    }
    print(json.dumps(result, indent=1))
    return 1 if failures or not archives else 0


if __name__ == "__main__":
    raise SystemExit(main())
