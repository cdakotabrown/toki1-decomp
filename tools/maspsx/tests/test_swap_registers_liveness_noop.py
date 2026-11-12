"""tools/ido/swap_registers_liveness.py must always write its output.

The build driver advances a PostProc chain to the step's output file
whenever the step exits 0.  When the tool found no safe rename it used to
exit 0 WITHOUT writing the output, so the next step failed on a missing
file and the whole compile aborted (func_80074E98: "objcopy:
func_step2.o: No such file", reported as a strip_trailing_instructions
failure).  With no rename the output is the input, unchanged."""
import os
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest

TOOL = os.path.normpath(os.path.join(
    os.path.dirname(os.path.abspath(__file__)), "..", "..", "ido",
    "swap_registers_liveness.py"))
AS = shutil.which("mipsel-linux-gnu-as")

# addiu v0,zero,1 ; jr ra ; nop
WORDS = (0x24020001, 0x03E00008, 0x00000000)


@unittest.skipIf(AS is None, "mipsel-linux-gnu-as not installed")
class TestSwapRegistersLivenessNoop(unittest.TestCase):
    def test_no_rename_still_writes_output(self):
        with tempfile.TemporaryDirectory() as d:
            src = os.path.join(d, "f.s")
            obj = os.path.join(d, "f.o")
            out = os.path.join(d, "out.o")
            exe = os.path.join(d, "exe.bin")
            with open(src, "w") as f:
                f.write(".set noreorder\n.text\n.globl f\nf:\n"
                        "addiu $2,$0,1\njr $31\nnop\n")
            subprocess.run([AS, "-EL", "-march=r3000", "-o", obj, src],
                           check=True, capture_output=True)
            # fake target image: the same three words at 0x80041000
            with open(exe, "wb") as f:
                f.write(b"\0" * 0x800 + b"".join(struct.pack("<I", w) for w in WORDS))
            r = subprocess.run(
                [sys.executable, TOOL, obj, out, "--target-exe", exe,
                 "--address", "0x80041000"],
                capture_output=True, text=True)
            self.assertEqual(0, r.returncode, r.stderr)
            self.assertIn("No safe renames", r.stdout)
            self.assertTrue(os.path.isfile(out), "no-op step wrote no output")
            with open(obj, "rb") as a, open(out, "rb") as b:
                self.assertEqual(a.read(), b.read())


if __name__ == "__main__":
    unittest.main()
