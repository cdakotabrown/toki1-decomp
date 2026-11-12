#!/usr/bin/env python3
"""Self-test of overlay_data_from_source.py on IDO 5.2 fixtures.

Checks the IDO facts the layout model rests on (literals before jump tables,
no literal pooling, 4-byte datum alignment) and that a function whose
.rodata holds literals AND a jump table links with the two at independent
addresses, every .text reference resolving to the right one.
"""
import struct
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "overlay"))
import overlay_data_from_source as M  # noqa: E402

CC = ROOT / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
FIXTURE = r'''
extern void f(int, const char *);
void a(int x) {
    f(0, "ABCDEFGHI");
    switch (x) {
    case 0: f(1, 0); break; case 1: f(2, 0); break; case 2: f(3, 0); break;
    case 3: f(4, 0); break; case 4: f(5, 0); break; case 5: f(7, 0); break;
    }
    f(9, "ABCDEFGHI");
}
'''


def compile_fixture(td: Path) -> Path:
    src = td / "fx.c"
    src.write_text(FIXTURE)
    obj = td / "fx.o"
    subprocess.run([str(CC), "-O2", "-G0", "-mips1", "-EL", "-non_shared",
                    "-c", str(src), "-o", str(obj)], check=True, cwd=td)
    return obj


@unittest.skipUnless(CC.exists(), "IDO 5.2 recomp not built")
class DataFromSourceTest(unittest.TestCase):
    def test_pool_split_and_link(self):
        with tempfile.TemporaryDirectory() as td_s:
            td = Path(td_s)
            obj = M.Obj(compile_fixture(td))
            lit, jt_off, jt = M.literal_pool_size(obj)
            # two copies of the same literal (no pooling), each 12 bytes
            self.assertEqual(lit, 24)
            self.assertEqual(jt_off, 24)
            self.assertEqual(jt, 24)
            ro = obj.data(".rodata")
            self.assertEqual(ro[:10], b"ABCDEFGHI\0")
            self.assertEqual(ro[12:22], b"ABCDEFGHI\0")
            split = M.split_rodata(obj.path, td / "fx.split.o", jt_off)
            lit_base, jt_base, text = 0x80150000, 0x80160004, 0x80140000
            out, _ = M.link_at(split, "RENSYU", td, text, {".rodata": lit_base},
                               {M.JT_SYM: jt_base, "f": 0x80100000}, set())
            tx = out[".text"]
            words = [struct.unpack_from("<I", tx, i)[0]
                     for i in range(0, len(tx), 4)]
            his = {w & 0xFFFF for w in words if w >> 26 == 0x0F}   # lui
            los = {w & 0xFFFF for w in words
                   if w >> 26 in (0x09, 0x23)}                     # addiu/lw
            self.assertIn(0x8015, his)                 # literal pool
            self.assertIn(0x8016, his)                 # jump table
            self.assertIn(0x0000, los)                 # first literal
            self.assertIn(0x000C, los)                 # second literal
            self.assertIn(0x0004, los)                 # the jump table word
            jt_words = out[".rodata"][jt_off:jt_off + jt]
            for i in range(0, jt, 4):
                tgt = struct.unpack_from("<I", jt_words, i)[0]
                self.assertTrue(text <= tgt < text + len(tx))


if __name__ == "__main__":
    unittest.main()
