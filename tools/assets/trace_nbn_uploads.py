#!/usr/bin/env python3
"""Validate and summarize NBN provenance JSONL emitted by the PC port.

The raw trace is intentionally the authority: this tool does not guess bpp,
dimensions, palettes, or archive families. It groups exact source ranges and
GPU word rectangles and can emit a compact evidence file for later manifests.
"""

from __future__ import annotations

import argparse
import json
from collections import Counter, defaultdict
from pathlib import Path
from typing import Any


def load_events(path: Path) -> list[dict[str, Any]]:
    events: list[dict[str, Any]] = []
    with path.open("r", encoding="utf-8") as stream:
        for line_number, line in enumerate(stream, 1):
            line = line.strip()
            if not line:
                continue
            try:
                event = json.loads(line)
            except json.JSONDecodeError as exc:
                raise SystemExit(f"{path}:{line_number}: invalid JSON: {exc}") from exc
            if not isinstance(event, dict) or "event" not in event:
                raise SystemExit(f"{path}:{line_number}: event object required")
            events.append(event)
    return events


def summarize(events: list[dict[str, Any]]) -> dict[str, Any]:
    loads = [event for event in events if event["event"] == "file_load"]
    uploads = [event for event in events if event["event"] == "load_image"]
    moves = [event for event in events if event["event"] == "move_image"]
    by_archive: dict[str, list[dict[str, Any]]] = defaultdict(list)

    for event in uploads:
        archive = event.get("archive")
        rect = event.get("rect", {})
        required = ("source_offset", "source_length")
        if not isinstance(archive, str) or any(key not in event for key in required):
            raise SystemExit(f"malformed load_image event {event.get('event_id', '?')}")
        if any(key not in rect for key in ("x", "y", "w_words", "h")):
            raise SystemExit(f"malformed rect in event {event.get('event_id', '?')}")
        expected = int(rect["w_words"]) * int(rect["h"]) * 2
        if int(event["source_length"]) != expected:
            raise SystemExit(
                f"event {event.get('event_id', '?')}: source_length "
                f"{event['source_length']} != rectangle byte size {expected}"
            )
        by_archive[archive].append(event)

    archives: list[dict[str, Any]] = []
    for archive, archive_uploads in sorted(by_archive.items()):
        unique: dict[tuple[int, int, int, int, int, int], dict[str, Any]] = {}
        for event in archive_uploads:
            rect = event["rect"]
            key = (
                int(event["source_offset"]),
                int(event["source_length"]),
                int(rect["x"]),
                int(rect["y"]),
                int(rect["w_words"]),
                int(rect["h"]),
            )
            item = unique.setdefault(
                key,
                {
                    "source_offset": key[0],
                    "source_length": key[1],
                    "rect": {
                        "x": key[2],
                        "y": key[3],
                        "w_words": key[4],
                        "h": key[5],
                    },
                    "observations": 0,
                    "scenes": Counter(),
                },
            )
            item["observations"] += 1
            item["scenes"][str(event.get("scene", "boot"))] += 1
        normalized = []
        for item in unique.values():
            item["scenes"] = dict(sorted(item["scenes"].items()))
            normalized.append(item)
        normalized.sort(key=lambda item: (item["source_offset"], item["rect"]["y"], item["rect"]["x"]))
        archives.append(
            {
                "archive": archive,
                "upload_events": len(archive_uploads),
                "unique_uploads": normalized,
            }
        )

    return {
        "schema": "tokimemo.nbn.runtime-evidence/v1",
        "event_counts": {
            "total": len(events),
            "file_load": len(loads),
            "load_image": len(uploads),
            "move_image": len(moves),
        },
        "archive_count": len(archives),
        "archives": archives,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("trace", type=Path, help="NBN_TRACE JSONL file")
    parser.add_argument("--output", "-o", type=Path, help="write normalized evidence JSON")
    args = parser.parse_args()

    report = summarize(load_events(args.trace))
    rendered = json.dumps(report, indent=2, ensure_ascii=False) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(rendered, encoding="utf-8")
    else:
        print(rendered, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
