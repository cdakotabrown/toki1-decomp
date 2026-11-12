# `asm/overlay/`: per-function disassembly of the 26 overlay EXNs

Each `asm/overlay/<OV>/<symbol>.s` is one function of one overlay, disassembled
from `assets/overlays/<OV>.EXN` -- which is the player's own disc. They are the
asm side of the overlay build (`tools/overlay/build_clean_exn.py` composes the
26 clean EXNs from `src/overlay/<OV>/*.c` plus these listings) and the compare
target for every overlay gate.

## Generated, not tracked

Konami's code is not in this repository. Almost every file here is produced
from the disc by one tracked tool:

```bash
make asm-overlays                    # ~3 s; writes what is missing
python3 tools/overlay/emit_asm_overlays.py --force   # rewrite everything
python3 tools/overlay/emit_asm_overlays.py --check   # verify, write nothing
```

`emit_asm_overlays.py` runs `tools/overlay/extract_exn_functions.py` over each
overlay and names the output from the tracked manifest
`tools/overlay/asm_overlay_manifest.json`, which records, per file, the overlay,
the symbol name, the load address and the SHA-1 of the generated text. No retail
bytes are in the manifest; the SHA-1 is what makes "regenerated from the disc"
checkable, and a mismatch is a hard error rather than a silent rewrite. 8,938
listings plus the 26 `SUMMARY.md` files are covered that way. `make
check-exn-all` runs the generator first, so a fresh clone only needs the disc.

You need the overlays on disk first: `make extract-assets` fills
`assets/extracted_iso/CDROM/EXEDIR/*.EXN` and copies them to
`assets/overlays/`. The generator also finds them in
`assets/extracted_iso/CDROM/EXEDIR/` directly.

## The 33 tracked exceptions

These cannot be produced by the generator, so they stay in git (`.gitignore`
lists them explicitly):

- **Hand-merged windows** (`/* 0x... (merged block) */`): one file covers
  several symbols whose boundaries the scanner splits differently --
  `TT/tt_time_set.s`, `TT/tt_seq_step.s`, `OPTION/option_menu_in.s`,
  `TAIIKU/bg_layer_scroll.s` and the rest of the `text differs` group in the
  manifest generator's report.
- **Hand-written thunks** without an `Address:` header:
  `EVENT/func_80136FA0_thunk.s`, `EVENT/func_80152E90_thunk.s`.
- **Gap fragments** recovered by `build_clean_exn.py --gen-gap-asm` (padding
  and orphan `jr $ra; nop` bodies the scanner does not emit):
  `OPTION/option_nop0.s`, the `dummy_func*` files and the `func_8014xxxx`
  holes.

If you add an overlay listing by hand, add it to this list and to `.gitignore`.