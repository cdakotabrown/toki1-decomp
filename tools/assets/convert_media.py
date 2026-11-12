#!/usr/bin/env python3
"""Convert the remaining PS1-native media into everyday formats.

  STR movies -> .mp4 via ffmpeg psxstr WHEN the .STR retains its XA
                subheaders. NOTE (measured 2026-08-15): the copies in
                assets/extracted_iso/ were extracted as 2048-byte user data
                (subheaders stripped), which ffmpeg cannot parse -- those are
                recorded as needs-jpsxdec in the manifest. Decode them with
                tools/assets/jpsxdec.sh against the raw Track-1 .bin. This
                script does that automatically when the disc image exists.
  BS frames  -> .png via jpsxdec using the dimensions supplied by the game's
                MDEC call sites. BS files do not carry width/height metadata:
                C_* cut-ins are 320x88, ordinary scene/prologue stills are
                320x160, and X_*/title/name-entry/TACO assets are 320x240.
  XA audio   -> individual .wav files via jpsxdec against raw Track 1. The
                filesystem copies lack the sector/file/channel boundaries
                needed to split the thousands of voice clips correctly.
  SEQ music  -> .mid for standard Sony SEQp files. NOTE (measured
                2026-08-15): this game's .SEQ use a KONAMI-CUSTOM format
                (magic 0x40000000, not 'pQES') -- those are copied to music/
                with a needs-reversing note instead. Reversing the format is
                a roadmap item; the PS1 SsSeq player code in the decomp is
                the reference.
  SEQ music  -> .wav via fluidsynth + the matching .sf2 (only if fluidsynth
                is installed; skipped silently otherwise)

Outputs into extracted_assets/{video,stills,xa_wav,music}/ (gitignored).
Usage: .venv/bin/python3 tools/assets/convert_media.py [--only video|audio|music]
"""
import json
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
DEST = ROOT / "extracted_assets"
CDROM = ROOT / "assets/extracted_iso/CDROM"
ISO = ROOT / "assets/extracted_iso"
JPSXDEC = ROOT / "tools/assets/jpsxdec.sh"
JPSX_INDEX = ROOT / "assets/jpsxdec_index.idx"
DISC_GLOB = "*Track 1*.bin"
MAN = {"video": [], "audio": [], "music": [], "notes": [], "failed": []}


def run(cmd, cwd=None):
    r = subprocess.run(cmd, capture_output=True, text=True, cwd=cwd)
    return r.returncode == 0, (r.stdout + r.stderr).strip()[-200:]


def uniq(p):
    return str(p.relative_to(ROOT)).replace("/", "__")


def record(kind, path):
    rel = str(path.relative_to(ROOT))
    if rel not in MAN[kind]:
        MAN[kind].append(rel)


def find_disc():
    discs = sorted((ROOT / "disks").glob(DISC_GLOB))
    return discs[0] if discs else None


def do_jpsxdec_video(out):
    """Decode XA-sector STR movies from raw Track 1."""
    disc = find_disc()
    if not JPSXDEC.exists() or disc is None:
        MAN["failed"].append({
            "what": "raw-disc STR video",
            "why": "jpsxdec wrapper or raw Track-1 .bin is unavailable",
        })
        return
    existing = sorted(out.glob("*.avi"))
    if not existing:
        if not JPSX_INDEX.exists():
            ok, msg = run([str(JPSXDEC), "-f", str(disc), "-x",
                           str(JPSX_INDEX)])
            if not ok:
                MAN["failed"].append({"what": "jpsxdec disc index",
                                      "why": msg})
                return
        ok, msg = run([str(JPSXDEC), "-x", str(JPSX_INDEX),
                       "-a", "video", "-dir", str(out),
                       "-vidfmt", "avi:mjpg"])
        if not ok:
            MAN["failed"].append({"what": "raw-disc STR video",
                                  "why": msg})
    for path in sorted(out.glob("*.avi")):
        record("video", path)


def do_bs_stills():
    """Decode standalone MDEC bitstreams with game-derived dimensions."""
    out = DEST / "stills"
    out.mkdir(parents=True, exist_ok=True)
    if not JPSXDEC.exists():
        MAN["failed"].append({"what": "BS stills",
                              "why": "jpsxdec wrapper is unavailable"})
        return
    for src in sorted(CDROM.rglob("*.BS")):
        dst = out / (uniq(src) + ".png")
        rel_parts = src.relative_to(CDROM).parts
        # The BS header has no dimensions. These families come directly from
        # decdct call sites: it drains one 16px-wide MDEC strip at a
        # time into a caller-supplied width/height rectangle.
        if (rel_parts[0] in {"FUKUTEST", "NAME_ENT", "TACO"} or
                (rel_parts[0] == "BS" and src.name.startswith("X_"))):
            dim = "320x240"
        elif rel_parts[0] == "BS" and src.name.startswith("C_"):
            dim = "320x88"
        else:
            dim = "320x160"
        if not dst.exists():
            # jpsxdec chooses the output name beside its input. Decode in a
            # temporary directory so source assets and logs remain untouched.
            with tempfile.TemporaryDirectory(prefix="toki-bs-") as td:
                tmp = Path(td) / src.name
                shutil.copyfile(src, tmp)
                ok, msg = run([str(JPSXDEC), "-f", str(tmp), "-static",
                               "bs", "-dim", dim, "-fmt", "png"],
                              cwd=td)
                made = tmp.with_suffix(".png")
                if ok and made.exists():
                    shutil.move(str(made), dst)
                else:
                    MAN["failed"].append({"what": uniq(src), "why": msg})
                    continue
        record("video", dst)
    MAN["notes"].append({
        "what": "BS dimensions",
        "note": "BS carries no dimensions. C_* cut-ins were decoded at "
                "320x88; other CDROM/BS and PROLODIR images at 320x160; "
                "X_*, FUKUTEST, NAME_ENT, and TACO at 320x240, matching the "
                "game's decdct MDEC call sites.",
    })


def ensure_jpsxdec_index():
    """Return whether a usable raw-disc index exists, creating it if needed."""
    if JPSX_INDEX.exists():
        return True
    disc = find_disc()
    if not JPSXDEC.exists() or disc is None:
        MAN["failed"].append({
            "what": "jpsxdec disc index",
            "why": "jpsxdec wrapper or raw Track-1 .bin is unavailable",
        })
        return False
    ok, msg = run([str(JPSXDEC), "-f", str(disc), "-x", str(JPSX_INDEX)])
    if not ok:
        MAN["failed"].append({"what": "jpsxdec disc index", "why": msg})
    return ok


def do_audio():
    """Split XA file/channel streams into ordinary PCM WAV files."""
    out = DEST / "xa_wav"
    out.mkdir(parents=True, exist_ok=True)
    wavs = sorted(out.rglob("*.wav"))
    if not wavs and ensure_jpsxdec_index():
        ok, msg = run([str(JPSXDEC), "-x", str(JPSX_INDEX),
                       "-a", "audio", "-dir", str(out)])
        if not ok:
            MAN["failed"].append({"what": "raw-disc XA audio", "why": msg})
        wavs = sorted(out.rglob("*.wav"))

    by_source = {}
    total_bytes = 0
    for path in wavs:
        # jpsxdec names streams SOURCE.XA[index].wav.
        source = path.name.split("[", 1)[0]
        by_source[source] = by_source.get(source, 0) + 1
        total_bytes += path.stat().st_size
    report = {
        "format": "RIFF/WAVE PCM decoded from raw-disc XA sectors by jpsxdec",
        "count": len(wavs),
        "total_bytes": total_bytes,
        "by_source": dict(sorted(by_source.items())),
        "note": "Each WAV is one indexed XA file/channel stream; SF2 files "
                "are instrument banks and SEQ files remain proprietary.",
    }
    report_path = DEST / "xa_manifest.json"
    report_path.write_text(json.dumps(report, indent=1) + "\n")
    record("audio", report_path)
    if wavs:
        MAN["notes"].append({
            "what": "XA audio",
            "note": f"{len(wavs)} VLC-compatible PCM WAV streams are under "
                    "extracted_assets/xa_wav; see xa_manifest.json.",
        })


def do_video():
    out = DEST / "video"
    out.mkdir(parents=True, exist_ok=True)
    strs = sorted(ISO.rglob("*.STR")) + sorted(CDROM.rglob("*.str"))
    for f in strs:
        dst = out / (uniq(f) + ".mp4")
        if dst.exists():
            record("video", dst)
            continue
        ok, msg = run(["ffmpeg", "-y", "-f", "psxstr", "-i", str(f),
                       "-c:v", "libx264", "-pix_fmt", "yuv420p",
                       "-c:a", "aac", str(dst)])
        if not ok:  # some STRs are audio-only or video-only
            ok, msg2 = run(["ffmpeg", "-y", "-f", "psxstr", "-i", str(f),
                            "-c:v", "libx264", "-pix_fmt", "yuv420p",
                            "-an", str(dst)])
            msg += " | " + msg2
        if ok and dst.exists():
            record("video", dst)
        else:
            MAN["failed"].append({"what": uniq(f), "why":
                "no XA subheaders (2048-byte extraction) -- decode from the "
                "raw Track-1 .bin with tools/assets/jpsxdec.sh"})
        print(f"video: {f.name} {'ok' if ok else 'FAIL'}", flush=True)
    do_jpsxdec_video(out)
    do_bs_stills()


def seq_to_mid(seq: bytes):
    """SEQp -> SMF0. Header: magic 'pQES', u32 version, u16 PPQN (BE),
    u24 tempo (BE, usec/quarter), u16 rhythm; events are SMF running-status
    deltas already."""
    if seq[:4] not in (b"pQES", b"SEQp"):
        raise ValueError("not a SEQp file")
    ppqn = struct.unpack(">H", seq[8:10])[0]
    tempo = int.from_bytes(seq[10:13], "big")
    events = seq[15:]
    track = bytearray()
    track += b"\x00\xff\x51\x03" + tempo.to_bytes(3, "big")
    track += events
    if not track.endswith(b"\xff\x2f\x00"):
        track += b"\x00\xff\x2f\x00"
    out = bytearray()
    out += b"MThd" + struct.pack(">IHHH", 6, 0, 1, ppqn)
    out += b"MTrk" + struct.pack(">I", len(track)) + track
    return bytes(out)


def do_music():
    out = DEST / "music"
    out.mkdir(parents=True, exist_ok=True)
    have_fs = shutil.which("fluidsynth") is not None
    for f in sorted(CDROM.rglob("*.SEQ")):
        mid = out / (uniq(f) + ".mid")
        if not mid.exists():
            data = f.read_bytes()
            try:
                mid.write_bytes(seq_to_mid(data))
                MAN["music"].append(str(mid.relative_to(ROOT)))
            except Exception:
                dst = out / uniq(f)
                if not dst.exists():
                    import shutil as _sh
                    _sh.copyfile(f, dst)
                MAN["failed"].append({"what": uniq(f), "why":
                    "Konami-custom sequence (magic %s) -- format reversal is "
                    "a roadmap item; raw copy kept" % data[:4].hex()})
                continue
        record("music", mid)
        # optional audible render with the sibling soundfont
        if have_fs:
            wav = out / (uniq(f) + ".wav")
            if wav.exists():
                continue
            sf2s = sorted((DEST / "audio").glob(
                "*" + f.parent.name + "*" + ".sf2")) or \
                sorted((DEST / "audio").glob("*.sf2"))
            if sf2s:
                ok, msg = run(["fluidsynth", "-ni", "-F", str(wav),
                               "-r", "44100", str(sf2s[0]), str(mid)])
                if ok and wav.exists():
                    record("music", wav)
    print(f"music: {len(MAN['music'])} outputs "
          f"(fluidsynth={'yes' if have_fs else 'no - .mid only'})")


def main():
    only = None
    if "--only" in sys.argv:
        only = sys.argv[sys.argv.index("--only") + 1]
    manifest_path = DEST / "media_manifest.json"
    if manifest_path.exists():
        try:
            previous = json.loads(manifest_path.read_text())
            for kind in MAN:
                if isinstance(previous.get(kind), list):
                    MAN[kind].extend(previous[kind])
        except (OSError, ValueError):
            pass
    if only in (None, "video"):
        do_video()
    if only in (None, "audio"):
        do_audio()
    if only in (None, "music"):
        do_music()
    # Category-only reruns retain other inventories. De-duplicate scalar paths
    # and structured notes/failures so repeated idempotent runs stay stable.
    for kind in ("video", "audio", "music"):
        MAN[kind] = sorted(set(MAN[kind]))
    for kind in ("notes", "failed"):
        seen = set()
        unique = []
        for item in MAN[kind]:
            key = json.dumps(item, sort_keys=True)
            if key not in seen:
                seen.add(key)
                unique.append(item)
        MAN[kind] = unique
    manifest_path.write_text(json.dumps(MAN, indent=1) + "\n")
    print(json.dumps({k: len(v) for k, v in MAN.items()}, indent=1))
    return 0


if __name__ == "__main__":
    sys.exit(main())
