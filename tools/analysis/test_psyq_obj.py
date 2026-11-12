"""Regression tests for psyq_obj: SN Psy-Q .OBJ / .LIB parsing.

The bug these guard (fixed 2026-10-16): a RELOCATION record's u16 offset is
relative to the start of the PRECEDING BYTES chunk, not to the section.  The
parser stored it raw, so in any section emitted as more than one chunk every
relocation after the first chunk sat on the wrong word -- and every tool that
read "which symbol does this function reference, in what order" off the
relocation table was reading the wrong references.

    python3 tools/analysis/test_psyq_obj.py
"""
import os
import struct
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import psyq_obj as P  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))))
# Sony's archives are not in the tree (2026-11 cleanup); point PSYQ_SDK_DIRS at
# your own copy to run the archive-wide cases, otherwise they are skipped.
LIB_DIRS = [d for d in os.environ.get("PSYQ_SDK_DIRS", "").split(os.pathsep) if d]
if not LIB_DIRS:
    LIB_DIRS = [os.path.join(ROOT, "tools", "psyq-sdk", v, "lib")
                for v in ("2.5", "4.5")]

# opcodes a relocation of each type can legally patch
LO_OPS = {0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x20, 0x21, 0x22, 0x23,
          0x24, 0x25, 0x26, 0x28, 0x29, 0x2a, 0x2b, 0x2e, 0x32, 0x3a}
OPCLASS = {82: {0x0f}, 84: LO_OPS, 74: {0x02, 0x03}}


def bad_relocs(sec):
    """Relocations whose target word's opcode the reloc type cannot patch."""
    out = []
    for off, rt, _e in sec.relocs:
        ok = OPCLASS.get(rt)
        if ok is None:
            continue
        if off % 4 or off + 4 > len(sec.data):
            out.append((off, rt))
            continue
        if (struct.unpack_from("<I", sec.data, off)[0] >> 26) not in ok:
            out.append((off, rt))
    return out


def _pstr(s):
    return bytes([len(s)]) + s.encode("latin-1")


class SyntheticChunks(unittest.TestCase):
    """A hand-built object: .text as two BYTES chunks, one reloc after each."""

    def build(self):
        lui = struct.pack("<I", 0x3C020000)       # lui $v0,0
        jal = struct.pack("<I", 0x0C000000)       # jal 0
        nop = b"\0" * 4
        c1 = lui + nop                            # section +0x0 .. +0x8
        c2 = nop + nop + jal + nop                # section +0x8 .. +0x18
        rec = b"LNK\x02"
        rec += bytes([P.OP_SECTION]) + struct.pack("<HHB", 1, 0, 8) + \
            _pstr(".text")
        rec += bytes([P.OP_IMPORTED_SYMBOL]) + struct.pack("<H", 5) + \
            _pstr("callee")
        rec += bytes([P.OP_SWITCH]) + struct.pack("<H", 1)
        rec += bytes([P.OP_BYTES]) + struct.pack("<H", len(c1)) + c1
        rec += bytes([P.OP_RELOCATION, 82]) + struct.pack("<H", 0) + \
            bytes([P.EX_SYMBOL]) + struct.pack("<H", 5)
        rec += bytes([P.OP_BYTES]) + struct.pack("<H", len(c2)) + c2
        # offset 8 within the SECOND chunk = section offset 0x10
        rec += bytes([P.OP_RELOCATION, 74]) + struct.pack("<H", 8) + \
            bytes([P.EX_SYMBOL]) + struct.pack("<H", 5)
        rec += bytes([P.OP_END])
        return rec

    def test_offset_is_chunk_relative(self):
        o = P.parse_obj(self.build(), "SYN")
        t = o.section_by_name(".text")
        self.assertEqual(len(t.data), 0x18)
        self.assertEqual([(off, rt) for off, rt, _e in t.relocs],
                         [(0x0, 82), (0x10, 74)])
        self.assertEqual(bad_relocs(t), [])


SDK_PRESENT = any(os.path.isdir(d) for d in LIB_DIRS)


class RealArchives(unittest.TestCase):
    """Every archive the identification tools load (needs a user-supplied SDK)."""

    @classmethod
    def setUpClass(cls):
        if not SDK_PRESENT:
            raise unittest.SkipTest("no Psy-Q SDK on disk (set PSYQ_SDK_DIRS)")
        cls.members = {}
        for lp in P.iter_libs(LIB_DIRS):
            ver = os.path.basename(os.path.dirname(os.path.dirname(lp)))
            for m in P.parse_lib(lp):
                cls.members[(ver, m.libname, m.name)] = m

    def test_vmanager_multi_chunk(self):
        """2.5 LIBSND VMANAGER: .text is many chunks; every reloc must land
        on a word of its type (507 of 1,181 did not before the fix)."""
        m = self.members[("2.5", "LIBSND.LIB", "VMANAGER")]
        t = m.obj.section_by_name(".text")
        self.assertEqual(len(t.data), 17528)
        checked = [r for r in t.relocs if r[1] in OPCLASS]
        self.assertEqual(len(checked), 1181)
        # relocations exist far beyond any single chunk's reach from 0
        self.assertGreater(max(off for off, _rt, _e in t.relocs), 0x4000)
        self.assertEqual(bad_relocs(t), [])
        offs = [off for off, _rt, _e in t.relocs]
        self.assertEqual(len(offs), len(set(offs)),
                         "two relocations on one word")

    def test_every_text_reloc_lands_on_its_opcode_class(self):
        tot = 0
        bad = []
        for key, m in sorted(self.members.items()):
            for s in m.obj.sections.values():
                if not s.name.startswith(".text"):
                    continue
                tot += sum(1 for r in s.relocs if r[1] in OPCLASS)
                b = bad_relocs(s)
                if b:
                    bad.append((key, s.name, len(b)))
        self.assertGreater(tot, 30000)
        self.assertEqual(bad, [])

    def test_every_reloc_in_bounds(self):
        for key, m in self.members.items():
            for s in m.obj.sections.values():
                for off, rt, _e in s.relocs:
                    self.assertLessEqual(off + 4, len(s.data), (key, s.name))


if __name__ == "__main__":
    unittest.main()
