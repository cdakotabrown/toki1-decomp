# psxiso - PlayStation ISO Tools (dumpsxiso / mkpsxiso)

This directory contains two Linux binaries used for PlayStation disc image extraction and reconstruction:

- `dumpsxiso`: extract files from a PS1 disc image (supports 2352-byte/sector images) and optionally emit an `mkpsxiso` XML project.
- `mkpsxiso`: rebuild a disc image from an XML project.

These tools are useful when you need to inspect disc contents or rebuild a patched image.

## Contents

| File | Purpose |
|------|---------|
| `dumpsxiso` | Dump/extract ISO contents, optionally generate XML |
| `mkpsxiso` | Build ISO from XML |

## dumpsxiso

```
dumpsxiso [-h|--help] [-x <path>] [-s <path>.xml] <isofile>
```

Common options:

- `-x <path>` destination directory for extracted files
- `-s <path>.xml` write an `mkpsxiso`-compatible XML project file
- `-S|--sort-by-dir` group XML entries by directory
- `-e|--encode <codec>` encode CDDA audio (default `wave`; supports `wave`, `pcm`, `flac`)

Example:

```bash
mkdir -p /tmp/disc_dump
tools/psxiso/dumpsxiso -x /tmp/disc_dump -s /tmp/disc.xml disks/game.bin
```

## mkpsxiso

```
mkpsxiso [-h|--help] [-y] [-q|--quiet] [-o|--output <file>] [-lba <file>]
  [-lbahead <file>] [-rebuildxml <file>] [-noisogen] <xml>
```

Common options:

- `-o|--output <file>` output image file (overrides `image_name` in XML)
- `-c|--cuefile <file>` output cue sheet (overrides `cue_sheet` in XML)
- `-l|--label <id>` volume ID (overrides `<volume>`)
- `-y` always overwrite output files
- `-lba <file>` write a log of file LBAs
- `-lbahead <file>` write a C header of file LBAs

Example:

```bash
tools/psxiso/mkpsxiso -o rebuilt.bin /tmp/disc.xml
```

## Notes

- `dumpsxiso` produces XML that preserves disc file ordering; this is critical for reconstructing byte-identical images.
- These binaries are not wrappers around `genisoimage`; they are PS1-specific tools.


## Licence

mkpsxiso and dumpsxiso 2.10 are by Lameguy64 and contributors, unmodified Linux builds of <https://github.com/Lameguy64/mkpsxiso>, licensed GPL-2.0. The licence text is in `tools/licenses/GPL-2.0.txt`, and the corresponding source is listed in `tools/licenses/README.md`.
