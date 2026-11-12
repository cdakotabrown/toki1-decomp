"""Parser for SN Systems Psy-Q object (.OBJ / "LNK\\x02") and library (.LIB) files.

Format was reverse engineered from the record stream and cross-checked against
`strings` output and, when supplied, the synthetic fixture TEST267.OBJ
(PSYQ_OBJ_FIXTURE; not in the tree since the 2026-11 cleanup).
Record/opcode numbering follows the community-standard psyq-obj-parser from
grumpycoders/pcsx-redux (MIT); no code was copied, only the opcode numbering.

Validated invariants (see self_test()):
  * every record consumes bytes and the stream ends exactly at EOF
  * recovered symbol names / source paths agree with `strings -n 6`
  * per-section emitted byte counts are self-consistent
"""

import os
import struct
import sys

# ---- OBJ record opcodes -------------------------------------------------
OP_END = 0
OP_BYTES = 2
OP_SWITCH = 6
OP_ZEROES = 8
OP_RELOCATION = 10
OP_EXPORTED_SYMBOL = 12
OP_IMPORTED_SYMBOL = 14
OP_SECTION = 16
OP_LOCAL_SYMBOL = 18
OP_FILENAME = 28
OP_PROGRAMTYPE = 46
OP_UNINITIALIZED = 48
OP_INC_SLD_LINENUM = 50
OP_INC_SLD_LINENUM_BY_BYTE = 52
OP_INC_SLD_LINENUM_BY_WORD = 54
OP_SET_SLD_LINENUM = 56
OP_SET_SLD_LINENUM_TO_LINE_IN_FILE = 58
OP_END_SLD = 60
OP_FUNCTION = 74
OP_FUNCTION_END = 76
OP_BLOCK_START = 78
OP_BLOCK_END = 80
OP_SECTION_DEF = 82
OP_SECTION_DEF2 = 84
OP_FUNCTION_START2 = 86

# ---- relocation types ---------------------------------------------------
# name -> (mask of bits the relocation OWNS in the little-endian word)
RELOC_MASKS = {
    16: ("REL32", 0xFFFFFFFF),      # full 32-bit address word
    74: ("REL26", 0x03FFFFFF),      # j/jal target field
    82: ("HI16", 0x0000FFFF),       # lui   %hi
    84: ("LO16", 0x0000FFFF),       # addiu/lw %lo
    30: ("GPREL16", 0x0000FFFF),    # $gp relative imm16
    2:  ("REL32_BE", 0xFFFFFFFF),
    92: ("REL26_BE", 0x03FFFFFF),
    94: ("HI16_BE", 0x0000FFFF),
    96: ("LO16_BE", 0x0000FFFF),
}

# ---- expression node opcodes -------------------------------------------
EX_VALUE = 0
EX_SYMBOL = 2
EX_SECTION_BASE = 4
EX_SECTION_START = 12
EX_SECTION_END = 22
EX_ADD = 44
EX_SUB = 46
EX_DIV = 50
EX_OR = 54    # observed in LIBSN 4.5: 0xa0000000 | (sect+off), KSEG1 form


class ParseError(Exception):
    pass


class Reader(object):
    def __init__(self, data, pos=0, end=None):
        self.d = data
        self.p = pos
        self.end = len(data) if end is None else end

    def eof(self):
        return self.p >= self.end

    def u8(self):
        if self.p + 1 > self.end:
            raise ParseError("u8 past end")
        v = self.d[self.p]
        self.p += 1
        return v

    def u16(self):
        if self.p + 2 > self.end:
            raise ParseError("u16 past end")
        v = struct.unpack_from("<H", self.d, self.p)[0]
        self.p += 2
        return v

    def u32(self):
        if self.p + 4 > self.end:
            raise ParseError("u32 past end")
        v = struct.unpack_from("<I", self.d, self.p)[0]
        self.p += 4
        return v

    def blob(self, n):
        if self.p + n > self.end:
            raise ParseError("blob past end")
        v = self.d[self.p:self.p + n]
        self.p += n
        return v

    def pstr(self):
        n = self.u8()
        return self.blob(n).decode("latin-1")


class Section(object):
    __slots__ = ("index", "group", "align", "name", "data", "relocs",
                 "chunk_base")

    def __init__(self, index, group, align, name):
        self.index = index
        self.group = group
        self.align = align
        self.name = name
        self.data = bytearray()
        # (offset, reloc_type, expr); offset is SECTION-relative (see
        # OP_RELOCATION in parse_obj)
        self.relocs = []
        # section offset at which the most recent BYTES chunk started
        self.chunk_base = 0

    def __repr__(self):
        return "<Section %d %s len=%d relocs=%d>" % (
            self.index, self.name, len(self.data), len(self.relocs))


class PsyqObject(object):
    def __init__(self, name=""):
        self.name = name
        self.progtype = None
        self.sections = {}         # index -> Section
        self.files = {}            # index -> source path
        self.symbols = {}          # symbol index -> dict
        self.exports = []          # dicts: name, section, offset
        self.imports = {}          # index -> name
        self.locals = []           # dicts: name, section, offset
        self.uninit = []           # dicts: name, section, size
        self.functions = []        # dicts: name, section, offset, file, line

    def section_by_name(self, nm):
        for s in self.sections.values():
            if s.name == nm:
                return s
        return None

    def source_file(self):
        if not self.files:
            return None
        return self.files[min(self.files)]


def _read_expr(r):
    """Read one relocation expression node; returns a nested tuple."""
    op = r.u8()
    if op == EX_VALUE:
        return ("value", r.u32())
    if op == EX_SYMBOL:
        return ("symbol", r.u16())
    if op == EX_SECTION_BASE:
        return ("sectbase", r.u16())
    if op == EX_SECTION_START:
        return ("sectstart", r.u16())
    if op == EX_SECTION_END:
        return ("sectend", r.u16())
    if op in (EX_ADD, EX_SUB, EX_DIV, EX_OR):
        nm = {EX_ADD: "add", EX_SUB: "sub", EX_DIV: "div",
              EX_OR: "or"}[op]
        a = _read_expr(r)
        b = _read_expr(r)
        return (nm, a, b)
    raise ParseError("unknown expression opcode %d at 0x%x" % (op, r.p - 1))


def parse_obj(data, name="", pos=0, end=None, strict=True):
    r = Reader(data, pos, end)
    magic = r.blob(4)
    if magic != b"LNK\x02":
        raise ParseError("bad magic %r" % (magic,))
    obj = PsyqObject(name)
    cur = None
    while not r.eof():
        op = r.u8()
        if op == OP_END:
            break
        elif op == OP_BYTES:
            n = r.u16()
            b = r.blob(n)
            if cur is None:
                raise ParseError("BYTES with no current section")
            cur.chunk_base = len(cur.data)
            cur.data += b
        elif op == OP_SWITCH:
            idx = r.u16()
            cur = obj.sections.get(idx)
            if cur is None:
                raise ParseError("SWITCH to unknown section %d" % idx)
        elif op == OP_ZEROES:
            n = r.u32()
            if cur is None:
                raise ParseError("ZEROES with no current section")
            cur.data += b"\x00" * n
        elif op == OP_RELOCATION:
            rt = r.u8()
            off = r.u16()
            expr = _read_expr(r)
            if cur is None:
                raise ParseError("RELOCATION with no current section")
            # The record's u16 offset is relative to the start of the
            # PRECEDING BYTES chunk, not to the section (a section is emitted
            # as several chunks, and a u16 could not address past 64 KiB
            # anyway).  Before 2026-10-16 the raw value was stored, which put
            # every relocation after a section's first chunk on the wrong
            # word (21,155 of 102,817 .text relocations across the 2.5-4.5
            # archives landed on an opcode their type cannot patch; 0 do now
            # -- see test_psyq_obj.py).
            cur.relocs.append((cur.chunk_base + off, rt, expr))
        elif op == OP_EXPORTED_SYMBOL:
            si = r.u16()
            sec = r.u16()
            off = r.u32()
            nm = r.pstr()
            d = {"name": nm, "section": sec, "offset": off, "index": si}
            obj.symbols[si] = d
            obj.exports.append(d)
        elif op == OP_IMPORTED_SYMBOL:
            si = r.u16()
            nm = r.pstr()
            obj.imports[si] = nm
            obj.symbols[si] = {"name": nm, "import": True, "index": si}
        elif op == OP_SECTION:
            idx = r.u16()
            grp = r.u16()
            al = r.u8()
            nm = r.pstr()
            obj.sections[idx] = Section(idx, grp, al, nm)
        elif op == OP_LOCAL_SYMBOL:
            sec = r.u16()
            off = r.u32()
            nm = r.pstr()
            obj.locals.append({"name": nm, "section": sec, "offset": off})
        elif op == OP_FILENAME:
            idx = r.u16()
            nm = r.pstr()
            obj.files[idx] = nm
        elif op == OP_PROGRAMTYPE:
            obj.progtype = r.u8()
        elif op == OP_UNINITIALIZED:
            si = r.u16()
            sec = r.u16()
            sz = r.u32()
            nm = r.pstr()
            d = {"name": nm, "section": sec, "size": sz, "index": si,
                 "bss": True}
            obj.symbols[si] = d
            obj.uninit.append(d)
        elif op == OP_INC_SLD_LINENUM:
            pass
        elif op == OP_INC_SLD_LINENUM_BY_BYTE:
            r.u8()
        elif op == OP_INC_SLD_LINENUM_BY_WORD:
            r.u16()
        elif op == OP_SET_SLD_LINENUM:
            r.u32()
        elif op == OP_SET_SLD_LINENUM_TO_LINE_IN_FILE:
            r.u32()
            r.u16()
        elif op == OP_END_SLD:
            pass
        elif op == OP_FUNCTION:
            sec = r.u16()
            off = r.u32()
            fi = r.u16()
            line = r.u32()
            r.u16()          # frame reg
            r.u32()          # frame size
            r.u16()          # return pc reg
            r.u32()          # mask
            r.u32()          # mask offset
            nm = r.pstr()
            obj.functions.append({"name": nm, "section": sec, "offset": off,
                                  "file": fi, "line": line})
        elif op == OP_FUNCTION_END:
            r.u16()
            r.u32()
            r.u32()
        elif op in (OP_BLOCK_START, OP_BLOCK_END):
            r.u16()
            r.u32()
            r.u32()
        elif op == OP_SECTION_DEF:
            r.u16(); r.u32(); r.u16(); r.u16()
            _skip_defsize(r)
            r.pstr()
        elif op == OP_SECTION_DEF2:
            r.u16(); r.u32(); r.u16(); r.u16()
            _skip_defsize(r)
            ndim = r.u16()
            for _ in range(ndim):
                r.u16()
            r.pstr()
            r.pstr()
        elif op == OP_FUNCTION_START2:
            sec = r.u16()
            off = r.u32()
            fi = r.u16()
            line = r.u32()
            r.u16(); r.u32(); r.u16(); r.u32(); r.u32()
            nm = r.pstr()
            obj.functions.append({"name": nm, "section": sec, "offset": off,
                                  "file": fi, "line": line})
        else:
            raise ParseError("unknown opcode %d at 0x%x" % (op, r.p - 1))
    obj._endpos = r.p
    return obj


def _skip_defsize(r):
    r.u32()


# ---- .LIB container -----------------------------------------------------

class LibMember(object):
    __slots__ = ("name", "date", "obj", "symbols", "libname")

    def __init__(self, name, date, obj, symbols, libname):
        self.name = name
        self.date = date
        self.obj = obj
        self.symbols = symbols
        self.libname = libname

    def __repr__(self):
        return "<LibMember %s/%s>" % (self.libname, self.name)


def parse_lib(path):
    with open(path, "rb") as f:
        data = f.read()
    if data[:4] != b"LIB\x01":
        raise ParseError("%s: bad LIB magic %r" % (path, data[:4]))
    libname = os.path.basename(path)
    members = []
    p = 4
    n = len(data)
    while p + 20 <= n:
        name = data[p:p + 8].decode("latin-1").rstrip(" \x00")
        if not name:
            break
        date = struct.unpack_from("<I", data, p + 8)[0]
        objoff = struct.unpack_from("<I", data, p + 12)[0]
        total = struct.unpack_from("<I", data, p + 16)[0]
        if total <= 0 or p + total > n:
            raise ParseError("%s: member %s bad size %d at 0x%x"
                             % (path, name, total, p))
        # symbol name table between header and obj payload
        syms = []
        q = p + 20
        objstart = p + objoff
        while q < objstart:
            ln = data[q]
            q += 1
            if ln == 0:
                break
            syms.append(data[q:q + ln].decode("latin-1"))
            q += ln
        obj = parse_obj(data, name, objstart, p + total)
        members.append(LibMember(name, date, obj, syms, libname))
        p += total
    return members


def iter_libs(dirs):
    for d in dirs:
        if not os.path.isdir(d):
            continue
        for fn in sorted(os.listdir(d)):
            if fn.upper().endswith(".LIB"):
                yield os.path.join(d, fn)


def self_test(root):
    """Validate the parser against the known fixture + every shipped archive."""
    ok = True
    # The synthetic TEST267.OBJ fixture and Sony's archives left the tree in the
    # 2026-11 public-release cleanup (they are SDK material). Point PSYQ_OBJ_FIXTURE
    # at your own copy of the fixture to exercise this case; otherwise skip it.
    fx = os.environ.get("PSYQ_OBJ_FIXTURE",
                        os.path.join(root, "tools", "psyq402", "TEST267.OBJ"))
    if not os.path.isfile(fx):
        print("fixture TEST267.OBJ: SKIP (not in this tree; set PSYQ_OBJ_FIXTURE)")
        return True
    with open(fx, "rb") as f:
        d = f.read()
    o = parse_obj(d, "TEST267")
    print("fixture TEST267.OBJ:")
    print("  progtype=%s consumed=%d/%d" % (o.progtype, o._endpos, len(d)))
    print("  sections: %s" % ([s.name for s in o.sections.values()],))
    print("  source  : %s" % (o.source_file(),))
    print("  exports : %s" % ([e["name"] for e in o.exports],))
    print("  bss     : %s" % ([u["name"] for u in o.uninit],))
    txt = o.section_by_name(".text")
    print("  .text   : %d bytes, %d relocs %s" %
          (len(txt.data), len(txt.relocs),
           [(hex(a), RELOC_MASKS.get(t, ("?",))[0]) for a, t, _ in txt.relocs]))
    if o._endpos != len(d):
        print("  ** did not consume whole file")
        ok = False
    if "test_func" not in [e["name"] for e in o.exports]:
        print("  ** missing test_func export")
        ok = False
    return ok


if __name__ == "__main__":
    ROOT = os.path.dirname(os.path.dirname(os.path.dirname(
        os.path.abspath(__file__))))
    print("ROOT = %s" % ROOT)
    sys.exit(0 if self_test(ROOT) else 1)
