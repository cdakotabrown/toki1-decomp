#!/usr/bin/env python3
"""Overlay symbol-name table: the single place that turns an overlay symbol NAME
into a RAM address (and back) once names stop carrying their address.

Until 2026-10 every overlay function was `func_XXXXXXXX`, so the address was
read off the name.  `tools/overlay/apply_overlay_symbols.py` lands real names;
this module is what every tool that used to decode `func_<ADDR>` consults
instead.  It is deliberately light (stdlib only) so that anything, including
resolve_overlay_relocs, overlay_postproc and build_clean_exn, can import it
without a cycle.

Records live in config/symbol_names.json (docs/SYMBOL-NAMING-PIPELINE.md §3 v2):

    "0x800429B8"          main record     (scope "main", the historic form)
    "RENSYU:0x80132040"   overlay record  ("scope": "RENSYU")

An overlay record's address is the one its `func_<ADDR>` name carried, i.e.
the PROLOGUE address.  The `.sym` window may start earlier when a hoisted
cross-prologue head was absorbed (`0x801320E0:688:func_801320E8`).  Callers
enter at the head, so a REFERENCE by a landed function name resolves to the
window start (`_Tables.entry`, 2026-11-10; retail's RENSYU step table holds
0x801320E0); overlay_table()/func_addr() keep the record (prologue) address
for placement.  `func_<HEADADDR>` spellings are retired to the landed name by
tools/naming/retire_head_refs.py.

Resolution order for a symbol seen in overlay OV (name_to_addr):
  1. OV's landed overlay names             (scoped: the same name may exist in
                                            several overlays at different addresses)
  2. the landed MAIN names
  3. the `<prefix>_XXXXXXXX` suffix rule   (address names, unchanged)

Refusals are HARD (NameTableError), like the main table.

    python3 tools/overlay/overlay_names.py --check          # validate the table
    python3 tools/overlay/overlay_names.py OV NAME|0xADDR   # resolve one
"""
import json
import os
import re
import sys
from pathlib import Path
from typing import Dict, List, Optional, Tuple

ROOT = Path(__file__).resolve().parent.parent.parent
NAMES_FILE = ROOT / "config" / "symbol_names.json"
SYM_DIR = ROOT / "config" / "overlay"

NAME_ADDR_RE = re.compile(r'^[A-Za-z_][A-Za-z0-9_]*?_([0-9A-Fa-f]{8})(?:_[a-z]{1,3})?$')
FUNC_NAME_RE = re.compile(r'^func_([0-9A-Fa-f]{8})$')
RAM_LO = 0x80000000
RAM_HI = 0x801FFFFF
OVERLAY_BASE = 0x80132000


class NameTableError(RuntimeError):
    """config/symbol_names.json cannot be used as a name -> address table."""


def split_key(key: str) -> Tuple[str, int]:
    """'0x8004ABCD' -> ('main', addr);  'RENSYU:0x80132040' -> ('RENSYU', addr)."""
    if ':' in key:
        scope, _, a = key.partition(':')
        return scope, int(a, 16)
    return 'main', int(key, 16)


def make_key(scope: str, addr: int) -> str:
    return f'0x{addr:08X}' if scope == 'main' else f'{scope}:0x{addr:08X}'


class _Tables:
    def __init__(self):
        self.main: Dict[str, int] = {}
        self.ov: Dict[str, Dict[str, int]] = {}
        self.rev: Dict[str, Dict[int, str]] = {}     # ov -> addr -> landed name
        self.funcs: Dict[str, set] = {}              # ov -> landed FUNCTION names
        self.shadows: set = set()                    # (ov, name) with a reviewed shadows_main
        self.problems: List[str] = []
        self.entry: Dict[str, Dict[str, int]] = {}  # ov -> landed func -> head entry
        self.main_entry: Dict[str, int] = {}  # main landed func -> registry (head) start


_CACHE: Optional[_Tables] = None


def load_tables(force: bool = False) -> _Tables:
    """Parse config/symbol_names.json once.  TOKI_NO_NAME_TABLE=1 disables it
    (negative-control runs only, as in resolve_overlay_relocs)."""
    global _CACHE
    if _CACHE is not None and not force:
        return _CACHE
    t = _Tables()
    if os.environ.get("TOKI_NO_NAME_TABLE") == "1":
        _CACHE = t
        return t
    try:
        doc = json.loads(NAMES_FILE.read_text())
        entries = doc["symbols"].items()
    except (OSError, ValueError, KeyError, AttributeError) as exc:
        raise NameTableError(f"{NAMES_FILE} unreadable: {exc}") from exc
    for key, e in entries:
        if not e.get("landed") or e.get("kind") not in ("func", "data"):
            continue
        name = e.get("name")
        try:
            scope, addr = split_key(key)
        except ValueError:
            t.problems.append(f"bad address key {key!r}")
            continue
        if not name:
            t.problems.append(f"{key}: landed entry without a name")
            continue
        if scope != e.get("scope", "main"):
            t.problems.append(f"{key}: record scope {e.get('scope')!r} disagrees with its key")
            continue
        if not (RAM_LO <= addr <= RAM_HI):
            t.problems.append(f"{name}: landed at {key}, outside the RAM window")
            continue
        if scope != 'main' and addr < OVERLAY_BASE:
            t.problems.append(f"{scope}:{name}: overlay record at {key} is below the overlay base")
            continue
        m = NAME_ADDR_RE.match(name)
        if m:
            if int(m.group(1), 16) != addr:
                t.problems.append(f"{name}: landed at {key} but its suffix names "
                                  f"0x{int(m.group(1), 16):08X}")
            continue            # agreeing address-shaped name: suffix rule owns it
        tab = t.main if scope == 'main' else t.ov.setdefault(scope, {})
        if name in tab and tab[name] != addr:
            t.problems.append(f"{scope}:{name}: landed at both 0x{tab[name]:08X} and {key}")
            continue
        tab[name] = addr
        # views (2026-11-10): the same address declared again under another
        # type or `volatile` -- `views: [{"name": .., "offset": n}]` on the
        # host record; a view name stands for host + offset
        for v in e.get("views") or ():
            vn, va = v.get("name"), addr + int(v.get("offset", 0))
            if not vn:
                continue
            if vn in tab and tab[vn] != va:
                t.problems.append(f"{scope}:{vn}: view of {name} also landed at 0x{tab[vn]:08X}")
                continue
            tab[vn] = va
        if scope != 'main' and e.get("shadows_main"):
            t.shadows.add((scope, name))
        if scope != 'main' and e.get("kind") == "func":
            t.funcs.setdefault(scope, set()).add(name)
        if scope != 'main':
            r = t.rev.setdefault(scope, {})
            if addr in r and r[addr] != name:
                t.problems.append(f"{scope}: 0x{addr:08X} landed as both {r[addr]} and {name}")
            r[addr] = name
    for scope, tab in t.ov.items():
        for name in tab:
            if name in t.main and t.main[name] != tab[name] and (scope, name) not in t.shadows:
                t.problems.append(f"{scope}:{name}: also a landed MAIN name (0x{t.main[name]:08X}); "
                                  "a reference to it would be ambiguous")
    # Absorbed heads: a landed function whose .sym window starts BEFORE its
    # record address (the prologue) is entered at the window start, the
    # hoisted head.  A reference by name stands for that entry -- the
    # address callers jal -- not for the prologue (2026-11-09).
    for scope, fnames in t.funcs.items():
        for start, size, name in parse_sym(scope):
            a = t.ov[scope].get(name)
            if name in fnames and a is not None and start < a < start + size:
                t.entry.setdefault(scope, {})[name] = start
    # The same for MAIN functions: a record keyed at the prologue whose
    # config/level3_functions.json entry (same name) starts up to 184 bytes
    # earlier, at the absorbed head.  The registry address is the entry the
    # main build places the name at and the address callers jal, so an
    # overlay's reference by that name stands for it (2026-11-10).
    try:
        reg = json.loads((NAMES_FILE.parent / "level3_functions.json").read_text())["functions"]
    except (OSError, ValueError, KeyError):
        reg = []
    for f in reg:
        name = f.get("name")
        a = t.main.get(name)
        try:
            r = int(f["address"], 16)
        except (KeyError, TypeError, ValueError):
            continue
        if a is not None and r < a <= r + 184:
            t.main_entry[name] = r
    _CACHE = t
    return t


def check_tables(force: bool = True) -> List[str]:
    """Problems with the landed names (empty list = consistent)."""
    return list(load_tables(force=force).problems)


def main_table() -> Dict[str, int]:
    t = load_tables()
    if t.problems:
        raise NameTableError(f"{NAMES_FILE}: {len(t.problems)} inconsistent landed name(s): "
                             + "; ".join(t.problems[:10]))
    return t.main


def overlay_table(ov: str) -> Dict[str, int]:
    """OV's landed names -> address (overlay scope only; main names not merged)."""
    t = load_tables()
    if t.problems:
        raise NameTableError(f"{NAMES_FILE}: {len(t.problems)} inconsistent landed name(s): "
                             + "; ".join(t.problems[:10]))
    return t.ov.get(ov.upper(), {})


def scoped_table(ov: Optional[str]) -> Dict[str, int]:
    """Main names overlaid with OV's own (overlay wins; the two may only differ
    when check_tables found no ambiguity, i.e. never in a consistent tree)."""
    tab = dict(main_table())
    tab.update(load_tables().main_entry)
    if ov:
        tab.update(overlay_table(ov))
        tab.update(load_tables().entry.get(ov.upper(), {}))
    return tab


_AMBIENT: List[Optional[str]] = [None]


def set_ambient(ov: Optional[str]) -> None:
    """Default overlay for name_to_addr callers that do not pass one (tools that
    process one overlay's function at a time, e.g. screen_semis workers)."""
    _AMBIENT[0] = ov.upper() if ov else None


def name_to_addr(sym: str, ov: Optional[str] = None) -> Optional[int]:
    """RAM address a relocation symbol stands for, else None.

    With `ov`, that overlay's landed names are consulted first.  Without it, a
    name landed in exactly one overlay (or at one address across overlays)
    still resolves, but a name landed at DIFFERENT addresses in different
    overlays is ambiguous and raises NameTableError: the caller must say which
    overlay it is looking at (or set_ambient() once for a one-overlay worker).
    """
    ov = ov or _AMBIENT[0]
    if ov:
        a = overlay_table(ov).get(sym)
        if a is not None:
            return load_tables().entry.get(ov.upper(), {}).get(sym, a)
    a = main_table().get(sym)
    if a is not None:
        return load_tables().main_entry.get(sym, a)
    if not ov:
        t = load_tables()
        hits = {t.entry.get(o, {}).get(sym, tab[sym]) for o, tab in t.ov.items() if sym in tab}
        if len(hits) > 1:
            raise NameTableError(f"{sym}: landed at different addresses in different overlays "
                                 f"({', '.join(f'0x{x:08X}' for x in sorted(hits))}); "
                                 "the caller must pass the overlay")
        if hits:
            return hits.pop()
    m = NAME_ADDR_RE.match(sym)
    if m:
        a = int(m.group(1), 16)
        if RAM_LO <= a <= RAM_HI:
            return a
    return None


# ------------------------------------------------------------------ .sym files
def parse_sym(ov: str) -> List[Tuple[int, int, str]]:
    """[(window_addr, size, name)] in file order (duplicates kept)."""
    p = SYM_DIR / f"{ov.upper()}.sym"
    out: List[Tuple[int, int, str]] = []
    if not p.exists():
        return out
    for line in p.read_text().splitlines():
        line = line.strip()
        if not line or line.startswith('#'):
            continue
        parts = line.split(':')
        if len(parts) >= 3:
            try:
                out.append((int(parts[0], 16), int(parts[1]), parts[2].strip()))
            except ValueError:
                continue
    return out


def func_addr(ov: str, name: str) -> Optional[int]:
    """The address a FUNCTION name stands for in overlay OV: landed name first,
    then the func_<ADDR> suffix.  None when neither applies."""
    a = overlay_table(ov).get(name)
    if a is not None:
        return a
    m = FUNC_NAME_RE.match(name)
    if m:
        return int(m.group(1), 16)
    return None


def addr_name(ov: str, addr: int) -> str:
    """Name of the overlay function whose entry address is ADDR: the landed
    name, else func_<ADDR>."""
    n = load_tables().rev.get(ov.upper(), {}).get(addr)
    return n if n else f"func_{addr:08X}"


def sym_name_for(ov: str, addr: int) -> Optional[str]:
    """The .sym symbol for the function at name-address ADDR (covers an
    absorbed head: the window may start before ADDR)."""
    want = addr_name(ov, addr)
    for _, _, n in parse_sym(ov):
        if n == want:
            return n
    return None


def is_func_symbol(ov: str, name: str) -> bool:
    """Is NAME a function symbol of overlay OV in its .sym?  `func_<ADDR>` (any
    prefix-and-address spelling the .sym uses) or a landed function name."""
    return name.startswith('func_') or name in load_tables().funcs.get(ov.upper(), ())


def current_name(ov: str, fn: str) -> str:
    """Follow a landed rename: `func_<ADDR>` whose function has since been given
    a real name returns that name; any other name (including a landed one) is
    returned unchanged.  For fixtures and CLIs that spell functions by the old
    address name (`--only RENSYU/func_80132040`, negative-control lists)."""
    m = FUNC_NAME_RE.match(fn)
    if m:
        n = load_tables().rev.get(ov.upper(), {}).get(int(m.group(1), 16))
        if n:
            return n
    return fn


def src_stems(ov: str) -> List[str]:
    """Function-C stems of src/overlay/<OV> (every *.c is a function file)."""
    d = ROOT / "src" / "overlay" / ov.upper()
    return sorted(p.stem for p in d.glob("*.c")) if d.exists() else []


def main(argv=None) -> int:
    argv = list(sys.argv[1:] if argv is None else argv)
    if argv == ['--check']:
        probs = check_tables(force=True)
        t = load_tables()
        print(f"overlay name table: main={len(t.main)} "
              + ' '.join(f"{o}={len(v)}" for o, v in sorted(t.ov.items())))
        for p in probs:
            print("PROBLEM", p)
        return 1 if probs else 0
    if len(argv) == 2:
        ov, what = argv
        if what.lower().startswith('0x'):
            print(addr_name(ov, int(what, 16)))
        else:
            a = name_to_addr(what, ov)
            print('unresolved' if a is None else f"0x{a:08X}")
        return 0
    print(__doc__)
    return 2


if __name__ == '__main__':
    sys.exit(main())
