# CLAUDE.md

Orientation for agents working in **toki1-decomp**, the byte-exact
decompilation of *Tokimeki Memorial: Forever with You* (SLPS-00065, PS1).
These rules override default behaviour. The disc and setup are in
`README.md`; `toki1-english` and `toki1-pc` build on this repository and
are not needed here.

## State

Every function is terminal: overlays 6,111 byte-perfect C + 7 asm_only; the
main executable 1,375 C files + 173 asm_only (assembly by origin), 0 open.
All 7,486 shipped C functions are PROVEN equivalent to their PostProc-free
compile; 4,048 match with neither `@PostProc` nor `@CompileEnv` (the strict
number, quoted beside the effective one). Numbers go
stale: regenerate before quoting (`tools/analysis/audit_main_functions.py`;
serial `tools/overlay/verify_overlay_bytes.py <OV>` then
`apply_resolved_status.py --demote`; `prove_equivalence.py`; the two
coverage gates). Never restore an older, higher figure. Remaining work:
symbol naming (zero `func_`/meaningless `D_`, style in
`docs/reference/NAMING-CONVENTIONS-1994.md`) and optional removal of
ALLOWED PostProc.

## The rule: the C is the source of truth

A function is done when it is `byte_perfect` (the build reproduces its bytes
from its C, only ALLOWED PostProc, proven equivalent) or `asm_only` (no C,
with a blocker category in `config/c_unmatchable.txt`). An honest
`asm_only` beats a fabricated match. **Fabrication never counts.**

When C does not match, in this order: (1) rewrite the C; (2) decomp-permuter
(`make setup-permuter`); (3) a compiler hook (`@CompileEnv`, see
`docs/reference/UOPT-HOOKS-REFERENCE.md`); (4) `asm_only` with a blocker;
(5) ALLOWED PostProc, never a paint that changes dataflow. A refusal from
`tools/overlay/gen_reg_patches.py` means the C is wrong.

**Banned, no exceptions** (the build refuses them; `tools/build/banned_postproc.py`):
`patch_instruction`, `patch_insn_word`, `write_insn_at_offset`,
`insert_instruction_and_shift`, `insert_move_and_shift`,
`propagate_stack_to_reg`, `patch_load_signedness`, a `custom_script` that
writes instruction words or replaces `.text`, and the restricted steps
`fill_delay_slots`, `swap_spill_with_move`, `invert_branch`,
`convert_addiu_to_slti`, `nop_at_offset` (except `--dead-arg-home`),
`patch_insn_imm16`, `adjust_branch_offset`, `convert_ori_to_addiu` /
`convert_addiu_to_ori`, any register paint to or from `$zero`, and
`scheduler_normalizer`'s retail-copied branch displacements.
`tools/build/postproc_grandfathered.json` is empty and only shrinks.
Also banned: the fabricated cross-prologue head stub (`return D_x;` plus
`strip_trailing_instructions` on an 8-byte symbol: absorb the head into the
host function instead) and a large `pad_trailing_nop` that fakes a count.
`@CompileEnv: IDO52_SKIP_PASSES=codemotion` miscompiles: never use it.
ALLOWED steps and their syntax: `docs/reference/POSTPROC-ANNOTATION-REFERENCE.md`.

## Gates before claiming a win

```bash
python3 tools/overlay/test_overlay_match.py <OV> <FUNC> -v      # TRUE WIN line, len t=X c=X equal
python3 tools/overlay/audit_raw_count.py <OV> <FUNC>            # PASS, delta=0
python3 tools/analysis/test_single_match.py FUNC                # main: Body == target
python3 tools/analysis/audit_main_relocs.py --only FUNC         # main: right names at reloc sites
python3 tools/analysis/audit_reloc_positions.py FUNC
python3 tools/analysis/audit_plain_vs_shipped.py --only <OV/FUNC>   # SAME
python3 tools/analysis/prove_equivalence.py --only <OV/FUNC>        # PROVEN
```

After any change to `src/`, `config/`, `tools/` or the hooks, also:
`make check-raw build-c check-exn-all check-clean-main`,
`audit_plain_vs_shipped.py`, `prove_equivalence.py` (whole tree),
`tools/overlay/audit_fabrication.py --strict`,
`tools/overlay/audit_truncated_compare.py`,
`tools/analysis/audit_registry_overlaps.py`,
`tools/analysis/audit_main_coverage.py`,
`tools/overlay/audit_overlay_coverage.py` (0 uncovered). An UNPROVEN verdict
is never assumed equivalent. The model behind the proof:
`docs/reference/EQUIVALENCE-PROOF.md`.

## Compilers

- Game code (0x80041000-0x80085FFF and the overlays): IDO 5.2,
  `tools/ido-static-recomp/build/5.2/out/cc -O2 -G0 -mips1 -EL -non_shared`.
  C89 only: `/* */` comments, declarations at the top of a block.
  `tools/build/ido_flags.py` is the one definition of the command line
  (it adds `-Wc,-pic1`, the 40-byte frame minimum).
- Library region (0x80086000-0x800BFFFF): IDO (moves become `addu`), Psy-Q
  GCC 2.6.0/2.7.2/2.8.1 (`@Compiler: gcc260` ...) or IDO codegen with ASPSX
  assembly (`@Compiler: ido52-gas`).
- The compiler hooks live in generated C that `make clean` of the recomp
  wipes: `make setup-ido52` reapplies the chain
  (`python3 tools/ido-static-recomp/apply_all_hooks.py --check`).

## Where things are

| path | what |
|---|---|
| `src/main/matched/` | main-executable C; enters the build when registered in `config/level3_functions.json` |
| `src/overlay/<OV>/` | overlay C; symbol windows in `config/overlay/<OV>.sym` (tracked source, never regenerate) |
| `include/` | headers |
| `asm/overlay/` | tracked assembly exceptions; everything else under `asm/` is generated (`make asm-overlays asm-main`) |
| `config/` | registry, `.sym` maps, `symbol_names.json` (the name ledger), `c_unmatchable.txt` |
| `tools/overlay/`, `tools/analysis/` | per-function tests, gates, audits |
| `tools/build/`, `tools/postproc/`, `tools/ido/` | the build and the post-processing steps |
| `docs/reference/` | proof, hooks, PostProc, `MATCHING-TRICKS.md`, naming style, `GAME-ARCHITECTURE.md` |

## Traps that cost the most

1. Write overlay C from `python3 tools/overlay/dis_overlay.py <OV> <FUNC>`
   (the full `.sym` window), never from `asm/overlay/<OV>/<func>.s` alone.
2. A symbol address is a hypothesis: IDO -O2 hoists a function's first global
   load above its prologue. Widen the window with
   `@SymbolStart`/`@SymbolSize`; never invent a parameter for it.
3. Named `extern` globals, never raw-address casts. `extern u8 D_x[];`
   (`lui;addiu`) and `extern u8 *D_x;` (`lui;lw`) differ in length.
4. `RELOCATION-ONLY` from the masked test can hide a real diff; trust the
   resolver line (`resolve_overlay_relocs.py <OV> <FUNC> -v`).
5. `Exact: X/N` uses the shorter length: a shorter body scores higher. Check
   `Body == target` (main) or `len t=X c=X` (overlays).
6. The main-binary harness copies relocation immediates from retail: a wrong
   symbol name still prints BYTE-PERFECT. Run `audit_main_relocs.py`.
7. A negative `%lo` subtracts (`lui 0x800c` + `-24168` is 0x800BA198).
8. `verify_overlay_bytes.py --all --parallel` gives false demotions;
   re-verify an untouched overlay serially before believing a regression.
9. `remap_stack_slots`: pass `--keep-frame` when a map key equals the frame
   size. `strip_leading_instructions` on main: use `--count auto`.

## Working rules

- Commit with explicit paths; never `git add -A`.
- Before every push: `tools/ci/run_ci_locally.sh` must end `CI would pass`.
  Change `.github/workflows/build.yaml` and that script together.
- Generated output goes under `build/`, never the repository root.
- Never commit anything from the disc (`disks/`, `assets/`, generated `asm/`,
  `include/overlay_text/`).
