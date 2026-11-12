#!/usr/bin/env python3
"""One-command conversion of ALL game assets into a local, gitignored
browseable tree: extracted_assets/  (PNGs, OBJs, SF2s, raw copies + manifest).

Wraps the existing single-format converters (audited 2026-08-15):
  pc/tools/tim_tool.py t2p     TIM -> PNG        (+ embedded-TIM carving here)
  pc/tools/tmd_to_obj.py       TMD -> OBJ (3D models)
  pc/tools/vab2sf2.py          VH+VB pair -> SF2 (instrument banks)
  convert_media.py            raw-disc XA -> correctly split playable WAVs

Sources: assets/extracted_iso/CDROM/** + assets/overlays/*.EXN +
disks/SLPS_000.65 (the last two are scanned for EMBEDDED TIMs -- PS1 games
routinely pack images inside code/data blobs).

Layout:
  extracted_assets/images/   <src>__<name>.png (+ carved __at_<off>.png)
  extracted_assets/models/   *.obj
  extracted_assets/audio/    *.sf2 instrument banks
  extracted_assets/xa_wav/   split PCM voice/streaming-audio clips
  extracted_assets/raw/      SEQ + BS + unconverted (BS frames need jpsxdec --
                             see tools/assets/jpsxdec.sh; noted in manifest)
  extracted_assets/manifest.json   everything found/converted/failed

Idempotent: existing outputs are skipped. Long runtime is fine.
Usage: .venv/bin/python3 tools/assets/convert_all.py [--only images|models|audio]

The extracted filesystem .XA files do not retain enough sector context to
split streams safely. Run `convert_media.py --only audio` against raw Track 1
for the VLC-compatible WAV set; do not concatenate XA streams.
"""
import json
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from repo_paths import ENGLISH, PC  # noqa: E402

PY = str(ROOT / ".venv/bin/python3")
DEST = ROOT / "extracted_assets"
CDROM = ROOT / "assets/extracted_iso/CDROM"
MAN = {"images": [], "models": [], "audio": [], "raw": [], "failed": []}


# The converters belong to the PC port (pc/tools) and the English patch
# (translation/tools); docs/REPO-SPLIT-PLAN.md.  Without that part its formats
# are skipped, and said so once.
_SKIPPED = set()


def part_tool(part, root, rel):
    if root is None:
        if rel not in _SKIPPED:
            print(f"SKIP {rel}: the {part} is not present (tools/repo_paths.py)")
            _SKIPPED.add(rel)
        return None
    return str(root / rel)


def run(cmd):
    if None in cmd:
        return False, "converter not present (see the SKIP line above)"
    r = subprocess.run(cmd, capture_output=True, text=True, cwd=ROOT)
    return r.returncode == 0, (r.stdout + r.stderr).strip()[-300:]


def uniq_name(src):
    rel = src.relative_to(ROOT)
    return str(rel).replace("/", "__")


def tim_sections_ok(data, off):
    """Validate a TIM at off; return total size or 0."""
    if data[off:off + 4] != b"\x10\x00\x00\x00":
        return 0
    flag = struct.unpack_from("<I", data, off + 4)[0]
    if flag not in (0x00, 0x01, 0x02, 0x03, 0x08, 0x09, 0x0A, 0x0B):
        return 0
    p = off + 8
    nsec = 2 if flag & 0x8 else 1
    for si in range(nsec):
        if p + 12 > len(data):
            return 0
        sz = struct.unpack_from("<I", data, p)[0]
        if sz < 12 or sz > 0x200000 or p + sz > len(data):
            return 0
        # dimension sanity: PS1 VRAM is 1024x512; a section's byte size must
        # equal 12 + w16*2*h exactly. Without this, code bytes carve as
        # "TIMs" with shapes like 2x8192 (measured 2026-08-15).
        w16, h = struct.unpack_from("<HH", data, p + 8)
        if not (1 <= w16 <= 512 and 1 <= h <= 512):
            return 0
        if sz != 12 + w16 * 2 * h:
            return 0
        p += sz
    return p - off


def convert_tim_bytes(data, off, size, tag, outdir):
    out = outdir / f"{tag}.png"
    if out.exists():
        return True
    with tempfile.NamedTemporaryFile(suffix=".tim", delete=False) as tf:
        tf.write(data[off:off + size])
        tmp = tf.name
    ok, msg = run([PY, part_tool("PC port", PC, "pc/tools/tim_tool.py"), "unpack", tmp, str(out)])
    Path(tmp).unlink()
    if ok and out.exists():
        MAN["images"].append(str(out.relative_to(ROOT)))
    else:
        MAN["failed"].append({"what": tag, "why": msg})
    return ok


def do_images():
    outdir = DEST / "images"
    outdir.mkdir(parents=True, exist_ok=True)
    # standalone .TIM files
    for f in sorted(CDROM.rglob("*.TIM")):
        d = f.read_bytes()
        sz = tim_sections_ok(d, 0)
        if sz:
            convert_tim_bytes(d, 0, sz, uniq_name(f), outdir)
    # embedded TIMs in containers, overlays, exe
    scan = [p for p in sorted(CDROM.rglob("*"))
            if p.is_file() and p.suffix in (".BIN", ".NBN", ".DAT", "")]
    scan += sorted((ROOT / "assets/overlays").glob("*.EXN"))
    exe = ROOT / "disks/SLPS_000.65"
    if exe.exists():
        scan.append(exe)
    for f in scan:
        d = f.read_bytes()
        base = uniq_name(f)
        i = 0
        found = 0
        while True:
            i = d.find(b"\x10\x00\x00\x00", i)
            if i < 0:
                break
            sz = tim_sections_ok(d, i)
            if sz:
                convert_tim_bytes(d, i, sz, f"{base}__at_{i:#x}", outdir)
                found += 1
                i += sz
            else:
                i += 4
        if found:
            print(f"{base}: {found} embedded TIMs", flush=True)


def do_models():
    outdir = DEST / "models"
    outdir.mkdir(parents=True, exist_ok=True)
    for f in sorted(CDROM.rglob("*.TMD")):
        out = outdir / (uniq_name(f) + ".obj")
        if out.exists():
            continue
        ok, msg = run([PY, part_tool("PC port", PC, "pc/tools/tmd_to_obj.py"), str(f), str(out)])
        (MAN["models"].append(str(out.relative_to(ROOT))) if ok and
         out.exists() else MAN["failed"].append(
             {"what": str(f.relative_to(ROOT)), "why": msg}))


def do_audio():
    outdir = DEST / "audio"
    outdir.mkdir(parents=True, exist_ok=True)
    for vh in sorted(CDROM.rglob("*.VH")):
        vb = vh.with_suffix(".VB")
        if not vb.exists():
            MAN["failed"].append({"what": str(vh.relative_to(ROOT)),
                                  "why": "no matching .VB"})
            continue
        out = outdir / (uniq_name(vh) + ".sf2")
        if out.exists():
            continue
        ok, msg = run([PY, part_tool("PC port", PC, "pc/tools/vab2sf2.py"), str(vh), str(vb),
                       "-o", str(out)])
        if not (ok and out.exists()):
            ok2, msg2 = run([PY, part_tool("PC port", PC, "pc/tools/vab2sf2.py"), str(vh), str(vb),
                             str(out)])
            ok, msg = ok2, msg + " | " + msg2
        (MAN["audio"].append(str(out.relative_to(ROOT))) if ok and
         out.exists() else MAN["failed"].append(
             {"what": str(vh.relative_to(ROOT)), "why": msg}))


def do_raw():
    outdir = DEST / "raw"
    outdir.mkdir(parents=True, exist_ok=True)
    for pat, note in (("*.SEQ", "PS1 MIDI-like sequence; pair with the .sf2 "
                       "from the same directory"),
                      ("*.BS", "PS1 bitstream frame; decode with "
                       "tools/assets/jpsxdec.sh")):
        for f in sorted(CDROM.rglob(pat)):
            out = outdir / uniq_name(f)
            if not out.exists():
                shutil.copyfile(f, out)
            MAN["raw"].append({"file": str(out.relative_to(ROOT)),
                               "note": note})


def do_nbn():
    """Konami NBN archives (raw VRAM data, no headers) via the heuristic
    extractor from the translation subproject."""
    outroot = DEST / "nbn"
    outroot.mkdir(parents=True, exist_ok=True)
    for f in sorted(CDROM.rglob("*.NBN")):
        out = outroot / uniq_name(f)
        if out.exists():
            MAN["raw"].append({"file": str(out.relative_to(ROOT)),
                               "note": "NBN heuristic candidates only"})
            continue
        ok, msg = run([PY, part_tool("English patch", ENGLISH, "translation/tools/nbn_extract.py"), str(f),
                       "-o", str(out)])
        if ok:
            MAN["raw"].append({"file": str(out.relative_to(ROOT)),
                               "note": "NBN heuristic candidates only"})
        else:
            MAN["failed"].append({"what": str(f.relative_to(ROOT)),
                                  "why": msg})


def main():
    only = None
    if "--only" in sys.argv:
        only = sys.argv[sys.argv.index("--only") + 1]
    DEST.mkdir(exist_ok=True)
    if only in (None, "nbn"):
        do_nbn()
    if only in (None, "images"):
        do_images()
    if only in (None, "models"):
        do_models()
    if only in (None, "audio"):
        do_audio()
    if only in (None, "raw"):
        do_raw()
    (DEST / "manifest.json").write_text(json.dumps(MAN, indent=1))
    print(json.dumps({k: len(v) for k, v in MAN.items()}, indent=1))
    return 0


if __name__ == "__main__":
    sys.exit(main())
