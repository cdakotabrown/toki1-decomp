#!/usr/bin/env python3
"""Cluster the NBN corpus without pretending byte similarity proves a layout.

The report separates archives covered by evidence-backed manifests from the
remaining corpus.  Unsolved archives are grouped by directory, normalized
filename, size, and a coarse per-sector byte profile.  Known layouts with the
same total size are listed only as candidates for representative validation.
"""
import argparse
import hashlib
import json
import math
import re
from collections import Counter, defaultdict
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / "assets/extracted_iso/CDROM"
MANIFEST_ROOT = ROOT / "extracted_assets/nbn_proven"
DEFAULT_OUTPUT = ROOT / "extracted_assets/nbn_family_report.json"
SECTOR = 2048


def entropy(data):
    if not data:
        return 0.0
    counts = Counter(data)
    length = len(data)
    return -sum((count / length) * math.log2(count / length)
                for count in counts.values())


def sector_class(data):
    zero = data.count(0) / len(data)
    ent = entropy(data)
    if zero == 1.0:
        return "Z"
    if zero >= 0.75:
        return "z"
    if ent < 3.0:
        return "L"
    if ent < 6.0:
        return "M"
    if ent < 7.5:
        return "H"
    return "X"


def profile(data):
    return "".join(sector_class(data[offset:offset + SECTOR])
                   for offset in range(0, len(data), SECTOR))


def name_pattern(name):
    stem = Path(name).stem.upper()
    # Preserve semantic suffixes such as W/R while folding numeric and hex IDs.
    stem = re.sub(r"(?<![A-Z])[0-9A-F]+(?![A-Z])", "#", stem)
    stem = re.sub(r"\d+", "#", stem)
    return stem


def load_known(manifest_root):
    known = {}
    layouts_by_size = defaultdict(list)
    for manifest in sorted(manifest_root.rglob("manifest.json")):
        document = json.loads(manifest.read_text())
        for archive in document.get("archives", []):
            rel = archive["archive"]
            known[rel] = {
                "family": archive.get("family", document.get("family", "known")),
                "confidence": archive.get("confidence", "confirmed"),
                "manifest": manifest.relative_to(ROOT).as_posix(),
            }
            image = archive.get("image")
            if image and image.get("palette_offset") is not None:
                key = (archive["size"], image.get("width"), image.get("height"),
                       image.get("bpp"), image.get("pixel_offset"),
                       image.get("palette_offset"))
                layouts_by_size[archive["size"]].append(key[1:])
    # TITLE is independently measured but its dedicated manifest is not yet in
    # nbn_proven. Keep that distinction explicit in the report.
    title = "assets/extracted_iso/CDROM/FUKUTEST/TITLE.NBN"
    if (ROOT / title).is_file():
        known[title] = {
            "family": "title-v1",
            "confidence": "confirmed",
            "manifest": None,
            "note": "established sector layout; dedicated manifest pending",
        }
    return known, layouts_by_size


def collapse_profile(value):
    return re.sub(r"(.)\1{3,}", lambda m: f"{m.group(1)}{{{len(m.group(0))}}}",
                  value)


def main():
    parser = argparse.ArgumentParser(
        description="classify solved and unresolved NBN archive families")
    parser.add_argument("--source", type=Path, default=SOURCE)
    parser.add_argument("--manifests", type=Path, default=MANIFEST_ROOT)
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    args = parser.parse_args()

    known, layouts_by_size = load_known(args.manifests)
    records = []
    for path in sorted(args.source.rglob("*.NBN")):
        data = path.read_bytes()
        rel = path.relative_to(ROOT).as_posix()
        directory = path.parent.relative_to(args.source).as_posix()
        records.append({
            "archive": rel,
            "directory": directory,
            "filename": path.name,
            "name_pattern": name_pattern(path.name),
            "size": len(data),
            "sectors": (len(data) + SECTOR - 1) // SECTOR,
            "sector_aligned": len(data) % SECTOR == 0,
            "sha1": hashlib.sha1(data).hexdigest(),
            "profile": profile(data),
            "known": known.get(rel),
        })

    unresolved = [record for record in records if not record["known"]]
    groups = defaultdict(list)
    for record in unresolved:
        key = (record["directory"], record["name_pattern"], record["size"])
        groups[key].append(record)

    clusters = []
    for index, (key, members) in enumerate(sorted(groups.items()), 1):
        profiles = Counter(member["profile"] for member in members)
        layouts = sorted(set(layouts_by_size.get(key[2], [])), key=str)
        clusters.append({
            "id": f"candidate-{index:02d}",
            "directory": key[0],
            "name_pattern": key[1],
            "size": key[2],
            "count": len(members),
            "representative": members[0]["archive"],
            "members": [member["archive"] for member in members],
            "profile_variants": len(profiles),
            "profiles": [{"profile": collapse_profile(value), "count": count}
                         for value, count in profiles.most_common()],
            "same_size_known_layout_candidates": [
                {"width": layout[0], "height": layout[1], "bpp": layout[2],
                 "pixel_offset": layout[3], "palette_offset": layout[4]}
                for layout in layouts
            ],
            "classification": "candidate",
            "warning": "grouping is triage evidence, not a proven file layout",
        })

    result = {
        "schema": "tokimemo.nbn.family-classification/v1",
        "method": {
            "group_key": ["directory", "normalized filename", "exact size"],
            "sector_profile": "2048-byte entropy/zero-density classes",
            "claim_boundary": "clusters select representatives; they do not prove layouts",
        },
        "summary": {
            "archives": len(records),
            "evidence_backed": len(records) - len(unresolved),
            "unresolved": len(unresolved),
            "candidate_clusters": len(clusters),
        },
        "known_families": dict(sorted(Counter(
            record["known"]["family"] for record in records if record["known"]
        ).items())),
        "candidate_clusters": clusters,
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=1) + "\n")
    summary = result["summary"]
    print(f"{summary['evidence_backed']}/{summary['archives']} evidence-backed; "
          f"{summary['unresolved']} unresolved in "
          f"{summary['candidate_clusters']} candidate clusters -> {args.output}")
    return 0 if records else 1


if __name__ == "__main__":
    raise SystemExit(main())
