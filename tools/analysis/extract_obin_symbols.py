#!/usr/bin/env python3
"""
Extract ALL symbols from o.bin (MIPSEL ECOFF debug symbol file).

o.bin is from a development build of Tokimeki Memorial (SLPS-00065).
It contains 2,146 external symbols including:
  - 1,958 function names (st=1 Global, sc=5 Undefined)
  - 188 global variable names NOT in the existing symbol_map.json
  - 9 data symbols (menu0-menu7, olh_menu)
  - 5 procedure entries (olh_main, olh_init, olh, olh_exit, move_menu_title)

Usage:
    python3 tools/analysis/extract_obin_symbols.py [--all] [--new-only] [--variables] [--json output.json]

Options:
    --all        Print all 2,146 symbols
    --new-only   Print only symbols not in symbol_map.json (default)
    --variables  Print only global variable symbols (not function names)
    --json FILE  Write output as JSON to FILE
    --csv FILE   Write output as CSV to FILE
"""

import struct
import json
import sys
import argparse
from pathlib import Path

BASE_DIR = Path(__file__).parent.parent.parent
OBIN_PATH = BASE_DIR / "disks" / "o.bin"
SYMBOL_MAP_PATH = BASE_DIR / "build" / "slps00065" / "symbols" / "symbol_map.json"

# ECOFF symbol type names
ST_NAMES = {
    0: "Nil", 1: "Global", 2: "Static", 3: "Param", 4: "Local",
    5: "Typedef", 6: "Proc", 7: "Block", 8: "End", 9: "Member",
    10: "IsParam", 11: "RegReloc", 12: "Forward", 13: "StaticProc",
    14: "Constant"
}

# ECOFF storage class names
SC_NAMES = {
    0: "Nil", 1: "Text", 2: "Data", 3: "Bss", 4: "Register",
    5: "Undefined", 6: "CdbLocal", 7: "CdbSystem", 8: "Bits",
    9: "CdbSReg", 10: "RegImage", 11: "Info", 12: "UserStruct",
    13: "SData", 14: "SBss", 15: "RData", 16: "Var", 17: "Common",
    18: "SCommon", 19: "VarRegister", 20: "Variant", 21: "SUndefined",
    22: "Init", 23: "BasedVar", 24: "Xdata", 25: "Pdata",
    26: "Fini", 27: "NonGP"
}


def parse_ecoff_header(data):
    """Parse ECOFF file header and symbolic header."""
    # File header (20 bytes for MIPS ECOFF)
    magic = struct.unpack_from('<H', data, 0)[0]
    assert magic == 0x0162, f"Not a MIPS ECOFF file (magic=0x{magic:04x})"

    symptr = struct.unpack_from('<I', data, 8)[0]

    # Symbolic header (HDRR) at symptr
    hdr = {}
    hdr['magic'] = struct.unpack_from('<H', data, symptr)[0]
    hdr['vstamp'] = struct.unpack_from('<H', data, symptr + 2)[0]
    hdr['ilineMax'] = struct.unpack_from('<I', data, symptr + 4)[0]
    hdr['cbLine'] = struct.unpack_from('<I', data, symptr + 8)[0]
    hdr['cbLineOffset'] = struct.unpack_from('<I', data, symptr + 12)[0]
    hdr['idnMax'] = struct.unpack_from('<I', data, symptr + 16)[0]
    hdr['cbDnOffset'] = struct.unpack_from('<I', data, symptr + 20)[0]
    hdr['ipdMax'] = struct.unpack_from('<I', data, symptr + 24)[0]
    hdr['cbPdOffset'] = struct.unpack_from('<I', data, symptr + 28)[0]
    hdr['isymMax'] = struct.unpack_from('<I', data, symptr + 32)[0]
    hdr['cbSymOffset'] = struct.unpack_from('<I', data, symptr + 36)[0]
    hdr['ioptMax'] = struct.unpack_from('<I', data, symptr + 40)[0]
    hdr['cbOptOffset'] = struct.unpack_from('<I', data, symptr + 44)[0]
    hdr['iauxMax'] = struct.unpack_from('<I', data, symptr + 48)[0]
    hdr['cbAuxOffset'] = struct.unpack_from('<I', data, symptr + 52)[0]
    hdr['issMax'] = struct.unpack_from('<I', data, symptr + 56)[0]
    hdr['cbSsOffset'] = struct.unpack_from('<I', data, symptr + 60)[0]
    hdr['issExtMax'] = struct.unpack_from('<I', data, symptr + 64)[0]
    hdr['cbSsExtOffset'] = struct.unpack_from('<I', data, symptr + 68)[0]
    hdr['ifdMax'] = struct.unpack_from('<I', data, symptr + 72)[0]
    hdr['cbFdOffset'] = struct.unpack_from('<I', data, symptr + 76)[0]
    hdr['crfd'] = struct.unpack_from('<I', data, symptr + 80)[0]
    hdr['cbRfdOffset'] = struct.unpack_from('<I', data, symptr + 84)[0]
    hdr['iextMax'] = struct.unpack_from('<I', data, symptr + 88)[0]
    hdr['cbExtOffset'] = struct.unpack_from('<I', data, symptr + 92)[0]

    return hdr


def extract_all_symbols(data, hdr):
    """Extract all external symbols from ECOFF."""
    ss_off = hdr['cbSsExtOffset']
    ss_size = hdr['issExtMax']
    ext_off = hdr['cbExtOffset']
    ext_count = hdr['iextMax']

    strings_raw = data[ss_off:ss_off + ss_size]
    symbols = []

    for i in range(ext_count):
        off = ext_off + i * 16
        iss = struct.unpack_from('<I', data, off + 4)[0]
        value = struct.unpack_from('<I', data, off + 8)[0]
        st_sc = struct.unpack_from('<H', data, off + 12)[0]
        st = st_sc & 0x3F
        sc = (st_sc >> 6) & 0x1F

        end = strings_raw.index(0, iss) if iss < len(strings_raw) and 0 in strings_raw[iss:] else len(strings_raw)
        name = strings_raw[iss:end].decode('ascii', errors='replace')

        symbols.append({
            'name': name,
            'address': f'0x{value:08x}',
            'addr_int': value,
            'st': st,
            'sc': sc,
            'st_name': ST_NAMES.get(st, f'Unknown({st})'),
            'sc_name': SC_NAMES.get(sc, f'Unknown({sc})'),
            'is_function': (st == 1 and sc == 5) or (st == 6 and sc == 1),
            'is_variable': st == 1 and sc != 5,
            'is_procedure': st == 6 and sc == 1,
        })

    return symbols


def load_known_symbols():
    """Load existing symbol map."""
    if not SYMBOL_MAP_PATH.exists():
        return set()
    with open(SYMBOL_MAP_PATH) as f:
        data = json.load(f)
    return set(s['name'] for s in data['symbols'])


def main():
    parser = argparse.ArgumentParser(description='Extract symbols from o.bin')
    parser.add_argument('--all', action='store_true', help='Print all symbols')
    parser.add_argument('--new-only', action='store_true', help='Print only new symbols (default)')
    parser.add_argument('--variables', action='store_true', help='Print only variable symbols')
    parser.add_argument('--functions', action='store_true', help='Print only function symbols')
    parser.add_argument('--json', type=str, help='Write JSON output to file')
    parser.add_argument('--csv', type=str, help='Write CSV output to file')
    parser.add_argument('--header', action='store_true', help='Print ECOFF header info')
    args = parser.parse_args()

    if not OBIN_PATH.exists():
        print(f"Error: {OBIN_PATH} not found", file=sys.stderr)
        sys.exit(1)

    with open(OBIN_PATH, 'rb') as f:
        data = f.read()

    hdr = parse_ecoff_header(data)

    if args.header:
        print("=== ECOFF Symbolic Header ===")
        for k, v in hdr.items():
            print(f"  {k}: {v}")
        print()

    symbols = extract_all_symbols(data, hdr)
    known = load_known_symbols()

    # Filter
    if args.variables:
        symbols = [s for s in symbols if s['is_variable']]
    elif args.functions:
        symbols = [s for s in symbols if s['is_function']]
    elif not args.all:
        # Default: new only
        symbols = [s for s in symbols if s['name'] not in known and len(s['name']) > 2]

    symbols.sort(key=lambda s: s['addr_int'])

    # Output
    if args.json:
        out = [{'name': s['name'], 'address': s['address'],
                'type': s['st_name'], 'storage': s['sc_name']} for s in symbols]
        with open(args.json, 'w') as f:
            json.dump({'symbols': out, 'count': len(out)}, f, indent=2)
        print(f"Wrote {len(out)} symbols to {args.json}")
    elif args.csv:
        with open(args.csv, 'w') as f:
            f.write("address,name,type,storage\n")
            for s in symbols:
                f.write(f"{s['address']},{s['name']},{s['st_name']},{s['sc_name']}\n")
        print(f"Wrote {len(symbols)} symbols to {args.csv}")
    else:
        print(f"{'Address':<12} {'Type':<10} {'Storage':<10} Name")
        print("-" * 60)
        for s in symbols:
            print(f"{s['address']}  {s['st_name']:<10} {s['sc_name']:<10} {s['name']}")
        print(f"\nTotal: {len(symbols)} symbols")


if __name__ == '__main__':
    main()
