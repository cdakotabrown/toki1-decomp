#!/usr/bin/env python3
"""build_clean_exn.py - Whole-EXN clean build from SOURCE.

Builds a byte-identical overlay EXN where every CODE byte comes from source:

  * byte_perfect / reloc_only functions -> compile their C with the existing
    per-function pipeline (IDO/GCC + PostProc), link at the true VA with every
    name-encoded symbol defined at its name address (resolve_overlay_relocs
    machinery), strip the 16-byte types.h thunk prefix via the .text window.
  * semi_matched / count_mismatch / asm_only functions (and any C that fails
    the strict per-function byte compare) -> ASSEMBLE the committed
    disassembly `asm/overlay/<OV>/<fn>.s` at its VA (the as-path). The .s is
    translated mechanically (absolute branch targets -> PC-relative
    expressions, raw div/divu -> $zero 3-op form) and run through
    mipsel-linux-gnu-as + ld. NO code bytes are ever copied from the EXN.
  * inter-function gaps -> modeled explicitly. Original gap bytes are READ
    ONLY to verify they are padding (all-zero); verified padding is emitted
    as zero fill. A gap containing real code must have its own orphan .s
    (asm/overlay/<OV>/func_<ADDR>.s); otherwise it is recorded as
    UNRESOLVED debt and zero-filled so the gate fails precisely there.
  * .data/.rodata region (everything after the last code item, incl. all
    text/strings/jr-tables) -> spliced from the gitignored extraction
    assets/overlays/<OV>.EXN (the one allowed use of original bytes).

The strict gate (--verify) SHA1-compares the built file against the extracted
original and prints a first-divergence hexdump with the owning region.

Debt accounting: config/clean_build_debt.json is regenerated per build with
per-overlay counts + function lists of {from_c, from_asm, c_fallback_to_asm,
unresolved, data_bytes_from_extraction, gap_bytes}.  That file is TRACKED and
holds only what is a function of the source and the disc, so a fresh build
leaves the checkout clean; the per-run facts (cache hits/misses, seconds, the
date of the run) go to build/clean_build_debt.runtime.json (RUNTIME_FILE).

Usage:
    python3 tools/overlay/build_clean_exn.py RENSYU --verify
    python3 tools/overlay/build_clean_exn.py OLH --verify -v
    python3 tools/overlay/build_clean_exn.py --all --verify

Output: build/overlay/<OV>/<OV>.clean.EXN
"""
import concurrent.futures
import hashlib
import json
import os
import re
import struct
import subprocess
import sys
import tempfile
import time
from pathlib import Path
from typing import Dict, List, Optional, Tuple

ROOT = Path(__file__).parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "overlay"))
sys.path.insert(0, str(ROOT / "tools" / "build"))

from parse_postproc import (  # noqa: E402
    parse_symbol_start_text, parse_symbol_size_text,
)

from resolve_overlay_relocs import (   # noqa: E402
    parse_sym_file, compile_object, link_resolved, get_target_bytes,
    parse_static_addrs_text, StaticAddrError, link_options_from_source,
)
from build_overlay import add_exn_checksum  # noqa: E402
import overlay_names as ON  # noqa: E402

CONFIG_DIR = ROOT / "config" / "overlay"
SRC_OVERLAY_DIR = ROOT / "src" / "overlay"
ASM_OVERLAY_DIR = ROOT / "asm" / "overlay"
ASSETS_DIR = ROOT / "assets" / "overlays"
BUILD_DIR = ROOT / "build" / "overlay"
STATUS_FILE = ROOT / "config" / "overlay_match_status.json"
DEBT_FILE = ROOT / "config" / "clean_build_debt.json"
# Per-run, untracked: cache statistics and the run date (update_debt_file).
RUNTIME_FILE = ROOT / "build" / "clean_build_debt.runtime.json"
# Keys of an overlay's debt entry that change from run to run with the same
# source; they never reach the tracked DEBT_FILE.
RUNTIME_KEYS = ("cache", "generated")

AS = "mipsel-linux-gnu-as"
LD = "mipsel-linux-gnu-ld"
OBJCOPY = "mipsel-linux-gnu-objcopy"

CACHE_DIR = ROOT / "build" / "clean_exn_cache"
# Bump when the production semantics change (translator rules, link script,
# window logic, ...). Compiler identity is mixed into each key separately.
CACHE_VERSION = "4"

OVERLAY_BASE = 0x80132000
EXN_SIZE = 192 * 1024          # incl. 4-byte checksum trailer
PAYLOAD_SIZE = EXN_SIZE - 4

# Statuses whose C is allowed into the clean build (per spec: the resolver
# must have said byte_perfect; reloc_only C is TRIED and falls back to .s
# with a debt record when the strict compare fails, e.g. rodata-blocked).
C_ELIGIBLE_STATUSES = {"byte_perfect", "reloc_only"}

BRANCH_MNEMONICS = {
    "b", "bal", "beq", "bne", "blez", "bgtz", "bltz", "bgez",
    "bltzal", "bgezal", "beql", "bnel", "blezl", "bgtzl",
    "bltzl", "bgezl", "bltzall", "bgezall",
}


# ---------------------------------------------------------------------------
# .s translation (mechanical; operates ONLY on the committed .s text)
# ---------------------------------------------------------------------------

def _strip_block_comments(text: str) -> str:
    """Remove /* ... */ comments (the disassembly headers), preserving lines."""
    out = []
    in_comment = False
    for line in text.splitlines():
        buf = []
        i = 0
        while i < len(line):
            if in_comment:
                j = line.find("*/", i)
                if j < 0:
                    i = len(line)
                else:
                    in_comment = False
                    i = j + 2
            else:
                j = line.find("/*", i)
                if j < 0:
                    buf.append(line[i:])
                    i = len(line)
                else:
                    buf.append(line[i:j])
                    in_comment = True
                    i = j + 2
        out.append("".join(buf))
    return "\n".join(out)


def parse_s_header(s_path: Path) -> Tuple[Optional[int], Optional[int]]:
    """Return (address, size_bytes) from the disassembly header comment."""
    text = s_path.read_text(errors="ignore")[:2000]
    addr = size = None
    m = re.search(r"Address:\s*(0x[0-9A-Fa-f]+)", text)
    if m:
        addr = int(m.group(1), 16)
    m = re.search(r"Size:\s*(\d+)\s*bytes", text)
    if m:
        size = int(m.group(1))
    return addr, size


def translate_asm(s_path: Path, func_addr: int) -> Tuple[str, int]:
    """Translate committed disassembly so GNU as can assemble it at func_addr.

    Transformations (all derived from the .s TEXT, never from binaries):
      - absolute branch targets `0xADDR` -> `. + (target - insn_addr)` so gas
        resolves the PC-relative offset at assembly time regardless of where
        the section nominally sits;
      - `div rs, rt` / `divu rs, rt` -> `div $zero, rs, rt` (raw machine form;
        the 2-op form is a gas macro that expands to break-checked sequences);
      - `break 0xN` / `syscall 0xN` -> `.word` with the code field re-encoded
        from the .s operand (gas places the code field differently);
      - `$cN_M` coprocessor register spelling -> `$M` for lwc2/swc2.

    Returns (translated_text, body_size_bytes). body_size counts the 4-byte
    emissions (instructions + .word) so callers can strip gas's 16-byte
    .text section alignment padding from the objcopy output.
    """
    text = _strip_block_comments(s_path.read_text(errors="ignore"))
    out = []
    addr = func_addr
    for raw in text.splitlines():
        line = raw.rstrip()
        code = line.split("#", 1)[0].strip()
        if not code:
            out.append(line)
            continue
        if code.endswith(":"):
            out.append(line)
            continue
        if code.startswith("."):
            if code.startswith(".word"):
                addr += 4
            out.append(line)
            continue

        # instruction line
        m = re.match(r"^(\w+)\s*(.*)$", code)
        if not m:
            out.append(line)
            addr += 4
            continue
        mnem, ops = m.group(1), m.group(2)

        if mnem in BRANCH_MNEMONICS:
            bm = re.match(r"^(.*?)(0x[0-9A-Fa-f]+)\s*$", ops)
            if bm:
                target = int(bm.group(2), 16)
                delta = target - addr
                # gas refuses `bltzal/bgezal $ra, ...` (rs=$31 clobbers the
                # link register) — data words in mixed chunks can decode to
                # this. Re-encode the REGIMM word from the parsed fields.
                link_rt = {"bltzal": 0x10, "bgezal": 0x11,
                           "bltzall": 0x12, "bgezall": 0x13}
                if mnem in link_rt and re.match(
                        r"^\$(ra|31)\s*,\s*$", bm.group(1)):
                    off16 = ((target - (addr + 4)) >> 2) & 0xFFFF
                    word = (1 << 26) | (31 << 21) | (link_rt[mnem] << 16) \
                        | off16
                    line = (f"    .word   0x{word:08X}  "
                            f"# {mnem} $ra, 0x{target:08X}")
                else:
                    expr = f". + {delta}" if delta >= 0 else f". - {-delta}"
                    line = f"    {mnem:<7s} {bm.group(1)}{expr}"
        elif mnem in ("div", "divu"):
            regs = [r.strip() for r in ops.split(",")]
            if len(regs) == 2:
                line = f"    {mnem:<7s} $zero, {regs[0]}, {regs[1]}"
        elif mnem in ("break", "syscall"):
            cm = re.match(r"^0x([0-9A-Fa-f]+)$", ops.strip())
            if cm:
                codefield = int(cm.group(1), 16) & 0xFFFFF
                funct = 0x0D if mnem == "break" else 0x0C
                word = (codefield << 6) | funct
                line = f"    .word   0x{word:08X}  # {mnem} 0x{codefield:X}"
        if "$c" in line:
            line = re.sub(r"\$c\d_(\d+)", r"$\1", line)

        out.append(line)
        addr += 4
    return "\n".join(out) + "\n", addr - func_addr


def assemble_at_va(s_path: Path, func_addr: int, tmpdir: Path,
                   verbose: bool = False) -> Optional[bytes]:
    """Assemble a committed .s at its VA; return the BODY bytes (gas's
    16-byte .text alignment padding stripped) or None."""
    translated, body_size = translate_asm(s_path, func_addr)
    ts = tmpdir / (s_path.stem + ".trans.s")
    ts.write_text(translated)
    obj = tmpdir / (s_path.stem + ".as.o")
    r = subprocess.run([AS, "-march=mips1", "-mabi=32", "-EL",
                        "-o", str(obj), str(ts)],
                       capture_output=True, text=True)
    if r.returncode != 0:
        if verbose:
            print(f"    [as FAIL] {s_path.name}: {r.stderr.strip()[:300]}")
        return None
    script = tmpdir / (s_path.stem + ".ld")
    script.write_text(
        'OUTPUT_FORMAT("elf32-tradlittlemips")\nOUTPUT_ARCH(mips)\n'
        "SECTIONS {\n"
        f"  .text 0x{func_addr:08X} : SUBALIGN(4) {{ *(.text*) }}\n"
        "  /DISCARD/ : { *(.reginfo) *(.mdebug*) *(.comment) *(.pdr)\n"
        "                *(.gnu.attributes) *(.MIPS.abiflags) }\n"
        "}\n")
    elf = tmpdir / (s_path.stem + ".as.elf")
    r = subprocess.run([LD, "-T", str(script), "-e", "0",
                        "--no-warn-mismatch", "-o", str(elf), str(obj)],
                       capture_output=True, text=True)
    if r.returncode != 0:
        if verbose:
            print(f"    [ld FAIL] {s_path.name}: {r.stderr.strip()[:300]}")
        return None
    binp = tmpdir / (s_path.stem + ".as.bin")
    subprocess.run([OBJCOPY, "-O", "binary", "-j", ".text",
                    str(elf), str(binp)], capture_output=True)
    try:
        data = binp.read_bytes()
    except OSError:
        return None
    if len(data) < body_size:
        if verbose:
            print(f"    [as SHORT] {s_path.name}: {len(data)} < body "
                  f"{body_size}")
        return None
    if any(data[body_size:]):
        if verbose:
            print(f"    [as PAD?] {s_path.name}: non-zero bytes beyond "
                  f"body size {body_size}")
        return None
    return data[:body_size]


# ---------------------------------------------------------------------------
# Layout
# ---------------------------------------------------------------------------

class CodeItem:
    def __init__(self, name: str, addr: int, size: int, in_sym: bool):
        self.name = name
        self.addr = addr
        self.size = size          # effective (clipped) size in the layout
        self.in_sym = in_sym      # False -> orphan fragment (as-path only)
        self.source = None        # 'c' | 'asm' | 'unresolved'
        self.note = ""

    @property
    def end(self) -> int:
        return self.addr + self.size


def parse_c_annotations(c_path: Path) -> Dict[str, int]:
    """@SymbolStart/@SymbolSize/@RodataBase overrides + ASM-ONLY marker."""
    out: Dict[str, int] = {}
    try:
        content = c_path.read_text(errors="ignore")
    except OSError:
        return out
    # ANCHORED: a directive only counts on a line that OPENS with it, so a
    # header that merely discusses @SymbolStart/@SymbolSize in prose cannot
    # re-window the function (see parse_postproc.directive_bodies).
    v = parse_symbol_start_text(content)
    if v is not None:
        out["start"] = v
    v = parse_symbol_size_text(content)
    if v is not None:
        out["size"] = v
    m = re.search(r"@RodataBase:\s*(0x[0-9A-Fa-f]+)", content)
    if m:
        out["rodata_base"] = int(m.group(1), 16)
    # @StaticAddr (a file-private static's retail address; see
    # resolve_overlay_relocs.parse_static_addrs_text). A bad directive is a
    # hard error there; here it simply leaves the static unplaced.
    try:
        sa = parse_static_addrs_text(content)
    except StaticAddrError:
        sa = {}
    if sa:
        out["static_addrs"] = sa
    if "Status: ASM-ONLY" in content or "@AsmOnly" in content:
        out["asm_only"] = 1
    return out


def build_layout(overlay: str) -> List[CodeItem]:
    """Merge sym-map functions with orphan .s fragments; clip overlaps."""
    funcs = parse_sym_file(overlay)
    items: List[CodeItem] = []
    for name, (addr, size) in funcs.items():
        # The window is EXACTLY the one the authoritative per-function gates
        # use (resolve_overlay_relocs.resolve_function, audit_overlay_coverage):
        # the .sym address/size, overridden by an anchored @SymbolStart /
        # @SymbolSize in the source. The .sym address is NOT the name address
        # for 2,381 entries -- a window widened BACKWARDS over an absorbed
        # cross-prologue head (the FRONT class). An older rule here "trusted
        # the name" instead, which placed every such C window 4-256 bytes too
        # late: the C (which emits the head itself) then failed the strict
        # compare and the build silently fell back to the .s (2026-10-04).
        c_path = SRC_OVERLAY_DIR / overlay / f"{name}.c"
        if c_path.exists():
            ann = parse_c_annotations(c_path)
            addr = ann.get("start", addr)
            size = ann.get("size", size)
        items.append(CodeItem(name, addr, size, in_sym=True))

    sym_windows = [(it.addr, it.end) for it in items]

    def _inside_sym_window(a: int, n: int) -> bool:
        return any(lo <= a and a + n <= hi for lo, hi in sym_windows)

    asm_dir = ASM_OVERLAY_DIR / overlay
    if asm_dir.exists():
        for s_path in sorted(asm_dir.glob("*.s")):     # renamed functions: <name>.s
            name = s_path.stem
            if name in funcs:
                continue
            addr, size = parse_s_header(s_path)
            if addr is None:
                addr = ON.func_addr(overlay, name)
            if addr is None or size is None:
                print(f"  [WARN] orphan {s_path.name}: no Address/Size header")
                continue
            # A .s that is not a .sym symbol and lies wholly inside a .sym
            # window is a stale disassembly of bytes that window already owns
            # (an absorbed head, a merged over-split case body, a gap
            # fragment generated under the old name-address rule). It must
            # not become a layout item: as one it CLIPPED the owning window,
            # so the C was compared over a prefix only and the .s supplied
            # the rest. It stays in asm_fragment_index for as-path
            # composition of non-C windows.
            if _inside_sym_window(addr, size):
                continue
            items.append(CodeItem(name, addr, size, in_sym=False))

    items.sort(key=lambda it: (it.addr, -it.size))
    # Dedupe same-address items: a @SymbolStart-shifted C window (boundary
    # recovery) can land exactly on an orphan-fragment .s; keep the larger
    # extent (the recovered window subsumes the partial disassembly).
    deduped: List[CodeItem] = []
    for it in items:
        if deduped and deduped[-1].addr == it.addr:
            continue
        deduped.append(it)
    items = deduped
    # Clip each item at the next item's start (sym sizes sometimes overlap the
    # next symbol; the smaller of declared vs available span is the honest
    # body extent — the remainder is modeled as a gap or the next function).
    for i, it in enumerate(items):
        if i + 1 < len(items):
            avail = items[i + 1].addr - it.addr
            if it.size > avail:
                it.note = (f"window clipped {it.size} -> {avail} at "
                           f"{items[i + 1].name}")
                it.size = avail
    return items


# ---------------------------------------------------------------------------
# Per-function byte production
# ---------------------------------------------------------------------------

def produce_c_bytes(overlay: str, item: CodeItem, tmpdir: Path,
                    verbose: bool = False) -> Optional[bytes]:
    """Compile+postproc+link one function's C at its VA; return the .text
    window (thunk prefix excluded by link_resolved's windowing)."""
    src = SRC_OVERLAY_DIR / overlay / f"{item.name}.c"
    if not src.exists():
        return None
    ann = parse_c_annotations(src)
    if ann.get("asm_only"):
        return None
    obj = compile_object(overlay, item.name, src, item.addr, tmpdir)
    if obj is None:
        if verbose:
            print(f"    [cc FAIL] {item.name}")
        return None
    try:
        # Same link call as the authoritative resolve_function: the link
        # options come from link_options_from_source and func_name selects
        # the function's own symbol inside a multi-function TU (without it
        # the window starts at .text+0 -- e.g. a gcc TU whose first function
        # is a helper, TAIIKU func_80148AA0).
        opts = link_options_from_source(src.read_text(errors="ignore"))
        linked = link_resolved(obj, item.addr, item.size, tmpdir,
                               rodata_base=opts["rodata_base"],
                               func_name=item.name,
                               static_addrs=opts["static_addrs"],
                               overlay_name=overlay,
                               literal_base=opts["literal_base"],
                               data_base=opts["data_base"])
    except StaticAddrError as exc:
        if verbose:
            print(f"    [@StaticAddr refused] {item.name}: {exc}")
        linked = None
    if linked is None:
        if verbose:
            print(f"    [link FAIL] {item.name}")
        return None
    return linked[0]


_FRAG_INDEX: Dict[str, Dict[int, Path]] = {}


def asm_fragment_index(overlay: str) -> Dict[int, Path]:
    """Map disassembly start address -> .s path for every committed .s in the
    overlay (header `Address:` wins; the name-encoded address is the
    fallback). Memoized per overlay."""
    if overlay in _FRAG_INDEX:
        return _FRAG_INDEX[overlay]
    index: Dict[int, Path] = {}
    asm_dir = ASM_OVERLAY_DIR / overlay
    if asm_dir.exists():
        for s_path in sorted(asm_dir.glob("*.s")):     # renamed functions: <name>.s
            addr, _size = parse_s_header(s_path)
            if addr is None:
                addr = ON.func_addr(overlay, s_path.stem)
            if addr is not None:
                index[addr] = s_path
    _FRAG_INDEX[overlay] = index
    return index


def produce_asm_bytes(overlay: str, item: CodeItem, tmpdir: Path,
                      verbose: bool = False) -> Optional[bytes]:
    """Compose the item's window from committed .s fragments.

    A layout slot is not always one .s: an @SymbolStart-shifted window can
    span a leading stub fragment plus the main body (e.g. TT
    `func_80134D14` at 0x80134D0C = func_80134D0C.s (8B) + func_80134D14.s).
    Walk the window assembling every fragment at its own address; verified
    all-zero original spans between/after fragments become zero fill."""
    index = asm_fragment_index(overlay)
    out = bytearray()
    cur = item.addr
    while cur < item.end:
        s_path = index.get(cur)
        if s_path is None:
            # No fragment starts here. If the rest of the window (up to the
            # next known fragment) is original padding, zero-fill it.
            nxt = min((a for a in index if cur < a < item.end),
                      default=item.end)
            span = get_target_bytes(overlay, cur, nxt - cur)
            if any(span):
                if verbose:
                    print(f"    [no .s] {item.name}: window hole at "
                          f"0x{cur:08X}+{nxt - cur} is not padding")
                return None
            out.extend(bytes(nxt - cur))
            cur = nxt
            continue
        data = assemble_at_va(s_path, cur, tmpdir, verbose=verbose)
        if data is None:
            return None
        room = item.end - cur
        if len(data) > room:
            # The disassembly window overshoots into the NEXT symbol (the
            # sym map has overlapping entries). The next item produces its
            # own bytes from its own source, so truncating drops duplicates
            # — it never loses unique code.
            if verbose:
                print(f"    [note] {item.name}: fragment {s_path.stem} "
                      f"body {len(data)} truncated to {room}")
            data = data[:room]
        out.extend(data)
        cur += len(data)
    return bytes(out)


# ---------------------------------------------------------------------------
# Object/bytes cache
# ---------------------------------------------------------------------------

def _hash_file(h, path: Path):
    try:
        h.update(path.read_bytes())
    except OSError:
        h.update(b"<missing>")


_COMPILER_ID = None


def _compiler_id() -> str:
    """Cheap toolchain-identity string mixed into every cache key."""
    global _COMPILER_ID
    if _COMPILER_ID is None:
        parts = [CACHE_VERSION]
        try:
            st = (ROOT / "tools" / "ido-static-recomp" / "build" / "5.2" /
                  "out" / "cc").stat()
            parts.append(f"cc:{st.st_size}:{int(st.st_mtime)}")
        except OSError:
            parts.append("cc:none")
        for tool in (AS, LD):
            try:
                r = subprocess.run([tool, "--version"], capture_output=True,
                                   text=True, timeout=10)
                parts.append(r.stdout.splitlines()[0] if r.stdout else tool)
            except Exception:
                parts.append(f"{tool}:unknown")
        _COMPILER_ID = "|".join(parts)
    return _COMPILER_ID


def item_cache_key(overlay: str, item: CodeItem, status: str) -> str:
    """Key over every input that can change the produced bytes: the C source
    (incl. its @PostProc/@CompileEnv/@CompileFlags header), the .s (both the
    as-path input and scheduler_normalizer's target), any custom PostProc
    scripts the C references, layout placement, status routing, the original
    function window (drives fallback/divergence decisions) and toolchain id."""
    h = hashlib.sha1()
    h.update(_compiler_id().encode())
    h.update(f"{overlay}/{item.name}@{item.addr:08X}+{item.size}:{status}"
             .encode())
    c_path = SRC_OVERLAY_DIR / overlay / f"{item.name}.c"
    _hash_file(h, c_path)
    # every .s fragment that can contribute to this window (composition),
    # plus the item's own-name .s (scheduler_normalizer target)
    index = asm_fragment_index(overlay)
    for addr in sorted(a for a in index if item.addr <= a < item.end):
        _hash_file(h, index[addr])
    _hash_file(h, ASM_OVERLAY_DIR / overlay / f"{item.name}.s")
    # custom PostProc scripts referenced from the C header
    try:
        content = c_path.read_text(errors="ignore")[:20000]
        for m in re.finditer(r"@PostProc:.*?(\S+\.py)", content):
            script = (ROOT / m.group(1)) if not m.group(1).startswith("/") \
                else Path(m.group(1))
            _hash_file(h, script)
    except OSError:
        pass
    try:
        orig = get_target_bytes(overlay, item.addr, item.size)
        h.update(orig)
    except OSError:
        pass
    return h.hexdigest()


def cache_load(overlay: str, item: CodeItem, key: str) -> Optional[Dict]:
    p = CACHE_DIR / overlay / f"{item.name}.json"
    if not p.exists():
        return None
    try:
        entry = json.loads(p.read_text())
    except (json.JSONDecodeError, OSError):
        return None
    if entry.get("key") != key:
        return None
    if entry.get("bytes_hex") is not None:
        entry["bytes"] = bytes.fromhex(entry["bytes_hex"])
    else:
        entry["bytes"] = None
    return entry


def cache_store(overlay: str, item: CodeItem, key: str, result: Dict):
    d = CACHE_DIR / overlay
    d.mkdir(parents=True, exist_ok=True)
    entry = {"key": key,
             "source": result.get("source"),
             "fallback": result.get("fallback", False),
             "divergent": result.get("divergent", False),
             "note": result.get("note", ""),
             "bytes_hex": result["bytes"].hex()
             if result.get("bytes") is not None else None}
    tmp = d / f".{item.name}.json.tmp"
    tmp.write_text(json.dumps(entry))
    tmp.replace(d / f"{item.name}.json")


# ---------------------------------------------------------------------------
# Per-item production (cacheable unit; runs in a worker thread)
# ---------------------------------------------------------------------------

def produce_item(overlay: str, item: CodeItem, status: str,
                 original: bytes, verbose: bool = False) -> Dict:
    """Produce one item's bytes. Returns {bytes|None, source, fallback,
    divergent, note}. Never copies code bytes from the EXN; the original is
    used only for the strict per-function verification compare."""
    orig_fn = original[item.addr - OVERLAY_BASE:item.end - OVERLAY_BASE]
    result = {"bytes": None, "source": None, "fallback": False,
              "divergent": False, "note": ""}
    with tempfile.TemporaryDirectory() as td:
        tmpdir = Path(td)
        if status in C_ELIGIBLE_STATUSES:
            produced = produce_c_bytes(overlay, item, tmpdir, verbose=verbose)
            if produced is not None and len(produced) == item.size \
                    and produced == orig_fn:
                result.update(bytes=produced, source="c")
                return result
            asm_bytes = produce_asm_bytes(overlay, item, tmpdir,
                                          verbose=verbose)
            if asm_bytes is not None:
                result.update(bytes=asm_bytes, source="asm", fallback=True,
                              divergent=(asm_bytes != orig_fn),
                              note=f"C({status}) failed strict compare")
                return result
            result["note"] = f"C({status}) mismatched and no .s fallback"
            return result
        asm_bytes = produce_asm_bytes(overlay, item, tmpdir, verbose=verbose)
        if asm_bytes is not None:
            result.update(bytes=asm_bytes, source="asm",
                          divergent=(asm_bytes != orig_fn))
        else:
            result["note"] = "no .s produced bytes"
        return result


def produce_all(overlay: str, items: List[CodeItem], ov_status: Dict,
                original: bytes, jobs: int, use_cache: bool,
                verbose: bool = False) -> Tuple[Dict[str, Dict], Dict]:
    """Produce every item's bytes (cache-first, then parallel workers).
    Returns ({item.name: result}, cache_stats)."""
    results: Dict[str, Dict] = {}
    todo: List[Tuple[CodeItem, str, str]] = []
    stats = {"hits": 0, "misses": 0, "seconds": 0.0}
    t0 = time.monotonic()
    for item in items:
        st = ov_status.get(item.name, {}).get("status", "no_status")
        key = item_cache_key(overlay, item, st)
        if use_cache:
            hit = cache_load(overlay, item, key)
            if hit is not None:
                results[item.name] = hit
                stats["hits"] += 1
                continue
        todo.append((item, st, key))
    stats["misses"] = len(todo)

    if todo:
        def work(entry):
            item, st, key = entry
            res = produce_item(overlay, item, st, original, verbose=verbose)
            if use_cache:
                cache_store(overlay, item, key, res)
            return item.name, res

        with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as ex:
            for name, res in ex.map(work, todo):
                results[name] = res
    stats["seconds"] = round(time.monotonic() - t0, 1)
    return results, stats


# ---------------------------------------------------------------------------
# Whole-EXN build
# ---------------------------------------------------------------------------

def build_clean_exn(overlay: str, verbose: bool = False, jobs: int = 4,
                    use_cache: bool = True,
                    data_from_source: bool = False) -> Dict:
    exn_path = ASSETS_DIR / f"{overlay}.EXN"
    if not exn_path.exists():
        return {"error": f"missing extraction {exn_path}"}
    original = exn_path.read_bytes()
    if len(original) != EXN_SIZE:
        return {"error": f"unexpected EXN size {len(original)}"}

    status_all = json.load(open(STATUS_FILE)) if STATUS_FILE.exists() else {}
    ov_status = status_all.get(overlay, {})

    items = build_layout(overlay)
    if not items:
        return {"error": "empty layout"}

    payload = bytearray(PAYLOAD_SIZE)
    debt = {
        "from_c": [], "from_asm": [], "c_fallback_to_asm": [],
        "clipped": [it.name + ": " + it.note for it in items if it.note],
        "unresolved": [], "gaps": [],
        "counts": {},
    }
    text_bytes_from_c = 0
    text_bytes_from_asm = 0
    gap_bytes = 0
    divergent_fns = []

    # Phase 1: produce every item's bytes (cache-first, parallel workers).
    results, cache_stats = produce_all(overlay, items, ov_status, original,
                                       jobs=jobs, use_cache=use_cache,
                                       verbose=verbose)

    # Phase 2: serial placement + gap modeling.
    cursor = OVERLAY_BASE
    for item in items:
        # ---- gap before this item ----
        if item.addr > cursor:
            gsz = item.addr - cursor
            orig_gap = original[cursor - OVERLAY_BASE:
                                item.addr - OVERLAY_BASE]
            if any(orig_gap):
                # A code-bearing gap: compose it from committed .s fragments
                # exactly like an as-path item (covers dedupe-dropped shifted
                # sym windows whose .s still exists).
                pseudo = CodeItem(f"gap_{cursor:08X}", cursor, gsz,
                                  in_sym=False)
                gap_bytes_produced = None
                with tempfile.TemporaryDirectory() as gtd:
                    gap_bytes_produced = produce_asm_bytes(
                        overlay, pseudo, Path(gtd), verbose=verbose)
                if gap_bytes_produced is not None \
                        and len(gap_bytes_produced) == gsz:
                    payload[cursor - OVERLAY_BASE:
                            item.addr - OVERLAY_BASE] = gap_bytes_produced
                    debt["from_asm"].append(pseudo.name)
                    text_bytes_from_asm += gsz
                    debt["gaps"].append({"addr": f"0x{cursor:08X}",
                                         "size": gsz,
                                         "kind": "asm_fragments"})
                    if gap_bytes_produced != bytes(orig_gap):
                        divergent_fns.append(pseudo.name)
                else:
                    debt["unresolved"].append({
                        "kind": "gap_with_code", "addr": f"0x{cursor:08X}",
                        "size": gsz,
                        "hint": f"needs orphan asm/overlay/{overlay}/"
                                f"func_{cursor:08X}.s"})
                    debt["gaps"].append({"addr": f"0x{cursor:08X}",
                                         "size": gsz, "kind": "UNRESOLVED"})
                    if verbose:
                        print(f"  [GAP-CODE] 0x{cursor:08X}+{gsz}: original "
                              f"gap is NOT padding — needs a .s")
            else:
                debt["gaps"].append({"addr": f"0x{cursor:08X}",
                                     "size": gsz, "kind": "zero_fill"})
                gap_bytes += gsz
            # payload already zero-initialized: nothing to write
        elif item.addr < cursor:
            debt["unresolved"].append({
                "kind": "overlap", "addr": f"0x{item.addr:08X}",
                "name": item.name,
                "note": f"overlaps previous item end 0x{cursor:08X}"})
            if verbose:
                print(f"  [OVERLAP] {item.name} @0x{item.addr:08X} < "
                      f"cursor 0x{cursor:08X}")
            cursor = max(cursor, item.end)
            continue

        # ---- the item itself ----
        st = ov_status.get(item.name, {}).get("status", "no_status")
        res = results.get(item.name, {"bytes": None, "note": "no result"})
        produced = res.get("bytes")
        source = res.get("source")

        if produced is None or len(produced) != item.size:
            debt["unresolved"].append({
                "kind": "no_bytes", "name": item.name,
                "addr": f"0x{item.addr:08X}", "size": item.size,
                "status": st, "note": res.get("note", "")})
            if verbose:
                print(f"  [UNRESOLVED] {item.name} ({st}): "
                      f"{res.get('note', 'no source produced bytes')}; "
                      f"zero-filled")
            cursor = item.end
            continue

        payload[item.addr - OVERLAY_BASE:item.end - OVERLAY_BASE] = produced
        item.source = source
        if res.get("fallback"):
            debt["c_fallback_to_asm"].append(
                {"name": item.name, "status": st})
        if source == "c":
            debt["from_c"].append(item.name)
            text_bytes_from_c += item.size
        else:
            debt["from_asm"].append(item.name)
            text_bytes_from_asm += item.size
            if res.get("divergent"):
                divergent_fns.append(item.name)
                if verbose:
                    print(f"  [ASM-DIVERGE] {item.name}: assembled bytes "
                          f"differ from original (assembled honestly)")
        cursor = item.end

    text_end = cursor

    # ---- data/rodata region: the one allowed splice ----
    # --data-from-source replaces the splice with sections compiled from the
    # overlay's C (tools/overlay/overlay_data_from_source.py).
    data_start_off = text_end - OVERLAY_BASE
    data_report = None
    if data_from_source:
        from overlay_data_from_source import (build_data_region,
                                              DataLayoutError)
        try:
            data_report = build_data_region(overlay, items, text_end,
                                            payload, OVERLAY_BASE,
                                            verbose=verbose)
        except DataLayoutError as exc:
            return {"error": f"data-from-source: {exc}"}
        data_bytes = 0
    else:
        payload[data_start_off:PAYLOAD_SIZE] = original[data_start_off:
                                                        PAYLOAD_SIZE]
        data_bytes = PAYLOAD_SIZE - data_start_off

    # ---- write + checksum ----
    build_dir = BUILD_DIR / overlay
    build_dir.mkdir(parents=True, exist_ok=True)
    bin_path = build_dir / f"{overlay}.clean.bin"
    out_path = build_dir / f"{overlay}.clean.EXN"
    bin_path.write_bytes(bytes(payload))
    if not add_exn_checksum(bin_path, out_path):
        return {"error": "checksum step failed"}

    debt["counts"] = {
        "functions_total": len(items),
        "from_c": len(debt["from_c"]),
        "from_asm": len(debt["from_asm"]),
        "c_fallback_to_asm": len(debt["c_fallback_to_asm"]),
        "unresolved": len(debt["unresolved"]),
        "asm_divergent": len(divergent_fns),
        "text_bytes_from_c": text_bytes_from_c,
        "text_bytes_from_asm": text_bytes_from_asm,
        "gap_bytes": gap_bytes,
        "data_bytes_from_extraction": data_bytes,
    }
    debt["data_region"] = {"start": f"0x{text_end:08X}", "size": data_bytes}
    debt["asm_divergent"] = divergent_fns
    debt["cache"] = cache_stats
    if data_report is not None:
        debt["data_from_source"] = {
            k: data_report[k] for k in ("rodata_start", "data_start",
                                        "bss_start", "bss_end",
                                        "named_checked", "text_relinked")}
        debt["data_from_source"]["parts"] = data_report["parts"]
    debt["boundary_audit"] = audit_text_data_boundary(payload, text_end)
    if verbose and debt["boundary_audit"]["suspects"]:
        for s in debt["boundary_audit"]["suspects"]:
            print(f"  [BOUNDARY] {s['kind']} @ {s['addr']}: {s['detail']}")
    return {"out_path": out_path, "debt": debt, "text_end": text_end}


# ---------------------------------------------------------------------------
# Text/data boundary audit
# ---------------------------------------------------------------------------

def audit_text_data_boundary(payload: bytes, text_end: int) -> Dict:
    """Detect code hiding in the data splice (which would be byte-equal and
    therefore invisible to the SHA1 gate).

    Two independent detectors over the spliced region:
      1. jal-reachability: any `jal`/`j` in the BUILT text region whose target
         lands at/after text_end means the splice contains reachable code.
      2. shape scan: an `addiu sp,sp,-N` prologue word followed by a
         `jr ra` (0x03E00008) within 256 words, with >=60% of the intervening
         words decoding as common MIPS opcodes, flags a code-shaped span
         (jr-tables / SJIS text do not match this pattern).
    """
    suspects = []
    data_off = text_end - OVERLAY_BASE
    common_ops_t = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                    32, 33, 34, 35, 36, 37, 40, 41, 43}

    def _target_is_codelike(target: int) -> bool:
        """Does the splice at `target` decode like an actual FUNCTION?
        Requires both opcode plausibility and a reachable `jr ra` — this
        filters (a) data-in-text words that decode as jal and (b) real jals
        into space where a co-loaded runtime module materializes (the file
        bytes there are pointer tables/SJIS, which never contain jr ra)."""
        toff = target - OVERLAY_BASE
        span = struct.unpack_from("<16I", payload,
                                  min(toff, PAYLOAD_SIZE - 64))
        plausible = sum(1 for x in span if (x >> 26) in common_ops_t
                        and x != 0)
        if plausible < 10:
            return False
        n = min(128, (PAYLOAD_SIZE - toff) // 4)
        tail = struct.unpack_from(f"<{n}I", payload, toff)
        return 0x03E00008 in tail  # jr ra

    # 1. jal targets from text into the splice (only code-like targets)
    for off in range(0, data_off, 4):
        w = struct.unpack_from("<I", payload, off)[0]
        op = w >> 26
        if op in (2, 3):  # j / jal
            target = (OVERLAY_BASE & 0xF0000000) | ((w & 0x3FFFFFF) << 2)
            if text_end <= target < OVERLAY_BASE + PAYLOAD_SIZE \
                    and _target_is_codelike(target):
                suspects.append({
                    "kind": "jal_into_splice",
                    "addr": f"0x{OVERLAY_BASE + off:08X}",
                    "detail": f"{'jal' if op == 3 else 'j'} 0x{target:08X} "
                              f"targets a code-like span in the data splice"})

    # 2. prologue-shaped spans inside the splice
    common_ops = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                  32, 33, 34, 35, 36, 37, 40, 41, 43}
    n_words = (PAYLOAD_SIZE - data_off) // 4
    words = struct.unpack_from(f"<{n_words}I", payload, data_off)
    i = 0
    while i < n_words:
        w = words[i]
        # addiu sp,sp,-N (op 9, rs=rt=29, negative imm)
        if (w >> 16) == 0x27BD and (w & 0x8000):
            for j in range(i + 1, min(i + 256, n_words)):
                if words[j] == 0x03E00008:  # jr ra
                    span = words[i:j + 1]
                    plausible = sum(1 for x in span
                                    if (x >> 26) in common_ops and x != 0)
                    if plausible / len(span) >= 0.6 and len(span) >= 4:
                        suspects.append({
                            "kind": "prologue_shape",
                            "addr": f"0x{OVERLAY_BASE + data_off + i*4:08X}",
                            "detail": f"{len(span)} words decode as a "
                                      f"prologue..jr-ra body inside the "
                                      f"splice"})
                        i = j
                    break
        i += 1

    return {"clean": not suspects, "suspects": suspects[:50],
            "suspect_count": len(suspects)}


# ---------------------------------------------------------------------------
# Verify gate
# ---------------------------------------------------------------------------

def region_of(overlay: str, addr: int) -> str:
    """Human label for the layout region owning an address (for diagnostics)."""
    for it in build_layout(overlay):
        if it.addr <= addr < it.end:
            return f"{it.name} (+0x{addr - it.addr:X})"
    return "gap/data"


def verify_build(overlay: str, out_path: Path) -> Dict:
    original = (ASSETS_DIR / f"{overlay}.EXN").read_bytes()
    built = out_path.read_bytes()
    sha_o = hashlib.sha1(original).hexdigest()
    sha_b = hashlib.sha1(built).hexdigest()
    res = {"sha1_original": sha_o, "sha1_built": sha_b,
           "sha1_match": sha_o == sha_b, "first_divergence": None,
           "divergent_bytes": 0}
    if sha_o == sha_b:
        print(f"  [VERIFY] {overlay}: BYTE-PERFECT (sha1 {sha_o})")
        return res
    n = min(len(original), len(built))
    first = None
    ndiff = 0
    for i in range(n):
        if original[i] != built[i]:
            ndiff += 1
            if first is None:
                first = i
    ndiff += abs(len(original) - len(built))
    res["divergent_bytes"] = ndiff
    if first is not None:
        addr = OVERLAY_BASE + first
        a = first & ~0xF
        res["first_divergence"] = {
            "file_offset": f"0x{first:X}", "addr": f"0x{addr:08X}",
            "region": region_of(overlay, addr)}
        print(f"  [VERIFY] {overlay}: DIVERGES ({ndiff} bytes) — first at "
              f"file+0x{first:X} = 0x{addr:08X} in {res['first_divergence']['region']}")
        print(f"    original: {original[a:a+16].hex()}")
        print(f"    built:    {built[a:a+16].hex()}")
    return res


# ---------------------------------------------------------------------------
# Debt file
# ---------------------------------------------------------------------------

def _load_json(path: Path) -> Dict:
    if path.exists():
        try:
            return json.load(open(path))
        except (json.JSONDecodeError, OSError):
            pass
    return {}


def write_json_if_changed(path: Path, data: Dict):
    """Write `data` as the debt files' canonical JSON, only when it differs
    (an unchanged build leaves the tracked file's bytes and mtime alone)."""
    text = json.dumps(data, indent=2, sort_keys=True) + "\n"
    try:
        if path.read_text() == text:
            return
    except OSError:
        pass
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text)


def update_debt_file(overlay: str, debt: Dict, verify: Optional[Dict]):
    import datetime
    db = _load_json(DEBT_FILE)
    db.setdefault("_meta", {})
    db["_meta"]["tool"] = "tools/overlay/build_clean_exn.py"
    entry = {k: v for k, v in debt.items() if k not in RUNTIME_KEYS}
    if verify is not None:
        entry["verify"] = {k: verify[k] for k in
                           ("sha1_match", "sha1_built", "sha1_original",
                            "first_divergence", "divergent_bytes")}
    db[overlay] = entry
    write_json_if_changed(DEBT_FILE, db)
    runtime = _load_json(RUNTIME_FILE)
    runtime[overlay] = {"cache": debt.get("cache", {}),
                        "generated": datetime.date.today().isoformat()}
    write_json_if_changed(RUNTIME_FILE, runtime)


# ---------------------------------------------------------------------------
# Orphan-fragment .s generation (gap_with_code recovery)
# ---------------------------------------------------------------------------

def _emit_orphan_s(overlay: str, original: bytes, addr: int,
                   span: int) -> int:
    """Write asm/overlay/<OV>/func_<ADDR>.s disassembling [addr, addr+span)
    (trailing zero padding trimmed, one delay-slot word kept). Returns 1 if
    a file was written."""
    from extract_exn_functions import disassemble_function
    frag = original[addr - OVERLAY_BASE:addr - OVERLAY_BASE + span]
    size = len(frag)
    while size >= 4 and frag[size-4:size] == b"\x00" * 4:
        size -= 4
    if size == 0:
        return 0
    # keep one trailing zero word (delay-slot nop after jr/j) if trimmed
    size = min(size + 4, len(frag)) if size < len(frag) else size
    name = f"func_{addr:08X}"
    out = ASM_OVERLAY_DIR / overlay / f"{name}.s"
    if out.exists():
        return 0
    lines = [
        ".set noreorder", ".set noat", "", ".text",
        f".globl {name}", "",
        f"/* Function: {name}",
        f" * Overlay: {overlay}",
        f" * Address: 0x{addr:08X}",
        f" * Size: {size} bytes ({size // 4} instructions)",
        " * Kind: orphan — gap fragment recovered by "
        "build_clean_exn.py --gen-gap-asm",
        " */",
        f"{name}:",
    ]
    lines.extend(disassemble_function(
        original, addr - OVERLAY_BASE, size, OVERLAY_BASE))
    out.write_text("\n".join(lines) + "\n")
    print(f"  [gen] {out} ({size} bytes)")
    return 1


def gen_gap_asm(overlay: str) -> int:
    """Emit orphan disassembly .s for (a) every layout GAP whose original
    bytes are real code and (b) every HOLE inside a non-byte_perfect item's
    window that no committed .s fragment covers (e.g. an @SymbolStart-shifted
    window whose leading stub has no .s). Uses the SAME disassembler that
    produced the committed asm/ tree (extract_exn_functions). This is
    committed-disassembly generation, not byte-splicing: the build then
    ASSEMBLES the .s like any other."""
    original = (ASSETS_DIR / f"{overlay}.EXN").read_bytes()
    status_all = json.load(open(STATUS_FILE)) if STATUS_FILE.exists() else {}
    ov_status = status_all.get(overlay, {})
    items = build_layout(overlay)
    made = 0
    cursor = OVERLAY_BASE
    for it in items:
        if it.addr > cursor:
            gap = original[cursor - OVERLAY_BASE:it.addr - OVERLAY_BASE]
            if any(gap):
                made += _emit_orphan_s(overlay, original, cursor,
                                       it.addr - cursor)
        cursor = max(cursor, it.end)

    # window holes (fragment-composition dry walk, mirrors produce_asm_bytes)
    _FRAG_INDEX.pop(overlay, None)
    index = asm_fragment_index(overlay)
    sizes = {a: (parse_s_header(p)[1] or 0) for a, p in index.items()}
    for it in items:
        st = ov_status.get(it.name, {}).get("status", "no_status")
        if st == "byte_perfect":
            continue
        cur = it.addr
        while cur < it.end:
            if cur in index and sizes[cur] > 0:
                cur += sizes[cur]
                continue
            nxt = min((a for a in index if cur < a < it.end),
                      default=it.end)
            span = original[cur - OVERLAY_BASE:nxt - OVERLAY_BASE]
            if any(span):
                made += _emit_orphan_s(overlay, original, cur, nxt - cur)
            cur = nxt
    _FRAG_INDEX.pop(overlay, None)
    if made == 0:
        print(f"  [{overlay}] no code-bearing gaps/holes without .s")
    return made


def run_one(overlay: str, do_verify: bool, verbose: bool,
            jobs: int = 4, use_cache: bool = True,
            data_from_source: bool = False) -> bool:
    print(f"\n=== clean build {overlay} ===")
    res = build_clean_exn(overlay, verbose=verbose, jobs=jobs,
                          use_cache=use_cache,
                          data_from_source=data_from_source)
    if "error" in res:
        print(f"  [ERROR] {res['error']}")
        return False
    debt = res["debt"]
    c = debt["counts"]
    cs = debt.get("cache", {})
    ba = debt.get("boundary_audit", {})
    print(f"  functions: {c['from_c']} from C, {c['from_asm']} from asm "
          f"({c['c_fallback_to_asm']} C-fallbacks), "
          f"{c['unresolved']} unresolved, {c['asm_divergent']} asm-divergent")
    print(f"  bytes: C={c['text_bytes_from_c']} asm={c['text_bytes_from_asm']} "
          f"gap-fill={c['gap_bytes']} data-splice={c['data_bytes_from_extraction']}")
    print(f"  cache: {cs.get('hits', 0)} hits, {cs.get('misses', 0)} misses "
          f"({cs.get('seconds', '?')}s)  boundary: "
          + ("clean" if ba.get("clean") else
             f"{ba.get('suspect_count', '?')} SUSPECTS"))
    dfs = debt.get("data_from_source")
    if dfs:
        print(f"  data from source: .rodata 0x{dfs['rodata_start']:08X} "
              f".data 0x{dfs['data_start']:08X} .bss 0x{dfs['bss_start']:08X}"
              f"-0x{dfs['bss_end']:08X}; {len(dfs['parts'])} parts, "
              f"{dfs['named_checked']} names checked, "
              f"{dfs['text_relinked']} functions relinked")
        if verbose:
            for a, n, what in dfs["parts"]:
                print(f"    {a} +{n:<5} {what}")
    print(f"  wrote {res['out_path']}")
    verify = None
    if do_verify:
        verify = verify_build(overlay, res["out_path"])
    update_debt_file(overlay, debt, verify)
    # A byte_perfect function whose C the clean build cannot reproduce is a
    # contradiction between this build and the per-function gates: one of
    # them is wrong.  The EXN can still be byte-perfect (the .s fills in),
    # so fail the gate explicitly rather than hide it in the debt counts.
    bp_fallback = [f["name"] for f in debt["c_fallback_to_asm"]
                   if f.get("status") == "byte_perfect"]
    if bp_fallback:
        print(f"  [GATE] {len(bp_fallback)} byte_perfect function(s) fell "
              f"back to .s: {' '.join(bp_fallback[:10])}")
        return False
    return (verify is None) or verify["sha1_match"]


def print_report():
    """Tree-wide debt summary (north-star metric: honest C fraction)."""
    if not DEBT_FILE.exists():
        print("no debt file yet — run a build first")
        return
    db = json.load(open(DEBT_FILE))
    ovs = sorted(k for k in db if not k.startswith("_") and k != "main")
    hdr = (f"{'overlay':10} {'gate':6} {'fns':>5} {'C':>5} {'asm':>5} "
           f"{'fall':>4} {'unres':>5} {'%code-C':>8} {'code-C':>8} "
           f"{'code-asm':>8} {'gap':>5} {'data':>7} {'bnd':>4}")
    print(hdr)
    print("-" * len(hdr))
    tot = {"fns": 0, "c": 0, "asm": 0, "fall": 0, "unres": 0,
           "bc": 0, "ba": 0, "gap": 0, "data": 0, "gates": 0, "ok": 0}
    for ov in ovs:
        e = db[ov]
        c = e.get("counts", {})
        v = e.get("verify", {})
        ba = e.get("boundary_audit", {})
        bc = c.get("text_bytes_from_c", 0)
        bas = c.get("text_bytes_from_asm", 0)
        code = bc + bas
        pct = f"{100.0 * bc / code:7.1f}%" if code else "      -"
        gate = ("PASS" if v.get("sha1_match")
                else ("FAIL" if v else "-"))
        bnd = "ok" if ba.get("clean", True) else f"{ba.get('suspect_count')}!"
        print(f"{ov:10} {gate:6} {c.get('functions_total', 0):>5} "
              f"{c.get('from_c', 0):>5} {c.get('from_asm', 0):>5} "
              f"{c.get('c_fallback_to_asm', 0):>4} "
              f"{c.get('unresolved', 0):>5} {pct:>8} {bc:>8} {bas:>8} "
              f"{c.get('gap_bytes', 0):>5} "
              f"{c.get('data_bytes_from_extraction', 0):>7} {bnd:>4}")
        tot["fns"] += c.get("functions_total", 0)
        tot["c"] += c.get("from_c", 0)
        tot["asm"] += c.get("from_asm", 0)
        tot["fall"] += c.get("c_fallback_to_asm", 0)
        tot["unres"] += c.get("unresolved", 0)
        tot["bc"] += bc
        tot["ba"] += bas
        tot["gap"] += c.get("gap_bytes", 0)
        tot["data"] += c.get("data_bytes_from_extraction", 0)
        if v:
            tot["gates"] += 1
            tot["ok"] += 1 if v.get("sha1_match") else 0
    print("-" * len(hdr))
    code = tot["bc"] + tot["ba"]
    pct = f"{100.0 * tot['bc'] / code:7.1f}%" if code else "      -"
    print(f"{'TOTAL':10} {tot['ok']:>3}/{tot['gates']:<2} {tot['fns']:>5} "
          f"{tot['c']:>5} {tot['asm']:>5} {tot['fall']:>4} "
          f"{tot['unres']:>5} {pct:>8} {tot['bc']:>8} {tot['ba']:>8} "
          f"{tot['gap']:>5} {tot['data']:>7}")
    print(f"\nnorth star (overlays): {pct.strip()} of code bytes from "
          f"honest C ({tot['bc']}/{code}); gates byte-perfect: "
          f"{tot['ok']}/{tot['gates']}")
    if "main" in db:
        mc = db["main"].get("counts", {})
        mv = db["main"].get("verify", {})
        gate = ("PASS" if mv.get("sha1_match")
                else ("FAIL" if mv else "-"))
        print(f"\nmain binary (build_clean_main.py): gate {gate} | "
              f"{mc.get('c_functions', 0)} C functions = "
              f"{mc.get('bytes_from_c', 0)} bytes | "
              f"as-path round-trip {mc.get('bytes_from_asm_roundtrip', 0)} "
              f"bytes (incl. interleaved data) | ROM fallback "
              f"{mc.get('bytes_rom_fallback', 0)} bytes | header splice "
              f"{mc.get('header_bytes_spliced', 0)}")
    if "_iso" in db:
        iv = db["_iso"].get("verify", {})
        rec = db["_iso"].get("reconciliation", {})
        if iv:
            ok = iv.get("track1_match") and iv.get("track2_match")
            print(f"disc (build_clean_iso.py): "
                  f"{'BYTE-PERFECT' if ok else 'DIVERGES'} "
                  f"(track1 {'ok' if iv.get('track1_match') else 'X'}, "
                  f"track2 {'ok' if iv.get('track2_match') else 'X'}; "
                  f"{len(rec.get('reconciled', []))} metadata sectors "
                  f"reconciled, {len(rec.get('refused', []))} refused)")


def main():
    import argparse
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("overlays", nargs="*", help="overlay name(s), e.g. RENSYU")
    ap.add_argument("--all", action="store_true")
    ap.add_argument("--verify", action="store_true",
                    help="SHA1 gate vs assets/overlays/<OV>.EXN")
    ap.add_argument("--gen-gap-asm", action="store_true",
                    help="emit orphan .s for code-bearing gaps, then exit")
    ap.add_argument("--report", action="store_true",
                    help="print the tree-wide debt summary table and exit")
    ap.add_argument("-j", "--jobs", type=int, default=os.cpu_count() or 4,
                    help="parallel produce workers (default: cpu count)")
    ap.add_argument("--no-cache", action="store_true",
                    help="bypass build/clean_exn_cache")
    ap.add_argument("--data-from-source", action="store_true",
                    help="build the .rodata/.data/.bss region from source "
                         "(overlay_data_from_source.py) instead of copying "
                         "it from the EXN; opt-in")
    ap.add_argument("-v", "--verbose", action="store_true")
    args = ap.parse_args()

    if args.report:
        print_report()
        return

    if args.all:
        overlays = sorted(p.stem for p in CONFIG_DIR.glob("*.sym"))
    else:
        overlays = [o.upper() for o in args.overlays]
    if not overlays:
        ap.print_help()
        sys.exit(1)

    os.chdir(ROOT)  # custom PostProc scripts may assume repo-root cwd

    if args.gen_gap_asm:
        total = sum(gen_gap_asm(ov) for ov in overlays)
        print(f"[DONE] generated {total} orphan .s")
        return

    ok = 0
    for ov in overlays:
        if run_one(ov, args.verify, args.verbose, jobs=args.jobs,
                   use_cache=not args.no_cache,
                   data_from_source=args.data_from_source):
            ok += 1
    print(f"\n[DONE] {ok}/{len(overlays)} overlays "
          + ("byte-perfect" if args.verify else "built"))
    sys.exit(0 if ok == len(overlays) else 1)


if __name__ == "__main__":
    main()
