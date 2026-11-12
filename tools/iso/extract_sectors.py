#!/usr/bin/env python3
"""extract_sectors.py - copy a file's user data out of a MODE2/2352 track.

    python3 tools/iso/extract_sectors.py IMAGE START_SECTOR SIZE OUT

Reads SIZE bytes of Form-1 user data (bytes 24..2071 of each raw sector)
starting at START_SECTOR.  Used by `make extract-exe` / `make extract-obin`
(the former inline `python -c` one-liners were a syntax error: a `while`
cannot follow `;` on one line).
"""
import sys


def main():
    if len(sys.argv) != 5:
        raise SystemExit(__doc__)
    image, start, size, out = sys.argv[1], int(sys.argv[2], 0), \
        int(sys.argv[3], 0), sys.argv[4]
    data = bytearray()
    with open(image, "rb") as f:
        sector = start
        while len(data) < size:
            f.seek(sector * 2352)
            raw = f.read(2352)
            if len(raw) != 2352:
                raise SystemExit(f"{image}: short read at sector {sector}")
            data += raw[24:24 + 2048]
            sector += 1
    with open(out, "wb") as f:
        f.write(data[:size])
    print(f"Extracted {out} ({size} bytes)")


if __name__ == "__main__":
    main()
