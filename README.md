# toki1-decomp

A byte-exact decompilation of **Tokimeki Memorial: Forever with You**
(Konami, 1995, PlayStation, SLPS-00065). The build reproduces the main
executable and all 26 overlays byte for byte, from C except for 180
functions that are assembly by origin.

Unofficial fan project, not affiliated with Konami or Sony. No game data is
included: you supply the disc.

## What matching means here

- **7,486 functions are C**: 6,111 in the overlays and 1,375 source files
  in the executable. **180 are assembly**: 7 in the overlays (Sony's
  LIBGTE `RotMatrixX/Y/Z`, linked into two overlays, and a BIOS stub) and
  173 in the executable (Sony assembly members, BIOS thunks, GTE
  routines), extracted from your disc at build time.
- **4,048 of the 7,486 (54%) match with the stock compiler and no
  post-processing.** 2,899 reach the retail bytes only after `@PostProc`
  directives (register renaming, instruction reordering, stack-slot
  remapping) on the compiled object, and 1,223 are compiled with
  `@CompileEnv` switches: env-gated hooks in this project's patched
  recompilation of IDO 5.2 that steer the optimizer's choices for that
  file (684 use both).
- For every C function, `tools/analysis/prove_equivalence.py` proves that
  the shipped object computes the same as the compile of its C without the
  `@PostProc` steps (with its `@CompileEnv`, if any): 7,486 / 7,486
  PROVEN. Directives that write retail instruction words or change an
  opcode or a branch condition are refused by the build.

## Disc

Japan, Rev 1, raw BIN/CUE. Put the three files in `disks/` (copies or
symlinks) under these names:

    disks/Tokimeki Memorial - Forever with You (Japan) (Rev 1).cue
    disks/Tokimeki Memorial - Forever with You (Japan) (Rev 1) (Track 1).bin
    disks/Tokimeki Memorial - Forever with You (Japan) (Rev 1) (Track 2).bin

| file | size | SHA-1 |
|---|---:|---|
| Track 1 | 702,104,928 | `7b2f72d1319db27d0700070aa0bffe886b5ef7a4` |
| Track 2 | 45,151,344 | `6cdd8500168ab4ecb18b78ef7589586d26bc4501` |
| `SLPS_000.65` (inside Track 1) | 690,176 | `2d8d474816e4bf937b09dc3b017f550f7ce4e14b` |

Any other release or a bad dump fails the first step.

## Build and verify

Linux or WSL2 with Python 3.10+, `build-essential` and
`binutils-mipsel-linux-gnu`. About ten minutes.

```bash
make setup                   # venv, disc check, extraction
make setup-ido52             # the IDO 5.2 compiler
make asm-overlays asm-main   # listings from your disc
make check-raw               # assemble: prints the retail SHA-1
make build-c                 # compile the C: the same SHA-1
make check-exn-all check-clean-main   # every overlay and the executable from source
.venv/bin/python3 tools/analysis/prove_equivalence.py --jobs $(nproc)   # RESULT: N / N PROVEN
```

Rules for changing the C, and the other gates: `CLAUDE.md`.

## Layout

| path | what |
|---|---|
| `src/main/matched/` | the executable's C, one file per function |
| `src/overlay/<OV>/` | each overlay's C |
| `include/` | headers |
| `asm/overlay/` | the few windows kept as assembly |
| `config/` | function registry, symbol maps, names |
| `tools/` | compilers, build, gates |
| `docs/reference/` | proof model, compiler hooks, directives, matching tricks, naming, game architecture |

`toki1-english` (the English patch) and `toki1-pc` (the PC port) build on
this repository and expect it beside them as `../toki1-decomp`.

## Licence

MIT (`LICENSE`). Third-party components, including the compilers, keep
their own terms: `THIRD-PARTY.md`.
