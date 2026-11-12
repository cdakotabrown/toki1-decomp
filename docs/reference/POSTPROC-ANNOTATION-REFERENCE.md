# @PostProc Annotation Reference

**Last updated: 2026-05-11**

Matched C files document their post-processing requirements via `@PostProc:` header
annotations. The build system (`build_level3_c.py` + `parse_postproc.py` for main
binary; `tools/overlay/overlay_postproc.py` for overlays) parses these automatically.
Each `@PostProc:` line specifies one post-processing step, applied in order.

## Supported Directives

### Frame & stack
| Directive | Example | Description |
|-----------|---------|-------------|
| `patch_stack_complete` | `--old 24 --new 40` | Frame size + RA/s-reg offset patching |
| `add_leaf_frame` | `--frame-size 8` | Add frame to leaf function |
| `remap_stack_slots` | `--from X --to Y` | Remap sp-relative offsets |
| `rebase_store_to_sp` | `--offset X --base REG` | Convert reg-relative store to sp-relative |

### Register manipulation
| Directive | Example | Description |
|-----------|---------|-------------|
| `swap_registers` | `swap_registers a0:v1 --unidirectional [--exclude-offsets 0xN,0xM]` | Register swap. `--unidirectional` is preferred for chains; `--exclude-offsets` are .text-relative (function offset + 0x10 for thunk pad). |
| `swap_registers_liveness` | `REG1->REG2` | Liveness-aware register swap |
| `rename_register_chain` | `--chain a:b --chain c:b` | Multi-hop chain rename (collapse temps to dispatch reg) |
| `patch_insn_reg_field` | `0xN:rt:OLD:NEW` | Surgical register field patch (rs/rt/rd) |
| `patch_insn_imm16` | `0xN:8:VAL` | Surgical 16-bit immediate field patch |

### Move encoding
| Directive | Example | Description |
|-----------|---------|-------------|
| `convert_or_to_addu` | `convert_or_to_addu` | OR→ADDU move encoding (library region) |
| `convert_ori_to_addiu` | `--offsets X,Y,Z` | ORI→ADDIU opcode swap (WORD INDICES, +4 for thunk pad) |
| `swap_lui_addr_form` | `--offset 0xN --to addiu` | **NEW (2026-05-09)** Convert lui+ori ↔ lui+addiu address-load form |
| `swap_spill_with_move` | `--offset 0xN --dst v0` | **NEW (2026-05-09)** Replace `sw $rT, M($sp)` with `or $rD, $rT, $0` (semantic SW→OR transform) |

### Delay-slot scheduling
| Directive | Example | Description |
|-----------|---------|-------------|
| `unfill_branch_delay` | `--offset 0xN` | Cond-branch: NOP delay slot, decrement target by 1 |
| `unfill_jal_delay` | `--offset 0xN [--max-search K]` | **NEW (2026-05-09)** jal/jr/jalr/j: NOP delay slot via forward-NOP swap |
| `hoist_out_of_delay_slot` | `--offset 0xN` | **NEW (2026-05-09)** Swap branch with delay-slot insn; PC-relative offset adjusted -1 |
| `epilogue_share_into_branches` | — | **BANNED (2026-07-18)** by the test pipeline (injects instruction words). The 2026-05-09 allowance is obsolete; do not use. |
| `nop_at_offset` | `--offset 0xN` | NOP a specific instruction |

### Branch & control flow
| Directive | Example | Description |
|-----------|---------|-------------|
| `invert_branch` | `--offsets 0xN 0xM ...` | Invert branch polarity (beq↔bne) |
| `swap_branch_operands` | `--offsets 0xN` | Swap rs/rt operands on conditional branch |
| `adjust_branch_offset` | `--offset 0xN --imm Y` | Patch a branch's signed 16-bit offset |

### Cross-prologue / thunks
| Directive | Example | Description |
|-----------|---------|-------------|
| `prepend_thunk` | — | ⛔ **BANNED** (see below). Fabricates entry instructions. |
| `append_trailing_instructions` | — | ⛔ **BANNED** (see below). Use `@SymbolSize` to trim an over-counted window. |
| `strip_leading_instructions` | `--count 2` | Remove cross-prologue from function body |
| `strip_trailing_instructions` | `--count N` | Remove trailing nops/padding. ⚠️ **BANNED on a ≤16-byte symbol** — see "Fabricated cross-prologue-head stub" below. |
| `strip_arg_homes` | `--count N` | Remove sw arg-home pairs at function entry |
| `strip_div_checks` | `strip_div_checks` | Remove division-by-zero check pattern |
| `pad_trailing_nop` | `--count N` | Add N trailing nops (for symbol size) |

### Instruction-level edits
| Directive | Example | Description |
|-----------|---------|-------------|
| `swap_instructions` | `--offset1 0x4 --offset2 0x8` | Swap two instructions by offset |
| `cse_lui_at` | `cse_lui_at` | Remove redundant `lui $at` instructions (legacy: ALL of them) |
| `cse_lui_at` (scoped) | `cse_lui_at --offsets 0x30,0x38` / `--same-symbol-only` | Remove ONLY the named `lui $at` sites (or only same-symbol redundant pairs) — use when the target shares one `lui $at` per symbol/object group and re-emits a fresh one for the next group. `--offsets` are `.text` byte offsets of the object as seen by *this* step in the chain (`python3 tools/ido/cse_lui_at.py obj.o --list` prints them; body starts at +0x10). Branch displacements spanning a removed insn are auto-fixed in scoped mode (`--no-fix-branches` to disable) |
| `cse_lui_at` (nop-in-place) | `cse_lui_at --offsets 0x490 --nop-in-place` | **NEW 2026-08-11.** Same redundancy selection and the same removal-only guarantee as scoped mode, but the removed `lui $at` slot is left as a `nop` and only its HI16 reloc is dropped — nothing shifts (no branch retargeting, no reloc offset fixups, no symbol-size change). For targets where the original as1 shared `$at` across %hi-identical store macros and the vacated load-use-hazard slot survives as a literal `nop` (count must be preserved). Strictly more conservative than compacting mode. Soundness burden: the kept preceding `lui $at` must be the only reaching def (no `$at` clobber, no branch target in between — the tool's live-`$at` walk checks this) and the two symbols' `%hi` must be equal including lo16-carry. First use + worked soundness proof: `ETC/func_801439F0` header. Wanted-hook alternative (`IDO52_AS1_SHARE_AT`) remains unbuilt |
| `block_reorder` | `--block X --target Y` | Reorder within basic blocks |
| `scheduler_normalizer` | `[--block-aware] [--const-fp] [--store-fp] [--load-fp] [--no-forced-reorder]` | Reorder to match target scheduling tie-breaks. Fingerprint flags pair reordered ops the position matcher misses, then `gen_reg_patches` paints the regs: `--const-fp` (li/lui by value), `--store-fp` (sp-stores by offset), **`--load-fp`** (loads by op+base+offset, **NEW 2026-06-14**, mirror of store-fp for same-base record loads scheduled in swapped order, e.g. `p->y` vs `p->dz`). **`--no-forced-reorder`** (**NEW 2026-07-19**) disables the dependency-bypassing forced-reorder fallback (see note below) — leaves a block unnormalized rather than permuting past data deps; use to audit which matches rely on it. The fallback fires on ~63% of normalizer functions and now always prints a `[FORCED-REORDER]` stderr warning; a 2026-07-19 audit confirmed it never fabricates bytes for a genuine win, but it can let a real codegen diff hide behind reloc-masking, so **verify any forced-reorder match with `resolve_overlay_relocs.py`**, never the masked test. |
| `fix_sign_test_pattern` | `fix_sign_test_pattern` | IDO 5.1 `and+beqz` sign-test fix |

### Top-level / scripts
| Directive | Example | Description |
|-----------|---------|-------------|
| `asm_intercept` | `SCRIPT_NAME` | Assembly-text-level PostProc (alternative to ELF binary edits) |
| `custom_script` | `PATH` | Custom Python script (BANNED for .text replacement; allowed for register patches/reorders/reloc fixup only) |

---

## ⛔ BANNED Directives

The following are **policy-banned** because they violate the matching contract by
inserting hardcoded bytes or shifting unrelated instructions.

**The test pipeline (`test_overlay_match.py`) will REFUSE TO RUN if any of these
are detected in a source file.** Do NOT try to work around this.

| Directive | Why Banned |
|---|---|
| `patch_instruction` | Replaces entire instruction word with hardcoded target hex. That's not decompilation. |
| `patch_insn_word` | Alias for `patch_instruction`; same violation, same ban. |
| `insert_instruction_and_shift` | Inserts a new instruction and shifts subsequent code. Creates instructions from thin air. |
| `insert_move_and_shift` | Variant of `insert_instruction_and_shift`; same ban. |
| `write_insn_at_offset` | Writes an entire instruction word at a binary offset. Same as `patch_insn_word` — overwrites compiled output with hardcoded bytes. |
| `custom_script` (`.text` replace) | Replaces the entire `.text` section — same as hand-writing ASM. |

Also banned by name: `prepend_thunk`, `append_trailing_instructions` /
`append_trailing`, `epilogue_share_into_branches`.

### ⛔ Fabricated cross-prologue-head stub (a *shape*, not a directive name)

Enforced by `check_fabricated_head_stub()` in `tools/build/banned_postproc.py`, called
from `test_overlay_match.py`. **1,544 instances were found tree-wide on 2026-07-21, all
counted `byte_perfect`.**

IDO -O2 hoists a function's first global read ABOVE its prologue:

```
80134228: lui   $t6, 0x800F     <-- cross-prologue head: NOT a function.
8013422C: lw    $t6, -1948($t6)     No jr ra; falls through into the prologue.
80134230: addiu $sp, $sp, -40   <-- prologue
```

Some `.sym` maps registered those two words as a standalone symbol. The fabrication is
to write `return D_800EF864;` for it — which compiles to **four** instructions
(`lui`, load, `jr ra`, `nop`) — and then chop the last two:

```c
/* ⛔ BANNED: fabrication by subtraction. The target contains no jr ra to strip. */
/* @PostProc: strip_trailing_instructions --count 2 */
s32 func_80134228(void) { return D_800EF864; }
```

**The fix is ABSORPTION** — give the head back to the host function that follows it, so
the compiler emits it:

```c
/* @SymbolStart: 0x80134228 */   /* window widened back over the head */
/* @SymbolSize: 56 */
void func_80134230(s32 param0)   /* the "hidden param" never existed */
{
    if ((u32)param0 < (u32)D_800EF864) {   /* named extern, read FIRST */
        func_800429FC();
    }
}
```

Rules:
- Use a **named `extern`**. A raw-address cast or a `volatile` cast macro blocks the
  hoist — IDO emits the load *after* `addiu sp` and the head never appears.
- Delete the register paints that only existed to paint the arg register to the head's
  register (`...:a0:t6`). A stale `patch_insn_reg_field` **aborts the entire PostProc
  chain**, so `patch_stack_complete` silently never runs and it looks like a frame bug.
- The absorbed head changes the compiled frame (often 24→32), so re-read
  `patch_stack_complete --old` off the fresh compile — a wrong `--old` silently no-ops.
- Tool: `tools/overlay/absorb_stub.py <OV> <STUB> --apply` (sym rewrite, window,
  offset shift, backup, strict verify, surgical `--revert`).
- **If the host cannot reproduce the head**: `--revert`, then leave a comment-only
  `Status: ASM-ONLY` marker — no body, no directives. That is still correct; it removes
  the fabrication. Never keep the `strip_trailing_instructions`.
- **Never rename a caller's reference** to an absorbed head. Callers legitimately `jal`
  the head address; the resolver derives addresses from the symbol NAME, so
  `extern void func_<HEADADDR>()` still links with no `.sym` entry.

`strip_trailing_instructions` stays ALLOWED elsewhere — notably the main binary's
types.h thunks. It is the ≤16-byte-symbol context that makes it fabrication.

**The test pipeline enforces this at runtime** via `tools/build/banned_postproc.py`.
Any file with a banned `@PostProc:` directive will cause the test to exit with an error.

**ALLOWED alternatives to banned directives:**
- `patch_insn_reg_field` — patches specific register fields ONLY (not the whole word)
- `patch_insn_imm16` — patches 16-bit immediate field ONLY
- `swap_registers` / `rename_register_chain` — full register swaps
- `swap_lui_addr_form` — semantic ori↔addiu form swap
- `swap_spill_with_move` — semantic SW→OR transform
- `unfill_jal_delay` / `unfill_branch_delay` — NOP out delay slots (no insertion)
- `hoist_out_of_delay_slot` — position swap with offset adjustment
- `scheduler_normalizer` — instruction reordering to match target

**If the C source fundamentally doesn't match**: Rewrite the C. Do NOT paper over
mismatches with hardcoded bytes. If the function is truly unmatchable from C,
add it to `config/c_unmatchable.txt` with a blocker category.

### ⚠️ Auditing a `patch_insn_reg_field` layer (2026-08-09)

`patch_insn_reg_field` is ALLOWED, but a layer becomes a DEFECT when the
rewritten object stops computing the compiler's values — and the masked
compare, the name-derived resolver and `semcheck` can all still score it.
Four audits exist; the full write-up (including every measured
false-positive mode) is in each tool's module docstring:

| Tool | Signal |
|------|--------|
| `tools/overlay/audit_paint_bans.py` | PROVABLE ban classes (compiled nop, REGIMM/blez/bgtz `rt`, zero↔reg on a branch, stale `old` reg, offset outside body) |
| `tools/overlay/audit_paint_isomorphism.py` | ADVISORY reaching-definition producer changes (~15% FP) |
| `tools/overlay/minimize_paint_strip.py` | repair-aware minimal strip PROPOSAL |
| `tools/overlay/audit_raw_count.py` | PROVABLE raw instruction count with zero PostProc |

Three rules that are easy to get wrong:

- ⭐ The `rt`-field branch ban covers **REGIMM (op 1) and blez/bgtz (op 6/7)
  only**. `beq`/`bne` `rt` is an ordinary register operand — painting it is
  legal.
- ⭐ A paint whose removal *creates* an orphaned read is REPAIRING upstream
  `scheduler_normalizer` damage, not causing it. Do not strip it.
- ⭐ A paint can re-point a `%lo` at a different `lui %hi` and send a store to
  the **wrong global** with an identical dataflow shape. Nothing above sees
  that; `gen_reg_patches.py` now refuses to emit it.

`gen_reg_patches.py` enforces the provable classes at EMIT time (no
`--force`); it prints the reason and the suggested alternative, which is
almost always "this is a C bug, not a paint target".

---

## Per-Function Compile Flags

Use `@CompileFlags:` for non-standard IDO flags:
```c
/* @CompileFlags: -Wo,-loopunroll,0 */
```

## Per-Function Compile Environment

Use `@CompileEnv:` to pass IDO52_* uopt/ugen hook env vars:
```c
/* @CompileEnv: IDO52_EXCLUDE_REGS=v0 IDO52_NO_FINDINDUCT_STRIDE=1 */
```

The env vars are merged into the cc invocation environment by the build system.
See `docs/reference/UOPT-HOOKS-REFERENCE.md` for the complete hook list.

## Per-Function Symbol Size Override

Sometimes the .sym file's symbol size includes trailing bytes from the next
function (cross-prologue scheduling artifact). Override with:
```c
/* @SymbolSize: 172 */
```

This forces the verifier to compare exactly N bytes from the function address.

## Link-Placement Directives (overlay resolver): `@RodataBase`, `@StaticAddr`

The name-derived resolver (`tools/overlay/resolve_overlay_relocs.py`) links every
`func_/D_<addr>` symbol at the address its NAME encodes. Two kinds of reference
have no such name, because they point into the object's OWN sections; these
directives place those sections. Neither copies anything from the target: a
wrong value fails the strict compare like a wrong `D_` name would.

```c
/* @RodataBase: 0x80150A20 */          /* this TU's .rodata block (jump table) */
/* @StaticAddr: D_80150164 0x80150164 */  /* a file-private static datum */
```

- **`@RodataBase: 0xADDR`** — places the object's `.rodata` at ADDR. Derive it
  with `apply_rodata_base.py` (self-validates by requiring a full match).
- **`@StaticAddr: NAME 0xADDR`** (2026-10-03) — for a `static` global DEFINED in
  the TU. IDO keeps a global in a caller-saved register across calls (store
  before each `jal`, reload after) only when it is file-private, so when retail
  shows that pattern the honest C is `static`, not `extern`
  (`ETC/func_801438D0`, D_80150164 in `$a3`). A static relocates against the
  object's `.bss`/`.data` section symbol; the directive places that section so
  the static lands on ADDR. Checks (hard refusal, `StaticAddrError`):
  an address-encoded NAME (`D_<8 hex>`) must equal ADDR; the static must exist
  in the object (found by name in the ELF symtab for gcc, in `.mdebug` for IDO,
  which drops statics from the symtab); every static sharing a placed section
  must be listed and all must agree on one base. One line per static; anchored
  (the line must open with the directive, so header prose never counts).
  Other TUs that touch the same datum keep `extern` — the static belonged to
  the retail TU that defined it. Regression fixtures:
  `python3 tools/overlay/resolve_overlay_relocs.py --self-test`.
- Consumers read both through `resolve_overlay_relocs.link_options_from_source`
  (resolve_function, derive_rodata_base, sbs.py, aligned_diff.py,
  build_clean_exn.py, screen_semis.py). The masked `test_overlay_match` /
  `verify_overlay_bytes` path needs nothing: section relocs are masked there,
  and its authoritative ✅ line comes from `resolve_function`.
  `build_overlay.py` (whole-overlay link) honours neither directive yet.

## Per-Function Compiler Override

The build system supports per-function compiler/env overrides via
`build_all_c.py --cc-overrides`:
```json
{
  "func_name": {
    "cc": "./tools/ido-static-recomp/build/5.2/out/cc",
    "env": {"SOME_VAR": "value"}
  }
}
```

For per-function GCC compilation (vs default IDO), use:
```c
/* @Compiler: gcc260 */    /* or gcc272, gcc281 */
```

---

## Important Notes

- Each `@PostProc:` line specifies ONE step (multiple operations of the same type
  require multiple lines)
- Steps apply in order from top to bottom
- Most offset arguments are **function-relative** (the dispatcher adds the 0x10
  thunk pad automatically). Exceptions noted in tool descriptions.
- `convert_ori_to_addiu --offsets` takes **WORD INDICES** in .text (function-relative
  index + 4 for the thunk preamble)
- `swap_registers --exclude-offsets` is **.text-relative** (function offset + 0x10)
  — historical inconsistency with most other tools
- See `tools/overlay/overlay_postproc.py` for the overlay parser/dispatcher
- See `tools/build/parse_postproc.py` for the main-binary parser

## Ordering & hygiene rules (2026-07-18)

- Steps run in FILE ORDER. `adjust_branch_offset` (sets the imm VERBATIM),
  `nop_at_offset`, and zero-dead custom scripts must be placed LAST — after
  any `scheduler_normalizer` — or their offsets miss the final stream shape
  (and a failed step silently aborts the WHOLE chain, falling back to raw).
- `patch_insn_reg_field` lines are NOT cumulative: only one line applies.
  Merge all paints into a single line; strip before regenerating
  (`gen_reg_patches` APPENDS).
- Duplicate directives (e.g. two `patch_stack_complete`) fail the chain:
  the second application errors and everything downstream is skipped.
- A wrong `patch_stack_complete --old` (not matching the RAW prologue)
  silently aborts the chain — symptom: exact-% collapses and no step applies.
- `scheduler_normalizer --fence <startword>-<endword>` pins a word range:
  use it to protect a tail from long-range same-shape drags, or to pin the
  OTHER case of a dispatcher so a within-case delay swap can occur without
  cross-case reloc drag.
