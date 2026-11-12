#!/usr/bin/env python3
"""build_clean_iso.py - Assemble a byte-identical game disc from the clean build.

Pipeline (spec: docs/CLEAN-DISK-BUILD-SPEC.md "Disk assembly"):
  1. Stage the extracted ISO tree (gitignored assets/extracted_iso) into
     build/clean_iso/staging via symlinks, OVERRIDING:
       - all 26 CDROM/EXEDIR/<OV>.EXN with the clean-built EXNs
         (build/overlay/<OV>/<OV>.clean.EXN) — each must have passed the
         whole-file gate (SHA1 vs extraction) or the stage refuses it;
       - PSX.EXE with the built main executable (build/SLPS_000.65.new)
         when present AND SHA1-verified; otherwise the extraction's own
         PSX.EXE is used and reported as debt.
  2. mkpsxiso with the PRISTINE dumpsxiso layout.xml (LBA layout preserved).
  3. Metadata reconciliation (see below), then gate: byte compare of the
     track 1 / track 2 regions against the user's original dump in disks/.

## Why reconciliation exists (measured, not hand-waved)

mkpsxiso 2.10 reproduces 298,511 of 298,514 track-1 sectors bit-exactly.
The remaining THREE sectors are volume metadata it cannot express:

  * sector 22 (root directory) and sector 14515 (CDROM directory): the XA
    attribute field of the `.` / `..` self-records. The original master
    gives the root's own records perms 0x000 (attr 0x8800) while every
    other record gets 0x555 (0x8D55); mkpsxiso derives `.`/`..` attrs
    internally and no XML attribute reaches them (verified empirically:
    default_attributes / directory_tree / per-dir xa_perm all leave these
    three records at 0x8D55, and per-dir overrides corrupt OTHER records).
  * sector 298513 (last sector of the track-1 postgap): all-zero data but
    ~24 junk bytes in the ECC area — mastering leftovers, not any standard
    encoding; mkpsxiso emits a fully zero dummy sector.

Reconciliation copies ONLY those sectors from the user's own original dump
(disks/, gitignored) after PROVING the divergence is confined to the classes
above: for directory sectors, every differing payload byte must lie in a
`.`/`..` XA attribute field (or the EDC/ECC derived from it); for the
postgap sector, the data area must be all-zero on both sides. Any other
difference refuses to reconcile and fails the gate. Game content can never
pass through this path.

Usage:
    python3 tools/iso/build_clean_iso.py [--verify] [--allow-original-exe]
"""
import hashlib
import json
import re
import shutil
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).parent.parent.parent

EXTRACTED = ROOT / "assets" / "extracted_iso"
DISKS = ROOT / "disks"
MKPSXISO = ROOT / "tools" / "psxiso" / "mkpsxiso"
STAGING = ROOT / "build" / "clean_iso" / "staging"
OUT_DIR = ROOT / "build" / "clean_iso"
OVERLAY_BUILD = ROOT / "build" / "overlay"
ASSETS_OVERLAYS = ROOT / "assets" / "overlays"
# Prefer the stage-4 clean main build; fall back to the legacy level3 output.
_MAIN_CANDIDATES = (ROOT / "build" / "SLPS_000.65.clean",
                    ROOT / "build" / "SLPS_000.65.new")
MAIN_BUILT = next((p for p in _MAIN_CANDIDATES if p.exists()),
                  _MAIN_CANDIDATES[0])
DEBT_FILE = ROOT / "config" / "clean_build_debt.json"

TRACK1 = DISKS / ("Tokimeki Memorial - Forever with You (Japan) (Rev 1) "
                  "(Track 1).bin")
TRACK2 = DISKS / ("Tokimeki Memorial - Forever with You (Japan) (Rev 1) "
                  "(Track 2).bin")

SEC = 2352


def sha1(p: Path) -> str:
    h = hashlib.sha1()
    with open(p, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def stage(allow_original_exe: bool) -> dict:
    """Build the staging tree. Returns provenance dict."""
    prov = {"exns_clean": [], "exns_refused": [], "exe": None}
    if STAGING.exists():
        shutil.rmtree(STAGING)
    STAGING.mkdir(parents=True)

    # symlink top-level entries from the extraction
    for entry in EXTRACTED.iterdir():
        if entry.name in ("layout.xml", "README.md"):
            continue
        (STAGING / entry.name).symlink_to(entry)

    # CDROM must be a real dir so we can override EXEDIR
    (STAGING / "CDROM").unlink()
    (STAGING / "CDROM").mkdir()
    for entry in (EXTRACTED / "CDROM").iterdir():
        (STAGING / "CDROM" / entry.name).symlink_to(entry)
    (STAGING / "CDROM" / "EXEDIR").unlink()
    exedir = STAGING / "CDROM" / "EXEDIR"
    exedir.mkdir()
    for entry in (EXTRACTED / "CDROM" / "EXEDIR").iterdir():
        (exedir / entry.name).symlink_to(entry)

    # override EXNs with clean builds (only if byte-identical to extraction —
    # the whole point is that they ARE; a non-verified clean EXN is refused).
    # Copy (not symlink) and stamp the ORIGINAL's mtime: mkpsxiso takes the
    # directory-record recording dates from source mtimes, and a fresh
    # build-output mtime would rewrite ~30 directory sectors to today.
    import os
    for exn in sorted(ASSETS_OVERLAYS.glob("*.EXN")):
        ov = exn.stem
        clean = OVERLAY_BUILD / ov / f"{ov}.clean.EXN"
        target = exedir / f"{ov}.EXN"
        iso_orig = EXTRACTED / "CDROM" / "EXEDIR" / f"{ov}.EXN"
        if clean.exists() and sha1(clean) == sha1(exn):
            target.unlink()
            shutil.copy(clean, target)
            st = iso_orig.stat()
            os.utime(target, (st.st_atime, st.st_mtime))
            prov["exns_clean"].append(ov)
        else:
            prov["exns_refused"].append(ov)

    # Track 2 audio: dumpsxiso's TR02.WAV extraction is SILENT (it cannot
    # rip CDDA), so the staged WAV is regenerated from the user's own
    # Track 2 dump (audio-asset tier: gitignored, never committed): WAV
    # header from the extraction stub + PCM = dump minus its 150-sector
    # silent pregap (mkpsxiso re-emits the pregap from the cue INDEX gap).
    orig_dadir = EXTRACTED / "CDROM" / "DADIR"
    if TRACK2.exists() and (orig_dadir / "TR02.WAV").exists():
        dadir = STAGING / "CDROM" / "DADIR"
        dadir.unlink()
        dadir.mkdir()
        stub = (orig_dadir / "TR02.WAV").read_bytes()
        t2 = TRACK2.read_bytes()
        pcm = t2[150 * SEC:]
        if not any(t2[:150 * SEC]) and len(pcm) == len(stub) - 44:
            (dadir / "TR02.WAV").write_bytes(stub[:44] + pcm)
            st = (orig_dadir / "TR02.WAV").stat()
            os.utime(dadir / "TR02.WAV", (st.st_atime, st.st_mtime))
            st = orig_dadir.stat()
            os.utime(dadir, (st.st_atime, st.st_mtime))
            prov["track2_audio"] = "regenerated from dump (extraction " \
                                   "WAV is silent)"
        else:
            dadir.rmdir()
            dadir.symlink_to(orig_dadir)
            prov["track2_audio"] = "extraction stub (dump shape unexpected)"

    # override PSX.EXE with the built main binary when verified
    orig_exe = EXTRACTED / "PSX.EXE"
    if MAIN_BUILT.exists() and sha1(MAIN_BUILT) == sha1(orig_exe):
        (STAGING / "PSX.EXE").unlink()
        shutil.copy(MAIN_BUILT, STAGING / "PSX.EXE")
        st = orig_exe.stat()
        os.utime(STAGING / "PSX.EXE", (st.st_atime, st.st_mtime))
        prov["exe"] = f"built ({MAIN_BUILT.relative_to(ROOT)}, SHA1-verified)"
    elif allow_original_exe:
        prov["exe"] = "EXTRACTION FALLBACK (built exe missing/unverified)"
    else:
        prov["exe"] = "missing"
        raise SystemExit(
            "[clean-iso] build/SLPS_000.65.new missing or SHA1-mismatched; "
            "run tools/build/build_level3_c.py first or pass "
            "--allow-original-exe to stage the extraction's PSX.EXE")

    shutil.copy(EXTRACTED / "layout.xml", STAGING / "layout.xml")

    # LAST (after every tree mutation): stamp the staged real directories
    # with the original mtimes — mkpsxiso derives directory-record recording
    # dates from source mtimes, and mkdir/copy inside a dir refreshes it.
    for staged, orig in ((STAGING / "CDROM", EXTRACTED / "CDROM"),
                         (STAGING / "CDROM" / "EXEDIR",
                          EXTRACTED / "CDROM" / "EXEDIR"),
                         (STAGING / "CDROM" / "DADIR",
                          EXTRACTED / "CDROM" / "DADIR"),
                         (STAGING, EXTRACTED)):
        if staged.is_dir() and not staged.is_symlink():
            st = orig.stat()
            os.utime(staged, (st.st_atime, st.st_mtime))
    return prov


def run_mkpsxiso() -> Path:
    out_bin = OUT_DIR / "clean_disc.bin"
    out_cue = OUT_DIR / "clean_disc.cue"
    import os
    mkp = MKPSXISO
    if not os.access(mkp, os.X_OK):
        # the tracked binary is mode 100644; run an executable copy
        mkp = OUT_DIR / "mkpsxiso"
        if not mkp.exists() or mkp.stat().st_size != MKPSXISO.stat().st_size:
            shutil.copy(MKPSXISO, mkp)
            mkp.chmod(0o755)
    r = subprocess.run([str(mkp), "-y", "-q",
                        "-o", str(out_bin), "-c", str(out_cue),
                        str(STAGING / "layout.xml")],
                       capture_output=True, text=True)
    if r.returncode != 0:
        raise SystemExit(f"[clean-iso] mkpsxiso failed: {r.stderr[-500:]}")
    return out_bin


# ---------------------------------------------------------------------------
# Metadata reconciliation (narrow, verified)
# ---------------------------------------------------------------------------

def _dir_record_dotdot_xa_fields(payload: bytes):
    """Yield payload offsets of the XA attribute field for `.`/`..` records
    at the START of a directory sector. Record layout: 33-byte base + 1-byte
    name (0x00/0x01) + XA system-use (owner 4B at +34, attr 2B at +38,
    'XA' sig at +40)."""
    off = 0
    for _ in range(2):  # "." then ".."
        if off + 42 > len(payload):
            return
        rec_len = payload[off]
        if rec_len < 34 or payload[off + 32] != 1 or \
                payload[off + 33] not in (0, 1):
            return
        if payload[off + 40:off + 42] == b"XA":
            yield off + 38
        off += rec_len


def reconcile(built: Path, orig_track1: Path) -> dict:
    """Copy mkpsxiso-inexpressible metadata sectors from the original dump,
    PROVING each divergence is confined to the allowed classes first."""
    res = {"reconciled": [], "refused": []}
    n1 = orig_track1.stat().st_size // SEC
    with open(built, "r+b") as fb, open(orig_track1, "rb") as fo:
        for s in range(n1):
            fo.seek(s * SEC)
            fb.seek(s * SEC)
            a = fo.read(SEC)
            b = fb.read(SEC)
            if a == b:
                continue
            payload_a, payload_b = a[24:2072], b[24:2072]
            ok = False
            if payload_a == payload_b:
                # EDC/ECC-only difference is derived data — never allowed
                # alone (it would mean an encoding bug), except the postgap
                # junk class below.
                ok = not any(payload_a)  # all-zero postgap sector
                kind = "postgap_ecc_junk"
            elif not any(payload_a) and not any(payload_b):
                ok = True
                kind = "postgap_ecc_junk"
            else:
                # directory-sector class: every payload diff must be inside
                # a ./.. XA attribute field
                xa_offs = set()
                for o in _dir_record_dotdot_xa_fields(payload_a):
                    xa_offs.update((o, o + 1))
                diffs = {i for i in range(2048)
                         if payload_a[i] != payload_b[i]}
                ok = bool(diffs) and diffs <= xa_offs
                kind = "dotdot_xa_attr"
            if ok:
                fb.seek(s * SEC)
                fb.write(a)
                res["reconciled"].append({"sector": s, "class": kind})
            else:
                res["refused"].append({"sector": s})
    return res


def verify(built: Path) -> dict:
    """Byte-compare the built image's track regions against the dump."""
    res = {}
    t1_size = TRACK1.stat().st_size
    t2_size = TRACK2.stat().st_size
    h1 = hashlib.sha1()
    h2 = hashlib.sha1()
    with open(built, "rb") as f:
        remaining = t1_size
        while remaining:
            chunk = f.read(min(1 << 20, remaining))
            h1.update(chunk)
            remaining -= len(chunk)
        remaining = t2_size
        while remaining:
            chunk = f.read(min(1 << 20, remaining))
            if not chunk:
                break
            h2.update(chunk)
            remaining -= len(chunk)
        extra = len(f.read())
    res["track1_sha1_built"] = h1.hexdigest()
    res["track1_sha1_original"] = sha1(TRACK1)
    res["track1_match"] = res["track1_sha1_built"] == \
        res["track1_sha1_original"]
    res["track2_sha1_built"] = h2.hexdigest()
    res["track2_sha1_original"] = sha1(TRACK2)
    res["track2_match"] = res["track2_sha1_built"] == \
        res["track2_sha1_original"]
    res["trailing_extra_bytes"] = extra
    return res


def main():
    import argparse
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--verify", action="store_true",
                    help="byte-compare track regions vs the disks/ dump")
    ap.add_argument("--allow-original-exe", action="store_true",
                    help="stage the extraction's PSX.EXE if the built main "
                         "binary is missing (reported as debt)")
    args = ap.parse_args()

    for req in (EXTRACTED / "layout.xml", MKPSXISO, TRACK1, TRACK2):
        if not req.exists():
            raise SystemExit(f"[clean-iso] missing input: {req}")

    print("[clean-iso] staging...")
    prov = stage(args.allow_original_exe)
    print(f"  EXNs from clean build: {len(prov['exns_clean'])}/26"
          + (f"; REFUSED (unverified): {prov['exns_refused']}"
             if prov["exns_refused"] else ""))
    print(f"  PSX.EXE: {prov['exe']}")

    print("[clean-iso] mkpsxiso...")
    built = run_mkpsxiso()

    print("[clean-iso] metadata reconciliation...")
    rec = reconcile(built, TRACK1)
    for r in rec["reconciled"]:
        print(f"  sector {r['sector']}: {r['class']} (copied from dump, "
              f"class-verified)")
    for r in rec["refused"]:
        print(f"  sector {r['sector']}: REFUSED — divergence outside "
              f"allowed metadata classes")

    result = {"provenance": prov, "reconciliation": rec}
    if args.verify:
        print("[clean-iso] verifying track regions vs dump...")
        v = verify(built)
        result["verify"] = v
        ok = v["track1_match"] and v["track2_match"] \
            and v["trailing_extra_bytes"] == 0
        print(f"  track1: {'MATCH' if v['track1_match'] else 'MISMATCH'} "
              f"({v['track1_sha1_built']})")
        print(f"  track2: {'MATCH' if v['track2_match'] else 'MISMATCH'} "
              f"({v['track2_sha1_built']})")
        if v["trailing_extra_bytes"]:
            print(f"  trailing extra bytes: {v['trailing_extra_bytes']}")
        print(f"[clean-iso] {'DISC BYTE-PERFECT' if ok else 'DISC DIVERGES'}"
              f" -> {built}")
    else:
        ok = not rec["refused"]
        print(f"[clean-iso] built {built}")

    # record into the debt file under "_iso"
    db = {}
    if DEBT_FILE.exists():
        try:
            db = json.load(open(DEBT_FILE))
        except Exception:
            db = {}
    db["_iso"] = result
    DEBT_FILE.write_text(json.dumps(db, indent=2, sort_keys=True) + "\n")
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
