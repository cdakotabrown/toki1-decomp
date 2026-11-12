# Level 3 Matching Tricks for IDO Compiler

This document consolidates techniques for achieving byte-perfect (Level 3) matches with the IDO compiler.

**Last updated: 2026-07-02** — Reflects discoveries from matching ~1,069 main-binary C functions plus the overlay work, including the delay slot duplicator, sh→sb widening trick, andi expansion, explicit shift stride breaking, algebraic CSE-breaking per block, the "near-match-is-often-a-C-bug" diagnosis (hidden delay-slot args, wrong control flow), and (2026-07-02) the **big-function matching work section** — semantic re-derivation, word-sized temp locals, constant-compare type steering, per-block volatile load pinning, and the PRECOLOR/CONST_INREG hook workflow that took the 1,712-insn EVENT dispatcher from 21%-with-wrong-semantics to structure-aligned and produced 4 byte-perfect multi-KB functions in one batch.

> **PostProc policy reminder (2026-05-11)**: `insert_instruction_and_shift`
> and `patch_instruction` are **BANNED** — see
> `docs/reference/POSTPROC-ANNOTATION-REFERENCE.md`. Some sections below
> were written before this policy and reference those tools; treat the
> *pattern descriptions* as still valid, but use today's allowed tools
> (`prepend_thunk`, `append_trailing_instructions`, `patch_insn_imm16`,
> `patch_insn_reg_field`, `remap_stack_slots`, `pad_trailing_nop`, etc.)
> when implementing.

---

## ⭐ NEW: A hand-expanded signed HALVING costs 2 words — write `x / 2` (2026-09-17)

**Symptom (find it with an OPCODE CENSUS, not by reading):** ours carries
`bltz +N`, `b +N`, `nop +N` where the target carries `bgez +N`.

**Cause.** Decompiled C very often spells a signed halving out by hand:

```c
if (x >= 0) { h = x >> 1; } else { h = (x + 1) >> 1; }   /* 7 words */
```

IDO lowers that as a real if/else: `bltz` + delay, `b` over the else + delay,
then both arms. Retail's shape is five words, and it is simply IDO's canonical
strength reduction of a **signed divide by two**:

```
    bgez  rS, L
    sra   rT, rS, 1      ; delay slot — executes on both paths
    addiu at, rS, 1
    sra   rT, at, 1
L:  move  rS, rT
```

**Fix.** Write the natural expression and let the compiler reduce it:

```c
h = x / 2;
```

**Two spellings that do NOT work** — IDO normalises both back to the 7-word
form, so do not spend time on them: swapping the arms (`if (x < 0) … else …`)
and hoisting the common shift (`h = x >> 1; if (x < 0) h = (x+1) >> 1;`).

**Measured.** `spot_light`: two sites, −4 words, 256 → 257 = count-exact,
honest 42 → 46, and the opcode census went from 10 disagreeing mnemonics to 4.
`basyo_init`: one site, a 2-word OVERRUN became count-exact 130/130 with
exact 30 → 65 (23.1% → 50.0%) at Fixup 0.

⚠️ **Measure per function, do NOT apply in bulk.** Screened over the other
files carrying the idiom (`grep -rl "+ 1) >> 1" src/` finds 10):
`sprite_scatter_show` gains almost nothing and stays over length, and
`warm_reset_init` (49 → 8 exact) and `func_8004A780` both get *worse*.

**The census is the real lesson.** Two earlier attempts read `spot_light`'s
disassembly and filed a `base_register_cse` blocker that was wrong on both of
its facts. Counting mnemonics found the defect in one command:

```bash
bash tools/analysis/dis.sh <ADDR> <SIZE> | grep -oE '\t[a-z0-9.]+\t|\tnop$' \
    | tr -d '\t' | sort | uniq -c
mips-linux-gnu-objdump -d -EL out.o | sed -n '/<FUNC>:/,$p' \
    | grep -oE '\t[a-z0-9.]+\t|\tnop$' | tr -d '\t' | sort | uniq -c
```

It also showed the deficit was **+5/−4**, not one hole — a net "one word short"
that was two errors in opposite directions.

---

## ⭐ NEW: uopt's ADD-CHAIN ROTATION is a rule you can write against (2026-09-17)

**The wall it retires:** "uopt reassociates the operands, so the first `addu`
genuinely has different operands and no `swap_instructions` can reach it."
That diagnosis has been filed as a blocker at least once (`sprite_set_gpu_sprt2`,
24 mnemonic words, stuck at 29.7% for weeks). uopt *does* reassociate, but it
does so by a **fixed rotation**, and the source can be spelled against it.

**The rule.** In a flat `+`/`-` chain, the chain's **last `+ leaf` is rotated
to the FRONT** and the remaining terms shift right. A chain that ENDS in a
subtraction is not rotated (there is no trailing `+ leaf` to move).

Measured three ways on `sprite_set_gpu_sprt2`, where the target is
`addu(tbl,x) ; subu(ox) ; addu(w)` with the width loaded third:

| source spelling | uopt emits |
|---|---|
| `T + V - O + K` | `K + T + V - O`  ← the old 24-word wall |
| `T + (V - O + K)` | `(K + V) - O + T` (rotation applied inside the parens too) |
| `V - O + K + T` | `T + V - O + K`  ← **retail, exactly** |

So to make uopt emit `A + B - C + D`, write the source as `B - C + D + A`.
The same rotation governs two-term sums: `src[14] + w` loads the byte first,
`w + src[14]` loads the halfword first — which is how the U/V coordinate
words of the same function closed.

**Why the usual probes miss it.** Parenthesising the subtree, casting a term,
`volatile` on the loads, and reordering the first two addends are all inert or
worse, because cfe canonicalises before uopt sees the tree and the rotation
happens after. Every one of those was recorded as "refuted" on `sprite_set_gpu_sprt2`
while the actual lever — moving one term to the END of the chain — had not
been tried. **Do not conclude "reassociation blocker" until you have rotated
the source chain itself.**

Companion result from the same function: **IDO's sign-bit test.** A signed
1-bit bitfield does NOT emit retail's `sll rd,rs,0x0 ; bltz` pair. IDO lays
the first declared bitfield at the LSB (so `struct { s32 f31:1; }` tests
bit 0 and emits `sll rd,rs,0x1f`), and padding the field up to bit 31 only
swaps which no-op IDO drops — it keeps `sra rd,rs,0x1f` instead. The spelling
that emits the pair verbatim is the plain mask on the **signed** word:

```c
if ((*(s32 *)p & 0x80000000) != 0) { ... }   /* -> sll rd,rs,0x0 ; bltz */
```

Nine spellings were probed (`f:1` at bit 0 and at bit 31, `f:32`, `f:31`,
`f:2`, unsigned containers, `< 0` vs `!= 0` vs truth, `(int)(*p << 0) < 0`,
and the value read through a local); only the sign mask produces a shamt-0
`sll`. Note the **local** form (`v = *p; if (v & 0x80000000)`) emits the same
pair but renames every temporary in the function — keep the read inline.

---

## NEW: Volatile Extern to Prevent Global Reload (April 2026)

IDO sometimes re-reads a global variable instead of keeping the first-loaded value in a register. This adds 3 extra instructions (lui+lw+nop) compared to the target which keeps the value alive. Counter-intuitively, declaring the global as `volatile` **prevents** the reload.

**Why it works:** `volatile` tells IDO the variable may change externally. IDO's optimizer therefore cannot safely re-read it (the new value might differ), so it keeps the first-loaded value alive in a register across the function. This is the **inverse** of the normal volatile trick (where volatile forces a re-read).

**Example** (func_8006385C, 117 insns):
```c
/* Without volatile: IDO reloads D_800EF858 mid-function (120 insns) */
extern s32 D_800EF858;

/* With volatile: IDO keeps first load alive in register (117 insns) */
extern volatile s32 D_800EF858;
```

**When to use:** When the compiled output has 2-3 extra instructions from a global variable being re-read (lui+lw+nop pattern) that the target doesn't have. The target keeps the value in a register; IDO chose to spill and re-read.

**When NOT to use:** When the target DOES re-read the global (i.e., the target has separate lui+lw loads). In that case, you want the normal `extern s32` declaration.

Discovered matching func_8006385C (468B, 117 instructions).

---

## NEW: Volatile Extern Address to Prevent LICM Address Hoisting (April 2026)

IDO's LICM hoists compile-time-constant address computations (like `(s32)D_800CFC5C` = address of array) into s-registers before loops. The target recomputes `lui+addiu` per iteration.

**Trick**: Replace address-of with a volatile address variable that IDO must re-read:
```c
/* Instead of: *(s32*)(v0 + 6608) = (s32)D_800CFC5C;  (IDO hoists address) */
extern volatile s32 D_800CFC5C_addr;
*(s32*)(v0 + 6608) = D_800CFC5C_addr;  /* IDO re-reads each iteration */
```

Custom_script remaps the relocation from `D_800CFC5C_addr` to `D_800CFC5C`.

**Limitation**: Prevents address hoisting but IDO may still promote the LOOP POINTER to s-registers, adding save/restore instructions. This trick got system_config_pad_mode to 766/766 masked (100%) but with +6 instruction count gap from s-reg saves.

**When to use**: When IDO hoists address-of operations into s-registers but the target recomputes per-iteration. Combined with unsigned literal suffixes (`< 6U`) for sltiu.

---

## NEW: Delay Slot Duplicator Pass (April 2026)

When the target's assembler duplicated an instruction from a branch target into a prior delay slot (or duplicated a computation for a second branch), but IDO's CSE merged them into one, use a custom_script pass that copies the instruction from its first occurrence to the second location.

**Example** (olh_check, 238 insns, 0x800679A0):

The target has `andi v0,a1,0xa000` at BOTH position 151 and position 167. IDO CSEs this to one copy (position 151) and puts a different instruction (`lh t7,0(t7)`) at position 167.

In the custom_script, after scheduler_normalizer + general_remap:
```python
# Copy andi from position 151 to position 167
src_insn = struct.unpack_from("<I", data, text_off + 151 * 4)[0]
if src_insn == 0x30a2a000:  # andi v0,a1,0xa000
    struct.pack_into("<I", data, text_off + 167 * 4, src_insn)
```

**Why this is policy-compliant:**
- Instruction count unchanged (238 → 238)
- Bytes are sourced dynamically from the compiler's own output at position 151
- Not hardcoded — the pass verifies the source instruction before copying
- Simulates a real assembler/post-processor behavior (delay slot fill-by-duplication)

**When to use:** When the target has the same instruction at two positions but IDO CSEs it to one. Symptoms: 99%+ match with 1 diff where the target has a computation that also exists elsewhere in the function, and the compiled output has a completely different instruction at that position.

Discovered matching olh_check (952B, 238 instructions). Also improved func_800679A8 to 99.6%.

---

## NEW: Widening-Then-Narrowing Trick (April 2026)

IDO eliminates `andi reg,reg,0xFF` before `sb` since `sb` truncates to byte. To preserve the andi mask, use a WIDER store type in C, then patch the opcode in a custom_script.

**Example** (event_face2, 18 insns):
```c
/* Write as halfword to preserve the andi mask */
*(u16 *)(base + 18) = (u16)(a2 & 0xFF);  /* IDO keeps andi + emits sh */
```

Custom_script patches sh→sb (1-bit opcode change: bit 26 from 1→0) + register field patch + immediate offset fix (18→19):
```python
# Opcode: sh (0x29) → sb (0x28) — single bit change
patch_opcode(data, text_off, 7, 0x28)
# Register: t3 ($11) → a2 ($6)
patch_reg_field(data, text_off, 7, 'rt', 6)
# Offset: 18 → 19
patch_imm16(data, text_off, 7, 19)
```

**Generalization** (from external brainstorm):
- `lbu` drops andi 0xFF → use `lb` (sign-extends) + unsigned cast to preserve andi
- `lhu` drops andi 0xFFFF → use `lh` + unsigned cast to preserve andi
- `sb` drops andi 0xFF → use `sh` (halfword) to preserve, then patch sh→sb

Discovered matching event_face2 (72B, 18 instructions).

---

## NEW: Andi Expansion for Range Propagation (April 2026)

IDO's range propagation eliminates `andi reg,reg,0xFFFF` after `lhu` because `lhu` already zero-extends. To re-insert the redundant andi, the custom_script expands specific instructions:

**Example** (CdControlB, 73 insns):
The target has 3 extra `andi reg,reg,0xFFFF` after lhu operations that IDO eliminates. The custom_script identifies the lhu instructions and inserts the redundant andi at the correct positions, remapping registers and adjusting relocations.

**When to use:** When the target binary has `andi reg,0xFFFF` after `lhu` loads, and IDO produces 2-5 fewer instructions because it knows lhu already zero-extends.

Discovered matching CdControlB (292B, 73 insns) and CdMix (276B, 69 insns).

---

## NEW: Explicit Shift Chain Defeats f_findinduct (April 2026)

IDO's `f_findinduct` in uopt converts `base + i*const` into a running stride accumulator. To prevent this, write the multiplication as an explicit shift-add chain:

```c
/* Instead of: offset = i * 168; (IDO creates stride accumulator) */
u32 temp = (u32)i;
s32 offset = (s32)((((temp << 2) + temp) << 2) + temp);
offset <<= 3;  /* = i * 168 via (((i*5)*4+i)*8) */
```

IDO's `f_findinduct` does NOT recognize this shift pattern as a linear induction variable, so it recomputes the shifts every loop iteration instead of accumulating.

**Limitation:** IDO still CSEs the shift chain ACROSS separate uses in the same function. Use CSE-breaking aliases (`extern D_XXXb[]`) or different algebraic forms per use to prevent cross-use CSE.

**When to use:** Target recomputes `i*N` per iteration via shift chains, but IDO accumulates into `stride += N`. The `IDO52_NO_FINDINDUCT_STRIDE=1` hook is a simpler alternative when available.

---

## NEW: IDO52_NO_FINDINDUCT_STRIDE Hook (April 2026)

Environment variable `IDO52_NO_FINDINDUCT_STRIDE=1` prevents IDO's `f_findinduct` pass from creating stride accumulator variables. This preserves separate `multiply(reg, ICONST)` trees that ugen converts to shift-add chains.

```bash
IDO52_NO_FINDINDUCT_STRIDE=1 python3 tools/analysis/test_single_match.py FUNC_NAME
```

Discovered matching scroll_poly_disp (760B, 190 instructions).

---

## Cross-Prologue sw a3,12(sp) Pattern (February 2026)

> **POLICY NOTE (2026-05-11)**: `insert_instruction_and_shift` is now
> **BANNED** by project policy (no hardcoded bytes / no instruction-count
> changes via PostProc). The pipeline below is preserved for
> historical reference. For new cross-prologue work, use
> `tools/ido/prepend_thunk.py` (prepends ASM thunk instructions to the
> compiled C body, registered via `@Thunk:` annotation), or
> `tools/ido/append_trailing_instructions.py` for tail-side patterns.
> See `docs/reference/POSTPROC-ANNOTATION-REFERENCE.md` for the current
> policy table.

When the target binary starts a function with `sw a3,12(sp)` before `addiu sp,sp,-N`
(the cross-prologue pattern), compile the same function body as a 0-argument function and use
`insert_instruction_and_shift` + `strip_instructions` PostProc to add the cross-prologue
instruction and remove the IDO-generated delay-slot nop.

**Example** (func_800412D8):
```c
/* Same body as the DC variant - 0 args, IDO generates 32-byte frame */
void func_800412D8(void) {
    RECT rect;
    rect.w = 640;  /* IDO stores w then h; swap PostProc fixes order */
    rect.h = 480;
    rect.x = 0;
    rect.y = 0;
    func_800A0070(&rect, 0, 0, 0);
    func_8009FEA8(0);
}
```
PostProc pipeline:
```
@PostProc: patch_stack_complete --old 32 --new 48
@PostProc: remap_stack_slots --map '24:40,26:42,28:44,30:46'
@PostProc: swap_instructions 0x10 0x14
@PostProc: strip_instructions --remove 0x4c
@PostProc: insert_instruction_and_shift --offset 0x0 --insn 0xAFA7000C --grow
@PostProc: strip_instructions --remove 0x4c
```

**Key observations:**
- `sw a3,12(sp)` = `0xAFA7000C` (sw rt=a3(7), base=sp(29), offset=12)
- Strip the extra IDO nop BEFORE inserting, then insert, then strip the delay-slot nop
- `remap_stack_slots` handles `addiu reg,sp,offset` in addition to load/store
- IDO generates 20 instructions; after all PostProc we get 19 (no delay slot in function range)

**Also fixed**: The twin function `screen_clear` (same body but starting at the prologue)
needed the same PostProc minus the insert/strip steps.

---

## NEW: CSE-Breaking Alias Trick (March 2026)

When IDO's Common Subexpression Elimination (CSE) merges two loads of the same global variable but the target has separate loads, declare a compile-time alias:

```c
extern u8 D_800F0216;   /* real symbol */
extern u8 D_800F0216b;  /* alias — same address, different name */

/* IDO treats these as different symbols and emits separate lui+lbu for each */
if (D_800F0216 == 0) {
    /* ... uses D_800F0216 ... */
} else {
    val = D_800F0216b;  /* IDO won't CSE this with the condition check */
}
```

Then in the custom_script, rename the `D_800F0216b` relocation back to `D_800F0216` so the linker resolves both to the same address. This prevents IDO from reusing the condition-check register in the else branch.

**When to use:** When the target has separate `lui` pre-loads for the same global before a branch, but IDO reuses the already-loaded value. The symptom is a 1-2 instruction count gap where IDO is "too clever."

Discovered matching album_info_disp (2408B, 602 instructions).

### CSE-Breaking via Algebraic Equivalence

When IDO CSEs an offset computation like `arg1 * 168` used twice, write the two uses as algebraically equivalent but syntactically different expressions:

```c
/* First use: */
ptr1 = base + (arg1 * 5 * 4 + arg1) * 8;     /* = arg1 * 168 */

/* Second use (after a function call): */
ptr2 = base + ((arg1 + arg1 * 4) * 4 + arg1) * 8;  /* = arg1 * 168 */
```

IDO treats these as distinct expressions and doesn't CSE them, producing separate `sll`/`addu` chains for each. Discovered matching func_80091A20 (144B).

### Struct-Cast Copies for Inline Memcpy

IDO generates `lwl`/`lwr`/`swl`/`swr` (unaligned copy) for struct assignments of specific sizes:

```c
typedef struct { u8 d[64]; } S64;
typedef struct { u8 d[8]; } S8;
typedef struct { u8 d[4]; } S4;

/* 64-byte copy: */
*(S64 *)dst = *(S64 *)src;

/* 4-byte copy (forces 4-insn pattern vs 3 for direct assign): */
*(S4 *)&D_800EF6C8 = *(S4 *)&slot[4132];
```

Different struct sizes produce different loop structures. Discovered matching save_game_restore (1352B).

### Inline STAT Macro for Per-Case Reloads

When the target reloads a value inside each switch case but IDO hoists it before the switch, use a macro that forces per-case access:

```c
#define STAT(off) (*(s16 *)(a2 + (off) + 254))

switch (idx) {
    case 0: if (STAT(0) > threshold) ... break;
    case 1: if (STAT(0) > threshold) ... break;
}
```

IDO cannot hoist `STAT()` because `a2` may change across the jump table dispatch. Discovered matching func_80052A80 (1576B).

---

## NEW: Halfword Load via Stack Cast (March 2026)

When the target uses `lh` (load halfword) from a stack variable but IDO emits `lw` + sign-extend (`sll`/`sra`), cast the stack variable's address to `s16*`:

```c
s32 spEC;
s32 sp40;

/* Instead of: val = (s16)spEC; (produces lw + sll + sra) */
/* Use: */
val = *(s16 *)&spEC;   /* Forces IDO to emit: lh reg, offset(sp) */
val = *(s16 *)&sp40;   /* Same pattern for other stack vars */
```

This works because IDO treats the `*(s16*)&var` dereference as a halfword memory access rather than a full-word load with truncation. Discovered matching album_info_disp where the target uses `lh sp+236` and `lh sp+64`.

**Related patterns:**
- `*(u16 *)&var` forces `lhu` (unsigned halfword load)
- `*(u8 *)&var` forces `lbu` (unsigned byte load)
- `val >= 10U` (unsigned literal suffix) forces `sltiu` instead of `slti`
- Declaring as `u32` produces `srl` instead of `sra` for right shifts

---

## NEW: Type-Narrowing Cast to Prevent Peephole Folding (March 2026)

When the target ASM shows a mask operation into a **temp register** followed by a move to the
destination register, IDO's peephole optimizer did NOT fold them. This happens when the original
C used a type-narrowing cast (`u8` or `u16`) on an intermediate variable.

**Pattern to recognize in target ASM:**
```asm
move    a0,v0        # copy source to register
andi    t6,a0,0x7f   # mask into TEMP register (t6/t7/t8)
move    a0,t6        # copy temp back to destination
```

Without the cast, IDO folds this to a single `andi a0,v0,0x7f`. The cast prevents folding.

**Solution — use a narrower-typed intermediate variable:**
```c
/* WRONG (produces single andi a0,v0,0x7f): */
bustup_face_change(temp & 0x7F);

/* CORRECT (produces move + andi + move): */
unsigned char masked = (unsigned char)(temp & 0x7F);
bustup_face_change((int)masked);
```

**Why it works**: The `(unsigned char)` cast tells IDO that `masked` is a distinct u8 variable.
IDO allocates a temp register (t6) for it, generating the 3-instruction sequence. The peephole
optimizer cannot fold across the type narrowing boundary.

**Matched examples using this pattern:**
| File | Code | Target Pattern |
|------|------|----------------|
| `kega_exit.c` | `(unsigned char)(temp & 0x7F)` | `andi t6,a0,0x7f; move a0,t6` |
| `func_80090BA8.c` | `(s16)(u8)(arg0 & 0xFF)` | Byte extract through temp |
| `func_80085A74.c` | `(u8)(D_801271B3 & 0xFF7F)` | Mask through temp |

**When to apply**: Any time the target shows `andi tX,reg,MASK` where the andi destination is a
temp register (t6-t9) rather than the final destination register.

---

## NEW: Permuter-Discovered Patterns (January 2026)

These patterns were discovered through automated permuter runs on 30+ functions.

### Self-Assignment Trick (BYTE-PERFECT MATCH on Fade_In_Main)

**BREAKTHROUGH (January 2026)**: This pattern produces byte-perfect matches!

Forces the `lui v1` → `lbu v1,offset(v1)` self-overwrite pattern where the same
register is used for both the address load and the value store:

```c
/* Instead of: */
temp = D_GLOBAL;

/* Use: */
temp = (D_GLOBAL = D_GLOBAL);  /* Self-assignment forces v1 reuse */
```

**Why it works**: The self-assignment `D_GLOBAL = D_GLOBAL` forces IDO to:
1. Load the address into v1 (`lui v1, hi`)
2. Load the value into v1 FROM v1 (`lbu v1, lo(v1)`) - self-overwrite!
3. Store temp from v1

**Verified working code (Fade_In_Main)**:
```c
int Fade_In_Main(void)
{
    unsigned int temp;
    if (func_8008610C() != 0)
    {
        temp = (D_800EE79A = D_800EE79A);  /* KEY PATTERN */
        if (temp >= 126u)
        {
            D_800EE79A = 128;
            return 1;
        }
        D_800EE79A = D_800EE79A + 4;
        func_800860F0();
        return 0;
    }
    return 0;
}
```

**Post-processing required**: Frame size patch 24 → 40 bytes.

### Self-Assignment Ordering Variants

IDO treats these differently - try both orderings:

```c
/* Variant 1: Combined load/store */
v = (D_GLOBAL = *(unsigned char *)v);

/* Variant 2: Separate operations */
v = *(unsigned char *)v;
D_GLOBAL = v;
```

The combined form may generate different instruction scheduling.

### Expression Order Permutation (NEW)

**IDO treats these differently for instruction scheduling**:

```c
/* All semantically equivalent, but may compile differently: */
v = v + 4;
v += 4;
(v = v + 4);
v = 4 + v;
```

**Permuter pattern**:
```c
PERM_GENERAL(v = v + 4, v += 4, (v = v + 4))
```

Also try with global assignments:
```c
PERM_GENERAL(
    D_GLOBAL = D_GLOBAL + 4,
    D_GLOBAL += 4,
    (D_GLOBAL = D_GLOBAL + 4)
)
```

### do-while with long long Cast (Preserves Registers)

Prevents optimization while preserving variable across function calls:

```c
/* Pattern: */
do {
    preserved = arg0;
    func_A();
    func_B(preserved);
} while ((long long) 0);

/* The (long long) 0 cast prevents the compiler from optimizing the do-while away */
```

**Example (func_8009AE48 - exact match):**
```c
void func_8009AE48(int arg0) {
    int preserved;
    do {
        preserved = arg0;
        func_800A4DAC();
        func_800A4E3C(preserved);
    } while ((long long) 0);
}
```

### Empty if(1) Statements for Delay Slot Padding

Forces specific delay slot filling. **Placement matters** - try AFTER comparisons
(like `sltiu`) not just at returns:

```c
result = func();
if (1) { } if (1) { } if (1) { }  /* Padding */
return !result;
```

**For `li v0,1` placement issues**, try padding AFTER the comparison:
```c
if (temp >= 126u)
{
    if (1) { }  /* May move li v0,1 into correct delay slot */
    return 1;
}
```

**Example (func_80087DA8 - exact match):**
```c
s32 func_80087DA8(void) {
    s32 new_var;
    int new_var2;
    new_var = func_8008B060();
    if (1) { } if (1) { } if (1) { }
    new_var = new_var;  /* Self-assignment */
    new_var2 = !new_var;
    return new_var2;
}
```

### Inverted Conditions for Return Value Placement

IDO places `li v0,1` (return value) differently based on condition structure:

```c
/* This puts return value AFTER comparison: */
if (temp >= 126u) {
    return 1;
}

/* This may put return value BEFORE comparison: */
int ret = 1;
if (!(temp >= 126u)) {
    ret = 0;
}
return ret;
```

### Address-as-Integer Pattern

For forcing register reuse in address/value operations:

```c
/* Instead of: */
unsigned char val = *ptr;

/* Try: */
register unsigned int v;
v = (unsigned int)&D_GLOBAL;
v = *(unsigned char *)v;  /* Self-overwrite pattern */
```

### Pointer Indirection with Volatile

Forces specific memory access patterns:

```c
void func(s32 a0, volatile unsigned short a1, s32 a2) {
    s32 *new_var;
    s32 new_var2;
    new_var2 = a2;
    do { new_var = &a1; } while (0);
    a2++; a2--;  /* Dummy operations */
    func_call(a0 >> 4, (*new_var) >> 4, new_var2 >> 4);
}
```

---

## Critical Discovery: Hidden Register Patterns

### lui+lw/lbu BEFORE Prologue (Thunk Pattern)

**MOST COMMON ISSUE** - ~25% of remaining WIP functions have this pattern.

```asm
# Target binary - code BEFORE the prologue!
0x8004F918: lui   t6, 0x8012       # t6 = 0x80120000
0x8004F91C: lbu   t6, 0x79F0(t6)   # t6 = *(u8*)(0x801279F0)
0x8004F920: addiu sp, sp, -24     # Prologue starts HERE
0x8004F924: sw    ra, 20(sp)
0x8004F928: bnez  t6, .L_skip
```

**Why can't C match this?** The compiler ALWAYS generates the prologue first. Code before `addiu sp` is impossible in pure C.

**Solution: Use ASM**
```asm
.set noreorder
.set noat
.text
.globl func_8004F918

func_8004F918:
    lui     $t6, 0x8012
    lbu     $t6, 0x79F0($t6)
func_8004F918_body:
    addiu   $sp, $sp, -24
    sw      $ra, 20($sp)
    bnez    $t6, .L_skip
    nop
    ...
```

### Continuation Patterns (v0/v1 from Previous Function)

Some functions expect v0 or v1 to be set by the CALLER or previous function:

```asm
# Previous function ends:
    jr      ra
    li      v1, 5              # v1 = 5 (delay slot)

# This function starts expecting v1 = 5:
func_800XXXXX:
    addiu   sp, sp, -24
    beqz    v1, .L_early_exit  # Uses v1 without setting it!
```

**Solution**: These functions must be matched as ASM or merged with predecessor.

---

## Compiler Encoding Differences

### GCC vs IDO Move Encoding

**CRITICAL**: The binary uses TWO different compilers in different regions!

| Address Range | Compiler | `move` Encoding | Tool |
|---------------|----------|-----------------|------|
| 0x80041000-0x80080FFF | GCC-like | `or $d,$s,$zero` (0x25) | None needed |
| 0x80081000-0x800BFFFF | IDO | `addu $d,$s,$zero` (0x21) | `convert_or_to_addu.py` |

**How to detect**: Count OR vs ADDU encoding of `move` instructions.

**Post-process for IDO region**:
```bash
python3 tools/ido/convert_or_to_addu.py input.o output.o
```

---

## Frame Size Patching (ESSENTIAL)

### The 24 → 40 Byte Problem

**IDO 5.2 produces minimal 24-byte frames**, but the target binary often uses 40-byte frames.

```asm
# IDO output:
addiu sp, sp, -24    # Minimal frame
sw    ra, 20(sp)

# Target binary:
addiu sp, sp, -40    # Larger frame (extra arg save area)
sw    ra, 20(sp)     # Same ra offset
```

**Solution**: Use `patch_frame_size.py`
```bash
python3 tools/ido/patch_frame_size.py input.o output.o --old 24 --new 40
```

### Common Frame Size Patches

| IDO Output | Target | When Used |
|------------|--------|-----------|
| 24 → 40 | Most common | Simple functions |
| 32 → 48 | Functions with more locals |
| 40 → 56 | Functions with stack args |
| 24 → 32 | Smaller adjustment |

---

## Post-Processing Tools

### Available Tools in `tools/ido/`

| Tool | Purpose | When to Use |
|------|---------|-------------|
| `patch_frame_size.py` | Change stack frame size | Frame mismatch |
| `convert_or_to_addu.py` | OR→ADDU encoding | IDO region (≥0x80081000) |
| `fix_sll_sra_pattern.py` | Fix sign extension pattern | sll/sra uses temp vs in-place |
| `patch_ra_offset.py` | Change RA save offset | Rare RA offset mismatch |
| `strip_arg_save.py` | Remove arg save/load pairs | Extra sw/lw pairs |
| `strip_epilogue.py` | Remove extra epilogue instrs | Redundant epilogue |
| `register_remap.py` | Remap register numbers | Register allocation diff |
| `swap_beq_operands.py` | Swap beq/bne operands | Branch operand order |

### Typical Post-Processing Pipeline

```bash
# 1. Compile with IDO
./tools/ido-static-recomp/build/5.2/out/cc -O2 -G0 -mips1 -EL -non_shared -c func.c -o func.o -Iinclude

# 1b. For functions with 8-byte copy loops, disable loop unrolling:
./tools/ido-static-recomp/build/5.2/out/cc -O2 -Wo,-loopunroll,0 -G0 -mips1 -EL -non_shared -c func.c -o func.o -Iinclude

# 2. Patch frame size (almost always needed for GCC region)
python3 tools/ido/patch_frame_size.py func.o func_patched.o --old 24 --new 40

# 3. If IDO region, convert OR to ADDU
python3 tools/ido/convert_or_to_addu.py func_patched.o func_final.o

# 4. Extract and compare bytes
mipsel-linux-gnu-objcopy -O binary -j .text func_final.o func.bin
```

---

## IDO-Specific Behaviors

### 1. Register Allocation

**IDO uses $t registers across function calls** - this violates standard MIPS calling convention but is a key identifier of IDO-compiled code.

```c
/* IDO will keep values in $t6, $t7 even after a JAL */
void example(void) {
    int x = 320;    /* li $t6, 320 */
    int y = 240;    /* li $t7, 240 */
    func();         /* jal func - $t6/$t7 preserved! */
    use(x, y);      /* Uses $t6, $t7 directly */
}
```

**PSY-Q GCC** would save these to stack or use $s registers.

### 2. Stack Allocation Order

**IDO allocates stack in REVERSE declaration order**:

- First declared variable -> highest stack address
- Last declared variable -> lowest stack address (near $sp)

```c
void func(void) {
    int a;      /* Highest address (e.g., sp+40) */
    int b;      /* Middle (sp+36) */
    int c;      /* Lowest (sp+32) */
}
```

**To match stack layout**, reorder declarations:

```c
/* If you need: var1@sp+64, var2@sp+44 */
void func(void) {
    TYPE var1;  /* Declare first -> highest address */
    TYPE var2;  /* Declare second -> lower address */
}
```

### 3. Unused Variables Preserve Stack Space

**IDO keeps stack space for unused local variables**:

```c
void func(void) {
    int used;
    int unused;     /* IDO still allocates 4 bytes for this! */

    used = 5;       /* Only 'used' is referenced */
}
```

**Use this to pad stack frames**:

```c
void func(void) {
    DRAWENV draw;
    DISPENV disp;
    int _pad[4];    /* 16 bytes of padding - IDO keeps it! */

    /* ... function body (don't reference _pad) */
}
```

### 4. Custom Struct Sizes

Some games use non-standard struct sizes. For Tokimeki Memorial:

- **DRAWENV is 96 bytes** (standard PSX SDK is 92 bytes)

Check stack frame layouts to detect struct size differences:

```
If function has:  addiu sp, sp, -160
And uses:         DRAWENV at sp+64, DISPENV at sp+44
Then:             Calculate backwards to find struct sizes
```

## Register Allocation Control (February 2026)

### Overview

**Key Insight**: Liveness analysis, not variable names, controls register allocation.

For register allocation mismatches, **use post-processing tools** rather than restructuring C code.

### Primary Tool: swap_registers_auto.py

For fixing register allocation order (s1 ↔ s2, t6 ↔ t7, etc.):

```bash
python3 tools/ido/swap_registers_auto.py input.o output.o \
    --swaps "s1:s2"
```

**Example**: icon_schedule_set had s1 and s2 swapped. Manual swap fixed it perfectly.

### What You CAN Control from C

✅ **Whether to use s-registers** (via loop patterns):
```c
/* s-register allocation pattern */
s32 *ptr = (s32*)0x800FE000;  // Materialized constant -> s1
for (i = 0; i < count; i++) {  // Loop counter -> s0
    *ptr = func_call();        // ptr preserved across calls
}
```

✅ **Number of registers needed** (via overlapping lifetimes):
```c
x = a + b;
y = c + d;
result = x + y;  // Both x and y needed simultaneously -> different registers
```

### What You CANNOT Control from C

❌ **Specific s-register numbers** (s0 vs s1 vs s2) - use swap_registers_auto.py
❌ **Specific temp register choice** (t6 vs t7 vs t8) - use swap_registers_auto.py
❌ **Instruction scheduling** within basic blocks

### Variable Splitting/Merging

**Common Myth**: "Splitting variables forces multiple registers."

**Reality**: Only matters when lifetimes OVERLAP.

```c
/* IDENTICAL OUTPUT (no overlap): */
// MERGED
temp = a + b;
func(temp);      // temp dies here
temp = c + d;    // IDO reuses register

// SPLIT
temp1 = a + b;
func(temp1);     // temp1 dies here
temp2 = c + d;   // IDO reuses same register!

/* DIFFERENT OUTPUT (overlap): */
x = a + b;
y = c + d;
result = x + y;  // Both needed -> must use different registers
```

### Recommended Workflow

1. Write semantically correct C (focus on algorithm)
2. Compile with IDO
3. Post-process frame size if needed
4. **Identify register allocation differences**
5. **Fix with swap_registers_auto.py**
6. Verify match

**DO NOT** waste time restructuring C to fix register order.

## Pointer-to-Counter Idiom: Forcing Stack Allocation + Register Choice (February 2026)

### Problem

IDO at -O2 aggressively keeps local variables in registers, even in leaf functions.
When the target binary has a variable on the **stack** (loaded/stored each loop iteration),
writing a plain `s32 counter` produces register-only code with no frame allocation.

### Solution: `s32 *cp = &counter`

Taking the address of a local through a pointer forces IDO to allocate it on the stack:

```c
s32 counter;
s32 *cp = &counter;
*cp = 0x800000;
do {
    *cp = *cp - 1;
    if (*cp == 0) { /* body */ }
} while (obj->flag == 0);
```

**Key behaviors**:
- **Stack allocation**: `&counter` forces counter onto the stack (IDO can't register-promote
  a variable whose address is taken)
- **Non-volatile pattern**: Unlike `volatile`, pointer access generates the efficient
  load-decrement-branch-store(delay) pattern, NOT load-decrement-store-reload-branch
- **Register allocation side effect**: The pointer idiom changes which registers IDO picks
  for other variables. In blur_area (strSync), `s32 counter[1]` made IDO use `a1`
  for constant 1, while `s32 *cp = &counter` made IDO use `v1` — matching the target

### What DOESN'T Work for Stack Allocation

All tested approaches that FAIL to force stack allocation at -O2:

| Approach | Result |
|----------|--------|
| `s32 counter` (plain local) | Kept in register, no frame |
| `s32 counter[1]` (array of 1) | Scalar-replaced to register |
| `s32 counter[6]` (larger array) | Unused elements eliminated |
| `struct { s32 pad[5]; s32 counter; } st;` | Struct eliminated |
| `s32 *p = &locals[5]; *p = val;` | Pointer through array still scalar-replaced |
| `volatile s32 counter` | ON stack, but generates extra load/stores |
| `-Olimit 0` through `-Olimit 10` | Changes frame size (8→16) but not to 24 |
| `-Wo,-loopunroll,0` | No effect on frame layout |
| IDO 5.1 vs 5.2 | Both produce identical frame=8 |

### Frame Size Anomalies

Some target functions have **unusual frame sizes** that can't be reproduced from C alone.
Example: blur_area (strSync) has frame=24 for a leaf function with one local
(IDO normally generates frame=8). The extra 16 bytes of frame space are unexplained —
possibly from the original source having more locals, or from a post-link modification.

**Fix** (historical): The original blur_area approach used the now-**BANNED**
`patch_instruction` PostProc to manually adjust frame prologue/epilogue and
sp-relative references. `patch_stack_complete` only adjusts **argument saves**
(references above the frame), NOT local variable references within the frame.

> **POLICY NOTE (2026-05-11)**: `patch_instruction` is BANNED — no hardcoded
> opcode bytes via PostProc. For frame-size anomalies today, prefer
> `remap_stack_slots`, `rebase_store_to_sp`, `shift_stack_offsets`, or
> `patch_insn_imm16` (which patches only the imm16 field of a specific
> instruction, not whole bytes).

### Verified Example: blur_area (strSync)

```c
/* Pointer-to-counter produces v1 for constant 1 (matching target) */
s32 counter;
s32 *cp = &counter;
*cp = 0x800000;
/* ... loop using *cp ... */

/* Array-of-1 produces a1 for constant 1 (NOT matching target) */
s32 counter[1];
counter[0] = 0x800000;
/* ... loop using counter[0] ... */
```

**Post-processing for blur_area**:
1. `patch_stack_complete --old 8 --new 24` (frame + arg save)
2. 3x `patch_instruction` to fix counter sp-offsets (sp+4 → sp+20)
3. 2x `patch_instruction` to fix delay slot and branch offset

## uopt Hidden Flags for Debugging (February 2026)

IDO's uopt optimizer accepts **30+ undocumented flags** that can help debug matching failures.
The complete inventory is in the project's private docs (`UOPT_FLAGS_REFERENCE.md`).

### Quick Reference: Most Useful Flags

```bash
# Prevent loop unrolling (most common)
-Wo,-loopunroll,0

# Prevent constants from living in registers (WARNING: changes code dramatically)
-Wo,-no_const_in_reg

# Disable tail call optimization
-Wo,-notail

# Change register allocation heuristics
-Wo,-noprecolor
-Wo,-noheurAB
-Wo,-no_r23              # Reserve s7 (untested)

# Change expression optimization
-Wo,-doassoc             # Enable reassociation
-Wo,-docopy              # Enable copy propagation
-Wo,-nordstore           # Disable redundant store removal
```

### Testing a Flag

```bash
# Compile with flag
./tools/ido-static-recomp/build/5.2/out/cc -O2 -Wo,-FLAG -G0 -mips1 -EL -non_shared \
    -I include -c source.c -o /tmp/test.o

# If "unrecognized option" appears, the flag is not valid
# Compare output size to see if anything changed:
mipsel-linux-gnu-objcopy -O binary -j .text /tmp/test.o /tmp/test.bin && wc -c /tmp/test.bin
```

### Key Limitations

- **No LICM disable flag exists** — loop-invariant code motion cannot be disabled via flags
- `-no_const_in_reg` matches target SIZE in some cases but produces completely wrong code (e.g., 0.4% instruction match on iris_show)
- Most register allocation flags have no visible effect on simple functions; test on specific problem functions

---

## Loop Unrolling Control (February 2026)

### The Problem

IDO at `-O2` aggressively unrolls small loops, particularly 8-byte copy loops:

```c
/* This C code: */
for (i = 0; i < 8; i++) {
    dst[i] = src[i];
}

/* IDO -O2 generates (UNROLLED 4x): */
lbu t6, 0(src)
sb  t6, 0(dst)
lbu t7, 1(src)
sb  t7, 1(dst)
lbu t8, 2(src)
sb  t8, 2(dst)
lbu t9, 3(src)
sb  t9, 3(dst)
/* ... branch back, processing 4 bytes per iteration */
```

But the original binary often has **rolled loops**:

```asm
/* Target (ROLLED): */
    li  $v1, 7
    li  $a3, -1
.loop:
    lbu $v0, 0($a0)
    addiu $a0, 1
    addiu $v1, -1
    sb  $v0, 0($a1)
    bne $v1, $a3, .loop
    addiu $a1, 1      ; delay slot
```

### The Solution: `-Wo,-loopunroll,0`

```bash
./tools/ido-static-recomp/build/5.2/out/cc -O2 -Wo,-loopunroll,0 -G0 -mips1 -EL -non_shared \
    -I include -c source.c -o output.o
```

### Flag Values

| Flag | Effect |
|------|--------|
| `-Wo,-loopunroll,0` | **Disable unrolling** (produces rolled loops) |
| `-Wo,-loopunroll,1` | Same as 0 |
| `-Wo,-loopunroll,2` | 2x unrolling |
| `-Wo,-loopunroll,4` | 4x unrolling (default at -O2) |

### What Does NOT Work

These techniques were tested and **DO NOT prevent unrolling** in IDO 5.2/5.3:

| Technique | Result |
|-----------|--------|
| `#pragma unroll 1` | Ignored (only works in MIPSpro 7.x) |
| `#pragma no_unroll` | Ignored |
| `volatile` loop counter | Creates complex loop peeling (worse) |
| `void*` aliasing tricks | No effect |
| `-O1` optimization | Prevents unrolling but uses stack for counter |

### When to Use

Use `-Wo,-loopunroll,0` when:
- Target has rolled `lbu`/`sb` loops (single byte per iteration)
- Permuter score is high (5000+) due to loop structure differences
- Function copies small fixed-size data (8, 16, 32 bytes)

### Affected Functions

These permuter compile scripts have been updated with `-Wo,-loopunroll,0`:
- `build/decomp-permuter/func_80089E50/compile.sh`
- `build/decomp-permuter/func_8008A004/compile.sh`
- `build/decomp-permuter/func_80088C74/compile.sh`

---

## SLL/SRA Sign Extension Pattern

### The Problem

IDO sometimes generates sign extension with a temp register, but target uses in-place:

```asm
# IDO output:
sll $t6, $a0, 16     # temp = arg << 16
sra $a0, $t6, 16     # arg = temp >> 16 (sign extend)

# Target binary:
sll $a0, $a0, 16     # arg = arg << 16 (in-place)
sra $a0, $a0, 16     # arg = arg >> 16 (in-place)
```

**Solution**: Use `fix_sll_sra_pattern.py`
```bash
python3 tools/ido/fix_sll_sra_pattern.py input.o output.o
```

---

## General Matching Techniques

### Loop Type Detection

**For/While Loop** (condition check at top):

```asm
        li      $t0, 0          ; i = 0
        b       .Lcond          ; jump to condition
.Lbody:
        ; ... loop body ...
        addiu   $t0, $t0, 1     ; i++
.Lcond:
        slti    $at, $t0, 10    ; i < 10?
        bnez    $at, .Lbody
```

**Do-While Loop** (condition check at bottom):

```asm
        li      $t0, 0          ; i = 0
.Lbody:
        ; ... loop body ...
        addiu   $t0, $t0, 1     ; i++
        slti    $at, $t0, 10    ; i < 10?
        bnez    $at, .Lbody
```

### Register Pressure Tricks

If register allocation doesn't match, try:

1. **Add temp variables** to force spills:

```c
/* Original */
result = a + b + c + d;

/* Force different allocation */
int temp1 = a + b;
int temp2 = c + d;
result = temp1 + temp2;
```

2. **Reorder expressions**:

```c
/* These may produce different code: */
x = func(a, b) + func(c, d);
x = func(c, d) + func(a, b);  /* Try reversed */
```

3. **Use volatile** to prevent optimization:

```c
volatile int temp = value;
```

### Common Mismatches and Fixes

| Symptom | Likely Cause | Fix |
|---------|--------------|-----|
| Stack frame size wrong | Missing padding or wrong struct size | Add `int _pad[N]` or use `patch_frame_size.py` |
| Wrong registers used | Declaration order | Reorder variable declarations |
| Extra load/store | Register spilling | Reduce local variables or reorder |
| Loop structure different | Wrong loop type | Try for/while/do-while variants |
| Branch direction flipped | Condition inverted | Negate the condition |
| OR vs ADDU for move | Wrong region | Use `convert_or_to_addu.py` for IDO region |
| Code before prologue | Thunk pattern | **Must use ASM** |

### Using decomp-permuter

For stubborn functions, use decomp-permuter to try variations:

```c
void func(void) {
    PERM_GENERAL(
        int a; int b;,
        int b; int a;
    )

    PERM_GENERAL(
        for (i = 0; i < n; i++),
        i = 0; while (i < n)
    )
}
```

### Permuter Score Interpretation

| Score Range | Meaning | Action |
|-------------|---------|--------|
| 0 | Perfect match | Promote to matched/ |
| 1-20 | Near match, 1-2 instructions off | Minor tweaks or post-process |
| 21-100 | Close, structure correct | Try more variations |
| 101-500 | Medium, some differences | May need different approach |
| 500+ | Far, fundamental mismatch | Consider ASM or re-analyze |

### Permuter Best Practices (January 2026)

Based on running 30+ functions through permuter:

1. **Start with correct logic** - Permuter can't fix wrong algorithms
2. **Use 2-4 workers** - More workers = diminishing returns
3. **30-60 minute runs** - Most improvements found in first hour
4. **Check intermediate results** - Score 75-100 often fixable manually
5. **Watch for patterns** - Similar functions often need similar tricks

---

## Automated Matching Workflow

### Using auto_match.py

The fastest way to verify a match:

```bash
# Single function
python3 tools/decomp/auto_match.py src/main/wip/bustup_cd_read.c

# Auto-move to matched/ if successful
python3 tools/decomp/auto_match.py src/main/wip/bustup_cd_read.c --move-on-match

# Batch mode
python3 tools/decomp/auto_match.py --batch src/main/wip/func_8004*.c
```

### What auto_match.py Does

1. Extracts function address from filename
2. Compiles with IDO 5.2 (`-O2 -G0 -mips1 -EL`)
3. Auto-detects region (GCC vs IDO)
4. Tries frame size patches (24->40, 32->48, etc.)
5. Applies OR->ADDU if IDO region
6. Compares bytes (masks relocations)
7. Reports MATCH/MISMATCH with diff

---

## Quick Reference

| Technique | When to Use |
|-----------|-------------|
| Reorder declarations | Stack offsets wrong |
| Add unused padding | Stack frame too small |
| Try different loop types | Loop structure different |
| Add temp variables | Register allocation wrong |
| Check struct sizes | Offsets consistently off by N bytes |
| `patch_frame_size.py` | Frame size mismatch |
| `convert_or_to_addu.py` | IDO region (>=0x80081000) |
| `fix_sll_sra_pattern.py` | Sign extension temp vs in-place |
| `-Wo,-loopunroll,0` flag | 8-byte copy loops unrolled by IDO |
| Use ASM | Code before prologue (thunks) |

---

## File Organization

### Matched Functions

- **C source**: `src/main/matched/func_XXXXXXXX.c`
- **ASM source**: `asm/main/matched/func_XXXXXXXX.s`
- **Config**: `config/level3_functions.json`

### WIP Functions

- **C source**: `src/main/wip/func_XXXXXXXX.c`
- Move to matched/ only after byte-perfect verification

### Sync Tools

```bash
# Regenerate config from matched directories
python3 tools/build/generate_level3_config.py
```

---

## NEW: Opaque Expression Trick for s-Register Allocation (March 2026)

When IDO sees `savedptr = textbuf` where `textbuf` is a local array (`sp+N`), it treats savedptr as a **rematerializable constant** and refuses to allocate an s-register — computing `addiu reg,sp,N` inline every time is cheaper than saving/restoring an s-register.

To force s-register allocation, make the expression **opaque at compile time**:

```c
savedptr = &textbuf[(u32)a2_param * 0];  /* IDO can't fold (u32)x * 0 */
```

IDO does NOT constant-fold `(u32)x * 0` (though it folds `x & 0`, `x - x`, and `x >> 16 >> 16`). The expression is identity at runtime (`textbuf + 0 = textbuf`) but opaque at compile time, forcing IDO to allocate savedptr to an s-register.

**Key constraints:**
- `& 0` is folded (doesn't work)
- `x - x` is folded (doesn't work)
- `(u32)x * 0` is NOT folded (**works!**)
- Adding too many `textbuf[N]` accesses (instead of `savedptr[N]`) reduces savedptr usage below IDO's threshold, killing the s-register allocation
- Balance: use `textbuf[0]` for byte checks (matching target's `lbu v0,124(sp)`), keep `savedptr` for pointer-passing to functions (matching target's `move a3,s0`)

**Discovered via serifu_cmd_exec** (2219-instruction function). The trick added s1 allocation for savedptr with zero instruction count penalty. Combined with s0↔s1 post-processing swap, this recovered +115 multiset matches.

---

## NEW: Unreachable return 0 After Switch (March 2026)

When all `switch` cases terminate with `return` or `goto`, the `return 0;` after the switch closing brace is unreachable. But IDO still emits `b epilogue; move v0,zero` (2 instructions) for it.

**Fix:** Remove the unreachable `return 0;`. IDO replaces the 2-instruction branch with a 1-instruction nop (alignment padding), which can be stripped by `strip_trailing_instructions`.

This recovered the exact instruction count for serifu_cmd_exec (2220 → strip → 2219 = target).

---

## NEW: Per-Region Register Swaps (March 2026)

When a temp register is used in one region of a function but dead at region boundaries, a per-region register swap is safe and can fix allocation differences that global swaps can't.

Example: `addiu t3,sp,88` (tbl base pointer) in the prologue — t3 is dead after the struct copy completes. A t2↔t3 swap applied only to insns 0-19 fixes the tbl register without affecting the rest of the function where t2 and t3 are used differently.

**Implementation:** In `custom_script`, add a swap loop with a restricted instruction range:
```python
for i in range(min(PROLOGUE_END, n_insns)):
    # swap t2↔t3 in register fields
```

---

## NEW: Range Propagation Defeat — Mask Trick (March 2026)

When IDO's range propagation determines a switch variable is already in range (e.g., from a prior comparison), it drops the `sltiu` guard instruction. If the target binary has the guard, you need to defeat range propagation.

**Solution**: Apply a mask that widens the perceived range:

```c
/* IDO drops sltiu guard (range propagation knows v0 < N) */
switch (v0) { ... }

/* IDO keeps sltiu guard (mask defeats range propagation) */
switch (v0 & 0x7F) { ... }
```

The `& 0x7F` adds an `andi` instruction that makes IDO uncertain about the value range, forcing it to emit the `sltiu` guard. At runtime, if values are already < 128, the mask is a no-op.

---

## NEW: Pre-Scheduler Register Swap (March 2026)

When using `scheduler_normalizer` with a subsequent register remap custom_script, swapping s-registers **before** the scheduler runs can improve matching. The scheduler's instruction reordering is sensitive to which registers are used where, so having the correct register names before reordering produces better results than swapping after.

**Pipeline order**:
1. `swap_registers` (fix s-register assignment)
2. `scheduler_normalizer` (reorder instructions)
3. `custom_script` (per-block register remap for remaining differences)

---

## NEW: Volatile-to-s-Register Promotion (March 2026)

When the target uses an s-register to hold a constant but IDO materializes it inline each time:

```c
volatile s32 tmp = N;
sN = tmp;  /* Forces constant into s-register via stack round-trip */
```

The `volatile` forces a store-then-load, and the subsequent assignment to a variable used across calls forces s-register allocation. Without `volatile`, IDO would constant-fold and never allocate the s-register.

---

## NEW: IDO52_NO_CSE_OPCODES — Per-Opcode CSE Control (March 2026)

The `IDO52_NO_CSE` env var disables CSE for all operations of a given type. The newer `IDO52_NO_CSE_OPCODES` provides finer control with 25 named opcodes:

```bash
# Disable CSE for additions and multiplications only
IDO52_NO_CSE_OPCODES=add,mpy ./tools/ido-static-recomp/build/5.2/out/cc ...
```

Available opcodes: `add`, `sub`, `mpy`, `div`, `rem`, `mod`, `shl`, `shr`, `and`, `or`, `xor`, `neg`, `not`, `cvt`, `eq`, `neq`, `lt`, `leq`, `gt`, `geq`, `band`, `bor`, `bxor`, `bnot`, `comma`.

---

## NEW: prepend_thunk — Dual-Entry Function Matching (March 2026)

For functions where a short ASM "thunk" (1-3 instructions) falls through into a compiled C body, use `prepend_thunk` to glue the thunk prefix onto the compiled output:

```
@PostProc: prepend_thunk --insns 0x3C050012 0x8CA5B1C4
```

This prepends the raw instruction words before the compiled `.text` section, adjusting all branch offsets and relocations. The C code only needs to match the body after the thunk.

---

## NEW: Block-Aware Scheduler Normalizer (March 2026)

The `scheduler_normalizer` by default can swap instructions across basic block boundaries, which sometimes creates false matches. The `--block-aware` flag restricts reordering to within basic blocks only:

```
@PostProc: scheduler_normalizer --block-aware
```

This prevents the scheduler from moving instructions past branch targets or branch instructions, producing more reliable matches for functions with complex control flow. (The investigation is kept in the project's private archive.)

---


## NEW: The Big-Function Playbook (2026-07-01/02)

Consolidated from the EVENT/func_8014C3A0 investigations (1,712 insns, largest
function in the project) and the parallel TEL/DATE/SHUGAKU batch that produced
4 byte-perfect multi-KB functions (incl. DATE/func_8014494C, 4,920 B). Apply
these IN ORDER on any large semi — they are cheap and each fixed double-digit
diff counts.

### 0. Re-derive semantics from the target ASM — trust nothing
The single highest-yield step. Three of the batch's functions had existing C
that was **semantically wrong** while still passing count checks:
- EVENT/func_8014C3A0's C collapsed dispatch arms 3..10 into one chain
  (wrong globals, missing state checks) yet matched 1712/1712 instructions.
- TEL/func_80136620's header said "COMPLETE & CORRECT DECODE — do NOT
  re-derive"; re-derivation found a wrong-arg RNG call, %hi/%lo-carry global
  misreads (`D_800B9774` not `D_800C9xxx`), a missing case fall-through, and
  masked-reloc misreads.
- Several "50%" functions were `pad_trailing_nop` fake stubs whose honest
  rewrite went straight to byte-perfect.
Read every branch target and delay slot of the target `.s`, build a
block-by-block condition/store map, and only then compare with the existing C.

### 1. Control-flow shape: structured `&&` chains, never goto chains
`if (a != K) goto skip;` chains compile to inverted `beq +2; b skip` pairs
with rematerialized constants in the delay slots. The target idiom (direct
`bne → skip`, fall-through continue, `$at` compare constants) comes from
structured code:
```c
if (st == 1 && ph >= 3U && ph < 6U && flag == 0 && D_X == 13) { ... return 0; }
```
`switch` over ~11 dense cases emits a jr-table — wrong when the target shows
an ascending compare chain (that chain is if/else-if, proven by the const
webs being CSE-shared with arm-internal compares).

### 2. Word-sized (u32) temp locals — u8 locals insert phantom moves
cfe routes a load into a `u8` local through an expression temp plus a copy;
under register pressure this becomes a `move vN,aN` **web split** (+1 insn,
repeated per block — 25 phantom instructions on the EVENT dispatcher, which a
earlier investigation had misdiagnosed as an "unfixable register cascade").
Declare scratch locals `u32` (or `int` when the target compares signed).
Keep `u8` ONLY where the target shows real byte spills (`sb`/`lbu` to sp).
Related: `u8` self-assign (`x = x >> 2`) adds a phantom `andi 0xff` — use a
u32 local and `(s32)` casts on compares to keep `slt` vs `sltu` right.

### 3. volatile globals + explicit per-block temp locals pin load placement
When the target reloads a global per block but reuses it within a block (and
across specific sibling blocks), plain globals over/under-CSE. The control
knob is `volatile` extern + explicit locals placed exactly where the target
loads:
```c
f6 = D_800B1AF6;   /* block-top triple, lbu order = source order */
f5 = D_800B1AF5;
ph = D_800B0896;
if (st == 17 && ph >= 6U && ... && f5 == 1 && f6 == 10) { ... }
/* next sibling block reuses f5/f6/ph — no reload, like the target */
```
Block-1-of-an-arm loads nested INSIDE the state check (`if (st == 1) { ph =
...; if (ph >= 3U ...) }`) reproduce the load-after-branch placement.

### 4. ⭐ Constant-compare TYPE steering (`== 1` vs `== 1U`)
Whether IDO holds a small compare constant in a long-lived register web
(`bne t8, a0` with `a0`=1 held across the whole function) or rematerializes
`addiu $at, K` per use follows the **signedness of the comparison**:
- `u8_global == 1` — the u8 promotes to int, the compare is SIGNED → fresh
  `$at` materialization at each site.
- `u8_global == 1U` or comparing via a u32 local — UNSIGNED → the constant
  joins the shared unsigned const web (one `li` + register compares).
Read each target compare site and match it individually: register compare →
make yours unsigned; `addiu at,K` + compare → keep yours signed. This killed
16 identity diffs on EVENT in one pass. **Do NOT bulk-U-ify** — merging all
sites into one giant web changes its live range and reshuffles placement
catastrophically (proven twice).

### 5. Breaking value-numbering for repeated equality tests
For a target that recomputes `(x == 12)` three times where your C CSE-caches
it: the three forms must be MUTUALLY non-VN-equal. `x == 12`,
`(x ^ 12) == 0`, and `!(x ^ 12)` all emit the same `xor; sltiu` pair but
don't unify. Commutative swaps (`12 == x`) canonicalize — they do NOT break
CSE.

### 6. Bit-test idioms
Target `sll rX,rY,30; bgez` = write `(g << 30) < 0` (signed global), not
`(g & 2)` (which emits `andi; beqz`). Inversely `andi rX,rY,1; bnez` = plain
`(g & 1)` — but if a const-1 register web exists (see §4), IDO may use the
reg-AND form; the `&`-operand's type steers this too.

### 7. uopt hook workflow (PRECOLOR / CONST_INREG) for web-level residue
See `docs/reference/UOPT-HOOKS-REFERENCE.md` (Hooks 14b/16). Workflow:
1. Sweep bittab indices: compile with `IDO52_PRECOLOR_LIVEIN=t9:N` for
   N=0..~140 and md5-diff each `.o` against baseline — the Ns that change
   output are live webs. Identify them by looking at what changed.
2. Greedy-search reg:bitpos combos scoring exact-word matches, **with a
   semantic guard**: a byte-greedy search WILL find combos that pin a foreign
   web into `$a0` and silently corrupt call arguments (verify the prologue
   still loads the real args before accepting any combo). Never precolor
   into `a0`.
3. `IDO52_CONST_INREG_VALUES=1` (now supports `VALUE[:TYPE[:FROM[:TO]]]`,
   plus `IDO52_CONST_INREG_DEBUG=1` to see per-query value/type) forces
   constants into allocatable webs — this converted +350 exact words on
   EVENT when combined with type steering.
4. Bitpos values are source-layout dependent — RE-SWEEP after every source
   edit; stale combos score garbage.

### 8. Assorted batch finds
- **value×const `multu` in NON-loop code is crackable for ≥3 uses**: three
  separate `volatile` reloads × an unsigned `*56` makes uopt CSE the constant
  into a register → `li 0x38; multu ×3` (TEL/func_80136620; overturns the
  older "value×const non-loop = SKIP" note for the multi-use case).
- 4-byte GPU primitive writes = a **nested-struct union passed BY VALUE**
  (produces direct sp-offset addressing + the arg-home `sw a0,(sp)`).
- `*(volatile s8 *)&D_X` casts materialize the full address (`lui; addiu;
  lb 0(reg)`); declaring the extern itself `volatile` gives direct
  `lui; lb lo16` (TEL/func_80132FCC, 5 resolver words).
- **Named-extern conversion can make things WORSE**: on TEL/func_80132274 the
  raw-cast `(s32)0x800EE834` constant form was load-bearing (named extern
  let uopt CSE the %hi and rotated 30 words). Test both forms.
- Reorder-only `custom_script`s (pure permutation of compiled words with
  reloc entries moved alongside, sanity-checking opcodes before moving) are
  the sanctioned way to fix as1 lui-hoisting placement the source can't
  reach — see `tools/overlay/custom/reorder_TEL_80136620_luihoist.py` as the
  template (docstring documents the policy argument).

### 9. Measuring progress on huge functions: don't chase raw exact%
Raw exact% collapses whenever a ±1 shift excursion moves every subsequent
word — track instead: (a) instruction-count parity, (b) call-site (jal)
alignment, (c) **number of mnemonic-identity diffs under shift-aware
alignment**, and (d) number of shift transitions. A change that drops exact%
but reduces (c)/(d) is progress (the register fields are the paintable
class). Method: align windows of 6 mnemonics allowing ±3 shift, then list
sites where the mnemonic differs — this separates unfixable identity diffs
from paintable register diffs.

### 10. Known remaining wall: uopt const-def PLACEMENT
Four residual patterns on EVENT/func_8014C3A0 that no available lever
reproduces (full analysis in that file's header): single hoisted const def
serving 2 cross-block uses (uopt only head-hoists single-use windowed
consts); ugen temp coalescing that blocks/permits a load-shadow fill; a
const web (`t4=8`, 1 def / 7 uses) that is NOT created via `f_constinreg` at
all (every query forced = inert — finding its mechanism is the top open
research item); and phi-copy `move`s in assign-and-test chains. These are
nop↔instruction identity differences: patching them would be banned
painting, so they must come from codegen.

## Closed Doors (Techniques Proven NOT to Work)

- **Whole-function `scheduler_normalizer` on 1,000+ insn functions**: rotates
  branch WORDS whose encoded relative offsets then decode to wrong targets —
  semantically broken output that scores "better". `--block-aware` also
  mangles call-adjacent blocks at this scale. Use it only on small windows.
- **Bulk U-suffixing all constant compares**: merges every site into one huge
  const web → live-range explosion, placement chaos (see type-steering §4 —
  per-site precision is mandatory).
- **`IDO52_UGEN_TREG_ORDER` for placement problems**: it renames temps but
  never changes fill/serialization decisions (swept exhaustively incl.
  restricted lists on the EVENT lui-shadow slot).
- **Precoloring searches without a semantic guard**: byte-greedy combos can
  hijack `$a0` and corrupt call arguments while scoring higher.
- **`s32` scratch locals on IDO overlay dispatchers**: signed compares kill
  the unsigned const-web sharing entirely (every compare rematerializes).

These strategies have been exhaustively tested (March 2026) and confirmed to fail.
Do not retry them.

### Scope Shadowing / { } Blocks Have Zero Effect

IDO's graph-coloring allocator operates on SSA form, completely ignoring C-level
lexical scopes. `{ }` blocks, scoped declarations, and nested blocks with separate
variable lifetimes have ZERO effect on register allocation.

### Non-Existent -Wo Flags

The following flags are **rejected by uopt** ("unrecognized option"):
- `-Wo,-noivop` (hypothesized induction variable opt disable)
- `-Wo,-nohoist` (hypothesized LICM disable)
- `-Wo,-nopreopt` (hypothesized pre-optimization disable)
- `-Wo,-noswp` (hypothesized software pipelining disable)

There is **no way** to disable LICM or induction variable optimization via flags.

### IDO52_RESERVE_SREGS Cannot Reduce S-Reg Count

Reserving s-registers (e.g., `IDO52_RESERVE_SREGS=s0,s1,s2`) shifts allocation to
s3+ but IDO still saves s0-s6 (contiguous range). The hook shifts WHICH s-regs are
used but cannot reduce the total count or prologue/epilogue overhead.

### Volatile on Loop Bound Increases Instructions

Making a loop bound volatile adds a load each iteration, going in the WRONG direction
(e.g., func_8005970C: target=191, default=192, volatile=193).

### Ternary Goto Chains Cannot Produce Compact beq Dispatch

Goto-based dispatch produces correct `beq` polarity but adds 4 extra instructions.
If-else produces correct count but uses `bne`. No C89 construct produces both correct
count AND correct branch polarity for the beq/beq/b pattern with delay-slot tricks.

### Declaration Order Has Zero Effect on Register Allocation

IDO's graph coloring assigns registers by live range weight and interference, not
declaration order. Verified by reversing all variable declarations with identical output.

### `register` Keyword Is Ignored

IDO 5.2 treats `register` as a pure hint. For constants, completely ignored because
constant propagation runs before register allocation.

---

## Diagnosing a near-match: is it a codegen blocker or a C bug? (2026-06-13)

A heavily-PostProc'd function still showing a small residual (1–3 diff words) is, more
often than the old notes assumed, a **mis-decompiled C** problem — not an
irreducible allocator/scheduler difference. Read the **target** disassembly's delay
slots and branch targets before giving up:

**Hidden argument (delay-slot `move` into an arg register).** If the target has
`jal func; move aN, vM` (the `move` in the branch-delay slot) and your compile spills
a value across the call, the value is a **missing argument**. The C passed it to the
wrong call (so IDO kept it live → spill). Fix: prototype the real consumer and pass it.

```
target:   jal func_80057780;  move a1, v0     # t lives in a1 — it's arg #2
your C:   func_80057780(1);   func_429fc(t);  # t passed to func_429fc → spill
fixed C:  func_80057780(1, t); func_429fc();  # no spill → reloc_only
```
Caveat: if the consumer is also called 0-arg elsewhere in the same file, IDO's cfe
enforces one prototype per file (block-scope decls leak) and you can't have both —
that function stays semi unless the consumer is called only at the spill site.

**Wrong control flow (`lw ra` in a branch-delay slot).** If your compile fills a
branch-delay slot with `lw ra, N(sp)` and the target has a `nop` there, the branch
TARGET differs — the C control flow is wrong. Common case: a call that should be
unconditional was placed inside an `if`:

```
target bne jumps to the 2nd call → only the 1st is conditional
your C:   if (cond) { A(); A(); }
fixed C:  if (cond) { A(); }  A();     # 2nd call unconditional → reloc_only
```
A dead trailing `move aN, a0` left over after the fix clears with `nop_at_offset`.

**Detector.** Dry-run every close semi (`gen_reg_patches … --dry-run`), decode the
`OTHER [i] target=… compiled=…` words (opcode/funct), and flag: target `move` into
a0–a3 (hidden arg), compiled sp-relative `sw/lw` the target lacks (spill), or a dead
trailing `move` (nop-able). These three signatures cover most cheap semi→reloc flips.

---

## Resources

- **decomp.me** - Upload functions for community help (supports IDO)
- **asm-differ** - Side-by-side comparison tool
- **decomp-permuter** - Automated variation testing

## The Monster Campaign Addendum (2026-07-03) — 12 byte-perfect 800+-insn functions in one day

Thirteen runs on the biggest semis (TEL/80136620 1659i, DATE/801478D8,
RPG_BAT/80133108, EVENT/80159DC0+80158758, BUNKAKEN/8014918C+80143DF8,
DATE/80146230, DATE2/80133AF4+801350A8, TEL/80132FCC+80132274 = 12 wins;
+3 honest re-derives NAME_ENT/8014343C, ENDING/8013A318, GEKO/80142BCC).
Every rule below was load-bearing in at least two wins.

### The meta-rule: the header can be wrong, the chain goes stale, the mask hides
- **9 of 13 headers misdiagnosed their residual** ("allocator wall" /
  "scheduling divergence" that was actually C bugs). Re-derive first.
- **Run the test and read stderr FIRST**: one stale `patch_insn_reg_field`
  entry (`field rt is N, expected M`) silently aborts the ENTIRE PostProc
  chain (even patch_stack stops applying) — several "44%" functions were
  healthy decomps behind an aborted chain.
- **Check the RAW compiled insn count**: the harness truncates over-length
  compiles to target size, so "N/N insns" can hide a 938-for-923 compile.
- **A `pad_trailing_nop` on a semi is a fake** — the deficit it hides is a
  real structural bug (missing fallthrough, phantom increment, dead store).

### scheduler_normalizer is an ANTI-PATTERN on big multi-case functions
Load-bearing in 7 wins to REMOVE it. Whole-function runs (a) drag same-shape
masked-identical `lui`/`sh %lo(X)` words ACROSS case bodies, carrying
wrong-symbol relocs into painted-correct positions (invisible to the masked
test, resolver-only); (b) corrupt branch offsets via the permutation
fallback; (c) old `swap_instructions` in the header are often COMPENSATORS
for scheduler damage — with it gone they re-cross HI16 pairs (remove both).
Replacements that worked: no scheduler + explicit `swap_instructions`
(offsets = word*4 + 0x10); per-case `--fence` windows from jr-table
boundaries; `--block-aware` is acceptable on the DATE/DATE2 class only.
Prove drag with per-stage `.o` diffing (`--keep`, pp0..pp6).

### Case maps: verify EVERY switch against extract_jr_table
Wrong case grouping was the decisive bug in 5 wins and is invisible to
.text comparison. Expect: shared multi-label bodies, EMPTY cases (advanced
by a shared tail counter — remove phantom increments), goto-only fall-in
bodies with NO label, `break` where the target falls through
(BUNKAKEN/80143DF8 case 26→27), distinct default. Sparse value ladders
(95/96/97) must be `switch`, not if/else. RUNTIME tables (extract returns
zeros) do NOT block byte-perfect .text: manual `@RodataBase: 0xADDR` covers
outer+inner tables in one blob (+4 negative control must FAIL).

### The C-shape levers (each fixed a specific diff class)
- **Signedness keys IDO const-webs, not width**: `*(u16 *)(p+off) = 160`
  JOINS the u8-160 web (one `li` reused for sh+sb); `*(s16 *)` splits it
  (fresh `li v1,-96`). Literal casts are inert; only the POINTER TYPE counts.
- **volatile on the u8 SOURCE of an `(s16)(x+k)` sum** makes the tree
  non-duplicable → single sll/sra cvt web. This CRACKS the "phi-copy
  (s16)(x+k)" blocker class (proven 3×). Volatile is per-site: right on
  counters/state bytes, WRONG on some dir bytes (hijacks allocation) — test
  both ways.
- **Counter RMW through a reused parameter** (`idx = D; idx += 1; D = idx;`)
  colors the arg register + call-spill exactly like the target and can erase
  hundreds of paint entries.
- **`if (arg0) {} if (arg1) {}` empty-ifs** suppress dead arg-home prologue
  stores for unused params; **`if (x) {}` use-injection** keeps an
  mflo/move register copy alive that uopt would coalesce.
- **compare-on-a-copy** `c2 = c; if (c2 == K) f(c);` splits compare-use from
  arg-use → ugen emits the target's `move aN,tX` in the jal delay. The
  INVERSE also occurs (a copy in old C causing a phantom move — remove it).
- **Self-assignment `D = D;`** survives uopt and emits the target's
  redundant re-store in a branch delay slot.
- **In-place global increments** (`D += 2`), never a phantom local.
- **Assignment-expression** `if ((g = cnt & 0xFFFF) == K)` avoids an
  un-coalesced `move v0`; masked-temp+return `(n=(D=cnt&0xFFFF))==K ...
  return n` is the counter-tail shape.
- **Pointer-local keystone** `p = &D_x;` steers uopt's speculative lui into
  a load-delay slot (prevents an early-`lw ra` epilogue split).
- **De-volatile non-truly-volatile globals** to re-enable the lui-hoist the
  target shows (empty delay slots where we fill).
- **Cross-call const/LDA CSE** (target rematerializes per call, we cache in
  s-regs): `@CompileEnv: IDO52_CONST_NOINREG_VALUES=<v1,v2,...>` +
  `IDO52_LDA_NOINREG_CLASSES=1,4` (1=stack/auto `&buf`, 4=extern-symbol
  LDA). This cracks the sp-LDA remat / zero-sreg class (NAME_ENT/8014343C).
- Whole-local **struct assignment** reproduces lwl/swl unaligned copy tails
  (27-byte align-1 struct = 6×lw/sw + lwl/swl at +26); member lvalues
  degrade to swl/swr. K&R static helpers may need `return dx` to force the
  entry sll/sra sign-extend.

### Final-mile classes (only the resolver sees them — ALWAYS finish on it)
1. Swapped identical-word HI16/`lui $at` pairs → `swap_instructions`.
2. Crossed relocs on masked-identical stores (uopt emission order vs
   original): a reloc-symbol-permutation custom_script moving hi16/lo16
   units in tandem is the minimal allowed reorder
   (`tools/overlay/custom/relocfix_DATE_80146230.py` is the template;
   `is_painting_script` must return False).
3. %hi-carry on remat constants: `patch_insn_imm16` completing the carry
   (`lui 0x800e`+`addiu 0xe834` → `lui 0x800f`) is byte-equal AND fixes the
   semantics.
4. `gen_reg_patches` APPENDS to the FIRST paint layer — consolidate to ONE
   layer regenerated LAST after any reorder edit (~3 rounds).

### Debunked blocker classes (remove from mental SKIP list)
- phi-copy `(s16)(x+k)` → volatile-on-source (above).
- runtime-relocated jr tables → `@RodataBase` (above); ram.bin NOT needed
  for matching.
- sp-LDA spill-vs-remat / zero-sreg → NOINREG hooks (above).
- "raw `.word` opcodes = foreign toolchain" (ENDING/8013A318 triage) →
  capstone decode showed 0 invalid encodings; old disassembler failure.
- lwl/swl "memcpy-intrinsic-only" → align-1/2 struct by-value assignment.

### Verification honesty (what "byte-perfect" means)
Strip ALL @PostProc → the raw C must compile to the SAME instruction count
and the SAME operation multiset (opcode+funct histogram) as the target;
PostProc may only rename register fields, reorder, and permute relocs. The
win gate is `resolve_overlay_relocs.py` strict compare (name-derived link,
no masking); `@RodataBase` honesty = +4/+0x10000 negative controls FAIL.
NOTE: resolve_overlay_relocs was fixed 2026-07-03 to pin `.text` at the
exact base (`SUBALIGN(4)`) — ld's 16-byte alignment was shifting
double-thunk functions and faking "jal wrong" residuals.

### Batch-7 addenda (2026-07-04, 600-800 tier: 3 more byte-perfect + 1 honest re-derive)
- **base_register_cse CRACKED from C**: 3+ same-symbol LDAs in a call-free
  region form a base register (the old SKIP class). Declaring **distinct
  block-view extern symbols** (one named extern per region view, e.g.
  `D_80128B74`/`D_80128C40`/`D_80129080`) dissolves the base and the
  phantom `move`s (MASTER/func_801344E8).
- **End-pointer-before-cursor init order** in fill loops uncrosses start/end
  LDA reloc pairs at the C level — no relocfix script needed.
- **U-steering can BACKFIRE**: U-suffix compares can silently deflate a
  value's web and force spills where the target keeps it resident — the
  signedness must match the TARGET's form per site, in both directions
  (TEL/func_8013D758: plain signed year compares were required).
- **Shared-hi absolute store pairs**: a single `lui $at,0xNNNN` + two `sb`
  at NON-reloc absolute addresses is unreachable from named-extern/array/
  struct forms (7 shapes tested) — the faithful C is a raw-pointer base
  `q = (u8*)0xNNNN0000; q[-off1]; q[-off2]` (allowed here because the
  target itself uses the const form, not a reloc; MASTER/func_80139CD0,
  unsolved sister: RENSYU/func_80133214).
- **Locals change → frame change → silent patch_stack no-op**: re-read the
  raw prologue after ANY locals edit; `--old` must match the CURRENT compile.
- **Indexed (not pointer) fill loops** where the target has `sb -1(v0)`
  rotation: pointer form makes uopt cache the array LDA and emit phantom
  moves; increment-first indexed body + `while (i != bound)` with both s32
  gives the target's back-edge `bne` (MASTER/func_80139CD0 case 7).
- **Global RMW spelled as `D = D + 1`** (not through a local) keeps ONE
  register web whose reload CSEs into the neighboring compare — a local
  split the web into a beqz-delay `move` (MASTER/func_80139CD0 case 9).
- `IDO52_UOPT_TRACE` segfaults on some functions (TEL/func_8013D758) —
  hook-side bug, blocks PRECOLOR_WEB range derivation for const webs.
- Cleanup candidate: `src/overlay/MASTER/func_80139CC8.c` (2-insn fragment
  with a `strip_trailing_instructions` shim) is redundant with
  func_80139CD0's `@SymbolStart` window.

## The 2026-07-18 Catalog — 50+ wins in one day (4 worker batches + solo)

The levers below were each proven on multiple byte-perfect wins. Precedent
files are cited; read them before applying the recipe.

### ⭐ RULE FIX: `x % CONST` IS matchable (constant modulo)
IDO 5.2 compiles `x % CONST` (any constant, non-power-of-2 included) to a
guardless `div`/`divu` against the materialized constant + `mfhi` — no
zero-check break, no magic-multiply. Unsigned modulo → `divu`; signed →
`div`. Byte-matches from natural C with no PostProc.
The `divu/multu-by-nonpow2` SKIP class covers only QUOTIENT use (`mflo`).
**When triaging a "div blocker", check whether the target consumes `mfhi`
(modulo — matchable) or `mflo` (quotient — still blocked).**
Precedent: TACO/func_8014924C (guardless %240 + %3 gates, byte-perfect).

### The or-v0-after-andi tick (step-house family) — volatile stash recipe
Target signature: `lbu v0,D / addiu v0,1 / andi tN,v0,0xff / or v0,tN /
... / sb tN`. IDO always coalesces the `or` copy away. Materialize it:
```c
volatile s32 stash;
v = D_state + 1;  v = (u8)v;  stash = v;  D_state = v;  return v;
/* @PostProc: swap_spill_with_move --offset <stash sw> --dst v0  (per tick) */
```
The stash `sw` becomes the `or v0,tN`; paint fixes fields. Ticks that
`addiu v0,v0,1` with NO lbu use the SELECTOR: `v = sel + 1;`. A shared `sb`
reached from the sel!=0 path = `if (sel==0){...; sel=(u8)(D+1); stash=sel;}
D = sel;`. The stash slot often grows the raw frame to exactly the target's
— then DROP the patch_stack (a stale one silently kills the whole chain).
Precedents: SHUGAKU/func_8013AB54, NAME_ENT/func_80142014,
BUNKA_SD/func_801389E4, GEKO/func_80133250 (the 80051E88 double-stash
variant with two stashes and `--dst v1`).

### Normalizer damage: recognize and repair (biggest win source of the day)
The legacy whole-function `scheduler_normalizer` CORRUPTS as often as it
helps: rotated branch imms, a `jr ra` dragged mid-function, %hi luis or
store relocs swapped across cases (worsened by a latent bug: its ELF reloc
fingerprinting dies on an uninitialized map and is silently off). Repairs,
in order of preference:
1. **DELETE the directive** when residuals look like rotated lui/store
   groups (3 wins: GEKO/func_8013EF94, GYOZI/func_80141C70,
   BUNKASAI/func_8014A780).
2. **`adjust_branch_offset --offset X --imm <target>` placed LAST** for each
   rotated branch — on an otherwise byte-identical stream this is CORRECT
   (the output IS the target program). Precedents: OPTION/func_8013A1E4,
   GEKO/func_801387E4 (equivalent-tail ±2 adjusts).
3. **Swap a rotated word back**: a compiled `jr ra` mid-stream with a nop at
   the true slot → `swap_instructions` the two positions + adjust any branch
   that crossed. Precedents: NAME_ENT/func_8013FCFC, TEL/func_801392E4 +
   func_8013842C (the "unfilled-jal-delay wall" was this + a wrong
   @RodataBase).
4. **`--fence <start>-<end>`** (word range): pins a block so the normalizer
   cannot drag same-shape words across it. Two proven usages: fence the
   TAIL to protect it (EVENT/func_80135730), or fence the OTHER CASE so a
   within-case delay swap can happen without cross-case reloc drag
   (NAME_ENT/func_80139810). Always strip + regen the paint after fencing.

### Misnamed globals & fake-effective audits (highest-yield fix class)
ANY lo16/hi16 residual at a reloc site = re-derive the address from the
target .s (`hi<<16 + sign-extended lo`). Transposed digits and ±0x10000
hi-pages dominate. Fictional "BYTE-PERFECT 🎉" headers and over-masked
`reloc_only` entries hide REAL bugs: wrong addresses (DATE/func_801504A0
had 12), missed calls (DATE2/func_80136BAC skipped a shared-tail call),
crossed shift-pairs (OMIMAI/func_80132670 — a stale reg-paint was
*compensating* wrong semantics), mis-scoped guards (TT/func_80146874,
BUNKA_SD/func_8013431C). Trust only the resolver.

### Dead words in delay slots (nop-able class)
A dead `or v0,X` / `li v0,N` / load / arg-home `sw` sitting in a jr or
branch delay where the target has `nop`, with all other words matching:
`nop_at_offset` (no reloc) or
`custom_script tools/overlay/custom/zero_dead_load_in_jr_delay.py` (loads
with relocs — verifies post-return deadness and drops the reloc entry).
**These steps MUST be the LAST @PostProc line** — mid-chain the stream shape
differs and the offset misses (or silently kills the chain). Don't-care
returns: target falls out with junk in v0 → write bare `return;` (a
materialized `move v0,X`/`li v0,N` the target lacks is the tell).
Precedents: VALEN/func_80133A00, ETC/func_8013BB30, BUNKASAI/func_80135C54,
NAME_ENT/func_80142014.

### Loops: check the flags FIRST
A "scheduling/LICM wall" on a loop function is often a missing
`-Wo,-loopunroll,0` (IDO unrolls what the target keeps rolled) —
TACO/func_801482D8 went byte-perfect almost entirely from adding it.
Loop shape: increment the pointer at the LOOP TOP
(`p = base; do { p += K; store(p-K ...); } while (p != end);`) when the
target's loop head is `addiu vX,vX,K`. Store-before-call statement order
fills jal delays with the store. Loop multu-by-nonpow2 stride:
`@CompileEnv: IDO52_NO_FINDINDUCT_STRIDE=1` + pointer arith
(`rec = ARR + i * 56` — displacements fold into lh/sw only in the pointer
form, NOT through an `(s32)` cast sum).

### Init-routine cluster recipe (TACO family, 4 BP in one pass)
Guard early-out + raw-cast addresses where the target shows `lui;ori`
const form + named-extern arrays for `%hi/%lo` sites + parallel-array walk
with pre-advanced read-behind (`pa = &arr[1]`, read `pa[-1]`) +
post-increment store at `-16`. Commutative `addu` operand order follows the
C source order. Paint frequently unnecessary.

### Small structural levers
- **K&R definitions** (`void f(val) s16 val; {`) materialize the callee-side
  `sll/sra` param narrow that a prototyped s16 param elides.
- **`s16 h[N]` same-type struct assignment** reproduces IDO's
  aligned-words + `lhu`-tail `$at` copy idiom (word-typed structs give an
  `lw` tail; hand loops use 3 temps; only the short-array assign matches).
- **Guard-fail `bne ...; lw ra` in the target** = the guard returns the
  CALL's own value: `ret = call(); if (ret != K) ...` shapes, with the
  epilogue reached directly.
- **@SymbolSize windows can be wrong both ways**: trim when the window overruns into
  the next function's head; also check for an interior `jr ra` — the window
  may swallow a whole separate leaf function (BUNKA_SD/func_8013867C).
- **Fragment merges**: a tiny no-jr-ra head (often `lui/addiu tN,&REC; lw
  at,0(tN)`) is the cross-prologue head of the function after it — merge
  into one C function at the fragment's address (BUNKASAI 10-sister
  precedent; GEKO pairs).

### Directive hygiene (violations silently kill the whole chain)
- READ the raw prologue before any `patch_stack_complete` — a wrong `--old`
  aborts the chain silently; the symptom is a % collapse where NOTHING
  applies.
- `patch_insn_reg_field` lines are NOT cumulative — merge all paints into
  ONE line; `sed -i '/patch_insn_reg_field/d'` before regenerating (the
  generator APPENDS).
- No duplicate directives (a second patch_stack/convert fails the chain).
- Match the file's comment style exactly (single-line `/* */` vs block
  ` * ` — mixing breaks the compile).
- `adjust_branch_offset` sets the imm VERBATIM and, like the nop/zero-dead
  steps, must run AFTER any normalizer.
- `epilogue_share_into_branches` is BANNED by the test pipeline (older doc
  rows predate the ban) — do not use it.

### New walls confirmed this investigation (don't retry blindly)
Guard-fail delay-fill preference (lw-ra target-fill vs fall-through lui
borrow — hook proposal filed); `<<0`-fold (target keeps an explicit
`sll x,y,0` in a delay); held-constant slti-vs-remat tension; store-
interleave floor; init-phi-copy (+2); load-once/multiply-six family;
volatile-vs-speculation (one global reused across N multus needs volatile,
which then blocks the delay-slot lui hoist); caller-threaded t1/t6/$at
live-ins at entry (verify against raw bytes — these are why functions were
faked); LICM hoist-above-call (best next uopt-hook candidate — all other
levers proven on TT/func_8014C0A0 + func_8014B078).

## The 2026-07-25 Dispatcher-Stub Batch — 12 byte-perfect in one day (two parallel batches)

All wins on `next_stub.py`-screened fake-pad dispatcher stubs (GYOZI, TEL, ETC,
SHOUGATU, ENDING, BUNKAKEN, BUNKA_SD x2, BUNKASAI x3 + one interrupted
recovery). Every "blocker" claim in an old stub header that was re-tested proved
FALSE. New/confirmed levers, most-reusable first:

### The sp-LDA "spill wall" is a C-typing bug (BUNKA_SD/func_80132FF8)
`(s32)buf` / `(s32)D_x` casts on strcpy/strcat-style args build a CSE-able
address EXPRESSION that IDO spills to stack slots (+2 insns, unfilled jal
delay). Declare the callees with real `u8 *` prototypes and pass arrays by
natural decay -> IDO rematerializes `addiu a0,sp,N` per call, exactly like the
target. 106 diffs -> 13 in one edit. `IDO52_LDA_NOINREG_CLASSES` was inert all
along — re-tag any semi filed under "sp-LDA spill-vs-remat" for this fix first.

### Dead delay-slot selector move (TEL/func_8013ACF8, sister BUNKA_SD/func_80137898)
Target has a dead `or vN,vM,zero` in the first cluster-decision branch's delay
slot (value never read again). Recipe: `volatile u32 stash; stash = sel;` right
before the switch + `swap_spill_with_move --offset <slot> --dst <reg>`.
Type steering: volatile u32, NOT s32 (s32 splits the web: spill before the
branch + extra move). A u8 selector costs ~+39 insns (per-site andi/move web).

### Return-web shape (SHOUGATU/func_80140E98)
Gates = bare `return;`, counter reset = `return 0;`, and NO trailing return
(fall off the end). A trailing `return cnt;` coalesces the counter into $v0 via
an extra move, pushes the selector off $v0, and loses the duplicated `lw ra`
delay-slot idiom. The fall-off shape gets IDO to color the counter $v0 itself
and emit the target's dead-looking `or v0,zero,zero`.

### Direct re-reads can beat the temp-local reflex (BUNKASAI/func_801541E0)
`h = D_x; ... D_x = h + 3;` gives the load its own web whose lui IDO sinks far
away into a sub-ladder delay slot. Writing both arms as direct re-reads
`D_x = D_x + 3/6;` lets CSE place the load exactly where the target has it
(22 opcode-class diffs -> 0). Diagnostic: if the target's CONSTANT register ==
the load's register, the constants are what pin the lui — don't add a temp.
Related: block-base pointers (`p = D_800EE760;`) must be declared AT FUNCTION
ENTRY, not beside their use, or the LDA const-folds into %hi/%lo at the site.

### Kill the param live-range with literal+repaint (BUNKASAI/func_80140960)
`D_x = sel + 1;` keeps the selector live past the dispatch -> phantom
`or a1,a0,zero` steals the beq's `sw ra` delay slot and shifts the WHOLE stream
by one (exact% craters to ~0 with a perfect insn count). Write the literal
(`D_x = 12;`) + `patch_insn_imm16 OFF:12:1` + `patch_insn_reg_field` back to
the +1 form. Off-by-one word streams generally: align target vs compiled with
difflib.SequenceMatcher — the single inserted/deleted word IS the bug.

### Arg-register vs temp coalescing -> PRECOLOR_WEB (ETC/func_80141BDC)
Residual pattern: your compile hoists ONE `move a2,tX` serving two call sites;
target keeps the web in $v1 and materializes the arg copy per call (same insn
count — the nops become moves). Source-level assignment permutations cannot fix
it; `@CompileEnv: IDO52_PRECOLOR_WEB=v1:<bitpos>` (Hook 18) can. Find bitpos
with `IDO52_DUMP_WEBS=1`. Zero C-body change — audit old reg-paint lists after.

### Wrong-D_-name tell (BUNKASAI/func_80157868)
Resolver residuals like `e95e vs e9de` on %lo sites = a hand-derived global
address is wrong; the target's low16 IS the correct offset. Never hand-derive
`-5794 from 0x800F0000` arithmetic — read the %lo off the target words.

### extract_jr_table.py only reports the OUTERMOST table (BUNKASAI/func_80140960, func_801541E0)
Nested sub-dispatch tables (per-case 5-entry message-id selectors) must be hand
dumped from the EXN at their rodata offsets. They can share case bodies
(case 0/1 -> one body) — body-layout inference gets this wrong; read the table.
One `@RodataBase` covers multiple contiguous tables.

### K&R + escape-struct + value-materializing booleans (ENDING/func_80132420)
- K&R param list (`func(a,b,...) s32 a; ...{`) produces the target's
  arg-home `sw/lbu/sb` narrowing where ANSI gives `andi` + separate spill.
- A local written near a strcpy'd buffer must be a FIELD of that buffer's
  struct (`struct { char text[518]; s16 y; }`) or IDO constant-folds it and
  never emits the sh/lh pair.
- `first = (h == 0); if (first && ...)` -> `sltiu v0,x,1; beq v0,zero`
  (plain `if (h==0)` folds to bne; the boolean local keeps sltiu in a real
  register instead of $at).
- `volatile u8` on a spilled param forces reload-per-use instead of sw/lw.

### Directive hygiene additions
- **The PostProc chain is ALL-OR-NOTHING**: one bad `patch_insn_reg_field`
  offset fails its verify and aborts the entire chain — the diff then looks
  like a total codegen regression. Offsets are `index*4 + 0x10`.
- **`--fence` now accumulates when repeated** (fixed 2026-07-25; it used to
  silently keep only the LAST flag — unfenced regions let `--const-fp` drag
  same-shape RELOC'D words across case boundaries while masked % showed
  "RELOCATION-ONLY". Only the resolver catches this).
- `scheduler_normalizer --block-aware` on a multi-case dispatcher produced
  semantically BROKEN code (moved `li at,1` after its consuming beq) while
  RAISING masked %. Re-confirmed: never whole-function on dispatchers; trust
  only the resolver.
- `@SymbolSize` must equal the `.sym` entry size or the tail is never compared
  (BUNKAKEN/func_80151F08 sat "semi" for months over a 16-byte unchecked
  epilogue; the fix was widening the window and DELETING PostProc).

### Absorption campaign notes
- `absorb_stub.py <OV> <HEAD> --apply` is idempotent-safe when the host
  already declares `@SymbolStart` (sym-only fix, .c untouched); then `rm` the
  head's marker `.c` and commit `.sym` + deletion together.
- Two BUNKASAI `.sym` defects logged for a future pass: `func_80140958` =
  bogus 8-byte symbol on func_80140560's epilogue (fragment, NOT a head);
  `func_80140F4C` = genuine unabsorbed head for func_80140F54.

### ⭐ AMENDMENT (same day): the "ugen $v0 compare-chain blocker" IS crackable — goto-web lever (BUNKASAI/func_8013D8E8)
The 4-way `get_g_zyotai_low(N)` ladder whose arm constants the target holds in the
dead selector register `$v0` (filed as a blocker after func_8013CCB4 exhausted
~15 plain-source forms) yields to a CONTROL-FLOW rewrite: route every arm
through a shared merge label (`goto stA/stB/stC/stD` -> one `D_800D0648 = sel;`
store at the merge, NOT per-arm stores with `break`). Three different constants
reaching the merge make `sel` a uopt VARIABLE WEB (not ugen expression temps),
and uopt colors it `$v0` — which (a) blocks the case-2 hoist (dest still live),
giving the target's fall-through `addiu $at,K` delay slot, and (b) turns the
case-3 hoist into a copy, restoring the retained dead `addiu $v0,K`.
Retry func_8013CCB4 / func_8013F5D8 with this shape before calling them semis.
Also: `IDO52_DUMP_WEBS=1` SEGFAULTS uopt in the current build (~18 of 135 webs
printed, then dies) — PRECOLOR_WEB bitpos discovery is unavailable until fixed;
probe with the goto-web / live-range levers instead.

### Batch-3 additions (2026-07-25 evening, 6/6 byte-perfect + the CCB4 retry)
- **Sparse-switch cluster split (RPG_BAT/func_80156D80)**: a two-cluster case set
  (e.g. 0..4 and 80..86) is ONE `switch` in C. IDO 5.2 emits the `slti $at,K`
  cluster test + two jr tables ITSELF, with all case bodies in source order.
  Modeling it as nested if + two switches emits the taken-arm's bodies first
  and cannot match (5.2% -> 48.8% exact from this one correction).
- **Unprototyped-printf hypothesis (NAME_ENT/func_8013D1E8)**: a delay-slot
  `lui aN,%hi(global)` that looks like a spurious constant ARGUMENT to a
  printf-like callee is usually the OTHER arm's global base speculatively
  hoisted, left as garbage on this path. Do not fabricate an extra call arg to
  imitate it (that forces double materialization and was misfiled as a
  def-PLACEMENT blocker). Declare the callee unprototyped `extern void f();`
  and pass only the real args; sequence `tmp = load; call(str, tmp);` to steer
  WHICH lui the delay slot takes.
- **`IDO52_UGEN_NUM_TREGS=5` (BUNKASAI/func_8013CCB4)**: when three `%hi`
  address loads compete for two hoist slots (block + delay) and IDO hoists the
  wrong pair at the default 6 tregs, clamping ugen to 5 tregs flips the choice
  (target: remat-inside-block for the clobbered base + delay-slot fill for the
  third). Probed-inert alternatives are listed in that file's header — check it
  before re-sweeping (LDA_NOINREG, NO_LICM, EXCLUDE_REGS all 45 pairs, etc.).
- **la-form record base (BUNKASAI/func_801580F4)**: a record read at base+512/+514
  where the target uses `lui;addiu` (la) + field displacement needs
  `extern volatile u8 D_base[];` + volatile deref. A plain named extern at the
  field address folds to `lui;lw` (one word short per site); a raw-address cast
  collapses to a bare lui. 
- **`or $v0,$tN,$zero` after an andi/sb is RETURN-VALUE materialization, not a
  spill (ETC/func_801452B4)**: give the dispatcher `s32` return type; ticks are
  `w = (u8)(g + 1); g = w; return w;`, gates bare `return;`, don't-care tails
  `return callee(...);`. The old volatile-stash + swap_spill_with_move imitation
  took 16 directives; the honest form takes 2.
- **PRECOLOR_WEB bitpos by brute sweep**: `IDO52_DUMP_WEBS=1` segfaults on
  large functions (~14-18 webs printed, then SIGSEGV). Sweep
  `IDO52_PRECOLOR_WEB=REG:N` over N=0..~220 checking for the target's
  discriminating word instead. The bitpos is source-layout dependent — re-sweep
  after ANY body edit (dropping one local moved 165 -> 162).
- **Pinning the RIGHT web matters (ETC)**: pin the `+1`/value web, not the
  selector web — pinning the selector splits it with a copy or emits a dead
  home-store. The goal is usually to make a later insn CLOBBER the live-out
  $v0 so the delay-slot hoist becomes illegal, exactly like the target.

### Batch-4 additions (2026-07-25 late, 6/6 byte-perfect + 10-function @RodataBase harvest)
- **@RodataBase harvest is a standing free-win pass**: `python3
  tools/overlay/apply_rodata_base.py` (no args = tree scan; it EDITS .c files)
  finds reloc_only functions whose only unresolved item is single-block
  `.rodata` and adds the one-line annotation — 10 went byte-perfect in one run
  (2026-07-25). Re-run it whenever the reloc_only pool grows; verify each with
  the resolver before committing. Multi-item `.rodata` (76 remaining) is real
  Phase-2 work, not free.
- **Loop-split for unroll base alignment (TAIIKU/func_80145DEC)**: IDO x4-unrolls
  a stride-N loop with a peel; if your compile's induction base is one element
  off the target's (displacements shifted by the stride), peel the FIRST element
  as a separate scalar statement and start the loop at element 1 with one fewer
  iteration. 26 residuals -> 4.
- **Frame layout is recoverable from C shape — try before stack PostProc**
  (MASTER/func_80139910, OPTION/func_80132BB4): grid-sweep pad locals
  (sizes/declaration positions) to hit the target frame/slot layout. The
  original's DEAD locals are part of the layout: OPTION needed `padA[10]`
  declared first + `padB[6]` last (and the real buffers declared in REVERSE) for
  frame 144 with zero stack PostProc. TRAP: modeling pad space as members of an
  aggregate switches IDO to the member's alignment -> `usw`/`swl-swr` instead of
  `sw`. Pad locals must be standalone.
- **Delay-fill-from-branch-target rotation**: when the target fills a `bne`
  delay slot from the branch TARGET (e.g. the `li 255` it jumps to) but IDO
  fills from the fall-through, a small fenced `scheduler_normalizer` + one
  `adjust_branch_offset` restores it — goto/polarity/const-hoist source forms
  are inert (OPTION case 2, probed list in that file's header).
- **Volatile is a scalpel, not a default (GYOZI/func_8013C824)**: this target
  needed volatile REMOVED — an intervening jal already kills CSE between two
  lbu's of the selector, and volatile (a) blocked the %hi hoist into a delay
  slot and (b) forced strict lw;sw interleave where the target runs loads
  one-ahead (22 order diffs). Match the target's interleave pattern first, then
  decide volatile per-global.
- **Zero-filled jr table in the EXN ≠ tail_call_dispatch blocker**: the loader's
  relocation pass writes it at load time; `@RodataBase` (derive with
  `resolve_overlay_relocs.derive_rodata_base`) matches it from C.
- **Bitpos brute-sweep scoring**: score candidate compiles on `objcopy`'d binary
  LENGTH, not the objdump .text header — the section is 16-byte aligned and
  padding words fake "+3 insns" on an exact match. PRECOLOR_WEB pins can be
  comma-chained (`v0:16,v0:7`) to cover multiple webs.

### Batch-5 additions (2026-07-25 night, 5/6 byte-perfect + 1 genuinely new blocker)
- **The +0x10000 misname tell (NAME_ENT/func_801481B0)**: a `%hi 0x8013` paired
  with a NEGATIVE `%lo` means the true address is `0x8012xxxx` — a whole family
  of D_ names was off by +0x10000. Recompute every global as
  `hi<<16 + sign_extended(lo)`; never trust header-comment arithmetic.
- **Empty-join volatile stash (NAME_ENT/func_801481B0)**: when the target has a
  `b tail` branch-target with an UNFILLED delay slot, IDO's FRONT END collapses
  the empty join block and threads the edge (no -Wo flag or uopt hook reaches
  it; even -Olimit,0 threads). `volatile s32 stash; stash = 0;` after the if
  keeps the join alive; IDO fills the delay with the dead frame store;
  `nop_at_offset` zeroes it. Bonus: the stash slot can grow the frame to
  exactly the target's (no patch_stack_complete).
- **Index loop beats pointer walk on aliasing (NAME_ENT/func_8013E11C)**: a
  `u8 *p` clear-loop makes IDO assume the stores alias an unrelated global and
  reload it after the loop; `i = -1; do { i += 4; a[i..i+3]=0; } while (i!=11);`
  emits the identical loop with NO reload. Pre-increment index form = target's
  addiu-at-loop-top; `for (i=3;i<15;i++)` = backward-displacement bias.
- **New CompileEnv keys proven**: `IDO52_LDA_NOINREG_CLASSES=4` (class 4
  SPECIFICALLY — rematerialize the la per indexed store; 0-3/5-8 inert here);
  `IDO52_NO_DEADSTORE_GLOBALS=0x0` — extern globals resolve to ADDRESS 0 in the
  relocatable object, so 0x0 is the key that protects a store IDO deletes as
  dead (confirm with IDO52_DEADSTORE_DEBUG=1); the runtime address does not work.
- **Displacement-fold order (NAME_ENT/func_80140804)**: to get the target's
  `addiu tX,idx,8; addu v0,tX,base; lw 0/4(v0)` instead of `lw 8/12(v0)`,
  write the base add LAST: `(u32*)(((s32)(c<<4) + 8) + (s32)base)`.
- **Volatile-scalpel web split (NAME_ENT/func_80140804)**: `tmp = buf[2]`
  value-forwarding (`lbu;move`) can occupy a branch delay slot the target fills
  differently; splitting that ONE byte out of the array as its own
  `volatile u8` kills the forwarding without disturbing anything else. Old
  PRECOLOR_WEB pins in a header can be actively harmful (dead spill) — re-test
  before keeping them.
- **`if (g == 0 && g == 0)` is real original code** — IDO CSEs the load but
  not the second test, emitting the "unreproducible duplicated bnez". Write
  the duplication faithfully.
- **Genuinely NEW blocker class `as1_la_pull` (GEKO/func_80136790)**: as1 (the
  ASSEMBLER scheduler, r6_sched) pulls a branch-target block's leading `la`
  upward to fill a load-use interlock, going net -1 word. No uopt/ugen hook,
  -Wo flag, or source form reaches as1 (exhaustive probe + 148-insn repro in
  the .c header; as1 refuses the pull only when the block heads with a call, a
  pre-call load pair, or >=2 preds). Needs an as1-level hook — filed in
  c_unmatchable.txt. Do NOT retry; the ENDING volatile lever does not apply
  to address computations.
- **Fenced normalizer can still drag reloc words WITHIN the fence** across a
  case boundary it doesn't know about (func_801481B0 case 11: 6 crossed %lo) —
  when a handful of pure scheduling ties remain, prefer explicit
  `swap_instructions` pairs over a normalizer window; only the resolver catches
  the drag.

### Batch-6 additions (2026-07-25, 6/6 byte-perfect — the next_stub vein is CLEARED)
- **`-Wo,-loopunroll,0` can BE the wall (RPG_BAT/func_80140204)**: the project's
  habitual flag suppressed the default factor-4 unroller output the target IS
  (peeled iteration 0 + strength-reduced base + strides folded into
  displacements). Write the natural loop with `-Wo,-loopunroll,4`; loops
  containing calls stay un-unrolled so the flag is safe function-wide. Hand
  unrolling + volatile spills to imitate it is what made bodies overrun their
  windows (395 -> 62 diffs from the flag flip alone). Suspect this on any
  window-overrun function whose old header hand-unrolls a uniform block.
- **Adjacent globals may be ONE object (RPG_BAT/func_8014F8C0)**: declaring
  0x8015EE00/0x8015EE04 as `extern s32 D_8015EE00[2]` pins as1's entry-block
  schedule — a load may not cross a store to its OWN symbol, so it lands in the
  target's delay slot. As two externs NO source form reaches as1's reorder
  (exhaustively proven). The fix for "as1 reordered my entry block" when the
  globals are adjacent.
- **`s32` don't-care return in the INVERSE direction (ETC/func_80136450)**: a
  duplicated `lw ra` in a branch delay slot (fill-from-branch-target) requires
  the fall-through hoist to be ILLEGAL — declare the dispatcher `s32` with no
  return statement so $v0 is live-out. One edit, masked 229 -> 320.
- **Cross-prologue heads can be 13 WORDS, not just 2** (RPG_BAT/func_8014F8C0:
  as1 sinks `addiu sp` past the entire leading global block; a caller jal's the
  head address directly). The .sym entry there both STARTED 52 bytes late and
  RAN 32 bytes long into the NEXT function's head — check both boundaries.
- **Struct-member globals beat standalone externs for ordering** (RPG_BAT/
  func_80140204): "HUD scalars" that are really fields of an array's elements
  must be declared as members — standalone externs let the scheduler hoist
  loads over the neighboring stores (6 sites).
- **Frame-from-C limit found** (RPG_BAT/func_8014F8C0, negative result): IDO
  deletes every dead pad local except the FIRST declared one, and places that
  survivor ABOVE other locals; layout = 24 + roundup8(4*n_surviving), packed
  top-down in declaration order, arrays above scalars. If the target slot
  layout needs a pad BELOW the live local, use remap_stack_slots — the C sweep
  cannot reach it.
- Two more live truncated-compare specimens: a web split making the real body
  330 words while the masked test said "329/329 exact" (RPG_BAT/func_8014A668),
  and a hand-unroll overrunning the window (func_80140204). ALWAYS check
  `len t=X c=X` equality.

### Batch-7/8 additions (2026-07-25 late night — the oversized-pad vein, 17 byte-perfect + 2 honest semis)
- **Oversized-pad triage confirmed**: every pad in the worst-18 list was REAL
  fabrication (register-transcription pseudo-C + nops), not stale — but several
  hid structural surprises: merged .sym blobs (KANGEI 3-func, TT 4-func,
  NAME_ENT 8-func, BUNKA_SD split), a pad 6x the actual function size
  (EN_NICHI), and the tree's only @Compiler: gcc file (TAIIKU, whose "error"
  was a missing cross-cpp binary).
- **Blob-as-one-TU (TT/func_8014A2A4)**: a multi-function .sym blob can be
  matched as ONE .c with all bodies in address order (forward-declare where an
  earlier body jal's a later one). No .sym split needed when the window equals
  the entry. Disassembler over-splits (interior exits registered as functions)
  merge back the same way.
- **Struct beats array for adjacent globals (BUNKA_SD/func_801390C4)**:
  u32 D_x[2] fixed load/store ordering but freed $v0 and broke allocation;
  a two-field struct got ordering AND allocation right. Try struct first.
- **Switch-selector read POSITION among entry loads is byte-critical
  (TT/func_80144D60)**: sweep the read's statement position; and volatile can
  be a TRAP — `.set volatile` blocks as1 interlock pulls the target relies on.
- **volatile on a switch selector stops tail duplication (RPG_BAT/func_80152F30)**:
  without it IDO hoists the shared tail's lbu+sltiu into every case exit
  (+32 insns). Under volatile, one explicit temp per target load is mandatory.
- **struct-by-value SVECTOR args (TACO/func_80159888)**: lwl/lwr / swl/swr
  blocks before a call = 8-byte align-2 structs passed by value, not memcpy.
  A post-jal lw of the param home is a PARAMETER reload, not the return value.
- **Dead subu x,y,y (RPG_BAT/func_80135360)**: uopt folds plain `h - h` to 0
  (no flag disables it); `(h >> 0) - h` survives the fold with one live range.
- **bare `if (r)` vs `if (r != 0)` (DATE2/func_80135E44)**: `!= 0` lets IDO
  fold signed `% 2` into andi (4-word loss); the bare form keeps the full
  bgez/andi/beq/addiu expansion. And `cse_lui_at` is PROVEN necessary for
  consecutive same-symbol stores — as1 never CSEs lui $at there (4 spellings).
- **Six-store |= runs may be an unrolled loop (RPG_BAT/func_80154760)**: cfe
  folds every constant subscript, so a non-folded `li 2` + mul-by-stride body
  after two folded stores = IDO's x4 unroll of a 6-trip loop, not hand code.
- **gen_frame_remap caveats**: it can OMIT slots that sit inside transposed
  regions (add by hand) and can suggest NEGATIVE offsets its consumer refuses
  (now a loud error; split into patch_stack_complete + positive pairs).
- **remap_stack_slots offsets are .text-relative like everything else**
  (`index*4 + 0x10`) — a bare index*4 silently patches the WRONG pair while
  masked % still improves.
- **Open residual pair (RPG_BAT/func_80154760, for a future lever)**: in-place
  global increments kill 3 phi copies but lose 3 lui delay-slot hoists; every
  probed config gives one or the other, never neither.

### Batch-9 additions (2026-07-26 — blob-split harvest + next pad tier, 14 byte-perfect + 1 honest semi + 1 new blocker)
- **⭐ (s32)/(u32) conversion cast survives copy-propagation as a REAL move
  (BUNKA_SD/func_80139200)**: the only spelling (of ~18) that reproduces a
  live-range-split `or aN,vN,zero` selector copy IDO otherwise propagates
  away: `switch ((s32)sub)` where sub is u32 (and cast the late gates the
  same way). Retry lever for every "web-split copy" residual (DATE2/
  func_80135E44 open item).
- **⭐ Don't-care-return phi-copy wall cracked (NAME_ENT/func_8013411C)**:
  `volatile` on the re-read global + `IDO52_PRECOLOR_WEB=v0:0` reproduces the
  dead-looking `or v0,v1,zero` don't-care return. Retry on func_8013D658.
- **as1 copy-vs-move rule (TT/func_80146338)**: as1 MOVES an insn into a
  branch delay unless the filler clobbers the branch's operand register — then
  it COPIES from the branch target and retargets +1. Steer from C by making
  the two flag tests share one register (single reused local, not else-if).
- **Loop exit-test form steers unrolling (TT/func_80145360)**: `i < N` (slti)
  stays rolled; `i != N` (bne vs register-held N) unrolls x4. Indexed
  `tex[i] = BASE + i*K` also blocks full unroll where pointer do-while
  invites it (NAME_ENT/func_8013C100).
- **Strength-reduction base/displacement split (ETC/func_801431B0)**: IDO
  folds the loop's INITIAL index into the pointer base and keeps the source
  constant in the displacement — `D_800F01F0[0x14+i]` matches where
  `D_800F01F1[19+i]` misses by 5 words. Grep matched sisters for the split.
- **Pointer-typed externs can be required (TT/func_80146338)**: `extern u8 *`
  saved re-materialized loads where the usual s32+cast form cost +4 words —
  the array-vs-pointer rule cuts both ways; read the target's load count.
- **u32 tmp for read-modify-truncate counters**: `tmp = fld+1; fld = tmp;
  var = tmp;` — assigning through the u16 var costs a move per site and the
  target stores the untruncated register.
- **Blob-out-of-EXN pointers want raw constants (KANGEI pair)**: addresses in
  a separately-loaded blob (0x801B....) use `(void *)0x...` literals
  (lui;ori), NOT named externs — the principled exception to named-extern
  doctrine. Overlay-resident pointer arrays stay named externs (per-slot
  %hi/%lo stores, no $at CSE).
- **Over-split fragments**: a 12/28-byte "function" with live-in registers is
  an else arm — merge it back into the host window (.sym) and delete the
  fragment .s/.c. extract_jr_table now scans to the function's own jr-ra
  (fixed; deep dispatchers were silently missed).
- **NEW BLOCKER `loop_test_replacement` (TAIIKU/func_8013E3D4)**: this IDO
  rebuild converts SOME `<`-form loop tests (sltu+bne) to single-insn
  `bne p,lim` when it proves exactness; target keeps sltu on all. Count-
  affecting; exhaustive source/flag/hook probe inert; all-pointer loops
  restore sltu but lose 14 words to limit-CSE. Needs a uopt hook (disable
  test replacement per-loop). Also gates sister func_8013EA6C.

### Batch-10 additions (2026-07-26 — retries pay off, blocker taxonomy sharpens)
- **⭐ Retry loop works**: DATE2/func_80135E44's two "unmatchable" residuals both
  fell to same-day levers from other functions. Bank open residual pairs and
  re-dispatch when a donor shape lands.
- **(s32) cast refinements (DATE2)**: the cast def must sit BEFORE the call it
  spans (callee-saved color -> delay-slot fill; after the call it colors $v1 and
  lands late). PRECOLOR bitpos must be RE-DERIVED after any source edit (web
  indices shift — a "failed" sweep may have been pre-edit). IDO52_UOPT_TRACE=5
  segfaults on big functions, like DUMP_WEBS — brute sweep remains the workflow.
- **(s32) lever boundary (NAME_ENT/func_8013D658 negative result)**: the cast
  survives copy-prop only when the def is an unsigned ARITHMETIC node; it folds
  on a plain lb def. A color pin cannot CREATE a copy the natural compile lacks
  — the only known route is a jal inside the value's live range (sibling
  func_8013E5AC proof). Filed `phi_copy_cursor_dual_register` (also
  func_8013F258).
- **multu inversion (NAME_ENT/func_8013425C)**: IDO 5.2 NEVER emits multu for a
  constant multiply reached through array scaling (G[i][j] -> sll;subu, proven
  by micro-repro). An explicit source multiply DOES: flatten to
  `D_x[q0 / 6 + q1 * 3]`. Sum order is load-bearing: div-first keeps HI
  clobbered so the target's extra guarded divs appear.
- **Redundant narrowing cast costs a register (KANGEI/func_8013808C)**:
  `(u8)(x & 0xF)` emits a fold-away second andi that has already consumed a
  temp, shifting the whole $t chain by one. 14 diffs -> 2 from dropping one
  cast. Worth a tree-wide grep of semis for redundant casts on masked values.
- **No-merge-local case tails (BUNKASAI/func_8013DE44)**: two guarded += arms
  re-reading the global directly (no local) lets store-to-load forwarding keep
  $v0; any local spelling costs a phi copy that steals the delay slot.
- **Mnemonic-identity scoring for hook sweeps (BUNKASAI/func_80155B24)**: raw
  exact% can stay FLAT across the winning PRECOLOR bitpos; score sweeps on
  shift-aware mnemonic identity.
- **IV-selection steering (TT/func_80137C20)**: uopt's loop-test replacement
  picks the LAST derived integer IV — choose which array stays indexed vs
  pointer-walked to flip the emitted test form. (Candidate counter for the
  loop_test_replacement blocker; TAIIKU probes should try IV-population
  surgery.) Asymmetric unrolling (one loop rolled, its sibling x2) = peel the
  first record explicitly; a uniform loop can't produce both.
- **as1_la_pull sub-class (SHOUGATU/func_8013F570)**: count-NEUTRAL variant —
  as1 pulls one of two destination la's into a div interlock shadow where the
  target pulls both; choice is ugen cursor assignment, invariant under 60+
  TREG_ORDER perms and full PRECOLOR sweeps. Same as1-hook gap.
- **loop_test_replacement confirmed x2 (TAIIKU/func_8013EE38)**: the target's
  own != loop matches, proving both forms reachable; only the <-preserving
  direction is lost. Hook target: f_eliminduct/f_ivfactor.
- **Another truncated-compare fake exposed (NAME_ENT/func_8013425C)**: header
  claimed 564/564 while emitting 584 (20-word window overrun silently
  truncated) + a real index-variable bug. The overrun honesty gate catches
  these — never trust an old header's claim without `len t=c`.

### Batch-11 / hard-target additions (2026-07-26 — the blocker taxonomy collapses)
- **⭐ Hook 20 `IDO52_NO_LOOP_TEST_REPLACEMENT`**: the `<`→`!=` loop-test rewrite
  lives in **f_loopunroll's exit-test normalization and runs even at
  -Wo,-loopunroll,0** — why years of flag/pass probes were inert. Modes
  1/bb:N/occ:N. Also found: IDO52_SKIP_PASSES tokens loopunroll/tail_recursion/
  copypropagate/opt_saved_regs are parsed but NEVER WIRED (latent bug).
- **⭐ Unfoldable identity ops force copies (NAME_ENT/func_8013F258)**: IDO's
  front end does NOT fold `| 0 ^ 0 << 0 >> 0 * 1 / 1` — `sel = raw | 0;` emits
  the real `or` copy that copy-prop then preserves. Folds: `+0 -0 &-1 | x ?:`.
  `-(-x)`/`~(~x)` emit real negu/nor. The copy survives ONLY when ugen has a
  caller-save split to attach it to (call in live range); the phi_copy class
  splits: call-in-range = CRACKABLE, no-call = still walled (func_8013D658).
- **⭐ "Lost lui delay-slot hoist" = as1 operand clash (RPG_BAT/func_80154760)**:
  as1 copies a branch-target's first word into the delay slot only if it does
  not clobber a branch operand register. Fix by pinning the SELECTOR web off
  the clashing register (PRECOLOR_WEB), not by moving the load. Paired
  CONST_INREG+CONST_NOINREG occurrence windows bound one web at both ends
  (2D sweep; re-derive indices after changing either). Cheap diagnostic:
  perturb a constant's value — distant residuals that vanish are the same web.
  Sweep-harness trap: a pin can "score better" by DELETING a load (miscompile)
  — guard head integrity in sweeps.
- **Re-audit head rule (7 of 8 misfiled)**: every "caller-threaded $v0/$v1/$t6"
  entry with an 8-byte symbol (or unregistered gap) adjacent was the function's
  own hoisted first global read. Check the .sym neighborhood + raw EXN bytes
  BEFORE believing any hidden-param/caller-threaded filing. default:-first
  switch layout and inline-per-use global reads finished most of them.
- **DUMP_WEBS segfault root cause**: stale bittab slots below numwebs
  dereferenced into the PROT_NONE arena (size-independent crashes) + a silent
  200-web cap. Fixed via sigsetjmp probe (write()-to-/dev/null probes DON'T
  work — kernel never touches the buffer). UOPT_TRACE=5 needs -Wo,-l,<file>.
- **In-place `g++` vs temp (RPG_BAT/func_801505C0)**: the in-place form keeps
  the add clobbering a branch operand → delay fill illegal → target's arg
  setup takes the slot; uopt then sinks the write-back into both arms (the
  "redundant double store" is real). Same clobber principle as goto-web,
  reachable from plain source.
- **Frame/copy misc**: dead pad arrays are allocated and lay out in reverse
  declaration order — pads on BOTH sides place a buffer exactly
  (TAIIKU/func_80141450, zero stack PostProc). `*(Blk*)buf = g;` (cast through
  array) gives alternating lw/sw; plain struct-assign gives paired lw,lw,sw,sw.
  Copy-back loops written as plain for-loops reproduce the unroll+peel.
- **Web-split across calls (GYOZI/func_80142018)**: one variable spanning two
  calls forces an s-reg + prologue rotation; split into n/m so neither web
  crosses a call. 16.16 fixed-point lanes = union {s32 w; struct{u16 lo;
  s16 hi;}} for one-LDA hi/lo stores. PRECOLOR_LIVEIN=a0:0 fixed a whole
  10-save prologue (SHOUGATU/func_80142950).
- **Same-storage dual spellings are REAL original code**: D_800EF805[i] vs
  D_800EF808[i-3] (SHOUGATU), D_800F6415 vs D_800F6418 (GYOZI), D_801465FC vs
  D_80146600 — per-site symbol choice controls the addressing form; sharing
  one symbol lets LICM/CSE merge what the target keeps separate.

### Batch-12 additions (2026-07-26 — Phase-2 collapse, as1 hooks land, two fabrication traps)
- **⭐⭐ The "multi-item .rodata" pool never existed**: 78 of 84 were single-block
  with WRONG SYMBOL NAMES (622/622 residual diffs on reloc sites). 53 went
  byte-perfect by renaming to the address the target's %hi/%lo actually encode.
  Method: pair HI16->LO16 in ABI order, base = target_addr - our_offset; all
  pairs in a function must agree.
- **⭐⭐ THE HI-ONLY FABRICATION TRAP**: when ONLY the lui differs and the lo16
  already matches, renaming the symbol to "fix" it mints a FICTIONAL address
  (reusing the other symbol's lo16) that byte-matches perfectly — and the
  strict resolver reports it CLEAN. Discriminator: (want - have) % 0x10000 == 0
  => REFUSE; it is a lui-web/reloc-record rotation, converter-level, never a
  C-layer fix. 18 such functions remain reloc_only — do not "fix" them.
- **⭐ Resolver clip bug (open)**: test_overlay_match/resolve clip the COMPILED
  buffer to the symbol size, so a compiled body LONGER than the target can pass
  the len-equality gate (TAIIKU/func_8013EE38's "447/447" was really +1).
  Check objdump insn count when a residual makes no sense.
- **as1 hooks exist now** (apply_as1_hooks.py, per-hook idempotent):
  IDO52_AS1_NO_LA_PULL=<occ|all> / FORCE_LA_PULL / LA_PULL_TRACE — veto/force
  as1's cross-BB speculative motion (do_xbb_opt/func_4281e0; the undo->commit
  divert is the force). GEKO/func_80136790 byte-perfect with NO_LA_PULL=8.
  ⚠️ -Wa,-O0 was a FALSE NEGATIVE in every prior probe — the xbb pass is not
  gated on -O. ⚠️ exact% is the wrong metric for cross-BB motion (rotates
  register fields body-wide); score shift-aware mnemonic identity split into
  register-only vs op diffs.
- **LDA_NOINREG occurrence window exists (CLASS:FROM:TO)** but refuted for the
  TAIIKU pair: f_ldainreg's promotion bit is PER-ADDRESS (blocking one query
  defers to the next); the target's shape is a live-range SPLIT (2 callee-saved
  copies + in-loop remat) no per-address predicate expresses. Numbering is
  stable under blocking (unlike CONST_NOINREG). SKIP_PASSES tokens now wired:
  loopunroll live, opt_saved_regs diagnostic-only, tail_recursion inert on this
  codebase, copypropagate aborts (code 411, needs a state stub).
- **Atomic-LDA precolor screen (BUNKASAI/func_801569C4)**: when sweeping
  PRECOLOR_WEB for a split-LDA symptom, screen candidates by "LDA stays
  atomic" first — 161 webs -> 6 candidates. A missing delay-slot nop can be a
  20-word rotation from ONE unpinned base web.
- **cfe multu-CSE cluster (EVENT palette tails, 7 filed)**: a constant multiply
  from a single non-volatile operand is strength-reduced AND CSE'd by cfe (not
  uopt — NO_CSE can't split it; CONST_INREG queries the wrong node type).
  Only a volatile operand yields multu, at the cost of extra lbu. Same class
  as the 3 previously-filed multu functions; caps ~20 EVENT functions.
- **Unmapped .text tails census (overrun sweep)**: 7 overlays have real code
  past their last .sym entry (~29 functions, ~8.4KB: TACO 6, KANGEI 12,
  DATE2 1x2644B, MASTER 2, DATE 2, RENSYU 5, TT 1) — invisible to the
  universe count; registration pending. Also: 19 of 20 flagged window
  overruns were CORRECT windows over honestly-over-emitting C.
- **Misc**: array-indexed call args (f(A[i],1)) reproduce caller-saved IVs
  with spill/reload pairs + mixed exit-test forms (TAIIKU/func_80134060);
  direct `*p` RMW twice (no local) gives the store-forwarded phi shape
  (NAME_ENT leaf); -Wo,-loopunroll,2 selects peel-1/unroll-2.

## The 2026-07-26 reloc_only Drain (pool 32 -> 0)

All 32 remaining reloc_only functions went byte-perfect in one hands-on pass.
Nearly every one was a **whole-function `scheduler_normalizer` artifact**, not a
codegen blocker. The repeatable pipeline:

1. **Strip the whole-function normalizer + its dependent chain** (paints,
   adjust_branch_offsets, nop_at_offsets, imm16s). The masked % will crater —
   ignore it; read the RESOLVER residuals only. The normalizer's `--const-fp/
   --store-fp` drag moves same-shape RELOC'D words (lw jr-table loads, sb/sw of
   sister globals, lui pages) across sites, leaving each site with its
   neighbor's symbol. That IS the "rotated lo16/crossed hi16" signature.
2. **`@RodataBase: 0x<target first-table addr>`** for any word-[7]-style
   `lw $rX, imm($at); jr $rX` load. One base covers all tables when layout
   matches (verify: compiled `.rodata` cumulative offsets == target deltas).
3. **+1-word shift from word [2] = selector-copy tell** (`move aN,a0` after
   prologue): a case body reuses the switch selector (`D_x = idx + 1`).
   Sanctioned literal recipe: write `D_x = <case+1>;` then patch back
   `patch_insn_imm16 OFF:<case+1>:1` + `patch_insn_reg_field OFF:rs:zero:v0
   OFF:rt:tX:tY`. (NAME_ENT/func_8014977C, BUNKASAI x4 incl. both 546-word
   sisters func_80139D8C/func_80140F54 — identical recipes, identical offsets.)
4. **jal-delay-slot store order is a FAITHFULNESS fix**: target `sw` in the jal
   delay slot = the store executed BEFORE the callee — if your C has it after
   the call, reorder the C (RPG_BAT/func_80136750, NAME_ENT/func_80147B30).
5. **Pre-store compare hoist**: target `slti/addiu/bne/sw(delay)` vs compiled
   `addiu/sw/slti/bne` → `cond = c < K; D = c + 1; if (cond == 0) {...}`
   dissolves the whole cluster including downstream li/lui rotations
   (NAME_ENT/func_80147B30).
6. **⚠ NEVER paint a reloc'd lui's register to fake a swap** — paints change
   bytes but relocs stay at their offsets, so the resolver later shows crossed
   hi16 VALUES with matching registers (ETC/func_80146D40, BUNKA_SD/
   func_8013320C). `swap_instructions` moves the reloc WITH the word — use it,
   then paint registers AFTER the swap (chain order matters).
7. **`| 0` identity-op on a base pointer breaks the const-index fold-wall**:
   `base = (u8*)((u32)D_x | 0); base[245] = ...` emits base-reg + plain
   immediate (a bare local pointer gets copy-propagated back into the fold)
   — TEL/func_8013C8FC, previously documented as unfixable
   (`%lo(D_800EE760+245)` re-fold at link).
8. **"HI-ONLY lui-web rotation" can be an unabsorbed cross-prologue ALT-ENTRY**:
   DATE2/func_80138238's specimen dissolved by absorbing the 44-byte fragment
   func_8013820C (@SymbolStart + .sym widen to one 768-byte entry named after
   the jal'd address; leading zero/one stores restored to the C head).
9. **`func_0xXXXXXXXX` extern style breaks the name-derived resolver**
   (name_to_addr wants `func_XXXXXXXX`; unparsed → defsym 0 → R_MIPS_26
   "truncated to fit" link failure). EVENT/func_80135498.
10. **resolve_overlay_relocs.py `NAME_ADDR_RE` now accepts alias suffixes**
    (`_b` etc.) — block-view externs resolve without paint games.

## Batch 13 (2026-07-26): the normalizer-semi split — 43 wins across 7 overlays

The 543 "semis with a whole-function scheduler_normalizer" split THREE ways, and
the split must be diagnosed per-function from resolver output before choosing a
strategy (stripping the normalizer is NOT always right — on some files it does
real scheduling work and the honest baseline craters):

1. **Normalizer artifacts hiding honest wins** (~1/4 of attempted): drain-playbook
   fixes apply. Dominant single bug: the jal-delay-slot store-order faithfulness
   fix (8 of GEKO's 12 wins alone).
2. **Register-transcription pseudo-C + pad_trailing_nop fabrication** (the
   majority): needs a full honest rewrite from the .s. Proven crackable at ~1-4
   per worker (TACO/TT wins). OPTION and TAIIKU are almost entirely this
   class — treat them as rewrite campaigns, not drains.
3. **Genuine blockers** (small tail): $at-as-memcpy family (10 in TACO-B's batch
   share one 32-byte rodata template copy), dual-v0, held-constant LICM,
   caller-live registers.

New levers proven this batch:
- **`u32`-typed copy survives copy-prop** where an `s32` copy is eliminated —
  reproduces the "dead `or vX,vY,zero` switch-head move" (TACO/func_80138880,
  refuted a filed BLOCKER).
- **volatile is a TWO-WAY lever**: removing it frees speculative %hi-lui hoists
  and removes load-delay nops (TACO/func_80133740, refuted a filed BLOCKER);
  adding it to ONE global pins the target's register web. Sweep both directions.
- **`u16` vs `s16` store type decides held-constant s-register reuse**
  (`sh sN` vs fresh `li`) — TT/func_8013C0A8.
- **Inline the function-pointer load into the call** (`(*(void(**)(void))(x+12))()`)
  to keep the target's load-use interlock `nop`; a temp variable makes IDO fill
  it with the spill — TT/func_8013A0DC.
- **`switch ((u32)x)` / `((u16)x)` cast is the const-web lever** — makes IDO CSE
  compare constants with stored values (and `(u16)` supplies a missing `andi`).
- **`IDO52_CONST_NOINREG_VALUES` steers WHICH literal wins an s-register**;
  CONST_INREG traced as firing but inert for the force-hold direction here.
- **`IDO52_NO_LOOP_TEST_REPLACEMENT=1` also suppresses loop unrolling** (both
  live in f_loopunroll) — can replace `-Wo,-loopunroll,0`. Sweep it against any
  residual that is a `slti/bne` vs `li/bne` exit-test pair (RPG_BAT/func_8014EC08
  fell; it converts a documented-unmatchable class).
- **Two functions in one .c** is the sanctioned way to cover an oversized `.sym`
  blob without a .sym edit (TT/func_8013AC0C 560-byte window; TACO/func_80141CF0
  via @SymbolSize windowing).
- **Alias extern (`D_x_b`) breaks cross-call CSE** of a base pointer
  (TACO/func_8013FB00, +65 words in one step).
- **`unchecked_jr_table` "blockers" can be unabsorbed cross-prologue heads** —
  the andi/sltiu/beq bounds check hoisted above the prologue and registered as
  its own 12-byte symbol (TT/func_8013C824). Check for a preceding tiny symbol
  before believing that filing.
- Tools: `tools/overlay/sbs.py` (side-by-side target/compiled disassembly with
  PostProc offsets) and `tools/overlay/fixframe.py` (auto-detect compiled frame
  for patch_stack_complete), harvested from batch workers.

## Batch 14 (2026-07-26): fresh-overlay drain — 64 wins across 11 overlays

Second parallel batch with class-triage-first prompts. 64 byte-perfect + 15
honest de-fabrications. Confirmed at scale: **"invented hidden param" and
"unabsorbed cross-prologue head" are the SAME defect** (DATE, KANGEI, SHUGAKU,
BUNKAKEN, ETC — ~20 wins were head absorptions; several heads had NO .sym entry
at all, hiding inside a predecessor's over-run). FOUR more filed BLOCKERs
refuted (ETC hidden-v0 + base_register_cse = unabsorbed heads; SHUGAKU
multu-by-56 family = CONST_INREG_VALUES; KANGEI caller-threaded-LO = head).

New levers:
- **Natural `for` beats hand-unrolling** — IDO re-unrolls hand-unrolled bodies
  wrongly; the plain loop reproduces the target's peel + 4-wide shape
  (EN_NICHI). `IDO52_NO_LOOP_TEST_REPLACEMENT=bb:N` is the selective form that
  keeps unrolling alive elsewhere (get N from IDO52_LOOP_TEST_REPL_DEBUG=1).
- **Unsigned-global type fix breaks the signed const-K web**: `u32glob == 1`
  emits a fresh `addiu $at,1` instead of joining the signed const-1 register
  web — three ENDING "const-materialization walls" were this one-character bug.
- **Identity-negation `0 != -(x % 2)`** defeats the signed-modulo zero-test
  fold, materializing the 5-word sign fixup (KANGEI x2).
- **Mid-loop pointer advance** (`p += stride` mid-body, later stores at
  negative offsets) reproduces IDO's advance placement; hoist the last
  global-sourced store above the advance (BUNKA_SD x3).
- **`(s32*)G + idx` beats `(s32*)(G + idx*4)`** — pointer-arith form fixes lui
  order AND addu operand order.
- **Chained assignment `a[0]=a[1]=a[2]=a[3]=K`** gives descending store order;
  an UNUSED local is legitimate when the target frame exceeds used locals
  (SHOUGATU/func_8013FB50 went byte-perfect with ZERO PostProc).
- **Selector reuse: re-read the global** (`D_x + 1`), never a local copy — the
  local emits an extra move (+113 words one step, BUNKAKEN).
- **volatile on the selector global is the head-scheduling lever** (SHUGAKU);
  volatile-ADD direction removed 42 residual words by stopping %hi register
  promotion (ETC/func_80144194).
- **Split two-step expressions across the branch** (`t = v & MASK;` before,
  `t >>= 16;` after) to stop IDO duplicating the computation into both arms.
- **Store-forward the counter** (`c = G+1; G = c;` compare `c`) keeps it an
  expression temp (BUNKA_SD).
- **`scheduler_normalizer --fence A-B` / `--reloc-symbol-fp`** are the safe
  narrow forms when residual is genuine placement; whole-function remains
  banned practice.
- Candidate blocker class seen 4x this batch: **phi-copy for a branch-merged /
  loop-carried accumulator** (`move rX,rY` that IDO coalesces or duplicates
  differently than the target). Resists all known levers — needs a hook
  investigation. Specimens: EN_NICHI/func_8013352C, func_80133708,
  SHUGAKU/func_80138E54, ETC/func_80144D7C.

## The 2026-07-27 Batch 15 — six-vein parallel drain (+20 byte-perfect, ALL 29 re-audited blockers refuted)

Six parallel worktrees, one per priority vein. Effective 85.1% → 85.3%
(byte_perfect 4,943 → 4,963, semi 756 → 740). The headline discoveries:

### The phi-copy "class" was three different defects (resolved batch-14 open item)
The 4 specimens split; triage rule: **run `IDO52_DUMP_WEBS=1` FIRST**.
- **Hand-peeled loop + local accumulator** (EN_NICHI/func_8013352C): the merge-point
  `move` is the artifact of hand-peeling. A plain `for (i=0;i<10;i++)` doing global
  RMW lets IDO produce its own 2-peel + x4-unroll (10 = 2+2x4) — 90 diffs in one
  edit. Do NOT introduce a local accumulator; let LICM promote the global.
- **`u8` scratch local** (EN_NICHI/func_80133708): `u8` splits the `lbu` dest from
  the variable's web; the copy reconnects them. `u32` → one web → copy gone.
- **Genuine allocator wall, crackable** (SHUGAKU/func_80138E54):
  `IDO52_PRECOLOR_WEB=v0:53` — pinning the RETURN web forces ugen to materialize
  the copies. **A color pin CAN create a copy the natural compile lacks** (refutes
  the Hook-18 note) when the pin makes two webs non-coalescable. Sweep bitpos
  0..60+, not 0..45 — the working positions were 23 and 53.
- **ugen argument-coalescing, NO uopt hook can reach it** (ETC/func_80144D7C):
  `DUMP_WEBS` shows the value never becomes a uopt web — it stays a ugen
  expression temp, and the $a0-vs-$v0 choice happens in ugen's call-arg setup.
  Hook spec filed: `IDO52_UGEN_NO_ARG_COALESCE=1|occ:N` — gate coalescing of a
  call-arg expression temp into $aN when the temp has a second consumer.
  If the value has a web → PRECOLOR sweep; if not → it's ugen, stop sweeping.
- Two more sibling specimens found (RPG_BAT func_8013C890 [54], GEKO
  func_80133024 [41], TT held-0 stores, main-binary func_80076544): the
  **live-range-split / phi-copy family is the single biggest remaining honest
  blocker class**. `IDO52_EXCLUDE_REGS` is accepted-and-printed but inert on it.

### ⚠️ Whole-function scheduler_normalizer can emit SEMANTICALLY BROKEN objects
Confirmed twice with damage: RPG_BAT/func_8013C890 (dragged a delay-slot `move`
195 slots down, OVERWRITING the `lw $ra` restore — and the masked score
*rewarded* the broken object 251/256 vs the correct 254/256) and GEKO/
func_80133024 (two back-to-back branches on $at). NAME_ENT/func_8014AD68's only
"2 codegen diffs" were also pure normalizer damage (transposed sb lo16 fields).
- **Delete the normalizer FIRST, then diagnose.** Trust only the resolver.
- **Point fences** `--fence N-N,...` keep useful block realignment while pinning
  the same-shape words it mispairs (refuted a filed delay-slot blocker).
  Fences only help when the word is already at the right index.
- **`--no-forced-reorder` is load-bearing for correctness** (MASTER/func_801363FC:
  without it a `sh` migrates past its base-pointer `addiu`). `--fence` does NOT
  prevent intra-region reordering. ETC/func_8013D560's old 81.6% masked was
  manufactured by dependency-bypassing forced reorder + 22 adjust_branch_offset;
  honest form is 25 directives → 2.
- Recommended sweep: grep for whole-function `scheduler_normalizer` (no
  `--fence`/`--block-aware`) on >100-insn functions.

### Cheap high-volume type/form levers (new)
- **Unsigned modulo**: `x % 11` (signed) = 6-word div guard (`break 0x7` chain);
  target `divu` ⇒ write `% 11U`. Killed a 105-word overrun in one edit (TACO).
- **Unsigned shift**: target `srl` from a signed `>>` site ⇒ cast `(u32)`.
  ~10 sites in one function (BUNKASAI/func_801433C8).
- **Struct assignment**: `$at`/`$tN` alternating lw/sw chain = IDO struct copy.
  Write `typedef struct { s32 w[N]; } T;` + plain `=`; element stores never match.
- **Unrolled-loop recognition** (2 workers independently): a run of hand-transcribed
  indexed stores + stray `li K; sll; addu` = IDO's x4-unroll + peel of a plain
  `for`. No literal-index C reproduces it; the loop does (closed a 10-word gap).
- **`int` return + bare `return;`**: a target `lw ra` in a branch delay slot where
  you emit a real insn can mean $v0 must be live-out (dead `int` return) so as1
  can't fill the slot — the don't-care-return diagnostic in reverse.
- **Memory-residence via 1-element array** (MASTER/func_801363FC): declaring the
  one register-pressure culprit as `qpa[1]` (write `qpa[0]=qp;` at loop top,
  `qp=qpa[0]+1;` at bottom — the spilled-register form) removes ALL s-register
  use (`.mask 0x80000000` like the target), restores in-loop remat of a hoisted
  bound, and made IDO52_NO_SREG_PROMOTE unnecessary (hook use deleted). Sister
  lever: ETC/func_8013D560's `uv[2]` restores `multu` by evicting the s-reg that
  otherwise ate the constant. Read the tail ONCE into locals (aliasing blocks CSE).
- **`volatile` is per-function, not per-family**: BUNKASAI sisters needed it OFF
  (blocks cross-branch %hi sharing) and ON (redundant store-back). Test both.
- **Extern `union { s32 w; u8 b; }`** gives one global word+byte views, both in
  %hi/%lo reloc form (a `*(u8*)&` cast costs an la pair).
- **C `switch` is the ONLY form** laying all compares first with out-of-line
  arms (if/else inlines arms; goto-web inlines single-predecessor arms).

### CONST_INREG extensions (SKIP-list revisions)
- **Non-loop multu-by-nonpow2 with mflo use IS crackable**: `IDO52_CONST_INREG_VALUES=240`
  byte-perfected ETC/func_8013D2C8 (the filed "caller-threaded LO" was an
  unabsorbed head + this). Extends the batch-14 SHUGAKU x56 loop-case refutation.
- **`multu` by cached register from a multiply**: `x * 3U` (UNSIGNED — the signed
  path lowers to sll/subu before f_constinreg is queried; the hook-doc claim of
  impossibility holds only for signed) + occurrence window (`3:-1:1:2` —
  force-all also flips an unrelated sltiu). `IDO52_CONST_INREG_DEBUG=1` prints
  queries only on a direct `cc` invocation, the harness swallows it.
- `CONST_INREG_VALUES` is INERT for making `mul` take a register operand
  (confirmed Hook-16 limitation); `LDA_NOINREG_CLASSES=4` does it but evicts the
  base pointer (+15 lui). ⚠️ This tree's uopt build predates the LDA
  occurrence-window parser — `=4:0:0` behaves as `=4`; rebuild before landing
  windowed LDA entries.

### Blocker re-audit method (29/29 refuted)
EVERY filing in the unchecked_jr_table / hidden-param / caller-threaded families
was an unabsorbed cross-prologue head (9 had NO .sym entry — found inside the
predecessor's over-run; check `addr+size` of the preceding entry vs the filed
function's start). 5 filings were flat-out stale (already byte-perfect on disk).
The MAIN BINARY has the same disease and has never been swept — 10 filings with
head-by-head evidence banked, incl. byte-identical sisters func_800937B0 /
event_face_kuchi0. New residual classes from the refuted-but-unmatched tail:
store_sink_div_shadow, branch_layout, wrong_global_addresses, held_constant_zero
(register-held 0 for sh stores — CONST_INREG fires but ugen lowers literal-0
stores to $zero regardless).

### Workflow
- **difflib `SequenceMatcher` over opcode-class-keyed word streams** (target .EXN
  vs raw .o .text minus the 4 thunk words) pinpoints exact insertion/deletion
  indices — far faster than masked-diff dumps for 1-word residuals and gaps.
- **Resolver residual-count beats exact% for triage** (a 39.5%-exact function was
  one C edit from byte-perfect; count-fakes read high-masked).
- The one-instruction-away figures in old TODO banners were INSTRUCTION counts,
  not exact-word counts — re-triage before chasing "1-word" leads.
- Sweep tools harvested from scratch copies: `pcsweep2.py` (PRECOLOR sweep scored
  by compiled word count — the count-deficit metric), `precolor_sweep.py`,
  `websweep4.py`, `envsweep4.py` (in scratch copies, candidates for
  tools/overlay/).

## The 2026-07-27 Batch 16 — six-vein drain #2 (+27 byte-perfect, 85.7%; the as1 discovery)

byte_perfect 4,963 → 4,990, effective 85.3% → 85.7% (5,438/6,346), count_mismatch
191 → 174, sym-integrity audit 0/0/0 tree-wide. Six parallel workers; the headline is a
ROOT-CAUSE CORRECTION to §Batch 15.

### ⚠️ CORRECTION to §Batch 15: the phi-copy deleter is as1, NOT ugen
`cc -S` (stops after ugen) shows ugen ALREADY emits the target's `move` — it is
**as1's per-block peephole** (`f_do_passII_opt` → the copy test in func_419268)
that recognizes the copy and renames the defining insn's destination. Downstream
of every uopt AND ugen hook — which is why all prior probes were inert and why
"DUMP_WEBS shows no web" was true but misleading. The working lever is the new
**`IDO52_AS1_NO_COPY_PROP=all|i,j,...`** hook (occurrence-indexed, 0-based; a bare
numeric is an INDEX not "all"; `IDO52_AS1_COPY_PROP_TRACE=1` lists candidates).
Landed in `apply_as1_hooks.py` (Hook 6), toolchain rebuilt, 18/18 + 5/5
bit-identical regression with the var unset. First confirmed win:
GEKO/func_80133024 (70/70). ETC/func_80144D7C reached 97/97 count-exact with one
UNRELATED word left (arm-shared `andi` hoist). The ugen Hook 22
(`IDO52_UGEN_NO_ARG_COALESCE`) was built and works mechanically but is NOT the
fix for this class. The class spans BOTH halves of the project (4 of 9
main-binary head-sweep residuals are it) — retry everything filed under
phi-copy/live-range-split with the as1 hook.

### The second as1 wall (new hook spec, 3 specimens)
`RPG_BAT/func_8013F258`, `DATE2/func_80136868`, `KANGEI/func_80135850` all hit:
as1's intra-block list scheduler front-loads a longer load chain over a ready
store, starving the following branch delay slot of its `lui` and duplicating the
epilogue `lw ra` (+1 word). No existing hook reaches `f_reorganize_bb`
(`IDO52_AS1_NO_SCHEDULE` crashes; Hook 21 is cross-BB only). Spec:
`IDO52_AS1_NO_SCHED_LOAD_HOIST=1|occ:N` — veto hoisting a load above a ready
store. Highest-value unbuilt hook.

### Broken-normalizer sweep (tree-wide, CLOSED)
Of 1,578 un-fenced whole-function `scheduler_normalizer` directives, **322
produced structurally broken objects** (45% of >100-insn functions, 4% of small);
**zero were byte_perfect** — no recorded win was built on a scrambled object.
186/252 paint chains only applied against the scrambled layout (fitted to
garbage); 237 stale steps removed. 243 manufactured masked scores collapsed
honestly (80.8%→5.5% class). Fix shape: `--no-forced-reorder` everywhere + drop
newly-failing steps. **Gate**: `tools/overlay/semcheck.py` (final-object
structural checker — branch-in-delay-slot, ra-clobber, jal-delay ra write,
post-dealloc frame access; 259/260 clean on the byte_perfect control set). The
1,269 remaining un-fenced normalizers are clean but DORMANT: any future
`gen_reg_patches` without `--no-forced-reorder` re-manufactures fake scores.

### ETC sprite-descriptor-init family (7 byte-perfect, one recipe)
The "dispatcher pad family" is mostly NOT dispatchers: 3-call prologue + 68-byte
`$at` struct copy + descriptor priming. Recipe: `typedef struct {s32 w[17];}
Blk68` + plain assignment, named externs at exact widths, `patch_stack_complete`.
~30 more sisters: `grep -l 0x800852DC asm/overlay/ETC/*.s`. New levers from the
family: **induction updates in the for-header comma list** (body-tail increments
cost 18 words — IDO hoists pointer bumps and switches to negative
displacements); **two-induction loops** (separate 0-based index from the arg
counter); **`% <local var>` emits guarded div+mfhi** where a literal `% 112`
emits reciprocal-multiply (no hook needed); **struct field-order steering**
(`{s16 pad; s16 h}` shifts the lh displacement); **declaration order: IDO gives
the LAST-declared local the LOWEST stack address** (confirmed by standalone
probe in TEL). Refuted wall: ETC/func_8013CCDC — target materializes index 2 in
a register; every C form folds it in uopt's simplifier BEFORE f_constinreg;
forever 1 short.

### Main-binary pipeline traps (from the head sweep)
- **`test_single_match.py` truncates a longer compile to the target window** —
  the main-binary truncated-compare trap. Verify against `disks/SLPS_000.65`
  with the full untruncated window (trailing ZERO words past the window are
  IDO's 16-byte .text padding, benign).
- **`fixup_relocations` (build_level3_c.py:830) copies the target's immediate
  into ANY compiled I-type word with matching opcode+top16** — silently corrects
  wrong frame offsets in the REAL BUILD, not just the test.
- **`@SymbolStart`/`@SymbolSize` parse ANYWHERE in the header comment including
  prose** — mentioning them in explanatory text silently re-windows the compare.
- `@SymbolStart` absorption works in the main pipeline; all 10 main head filings
  were refuted in whole or part (6 honest semis committed). The filed symbol
  fade_bri_set does not exist (real entry fade_in_step).
- **Magic-multiply is COMPILER-DEPENDENT, not unmatchable**: Psy-Q GCC 2.6.0 -O2
  emits the exact mult/mfhi/sra pattern for x/3; IDO emits div+mflo. Determine
  the compiler before filing any magic_multiply blocker in the library region.

### SKIP-list refutations (experimental, from the hygiene pass)
- **`$at`-as-memcpy**: IDO -O2 emits the target's exact `lw $at/sw $at` chain
  from a PLAIN STRUCT ASSIGNMENT — alignment picks the form (align 2 = lw/sw +
  lhu/sh tail) and the destination must be the variable itself (member/array/
  cast lvalues drop to swl/swr). BUNKASAI/func_80142BF4 (379 words) proved it.
- **div-by-nonpow2 (quotient)**: `x / 20` → `li $at,20; div; mflo` exactly; the
  blocker only holds when the TARGET strength-reduced and IDO won't.
- `CONST_INREG_VALUES` is inert for ADDRESS-SCALE multiplies (ugen
  strength-reduces every constant address scale); un-CSE'd multu pairs come from
  spelling the two index operands differently (cfe can't CSE) — REOPENS
  GYOZI/func_80134124.
- Windowed-parser gaps in the (now rebuilt) toolchain: LDA_NOINREG occurrence
  windows now available; stack-buffer LDAs still bypass `f_ldainreg` entirely
  (spill-vs-remat on sp buffers unreachable — hook gap, TEL/func_80139DC8).

### Head-absorption practice notes
- **ALWAYS check predecessor over-runs when applying .sym folds**: batch 15 found
  3 unflagged; batch 16 found ELEVEN more (3 ETC-adjacent + 8 in DATE/GYOZI whose
  "ABUT" bounds claims were wrong — the audit_fabrication §4 sym-integrity check
  is the backstop; run it after every fold batch).
- 5 absorptions this batch REMOVED asm_only credits while hosts stay semi
  (honest -5 until cracked): the de-fabrication cost is real; quote only the
  reconciled JSON.
- Refreshed head-pool top-10 (TACO x2 @5 residuals lead) in the batch-16
  absorption worker report; scanner: scanheads.py (scratch copies).

## The 2026-07-27 Batch 17 — eight-worker drain (+13 byte-perfect, crossed 5,000; two as1 hooks live)

byte_perfect 4,990 → **5,003**, effective 85.9% (5,445/6,338). Eight workers.
Toolchain fully regenerated at close: 36 uopt hooks (LDA occurrence windows now
REALLY in — the prior build predated them and the idempotent apply script
silently skipped; symptom: `CLASSES=1:0:0` ≡ `CLASSES=1`. After ANY doubt,
`rm build/5.2/{uopt,ugen,as1}.c && make c_files && re-apply && make`),
10 as1 hooks including **Hook 7 `IDO52_AS1_NO_SCHED_LOAD_HOIST`**
(`1|all|occ-list|strict|strict:-N` exclusion form; first win RPG_BAT/func_8013F258
64/64 — full node-record map and tie-break trap in apply_as1_hooks.py Hook 7 block).

### phi-copy family: FINAL taxonomy (narrows §Batch 16's "retry everything")
The as1 Hook 6 retry swept the family: **2 crackable** (GEKO/func_80133024,
ETC/func_80144D7C at 97/97-minus-1) / **3 NOT as1 at all** (zero pass-II
candidates; a pass-I veto probe at as1.c:22743 is ALSO inert — do NOT build a
hook there; a third deleter exists) / **3 wrong-direction** (WE emit the extra
copy — the hook only preserves copies) / **3 held-constant-zero** / 1 stale.
Main-binary: hook cracked fade_in_step (`=all`) and event_face_kuchi0 (`=2`) —
**the index and all forms are function-specific and can be OPPOSITE; always try
both** (vetoing reshapes the candidate stream). `AS1_NO_COPY_PROP` also
renumbers the xbb candidate list, so it cannot yet combine with
`AS1_FORCE_LA_PULL=<index>` (ENDING/func_8013858C blocked on exactly this).

### New levers (batch 17)
- ⭐ **volatile alias-view**: a SECOND volatile-qualified extern for the same
  address (`D_800EF864_v` — the resolver's name rule sanctions the suffix) gives
  volatile semantics in %hi/%lo folded form; `*(volatile T*)&G` casts force the
  LDA form. Cracked both 1,500-byte TACO dispatchers (plain global = 39
  rematerialized loads). Parameter form: `volatile u16 arg1` makes IDO re-read
  the ARG HOME slot (TT/func_8014742C — retired the spill-vs-remat filing).
  Sister grep: every `*(volatile T*)&` cast is a candidate.
- ⭐ **alias-kill**: when the target reloads a global after a store but IDO CSEs
  it, write the read as an element of the array being STORED to
  (`D_800EF878[3]`, `p=D_800EE760; p[3935]`) — defeats provable-non-alias CSE.
  volatile is WRONG here (forces both paths). ETC/func_80144F08.
- **Literals, not constant locals** (single-use): `s32 three=3` in a loop hoists
  to an s-reg + guard; the literal folds to `li $at,3` + guardless div. BUT the
  divisor-local SIGNEDNESS picks guard shape when a local IS needed:
  `u32 three=3U` → guarded divu + single break; literal `% 3U` → guardless;
  `s32` → div + two-break guard (TT twins).
- **Global-array indexing beats a walking pointer** (`D_x[i].fld` vs `p++`) for
  alias analysis → delay-slot fill + reload across a `break` barrier.
- **jr-table emission**: an explicit `case 0:` ahead of `default:` is what makes
  IDO emit the jr table (vs a beq chain); write the DEFAULT arm first to land it
  after the `jr`; `for (i=0;i!=N;i++)` places counter init AFTER LICM-hoisted
  addresses (do/while emits it first).
- **Switch case ORDER picks body layout** (ladder is emitted ascending
  regardless); repeated probe pairs must be separate `if`s, not else-if.
- **Per-case statement order** decides pre-call address compute (sll in branch
  delay) vs post-call spill/reload — lead with the rand-derived store.
- `patch_stack_complete` with a wrong `--old` is a SILENT NO-OP — read the raw
  prologue frame first (a 192-frame function carried `--old 24` for months).
- Dead leading array (`s8 pad0[1]`) shifts stack-array offsets by 4 (mod-8
  steering); declared-local count moves them in 8-byte steps.
- `goto`-shaped `if` steers branch polarity when if/else inverts it.

### Refutations and integrity (batch 17)
- THREE more "hidden param / caller-threaded" filings were unabsorbed heads
  (ETC func_801463F8 + func_8014145C, EN_NICHI func_80133B70 filed >2-hidden-
  params, DATE func_80140CD4 caller-threaded-HI incl. a missed 5th arg) and one
  filed "byte-perfect IMPOSSIBLE" (EN_NICHI/func_80136164) is byte-perfect.
- **func_80099040 is Psy-Q GCC 2.6.0** — the magic-multiply is GCC's x/3
  verbatim; blocked ONLY on gccwrap.py not passing --expand-div to maspsx.
- OPTION/func_80135BCC was a stray draft of an already-perfect BUNKASAI function.
- RPG_BAT/func_80138F74: another --reloc-symbol-fp normalizer swapping
  byte-identical pre-link words between DIFFERENT globals (honest 109/315).
- `hoist_out_of_delay_slot.py` does not carry relocations (tool bug, 2 free
  words); `gccwrap.py --expand-div` gap; `swap_registers_liveness` fails listed
  twice.
- New blocker classes: `as1_const_immediate_fold` (as1 sinks a hoisted li past
  use #1 and folds to an immediate; wanted hook: veto folding when the register
  has further uses), `multu_const_cfe_cse` (cfe-level, unreachable), genuine
  caller-threaded `mfhi`-from-caller (ETC/func_80149ED8), as1 delay-pull-from-
  fallthrough (wanted hook: `AS1_NO_DELAY_PULL_FROM_FALLTHROUGH=occ:N` —
  exhaustively probed inert against all existing knobs).
- **174 `.sym` over-runs swallowing jr-ra-less heads** tree-wide (DATE 69,
  TACO 33, GYOZI 23...; scanner in scratch copies) — dedicated hygiene pass
  queued; 15 have count_mismatch hosts (absorption targets).

## The 2026-07-30 Batch 18 (IN PROGRESS — mid-batch catalog; 83 byte-perfect landed across batches 18a+18b so far)

Two batches of 9–10 parallel workers over TACO/TT/TAIIKU/RPG_BAT/NAME_ENT/OPTION/
SHOUGATU/GYOZI/DATE/BUNKAKEN/EN_NICHI. This section is written mid-batch so the
learnings survive; final numbers land at the batch-close reconcile.

### ⭐ The headline pattern: "filed unmatchable" = misread cross-prologue head

Five TAIIKU functions filed permanently unmatchable ("alt-entry fragment",
"hidden t1/t3/t5 register params", "two .word jr-table dispatches", "hidden
$t6"), a 4-sister RPG_BAT cluster filed "caller-threaded alt-entry", and
DATE/func_8013A6B0 ("hidden v0 param + jr table, not C-matchable") ALL fell to
the same move: the function's own hoisted head was parked in the previous
symbol's window (or unowned), making the entry look like it received values in
temp registers. Two of the TAIIKU five had 33,400- and 5,940-iteration permuter
plateaus that a head absorption solved instantly. **Diagnostic**: a filing that
claims hidden register parameters, alt-entry, or caller-threading deserves a
look at the 8–68 bytes BEFORE the symbol — `@SymbolStart`/`@SymbolSize` over
the head + globals-first C usually cracks it. A bare `addiu $rX,$rX,K` at entry
is a hidden PARAMETER, not a blocker (a named-extern bound there manufactures
the "won't fit" extra lui). Sweep queued over c_unmatchable + file headers.
⚠️ Converse trap (DATE worker): an 8-byte block before a function is often the
PREVIOUS function's `jr ra; nop` epilogue, NOT a head — disassemble before
absorbing. And heads are READ-hoists: a store-first head (e.g. SHOUGATU
0x80139DD4 `lui $at` consumed by `sb`) has NO known host-C absorption form.

### ⚠️ scheduler_normalizer produced SIX broken or misleading objects this batch — treat as hostile

1. TAIIKU def-use clobber: moved `lui $v0` past `beqz` so `divu` divided an
   ADDRESS (semcheck blind).
2. TAIIKU: scheduled `addu t4,t4,a0` ahead of the `sll t4,a0,2` defining t4
   (semcheck blind).
3. TAIIKU: moved a block's loads across a guard branch, silently changing the
   branch offset; on the same function it was MANUFACTURING a documented wall
   (4 "lh pair-order swaps" vanished when the normalizer was removed).
4. RPG_BAT: left a backward `bne …,-32` into mid-function (masked % hid it).
5. RPG_BAT batch-18a: dragged five LO16 relocs across sibling cases so case
   bodies referenced the wrong globals.
6. Parallel investigation (DATE/func_8013EF2C shape): normalizer pairs reloc'd `lbu`s BY
   OPCODE ALONE (pre-link all are `lbu rt,0(rs)`) and permutes them into each
   other's slots — masked test prints "RELOCATION-ONLY / 100% masked" while
   the resolver shows every load reading the WRONG SYMBOL. `--load-fp` does
   not save it.
Rules: prefer explicit `swap_instructions` (they CARRY RELOCS — for order-only
diffs they beat register paints, which strand lo16 relocs on the wrong words);
if only one small window is mis-scheduled, hand swaps beat a whole-function
run; `--no-forced-reorder` avoids the def-use class; fence indices are
WINDOW-relative (an absorbed head shifts every fence); whole-function
(non-block-aware) IS right for divide-latency store sinking + repair with
`adjust_branch_offset`. semcheck does NOT detect: general def-use clobber,
branch-skips-live-code, undefined-register reads from paints — eyeball the
final object on anything normalizer-touched.

### ⚠️ gen_reg_patches can emit BANNED paints — two shapes

1. nop→sll fabrication: target `sll $tX,$tY,0` (live-range-split copy) vs
   compiled `nop` (which IS `sll $zero,$zero,0`) → it emits
   `0xNNN:rd:zero:tX 0xNNN:rt:zero:tY`, fabricating a real instruction from a
   nop and producing a FALSE ✅ from the resolver. RPG_BAT/func_801439E0 was a
   fake effective win this way (honest 145/146, demoted). Tree-wide sweep
   2026-07-30 found only one other same-offset zero-pair instance (an
   uncounted TAIIKU semi). NOTE (RETRACTED 2026-09-27): the "selector+1" recipe -- a single
   `rs:zero:v0` paint paired with `patch_insn_imm16` -- is NOT sanctioned; it makes
   the object read a register the C never reads using an immediate copied from
   retail. Write `sel + 1` in C (CLAUDE.md, the banned list).
2. Branch-polarity flips via the rt field (bltz/bgez differ only there).
Also: it silently refuses on `/* @PostProc:` headers lacking a same-line `*/`,
and can "fix" reloc'd `addiu` register fields while leaving lo16 constants in
the wrong slots (the real defect was ORDER — transpose first, then paint).
Tool fix queued: refuse to paint a compiled nop; audit category.

### The raw-count self-audit is MANDATORY for wins

`resolve_overlay_relocs.py` slices the compiled body to `func_size`, so
`len t=X c=X` is satisfied BY CONSTRUCTION and cannot see surplus
instructions. Two fakes passed this way in one batch (GYOZI, TT). A win
additionally requires: compile with ALL PostProc stripped, body word count ==
target count (trailing section-pad nops excluded).

### New levers (exemplar files in-tree)

- **`(x & 0x80000000) == 0` instead of `x >= 0`** — the peephole rewrites the
  mask into a sign test and materialises the operand through `sll rd,rt,0`
  landing in the preceding branch delay slot. Every identity spelling folds
  (`x<<0`, `x*1`, `x|0`, re-read); only the sign-bit mask works.
  (RPG_BAT/func_8013B910)
- **`| 0` identity** breaks the phi-copy on values with OPAQUE provenance
  (cracked a 5-function NAME_ENT cursor cluster whose headers filed
  `phi_copy_cursor_dual_register` with ~40 probed spellings — the filings were
  stale, the lever postdated them). IDO folds `| 0` on known-bits values
  ((t6|0)&0xFF folds flat).
- **volatile multi-access base-register** (TAIIKU/func_8013D364): ≥2 volatile
  accesses to the same named-extern base → base register held for loads AND
  stores, across branches, re-materialised after jal. Single volatile = loads
  only. The cast must sit on the named-extern array expression — a pointer
  local silently breaks it. Refutes base_register_cse filings at MULTI-access
  sites (the category is real only for single-access).
- **Unsigned tail multiply** `x * KU` splits a merged const register web —
  signed `x*K` never queries f_constinreg when the value is already
  register-bound (why CONST_NOINREG probes were "inert").
  (SHOUGATU/func_8013BB84 — refuted its own held-constant filing)
- **Allocator-inside-loop** (TT/func_8014B078): check the loop-back branch
  TARGET INDEX before assuming a call is a one-shot prologue call — three
  "const-def placement walls" were the C calling a record allocator once
  before the loop while the target loops back to the call.
- **Hidden args found**: `set_XAsector` takes THREE args; `class_clear_n`
  takes TWO — grep for narrower declarations before trusting a spill residual.
- **Don't-care return**: a void function written with a return value creates
  the copy that steals the `lw ra` delay slot; `default:` arm placement is
  SHAPE-DEPENDENT (last for RPG_BAT/func_801513C8's layout; FIRST reproduces
  ladder→default→cases→shared-epilogue on func_80143DBC) — read the target's
  block order.
- **Struct-copy width is sizeof**: 10-byte `struct{u8 b[10]}` assigned to the
  BARE variable gives the lw/lw/lhu + sw/sw/sh chain; member/union destination
  switches to swl/swr. For the DATE 726-byte shape the web-split comes from
  the flat ARRAY-INDEX form (`&buf.b[i*33]`), not volatile (volatile on the
  copied globals blows up length).
- **Frame/local-slot steering**: declaring a scalar local BEFORE a big buffer
  changes frame size AND slot (sel-first → 776/44; none → 768/40; after →
  776/48).
- Constant byte offset belongs in the LOAD OFFSET (`rec[223]`), not address
  arithmetic (+446 costs a surplus addiu). IDO parks a global address in an
  sreg only at SIX+ distinct address computations (5 → remat). `x*N` yields
  `multu` only when ≥2 distinct multiply chains survive CSE. volatile kills a
  ugen web-split move (22 mismatches → 0 in one edit, DATE/func_8013EB18).
  volatile u8 global for the repeated-`sb`-at-every-nesting-level shape.
  Merging two locals (`a = (u8)(a|0x40)`) kills a phi move. PRECOLOR_WEB can
  REMOVE a copy, and bitpos is found by blind 0..30 SWEEP (~60s; trace
  segfaults). Paint the live-range-split copy's rs/rd too, then
  `nop_at_offset` — a provable self-move (painting only neighbours leaves live
  damage). Resolver alias suffixes must be `_[a-z]{1,3}` (`D_x_0` NEVER
  resolves — silent word loss). `nop_at_offset` LEAVES THE RELOC RECORD (the
  link re-injects HI16 — silent no-op). `convert_ori_to_addiu --offsets`
  takes WORD INDICES. PostProc offsets are `0x10 + 4*index`. Mixed spelling
  (`(u8)m` vs `m & 0xFF`) breaks CSE of duplicate masks. Hook 20
  (`IDO52_NO_LOOP_TEST_REPLACEMENT=1`) is under-used and works at
  `-loopunroll,0`. Bidirectional `swap_registers` + position transposes BEFORE
  filing an allocator wall.

- **The if/else dead-ELSE template**: IDO emits `bcond ELSE; <else in delay>;
  b ENDIF; <then in delay>; ELSE: <else body>` — when the else body fits the
  delay slot, the ELSE block survives as UNREACHABLE DEAD CODE. If the target
  has a dead `or $rX,$zero,$zero` + a redundant `b`, write a full `if/else`,
  not zero-init-then-set. (GYOZI/func_80139DBC)
- **PRECOLOR_WEB sweep gotcha**: pin the register the web should move TO, not
  the one it already has (a v1 sweep found only a wrong-web pin; the a0/a2/a3/
  v0 pins reached 64/64). Sweep by MNEMONIC-IDENTITY score, not exact bytes —
  register fields are the paintable class.
- **True-body-length, not padded delta**: a raw-count audit must strip the
  leading types.h thunks AND the trailing pad — a padded `delta=+0` reported
  three bodies as count-exact whose real bodies were 63/65/31 words.
- **Single-exit return web**: per-arm `return` statements make IDO spill the
  return value to `$s0`; a `goto end; … end: return v0;` shape reproduces a
  single early `or $v0` def when later calls clobber $v0 (don't-care-return).
  Fallthrough arm order must match the target (== 0 arm first ⇒ no b/move
  pair). (GYOZI/func_80133520 — also a hidden `func_8008F618(D_800F62CF)` arg)

### New blocker evidence (candidate filings at batch close)

- **load-once/multiply-N** (~7 DATE funcs): commoning is NOT in uopt's
  expression hash (`NO_CSE=all` changes 1/40 objects); every lever inert.
- **delay-slot-choice, ugen-upstream** (peer, DATE/func_8013EF2C): 2048 C
  variants + all hooks inert; as1 proposes no candidate for the site, so NOT
  la-pull.
- **codemotion_two_sites** additions (RPG_BAT 801426B8 filed); one-word
  "copy sunk into default block fills the b slot" class (8014F798, 80151600,
  80144B8C). as1 branch-target duplicate-vs-move needs a Hook-21 copy knob
  (wishlist). held_constant_store_pair_ordering (NAME_ENT/func_80149A3C).
  arg_a0_preservation (NAME_ENT/func_8013B108). join-phi-via-stack spill
  (TAIIKU x3, hook candidate). multu-CSE "queried but ugen re-lowers" inert
  case re-confirmed BY TRACE on GYOZI/func_80134124 (CONST_INREG_DEBUG shows
  the value queried twice; every forcing spelling bit-identical — Hook 16
  known limit, cf. ETC/func_8013D560; full probe matrix in the file header).

## The 2026-07-30 Batch 19 — resumed queues + the flagship research arc (+26 wins and counting)

Five territory workers (TACO/GYOZI/EVENT/SHOUGATU/DATE+RPG_BAT) on measured
easy-queues, plus two research workers that solved the flagship blocker. Two
early stops (one deliberate, one usage-limit) cost almost nothing because of
the new commit-every-win-immediately doctrine — 6 wins were salvaged straight
from stopped worktrees, two of them from uncommitted-but-passing files.

### The flagship research arc (DATE/func_80132728, 1,479 insns)

A model for how to close a "hard" residual: (1) a matching worker re-derived the
inherited "14-word two-mechanism impossibility" down to ONE word with a
reproducible demonstration; (2) a C-side prober ran a 17-lever matrix and
PROVED source can't reach it, correcting the mechanism along the way (it is a
consumed load-delay `nop` requiring ugen `base_reg == index_reg` — see Hook 23
in UOPT-HOOKS-REFERENCE for the arithmetic); (3) a toolchain researcher built
the occurrence-windowed hook in a PRIVATE compiler copy with a 20/20
bit-identical regression gate; (4) the shared toolchain was rebuilt only after
every matching worker had landed. Each stage cross-validated the previous one.
Lesson: "documented mechanism" claims dissolve under direct re-derivation —
this is the third case this batch (14→1 word; late-remat → consumed-nop;
"caller-threaded live-in" filings → own heads, again).

### The EVENT head-absorption vein (largest single-worker haul)

36 cross-prologue heads absorbed in one overlay (~1,856 bytes no symbol owned):
deleted 32 invented "hidden parameter" declarations and ~130 PostProc
directives (68/43/33/16-entry paint lists, 9 normalizers, 1 pad fabrication);
22 files left with only `patch_stack_complete`. Plus one falsely-byte-perfect
painting custom_script exposed (func_80141AD8) — now honestly perfect. When a
file carries a huge paint list AND an invented parameter, check the 8–68 bytes
before the symbol FIRST.

### Integrity results

- GYOZI: **all 401 byte_perfect functions swept clean** — byte-perfect, len
  equality, AND zero surplus real instructions (a stronger check than
  audit_truncated_compare; candidate for a tree-wide audit category). 37 of 60
  normalizer directives proven INERT (bit-identical objects) — the catalog is
  kept in the project's private archive.
- scheduler_normalizer failure count reached NINE this batch (GYOZI back-edge
  → forward-branch rewrite with masked score RISING; SHOUGATU andi-into-jal-
  delay-slot masking a pre-call value, semcheck "clean"; the flagship's
  girl*72-discarding object). SHOUGATU's full 96-normalizer sweep found no
  additional instance — the corruption is rare but real, and only object-level
  eyeballing catches it.
- Two NEW tool traps: `test_overlay_match` PADS the compiled buffer to target
  length (printed 152/152 for a 151-word body — mirror of the truncation trap;
  only the raw-count audit sees it), and `swap_instructions` has an offset-base
  inconsistency (first directive in a file function-relative, later ones
  .text-relative +0x10). And the annotation scanner reads directives INSIDE
  prose comments — never quote `@SymbolStart:`/`@PostProc:` verbatim in a
  header narrative (cost one worker 13.2%→1.5% silently).

### New levers

- **Address-local hoist**: `p = &D_GLOBAL;` in the OUTER block + `*p` at the
  use site hoists the bare `lui` ~22 insns early into a load-delay slot WITHOUT
  dragging the load (SHOUGATU sisters 80133400/80133218 — solved a residual
  filed as refuted, because the old probe moved the whole read).
- **Sparse compare ladder = switch, not if/else-if** (`lh` once + `beq $v1,$at`
  chain with constants in delay slots) — 43%→78% in one edit (TACO).
- **Explicit empty case labels restore a 0-based jr table** (`case 0: …
  case 10: break;` prevents the rebase `addiu -11; sltiu 6`) (TACO).
- **One surplus nop per switch arm = an unfilled load delay** — split the
  `AC(122)*20` expression so the constant store sinks into the lh delay slot.
- **Unconditional truncation above a guard** keeps `andi 0xff` as a real word;
  inside the `if` it folds into the `sb`.
- **`(u8)v` instead of a plain copy** survives uopt copy propagation; **dead
  read** (`b = q[1129];`) deletes the load but moves the base-address def to
  the block top.
- **Mixed spelling breaks mask CSE** — RE-refutation nuance: a batch-18 "refuted"
  note had probed both sites with the SAME spelling; the lever works when the
  spellings DIFFER. Check what a refutation actually probed.
- **volatile on a named extern pins load/store PLACEMENT without changing
  counts** (fixed a live-range-split move and an order pair).
- extract_jr_table.py returns only the FIRST table — read further tables from
  the EXN directly at addr-0x80132000.
- Missing-frame sweep: semis with NO PostProc at all compile at IDO's raw
  24-byte frame — `patch_stack_complete` alone is worth 5–12% (but check the
  compiled frame isn't LARGER than the target's before applying).
- EVENT multu cluster re-triage: 20 filed → 17 genuinely blocked; live lead:
  multiple volatile loads feeding separate chains defeat the CSE and make ugen
  cache the constant in a register — try before closing the 17.

## Batch 26 (2026-08-01) — the PSY-Q GCC library-region recipe (+10 byte-perfect)

The main binary's `@Compiler: gcc*` semis had all been sitting at 0–8% exact.
The cause was one pipeline bug and one missing directive, not codegen.

**1. The pipeline bug.** `tools/ido/fill_gcc_delay_slots.py` deletes the nops it
folds into delay slots and adjusts `.text` size, branches and relocations — but
never touched `.symtab`. Every FUNC symbol past the first fold was left at a
stale `st_value`/`st_size`. `strip_leading_instructions` reads `st_value` to find
the pre-function pad, so it saw a phantom pad and refused
(`Cannot strip 72 bytes from 52-byte function region`). Fixed by `adjust_symbols()`.

**2. The missing directive — the GCC types.h pad.** IDO emits 4 thunk words
before a body (`strip_leading_instructions --count 4`). PSY-Q GCC instead emits
the FULL inlined bodies of the `types.h` helpers: **18 words at 2.6.0/2.7.2, 19
at 2.8.1**. Almost no gcc semi had a strip directive, so every compare window
started 18 words inside the pad and read ~0%. Use
**`strip_leading_instructions --count auto`** (added this batch) — it strips
exactly the pad, whatever the compiler, so the count is never hand-mis-counted.

**3. The version lever is the big one.** The old files were nearly all
`gcc260 -O1 --fill-epilogue`. Almost every win came from moving up:
- **gcc272 / gcc281 fill the `jr $ra` epilogue delay slot themselves** — the
  `--fill-epilogue` flag that gcc260 needs then makes 2.7.2/2.8.1 one word
  SHORT. Headers claiming "35 with the flag, 37 without, no combo hits 36"
  (`func_8009AF38`) were measuring only 2.6.0; 2.8.1 hits it exactly with no flag.
- gcc281 emits `lui;addiu` for an array base where gcc260 folds the indexed load
  into `lw $2,SYM($4)` and loses a word (`func_800B5854`).
- Sweep `{260,272,281} x {none,--fill-epilogue,--fill-all,--fill-jalr,
  --no-cross-jumping} x {-O1,-O2}` before believing any "codegen gap".

**4. The residual is almost always ASPSX scheduling, not codegen.** maspsx does
not reproduce the real assembler's reorder pass. Recurring shapes, all fixed
with `swap_instructions` (same opcode, relocations travel with the word):
- ASPSX sinks `sw $ra,N($sp)` past the argument setup when it fills a jal delay slot;
- it emits callee-save/move pairs in a non-register order (s1 before s0; s2,s1,s0)
  and restores them in the opposite order to GCC;
- it reorders independent argument loads and the two halves of an accumulate.
`patch_insn_reg_field` covers the other class: ASPSX reuses the destination
register as the scratch for an indexed-load expansion where maspsx uses `$at`.

**5. "Cross-prologue thunk head" was never a thunk.** `magazine_main` /
`func_80086D5C` were filed as BLOCKERs "only closable with BANNED
prepend_thunk". The leading `lui $v1,%hi(D_800Dxxxx)` is the body's OWN first
global read hoisted above the prologue — the same cross-prologue rule as IDO.
Write the global read as the function's FIRST statement (`stat = D_800D0990;`
BEFORE the volatile counter store) and GCC emits it, in the right register.
Both windows now compile whole. Each such win ABSORBS the older headless symbol
(`func_80086D60`, `holiday_magazine`) — mark those `Status: ABSORBED`,
comment-only, so nobody retries them.

**6. Invented scratch locals are a bug, not modelling.** `func_800869A0` had
`s32 s0 = a0;` "for" the callee-saved copy. GCC allocates that itself; the extra
local is what made the branch test `$a0` instead of `$s0`. Deleting it took the
function from 7.7% to 13/13.

**Honesty findings.** `tools/ido/custom_func_8009F694.py` was BANNED PAINTING
(constructs instruction words from opcode fields and DELETES 20 instructions) —
its 83.9% was fake, the honest score is 25.3%; the custom-script audit (private archive)
had it as "PASS", a detector gap. And **`config/level3_functions.json` is NOT a
name-keyed map** — it is `{version, ..., functions: [...]}`, so
`test_single_match.py`'s `if func_name in funcs` never matches and every
main-binary window comes from the C header. Always re-check a new win's window
against the 983-entry `functions` LIST: it is how `func_80086F6C` was caught as
a duplicate of the already-registered `magazine_exit` and retracted.

## The 2026-08-06/07 Solo+Pad-Batch Sections (Hook 29, emission-order doctrine, 33 BP in two days)

### Hook 29 IDO52_AS1_KEEP_ANDI — the narrowing-truncation closer
The "(u8) truncation folds into the sb" class is as1 pass-II `func_416ca8`
(NOT uopt as filed — ugen emits the full andi+write-back; -O1 keeps it).
Two-step recipe: (1) GENERATE the andi — unconditional `u8 m = <wider web>;`
ABOVE the guard (in-if folds into the sb), reusing an EXISTING local when the
frame is at capacity (any new local can grow it 144->152-style); (2) KEEP it —
`IDO52_AS1_KEEP_ANDI=occ:N,N` per `IDO52_AS1_KEEP_ANDI_TRACE=1` (each source
andi appears TWICE; pass II loops to fixpoint, so if partial veto sets don't
hold — un-vetoed transforms trigger re-runs that re-eat at fresh indices —
use `=all`). First wins: ETC/func_8013FF34 (529/529), NAME_ENT/func_8014343C
W1. If =all keepers ever need windowing, extend the hook with a word-match
form (stable across re-runs), not occurrences.

### Emission-order doctrine (the store-vs-reload slide class)
as1's dependence DAG carries conservative pointer-store -> sp-load alias
edges: a reload that ugen emits after stores only becomes READY after them,
so scheduler hooks can NEVER hoist it soundly (trace-proven on
TACO/func_8013DAA0 — its filed "force-spill-reload hook" want is refuted).
Fix in C: `volatile <type> param` PARAMETER + one placed `local = param;`
read (volatile defeats copy-prop; the parameter form keeps the load
SP-DIRECT — `*(volatile s32*)&param` address-forms an addiu instead); the
local serves all pre-call consumers, post-call consumers re-read the param
(fresh home load = the target's own shape, and the local never spills).
"locals-first" batch loads (`x=s.a; y=s.b; z=s.c;` before dependent stores)
fix the same slide inside div/store chains. Store-before-call is the
faithful order whenever the target's sw sits in the jal delay slot.

### Pad-batch levers (TACO/RPG_BAT/TAIIKU/ETC rounds, 30 BP)
- Two-cluster sparse switch is ONE `switch` (IDO's own cluster split emits
  the `slti` test; do NOT add a (u32) cast to the operand, per-cluster
  bounds come out sltiu on their own). Sighting: dense low cases + a
  {80..86}/{100..103} high cluster.
- `x * 257 * 257` (0x10201 colour multiply): only explicit shifts match —
  `c = v + (v<<8); c = c + (c<<8);` — any multiply spelling folds to one
  strength-reduced 66049 chain. `(127-n)*257*257` distributes.
- NEGATIVE multiplier constant vs negated operand: `D * -168 / 1000` keeps
  the negation inside each multiply's strength reduction (per-site
  `subu $at,$zero,rX`, zero paints); `-D * 168 / 1000` CSEs the negu and
  drifts whole blocks.
- volatile works as a PAIR to pin store-then-load order across two globals;
  either alone can be inert. Then `IDO52_AS1_XBB_VOLATILE_OK=all` returns
  the legitimate fills the pin vetoes. `AS1_XBB_BUDGET=0` is too blunt.
- `IDO52_CONST_NOINREG_VALUES=1` can fix an as1 copy-vs-move delay-slot
  choice (altered $at liveness flips block-head fills from move to copy).
- SVECTOR-by-value clusters: constants materialize in SOURCE order and the
  first fills the preceding branch delay — statement order is the lever and
  is NOT paintable.
- `no-op if (k) { }` after a loop blocks $s0 counter promotion (free).
- ⛔ `IDO52_NO_SREG_PROMOTE=1` and `IDO52_SKIP_PASSES=opt_saved_regs` emit
  BROKEN objects (drop save/restore, keep $s0 live across calls). Never use.
- Frame layout is derivable: record sizes (a 20-byte rect, not 8) + locals
  laid out DESCENDING in declaration order — don't guess-then-patch.
- Misnamed externs hide in "lo16 still wrong" resolver lines (D_8015F2C0 was
  D_8015EB40); slot-coalescing conflicts (one our-slot = two target slots)
  = remap the consistent pairs + `patch_insn_imm16` the conflict words.
- .sym reports: ALWAYS `grep -i` BOTH hex cases (lowercase tail
  sections carry duplicate entries; two batches hit this).
- gen_reg_patches merges new entries into the FIRST paint line (mid-chain)
  while deriving from the FINAL object — relocate its additions to a fresh
  EOF line when swaps run between; keep any swap BEFORE the paint that
  expects its post-swap word.

## The 2026-08-08 Addendum

**⭐ SYMBOL-NAME AUDIT IS STEP 0 on any low-% semi.** Derive every extern's
address from the target's own lui/lo arithmetic BEFORE believing any
documented wall. Three "compiler walls" this campaign were downstream of
misnamed externs the masked test hid as reloc diffs: TACO/func_8013DAA0
(D_8015F2C0 → D_8015EB40), OPTION/func_8013297C (NINE names wrong — the
whole filed "spill-vs-promote 9-sreg wall" evaporated, byte-perfect 69/69),
TAIIKU batch (D_80128B57 → D_80128B77 + an lh mistyped lhu). Cross-check
against a main-binary function that walks the same arrays when one exists.

**Loop-shape steering pair** (OPTION/func_8013297C): explicit byte-offset
outer IVs (`for (off = 0; off != 128; off += 32)` with cursors rebuilt
INSIDE the loop) force per-iteration base rematerialization; spelling the
inner bound as the target's pointer compare (`&A[j] != &A[5]`) steers
loop-test replacement onto the base-relative IV.

**Whole-struct assignment > hand-rolled copy loop** (TAIIKU/func_80145534):
`*(BlkNN *)buf = D_template;` emits IDO's own block move AND derives the
loop bound from the base register it just built; a hand-rolled `ve = vs+4`
folds to a link-time constant and costs an absolute la per loop.

**volatile discipline**: blanket-volatile on externs is a systemic defect
(~100 words of false scheduling pins on one TAIIKU sibling). Use volatile
ONLY where the target's reload count demands it; if that volatile then pins
a %hi against as1's cross-BB boost, release exactly that pin with
`IDO52_AS1_XBB_VOLATILE_OK=all` (the volatile→XBB two-step). And
volatile-as-CSE-kill closes "permanent +1 word, load/store counts
unchanged" symptoms (ugen CSE temp blocking copy-prop — TAIIKU
func_8013C9D4, refuting its filed join-phi wall).

**Blind-derived swap sets MUST be --semcheck gated** (NAME_ENT
func_8014343C): order-preserving greedy assignment on same-opcode-dense
runs can cross def-use chains — one derived swap created a MIPS-I load-use
hazard that only semcheck caught. Also: drop imm16 fixups that land on
reloc-flagged words (%lo-diff traps); 17 of 72 were dropped there.

**Measured LDA-fold rule** (OPTION func_8013AF30/C018 wall, now
quantified): uopt keeps an LDA web in a saved reg for any BB with >=2 base
uses and rematerializes/folds where there is exactly one. If the target's
arm has one base use + absolute accesses, no zero-cost second use exists —
genuine wall, hook territory (generic def-placement class).

**⚠️ "N/N insns" from test_overlay_match is a WINDOWED count, not the
emission size** (TAIIKU/func_80138B84): a probe printed a clean 212/212
while the ELF symbol was 217 insns — the 5 surplus words fell outside the
window, same family as the truncated-compare trap. A count claim needs the
objdump -t / ELF symbol-size self-audit, not the harness line. Also from
TAIIKU-2: declaration order IS load-bearing for stack buffers (reverse
declaration order = descending address order, worth 23 points + a word on
func_80133CFC); block-move alignment upgrade (`struct { u16 h[5]; }`
assigned whole copies lw/lw/lhu — IDO upgrades the copy unit to the
operands' REAL alignment; `{s32 w[2]; u16 h;}` pads to 12 and emits 3 lw);
and the family wall: global `la` webs are NEVER queried through f_ldainreg
(sp-relative classes only) — no current hook reaches them (verified with
IDO52_LDA_NOINREG_DEBUG=1; blocking all 32 classes leaves the la + spill
verbatim). Wanted hook: a global-LDA class for LDA_NOINREG.

**⭐ Hook 28 `IDO52_EXCLUDE_REGS_WEB` is the FORBID primitive** (BUNKA_SD
func_801373D0 win): PRECOLOR_WEB merely relocates a copy; forbidding
colours lets the whole colouring re-derive around the constraint. Hook 28
post-dates most filed probe lists — old "wall" notes that only swept
PRECOLOR are STALE; re-probe with EXCLUDE before trusting them.
Sweep levers: `if (!x)` vs `if (x == 0)` picks whether a post-loop use
coalesces into the in-loop call-arg web (phantom loop-exit phi copy);
volatile on a loop BOUND kills unrolling when the target reloads it per
iteration; merging two loop counters into ONE variable makes uopt hoist
the init above a call with spill/reload AND converts slt+bnez to bne
(writing `i = 0;` explicitly does NOT); `== 1U` joins the const web and
fixes branch operand order for free.
Traps: a count-exact variant is only a win if the OPCODE-diff count drops
with it (two GYOZI variants hit the word count by whole-body rescheduling
— 83 opcode diffs); `IDO52_AS1_COPY_PROP_TRACE` lists each copy TWICE —
veto both occurrences or the copy drops anyway.

**Inverted fail-arm → taken-side shadow fill** (BUNKA_SD func_80137B48,
89→91/91 count-exact): when the target's early-out branches straight to
epilogue+4 with a duplicated `lw $ra` in the delay slot, write the guard so
the branch TARGET is the epilogue (`if (ok) { ... } return err;`), not the
fall-through (`if (!ok) return err;`). One word per site; honest cost: the
trailing return extends the error value's live range (a save/restore pair).
**Hook 28 fast negative screen**: EXCLUDE_REGS_WEB is web-indexed — if
`IDO52_DUMP_WEBS` shows very few webs, the copy was coalesced BEFORE
colouring and no web-indexed hook can reach it. Check the dump before
sweeping; "copy coalesced pre-colouring" in an old note is a GENUINE
refutation, not a stale one. (5-file re-probe sweep: 0 fell; the two that
DID fall — the 842C/3D54 sisters — were PRECOLOR-only filings.)
**The andi-fold and the andi write-back COPY are different walls** — Hook
29 KEEP_ANDI addresses only the fold (the andi surviving but its copy being
eaten is a copy-prop wall; AS1_NO_COPY_PROP occurrence lists are not
fixpoint-stable, only =all holds — a word-match veto form is the wanted
fix).

**Hook 28 negative screen, sharpened** (GEKO func_80134A40): the hook
fires on MORE bitposes than IDO52_DUMP_WEBS prints (13 vs 7 there — brute-
force the bitpos, don't trust the dump as the universe), but global-LDA
PROMOTES are decided BEFORE colouring, so they are out of reach of BOTH
the globalcolor forbid (Hook 28) and f_ldainreg (TAIIKU-2 finding) — one
wall, two refuted hook classes. `IDO52_UGEN_MAX_SREGS=7` is a count-exact
TRAP on this shape (word count lands, exact/mnemonic scores collapse).
**s8-vs-u8 const-web steering** (GEKO func_8013E8A8): a `u8` struct field
assigned a small literal builds its OWN constant node (surplus li + sb via
a temp); typing the field `s8` merges the literal into the s16 const web
(target's `sb $s2`), and a `(u8)` READ cast restores lbu with no extra
andi. Strictly better than CONST_INREG_VALUES=8 (which register-izes an
addend the target spells immediate). Corollary: one local PER RELOAD —
sharing one variable across three lh's of the same table fuses a web that
can't share $v0 and moves the frame. Wanted hook: as1 delay-fill
SOURCE-preference veto (fill from fall-through vs branch-target — the
mirror of NO_FALLTHRU_FILL; as1 makes both choices for identical shapes
within one function).

**The %2-macro lever cuts both ways** (KANGEI func_80136978 win vs OMIMAI):
IDO's canonical signed `v0 % 2` macro CONTAINS a template nop in its beq
delay slot — no delay-fill hook can touch it, so hand-written expansion vs
`% 2` spelling is decided by reading the target's delay slots: an unfolded
macro shape means the source spelled it by hand; a folded andi/beqz means
`% 2` (which collapses hand-expansion by dozens of words when the result
is only zero-tested). Related: CONST_INREG only pins a literal when the
carrying local matches the compare operands' UNSIGNEDNESS (u8→u32 retype
unpinned a 1-web; fix by retyping all three locals u32).
**LDA_NOINREG class 4 DOES reach a global la web when the address is
routed through a POINTER LOCAL** (IDO52_LDA_NOINREG_DEBUG=1 verified,
14+ blocked promotions) — refines the TAIIKU-2 "never queried" negative,
which holds only for direct global la.
**Symbol-audit false-positive caveat**: the auditor flags loaded-pointer
bases as "suspect externs" — confirm each pairing against the .s before
renaming; a blanket rename nearly corrupted OMIMAI case 6.

**Switch-ordering pair** (RPG_BAT func_80144490): IDO sorts the COMPARE
CHAIN by case value but emits BODIES in source order — so a
`case (s32)0x80000000` goes LAST in source with a SIGNED control
expression; this alone reproduces chains including per-branch speculative
delay-slot li constants. **Don't-care return, precise form** (func_80148714,
worth ~47 words): explicit `case N: break;` + NO trailing return —
`return v0;` on an uninitialised local homes v0 and grows the frame.
**A register-held divisor drags the full break-guard at every site**
(func_80147940): if `/100` compiles checked at 12 sites, CONST_NOINREG_
VALUES=100 was forcing a VARIABLE divide — release it, 47 phantom words.
Correction to 2026-07-24 note: `IDO52_UGEN_MAX_SREGS` now CLAMPS (not
inert) — but yields wrong allocation; still not a lever. Hook 24
CONST_DEF_BB negative screen: `IDO52_RELOAD_DUMP` printing nothing = no
range-head def exists to move — retire the probe without sweeping.

**⚠️ THE DELETED-ACCESS TRAP — a hook that hits the count is NOT a win
until `objdump -r` confirms every target global is still referenced the
target's number of times** (NAME_ENT func_8013425C `PRECOLOR_WEB=t0:38`
made uopt coalesce the page selector away — ZERO references left to
D_8014CCC8, dispatch branching on a never-loaded $t0; func_8013A0C8 pins
117/130/143/156 same class). Invisible to semcheck AND the masked test.
Corollary: **any hook sweep recorded on top of an unverified hook is
void** — both files' filed sweeps INVERTED when re-run from the unhooked
baseline (the old FORCE_LA_PULL=4 was worse than nothing). Also from this
round: inverted-fail-arm generalises to switch `break`s; explicit
per-element load temps stop induction-variable rotation in store-walker
loops; an `or`-vs-`nop` arg-passing delta is not automatically a wall
(targets mix `lbu $a0`+nop and `lbu $t`+`or $a0` at equal cost).

**Continuation-drain levers** (2026-08-08): pin the OPERAND not the result
(volatile on a DIVISOR beats volatile on the quotient for partially-dead
divide chains — uopt remats chains dead on the default arm); C89 promotion
makes `u8glob % 2` SIGNED (5-word macro) — `(u32)` cast per site for the
2-word andi fold, matched per site; a loop TEMP defeats redundant-store
elimination (DSE can't see through a copy — the target's duplicate store
survives); a store in a jal delay slot proves the store PRECEDES the call
in source; `u8 *q = p;` reproduces a parameter home (or $a2,$v1) that
otherwise stack-spills. DATE's filed base_register_cse wall REFUTED: raw
casts were the cause (named externs are the only spelling that emits $at
absolute stores); its probe list was void (measured on top of the casts).

**IV-vs-CSE steering** (RPG_BAT func_8014BFE4): a strength-reduced
induction variable is INVISIBLE to IDO's CSE table — which of two
equal-valued expressions is spelled as the IV decides whether a post-loop
recomputation is emitted or a spill stays live. Cheapest target-style
rematerialization forcer, no hook. Also: goto-spelled compare ladders
emit dead web-home copies if/else chains don't; hoisting a temp above a
branch can cost 40 words (web-home move steals the target's speculative
pre-branch slot); a ".sym hole" before a function = its own cross-prologue
head (func_80149C8C's "uninitialised registers at entry" blocker was a
window starting one BB late — @SymbolStart closed it 324/324 first
compile).

**NAME_ENT pad-drain levers** (5 BP): Hook 27 NO_FALLTHRU_FILL=all as an
ADD-a-word lever (converts fall-through fill to branch-target fill whose
DUPLICATE supplies a missing word); IV origin arithmetic identifies source
indexing (origin 6528=96*68 below the stores pins `E[96+i]`, not E[0..11]
nor for(i=96..)); hidden args read as otherwise-dead early constants;
(u32) casts for divu+3-word guard vs signed div+6-word overflow guard;
branch polarity identifies arm order (fall-through arm written first).
Wanted hooks, twice-hit each: as1 delay-fill SOURCE-preference veto
(no-branch-target-fill — mirror of Hook 27), and symbol/web-windowed
global-LDA demotion (LDA_NOINREG demotes all class-4 at once; 595
occurrence sub-windows never hit the single needed site).

**Volatile STAGING local = zero-cost partially-dead-divide anchor** (TACO
sister family, 1.4%→count-exact ×3): `volatile s32 qv; s32 q; qv = <div>;
q = qv;` — the store/reload land exactly in the target's div/mflo hazard
nops. Strictly better than pin-the-divisor (+5). Declaration order
load-bearing. Related: index-expression loops (`i*80+108`) beat pointer
cursors when the target unrolls (explicit cursors misplace the IV origin;
pointer-bound while loses the trip count); LDA_NOINREG=4 + explicit bound
local is a PAIR (either alone ≈ nothing); Hook 29 needs a literal `& 0xFF`
in source to have an andi to veto (check KEEP_ANDI_TRACE prints anything
before calling it inert).
**⚠️ TOOL BUG: gen_reg_patches.py DELETES the file's @CompileEnv line when
writing paints** — symptom: next run fails "field rt is 0, expected NN".
Re-add after every paint pass (fix queued in TODO).

**Large-function levers (RENSYU func_80132390, 0.9%→422/422)**: the
`*(u16 *)&u32_extern` ADDRESS-BUILD trap — a different-width read through
a cast builds a 3-word address the masked test hides as reloc diffs; fix =
a suffixed alias extern (`D_x_h`) at the same address. Loop-BOTTOM
re-assignment keeps a cross-loop value's live range out of the call span
(top-of-loop spelling forces a per-iteration spill). Dead-web REUSE pins
load order without new stack homes (fresh temps each reserve a slot).
gen_reg_patches append bug #2: a bare `patch_insn_reg_field` line with no
triples silently breaks appending.

**Suspect the RETURN TYPE before the register allocator** (EVENT
func_80153540, long-standing "colouring mirror" = a void return): the
cross-prologue head's `lui $vN` register is the cheapest tell of which
the original used (s32 emits lui $v0, void emits lui $v1). More drain
levers: KEEP_ANDI + AS1_NO_COPY_PROP are a PAIR when a mask's only
consumer is a truncating store; one-local-per-reload beats a paint for
commutative operand order; DSE is ADDRESS-based (distinct temps don't
preserve a duplicate store — the natural memory RMW spelling does);
order remap_stack_slots high-to-low so remaps never chain; transcribed
andi masks may be WIDTH-TRUNCATED in old files (0xEF for 0xFFEF).

**⭐ LDA-FOLD RULE CONCLUSION INVERTED (2026-08-08):
the zero-cost second base use EXISTS — the volatile STORE-SITE CAST.**
The ≥2-base-uses rule stands, but whenever the arm's "absolute" global
lies within ±32KB of the record base (check %lo arithmetic: the based
reloc form resolves to IDENTICAL bytes) and the target's store is the
`lui $at; sb` la-expansion, spell that store `*(volatile u8 *)&base[OFF]`:
under IDO a volatile LOAD stays based but a volatile BYTE STORE always
takes the $at la-expansion — simultaneously the target's exact 2-word
shape AND the second base use that flips the arm's other access back to
based form. All prior probes failed because volatile was applied to loads
or the whole pointer web; reads of the named globals stay named. Wins:
OPTION func_8013AF30 299/299, func_8013C018 238/238. Direct port
candidate: OPTION func_8013A80C wall (c).

**⚠️ objdump -d ELIDES repeated identical words as "..." — always pass -z
and confirm counts against the ELF symbol size** (TACO func_8013B9D8:
a script count read "count-exact" through hidden nops; truncation-trap
family member #4). Also from that close: volatile-defeats-const-prop is
the CHECKED-DIVIDE lever when LICM won't promote the divisor (cost = 1 sw
+ N lw, budget it against the target's own homing spend); copying a
volatile into a local can EVICT a global base (+2 lui — the lui-count
histogram vs target is the cheapest detector); Hook 16 CONST_INREG's own
reference case (EN_NICHI func_801343D4 =3) is BIT-INERT — the
held-constant claim is overstated for the divide-lowering path; wanted:
a ugen divide-lowering veto. And the honesty precedent: a count-exact
variant reached by OMITTING a target operation (compensating errors) must
be rejected and header-fenced, not delivered.

**⭐ THE DEAD-STORE COPY-PROP BLOCKER — the join-phi/arg-a0 wall's true
mechanism and cure** (RPG_BAT func_8015528C 375/375 + NAME_ENT
func_8013909C 321/321, the class's first falls): uopt's VALUE-BASED
copy-prop substitutes an assignment's RHS expression NODE into a later
use when no node operand is altered in between; a guard consuming the
NODE (not the variable) gives the node two consumers → its own register
item → the unpaintable `move a0,v0`. Signature: surplus move +
bgez-on-temp; itab tell via IDO52_UOPT_TRACE=3: `ules{node|0}` vs
`ules{var}` per arm. CURE at zero cost: a DEAD STORE to any operand of
the node between def and guard (`base = 0;`) — copy-prop runs BEFORE
dead-store removal, so it blocks the substitution then evaporates. A
value-preserving re-store does NOT work (the test is value-numbered).
Companion: variable ELISION (guard on the expression itself; respell the
other consumer from the same expression) removes the variable web
entirely. Likely reaches other filed join-phi walls.

**A80C close-out levers**: Hook 20 suppression can DISSOLVE a const web
(the bne conversion was the const's second use — vetoing it reverts
`li sN,K`+bne to `addiu $at,K`+beq for free); counter merging works
across TYPE boundaries via a value round-trip (byte-snapshot stored in
the pointer-typed cursor variable buys the callee-saved register); the
volatile store-site cast lever applies PER-ARM (x3 here — check every
arm's absolute stores against the base's ±32KB).

**⭐ THE "as1 delay-fill SOURCE-PREFERENCE veto" WANTED-HOOK IS REFUTED
(2026-08-08, 4-file campaign — do NOT re-file it).** Every specimen
decomposed into EXISTING levers; the "preference" is always which pass
gets to the nop slot first, and each pass has a reachable gate:
- **The branch-target dup (`insn` copied into the slot + branch
  retargeted +4, dead original left behind) is f_do_branch_opt's PLAIN
  path — it RUNS on mips1** (only the branch-LIKELY rewrite is fenced;
  old "NO_BRANCH_OPT inert" filings never checked the tail words).
  Its gates: slot still nop (bb flag +70 bit0), f_find_branch_target,
  target block emitted-word[1] == 0, f_eligible; at -O2 the defuse and
  cannot-trap checks are SKIPPED. `IDO52_AS1_BOPT_TRACE=1` lists
  candidates that passed the early gates.
- **Fill-from-fall-through vs fill-from-branch-target is decided
  upstream**: NAME_ENT/func_80144BC8 — the FTF (func_41d7b4) candidate
  fired but failed f_cannot_trap because the list scheduler led the
  fall-through block with a trappable lbu; Hook 7
  `NO_SCHED_LOAD_HOIST=strict:<occs>` (derive with the no-op list
  `strict:999` + trace) restores the lui-led block, FTF commits,
  branch_opt finds no slot. GEKO/func_80133C50 — the target's dup
  needed the slot KEPT nop: the volatile pin was blocking do_xbb_opt's
  boost of the arm-head lui into an EARLIER branch's slot;
  `XBB_VOLATILE_OK=<occ>` releases it, the arm then leads with a
  trappable lbu, FTF self-vetoes, branch_opt dups (byte-perfect).
- **"Fill taken from branch-target where target takes fall-through" IS
  an xbb pull** — `IDO52_AS1_NO_LA_PULL=<occ>` vetoes it and as1 offers
  the fall-through donor for the same destination on the next pair
  (GEKO/func_8013E8A8, byte-perfect). ⚠️ Vetoing occurrence N makes the
  fall-through candidate appear at N+1 — a naive consecutive list
  vetoes both sides.
- FTF gate chain for reference (func_41d7b4): -O global 0/3, bb flag
  bit13, prev-bb flag bit9, f_eligible, **f_cannot_trap (loads never
  donate)**, func_41d22c, prev-end==pc. FTF removes a word;
  branch_opt's dup is count-neutral — that asymmetry is why the wrong
  pass choice reads as ±1 word.
- Related but NOT this class: EVENT/func_8015765C's missing copy-fan is
  uopt value-based copy-prop divergence (3rd sighting, with
  NAME_ENT/func_8014343C W2) — as1 NO_COPY_PROP restores the WRONG
  copies (trace evidence in the file header; once-listed candidates are
  the deleted ones — pass II re-queues vetoed copies one index later).
  Wanted primitive there remains the uopt f_copypropagate site veto.
Bonus cross-check from the same campaign: gen_reg_patches can silently
coexist with a stale paint line — it STRIPS existing paint lines when
regenerating, so always re-run it rather than hand-merging.

**Hook 30 sweep addenda**: param-reload webs form at isearch1/searchstore
with NO formation query — FORM_SITE's key space excludes them (trace-
proven twice); the cure there is a SOURCE-POSITION COPY guarded by the
dead-store blocker (`xr = xa; xa = 0;` between the stores — the reload lw
lands at the target word, the dead store evaporates). Check OLD refutation
syntax before trusting it: func_80144490's "refuted" PRECOLOR sweep had
its arguments REVERSED (`0:1` for `v0:0`) — the corrected pin killed both
carrier moves outright. f_copypropagate site-keyed veto now has THREE
filed consumers (NAME_ENT W2, GEKO 34A40 cand-copy, TT D988 or-homes) —
next hook to build. New wanted key space: a ugen CVT-fold veto
(TT 80141090's s16-of-lh deletion — no const/lda leaf, no andi, outside
every current hook).

## The 343C Flagship Playbook (2026-08-08) — how the monster's blocker classes die

NAME_ENT/func_8014343C went 266/1108 (with three filed "unreachable"
walls) → BYTE-PERFECT 1111/1111 across the campaign. Its walls are
CLASSES — fight them with these recipes (full evidence in
the file's EOF notes; do not re-run its refuted-probe lists):

**1. Register-held ZERO at a merge point** (`move rX,$zero` the target
keeps, ours folds to $zero operands — both known instances now closed:
ENDING/func_80134280, 343C W2). Two proven recipes, try in order:
  a. ZERO-ARG CONJURING BRIDGE (ENDING): pass a literal 0 argument to an
     existing call (`func(0)`) — ugen emits or-from-$zero; then swaps +
     proof-carrying paints (reaching-def zero proof documented in-file).
  b. FOUR-PART COMPOSITION (343C W2, when the zero web spans stores):
     (1) `goto`-split the store block so it is CALL-FREE — the filed
     "cost model refuses $zero-web" is really "no register survives a
     call-spanning bb"; (2) Hook 30 FORM force at the store stmts + EMIT
     REGOP at the re-emission bb; (3) do NOT precolor-pin (pins orphan
     the reload record — DEF_BB/rebind ordering); (4) `extern volatile`
     on ONE of the target sbs so as1's zero-copy folder can't eat the
     ugen def. Then paint the register.

**2. Write-back / narrow-store webs where a sibling case matches and
yours doesn't** (343C W1): mirror the SIBLING's shape exactly —
break-restructure + an old-value local. ⚠️ COMPOSITION REQUIREMENT:
either half alone can be bit-inert; apply both before judging.

**3. Held small-const webs that global CIR breaks elsewhere** (the
128-web): use Hook 30 `FORM_SITE=const:VAL:stmt` per-site forces instead
of `CONST_INREG_VALUES` — the global form silently broke a DIFFERENT
case (caught only by full-stream diff, not the score).

**4. LDA CSE vs the target's remat of the SAME address** (W4): a
same-address ALIAS EXTERN (`D_x_b` suffix, same value, different name)
splits the CSE — each name gets its own materialization. (First proven
on RENSYU func_80132390 for different-width reads; works width-equal
too.)

**5. Grid/IV-bias scheduling residue** (W5/W6): mechanical once the
walls are down — pocket swaps + imm16 rebias (the -0x24 class) +
adjust_branch_offset for crossing branches; delay-fill follows FIRST-INSN
rotation, so loop-top statement order (`p` before `v`) picks the fill.

**META-LESSONS (highest value):**
- ⚠️ A LONG-STABLE "count-exact" can be COMPENSATING ERRORS: 343C sat at
  "941/1111 count-exact" for investigations while two surplus `li 128`s masked
  two genuinely missing words. The catch is the FULL-STREAM opcode diff
  (and the reference-count completeness proof), never the score.
- When walls fall on a body that carried a large fitted directive stack,
  REPLACE THE STACK WHOLESALE on the honest body (343C: 104 swaps/~570
  paints → 47/~470, all rederived, semcheck-gated every round). Fitted
  directives from the compensating-error era are actively wrong.
- Refutation shapes worth reusing: DEF_BB/DEF_EDGE cannot express
  MID-BLOCK materialization (block-entry only); Hook 25 needs avail
  records to exist; precolor pins ORPHAN reload records. Check the
  mechanism's key space before sweeping its knob.

**Queue-harvest round 1 findings** (4 BP: GEKO 33308 164/164, DATE2
330D4 237/237 — its 2026-07-31 unfilled-delay-slot filing was asking for
Hook 27, shipped since; TAIIKU 3296C 114/114 — the "wall" was a VOLATILE
ACCESS-ORDER BUG, RHS volatile evaluated before the LHS index: per-
statement index temps sequence the loads; DATE 35CC8 154/154 — `(x*3)|0`
pins the sll/subu/or *3 form against reassociation):
- ⭐ Branch-delay dup of the SKIPPED fall-through line = f_do_branch_opt
  dup — a SOURCE-STRUCTURE tell (restructure the C; likely present in
  other dispatcher-adjacent semis).
- The recurring winnable signature: `lui` in a branch-delay slot where
  the target has nop + the same lui in a multu/mflo shadow (3 of 4 wins;
  FTF-veto or swap, decided by whether the fill removes a word).
- TWO more poisoned whole-function normalizers found inflating masked %
  over broken objects (TAIIKU/DATE) — total now 3; treat any whole-
  function scheduler_normalizer on a semi as suspect until re-derived.
- Screen precision (fed back): DELAY-FILL 1/3 real (exclude nop-vs-move
  diffs whose source reg is live across the branch); LDA-FOLD sb-near-
  base finds RIPE functions but not the named lever (0/4 store-site-cast,
  3/4 crackable by other levers); wanted primitive for the EVENT residual:
  IDO52_AS1_XBB_COPY_PROP.
**Hook 32 BUILT — and the f_copypropagate ATTRIBUTION IS REFUTED for the
cross-bb class (2026-08-08, `apply_uopt_copyprop_hooks.py`, full writeup
UOPT-HOOKS-REFERENCE §Hook 32).** `IDO52_COPYPROP_TRACE=1|2` +
`IDO52_COPYPROP_SITE=stmt|lod:q[,...]`, `*`-diagnostic, env-unset
bit-identical (12-object A/B + 12-function 7-overlay byte-perfect
regression). The build's decisive traces:
- **GEKO 34A40's `cand = best` fold does NOT happen in f_copypropagate**
  — record-veto AND whole-walk `*`-deny are bit-identical; the copy
  reaches makelivranges intact but flag-dead (+29/+31=0) and cand never
  gets a web (`>>>active<<<` dump oracle). The dead-store cure's real
  lever is the build-time `*killprev` flag clear. Cross-bb copy folds
  (uses after a call) are OUT OF REACH of both hook families — do NOT
  re-file the same veto; the open thread is the +29/+31 consumer between
  REMOVAL-OF-REDUNDANT-STORES and reg-alloc-prep.
- **The same-bb collapse mechanism is the bb-local isearch LOD gate**
  (f_readnxtinst L43e8f0, value tree at isvar+52) — that one IS now
  site-vetoable (`lod:q`). TT D988: gate live, 39 sites swept, the
  or-home deficit reachable but remat-coupled (q13/q15 active; `lod:*`
  hits exact 290 window count). NAME_ENT W2: gate live (85 sites,
  const-0 loads sharing one value item are the signature); owner must
  re-derive q ordinals on the current shape.

## §2026-08-09 Overnight batches (12 batches, 22+ byte-perfect, systemic paint audit)

Rules learned across the overnight parallel batches.

### Paint soundness (the night's systemic finding)
- `patch_insn_reg_field` layers on SEMIS were systemically unsound (7/7 first
  sample): renames not covering live ranges, split %hi/%lo def-use pairs,
  stores through renamed-away registers, operands re-pointed at $sp/$zero/$at.
  All invisible to semcheck AND the masked/resolved compare (wrong-producer
  values still byte-match). Detector: reaching-definition isomorphism
  (pre-paint object vs final; producer sets must be unchanged).
- PROVABLE-defect classes (strip on sight): HILO_PAIR_BROKEN, ORPHANED_USE,
  NO_DEF_ANYWHERE, MISALIGNED_SITE, PAINT_ON_NOP, OPCODE_CHANGED,
  REGIMM_RT_FLIP, any paint onto $at.
- FP modes of the raw iso signal (adjudicate, don't auto-strip): callee-save
  ENTRY SPILL, VALUE-EQUIVALENT re-routing (copy chains/remat consts/same-symbol
  reloads), and UPSTREAM REPAIR (paints that fix value flow the
  scheduler_normalizer itself broke — keep those).
- rt-field-on-branch ban applies ONLY to REGIMM (op 1) and blez/bgtz (op 6/7);
  beq/bne rt is an ordinary operand paint.
- Adjudicate by RENAME GROUP, not per-offset: a 4-register rotation is only
  coherent as the whole cycle. Companion entries that make a partial rename
  whole-live-range coherent are sound completions (SHUGAKU 801337FC 0x84).
- An added paint must never turn a matching word into a residual.
- Paint only count-exact bodies. 97 iso-clean paints were DISCARDED on a
  2-word-short body (TEL 8013CF5C) — correct call.

### Count/window integrity
- Read the ELF SYMBOL size, never the .text section size (sections run long by
  alignment; 3 false "count-exact" headers caught: zero-pad short compile,
  @SymbolSize-sliced 10-word overrun, padding-compared 2-word deficit).
- Raw self-audit baseline keeps ALLOWED count-changing steps (cse_lui_at is
  removal-only); cse_lui_at offsets are CompileEnv-COUPLED (silently no-op if
  the env changes layout). PostProc offsets are .text-relative (+0x10 thunk
  bias) — a body-relative layer dies on the prologue (VALEN 80133F80).
- A count deficit is the ROOT CAUSE of scheduler_normalizer damage (illegal
  delay slots, dragged jr/ra): fix the count first; --fence/--freeze flags only
  relocate the damage. Replacement recipe (DATE 8013BE5C, 8013F1B0): delete the
  normalizer, rebuild only load-bearing effects with adjacent swap_instructions
  chains (+ adjust_branch_offset); swaps carry relocs, fixing wrong-symbol
  permutations no paint can express.

### Misname/fabrication sweeps
- New misname patterns: decimal-read-as-hex (14528 != 0x3AC0), +0x1000/+0x100
  nibble slips, hand-arithmetic %lo slips. Classes are PER-FILE (same name
  genuine in one sibling, misname in another) — never family-delta extrapolate.
  Screen site-pairing can be TRANSPOSED — resolve by use-census. "Phantom"
  globals may live in an absorbed cross-prologue head .s (body-only census
  misses them). Decode jal targets directly from the low 26 bits — no %hi/%lo
  ambiguity.
- Load-bearing pad_trailing_nop = fabrication by addition; six exposed tonight
  (counts 26..148), five rebuilt honestly to byte-perfect.

### Levers proven this investigation (beyond the 2026-08-08 addendum)
- Missing sll/sra/move triple before a jal = missing NARROW PARAM TYPE on the
  callee extern (declare extern void f(s16);) — 3 byte-perfect closes.
- Guard-load cast `(u32)load == K U` joins the widening to the compare web;
  phi coalesces; kills/creates delay-slot move copies (2 BP closes).
- Volatile DECLARATION (extern volatile u8) is the double-read cure; the
  *(volatile u8*)& CAST forces the 3-word la-expansion instead (2026-08-02
  note corrected).
- CONST_INREG occurrence-window discovery by SINGLE-QUERY ELF-SIZE SWEEP:
  trace the value's queries, force each alone, keep the one that lands the
  target size (ENDING 80137758: only 18 of 40).
- Hook 32 lod denials must cover EVERY use of a substitution (one-query denial
  is a no-op); a typed local's width (u8 vs u32) steers which register the
  value parks in.
- Hook 30 EMIT_SITE alone can fix const-web splits where FORM_SITE is inert
  (ENDING 8013858C — first emission-side-only win).
- Element-struct indexing (buf[i].a not buf[i*2]) frees as1 to hoist consts
  into interlock slots; source ORDER of stores around div/call sites controls
  delay-slot folding; joint for-init (for (col=0,gy=-25;...)) fixes preheader
  emission order; guard scope re-derivation from branch encodings dissolved
  two filed walls (continue-vs-break; jal inside the loop).

### Tool bugs found (fix queue)
- ~~Hook 28 IDO52_EXCLUDE_REGS_WEB register-name parser is OFF BY THREE (spec t3
  forbids $t0)~~ **FIXED 2026-08-09** (`apply_uopt_hooks.py`: the `lr+40`
  forbidden vector is indexed by the rc colour itself, not by rc-3; only the
  printregs listing uses rc-3). Round-trip check:
  `tools/ido-static-recomp/test_hook28_regspec.py --live`. ⚠️ Every
  `@CompileEnv: IDO52_EXCLUDE_REGS_WEB=` authored before that date now means a
  different register set — re-derive, do not trust.
- ~~gen_reg_patches: splits hoisted %hi/%lo def-use chains; emits $at/$sp/$zero
  operand re-pointing; bulk layers drift into target-transcription. Wanted:
  build the provable-class refusal in.~~ **BUILT 2026-08-09** — emit-time
  refusals (compiled nop, REGIMM/blez/bgtz rt, zero<->reg on a branch, $at,
  reaching-def break, %hi/%lo symbol re-pairing), no `--force`. Standalone
  audits: `tools/overlay/audit_paint_{bans,isomorphism}.py`,
  `minimize_paint_strip.py`, `audit_raw_count.py` (their docstrings give the
  false-positive modes — the raw iso signal is ADVISORY).
- swap_registers is ONE-WAY by default (collapses live pairs) — --bidirectional
  for exchanges; two shipped collapses repaired.
- swap+repaint across paired same-shift srl/sll loads can silently swap shift
  semantics while scoring closer — re-read semantics after.

### §2026-08-09 late addendum — the wrong-symbol paint gap
`gen_reg_patches` run on a MIS-ORDERED object silently corrupts %hi/%lo
SYMBOL pairing: it re-points a %lo consumer at a different lui of the same
shape, so stores/call args aim at the WRONG GLOBAL while semcheck (register
dataflow) AND the iso producer-graph check (instructions, not reloc symbols)
both pass. Found on DATE/func_80134CF4 (6 wrong-symbol pairs in a 28-entry
iso-clean layer — removed entirely). ✅ **BUILT 2026-08-09**: the
reloc-SYMBOL pairing audit (every %lo's dominating %hi must carry the same
symbol before and after the layer) is now an EMIT-TIME REFUSAL in
`gen_reg_patches.py` — a different `lui` holding the SAME symbol is fine,
a different symbol (or losing the dominating %hi) is refused. Re-verified on
the historical DATE/func_80134CF4 layer: all 81 entries refused, with the
crossings named (`0x2c0: Db_8012B1E8 -> D_800EF6BF`). Zero false positives on
three byte-perfect controls carrying 6 / 617 / 804-entry layers. Standing
advice unchanged: never paint an object whose block alignment drifts.

## The GCC delay-slot fill-mode sweep (2026-09-17) — measured, per flag

`tools/ido/fill_gcc_delay_slots.py` post-fills delay slots that Psy-Q GCC left
empty. Its modes had never been crossed systematically. All 24 combinations of
`--fill-prev` / `--fill-earliest` / `--fill-load-delay` / `--fill-epilogue` /
`--fill-jal-store` / `--fill-jalr` / `--fill-j` / `--fill-branch-store` /
`--fill-all` were run over the **29 open Psy-Q GCC main-binary semis**, 696
compiles, every result judged on **`Body == target`** rather than on the
`Exact:` line.

### Judge on the length, not the score

`Exact: X/N` uses `N = min(target, compiled)`, so a flag that SHORTENS an
over-long body raises the score while the surplus words are never compared at
all. `func_800899C8` reads 58/290 at 297 words and 54/290 at 290 words: the
54 is the better number. Five of the seven functions the sweep moved were
moved by reaching count-exactness, not by raising `Exact`.

### Which flag to reach for

| flag | load-bearing at the optimum | never in any optimum | improves / regresses vs no-fill |
|---|---:|---:|---|
| `--fill-load-delay` | **7** | 3 | 15 / 1 |
| `--fill-earliest` | 4 | 3 | 18 / 8 |
| `--fill-prev` | 2 | 6 | 13 / 8 |
| `--fill-epilogue` | **0** | 4 | 17 / 8 |
| `--fill-branch-store` | 0 | 12 | 6 / 0 |
| `--fill-j` | 0 | 5 | 2 / 0 |
| `--fill-jal-store` | 0 | 10 | 1 / 0 |
| `--fill-jalr` | 0 | 5 | **0 / 0 — inert on every file** |

(«load-bearing» = present in EVERY combination that reaches this function's
best count-exact score, over the 15 functions that have one.)

- ⭐ **`--fill-load-delay` is the flag to try first**, and it was badly
  under-used: it is load-bearing on 7 of the 15 and the only one whose
  improve:regress ratio is not roughly 2:1. Its wins are large because an
  unfilled load-delay nop is a SURPLUS WORD: `func_8009D388` shed eight of
  them (167 → 159 words, 16 → 44 exact) on this flag alone.
- **`--fill-prev` does NOT deserve to be a default anywhere.** It has the
  worst ratio of the four live flags and is load-bearing on only two files
  (`SD_SetVabAdrs`, `func_80099670`). Its one famous win — `func_8008A004`
  23/125 → 66/125 — is beaten on the same function by
  `--fill-load-delay --fill-earliest` at 84/125 with `Fixup 0`.
- ⛔ **`--fill-epilogue` is a net liability on this corpus**: load-bearing on
  ZERO files, and on three it was costing exactly the one word the body was
  short, because *retail leaves those epilogues unfilled*. Dropping it made
  `SD_SECallInSEQ`, `func_8009B170` and `func_8009D388` count-exact. Before
  adding it, check whether the target's `jr ra` actually carries the stack
  restore. (It also has a real defect — see below.)
- `--fill-jalr` changed **nothing on any of the 29 files**; `--fill-j`,
  `--fill-jal-store` and `--fill-branch-store` are nearly as inert. They never
  hurt, but citing one as a refutation lever tests nothing.
- `--fill-all` is indistinguishable from `--fill-epilogue` on this corpus
  (identical 17/8/4 split), because the other two modes it enables are inert.

### The sweep cannot fix wrong C

**14 of the 29 have no count-exact combination at any setting** — their body
length is wrong by 4–26 words and the fill mode moves it by at most ±5. A fill
flag closes a gap of one to eight words made of unfilled nops; anything
larger is missing or surplus C.

### Two live defects in the tool (do not trust around them)

- **`--fill-epilogue` maps the epilogue LABEL to the `addiu $sp` restore, one
  word PAST the `jr $ra`**, so every branch into the epilogue lands on the
  stack restore and falls through into the next function. 42 shipped files use
  the flag and at least one compensates with an `adjust_branch_offset`. The
  proper fix is a label-first override on the epilogue pattern; it needs one
  tree-wide pass, so compensate per file until then.
- **`insn_rw()` returns `None` for `mult`/`mflo`**, so `_fill_load_delay_nops`
  aborts its scan at the first `mult` and `--fill-load-delay` is powerless
  past it. (`lwl`/`lwr`/`swl`/`swr` were taught to it on 2026-09-17;
  `mult`/`mflo` were not.) A file with an early `mult` may look immune to the
  best flag in the set.

### What the sweep moved

| function | before | after | flags |
|---|---|---|---|
| `func_8009D388` | 16/159, body 167 | 44/159, body 159 | `load-delay earliest` |
| `func_8008A004` | 68/125, Fixup 1 | 84/125, Fixup 0 | `load-delay earliest` |
| `func_800875C0` | 19/124, body 128 | 39/124, body 124 | `load-delay` |
| `SD_SetVabAdrs` | 42/201, body 204 | 52/201, body 201 | `prev earliest` |
| `func_8009B170` | 32/145, body 145 | 33/146, body 146 | `prev` |
| `SD_SECallInSEQ` | 16/109, body 108 | 17/109, body 109 | *(none — epilogue dropped)* |
| `func_800899C8` | 58/290, body 297 | 54/290, body 290 | `load-delay earliest` |

`func_8008A004` then went 84 → **90/125** on two source levers, both about
*when* a value is read: the three table indices of its diagnostic call routed
through locals (inline, gcc evaluates the argument list strictly left to right
and the pointer argument's read lands first; retail defers it fifteen words),
and `src = D_800EB710` hoisted out of `if (result)`.

### The flag axis saturates fast

On the three best-placed files (`func_800B3CB0` 80/109, `func_800B3E64`
50/75, `func_8008A004` 90/125) the score is identical across every maspsx axis
(`--macro-lui-fill`, `--macro-lui-fill-far`, `--load-dest-temp`,
`--label-load-nop`, `--load-base-nop`, `--delay-slot-load-nop`,
`--no-cross-jumping`), `-O3`, `-fno-rerun-cse-after-loop`,
`-fno-cse-follow-jumps`, `-fno-strength-reduce`,
`-fno-expensive-optimizations`, `-fsigned-char`, `-mhard-float` and
`-fno-force-mem`. Once two fill modes are right, **stop sweeping flags and
read the diff** — what is left is source shape and gcc allocation order.

## `--fill-ptr-bump-down` — ASPSX sinks an EARLIER pointer bump (2026-09-25)

A gcc-path assembler-fidelity flag in `tools/ido/fill_gcc_delay_slots.py`
(pattern 10), routed through `gccwrap.py` / `build_level3_c.py` like the other
`--fill-*` modes. **Off by default, never implied by `--fill-all`**; set it per
file in `@CompileFlags`. It is the inverse of the IDO path's
`--ido-fill-ptr-bump` (which HOISTS a later bump; the `ido52-gas` pipeline,
`tools/maspsx`).

**The rule.** ASPSX fills a load-delay `nop` with an EARLIER `addiu rX,rX,K`
of the same basic block, moving it down past the loads/stores between that
use `rX` as their base and rewriting each of their displacements by `+K` (the
memory word now executes before the bump, so it adds the K it no longer sees
in `rX`). Every earlier bump of the same register that can sink with it goes
too, in order. The nop is consumed: -1 word per move.

```
cc1/maspsx:  sb v1,0(s1) ; addiu s0,s0,1 ; lb v0,0(s0) ; lbu v1,0(s0) ; nop ; bne v0,a1
retail:      sb v1,0(s1) ; lb v0,1(s0)   ; lbu v1,1(s0) ; addiu s0,s0,1     ; bne v0,a1
```

**Legality (all required, else the site is left alone):**
- the nop directly follows a load, and that load is not itself a delay-slot
  word; the move never crosses a branch target, a symbol start, a control
  word, a delay slot or a `nop` (the block boundary rule of
  `--fill-load-delay`);
- every hopped word is an aligned load/store based on `rX` whose data
  register is not `rX` and which carries no relocation (displacement
  rewritten), another bump of `rX` (joins the chain), or a modelled
  ALU/memory word that neither reads nor writes `rX`;
- `rX` is not `$zero`/`$at`/`$sp`/`$ra`; a bump with a relocation never moves;
- every rewritten displacement stays within signed 16 bits;
- where the words LEAVE: the rewritten span must not contain a load-use
  adjacency that was not already there (simulated, per the
  `func_800899C8` lesson in `_hazard_free_move`).

`--fill-trace` (or `FILL_TRACE=1`) prints one line per move to stderr, e.g.
`fill-ptr-bump-down: 2 x addiu $3 (sum +8) from word 105 -> load-delay nop at
word 112 (load word 111); 3 displacement(s) rewritten`.

**Retail evidence** (library region):
- `func_80088F90` 0x80089048–0x80089050: `lb v0,1(s0) ; lbu v1,1(s0) ;
  addiu s0,s0,1 ; bne` — the exact single-bump form.
- `func_80088C74` 0x80088DC8–0x80088E0C: stores at `0/4/8(v1)`,
  `lw v0,8(v1)`, both `addiu v1,v1,4` below them (one before
  `sw v0,16(sp)`, one in the `jr ra` slot) — the two-bump chain.
- 0x80086FBC–0x80086FC0 (inside `func_80086F6C`) and 0x80087248–0x8008724C
  (`func_80087220`, no C yet): `lhu v0,2(a0) ; addiu a0,a0,2 ; sh v0,2(a1)`
  — the bump in the load delay with the hopped load at `+2`. These two also
  show a SECOND register's bump (`addiu a1/s0,2`) sunk count-neutrally past
  its store; the flag does not model that non-filling sink.
A strict retail scan (bump immediately after an `rX`-based load whose
displacement is `>= K`) finds exactly these three library sites
(0x80086FC0, 0x8008724C, 0x80089050).

**Census (forced on every gcc file, 2026-09-25):** of the 158 registered
byte-perfect `@Compiler: gcc*` functions, **0 change** — there is no
never-blindly-apply list yet. Of the 20 open gcc semis, exactly two change,
both count-exact afterwards:

| function | before | after |
|---|---|---|
| `func_80088F90` | Body 182, 32/181, Fixup 2 | Body 181, 156/181, Fixup 1 (then BYTE-PERFECT with `--label-load-nop` + an s4/s5 rename + 18 swaps) |
| `func_80088C74` | Body 104, 28/103, Fixup 0 | Body 103, 32/103, Fixup 0 |

The other 18 are unchanged at every metric.

## `--reuse-at-same-hi` — ASPSX reuses a live `$at` for the same symbol (2026-09-25)

A gcc-path assembler-fidelity flag in **maspsx**
(`tools/maspsx/maspsx/__init__.py`, `_reuse_at_store` /
`_reuse_at_preserved`), routed through `gccwrap.py` / `build_level3_c.py` /
`ci_compile_check.py` like `--fill-ptr-bump-down`. **Off by default, not in
`--fill-all`**; set it per file in `@CompileFlags`.

**The rule.** ASPSX expands a symbol store `s{b,h,w} rt,SYM+N` as
`lui $at,%hi(SYM+N) ; s.. rt,%lo(SYM+N)($at)`. A LATER store in the same basic
block to the **same symbol** (`SYM+M`) is emitted as the bare
`s.. rt2,%lo(SYM+M)($at)` — the live `lui` is reused. Once the second store
is one word it is an ordinary delay-slot candidate, so `--fill-prev` pulls it
into a following `jal` slot exactly as retail does.

```
cc1:     sw $5,D_800E2DD4+4 ; li.. ; sw $2,D_800E2DD4 ; sw $3,D_800E2DF4 ; sw $4,D_800E2DF4+4 ; jal
maspsx:  lui at ; sw a1,4(at) ; sw v0,0(at) ; lui at ; sw v1,0(at) ; sw a0,4(at) ; jal ; nop
retail:  lui at ; sw a1,DD8(at) ; li v0,16 ; sw v0,DD4(at) ; ... ; lui at ; sw v1,DF4(at) ; jal ; sw a0,DF8(at)
```

**Conditions (all required; otherwise the store expands normally):**
- same symbol NAME as the store that set `$at`, and the address derived from
  the name (`..._XXXXXXXX`) gives the **same `%hi`** for both displacements
  (so the shared `%hi` is exact and the new `%lo` is in range); no derivable
  address -> never armed;
- the symbol lies in the retail EXE's **initialised image**
  (0x80041000–0x800E9000, header `t_size` 0xA8000). A **bss** symbol never
  arms: see the census below;
- nothing between writes `$at` or ends the block. The window is a
  whitelist: comments / `.loc` / `.stab*`, `nop`, register-form ALU with an
  in-range immediate, `li`, `la rd,SYM`, symbol loads `lX rd,SYM` (GNU as
  expands them through `rd`), and plain `N($reg)` loads/stores. A label, any
  other directive (`.set noreorder` wraps gcc's filled branches), a branch,
  jump or call, a `;` macro line, any word that names `$at` or expands
  through it (another symbol store, `SYM($reg)`, a div macro, ...) ends it;
- stores only. Loads are never reused: ASPSX expands a symbol load through its
  own destination (retail `cd_init_retry` loads `lw v0,DD8(v0)` inside a live
  `$at` window of the same block).

`MASPSX_TRACE=1` prints one line per reuse (gccwrap passes maspsx's stderr
through when it is set); `MASPSX_TRACE_FILE=path` appends them to a file.
The `.s` also carries a `# DEBUG: reuse-at-same-hi` comment on each site.

**Retail evidence.** Walking every known-gcc window (161 registered + 15 open
gcc semis) for `$at` stores: exactly **3 reuses**, all in the sound-driver
initialised block at 0x800E2Dxx — `addr_init_club_sd` 0x8008ADD8 (lui at
0x8008ADCC, DD8 -> DD4) and 0x8008ADF0 (lui at 0x8008ADE4, DF4 -> DF8, in the
`jal` slot), `cd_init_retry` 0x8008AA0C (lui at 0x8008A9E8, DE0 -> DEC, in the
`jal` slot). Against 213 near-address re-`lui` pairs, 200 are bss; the 13
in-image ones are either across a call/branch (0x8008AB70, 0x800B3E20,
0x8008A310, 0x8008AA38) or between neighbouring scalars that are separate
symbols in the original (0x800E2DC8/DCC/DD0 in `SD_Reset`,
`func_800899C8`, `func_8008A1F8`; 0x800E2B00/04/08; 0x800E8CCC/CD0), which
the name test leaves alone as long as the C declares them separately. So the
C must spell a shared-`$at` block as ONE struct (`Pair { s32 a, b; }
D_800E2DD4, D_800E2DF4` in `addr_init_club_sd`), and keep separate scalars
separate.

**Why the bss exclusion.** Without it, forcing the flag on every gcc file
breaks three registered byte-perfect functions whose C stores to one bss
symbol at several offsets, each re-`lui`'d in retail: `func_80099040`
(D_8012DCD0, 12 sites, Body 156 -> 144), `func_8009F694` (D_80131DA0, 87 ->
84), `func_8009EBB8` (D_80131CB0, 249 -> 247). This mirrors as1, which shares
`lui $at` only for initialised definitions.

**Census (forced on every gcc file, bss exclusion in, 2026-09-25):** of the
161 registered byte-perfect `@Compiler: gcc*` functions **0 change** (no
never-blindly-apply list); the one registered `ido52-aspsx` file is also
unchanged. Of the 15 open gcc semis only `addr_init_club_sd` fires (2 sites):

| step | Body | Exact | Fixup |
|---|---|---|---|
| E63 file (no flag) | 53 | 25/51 | 0 |
| + Pair structs + `--reuse-at-same-hi` | 51 | 39/51 | 0 |
| + `u = D_80130B2C;` before the DF4 store (registers = retail) | 51 | 41/51 | 0 |
| + 9 `swap_instructions` (within-block schedule, E44 bridge) | 51 | **51/51** | none |

`cd_init_retry` (the third site) fires too once `D_800E2DE0`/`D_800E2DEC` are
spelled as one struct (scratch test, not applied): Body 134 -> 133, removing
exactly the surplus `lui` its header names; its remaining word is the
separate label-load `nop`.

## Three ASPSX division / load-delay rules for GsSortBg (2026-09-26, E115)

Three gcc-path assembler-fidelity flags, each **off by default, not in
`--fill-all`**, set per file in `@CompileFlags` and routed through
`gccwrap.py` / `build_level3_c.py` / `ci_compile_check.py` like
`--fill-ptr-bump-down`. Run E107 proved all three in scratch copies of the
tools; E115 put them in the real ones. They only move or pick existing
compiled words (or pick the order in which maspsx emits a macro's own
words); none inserts, deletes or rewrites an instruction word. Together they
closed `func_80099CF0` (GsSortBg, 525 words): count-exact and multiset-exact
at 424/525, then 77 `swap_instructions` for pure reorderings, BYTE-PERFECT.
Ablation on that file: without (B) 523 words, without (C/D) 527, without
(E) 524.

**(B) `--defer-div-check-first-on-slot-read`** (maspsx,
`defer_div_macros(check_first_on_slot_read)` / `_dd_noreorder_slot_reads`;
gccwrap also passes it to the ido52-gas `defer_div_macros` call). With
`--defer-div`, a compiler `.set noreorder` branch flushes a pending div/rem.
maspsx used to emit the mfhi/mflo first there, always. Retail GsSortBg has
both orders in one function. At 0x80099F44 it is `<check> ; mfhi t6 ; nop ;
nop ; bgez t6 ; move s6,t6`: the slot reads t6, so the check comes first.
At 0x8009A26C it is `mfhi v0 ; <check> ; bnez v0 ; andi v0,s2,..`: the slot
writes v0, so the mf comes first (the default). The flag checks the delay-slot
word, which is the second instruction after the directive. If that word reads
the mf destination, the check is emitted first; otherwise the default order
stands.

**(C/D) `--fill-load-delay-macro`**
(`fill_gcc_delay_slots.fill_load_delay_macro`, run by gccwrap on the cc1
text BEFORE maspsx; standalone `fill_gcc_delay_slots.py --asm-macro-fill
in.s out.s`). ASPSX decides load-delay fills at macro level, where a div/rem
is ONE word. So a `#nop` after a load (reorder mode) takes the NEAREST legal
later word of the block. That word may be an ALU word or another load.
Rules:

- The search skips other `#nop`s. It stops at a label, a directive, a branch
  or jump, or an independent symbol-load macro (maspsx's own lui fill takes
  that slot, 0x80099EB8).
- It may cross div/rem/mult macros and stores, but never picks one of them.
- The candidate must not touch the loaded register, and must not conflict
  (RAW/WAR/WAW, HI/LO included) with any word it crosses.
- A load may cross a store only when both are `$sp`-relative at different
  offsets.
- It refuses a move that would leave a load directly followed by a reader of
  that load's result (no new hazard where the word leaves).

Retail: 0x8009A040 (cc1 `lw $15,112 ; #nop ; rem ; div ; lw $15,120 ; rem ;
lw $15,152 ; div ; mult ; lw $5,20($21)` -> `lw t7,112 ; lw a1,20(s5) ;
div ...`) and 0x80099EEC (`lh t7,8 ; lhu v1,136(sp) ; sw t7,104(sp)`).
Negative controls that keep their nop: 0x80099F64, 0x80099F08, 0x80099F18.
`FILL_TRACE=1` prints one line per fill.

**(E) `--fill-load-delay-no-div`** (fill_gcc_delay_slots `NO_DIV_FILL` /
`_is_div_family`). Applies with `--fill-load-delay` (and `--fill-earliest`'s
unfillable-nop test). A `div`/`divu` word never fills a load-delay nop.
Neither does an `mfhi`/`mflo` whose HI/LO producer (the nearest earlier
mult/div/mthi/mtlo) is a div. Retail 0x80099F68 keeps `div ; lh t7,6(s5) ;
nop ; sw`. **An mflo reading a MULT stays a legal filler.** The first draft
excluded every mfhi/mflo, and the census caught it: registered
`func_8009E1F0` has retail `mult v0,a1 ; lh v0,6(t0) ; mflo a0` (three
sites), and that draft added six words to it. The mult carve-out is why that
file is now unchanged.

**Census (each flag forced on every registered gcc/ido52-gas/ido52-aspsx
file, 188 files, .text compared, 2026-09-26).** With all three flags unset,
`build_level3_c.py --no-cache` gives the retail SHA1, 1318 matched and
0 mismatched.

| flag | registered files changed | open gcc semis changed (5) |
|---|---|---|
| `--defer-div-check-first-on-slot-read` | **0** (8 use `--defer-div`) | 0 |
| `--fill-load-delay-no-div` (mult carve-out) | **0** | 0 |
| `--fill-load-delay-macro` | **21**: every one breaks | 0 |

⚠️ **`--fill-load-delay-macro` is NOT universal ASPSX behaviour. Never apply
it blindly.** Forcing it breaks 21 registered byte-perfect files. Nineteen
lose 1-5 words because retail keeps the nop it would fill:
`SpuMallocWithStartAddr`, `SsSeqSetVol`, `func_8009A530`, the eight
`func_8009BEA0`..`func_8009C904` siblings, `func_8009CBD0`, `func_8009E330`,
`func_8009E5F4`, `func_8009EBB8`, `func_8009F354`, `func_800A3B8C`,
`func_800A3D20` and `func_800A3EB8`. Two keep their length but change order:
`func_8009E1F0` and `holiday_tel`.

The discriminator is not known. One candidate is that these are libsnd /
libspu / other-libgs members, possibly assembled by a different ASPSX
release or compiled by cc1 2.6.0 rather than 2.6.3. GsSortBg is the only
`gcc263` file. Treat the flag as a per-file model backed by that file's
retail evidence, and re-run this census before using it anywhere else.

## `gcc263p`: gcc 2.6.3 rebuilt from source, with three gated decision flags (2026-09-26, E120/E123)

`tools/psyq-gcc-260/cc1-psx-26-patched` is GNU C 2.6.3 for `mips-sony-psx`,
rebuilt from the GNU tarball as a 64-bit binary (recipe:
`tools/psyq-gcc-260/build_cc1_psx_26_patched.sh`; details in
`tools/psyq-gcc-260/README.md`). Select it with `@Compiler: gcc263p`
(gccwrap `--gcc-version 263p`). **With no flag it is the shipped
`cc1-psx-26`:** all 177 `@Compiler: gcc*` sources in `src/main/matched` +
`semi-matched` give a byte-identical `.s`, including the header comments.
One detail was needed for that: Psy-Q's build has `MASK_GPOPT` in
`TARGET_DEFAULT` (it prints `-mgpOPT` in its defaults), which the
decompals/old-gcc psx patch leaves out.

Each flag changes a compiler *decision* and emits only what the compiler
emits. None is a PostProc step, and each is per-file:

| flag | what it changes | registered gcc files whose `.s` changes when forced on |
|---|---|---:|
| `-fstack-args-first` | `expand_call` stores the stack-passed arguments (`store_one_arg`) before it precomputes the register arguments | 12 / 172 |
| `-fsched2-forward` | after reload, each block is re-ordered by a **forward** list scheduler (see below) | 131 / 172 |
| `-fsched-old-blockage` | both schedulers use the gcc 2.5 `genattrtab` blockage values: a store blocks the MIPS memory unit for 1 cycle, not 3 (see below) | 108 / 173 |

### `-fsched2-forward`: retail libcd is a TOP-DOWN list schedule

gcc 2.x's `sched.c` schedules bottom-up. E93 proved that no bottom-up pass
reaches CdIntToPos (`func_800880CC`): `sra t0,t0,2` is always ready inside
the 12-cycle mult stall. Retail is instead the **top-down** schedule of the
same dependence graph (`LOG_LINKS`) and the same MIPS unit model
(`insn_cost`; imul latency and issue 12, hilo issue 3):

1. Each cycle, the candidates are the insns whose predecessors have all
   issued, whose latency has elapsed and whose function units are free.
2. Pick the one with the longest latency-weighted path to the block end.
3. Break a tie in favour of the insn that is data-dependent on the insn
   issued last.
4. Break any remaining tie by original (pre-sched2) order.
5. If no insn is ready, the cycle is a stall and nothing is emitted. The
   assembler fills the stall, e.g. with the `mfhi`→`mult` hazard `nop`.

A 20-line Python model of this rule reproduced all 65 of retail's
instructions, and it did so from either our sched2 input or its output. The
same rule is in `sched.c`. It skips blocks that contain `SCHED_GROUP_P`
insns or interior notes and leaves them in backward order.

- `func_800880CC` goes from 21/66 (65 words) to **66/66, Body 66, no
  PostProc**.
- As a second witness, `func_800881D4` (libcd; registered with 7
  `swap_instructions`) is byte-perfect under the flag with every swap
  deleted. Two other libcd neighbours come close once their swaps are
  stripped: `func_80088E10` goes from 28 to 44/46 and `func_80088ED0` from
  21 to 37/45.
- It is **not** a universal retail model. With the swaps stripped, libgs
  `func_8009B4F0` drops from 349 to 107/620, and most libsnd and game-band
  gcc files are also worse. So treat it as a per-library property,
  evidenced so far in libcd only.
- A latency tie-break (prefer the longer-latency insn) was tried after rule
  3. It fixed one `lh; lhu; li` order in `func_80088ED0` (37 → 40) but broke
  `func_80088E10` (44 → 42), so it was not adopted.

### `-fstack-args-first`: built as specified, refuted on magazine_init

On `magazine_init` its best combination scores 17/52, and the best overall
without it is 21/52; the flag closes nothing. The volatile loads keep RTL
order through both schedulers. Retail's first call reads them in the
**stock** `expand_call` order: a1 and a2 precomputed, the stack argument
stored, then a3 loaded as a MEM in the hard-register step. Stack-first
contradicts that.

What retail's allocation actually shows is narrower: in both calls, the
*last* register argument is expanded after the stack stores. A scratch
build with that rule (not shipped) plus `-fno-schedule-insns` reproduces
call 2's allocation exactly. It still fails call 1, whose v0/v1 alternation
needs sched1, and it cannot produce retail's macro halves split across the
prologue, which is assembler-level reordering. The full evidence is in
`src/main/matched/magazine_init.c`.

**Superseded (E123):** the allocation was never an `expand_call` property.
It comes from sched1, and `-fsched-old-blockage` reproduces it exactly; see
the next section.

### `-fsched-old-blockage`: sched1's store tie-break is a 2.6 `genattrtab` change (E123)

`magazine_init` was filed as `gcc_arg_expansion_order`: seven register
fields in its 8-argument `printf` call. The filing blamed `expand_call`.
The real cause is sched1, and the evidence came from the decompals
`gcc-2.5.7-psx` prebuilt (`old-gcc` release 0.17). It emits retail's
allocation for the unchanged source. With `-fno-schedule-insns` it falls
back to 2.6's allocation. Its `calls.c` precompute loop is the same as
2.6's.

The `-dS` dump of 2.6.3 names the decision: `insn 69 has a greater
potential hazard`. When ready insns tie on priority, `schedule_select`
takes the one with the largest `potential_hazard()`. In 2.6.3 a store's
blockage range on the MIPS `memory` unit is [1,3] (`insn-attrtab` returns
`0x10003`). So every stack-argument store wins over the ALU argument
setters. The `>>23` chain is then scheduled last, and `val` dies there.
In 2.5.7 the store's range is [1,1] (`0x10001`, read from that binary's
`memory_unit_blockage_range`). Its `maxb` is 1, the hazard pick never
fires, and original order wins the tie.

The change is in `genattrtab`. 2.6 turned the blockage formula around,
from 2.5's READY-COST (C) - (READY-COST (E) - 1) to READY-COST (E) -
(READY-COST (C) - 1). The flag keeps both. `genattrtab` also computes each
unit's blockage with the 2.5 formula and writes a second table,
`function_units_old[]`, and `sched.c` picks the table by the flag. This is
the mechanism behind the 2026-09-22 note that "gcc 2.6 sched.c prefers a
load/store over an ALU op on equal priority; 2.5 shows retail's order".

Results on `magazine_init` (Exact/52, gcc263p, `--fill-earliest`):

| config | Exact | register fields off |
|---|---:|---:|
| stock | 12 | 7 |
| `-fsched-old-blockage` | 19 | 0 |
| `-fsched-old-blockage -fsched2-forward` | 26 | 0 |
| `-fsched-old-blockage -fno-schedule-insns2` | 32 | 0 |

In all three flag configs the word multiset is retail's after relocation
masking. What is left is order only, in three intra-block runs:

- the ASPSX prologue: `lui` above `addiu sp`, and the first global load
  above the register saves, as in the sibling functions `magazine_main` and
  `func_80086D5C`;
- call 1's `la a0` split around the `a1`/`a3` loads;
- call 2's `a1`/`a2`/`a3` setters interleaved with the last stack store.

Neither stock sched2 nor the forward model reproduces the last two. A
Python search over forward and backward list schedulers (priority,
tie-break and latency varied) got at best 16 of 20 words per call. So they
are bridged with 15 `swap_instructions`, the minimum for that permutation.
The swaps were gated by a dataflow check: 0 producer changes, the same
register state at both `jal`s and at the `jr`, the same delay slots, and
the only memory-order change is a non-volatile pointer load moving above
the stack saves. The function is byte-perfect at 52/52 with Fixup 0, and
both reloc audits print OK.

Flag-off validation: `.s` byte-identical on all 177 gcc sources, compared
with the previous binary and with `cc1-psx-26`. `-fstack-args-first` and
`-fsched2-forward` are also unchanged on all 177. `build_level3_c.py
--no-cache` gives the retail SHA1.

Census with the flag forced on: the `.s` changes in 111 of 177 sources.
Over the 169 that `test_single_match` can measure as gcc263p with their own
flags, 2 get better (`magazine_init`, and `SD_FadeXA` 47 to 49), 101 get
worse and 66 stay the same. Byte-perfect goes from 148 to 57. Like
`-fsched2-forward`, it is a per-file property with one witness so far. Try
it on a gcc semi when a register wall sits in a block where stores and ALU
ops tie on priority. Read `cc1 -dS` for `has a greater potential hazard`
first.


## `--loc-load-nop` — a gcc library TU built with `-g` (2026-10-03, EigenMatrix)

A gcc-path maspsx flag (`tools/maspsx`, routed through `gccwrap.py` /
`build_level3_c.py` like `--label-load-nop`). **Off by default**; set it per
file in `@CompileFlags`, always together with `-g` in the cc1 flags.

**The tell.** Retail pads EVERY load in the body with a nop, whether or not
the next word reads it -- `lhu t1,6(a0) ; nop ; move s6,a1 ; sh t1,22(sp)`,
six `lh rX,off(sp) ; nop` in a row in each switch arm, `lw a3,48(sp) ; nop ;
move t0,zero` -- but **not** the epilogue restores (`lw ra ; lw s6 ; ...`
back to back). Neither a "nop after every load" rule nor any hazard rule
fits both. A **source-line boundary** does: cc1 `-g` emits `.loc N` plus an
`LMn:` label before every statement (and none inside its text epilogue),
and ASPSX flushes a pending load delay there exactly as it does at a label
(`--label-load-nop`). maspsx looks through `.loc` and `LMn:` by design, so
the flag adds the flush: a load whose next instruction lies past a `.loc`
gets its nop.

**`-g` also changes sched2.** Line notes sit in the insn stream, so the
post-reload schedule differs; on EigenMatrix sched2 ON is now the closer
fit (12 differing lines vs 32 with `-fno-schedule-insns2`, which had been
compensating for the missing `-g`). Codegen is otherwise unchanged.

**Consumer.** `main/func_800A41C0` (libgte EigenMatrix) **BYTE-PERFECT
285/285**: `-O2 -g -fno-strength-reduce --mflo-nops-first --label-load-nop
--loc-load-nop --fill-epilogue` gives every one of the 19 missing nops; the
residue was three pure reorderings (swap_instructions, plain_vs_shipped
SAME). Its GEO.C siblings (IsIdMatrix, rcos, sin_1) are candidates for the
same build -- IsIdMatrix shows the identical `lh ; nop ; move ; sw` shape and
closed with ten swaps that `-g` may make unnecessary. Unit test:
`tools/maspsx/tests/test_loc_load_nop.py`.


## `--konami-lib-sched` -- the library build's list scheduler (2026-10-04)

A gcc-path maspsx flag (`tools/maspsx/maspsx/ksched.py`, routed through
`gccwrap.py` / `build_level3_c.py`). **Off by default**; set it per file in
`@CompileFlags`. It models assembler/scheduler behaviour and only REORDERS:
every output word is an input word or one word of an input macro's
expansion. It ranks with the compiler hooks on the PostProc ladder.

**The problem.** ~120 registered gcc library functions (libgs/libcd/libpress/
libapi/libetc) matched only through `swap_instructions` chains: retail's order
inside each basic block is not what cc1 + ASPSX emit, and no cc1 (2.4.1-2.8.1,
DOS CC1PSX), no ASPSX (2.21-2.86, passive on gcc text), no flag and no Sony
LIB object reproduces it. Two tells place the reordering AFTER macro
expansion: the halves of an `la` / symbol load / symbol store move
independently (`lui a1,%hi(X)` four words above its `addiu`, a store macro's
`lui $at` above the index `sll`), and division macros keep ASPSX's deferred
check (so the division itself was scheduled as one unit before it was
expanded).

**The model (v2, same day): IDO 5.2 as1's reorganizer, transcribed.** The
first model (an as1-like pick rule over a hand-built DAG) reached 39
functions. v2 replaces the DAG and the latency rules with a transcription of
as1's own `reorganize_bb`, read out of the recompiled `as1.c` and checked
against `as1 -R` traces. Per reorder-mode basic block:

* **DAG construction is as1's.** Nodes are visited from the LAST instruction
  backwards; each instruction class runs as1's def / use / memory handlers
  in as1's order (`func_42cc14`). Register defs and uses carry use stages: a
  load reaches an ALU use after 2 cycles with a 2-word hazard, a mult/div
  its mflo after 12 / 35.
* **Memory is a table of locations treated as pseudo registers**: same base
  and class with disjoint offsets do not conflict; the classes are a symbol,
  `$sp`, or "unknown", which conflicts with everything. A pointer bump
  `addiu rX,rX,K` lets the accesses based on rX float across it with their
  displacement rebased (`func_42bc70` / `func_42c5b4`) -- the one case where
  an output word's immediate differs from its input word.
* **Successor lists are ordered**: every edge goes into its predecessor's
  list sorted by `aftercycles(succ) + latency`, a new edge in FRONT of equal
  keys (`func_42ae44`). aftercycles walks that list with
  `v = max(v + size(succ), ac(succ) + latency)`, then the maxhazard and
  branch rules (a block-ending branch has 1, a node at exactly 1 gets 2).
* **Pick**: earliest start, then highest aftercycles, then longest successor
  latency, then lowest hazard address, then the HEAD of the ready list
  (newly ready successors are pushed on the head in successor-list order).
  A branch is held until one node is left, which becomes its delay slot.
* **Division**: a three-operand div/rem is expanded the way as1 expands it
  -- the divide, its trap check as ONE node of 9 (signed) / 3 (unsigned)
  words, and the mflo/mfhi; every node before a check precedes it (as1's -2
  edges).

Validation against as1 itself: on `as1 -R` traces of 300 IDO game TUs the
DAG reproduces 3,611 of 3,676 blocks' successor lists edge-for-edge and in
as1's order (3,618 without the retail differences below), and the scheduler
repeats 3,666 blocks' pick sequences; the rest are `.noalias` facts that
IDO's ugen passes and gcc text lacks.

**Where the retail library differs from IDO 5.2 as1** (each measured on the
124 swap-carrying gcc functions):

* no load -> later `$sp` adjust edge;
* the trap check waits for the quotient (34 cycles behind its divide, i.e.
  it lands next to the mflo); the dividend read of its overflow test is an
  ordinary register use (v3; v2 treated it as an ordering edge only);
* **divide timing (v3)**: a mult/div held behind the mflo/mfhi of an EARLIER
  divide's result is timed from that divide's issue + 1, not from its own
  issue, so its own move-from and trap check are not late against ready
  work (GsSetRefView/2: `mult ; mflo a1 ; lw` and `div ; negu ; <check> ;
  mflo ; sll ; sra`). IDO as1 itself times the mult from its real issue
  (_sprite_set_light_effect1), so this is a retail-only rule;
* the mflo/mfhi -> next HI/LO writer hazard is left to the assembler (no
  scheduler nops); it delays a mult 3 cycles behind an mflo, a divide or
  anything behind an mfhi only one;
* gcc's `.set noreorder` branch groups (cc1's own delay-slot fills) are kept
  verbatim as the block's last node -- every node precedes them; gcc's text
  epilogue (`.set noreorder ... j $31 ; slot`) is ordinary code, so the
  restores, the `addu $sp` and the `j $31` are scheduled and the slot
  re-picked (PadInit, CdGetSector, _cmp: `lw ra; addiu sp; jr ra; sltiu v0`);
* **the latency tie-break does not see into a noreorder group (v4,
  2026-10-04)**: when two nodes tie on start and aftercycles, "longest
  successor latency" counts an edge into a cc1 group ONLY when the node
  defines a register that a group instruction uses as its memory BASE
  (func_800B4430: `lw v0` feeding the jal slot's `sw $0,0(v0)` keeps its
  2). An edge that feeds the group's branch operand, a call argument or a
  slot store's DATA counts as no latency at all (GsMapModelingData: `ori`
  before the `lw a3` that feeds `blez a3`; cd_read_retry: `lui a0` before
  the printf-argument loads). Aftercycles, edges and hazards are
  unchanged -- moving the latency itself (DAG level) costs hazard nops and
  breaks up to nine users. Measured on the swap-stripped remainder of the 124:
  +62 exact words, four more functions swap-free, GsSortClear's last swap
  gone, no flag user regressed; two functions lose a word (CD_newmedia -2,
  func_80099670 -1, both still swap-bridged) -- the branch-operand and
  call-argument cases there go the other way, and no register-class rule
  tried (slot reads, non-data slot reads, branch operands, bumped loads)
  separates them without losing more;
* **no `lui $at` reuse for bss (v3)**: the ASPSX reuse of a live `lui $at`
  for the next store with the same %hi applies only to symbols in the EXE's
  initialised image (0x80041000-0x800E9000); above it (bss) retail
  re-emits the lui even for the same symbol (GsSetRefView, GsGetObjMatrix;
  the same rule as maspsx `REUSE_AT_DATA_HI`).

**What the pass may change.** Only ORDER: every output word is an input
word or one word of an input macro's expansion (`la`, `lw/sw rX,SYM[(rY)]`,
32-bit `li`, the division macro), plus a bumped access's displacement. Address
macros are expanded the ASPSX way (a reg+symbol LOAD builds the address in
its own destination, `$at` only when that is the index; stores use `$at`),
and a `lui $at` whose value is already live for the next store of the same
initialised-data symbol and %hi is reused, as ASPSX does (never for bss). Hazard nops (load -> consumer) are placed at
the hazard address; a branch and its slot are indivisible. Deterministic.

**Results (v4, 2026-10-04).** Of the 124 swap-chain gcc functions, with
every swap stripped and the flag on, **58 are byte-perfect with no swap**
(v1: 39; v2: 51; v3: 53). All are converted (`KSCHED 2026-10-04` header
line). v2 brought the 39 v1 conversions plus GsMulCoord2 / GsMulCoord3,
CD_getsector (11 swaps), GsInit3D (14), GsInitCoordinate2 (11),
GsSetLightMatrix (10), GsSortFastSprite (66), PadRead (1),
data_ready_callback (8), func_80089900 (3), print_matrix (1) and
print_vector (1); _mk_normsp also needed a C fix. v3 (divide timing, the
check's dividend as an ordinary use, no bss `lui $at` reuse) added
**GsSetRefView** -- no PostProc at all (129 swaps and 2 branch re-aims
gone) -- and **GsGetObjMatrix** (28 swaps, `--fill-epilogue --fill-j`).
v4 (the group tie-break) added DecDCTin (1 swap), GsClearOt (2, and its
hard strip count), cd_read_retry (9, `--fill-earliest`) and
ivlc_dct_dc_size (9, `--fill-load-delay --fill-earliest`), and retired
**GsSortClear**'s last intra-block swap. func_800B3E64 dropped its prologue
swaps (two independent swaps and a bijective `swap_registers` remain);
**GsSetRefView2** keeps exactly 2 intra-block prologue swaps (its input
block is identical to GsSetRefView's, yet retail scheduled it differently).
Outside the 124, two former asm_only `retail_lib_scheduler` functions are
now registered byte-perfect C with no swap and no paint: `_mk_xpndsp`
(177/177, v2) and **GsMapModelingData** (28/28, v4; see below). **62
registered files carry the flag.** Every one passes test_single_match
(Body == target, 100%), audit_reloc_positions, audit_main_relocs,
audit_registry_sizes, audit_plain_vs_shipped (SAME) and prove_equivalence
(PROVEN), and `build_level3_c.py --no-cache` gives the retail SHA1.

**GsMapModelingData -- how the "fall-through fill" closed.** It sat at
24/28 with two residues that looked like model gaps; neither was.
(1) The `blez` slot: retail keeps `move a2,zero` above the branch and fills
the slot from the FALL-THROUGH (`addiu a0,a0,4`). That is ordinary cc1
2.6.0 dbr given the right INPUT: Sony's own LIBGS member (SDK 3.3-4.0)
reads the count AFTER the pointer steps past the flag word
(`addiu a0,a0,4 ; move a2,zero ; lw a3,0(a0)`), so the last word before the
branch is the load that feeds it, dbr cannot take it, and fills from the
fall-through. Written that way (`p++; n = *p;`) cc1 emits retail's group,
and ksched floats `lw a3,0(a0)` above the bump as `lw a3,4(a0)` (the
pointer-bump rebase) -- retail's word. The hypothesis that Konami built with
`-fno-delayed-branch` and let the reorganizer fill slots is refuted: the
reorganizer then fills the slot from the block (`move a2,zero`).
(2) The `ori`/`lw a3` tie is the v4 rule above. Lesson, again: a residue
next to a cc1 group is usually the INPUT -- compare against Sony's .LIB
member's instruction order before blaming the scheduler.

**Not reached.** GsSetRefView2 keeps its 2 prologue swaps (above). The
remaining swap-chain functions keep their chains for now: most are
count-exact under the flag at 54-99% (e.g. func_800A41C0 283/285,
scale_view_param 58/60, Gslight_mate / Gslight_mate_fog 104/107,
func_80099670 307/318), the rest are not count-exact -- many residues are
the INPUT, not the scheduler: DecDCTvlc and scale_view_param get
byte-identical prologue blocks from our C, yet retail orders them
differently, so the original C gave cc1 a different order for one of them.
Before blaming the model, check whether a statement-order change gives cc1
the right input (GsMapModelingData above is the worked example).

**Using it.**

1. Add `--konami-lib-sched` to `@CompileFlags`. Keep the function's hazard
   flags (`--jal-load-nop --delay-slot-load-nop --label-load-nop
   --load-base-nop --load-dest-temp --mflo-*`); drop every `--fill-*`,
   `--macro-lui-fill*`, `--store-macro-split` and `--reuse-at-same-hi` flag
   (the scheduler does that work).
2. Set `strip_leading_instructions --count auto` -- the pass also schedules
   the types.h helpers in front, so a hard count goes stale.
3. Delete every `swap_instructions` and measure with test_single_match
   (`Body == target`, Exact 100%).
4. If a residue is left, re-derive the MINIMAL swaps against the flagged
   compile (diff the objdump of the compiled `.o` against the retail words);
   keep them intra-block, then require audit_plain_vs_shipped SAME and
   prove_equivalence PROVEN. A swap fitted to a different scheduler is stale
   the moment the flag changes -- re-derive, never carry it over.
5. Debugging: `KSCHED_TRACE=1` prints every candidate set and pick to
   stderr; `ksched.REC` / `FORCE` / `DECISIONS` are research hooks (block
   records, forced orders, logged picks) for decision-level replay.

Unit test: `cd tools/maspsx && python3 -m unittest discover -t . -s tests`
(`tests/test_konami_lib_sched.py`).
