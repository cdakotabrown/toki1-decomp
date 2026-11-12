#!/usr/bin/env python3
"""overlay_data_from_source.py - build an overlay's .rodata/.data/.bss from SOURCE.

Design and findings: docs/reference/OVERLAY-DATA-FROM-SOURCE.md.

build_clean_exn.py copies everything after the last code byte (the "data
splice") from the retail EXN.  This module replaces that splice, behind
`build_clean_exn.py --data-from-source`, with sections taken from compiled
objects and placed the way the retail linker placed them.

The model (measured on the IDO 5.2 recomp and on the retail overlays):

  * IDO 5.2 emits, per translation unit (TU),
      .rodata = [string literals and `const` file-scope data, in SOURCE order]
                [switch jump tables, in FUNCTION order]
      .data   = [initialised file-scope data and per-function initialiser
                 templates (auto aggregates, initialised statics), SOURCE
                 order]
      .bss    = [uninitialised file-scope data, in definition order]
    every datum 4-aligned, every section padded to 16.  IDO never pools two
    identical literals.
  * An overlay is SEVERAL TUs, contiguous in function address order.  The
    linker lays out every TU's .text, then every .rodata, every .data, every
    .bss, each input section 16-aligned; the bss is zero in the EXN.  A TU
    is named by its first function (`@TU:` in a data file, or
    data/tu_starts.txt).

The decomp splits each TU into one file per function, so the builder
re-concatenates it:

  * a DATA FILE per TU (src/overlay/<OV>/data/*.c, `/* @TU: <first fn> */`)
    holds the TU's file-scope definitions; it is the head of its TU, so its
    pieces come first in each of the TU's sections;
  * each function object contributes, in address order, its literal pool
    (the head of its .rodata), its jump tables (the tail of its .rodata),
    its anonymous .data (local-initialiser templates) and its .bss;
  * every function object is RE-LINKED with its sections at the computed
    addresses (no @RodataBase / @LiteralBase / @DataBase is read), and its
    .text must come out equal to the code the text phase produced;
  * every named symbol of a data file must land on its name address
    (D_<addr>, or a landed overlay name) -- the layout is computed, then
    checked against the names, never read from them;
  * a function pointer stored in data is the function's ENTRY (its
    @SymbolStart window start), as a linker defines it.

True (unpadded) section sizes, which the 16-byte object padding hides:
  * literal pool = offset of the first jump-table word (the first R_MIPS_32
    in .rel.rodata); with no jump table, the end of the literal with the
    highest referenced start, 4-aligned;
  * jump tables = last R_MIPS_32 + 4;
  * .data / .bss = a MEASURE compile of a copy of the source with a 1-byte
    marker appended to the section (IDO places it 4-aligned after the last
    datum, so the marker offset is the 4-aligned true size).

A function object whose .rodata holds BOTH literals and jump tables must
place the two at different addresses (all literals of the TU precede all
jump tables).  ld cannot split an input section, so the object is given a
second section base: every .text HI16/LO16 reference into the jump-table
part is re-pointed at a new symbol `__odfs_jt` (addend minus the split
offset), and the linker script defines `__odfs_jt` at the jump-table
address.  This changes relocation records and the addend fields the linker
overwrites anyway, never an opcode or a register; a HI16 shared between a
literal and a jump-table access is refused.
"""
import bisect
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Dict, List, Optional, Tuple

ROOT = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "overlay"))
sys.path.insert(0, str(ROOT / "tools" / "build"))

from resolve_overlay_relocs import (   # noqa: E402
    compile_object, name_to_addr, get_func_text_offset,
)
import overlay_names as ON  # noqa: E402

LD = "mipsel-linux-gnu-ld"
OBJCOPY = "mipsel-linux-gnu-objcopy"
SRC_OVERLAY_DIR = ROOT / "src" / "overlay"

R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16 = 2, 4, 5, 6
MARK_DATA = "__odfs_data_end"
MARK_BSS = "__odfs_bss_end"
JT_SYM = "__odfs_jt"
DATA_SECTIONS = (".rodata", ".data", ".bss")


class DataLayoutError(RuntimeError):
    pass


def align(x: int, a: int) -> int:
    return (x + a - 1) & ~(a - 1)


# ---------------------------------------------------------------------------
# ELF reading (pyelftools) -- only what the layout needs
# ---------------------------------------------------------------------------

class Obj:
    """Sections, symbols and REL records of one IDO object."""

    def __init__(self, path: Path):
        from elftools.elf.elffile import ELFFile
        from elftools.elf.relocation import RelocationSection
        self.path = path
        self.raw = bytearray(path.read_bytes())
        with open(path, "rb") as fh:
            elf = ELFFile(fh)
            self.sec: Dict[str, Dict] = {}
            self.sec_index: Dict[str, int] = {}
            for i, s in enumerate(elf.iter_sections()):
                self.sec_index[s.name] = i
                if s.name in (".text",) + DATA_SECTIONS:
                    self.sec[s.name] = {
                        "size": s["sh_size"], "offset": s["sh_offset"],
                        "data": (b"" if s["sh_type"] == "SHT_NOBITS"
                                 else s.data()),
                    }
            self.syms: List[Dict] = []
            symtab = elf.get_section_by_name(".symtab")
            self.symtab_index = self.sec_index.get(".symtab")
            for i, sym in enumerate(symtab.iter_symbols()):
                shndx = sym["st_shndx"]
                secname = (elf.get_section(shndx).name
                           if isinstance(shndx, int) and shndx > 0 else None)
                self.syms.append({
                    "i": i, "name": sym.name, "value": sym["st_value"],
                    "size": sym["st_size"], "sec": secname,
                    "type": sym["st_info"]["type"],
                    "bind": sym["st_info"]["bind"]})
            self.rel: Dict[str, List[Dict]] = {}
            for s in elf.iter_sections():
                if not isinstance(s, RelocationSection):
                    continue
                target = elf.get_section(s["sh_info"]).name
                recs = []
                for j, r in enumerate(s.iter_relocations()):
                    recs.append({"off": r["r_offset"], "type": r["r_info_type"],
                                 "sym": r["r_info_sym"],
                                 "rec_file_off": s["sh_offset"] + 8 * j})
                self.rel[target] = recs

    def size(self, name: str) -> int:
        return self.sec.get(name, {}).get("size", 0)

    def data(self, name: str) -> bytes:
        return self.sec.get(name, {}).get("data", b"")

    def section_symbol(self, name: str) -> Optional[int]:
        for s in self.syms:
            if s["type"] == "STT_SECTION" and s["sec"] == name:
                return s["i"]
        return None

    def defined(self, section: str) -> List[Dict]:
        return [s for s in self.syms
                if s["sec"] == section and s["type"] != "STT_SECTION"
                and s["bind"] == "STB_GLOBAL"]

    def text_word(self, off: int) -> int:
        return struct.unpack_from("<I", self.data(".text"), off)[0]


def _sx16(x: int) -> int:
    return x - 0x10000 if x & 0x8000 else x


def section_refs(obj: Obj, section: str) -> List[Tuple[int, List[int], int]]:
    """[(hi_off, [lo_offs], addend)] for every .text HI16/LO16 group that
    references `section` through its section symbol."""
    ssym = obj.section_symbol(section)
    out = []
    pending = None
    for r in obj.rel.get(".text", []):
        if r["sym"] != ssym:
            continue
        if r["type"] == R_MIPS_HI16:
            pending = [r["off"], [], None]
            out.append(pending)
        elif r["type"] == R_MIPS_LO16:
            lo = obj.text_word(r["off"]) & 0xFFFF
            if pending is None:
                raise DataLayoutError(f"{obj.path.name}: LO16 at "
                                      f"0x{r['off']:X} without a HI16")
            hi = obj.text_word(pending[0]) & 0xFFFF
            add = (hi << 16) + _sx16(lo)
            if pending[2] is not None and pending[2] != add:
                # one lui shared by two different addends (as1 CSE)
                pending[2] = ("mixed", pending[2], add)
            elif pending[2] is None:
                pending[2] = add
            pending[1].append(r["off"])
        elif r["type"] == R_MIPS_32:
            continue
    return [(g[0], g[1], g[2]) for g in out]


# ---------------------------------------------------------------------------
# Measuring true section sizes
# ---------------------------------------------------------------------------

def _strip_postproc(text: str) -> str:
    return "\n".join(l for l in text.splitlines() if "@PostProc" not in l)


def measure_sizes(overlay: str, name: str, src: Path, addr: int,
                  tmpdir: Path) -> Dict[str, int]:
    """True .data/.bss sizes (4-aligned) via a marker compile."""
    text = _strip_postproc(src.read_text(errors="ignore"))
    text += (f"\nchar {MARK_DATA}[1] = {{ 1 }};\nchar {MARK_BSS}[1];\n")
    mdir = tmpdir / "measure"
    mdir.mkdir(exist_ok=True)
    copy = mdir / src.name
    copy.write_text(text)
    o = compile_object(overlay, name, copy, addr, mdir)
    if o is None:
        raise DataLayoutError(f"{name}: measure compile failed")
    obj = Obj(o)
    out = {}
    for mark, sec in ((MARK_DATA, ".data"), (MARK_BSS, ".bss")):
        hit = [s for s in obj.syms if s["name"] == mark]
        if hit and hit[0]["sec"] == sec:
            out[sec] = hit[0]["value"]
            continue
        # Under -cckr IDO makes an uninitialised global COMMON, so the bss
        # marker does not land in .bss.  Only an empty .bss is then known.
        if Obj(compile_plain(overlay, name, src, addr, tmpdir)).size(sec):
            raise DataLayoutError(f"{name}: cannot measure {sec} "
                                  f"(marker not placed in it)")
        out[sec] = 0
    return out


def compile_plain(overlay: str, name: str, src: Path, addr: int,
                  tmpdir: Path) -> Path:
    pdir = tmpdir / "plain"
    pdir.mkdir(exist_ok=True)
    o = compile_object(overlay, name, src, addr, pdir)
    if o is None:
        raise DataLayoutError(f"{name}: compile failed")
    return o


def literal_pool_size(obj: Obj) -> Tuple[int, int, int]:
    """(lit_size, jt_off, jt_size) of an object's .rodata."""
    ro = obj.data(".rodata")
    jt_words = sorted(r["off"] for r in obj.rel.get(".rodata", [])
                      if r["type"] == R_MIPS_32)
    if jt_words:
        jt_off = jt_words[0]
        jt_end = jt_words[-1] + 4
        if jt_words != list(range(jt_off, jt_end, 4)):
            raise DataLayoutError(f"{obj.path.name}: .rodata relocations are "
                                  f"not one contiguous jump-table block")
        if any(ro[jt_end:]):
            raise DataLayoutError(f"{obj.path.name}: non-zero .rodata after "
                                  f"the jump tables")
        return jt_off, jt_off, jt_end - jt_off
    starts = []
    for _hi, _los, add in section_refs(obj, ".rodata"):
        if isinstance(add, tuple):
            starts += list(add[1:])
        elif add is not None:
            starts.append(add)
    if not starts:
        if any(ro):
            raise DataLayoutError(f"{obj.path.name}: unreferenced .rodata")
        return 0, 0, 0
    last = max(starts)
    nul = ro.index(b"\0", last)
    return align(nul + 1, 4), 0, 0


# ---------------------------------------------------------------------------
# Linking at computed addresses
# ---------------------------------------------------------------------------

def _defs_for(obj: Obj, overlay: str, skip: set) -> Tuple[str, List[str]]:
    lines, unresolved = [], []
    seen = set()
    for recs in obj.rel.values():
        for r in recs:
            s = obj.syms[r["sym"]]
            if s["type"] == "STT_SECTION" or s["name"] in seen:
                continue
            seen.add(s["name"])
            if s["name"] in skip:
                continue
            a = name_to_addr(s["name"], overlay)
            if a is None:
                if s["sec"] is None:
                    unresolved.append(s["name"])
                continue
            lines.append(f"  {s['name']} = 0x{a:08X};")
    return "\n".join(lines), unresolved


def split_rodata(obj_path: Path, out_path: Path, jt_off: int) -> Path:
    """Copy of the object whose .text references into .rodata[jt_off:] go
    through the symbol JT_SYM (addend - jt_off) instead of the .rodata
    section symbol.  Relocation records and addend fields only."""
    tmp = out_path.with_suffix(".sym.o")
    r = subprocess.run([OBJCOPY, "--add-symbol", f"{JT_SYM}=0,global",
                        str(obj_path), str(tmp)],
                       capture_output=True, text=True)
    if r.returncode != 0:
        raise DataLayoutError(f"objcopy --add-symbol: {r.stderr.strip()}")
    obj = Obj(tmp)
    jt_idx = [s["i"] for s in obj.syms if s["name"] == JT_SYM]
    if len(jt_idx) != 1:
        raise DataLayoutError("split: the jump-table symbol was not added")
    jt_idx = jt_idx[0]
    raw = obj.raw
    text_off = obj.sec[".text"]["offset"]
    ssym = obj.section_symbol(".rodata")
    recs = {r["off"]: r for r in obj.rel[".text"]}
    for hi_off, lo_offs, add in section_refs(obj, ".rodata"):
        if isinstance(add, tuple):
            a, b = add[1], add[2]
            if (a >= jt_off) != (b >= jt_off):
                raise DataLayoutError(
                    f"{obj_path.name}: one HI16 at 0x{hi_off:X} serves a "
                    f"literal AND a jump table -- cannot split")
            add = a if a >= jt_off else None
            if add is None:
                continue
            # every LO16 recomputes from its own immediate below
        elif add is None or add < jt_off:
            continue
        for off in [hi_off] + lo_offs:
            rec = recs[off]
            if rec["sym"] != ssym:
                raise DataLayoutError("split: unexpected relocation symbol")
            info = (jt_idx << 8) | rec["type"]
            struct.pack_into("<I", raw, rec["rec_file_off"] + 4, info)
        hi_word = obj.text_word(hi_off)
        new_hi = None
        for lo in lo_offs:
            w = obj.text_word(lo)
            full = ((hi_word & 0xFFFF) << 16) + _sx16(w & 0xFFFF)
            new = full - jt_off
            struct.pack_into("<I", raw, text_off + lo,
                             (w & 0xFFFF0000) | (new & 0xFFFF))
            h = ((new + 0x8000) >> 16) & 0xFFFF
            if new_hi is not None and new_hi != h:
                raise DataLayoutError("split: LO16s of one HI16 disagree")
            new_hi = h
        struct.pack_into("<I", raw, text_off + hi_off,
                         (hi_word & 0xFFFF0000) | new_hi)
    out_path.write_bytes(bytes(raw))
    return out_path


def link_at(obj_path: Path, overlay: str, tmpdir: Path, text_base: int,
            bases: Dict[str, int], extra_defs: Dict[str, int],
            skip_defs: set) -> Tuple[Dict[str, bytes], Dict[str, int]]:
    """Link one object with .text at text_base and each data section at its
    given base.  Returns ({section: bytes}, {global symbol: address})."""
    obj = Obj(obj_path)
    defs, unresolved = _defs_for(obj, overlay, skip_defs | set(extra_defs))
    defs += "\n" + "\n".join(f"  {k} = 0x{v:08X};"
                             for k, v in sorted(extra_defs.items()))
    if unresolved:
        raise DataLayoutError(f"{obj_path.name}: unresolved externs "
                              f"{sorted(unresolved)}")
    lines = [f"  .text 0x{text_base:08X} : SUBALIGN(4) {{ *(.text*) }}"]
    for sec in DATA_SECTIONS:
        if sec in bases:
            lines.append(f"  {sec} 0x{bases[sec]:08X} : SUBALIGN(1) "
                         f"{{ *({sec}) }}")
    script = ("OUTPUT_FORMAT(\"elf32-tradlittlemips\")\nOUTPUT_ARCH(mips)\n"
              "SECTIONS {\n" + "\n".join(lines) + "\n" + defs + "\n"
              "  /DISCARD/ : { *(.reginfo) *(.mdebug*) *(.comment) *(.pdr)"
              " *(.gnu.attributes) *(.MIPS.abiflags) *(.options)"
              " *(.gptab*) }\n}\n")
    ld = tmpdir / f"{obj_path.stem}.odfs.ld"
    ld.write_text(script)
    elf = tmpdir / f"{obj_path.stem}.odfs.elf"
    r = subprocess.run([LD, "-T", str(ld), "-o", str(elf), str(obj_path),
                        "--no-warn-mismatch", "-e", "0"],
                       capture_output=True, text=True)
    if r.returncode != 0:
        raise DataLayoutError(f"ld {obj_path.name}: {r.stderr.strip()}")
    from elftools.elf.elffile import ELFFile
    out: Dict[str, bytes] = {}
    syms: Dict[str, int] = {}
    with open(elf, "rb") as fh:
        e = ELFFile(fh)
        for sec in (".text",) + DATA_SECTIONS:
            s = e.get_section_by_name(sec)
            if s is not None and s["sh_type"] != "SHT_NOBITS":
                out[sec] = s.data()
        st = e.get_section_by_name(".symtab")
        for sym in st.iter_symbols():
            if sym["st_info"]["bind"] == "STB_GLOBAL" and sym.name:
                syms[sym.name] = sym["st_value"]
    return out, syms


# ---------------------------------------------------------------------------
# The region
# ---------------------------------------------------------------------------

def data_tu_sources(overlay: str) -> List[Path]:
    d = SRC_OVERLAY_DIR / overlay / "data"
    return sorted(d.glob("*.c")) if d.is_dir() else []


TU_RE = re.compile(r'(?m)^\s*(?:/\*|\*)\s*@TU:\s*([A-Za-z_][A-Za-z0-9_]*)')


def tu_starts(overlay: str, items) -> Tuple[List[int], Dict[Path, int]]:
    """The overlay's translation units, as the sorted list of their first
    function's address, and each data file's TU.  A TU is named by its first
    function: `@TU: <name>` in a data file, or one name per line in
    data/tu_starts.txt for a TU that has no file-scope data.  IDO pads each
    TU's .text to 16, so a TU start must be 16-aligned."""
    addr = {it.name: it.addr for it in items if it.in_sym}
    first = min(it.addr for it in items)
    starts = {first}
    per_file: Dict[Path, int] = {}
    names = []
    lst = SRC_OVERLAY_DIR / overlay / "data" / "tu_starts.txt"
    if lst.exists():
        for line in lst.read_text().splitlines():
            line = line.split("#", 1)[0].strip()
            if line:
                names.append((line, None))
    for f in data_tu_sources(overlay):
        m = TU_RE.search(f.read_text(errors="ignore"))
        names.append((m.group(1) if m else None, f))
    for n, f in names:
        a = first if n is None else addr.get(n)
        if a is None:
            raise DataLayoutError(f"{overlay}: TU start {n} is not a "
                                  f"function of the overlay")
        if a % 16:
            raise DataLayoutError(f"{overlay}: TU start {n} at 0x{a:08X} "
                                  f"is not 16-aligned")
        starts.add(a)
        if f is not None:
            if a in per_file.values():
                raise DataLayoutError(f"{overlay}: two data files for the "
                                      f"TU at {n}")
            per_file[f] = a
    return sorted(starts), per_file


def name_address(name: str, overlay: str) -> Optional[int]:
    try:
        return name_to_addr(name, overlay)
    except Exception:
        return None


def build_data_region(overlay: str, items, text_end: int, payload: bytearray,
                      overlay_base: int, verbose: bool = False) -> Dict:
    """Fill payload[text_end - base:] from source.  `items` are the layout
    CodeItems with .source set by the text phase.  Returns a report; raises
    DataLayoutError on any inconsistency."""
    rep: Dict = {"parts": [], "named_checked": 0, "text_relinked": 0}
    starts, dtu_tu = tu_starts(overlay, items)
    rep["tus"] = [f"0x{a:08X}" for a in starts]

    def tu_of(a: int) -> int:
        return bisect.bisect_right(starts, a) - 1

    with tempfile.TemporaryDirectory() as td_s:
        td = Path(td_s)
        # ---- 1. objects + true sizes ------------------------------------
        units = []   # (kind, name, src, addr, item, obj, sizes)
        for src in sorted(dtu_tu, key=lambda f: dtu_tu[f]):
            sub = td / f"dtu_{src.stem}"
            sub.mkdir()
            o = compile_object(overlay, src.stem, src, overlay_base, sub)
            if o is None:
                raise DataLayoutError(f"{src.name}: compile failed")
            obj = Obj(o)
            if obj.rel.get(".text"):
                raise DataLayoutError(f"{src.name}: the data TU has code "
                                      f"relocations")
            sizes = measure_sizes(overlay, src.stem, src, overlay_base, sub)
            lit, jt_off, jt_size = literal_pool_size_dtu(obj)
            sizes.update({"lit": lit, "jt_off": jt_off, "jt": jt_size,
                          "tu": tu_of(dtu_tu[src])})
            units.append(("dtu", src.stem, src, None, None, obj, sizes))
        for it in items:
            if it.source != "c":
                continue
            src = SRC_OVERLAY_DIR / overlay / f"{it.name}.c"
            sub = td / it.name
            sub.mkdir()
            o = compile_object(overlay, it.name, src, it.addr, sub)
            if o is None:
                raise DataLayoutError(f"{it.name}: compile failed")
            obj = Obj(o)
            if not any(obj.size(s) for s in DATA_SECTIONS):
                continue
            lit, jt_off, jt_size = literal_pool_size(obj)
            sizes = {"lit": lit, "jt_off": jt_off, "jt": jt_size,
                     ".data": 0, ".bss": 0, "tu": tu_of(it.addr)}
            if obj.size(".data") or obj.size(".bss"):
                sizes.update(measure_sizes(overlay, it.name, src, it.addr,
                                           sub))
            units.append(("fn", it.name, src, it.addr, it, obj, sizes))

        # Named .data/.bss definitions inside a function file duplicate the
        # data TU's (the unity TU defines them once, at the top); they must
        # agree byte for byte and contribute nothing.
        # A function file may also carry ANONYMOUS data after them: a
        # local-initialiser template, reached from .text through the section
        # symbol.  The anonymous part starts at the lowest such addend.
        for u in units:
            if u[0] != "fn":
                continue
            obj, sizes = u[5], u[6]
            for sec in (".data", ".bss"):
                named = obj.defined(sec)
                adds = [a for _h, _l, add in section_refs(obj, sec)
                        for a in (add[1:] if isinstance(add, tuple)
                                  else [add]) if a is not None]
                anon = min(adds) if adds else None
                sizes[f"{sec}_anon"] = 0
                if not named:
                    continue
                if sec == ".bss" and anon is not None:
                    raise DataLayoutError(f"{u[1]}: .bss mixes named and "
                                          f"anonymous data")
                # IDO records st_size 0 for an initialised array, so a
                # named extent is unknown: everything below the anonymous
                # part (or the whole section) is the duplicate, compared
                # byte for byte with the data file's definition.
                head = sizes[sec] if anon is None else anon
                if any(s["value"] >= head for s in named):
                    raise DataLayoutError(
                        f"{u[1]}: a named {sec} definition follows its "
                        f"anonymous data -- move it to the data file")
                u[6].setdefault("dups", []).extend(
                    (sec, s["name"], s["value"], head) for s in named)
                sizes[sec] -= head
                sizes[f"{sec}_anon"] = head

        # ---- 2. layout -----------------------------------------------------
        # The linker emits every TU's .rodata, then every TU's .data, then
        # every .bss, each input section 16-aligned.  Inside one TU's
        # .rodata IDO puts all literals (source order: the TU's data file,
        # then its functions) before all jump tables (function order).
        # Units are sorted by TU, data file first, then function address.
        units.sort(key=lambda u: (u[6]["tu"], 0 if u[0] == "dtu" else 1,
                                  u[3] or 0))
        place: Dict[str, Dict[str, int]] = {u[1]: {} for u in units}
        tus = sorted({u[6]["tu"] for u in units})

        def lay(cur: int, sec_keys, sec_name: str) -> int:
            for t in tus:
                members = [u for u in units if u[6]["tu"] == t]
                if not any(u[6][k] for u in members for k in sec_keys):
                    continue
                cur = align(cur, 16)
                for k in sec_keys:
                    for u in members:
                        if u[6][k]:
                            place[u[1]][k] = cur
                            cur += u[6][k]
            return cur

        cur = align(text_end, 16)
        rep["rodata_start"] = cur
        cur = lay(cur, ("lit", "jt"), ".rodata")
        cur = align(cur, 16)
        rep["data_start"] = cur
        cur = lay(cur, (".data",), ".data")
        cur = align(cur, 16)
        rep["bss_start"] = cur
        cur = lay(cur, (".bss",), ".bss")
        rep["bss_end"] = cur

        # ---- 3. link + place ---------------------------------------------
        region = bytearray(len(payload) - (text_end - overlay_base))
        region_base = text_end

        def put(addr: int, blob: bytes, what: str):
            o = addr - region_base
            if o < 0 or o + len(blob) > len(region):
                raise DataLayoutError(f"{what} at 0x{addr:08X} is outside "
                                      f"the data region")
            region[o:o + len(blob)] = blob
            rep["parts"].append((f"0x{addr:08X}", len(blob), what))

        for kind, name, src, addr, it, obj, sizes in units:
            p = place[name]
            bases: Dict[str, int] = {}
            extra: Dict[str, int] = {}
            obj_path = obj.path
            if sizes["lit"] and sizes["jt"]:
                obj_path = split_rodata(obj.path,
                                        obj.path.with_name(name + ".split.o"),
                                        sizes["jt_off"])
                bases[".rodata"] = p["lit"]
                extra[JT_SYM] = p["jt"]
            elif sizes["jt"]:
                bases[".rodata"] = p["jt"] - sizes["jt_off"]
            elif sizes["lit"]:
                bases[".rodata"] = p["lit"]
            for sec in (".data", ".bss"):
                if sec in p:
                    bases[sec] = p[sec] - sizes.get(f"{sec}_anon", 0)
            skip = set()
            dups = sizes.get("dups", [])
            if kind == "dtu":
                text_base = overlay_base   # no code; .text is types.h only
                # the data TU's own definitions are placed by the layout,
                # not by their names -- the names are CHECKED below
                skip = {s["name"] for sec in DATA_SECTIONS
                        for s in obj.defined(sec)}
                # A function named in data (a pointer table) is its ENTRY:
                # the start of its code window (@SymbolStart), as a linker
                # would define it.  The landed name table records the
                # pre-absorption prologue address for FRONT-class
                # functions (rensyu_main0: 0x801320E8, entry 0x801320E0).
                for fit in items:
                    if fit.in_sym:
                        extra[fit.name] = fit.addr
            else:
                text_base = addr - get_func_text_offset(obj.path, name)
                # function pointers in the function's own data (a local
                # table template) are entries too; a symbol the .text also
                # references must agree on one address
                entry = {fit.name: fit.addr for fit in items if fit.in_sym}
                text_syms = {obj.syms[r["sym"]]["name"]
                             for r in obj.rel.get(".text", [])}
                for sec in (".data", ".rodata"):
                    for r in obj.rel.get(sec, []):
                        sn = obj.syms[r["sym"]]["name"]
                        if sn not in entry:
                            continue
                        if sn in text_syms and \
                                name_address(sn, overlay) != entry[sn]:
                            raise DataLayoutError(
                                f"{name}: {sn} is called at its name "
                                f"address and stored as its entry")
                        extra[sn] = entry[sn]
                # a duplicated named definition: its section is not placed
                # by the layout; link it where its name says (the bytes are
                # checked against the data TU below)
                for sec in (".data", ".bss"):
                    want = {name_address(d[1], overlay) - d[2]
                            for d in dups if d[0] == sec
                            and name_address(d[1], overlay) is not None}
                    if len(want) > 1:
                        raise DataLayoutError(f"{name}: named {sec} "
                                              f"definitions disagree")
                    if want and sec not in bases:
                        bases[sec] = want.pop()
                    elif want and sec in bases:
                        # named head + anonymous tail: the head's names are
                        # assigned by the script; the bytes are checked below
                        pass
            out, syms = link_at(obj_path, overlay, td, text_base, bases,
                                extra, skip)
            ro = out.get(".rodata", b"")
            if sizes["lit"]:
                put(p["lit"], ro[:sizes["lit"]], f"{name} literals")
            if sizes["jt"]:
                o = sizes["jt_off"]
                put(p["jt"], ro[o:o + sizes["jt"]], f"{name} jump tables")
            if ".data" in p:
                h = sizes.get(".data_anon", 0)
                put(p[".data"], out[".data"][h:h + sizes[".data"]],
                    f"{name} .data")
            if kind == "dtu":
                for sec in DATA_SECTIONS:
                    for s in obj.defined(sec):
                        want = name_address(s["name"], overlay)
                        got = syms.get(s["name"])
                        if want is not None and got != want:
                            raise DataLayoutError(
                                f"{name}: {s['name']} lands at "
                                f"0x{(got or 0):08X}, its name says "
                                f"0x{want:08X}")
                        rep["named_checked"] += 1
            else:
                # .text must not move: compare with the text phase's bytes
                fo = addr - overlay_base
                tx = out[".text"]
                toff = get_func_text_offset(obj.path, name)
                body = tx[toff:toff + it.size]
                if body != bytes(payload[fo:fo + it.size]):
                    raise DataLayoutError(
                        f"{name}: .text linked against the computed data "
                        f"layout differs from the text build")
                rep["text_relinked"] += 1
                for sec, dname, off, size in dups:
                    a = name_address(dname, overlay)
                    if a is None:
                        raise DataLayoutError(f"{name}: {dname} has no "
                                              f"address")
                    rep.setdefault("dup_checks", []).append(
                        (name, dname, a, sec, off, size))

        # duplicated named definitions must equal the region bytes
        for fname, dname, a, sec, off, size in rep.get("dup_checks", []):
            u = [x for x in units if x[1] == fname][0]
            # the whole measured section, based where the name puts it
            a -= off
            blob = u[5].data(sec)[:size] if sec == ".data" else bytes(size)
            ro = a - region_base
            if bytes(region[ro:ro + size]) != blob:
                raise DataLayoutError(f"{fname}: its definition of {dname} "
                                      f"disagrees with the data TU")

        payload[text_end - overlay_base:] = region
        rep["units"] = [(u[0], u[1], {k: v for k, v in u[6].items()
                                      if k != "dups"}) for u in units]
        rep["place"] = {k: {s: f"0x{v:08X}" for s, v in d.items()}
                        for k, d in place.items()}
    return rep


def literal_pool_size_dtu(obj: Obj) -> Tuple[int, int, int]:
    """The data TU's .rodata: const definitions and literals, no jump
    tables.  Its true size is the end of the last definition (st_size is
    0 for an incomplete array, so fall back to the string extent)."""
    ro = obj.data(".rodata")
    if not any(ro):
        return 0, 0, 0
    end = 0
    for s in obj.defined(".rodata"):
        e = s["value"] + s["size"]
        if s["size"] == 0:
            e = ro.index(b"\0", s["value"]) + 1
        end = max(end, e)
    last_nz = max(i for i, b in enumerate(ro) if b)
    if last_nz >= end:
        # anonymous literals after the last named definition
        end = ro.index(b"\0", last_nz) + 1
    return align(end, 4), 0, 0
