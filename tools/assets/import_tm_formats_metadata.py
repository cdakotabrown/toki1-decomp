#!/usr/bin/env python3
"""Import graphics layouts from the 1996 TM Formats resource fork.

Accepts either unar's AppleDouble `.rsrc` output or a raw Macintosh resource
fork. Only declarative PSTM metadata is exported; third-party executable code
and resources are never copied into the repository.
"""
import argparse
import hashlib
import json
import struct
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
DEFAULT_OUTPUT = ROOT / "config/tm_formats_graphics_layouts.json"
APPLEDOUBLE_MAGIC = 0x00051607


def resource_fork(data):
    if len(data) >= 26 and struct.unpack_from(">I", data)[0] == APPLEDOUBLE_MAGIC:
        count = struct.unpack_from(">H", data, 24)[0]
        for index in range(count):
            entry_id, offset, length = struct.unpack_from(
                ">III", data, 26 + index * 12)
            if entry_id == 2:
                return data[offset:offset + length]
        raise ValueError("AppleDouble file has no resource-fork entry")
    return data


def pascal_name(data, offset):
    length = data[offset]
    return data[offset + 1:offset + 1 + length].decode("mac_roman")


def parse_pstm(fork):
    data_offset, map_offset, data_length, map_length = struct.unpack_from(
        ">IIII", fork)
    if data_offset + data_length > len(fork) or map_offset + map_length > len(fork):
        raise ValueError("invalid resource-fork bounds")
    type_rel, name_rel = struct.unpack_from(">HH", fork, map_offset + 24)
    type_base = map_offset + type_rel
    name_base = map_offset + name_rel
    type_count = struct.unpack_from(">H", fork, type_base)[0] + 1
    layouts = []
    for type_index in range(type_count):
        resource_type, count_minus_one, refs_rel = struct.unpack_from(
            ">4sHH", fork, type_base + 2 + type_index * 8)
        if resource_type != b"PSTM":
            continue
        for ref_index in range(count_minus_one + 1):
            ref = type_base + refs_rel + ref_index * 12
            resource_id, name_rel = struct.unpack_from(">hH", fork, ref)
            packed_offset = struct.unpack_from(">I", fork, ref + 4)[0]
            payload_offset = data_offset + (packed_offset & 0x00ffffff)
            payload_length = struct.unpack_from(">I", fork, payload_offset)[0]
            payload = fork[payload_offset + 4:payload_offset + 4 + payload_length]
            family = (pascal_name(fork, name_base + name_rel)
                      if name_rel != 0xffff else "")
            cursor = 0
            while cursor < len(payload):
                filename_length = payload[cursor]
                cursor += 1
                filename = payload[cursor:cursor + filename_length].decode(
                    "ascii").rstrip("\0")
                cursor += filename_length
                if cursor + 12 > len(payload):
                    raise ValueError(f"truncated PSTM record in {family}")
                width, height, pixels, palette = struct.unpack_from(
                    ">HHII", payload, cursor)
                cursor += 12
                layouts.append({
                    "family": family,
                    "resource_id": resource_id,
                    "filename": filename.removesuffix(";1"),
                    "width": width,
                    "height": height,
                    "pixel_offset": pixels,
                    "palette_offset": palette,
                })
    return layouts


def main():
    parser = argparse.ArgumentParser(
        description="export PSTM layout records from TM Formats")
    parser.add_argument("resource", type=Path,
                        help="raw resource fork or unar-produced .rsrc file")
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    args = parser.parse_args()
    source = args.resource.read_bytes()
    fork = resource_fork(source)
    records = parse_pstm(fork)
    result = {
        "schema": "tokimemo.tm-formats-layouts/v1",
        "provenance": {
            "title": "TM Formats",
            "author": "Tadanori Tezuka",
            "date": "1996-01-01",
            "source_url": "https://ftp.vector.co.jp/03/90/735/TM_Formats.sit.bin",
            "input_sha256": hashlib.sha256(source).hexdigest(),
            "resource_fork_sha256": hashlib.sha256(fork).hexdigest(),
        },
        "record_count": len(records),
        "records": records,
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=1) + "\n")
    print(f"imported {len(records)} layout records -> {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
