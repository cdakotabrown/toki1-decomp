#!/usr/bin/env python3
"""Resolve overlay relocations to convert reloc_only -> byte_perfect (HONESTLY).

A `reloc_only` overlay function is byte-identical to the target EXN except in
relocation fields: the C references `extern func_XXXXXXXX` / `extern D_XXXXXXXX`
placeholders whose addresses the overlay linker never resolves (the generated
linker.ld defines no addresses for these externs — see generate_linker.py), so
the JAL targets stay 0 and the LUI/ADDIU/LW %hi/%lo pairs stay 0.

KEY INSIGHT (the honest mechanism): every such symbol carries its own real
address in its NAME. `func_80011DFC` is the function at 0x80011DFC; `D_8015ABCD`
is the datum at 0x8015ABCD. So we can resolve the relocations the way the
original toolchain did — by LINKING the PostProc'd object at the function's true
address with each `func_/D_` symbol DEFINED at its name-encoded address — and let
GNU ld apply R_MIPS_26 / R_MIPS_HI16 / R_MIPS_LO16 for real. The relocated bytes
are then compared STRICTLY (no copy-from-target masking). A match proves the
symbol references are correct; it is NOT painting (we never read the answer).

Since 2026-09-27 a main-binary symbol may also carry a REAL name:
those resolve through the landed-name table in
config/symbol_names.json (name -> retail address, consulted before the suffix
rule; see load_name_table). Same honesty tier: the table is a rename ledger,
not a copy of the target.  Since 2026-10 an OVERLAY function may be renamed too
(tools/overlay/apply_overlay_symbols.py): its record is keyed "<OV>:0xADDR",
visible only inside that overlay, and resolves through overlay_names.py.

This differs fundamentally from fixup_relocations() (overlay_postproc.py /
build_level3_c.py), which COPIES the immediate from the target when opcode+regs
match — that reproduces the SHA1 but cannot independently verify the symbol is
right. Here ld computes the immediate from the symbol address; the only input is
the symbol NAME the decompiler already wrote.

Usage:
    # Validate the landed-name table (names that resolve before the suffix rule):
    python3 tools/overlay/resolve_overlay_relocs.py --check-name-table

    # Verify a single function (strict, name-derived link):
    python3 tools/overlay/resolve_overlay_relocs.py EVENT func_8013315C -v

    # Scan an overlay: report how many reloc_only become byte_perfect:
    python3 tools/overlay/resolve_overlay_relocs.py EVENT --scan

    # Scan everything reloc_only in the status JSON:
    python3 tools/overlay/resolve_overlay_relocs.py --scan-all

    # @StaticAddr regression fixtures (file-private static placement):
    python3 tools/overlay/resolve_overlay_relocs.py --self-test
"""
import sys
import os
import re
import json
import struct
import subprocess
import tempfile
from pathlib import Path
from typing import Dict, List, Optional, Tuple

ROOT = Path(__file__).parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "build"))
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

from parse_postproc import (parse_compile_flags, parse_compile_env, parse_compiler,
                            parse_symbol_start_text, parse_symbol_size_text)
from overlay_postproc import parse_postproc, apply_postproc, get_func_text_offset
from ido_flags import (base_flags, compile_flag_tail, glue_option_values,
                       parse_extra_cflags)

CONFIG_DIR = ROOT / "config" / "overlay"
SRC_OVERLAY_DIR = ROOT / "src" / "overlay"
ASSETS_DIR = ROOT / "assets" / "overlays"
INCLUDE_DIR = ROOT / "include"
STATUS_FILE = ROOT / "config" / "overlay_match_status.json"

IDO_CC = ROOT / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
GCCWRAP = ROOT / "tools" / "gcc" / "gccwrap.py"
# Base line from tools/build/ido_flags.py; per-file PIC mode added at the
# compile site via pic_flags().
IDO_FLAGS = base_flags([INCLUDE_DIR, f"{INCLUDE_DIR}/psyq"])
GCCWRAP_FLAGS = {"--fill-jalr", "--fill-epilogue", "--fill-all",
                 "--no-delay-slots", "--no-cross-jumping"}
OVERLAY_BASE = 0x80132000

LD = "mipsel-linux-gnu-ld"
OBJCOPY = "mipsel-linux-gnu-objcopy"
OBJDUMP = "mipsel-linux-gnu-objdump"

# Symbols whose name directly encodes their RAM address. The original decomp
# uses several prefixes for address-named externs: func_/sub_/FUN_ (code),
# D_/DATA_/jtbl_/jpt_/gUnknown_ (data). We match ANY identifier-prefix followed
# by exactly 8 hex digits, and additionally require the decoded value to be a
# plausible PS1 RAM address (0x8000_0000-0x801F_FFFF, the 2MB main RAM window) so
# that local labels that merely happen to contain hex (e.g. label_0001) can never
# be mis-resolved. Only symbols that actually appear as RELOCATION targets are
# ever passed here, so this is safe.
# Optional short lowercase suffix (D_800EE760_b, D_8014C9BC_s): the documented
# CSE-defeating alias-view idiom — the address still comes from the NAME, the
# suffix only makes the compiler treat it as a distinct symbol.
NAME_ADDR_RE = re.compile(r'^[A-Za-z_][A-Za-z0-9_]*?_([0-9A-Fa-f]{8})(?:_[a-z]{1,3})?$')
RAM_LO = 0x80000000
RAM_HI = 0x801FFFFF

# NAME TABLE (2026-09-27): a main-binary
# symbol that has been given its real name (`indexed_byte_lsb_set`, `bustup_cd_read`) no
# longer carries its address in the name, so the suffix rule above cannot link
# it. config/symbol_names.json is the single source of truth for those renames
# (retail address -> name; `landed: true` once the rename is in the tree). The
# table built from it is consulted FIRST; the suffix rule stays the fallback for
# every address-named extern. Nothing here reads the target binary: the table
# only says which retail address a NAME stands for, exactly as the `_XXXXXXXX`
# suffix does, so a match through it is the same honesty tier.
#
# Refusals are HARD (NameTableError), never a silent fall-through to the suffix
# rule: a missing/unreadable table, one name landed at two addresses, a landed
# address outside the RAM window, or an address-shaped landed name whose own
# suffix names a different address (`foo_8004ABCD` landed at 0x8004ABD0 would
# otherwise resolve one way here and another way everywhere the suffix is read).
# Address-shaped names that AGREE with their suffix are left to the suffix rule.
# TOKI_NO_NAME_TABLE=1 disables the table (negative-control runs only).
SYMBOL_NAMES_FILE = ROOT / "config" / "symbol_names.json"

# OVERLAY-SCOPED NAMES (tools/overlay/apply_overlay_symbols.py): an overlay
# function may carry a real name too.  Those records are keyed "<OV>:0xADDR" in
# the same file and are visible only inside their own overlay (every overlay
# loads at 0x80132000, so the same name or address means different things in
# different overlays).  The table logic lives in tools/overlay/overlay_names.py
# (stdlib-only, so overlay_postproc / build_clean_exn can use it too); this
# module re-exports it under its historical names.
import overlay_names as _ON                                        # noqa: E402
from overlay_names import NameTableError                           # noqa: E402,F401


def load_name_table(force: bool = False, overlay: Optional[str] = None) -> Dict[str, int]:
    """name -> retail address for every LANDED entry of config/symbol_names.json
    (kind func or data).  Raises NameTableError on any inconsistency.

    No `overlay`: the main-binary table (unchanged contract).  With one: the
    main table overlaid with that overlay's own landed names."""
    if force:
        _ON.load_tables(force=True)
    return _ON.scoped_table(overlay) if overlay else _ON.main_table()


def parse_sym_file(overlay_name: str) -> Dict[str, Tuple[int, int]]:
    sym_file = CONFIG_DIR / f"{overlay_name}.sym"
    funcs = {}
    if not sym_file.exists():
        return funcs
    for line in sym_file.read_text().splitlines():
        line = line.strip()
        if not line or line.startswith('#'):
            continue
        parts = line.split(':')
        if len(parts) >= 3:
            funcs[parts[2]] = (int(parts[0], 16), int(parts[1]))
    return funcs


def name_to_addr(sym: str, overlay: Optional[str] = None) -> Optional[int]:
    """Return the RAM address a relocation symbol stands for, else None.

    Order: (1) the OVERLAY's own landed names (when `overlay` is given);
    (2) the landed main-name table (config/symbol_names.json); (3) the address
    encoded in a func_/D_/jtbl_ name (the original rule, unchanged).  Without
    `overlay`, a name landed at different addresses in different overlays is
    refused (NameTableError): say which overlay you are in.
    """
    return _ON.name_to_addr(sym, overlay)


def get_reloc_symbols(obj_file: Path) -> List[Tuple[int, str, str]]:
    """Return [(offset, reloc_type, symbol_name), ...] for .text relocations."""
    r = subprocess.run([OBJDUMP, "-r", str(obj_file)],
                       capture_output=True, text=True)
    out = []
    in_text = False
    for line in r.stdout.splitlines():
        if line.startswith("RELOCATION RECORDS FOR"):
            in_text = ".text" in line
            continue
        if not in_text:
            continue
        m = re.match(r'([0-9a-f]+)\s+(\S+)\s+(\S+)', line)
        if m:
            out.append((int(m.group(1), 16), m.group(2), m.group(3)))
    return out


def compile_object(overlay_name: str, func_name: str, src_file: Path,
                   func_addr: int, tmpdir: Path,
                   extra_cflags: Optional[List[str]] = None) -> Optional[Path]:
    """Compile + PostProc one overlay function, returning the final object path.

    Mirrors test_overlay_match.test_overlay_function's build steps, but keeps
    relocation records (it does NOT extract raw .text). Returns None on error.

    ``extra_cflags`` appends screening flags to the compile line (see
    tools/overlay/screen_flag.py). Empty/None reproduces the stock line exactly.
    """
    content = src_file.read_text(errors='ignore')
    extra_cflags = list(extra_cflags or [])
    postproc_steps = parse_postproc(src_file)
    extra_flags = parse_compile_flags(src_file)
    compile_env = parse_compile_env(src_file)
    compiler = parse_compiler(src_file)

    obj_file = tmpdir / f"{func_name}.o"

    if compiler.startswith("gcc"):
        version_number = compiler[3:]
        cmd = [sys.executable, str(GCCWRAP), str(src_file), "-o", str(obj_file),
               "--gcc-version", version_number,
               "-I", str(INCLUDE_DIR), "-I", str(INCLUDE_DIR / "psyq")]
        cc1_extra = []
        for flag in list(extra_flags) + extra_cflags:
            (cmd.append(flag) if flag in GCCWRAP_FLAGS else cc1_extra.append(flag))
        if cc1_extra:
            cmd.extend(["--extra-flags", " ".join(cc1_extra)])
    else:
        cmd = [str(IDO_CC)] + IDO_FLAGS \
              + compile_flag_tail(src_file, compile_flags=extra_flags,
                                  extra_cflags=extra_cflags) + \
              ["-c", str(src_file), "-o", str(obj_file)]

    env = os.environ.copy()
    env["TMPDIR"] = str(tmpdir)
    if compile_env:
        env.update(compile_env)
    try:
        r = subprocess.run(cmd, capture_output=True, text=True, timeout=60,
                           env=env, cwd=str(tmpdir))
    except Exception:
        return None
    if r.returncode != 0:
        return None

    if postproc_steps:
        asm_file = ROOT / "asm" / "overlay" / overlay_name / f"{func_name}.s"
        try:
            pp_ok = apply_postproc(obj_file, postproc_steps, func_addr,
                                   overlay_name, verbose=False,
                                   target_asm=asm_file if asm_file.exists()
                                   else None)
        except SystemExit:
            return None
        except Exception:
            return None
        if not pp_ok:
            # D1: a partially-applied chain produces diffs that masquerade as
            # deep codegen residuals. Refuse to compare it.
            print("Error: PostProc chain failed (failing step named above) -- "
                  "refusing to compare partial output. Fix or remove the "
                  "stale directive.", file=sys.stderr)
            return None

    # apply_postproc writes the final result back into obj_file in-place
    # (its .pp{i}.o chain ends by replacing obj_file); confirm it exists.
    return obj_file if obj_file.exists() else None



# ---------------------------------------------------------------------------
# @StaticAddr: a file-private `static` datum defined in the TU (2026-10-03).
#
# IDO keeps a global in a CALLER-saved register across calls (store before the
# jal, reload after) only when it is a file-private `static`; retail did that
# for ETC's D_80150164, so the honest C defines it `static`. A static is a
# LOCAL definition: the object relocates every reference against its own
# `.bss`/`.data` SECTION symbol plus an offset, so there is no name for
# name_to_addr to decode and the link would place the section anywhere.
#
#     /* @StaticAddr: D_80150164 0x80150164 */
#
# places the section that holds that static so the static lands on ADDR. It is
# the .bss/.data sibling of @RodataBase, with two extra checks @RodataBase
# cannot make:
#   * a name of the form D_<8 hex> (or any NAME_ADDR_RE name) must name the
#     same address as the directive, else HARD refusal -- the directive can
#     never relocate a name that says otherwise;
#   * the static is located by NAME in the compiled object (ELF symtab for a
#     gcc object, the .mdebug local symbols for an IDO object, which drops
#     statics from the ELF symtab), so the section base is derived
#     (ADDR - offset of that static), never read from the target; every
#     static sharing a placed section must be listed, and all must agree on
#     one base, else HARD refusal.
# Like a D_<addr> extern, a wrong address fails the strict compare.
# ---------------------------------------------------------------------------

STATIC_ADDR_RE = re.compile(
    r'(?m)^\s*(?:/\*|\*)\s*@StaticAddr:\s*([A-Za-z_][A-Za-z0-9_]*)\s+'
    r'(0x[0-9A-Fa-f]+)\b')

# ECOFF storage classes for data (sym.h): scData=2, scBss=3, scRData=15? --
# only the ones IDO emits for a file-scope static at -G0 / -G8.
_ECOFF_SC_SECTION = {2: '.data', 3: '.bss', 13: '.sdata', 14: '.sbss',
                     15: '.rdata'}
_ECOFF_ST_STATIC = 2


class StaticAddrError(RuntimeError):
    """An @StaticAddr directive cannot be honoured."""


def parse_static_addrs_text(content: str) -> Dict[str, int]:
    """Parse every ANCHORED `@StaticAddr: NAME 0xADDR` line (a line that OPENS
    with the directive, inside a comment, so header prose that merely mentions
    it does not count). Refuses a duplicate name and an address-encoded name
    whose own address differs from the directive."""
    out: Dict[str, int] = {}
    for m in STATIC_ADDR_RE.finditer(content):
        name, addr = m.group(1), int(m.group(2), 16)
        if not (RAM_LO <= addr <= RAM_HI):
            raise StaticAddrError(
                f"@StaticAddr {name}: 0x{addr:08X} is outside PS1 RAM")
        enc = NAME_ADDR_RE.match(name)
        if enc and int(enc.group(1), 16) != addr:
            raise StaticAddrError(
                f"@StaticAddr {name}: its NAME encodes 0x{enc.group(1).upper()}"
                f" but the directive says 0x{addr:08X} -- refused")
        if name in out and out[name] != addr:
            raise StaticAddrError(f"@StaticAddr {name}: given twice")
        out[name] = addr
    return out


def object_statics(obj_file: Path) -> Dict[str, Tuple[str, int]]:
    """Map each file-private static DATUM of the object to (section, offset).

    gcc/GNU as keeps statics as STB_LOCAL symbols in the ELF symtab; IDO drops
    them from the symtab and records them only in .mdebug (stStatic, with the
    storage class naming the section). The mdebug written by the IDO recomp is
    BIG-endian; the magic (0x7009) picks the byte order."""
    from elftools.elf.elffile import ELFFile
    out: Dict[str, Tuple[str, int]] = {}
    with open(obj_file, 'rb') as fh:
        elf = ELFFile(fh)
        data = None
        symtab = elf.get_section_by_name('.symtab')
        if symtab is not None:
            for sym in symtab.iter_symbols():
                if (sym['st_info']['bind'] == 'STB_LOCAL'
                        and sym['st_info']['type'] == 'STT_OBJECT'
                        and isinstance(sym['st_shndx'], int)):
                    sec = elf.get_section(sym['st_shndx']).name
                    out[sym.name] = (sec, sym['st_value'])
        md = elf.get_section_by_name('.mdebug')
        if md is not None:
            fh.seek(0)
            data = fh.read()
            base = md['sh_offset']
            magic = struct.unpack_from('<H', data, base)[0]
            end = '<' if magic == 0x7009 else '>'
            if struct.unpack_from(end + 'H', data, base)[0] == 0x7009:
                hdr = struct.unpack_from(end + 'HH23I', data, base)
                isym_max, cb_sym = hdr[9], hdr[10]
                cb_ss = hdr[16]
                ifd_max, cb_fd = hdr[19], hdr[20]
                # FDR: adr, rss, issBase, cbSs, isymBase, csym, ... (72 bytes)
                for f in range(ifd_max):
                    fo = cb_fd + 72 * f
                    _adr, _rss, iss_base, _cbss, isym_base, csym = \
                        struct.unpack_from(end + '6I', data, fo)
                    for i in range(isym_base, min(isym_base + csym, isym_max)):
                        iss, value, bits = struct.unpack_from(
                            end + '3I', data, cb_sym + 12 * i)
                        if end == '>':
                            st, sc = bits >> 26, (bits >> 21) & 0x1f
                        else:
                            st, sc = bits & 0x3f, (bits >> 6) & 0x1f
                        if st != _ECOFF_ST_STATIC or sc not in _ECOFF_SC_SECTION:
                            continue
                        so = cb_ss + iss_base + iss
                        name = data[so:data.index(b'\0', so)].decode(
                            'ascii', 'replace')
                        out.setdefault(name, (_ECOFF_SC_SECTION[sc], value))
    return out


def static_section_bases(obj_file: Path,
                         static_addrs: Dict[str, int]) -> Dict[str, int]:
    """Section -> absolute base so every @StaticAddr static lands on its
    address. HARD refusal (StaticAddrError) if a named static is not defined
    in the object, two statics demand different bases for one section, or a
    placed section holds a static the directives do not list."""
    if not static_addrs:
        return {}
    statics = object_statics(obj_file)
    bases: Dict[str, int] = {}
    for name, addr in sorted(static_addrs.items()):
        if name not in statics:
            raise StaticAddrError(
                f"@StaticAddr {name}: no file-private static of that name is "
                f"defined in the object (an extern needs no directive)")
        sec, off = statics[name]
        base = (addr - off) & 0xFFFFFFFF
        if sec in bases and bases[sec] != base:
            raise StaticAddrError(
                f"@StaticAddr {name}: needs {sec} at 0x{base:08X} but another "
                f"static already placed it at 0x{bases[sec]:08X}")
        bases[sec] = base
    for name, (sec, _off) in statics.items():
        if sec in bases and name not in static_addrs:
            raise StaticAddrError(
                f"static {name} shares placed section {sec} but has no "
                f"@StaticAddr -- list every static of a placed section")
    return bases


# @LiteralBase: the address of this function's string-literal pool.  IDO
# emits a TU's .rodata as [every literal, source order][every jump table,
# function order], so in the original one-TU overlay a function's literals
# and its jump tables are NOT adjacent.  When a function's C uses string
# literals AND a switch table, its .rodata is split at the first jump-table
# word: the literal head is linked at @LiteralBase and the jump-table tail at
# @RodataBase (overlay_data_from_source.split_rodata; relocation records
# only).  With literals and no jump table, @LiteralBase alone places .rodata.
# Same honesty tier as @RodataBase: a wrong address fails the compare, and
# build_clean_exn --data-from-source recomputes both from the TU layout.
LITERAL_BASE_RE = re.compile(
    r'(?m)^\s*(?:/\*|\*)\s*@LiteralBase:\s*(0x[0-9A-Fa-f]+)\b')
# @DataBase: the address of this function's own anonymous .data -- the
# template of an initialised local aggregate (`T x[] = {...}` inside the
# function), which IDO emits in .data in source order and copies at entry.
# Places the object's .data section; symbols the template names (function
# pointers) only need to link, the per-function compare reads .text.
DATA_BASE_RE = re.compile(
    r'(?m)^\s*(?:/\*|\*)\s*@DataBase:\s*(0x[0-9A-Fa-f]+)\b')


def link_options_from_source(content: str) -> Dict:
    """The link_resolved keyword arguments a source's header asks for:
    `rodata_base` (@RodataBase) and `static_addrs` (@StaticAddr). Every
    consumer that links a function should use this so the two directives are
    honoured identically everywhere."""
    opts: Dict = {'rodata_base': None, 'static_addrs': None,
                  'literal_base': None, 'data_base': None}
    m = re.search(r'@RodataBase:\s*(0x[0-9A-Fa-f]+)', content)
    if m:
        opts['rodata_base'] = int(m.group(1), 16)
    m = LITERAL_BASE_RE.search(content)
    if m:
        opts['literal_base'] = int(m.group(1), 16)
    m = DATA_BASE_RE.search(content)
    if m:
        opts['data_base'] = int(m.group(1), 16)
    sa = parse_static_addrs_text(content)
    if sa:
        opts['static_addrs'] = sa
    return opts


def link_resolved(obj_file: Path, func_addr: int, func_size: int,
                  tmpdir: Path,
                  rodata_base: Optional[int] = None,
                  func_name: Optional[str] = None,
                  static_addrs: Optional[Dict[str, int]] = None,
                  overlay_name: Optional[str] = None,
                  literal_base: Optional[int] = None,
                  data_base: Optional[int] = None
                  ) -> Optional[Tuple[bytes, Dict[str, int], List[str]]]:
    """Link obj at func_addr with every func_/D_ symbol DEFINED at its
    name-encoded address. Returns (text_bytes_for_func, resolved_syms,
    unresolved_syms) or None on link failure.

    rodata_base: if given, place the object's `.rodata` at this absolute
    address (a single contiguous block — e.g. a switch jump table). This lets
    a function whose only unresolved reloc is the anonymous `.rodata` section
    symbol byte-match: ld computes the %hi/%lo from rodata_base, exactly as it
    does for a name-encoded D_/func_ symbol. SUBALIGN(4) is required — ld's
    default .rodata alignment (2**4) would bump a non-16-aligned base.

    static_addrs: {name: addr} from @StaticAddr -- place the section holding
    each named file-private static so the static lands on addr (see
    static_section_bases; raises StaticAddrError on any inconsistency).

    overlay_name: the overlay being linked.  Its landed names (apply_overlay_
    symbols.py) resolve before the main table; omitted, a name landed at
    different addresses in different overlays is refused rather than guessed.

    text_bytes_for_func is the func_size window starting at the function's
    .text offset (accounting for leading thunks via get_func_text_offset).
    """
    jt_def = ""
    if literal_base is not None:
        # @LiteralBase: split .rodata into the literal pool (at literal_base)
        # and the jump tables (at rodata_base, through a split symbol).
        from overlay_data_from_source import (Obj, literal_pool_size,
                                              split_rodata, JT_SYM,
                                              DataLayoutError)
        try:
            lit, jt_off, jt_size = literal_pool_size(Obj(obj_file))
            if jt_size:
                if rodata_base is None:
                    return None
                obj_file = split_rodata(obj_file,
                                        tmpdir / (obj_file.stem + '.lit.o'),
                                        jt_off)
                jt_def = f"  {JT_SYM} = 0x{rodata_base:08X};\n"
        except DataLayoutError:
            return None
        rodata_base = literal_base

    relocs = get_reloc_symbols(obj_file)
    resolved = {}
    unresolved = []          # all non-name-encoded reloc symbols
    undefined_externs = []   # subset that ld would leave undefined (need defsym)
    seen = set()
    for _off, _typ, sym in relocs:
        if sym in seen:
            continue
        seen.add(sym)
        addr = name_to_addr(sym, overlay_name)
        if sym == '__odfs_jt' and jt_def:
            continue
        if addr is not None:
            resolved[sym] = addr
        else:
            unresolved.append(sym)
            # Section symbols (.rodata/.data/.bss) are DEFINED in the object —
            # ld places them (at the wrong runtime address, so the function can
            # never match), but it does NOT fail. Truly-undefined externs
            # (function-local static data like D_func_..._0) would make ld fail;
            # define them at 0 so the link completes and the function lands in
            # the residual bucket with its unresolved symbols recorded, instead
            # of crashing the scan.
            if not sym.startswith('.'):
                undefined_externs.append(sym)

    # The function's own start offset inside .text (thunks live before it).
    text_off = get_func_text_offset(obj_file, func_name)
    # Place .text so the FUNCTION lands at func_addr: section base =
    # func_addr - text_off.
    section_base = func_addr - text_off

    defs = "\n".join(f"  {s} = 0x{a:08X};" for s, a in sorted(resolved.items()))
    # Sentinel definitions for genuinely-undefined externs (value is wrong on
    # purpose — these functions cannot be byte_perfect without a data map).
    defs += "\n" + "\n".join(f"  {s} = 0x00000000;"
                             for s in sorted(set(undefined_externs)))
    rodata_line = ""
    if rodata_base is not None:
        rodata_line = (f"  .rodata 0x{rodata_base:08X} : SUBALIGN(4) "
                       f"{{ *(.rodata*) }}\n")
    if data_base is not None:
        # @DataBase: the function's local-initialiser template.  The symbols
        # its words name (.rel.data) must link; their values only reach
        # .data, which this compare does not read.
        # A file-scope definition the function's file keeps for codegen (a
        # same-TU initialised global) precedes the template in .data and is
        # addressed by its own name; @DataBase names the template, the first
        # .data byte the code reaches through the section symbol.
        from overlay_data_from_source import Obj, section_refs
        _o = Obj(obj_file)
        _adds = [a for _h, _l, add in section_refs(_o, '.data')
                 for a in (add[1:] if isinstance(add, tuple) else [add])
                 if a is not None]
        _anon = min(_adds) if _adds else 0
        rodata_line += (f"  .data 0x{data_base - _anon:08X} : SUBALIGN(1) "
                        f"{{ *(.data) }}\n")
        unresolved = [u for u in unresolved if u != '.data']
        for _r in _o.rel.get('.data', []):
            _s = _o.syms[_r['sym']]
            if (_s['type'] == 'STT_SECTION' or _s['sec'] is not None
                    or _s['name'] in resolved
                    or _s['name'] in undefined_externs):
                continue
            _a = name_to_addr(_s['name'], overlay_name)
            if _a is not None:
                resolved[_s['name']] = _a
            else:
                undefined_externs.append(_s['name'])
        defs = "\n".join(f"  {s} = 0x{a:08X};"
                         for s, a in sorted(resolved.items()))
        defs += "\n" + "\n".join(f"  {s} = 0x00000000;"
                                 for s in sorted(set(undefined_externs)))
    # @StaticAddr: SUBALIGN(1) so the single input section starts EXACTLY at
    # the derived base (any padding would shift the static off its address).
    for sec, base in sorted(static_section_bases(obj_file,
                                                 static_addrs or {}).items()):
        rodata_line += (f"  {sec} 0x{base:08X} : SUBALIGN(1) "
                        f"{{ *({sec}) }}\n")
        # resolved by a checked directive -- no longer an unresolved symbol
        unresolved = [u for u in unresolved if u != sec]
    script = f"""OUTPUT_FORMAT("elf32-tradlittlemips")
OUTPUT_ARCH(mips)
SECTIONS {{
  /* Explicit address + SUBALIGN(4): with `. = base` ld aligns .text up to
     the input section's 16-byte alignment, silently shifting any function
     whose (func_addr - text_off) is not 16-aligned (e.g. two leading
     thunks -> text_off 0x10) and corrupting .text-relative relocs (jal to
     a same-TU static helper). Same fix as the .rodata placement line. */
  .text 0x{section_base:08X} : SUBALIGN(4) {{ *(.text*) }}
{rodata_line}{jt_def}{defs}
  /DISCARD/ : {{ *(.reginfo) *(.mdebug*) *(.comment) *(.pdr)
                *(.gnu.attributes) *(.MIPS.abiflags) }}
}}
"""
    ld_path = tmpdir / "resolve.ld"
    ld_path.write_text(script)
    elf_path = tmpdir / "resolved.elf"
    # --no-warn-mismatch tolerates ABI flag differences; -e0 silences entry warn.
    lr = subprocess.run(
        [LD, "-T", str(ld_path), "-o", str(elf_path), str(obj_file),
         "--no-warn-mismatch", "-e", "0"],
        capture_output=True, text=True)
    if lr.returncode != 0:
        return None
    bin_path = tmpdir / "resolved.bin"
    subprocess.run([OBJCOPY, "-O", "binary", "-j", ".text",
                    str(elf_path), str(bin_path)], capture_output=True)
    data = bin_path.read_bytes()
    # The function body begins at text_off within .text.
    window = data[text_off:text_off + func_size]
    return window, resolved, unresolved


def _sx16(x: int) -> int:
    return x - 0x10000 if (x & 0x8000) else x


def _hilo_addr(hi_word: int, lo_word: int) -> int:
    """Full 32-bit address from a MIPS HI16/LO16 pair."""
    return ((hi_word & 0xFFFF) << 16) + _sx16(lo_word & 0xFFFF)


def derive_rodata_base(overlay_name: str, func_name: str) -> Optional[int]:
    """Compute this function's own `.rodata` block base address (e.g. a switch
    jump table) ONCE from the target, for baking into an `@RodataBase`
    directive. Returns the base, or None if not a single-block .rodata case.

    Method: link with .rodata at 0 -> the HI16/LO16 against `.rodata` then
    encode the datum's OFFSET within our own object (our layout, honest). The
    target's HI16/LO16 encode the datum's real overlay ADDRESS. base =
    target_addr - our_offset. Self-validated by re-linking at base and
    requiring a FULL byte match (so multi-item .rodata that one base can't
    satisfy returns None).
    """
    funcs = parse_sym_file(overlay_name)
    if func_name not in funcs:
        return None
    func_addr, func_size = funcs[func_name]
    src_file = SRC_OVERLAY_DIR / overlay_name / f"{func_name}.c"
    if not src_file.exists():
        return None
    content = src_file.read_text(errors='ignore')
    # ANCHORED window parse: the directive counts only on a line that OPENS
    # with it, so header PROSE describing a proposed @SymbolStart/@SymbolSize
    # cannot silently re-window the authoritative compare.
    _sz = parse_symbol_size_text(content)
    if _sz is not None:
        func_size = _sz
    _st = parse_symbol_start_text(content)
    if _st is not None:
        func_addr = _st
    target = get_target_bytes(overlay_name, func_addr, func_size)

    with tempfile.TemporaryDirectory() as td:
        td = Path(td)
        obj = compile_object(overlay_name, func_name, src_file, func_addr, td)
        if obj is None:
            return None
        text_off = get_func_text_offset(obj, func_name)
        relocs = get_reloc_symbols(obj)
        his = [o for o, t, s in relocs if s == '.rodata' and 'HI16' in t]
        los = [o for o, t, s in relocs if s == '.rodata' and 'LO16' in t]
        if not his or not los:
            return None
        hi_off, lo_off = his[0] - text_off, los[0] - text_off
        if hi_off < 0 or lo_off < 0 or lo_off + 4 > func_size:
            return None
        sa = parse_static_addrs_text(content) or None
        linked = link_resolved(obj, func_addr, func_size, td, rodata_base=0,
                               func_name=func_name, static_addrs=sa,
                               overlay_name=overlay_name)
        if linked is None:
            return None
        ours = linked[0]

        def w(buf, off):
            return struct.unpack('<I', buf[off:off + 4])[0]

        our_off = _hilo_addr(w(ours, hi_off), w(ours, lo_off))
        tgt_addr = _hilo_addr(w(target, hi_off), w(target, lo_off))
        base = (tgt_addr - our_off) & 0xFFFFFFFF
        # validate: re-link at base and require a FULL match
        chk = link_resolved(obj, func_addr, func_size, td, rodata_base=base,
                            func_name=func_name, static_addrs=sa,
                            overlay_name=overlay_name)
        if chk is None:
            return None
        if chk[0] != target:
            return None
        return base


def get_target_bytes(overlay_name: str, func_addr: int, func_size: int) -> bytes:
    exn = ASSETS_DIR / f"{overlay_name}.EXN"
    off = func_addr - OVERLAY_BASE
    with open(exn, 'rb') as f:
        f.seek(off)
        return f.read(func_size)


def resolve_function(overlay_name: str, func_name: str,
                     verbose: bool = False,
                     extra_cflags: Optional[List[str]] = None,
                     src_override: Optional[Path] = None) -> Dict:
    """Compile, link with name-derived symbol addresses, compare STRICTLY.

    Returns dict: matched(bool), n_words, exact_after, unresolved(list of
    non-name-encoded symbols), diffs_after(list).

    ``extra_cflags`` screens an ad-hoc flag list through this authoritative
    path; ``src_override`` compiles a substitute copy of the source (screening
    harnesses use it so they never write to a tracked .c). Both default to the
    stock behaviour.
    """
    os.chdir(ROOT)
    extra_cflags = list(extra_cflags or [])
    funcs = parse_sym_file(overlay_name)
    if func_name not in funcs:
        return {'error': f"{func_name} not in {overlay_name}.sym"}
    func_addr, func_size = funcs[func_name]

    src_file = (Path(src_override) if src_override
                else SRC_OVERLAY_DIR / overlay_name / f"{func_name}.c")
    if not src_file.exists():
        return {'error': f"no source {src_file}"}
    content = src_file.read_text(errors='ignore')
    # ANCHORED window parse: the directive counts only on a line that OPENS
    # with it, so header PROSE describing a proposed @SymbolStart/@SymbolSize
    # cannot silently re-window the authoritative compare.
    _sz = parse_symbol_size_text(content)
    if _sz is not None:
        func_size = _sz
    _st = parse_symbol_start_text(content)
    if _st is not None:
        func_addr = _st
    # @RodataBase: a target-derived absolute address for this function's own
    # .rodata block (e.g. a switch jump table). Stored in the source like
    # @SymbolStart and resolved by VALUE here, so a wrong base fails the
    # strict compare (same honesty tier as a D_<addr>/func_<addr> name).
    # @StaticAddr: the same tier for a file-private static (see
    # static_section_bases); both come from link_options_from_source.
    try:
        link_opts = link_options_from_source(content)
    except StaticAddrError as exc:
        return {'error': f'@StaticAddr refused: {exc}'}
    rodata_base = link_opts['rodata_base']
    if "Status: ASM-ONLY" in content or "@AsmOnly" in content:
        return {'error': 'asm_only (no C)'}

    target = get_target_bytes(overlay_name, func_addr, func_size)

    with tempfile.TemporaryDirectory() as td:
        td = Path(td)
        obj = compile_object(overlay_name, func_name, src_file, func_addr, td,
                             extra_cflags=extra_cflags)
        if obj is None:
            return {'error': 'compile/postproc failed'}
        try:
            linked = link_resolved(obj, func_addr, func_size, td,
                                   rodata_base=rodata_base,
                                   func_name=func_name,
                                   static_addrs=link_opts['static_addrs'],
                                   overlay_name=overlay_name,
                                   literal_base=link_opts['literal_base'],
                                   data_base=link_opts['data_base'])
        except StaticAddrError as exc:
            return {'error': f'@StaticAddr refused: {exc}'}
        if linked is None:
            return {'error': 'link failed'}
        compiled, resolved, unresolved = linked

    n = min(len(target), len(compiled)) // 4
    exact_after = 0
    diffs_after = []
    for i in range(n):
        tw = struct.unpack('<I', target[i*4:i*4+4])[0]
        cw = struct.unpack('<I', compiled[i*4:i*4+4])[0]
        if tw == cw:
            exact_after += 1
        else:
            diffs_after.append((i, tw, cw))

    matched = (len(target) == len(compiled) and exact_after == n
               and len(diffs_after) == 0)

    result = {
        'matched': matched,
        'n_words': n,
        'exact_after': exact_after,
        'unresolved': unresolved,
        'resolved_count': len(resolved),
        'diffs_after': diffs_after,
        'len_target': len(target),
        'len_compiled': len(compiled),
    }

    if verbose:
        print(f"Function:  {func_name}  @ {hex(func_addr)}  size {func_size}")
        print(f"Resolved {len(resolved)} symbols by name; "
              f"{len(unresolved)} unresolved: {unresolved}")
        print(f"After name-derived link: {exact_after}/{n} words exact "
              f"(len t={len(target)} c={len(compiled)})")
        if matched:
            print("  *** BYTE-PERFECT (name-derived link, strict compare) ***")
        else:
            print(f"  {len(diffs_after)} residual diffs:")
            for i, tw, cw in diffs_after:
                kind = _diff_kind(tw, cw)
                print(f"    [{i:3d}] target={tw:08x} compiled={cw:08x}  {kind}")
    return result


def _diff_kind(tw: int, cw: int) -> str:
    ot, oc = (tw >> 26) & 0x3f, (cw >> 26) & 0x3f
    if ot != oc:
        return f"OPCODE DIFF ({ot}!={oc}) -- real codegen diff"
    if ot in (2, 3):
        return "jal/j target still wrong"
    if ot == 15:
        return "lui hi16 still wrong"
    if ot in (9, 13) or ot in range(32, 44):
        return "addiu/lw/sw lo16 still wrong"
    if ot in (4, 5):
        return "branch offset diff -- REAL (not a reloc)"
    return "reg/imm diff"


def scan_overlay(overlay_name: str, only_reloc_only: bool = True) -> Dict:
    """Scan reloc_only functions in an overlay; count name-derived byte_perfect."""
    status = json.load(open(STATUS_FILE)) if STATUS_FILE.exists() else {}
    ov_status = status.get(overlay_name, {})
    funcs = []
    for fn, info in ov_status.items():
        if not only_reloc_only or info.get('status') == 'reloc_only':
            funcs.append(fn)
    funcs.sort()

    n_bp = 0
    n_residual = 0
    n_err = 0
    n_unresolved = 0
    residual_examples = []
    unresolved_syms = set()
    provable = []   # funcs that strictly match after name-derived link
    for fn in funcs:
        r = resolve_function(overlay_name, fn, verbose=False)
        if 'error' in r:
            n_err += 1
            continue
        if r['unresolved']:
            n_unresolved += 1
            unresolved_syms.update(r['unresolved'])
        if r['matched']:
            n_bp += 1
            provable.append(fn)
        else:
            n_residual += 1
            if len(residual_examples) < 15:
                residual_examples.append((fn, len(r['diffs_after']),
                                          r['unresolved']))
    return {
        'overlay': overlay_name,
        'reloc_only_total': len(funcs),
        'byte_perfect': n_bp,
        'residual': n_residual,
        'errors': n_err,
        'with_unresolved': n_unresolved,
        'unresolved_syms': sorted(unresolved_syms),
        'residual_examples': residual_examples,
        'provable': provable,
    }


def gen_defsym(overlay_name: str, out_path: Optional[Path] = None) -> Dict:
    """Emit a GNU ld symbol-definition fragment for OVERLAY.

    Compiles every function in the overlay, collects every name-encoded
    relocation symbol (func_/D_/...), and writes `SYMBOL = 0xADDR;` lines. This
    is the artifact build_overlay.py is MISSING: it currently links with
    --noinhibit-exec, leaving all cross-references at 0 (which is exactly why
    these functions are stuck at reloc_only). INCLUDE-ing this fragment in the
    overlay linker script makes ld resolve the JAL / %hi / %lo relocations for
    real — turning the built EXN byte-identical to the shipped one at the
    relocation sites. Honest: every address comes from the symbol NAME, never
    from the target binary.
    """
    os.chdir(ROOT)
    funcs = parse_sym_file(overlay_name)
    src_dir = SRC_OVERLAY_DIR / overlay_name
    all_syms: Dict[str, int] = {}
    unresolved_syms = set()
    with tempfile.TemporaryDirectory() as td:
        td = Path(td)
        for fn, (fa, _sz) in sorted(funcs.items()):
            src = src_dir / f"{fn}.c"
            if not src.exists():
                continue
            content = src.read_text(errors='ignore')
            if "Status: ASM-ONLY" in content or "@AsmOnly" in content:
                continue
            obj = compile_object(overlay_name, fn, src, fa, td)
            if obj is None:
                continue
            for _off, _typ, sym in get_reloc_symbols(obj):
                addr = name_to_addr(sym, overlay_name)
                if addr is not None:
                    all_syms[sym] = addr
                elif not sym.startswith('.'):
                    unresolved_syms.add(sym)

    lines = [f"/* Symbol address definitions for {overlay_name} overlay. */",
             "/* Generated by resolve_overlay_relocs.py --gen-defsym.        */",
             "/* Every address is decoded from the symbol NAME (func_XXXX /  */",
             "/* D_XXXX = 0xXXXX), so ld resolves cross-overlay/main-binary  */",
             "/* relocations for real. INCLUDE this in the overlay linker.   */",
             ""]
    for sym, addr in sorted(all_syms.items()):
        lines.append(f"{sym} = 0x{addr:08X};")
    text = "\n".join(lines) + "\n"
    if out_path:
        out_path.write_text(text)
    return {
        'overlay': overlay_name,
        'defined': len(all_syms),
        'unresolved_local_data': sorted(unresolved_syms),
        'text': text,
    }



def self_test_static_addr() -> int:
    """Synthetic fixtures for @StaticAddr (parse refusals + placement).
    Returns the number of failures (0 = pass)."""
    fails = []

    def check(cond, what):
        print(f"  {'ok  ' if cond else 'FAIL'} {what}")
        if not cond:
            fails.append(what)

    # --- parse ------------------------------------------------------------
    check(parse_static_addrs_text(" * @StaticAddr: D_80150164 0x80150164\n")
          == {'D_80150164': 0x80150164}, "parse: anchored directive")
    check(parse_static_addrs_text(" * prose mentions @StaticAddr: D_80150164 "
                                  "0x80150164 here\n") == {},
          "parse: mid-line prose ignored")
    for bad, what in (("/* @StaticAddr: D_80150164 0x80150168 */\n",
                       "parse: name/address disagreement refused"),
                      ("/* @StaticAddr: foo 0x00001000 */\n",
                       "parse: non-RAM address refused"),
                      ("/* @StaticAddr: foo 0x80150000 */\n"
                       "/* @StaticAddr: foo 0x80150004 */\n",
                       "parse: duplicate name refused")):
        try:
            parse_static_addrs_text(bad)
            check(False, what)
        except StaticAddrError:
            check(True, what)

    # --- object placement -------------------------------------------------
    fx_one = ("static int D_80150164;\n"
              "int func_80150000(void) { D_80150164++; return D_80150164; }\n")
    fx_two = ("static int alpha; static int beta;\n"
              "int func_80150000(void) { alpha++; beta++; "
              "return alpha + beta; }\n")
    with tempfile.TemporaryDirectory() as td:
        td = Path(td)

        def build(src_text):
            src = td / "func_80150000.c"
            src.write_text(src_text)
            return compile_object("ETC", "func_80150000", src, 0x80150000, td)

        def hilo_targets(obj, sa):
            linked = link_resolved(obj, 0x80150000, 64, td,
                                   func_name="func_80150000", static_addrs=sa)
            if linked is None:
                return None, None
            win, _res, unres = linked
            words = [struct.unpack_from('<I', win, i)[0]
                     for i in range(0, len(win) - 3, 4)]
            addrs = set()
            for i, w in enumerate(words):
                if (w >> 26) == 0x0F:              # lui rX
                    rt = (w >> 16) & 31
                    for w2 in words[i + 1:i + 4]:
                        if (w2 >> 26) in (0x23, 0x2B) and ((w2 >> 21) & 31) == rt:
                            addrs.add(_hilo_addr(w, w2))
                            break
            return addrs, unres

        obj = build(fx_one)
        check(obj is not None, "fixture 1 compiles")
        if obj is not None:
            check(object_statics(obj).get('D_80150164', ('', -1))[0] == '.bss',
                  "fixture 1: static located in .mdebug as .bss")
            got, unres = hilo_targets(obj, {'D_80150164': 0x80150164})
            check(got == {0x80150164} and '.bss' not in (unres or []),
                  f"fixture 1: every %hi/%lo lands on 0x80150164 (got "
                  f"{sorted(hex(a) for a in (got or ()))})")
            got, unres = hilo_targets(obj, None)
            check(got != {0x80150164} and '.bss' in (unres or []),
                  "fixture 1: no directive -> .bss stays unresolved")
            try:
                hilo_targets(obj, {'D_80150168': 0x80150168})
                check(False, "fixture 1: directive for an absent static refused")
            except StaticAddrError:
                check(True, "fixture 1: directive for an absent static refused")

        obj = build(fx_two)
        check(obj is not None, "fixture 2 compiles")
        if obj is not None:
            st = object_statics(obj)
            a_off, b_off = st.get('alpha', ('', 0))[1], st.get('beta', ('', 0))[1]
            try:
                hilo_targets(obj, {'alpha': 0x80150000 + a_off})
                check(False, "fixture 2: unlisted static in a placed section refused")
            except StaticAddrError:
                check(True, "fixture 2: unlisted static in a placed section refused")
            got, _u = hilo_targets(obj, {'alpha': 0x80150100 + a_off,
                                         'beta': 0x80150100 + b_off})
            check(got == {0x80150100 + a_off, 0x80150100 + b_off},
                  "fixture 2: two consistent directives place both statics")
            try:
                hilo_targets(obj, {'alpha': 0x80150100 + a_off,
                                   'beta': 0x80150200 + b_off})
                check(False, "fixture 2: inconsistent section bases refused")
            except StaticAddrError:
                check(True, "fixture 2: inconsistent section bases refused")
    print(f"@StaticAddr self-test: {'PASS' if not fails else 'FAIL'} "
          f"({len(fails)} failure(s))")
    return len(fails)

def main():
    import argparse
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('overlay', nargs='?')
    ap.add_argument('func_name', nargs='?')
    ap.add_argument('-v', '--verbose', action='store_true')
    ap.add_argument('--scan', action='store_true',
                    help='Scan all reloc_only functions in OVERLAY')
    ap.add_argument('--scan-all', action='store_true',
                    help='Scan all reloc_only functions in every overlay')
    ap.add_argument('--report', metavar='OUT.json', default=None,
                    help='With --scan-all/--scan: write a JSON report listing the '
                         'provably-convertible (byte_perfect) functions per overlay')
    ap.add_argument('--extra-cflags', action='append', default=None,
                    metavar='FLAGS',
                    help='Extra compiler flags appended to the compile line, for '
                         'FLAG SCREENING (e.g. --extra-cflags -Wo,-rege,0). '
                         'Repeatable; one value may hold several flags separated '
                         'by WHITESPACE (commas are NOT separators -- IDO '
                         'pass-through flags embed them). An explicit -Wc,-picN '
                         'here overrides the derived PIC mode. Absent => the '
                         'command line is byte-identical to before.')
    ap.add_argument('--src', metavar='FILE', default=None,
                    help='Compile FILE instead of src/overlay/<OV>/<FUNC>.c '
                         '(screening harnesses pass a temp COPY).')
    ap.add_argument('--gen-defsym', metavar='OUT.ld', default=None,
                    help='Emit a GNU ld symbol-address fragment for OVERLAY '
                         '(defines every name-encoded reloc symbol) so the real '
                         'build can resolve relocations honestly')
    ap.add_argument('--self-test', action='store_true',
                    help='Run the @StaticAddr synthetic fixtures; exit 1 on '
                         'any failure')
    ap.add_argument('--check-name-table', action='store_true',
                    help='Validate the landed-name table (config/symbol_names.json) '
                         'and print its size; exit 1 on any inconsistency')
    args = ap.parse_args(glue_option_values(sys.argv[1:]))
    extra_cflags = parse_extra_cflags(args.extra_cflags)

    if args.self_test:
        sys.exit(1 if self_test_static_addr() else 0)

    if args.check_name_table:
        try:
            tab = load_name_table(force=True)
            ovt = {o: len(_ON.overlay_table(o)) for o in sorted(_ON.load_tables().ov)}
        except NameTableError as exc:
            print(f"NAME TABLE ERROR: {exc}"); sys.exit(1)
        print(f"name table OK: {len(tab)} landed non-address names "
              f"(from {SYMBOL_NAMES_FILE.relative_to(ROOT)})")
        print(f"overlay-scoped names OK: {sum(ovt.values())} "
              + (' '.join(f'{o}={n}' for o, n in ovt.items()) or '(none landed)'))
        return

    if args.gen_defsym is not None:
        if not args.overlay:
            print("--gen-defsym needs OVERLAY"); sys.exit(1)
        res = gen_defsym(args.overlay.upper(), Path(args.gen_defsym))
        print(f"[{res['overlay']}] defined {res['defined']} symbols -> "
              f"{args.gen_defsym}")
        if res['unresolved_local_data']:
            print(f"  {len(res['unresolved_local_data'])} unresolved local-data "
                  f"symbols (need overlay .rodata/.data base): "
                  f"{res['unresolved_local_data'][:6]}...")
        return

    if args.scan_all:
        status = json.load(open(STATUS_FILE))
        grand_bp = grand_total = grand_resid = grand_err = 0
        all_unresolved = set()
        report = {}
        for ov in sorted(status.keys()):
            res = scan_overlay(ov)
            if res['reloc_only_total'] == 0:
                continue
            print(f"{ov:12} reloc_only={res['reloc_only_total']:4} "
                  f"-> byte_perfect={res['byte_perfect']:4} "
                  f"residual={res['residual']:4} err={res['errors']:3} "
                  f"unresolved_fns={res['with_unresolved']:4}")
            grand_bp += res['byte_perfect']
            grand_total += res['reloc_only_total']
            grand_resid += res['residual']
            grand_err += res['errors']
            all_unresolved.update(res['unresolved_syms'])
            report[ov] = {
                'reloc_only_total': res['reloc_only_total'],
                'provable_byte_perfect': sorted(res['provable']),
                'residual': res['residual'],
                'errors': res['errors'],
            }
        print("-" * 70)
        print(f"TOTAL reloc_only={grand_total} -> byte_perfect={grand_bp} "
              f"residual={grand_resid} err={grand_err}")
        print(f"distinct non-name-encoded symbols: {len(all_unresolved)}")
        if args.report:
            report['_summary'] = {
                'reloc_only_total': grand_total,
                'provable_byte_perfect': grand_bp,
                'residual': grand_resid,
                'errors': grand_err,
            }
            with open(args.report, 'w') as f:
                json.dump(report, f, indent=2, sort_keys=True)
            print(f"Wrote report: {args.report}")
        return

    if args.scan:
        if not args.overlay:
            print("--scan needs OVERLAY"); sys.exit(1)
        res = scan_overlay(args.overlay.upper())
        print(json.dumps({k: v for k, v in res.items()
                          if k != 'residual_examples'}, indent=2))
        print("\nResidual examples (func, n_diffs, unresolved):")
        for ex in res['residual_examples']:
            print(f"  {ex}")
        return

    if not args.overlay or not args.func_name:
        ap.print_help(); sys.exit(1)
    r = resolve_function(args.overlay.upper(), args.func_name, verbose=True,
                         extra_cflags=extra_cflags, src_override=args.src)
    if 'error' in r:
        print("Error:", r['error']); sys.exit(1)
    sys.exit(0 if r['matched'] else 1)


if __name__ == '__main__':
    main()
