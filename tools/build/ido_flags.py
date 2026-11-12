#!/usr/bin/env python3
"""Single source of truth for the IDO 5.2 command line.

Every tool that shells out to ``tools/ido-static-recomp/build/5.2/out/cc``
should build its command line from here rather than copy-pasting a flag list.
Before this module existed the same five flags were duplicated in ~20 files,
which is why adding one flag (``-Wc,-pic1``) was a twenty-file change.

────────────────────────────────────────────────────────────────────────────
The PIC mode, a.k.a. "why every function used to carry patch_stack_complete"
────────────────────────────────────────────────────────────────────────────

``-Wc,`` is the cc driver's pass-through to **ugen** (the code generator).
``-pic1`` is an original, undocumented ugen option (present in the untouched
IRIX binary ``tools/ido-static-recomp/ido/5.2/usr/lib/ugen``, but with no
driver spelling in IRIX 5.2) that reserves the IRIX-4-era PIC linkage area:
a silent **+16 bytes in every frame ugen emits**, with the instruction stream
otherwise unchanged.  Frameless functions are unaffected.

That +16 is exactly what ``@PostProc: patch_stack_complete --old N --new N+16``
was painting in on ~4,500 overlay functions.  Konami built the game region
with the linkage area reserved and the Sony library region without it.

So ``-pic1`` and a ``+16`` ``patch_stack_complete`` are two spellings of the
same thing, and a translation unit uses exactly one of them — never both.
That is the rule this module encodes:

    A source file is compiled ``-Wc,-pic1`` unless it still carries a
    ``patch_stack``/``patch_stack_complete`` directive (or names a ``-Wc,-picN``
    explicitly in ``@CompileFlags``).

The coupling is deliberate and it FAILS LOUD, which is what makes an
incremental rollout safe: ``patch_stack_complete.py`` aborts with
``No function prologue found with frame size N`` when its ``--old`` does not
match, so a file that gets ``-pic1`` while still carrying the directive dies
with ``POSTPROC CHAIN FAILED`` rather than silently emitting wrong bytes.

Files that legitimately need the old footing but carry no directive — chiefly
the ~112 functions whose C was hand-padded with 16 bytes of invented dummy
locals to fake the frame — opt out with an explicit
``/* @CompileFlags: -Wc,-pic0 */``.  Those stamps are a to-do list: the real
fix is deleting the invented locals, after which the stamp comes off.

Region policy: the split
is per translation unit, not global.  Overlays and the main **game** region
(< 0x80086000) are ``pic1``; the Sony **library** region (>= 0x80086000) must
stay ``pic0`` — 48 of its 73 framed functions target frame 24, the stock IDO
minimum.
"""

import re
from pathlib import Path
from typing import Iterable, List, Optional, Sequence, Union

# ── The base command line ───────────────────────────────────────────────────
# C89, MIPS I, little-endian (-EL is critical), no small-data section.
BASE_IDO_FLAGS = ["-O2", "-G0", "-mips1", "-EL", "-non_shared"]

PIC1_FLAG = "-Wc,-pic1"
PIC0_FLAG = "-Wc,-pic0"

# Addresses below this are Konami game code; at or above is the Sony SDK
# library region, which was built without the PIC linkage area.
LIBRARY_REGION_START = 0x80086000

# Overlays all load here; every overlay function is game code.
OVERLAY_BASE = 0x80132000

_PIC_FLAG_RE = re.compile(r'-Wc,-pic[012]\b')

# Mirrors parse_postproc._COMMENT_PREFIX: a directive counts only when its
# line OPENS with the keyword after nothing but comment markers/whitespace.
# Duplicated (not imported) to keep this module dependency-free and importable
# from anywhere on sys.path.
_DIRECTIVE_RE = re.compile(
    r'^\s*(?:(?:/\*+|\*+|//)\s*)*@(?:PostProc|CompileFlags):\s*(.*)$')

_FRAME_DIRECTIVE_RE = re.compile(r'\bpatch_stack(?:_complete)?\b')


def _read(source: Union[str, Path, None]) -> str:
    if source is None:
        return ""
    p = Path(source)
    try:
        return p.read_text(errors="replace")
    except OSError:
        return ""


def has_frame_directive(source: Union[str, Path, None] = None,
                        content: Optional[str] = None) -> bool:
    """True if the file carries a @PostProc patch_stack[_complete] directive.

    Only real directive lines count — a prose mention of the tool name in a
    header comment does not (same discipline as parse_postproc).
    """
    if content is None:
        content = _read(source)
    for line in content.splitlines():
        m = _DIRECTIVE_RE.match(line)
        if m and _FRAME_DIRECTIVE_RE.search(m.group(1)):
            return True
    return False


def has_explicit_pic(source: Union[str, Path, None] = None,
                     content: Optional[str] = None,
                     compile_flags: Sequence[str] = ()) -> bool:
    """True if the file (or the caller) already names a -Wc,-picN mode."""
    if any(_PIC_FLAG_RE.search(f) for f in compile_flags):
        return True
    if content is None:
        content = _read(source)
    for line in content.splitlines():
        m = _DIRECTIVE_RE.match(line)
        if m and _PIC_FLAG_RE.search(m.group(1)):
            return True
    return False


def region_allows_pic1(address: Optional[int] = None,
                       region: str = "overlay") -> bool:
    """Region gate.  Overlays and main game code yes; Sony library no."""
    if region == "overlay":
        return True
    if region == "library":
        return False
    if address is not None:
        return address < LIBRARY_REGION_START
    # Unknown region and no address: refuse to guess, keep stock behaviour.
    return False


def pic_flags(source: Union[str, Path, None] = None,
              content: Optional[str] = None,
              compile_flags: Sequence[str] = (),
              address: Optional[int] = None,
              region: str = "overlay") -> List[str]:
    """The PIC portion of the command line for one translation unit.

    Returns ``[]`` (stock ``pic0``) or ``["-Wc,-pic1"]``.  Empty when the file
    names its own ``-Wc,-picN`` (the caller passes that through separately via
    ``@CompileFlags``), when it still carries a ``patch_stack`` directive, or
    when the region is the Sony library.
    """
    if not region_allows_pic1(address=address, region=region):
        return []
    if content is None:
        content = _read(source)
    if has_explicit_pic(content=content, compile_flags=compile_flags):
        return []
    if has_frame_directive(content=content):
        return []
    return [PIC1_FLAG]


def ido_flags(include_dirs: Iterable[Union[str, Path]] = (),
              source: Union[str, Path, None] = None,
              content: Optional[str] = None,
              compile_flags: Sequence[str] = (),
              address: Optional[int] = None,
              region: str = "overlay",
              with_pic: bool = True) -> List[str]:
    """Full flag list: base flags, ``-I`` dirs, and the PIC mode.

    ``with_pic=False`` yields the historical stock line, for tools that must
    reproduce a pre-rollout compile.
    """
    flags = list(BASE_IDO_FLAGS)
    flags += [f"-I{d}" for d in include_dirs]
    if with_pic:
        flags += pic_flags(source=source, content=content,
                           compile_flags=compile_flags,
                           address=address, region=region)
    return flags


def stripped_pic_extra(content: str,
                       compile_flags: Sequence[str] = (),
                       address: Optional[int] = None,
                       region: str = "overlay") -> List[str]:
    """Extra cflags that give a PostProc-STRIPPED copy of a source file the
    PIC mode its SHIPPED compile has.

    Stripping the ``@PostProc`` lines deletes any ``patch_stack`` directive,
    which would flip the derived mode of the copy from pic0 to pic1.  So:

    * the file names its own ``-Wc,-picN`` (``@CompileFlags``): ``[]`` -- the
      stripped copy keeps that directive and the compile passes it through.
      Appending anything here would OVERRIDE it under the driver's last-wins
      parsing (the 2026-10-04 bug: ``-Wc,-pic1`` stamps compiled pic0);
    * otherwise the shipped file's derived mode, spelled out explicitly:
      ``[-Wc,-pic1]`` or ``[-Wc,-pic0]``.
    """
    if has_explicit_pic(content=content, compile_flags=compile_flags):
        return []
    if not region_allows_pic1(address=address, region=region):
        return []
    return pic_flags(content=content, compile_flags=compile_flags,
                     address=address, region=region) or [PIC0_FLAG]


def base_flags(include_dirs: Iterable[Union[str, Path]] = ()) -> List[str]:
    """Base flags + includes, with no PIC decision (per-source, add later)."""
    return list(BASE_IDO_FLAGS) + [f"-I{d}" for d in include_dirs]


# ── Flag SCREENING support (--extra-cflags) ─────────────────────────────────
# Ad-hoc flags supplied by a screening harness, on top of whatever the source
# file already declares.  The
# whole point is that a candidate flag can be evaluated through the REAL
# pipeline (compile + PostProc + name-derived resolved link) instead of a
# hand-rolled monkeypatch.

def parse_extra_cflags(values: Optional[Iterable[str]]) -> List[str]:
    """Normalise repeated / whitespace-separated ``--extra-cflags`` values.

    ⚠️ **Commas are NOT separators.**  Every interesting IDO pass-through flag
    embeds commas (``-Wo,-rege,0``, ``-Wa,-Olimit,1``, ``-Wc,-pic1``), so
    splitting on ``,`` would shred them into garbage the driver rejects.  Repeat
    the option, or separate flags with whitespace inside one value:

        --extra-cflags -Wo,-rege,0 --extra-cflags -Wa,-noxbb
        --extra-cflags "-Wo,-rege,0 -Wa,-noxbb"
    """
    out: List[str] = []
    for v in values or ():
        if v is None:
            continue
        out.extend(v.split())
    return out


def glue_option_values(argv: Sequence[str],
                       option: str = "--extra-cflags") -> List[str]:
    """Rewrite ``--extra-cflags -Wo,-x`` into ``--extra-cflags=-Wo,-x``.

    argparse refuses a value that starts with ``-`` unless it is attached with
    ``=``, and *every* flag worth screening starts with ``-``.  Call this on
    ``sys.argv[1:]`` before ``parse_args`` so both spellings work.
    """
    out: List[str] = []
    i = 0
    argv = list(argv)
    while i < len(argv):
        if argv[i] == option and i + 1 < len(argv):
            out.append(f"{option}={argv[i + 1]}")
            i += 2
        else:
            out.append(argv[i])
            i += 1
    return out


def compile_flag_tail(source: Union[str, Path, None] = None,
                      content: Optional[str] = None,
                      compile_flags: Sequence[str] = (),
                      extra_cflags: Sequence[str] = (),
                      address: Optional[int] = None,
                      region: str = "overlay") -> List[str]:
    """PIC mode + the file's own ``@CompileFlags`` + screening flags, in order.

    This is the tail every IDO compile site appends after ``base_flags()``.
    With ``extra_cflags=()`` the result is byte-identical to the historical
    ``pic_flags(src, compile_flags=file_flags) + file_flags`` — the default
    build path is untouched.

    Composition with the derived PIC mode (``ido_flags`` rule: pic1 unless the
    file still carries a ``patch_stack`` directive or names its own
    ``-Wc,-picN``):

    * screening flags are considered when deciding whether an explicit PIC mode
      is already named, so **an explicit ``-Wc,-picN`` in extra_cflags
      suppresses the derived one** rather than colliding with it;
    * screening flags are emitted LAST, so under the driver's last-wins parsing
      they also beat anything the file declared.
    """
    file_flags = list(compile_flags)
    screen = list(extra_cflags)
    pic = pic_flags(source=source, content=content,
                    compile_flags=file_flags + screen,
                    address=address, region=region)
    return pic + file_flags + screen
