#!/usr/bin/env python3
"""save_builder.py — build / inspect / modify Tokimeki Memorial save files.

Works with:
  - Raw 8 KB save blocks (binary game data)
  - .mcs exports (128 B dir frame + 8 KB data)
  - reference-emulator harness RAM dumps for extracting live game state

Save Structure (based on decompilation analysis):
  D_800EE760 is the base of the save work area in RAM (256+ bytes).
  When written to memcard, the game copies ~8 KB starting at D_800EE760.

Key fields observed in file_select_main.c:
  - 11 character slots, ~56 bytes each starting at offset 0
  - Slot N: base + N*56
    - offset +454 (s16): mood/affection value
    - offset +456 (u32): bitfield flags
  - D_800F01F0 % 11: current save slot cursor
  - D_800BCC40: last-picked heroine for attract mode

Commands:
  dump    SAVE_FILE           — hex dump with field annotations
  info    SAVE_FILE           — summary of save state
  modify  SAVE_FILE -o OUT    — interactive edit (or --set flags)
  from-ram RAM.bin ADDR       — extract save from harness RAM dump
  to-mcs  SAVE_FILE OUT.mcs   — wrap 8KB save block into .mcs container
"""

import argparse
import struct
import sys
from pathlib import Path

# Constants
SAVE_BLOCK_SIZE = 8192
MCS_HEADER_SIZE = 128
MCS_TOTAL_SIZE = MCS_HEADER_SIZE + SAVE_BLOCK_SIZE

# Game constants (from decompilation)
NUM_HEROINES = 11
HEROINE_SLOT_SIZE = 56

# RAM addresses (relative to D_800EE760 = 0x800EE760)
SAVE_RAM_BASE = 0x800EE760

# Character names (from game data)
HEROINE_NAMES = [
    "Fujisaki Shiori",      # 0
    "Saotome Yuko",         # 1
    "Ayasaki Mei",          # 2
    "Ijuin Rei",            # 3
    "Kagami Mira",          # 4
    "Himoo Yuina",          # 5
    "Asahina Yukari",       # 6
    "Kisaragi Mio",         # 7
    "Mikihara Megumi",      # 8
    "Katagiri Ayako",       # 9
    "Yoshio (player)",      # 10
]


def load_save(path: str) -> bytes:
    """Load save data from file (.mcs or raw 8KB)."""
    with open(path, "rb") as f:
        data = f.read()

    if len(data) == MCS_TOTAL_SIZE:
        # MCS format: skip 128-byte header
        return data[MCS_HEADER_SIZE:]
    elif len(data) == SAVE_BLOCK_SIZE:
        return data
    else:
        sys.exit(f"Unknown save format: {len(data)} bytes (expected {SAVE_BLOCK_SIZE} or {MCS_TOTAL_SIZE})")


def save_to_mcs(save_data: bytes, filename: str, out_path: str):
    """Wrap raw save block into .mcs container."""
    if len(save_data) != SAVE_BLOCK_SIZE:
        sys.exit(f"Save data must be {SAVE_BLOCK_SIZE} bytes")

    header = bytearray(MCS_HEADER_SIZE)
    # State: 0x51 = used/first block
    struct.pack_into("<I", header, 0x00, 0x51)
    # Size: 8192
    struct.pack_into("<I", header, 0x04, SAVE_BLOCK_SIZE)
    # Link: 0xFFFF (no chain)
    struct.pack_into("<H", header, 0x08, 0xFFFF)
    # Filename (20 bytes max)
    fn_bytes = filename.encode("ascii")[:20]
    header[0x0A:0x0A + len(fn_bytes)] = fn_bytes
    # XOR checksum
    xor = 0
    for b in header[:MCS_HEADER_SIZE - 1]:
        xor ^= b
    header[MCS_HEADER_SIZE - 1] = xor

    with open(out_path, "wb") as f:
        f.write(header)
        f.write(save_data)
    print(f"Wrote {out_path} ({MCS_TOTAL_SIZE} bytes)")


def get_heroine_data(save: bytes, idx: int) -> dict:
    """Extract data for heroine slot idx (0-10)."""
    base = idx * HEROINE_SLOT_SIZE

    # Basic validation
    if base + HEROINE_SLOT_SIZE > len(save):
        return {"error": "out of bounds"}

    slot = save[base:base + HEROINE_SLOT_SIZE]

    # Extract known fields based on file_select_main.c analysis
    # charData[114] at byte offset 456 (114 * 4)
    # charData + 454 for s16 affection
    # These offsets are from D_800EE760 + slot*56 + offset
    # Actually the code accesses beyond 56 bytes so the struct is larger

    # Let's just dump the raw 56 bytes for now
    return {
        "name": HEROINE_NAMES[idx] if idx < len(HEROINE_NAMES) else f"Unknown {idx}",
        "raw_hex": slot.hex(),
        "words": [struct.unpack_from("<I", slot, i)[0] for i in range(0, min(56, len(slot)), 4)]
    }


def cmd_dump(args):
    """Hex dump with annotations."""
    save = load_save(args.save)

    print(f"Save size: {len(save)} bytes")
    print()

    # Dump heroine slots
    for i in range(NUM_HEROINES):
        data = get_heroine_data(save, i)
        print(f"=== Slot {i}: {data.get('name', '?')} ===")
        if "error" in data:
            print(f"  {data['error']}")
            continue
        print(f"  Raw: {data['raw_hex'][:64]}...")
        print(f"  Words: {data['words'][:4]}...")
        print()

    # Dump first 256 bytes hex
    print("=== First 256 bytes (hex) ===")
    for i in range(0, min(256, len(save)), 16):
        line = save[i:i+16]
        hex_str = " ".join(f"{b:02x}" for b in line)
        ascii_str = "".join(chr(b) if 32 <= b < 127 else "." for b in line)
        print(f"{i:04x}: {hex_str:<48} {ascii_str}")


def cmd_info(args):
    """Summary of save state."""
    save = load_save(args.save)

    print(f"Save file: {args.save}")
    print(f"Size: {len(save)} bytes")
    print()

    # Look for non-zero data patterns
    nonzero_ranges = []
    start = None
    for i, b in enumerate(save):
        if b != 0:
            if start is None:
                start = i
        else:
            if start is not None:
                nonzero_ranges.append((start, i))
                start = None
    if start is not None:
        nonzero_ranges.append((start, len(save)))

    print(f"Non-zero ranges: {len(nonzero_ranges)}")
    for s, e in nonzero_ranges[:10]:
        print(f"  {s:04x}-{e:04x} ({e-s} bytes)")
    if len(nonzero_ranges) > 10:
        print(f"  ... and {len(nonzero_ranges) - 10} more")


def cmd_from_ram(args):
    """Extract save from RAM dump."""
    with open(args.ram, "rb") as f:
        ram = f.read()

    addr = int(args.addr, 16) if args.addr.startswith("0x") else int(args.addr)

    # RAM dump from harness starts at 0x80000000 (or specified base)
    ram_base = int(args.base, 16) if args.base else 0x80000000
    offset = addr - ram_base

    if offset < 0 or offset + SAVE_BLOCK_SIZE > len(ram):
        sys.exit(f"Address {addr:#x} out of range for RAM dump (base {ram_base:#x}, size {len(ram)})")

    save = ram[offset:offset + SAVE_BLOCK_SIZE]

    out_path = args.out or "extracted_save.bin"
    with open(out_path, "wb") as f:
        f.write(save)
    print(f"Extracted {SAVE_BLOCK_SIZE} bytes from {addr:#x} -> {out_path}")


def cmd_to_mcs(args):
    """Wrap save block into .mcs container."""
    save = load_save(args.save)
    filename = args.name or "BISLPS-00065TOKIMEKI01"
    save_to_mcs(save, filename, args.out)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)

    s = sub.add_parser("dump", help="Hex dump with annotations")
    s.add_argument("save", help="Save file (.mcs or raw 8KB)")
    s.set_defaults(fn=cmd_dump)

    s = sub.add_parser("info", help="Summary of save state")
    s.add_argument("save", help="Save file")
    s.set_defaults(fn=cmd_info)

    s = sub.add_parser("from-ram", help="Extract save from RAM dump")
    s.add_argument("ram", help="RAM dump file from harness")
    s.add_argument("addr", help="Start address (e.g., 0x800EE760)")
    s.add_argument("--base", help="RAM dump base address (default 0x80000000)")
    s.add_argument("-o", "--out", help="Output file")
    s.set_defaults(fn=cmd_from_ram)

    s = sub.add_parser("to-mcs", help="Wrap save into .mcs container")
    s.add_argument("save", help="Raw 8KB save file")
    s.add_argument("out", help="Output .mcs file")
    s.add_argument("--name", help="Save filename (default: BISLPS-00065TOKIMEKI01)")
    s.set_defaults(fn=cmd_to_mcs)

    args = ap.parse_args()
    args.fn(args)


if __name__ == "__main__":
    main()
