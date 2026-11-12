# Equivalence proof of shipped objects (translation validation)

Tool: `tools/analysis/prove_equivalence.py` (built 2026-10-04; hardened the same
day by three passes, all described here).

## The question

For each shipped function, let **P** be what its C compiles to with every
`@PostProc` line removed (same `@CompileFlags`, `@CompileEnv` and PIC mode), and
**S** the object the build ships. S equals the retail bytes; that has already been
checked. This tool proves that **S does what P does for every input**. P is taken as
the meaning of the C, so a proof says the PostProc chain changed nothing that can be
observed.

A verdict is `PROVEN` (a simulation relation was found and every obligation below was
discharged), `REFUTED` (S contains a MIPS-I hazard, which is a definite defect), or
`UNPROVEN` (the reason is printed). Nothing is assumed in order to reach a proof,
apart from the assumptions listed in their own section below. Every rule that relaxes
an obligation leaves a **note** on the result, so the run's note census shows exactly
which rules a proof used.

P's PIC mode comes from `ido_flags.stripped_pic_extra` (the single definition shared
with `audit_plain_vs_shipped` and `audit_raw_count`): a file that names its own
`-Wc,-picN` keeps it, and anything else gets the derived mode of the *shipped* file,
spelled out explicitly. Before 2026-10-04 both audits appended `-Wc,-pic0` last,
which overrode a `-Wc,-pic1` stamp.

## Model

* **Machine.** MIPS I integer ISA plus COP0 and COP2, little-endian, with branch and
  jump delay slots. A branch reads its condition before its slot executes. A call reads
  its arguments after its slot. Sequential semantics are exact only when the code has no
  hazards, so hazards are checked first:
  * a load whose destination is read by the next word that executes, on any successor
    (an `lwl`/`lwr` pair is exempt);
  * `mfhi`/`mflo` followed within two words by `mult`, `div`, `mthi` or `mtlo`;
  * a load in a call's delay slot that does not match between P and S (the callee's
    first word sees the old value).

  A hazard in S makes the verdict REFUTED. A hazard in P is only noted
  (`plain-compile-hazard`). The plain compile leaves hazard scheduling to the
  assembler, and its meaning is the sequential one the compiler intended.
* **Values.** Hash-consed terms over block-entry variables. Relocated immediates are
  symbolic. When the symbol's address is known, a relocation takes its linked value
  instead, with `%hi` carrying the paired `%lo` exactly as GNU ld pairs them. That pair
  is the next `LO16` against the same symbol, in relocation order. Normalisation is
  exact algebra only:
  * constant folding, linear sums and commutative ordering;
  * `or x,0` = `addu x,0` = `x`;
  * in address arithmetic (`_lin`), the low word of a product by a constant,
    `lo(mult/multu(x, c))`, is `c*x mod 2^32`.

  Two terms are equal only if they are the same object.
* **Memory.** Non-frame memory is an ordered event list. A load's value is
  `ld(address, size, sign, block, the preceding stores/calls that may alias it)`, or the
  forwarded value of an exactly matching earlier store. Two accesses may alias unless
  one of these holds:
  * both are numerically resolved and their ranges are disjoint;
  * they use the same base term with disjoint offsets;
  * their address difference is `const + sum(k_i * atom_i) mod 2^32` and it is
    disjoint by congruence, or, with one atom of known interval, by an exact modular
    solve (`_affine_meets`, brute-force checked in the self-test);
  * one is a non-escaped frame slot and the other is not.

  Pointer provenance is **not** used. It is behind `PEQ_PROV=1` for experiments only.
* **Interval analysis** (`reg_intervals`, P only). It gives a signed range per register
  at each block entry. A branch on an `slti`/`sltiu` result refines the compared
  register **and every register copied from it** (`or/addu rd, rs, zero`) that is still
  equal at the branch. Joins take the hull, and a block whose entry state changes more
  than 48 times is widened to unknown. Its only use is to bound block-entry variables
  in the alias solve. It is checked against concrete execution by a fuzz
  (400 programs x 20 inputs, with copies and redefinitions between compare and branch).
* **Frame, mode A** (no stack address is formed). Each stack slot is a cell, a
  pseudo-register. Cells are related by value like registers, so a slot remap, frame
  resize or saved-register slot swap is only a renaming. At function entry, P and S
  cells at the same address are related. A call clobbers its home and stack-argument
  zone; the new contents are a function of the call and of the prior contents.
* **Frame, mode C** (a stack address escapes). The escaped objects come from the
  compiler. P is compiled again with IDO `-g3`, which must have P's frame layout (every
  `$sp`-relative word with the same operation and offset), and `.mdebug` is parsed.
  Each object's S base is a guess, from a formation pairing or a global vote, and the
  proof must succeed under it. Object accesses are memory events on `obj(i)+offset`.
  Objects must not overlap each other or any call zone, and one-past-the-end pointers
  are allowed. Two extensions, each noted or recorded:
  * **Unlocated locals** (`unlocated-local-area`). IDO `-O2 -g3` records many locals as
    `scRegister` with no frame address, arrays included. A formation that no located
    local explains is attributed to one object that over-approximates the unlocated
    candidates: aggregates, unknown types, and scalars whose address the C takes. That
    object is every placement of their total size that covers all unexplained
    formations, clipped to the local area: above the outgoing-argument area, below the
    save area and 0, and between located objects. A larger object is only stricter,
    because it turns more cells into ordered memory.
  * **A dynamic address no single local explains** (`local-area-object`, a third
    base-guess strategy after the position-paired and global-vote ones). IDO folds a
    constant of an index into the displacement, so `((u8 *)&obj + i)[j - 1140]`
    becomes `addu t5,sp,t7 ; lbu t9,-1064(t5)`: the `$sp` constant names no local. In
    C a frame address can only come from the address of a local, and pointer
    arithmetic stays inside that local, so the access lies in some candidate local
    (an aggregate, a local of unknown type, or a scalar whose address the C takes).
    All candidates are merged into ONE object spanning their hull. Every direct `$sp`
    access inside it becomes ordered memory (stricter than cells), accesses at two
    offsets of it are compared by exact address arithmetic, and the "distinct objects
    never alias" rule is not used at all. It is refused when a candidate has no frame
    address in the debug info, when a static address formation falls outside the
    hull, or when the hull holds a register save slot of either side.
  * **The outgoing argument area.** A by-value aggregate argument is built through a
    pointer to `sp+0`. The area `[sp, sp + 16 + 4*max argument words)` is then one
    object on both sides, with S's base fixed at S's frame bottom. A call reads its
    stack arguments from that object's memory and is ordered against it; later loads
    depend on the call, because the callee owns those bytes. A call zone may lie inside
    the area but must not straddle it.
* **Frame, mode B** (an escape and no usable debug info, for example GCC). The frame is
  shared memory with an identical layout. Each byte is either valid (P[b] = S[b]) or
  invalid after a call zone, tracked by a must-dataflow:
  * a frame load whose bytes are not valid at block entry is UNPROVEN;
  * since 2026-10-04, a frame load at a dynamic address (`FX`) needs *every* frame
    byte valid. It is refused after a call in the same block, or when any byte is
    invalid at block entry. It used to go unchecked.

## Obligations, per paired block (or per path in path mode)

1. **CFG shape.** Blocks are paired by a simultaneous traversal from the entry.
   Matching terminators must have the same kind and branch opcode. Jump-table entries
   must correspond, which requires byte- and relocation-identical data sections.
2. **Branch conditions and jump-table indices** are equal terms.
3. **Calls, syscall/break, COP2 and trapping arithmetic** form the same sequence.
   * Call targets must be equal.
   * **Argument registers and words.** A target that starts a function compares the
     callee's argument **words**. Those are the parameter count, except where the
     prototype passes something that may be an aggregate by value, or a 64-bit scalar.
     There the words are counted O32-style: an aggregate takes `ceil(size/4)` words,
     aligned to an even word when its alignment is 8. The sizes and alignments are
     measured by IDO itself: a probe compiled `-g` from the file plus one object of
     each type and `struct {char c; T x;}`. A callee whose layout cannot be measured is
     refused (`call-args`). Before 2026-10-04 the parameter count was used, so a call
     passing three 8-byte structs had only 3 of its 6 stack words compared.
   * Up to 24 stack argument words are compared (`MAX_STACK_ARGS`).
   * Any other target (mid-function, assembly) has its inputs read off the retail code
     by liveness, noted `callee-inputs`. If they cannot be derived, the verdict is
     UNPROVEN.
   * EVENT and GYOZI link against a different main layout. Their link bases are
     measured from their own `jal`s, and main-range calls use the C arity, noted
     `callee-outside-images`.
   * After the call, `$v0`/`$v1` are functions of the call and of their prior value.
     `$v0` is a pure function of the call when the callee is known to return a value;
     it is the distinct term `retv` when the file itself declares the callee `void`.
     The other clobbered registers are functions of the call and their prior value.
     `$ra` is the code address after the call.
4. **Stores** match one-to-one by identical (address, value, size, kind). Every
   non-frame load is matched one-to-one, because a load from hardware has an effect.
   On a path that ends in an IDO divide trap on both sides (same terminator, same
   conditions), RAM loads before the trap are released, but hardware loads stay
   matched. This is noted `ub-trap-path`. It does not treat division by zero as UB:
   both sides provably reach the same `break`.
5. **Order.** Every matched pair keeps its relative order when the pair may alias. A
   call or barrier is ordered against all memory except non-escaped frame slots. Two
   hardware accesses (`0x1F000000` and above) are always ordered. Two accesses to
   C-declared `volatile` RAM may be reordered under sequential semantics, noted
   `c-volatile-order`.
6. **Relation.** The relation each successor assumes holds on the edge, together with
   what the edge's own branch implies (`bne x,c` not taken means x = c). It is a
   greatest fixpoint, re-derived until consistent. Facts are:
   * pairs;
   * constants;
   * derived terms;
   * same-side linear differences (`p - q = K` on both sides);
   * **cross-side offsets** (2026-10-04): `P_p = S_s + K`, for code that bumps a
     counter in a delay slot on one side and at the join on the other. Every fact is
     re-checked on every edge.
7. **Return.** These must be equal: `$ra`, `$sp` (restored), `$s0`-`$s8`, `$gp`, and
   `$v0` unless the C function is `void`, subject to the indeterminate-return rules
   below.

### Indeterminate return (path-precise, 2026-10-04)

C defines the return value only on paths that end in `return <expr>;`. On a value-less
path (falling off the end, or a bare `return;`), `$v0` is indeterminate. The old rule
dropped the `$v0` check at **every** return once the function could fall off its end,
so a changed `return K;` value went undetected. Now `$v0` is exempt only where the
path is shown to be value-less. Every exemption is noted, together with
`indeterminate-return`:

* **No valued return at all** (`return_sites`: value-less exits, no `return <expr>;`,
  no `#define` containing `return`). Every path is value-less, so the function is
  proven as `void`.
* **`valueless-path`.** On this path P's `$v0` is the function-entry value (block 0
  has no predecessor), or was last written by a call to a callee **the file itself
  declares `void`**. A `return <expr>;` path must write its value into `$v0` after its
  last call, because `$v0` is caller-clobbered, so neither form can be one.
  Declarations seen only in `include/` do not count: an undeclared callee is `int` to
  the compiler. Applies only when the C has value-less exits.
* **C' marker** (`valueless-exit`). Each value-less exit is replaced by
  `return (RT)(long)&__peq_noval;`, an address nothing else can form, on the same line
  and with no newline, and the result is compiled as P'. S is proven against P', and
  `$v0` is exempt exactly where P''s term mentions `__peq_noval`. P' refines C (it fixes
  the indeterminate value and changes nothing else). This only works when the marker
  does not change the CFG shape.
* **C'' sentinels** (`sentinel-return`, `valueless-flow`, `valueless-edge`). This
  applies when every `return <expr>;` is an integer literal or a direct call
  `f(...)`.
  * Each literal is replaced by its own **sentinel**. A sentinel is in the return
    type's range, in the same one-instruction class, and equal to no literal of the
    file (raw or converted) and no 16-bit immediate of P. The result is compiled as P''.
    Nothing can equal a sentinel by knowledge of a comparison, so a `return K;` path of
    P'' ends with `$v0` equal to exactly that constant, and S must then hold K.
  * A path whose `$v0` is neither a sentinel nor possibly a value is value-less.
    "Possibly a value" means a constant, a call result when C has call returns, a
    block-entry variable other than the function entry's, junk, or a frame or zone
    load.
  * In block mode, a return block that writes neither `$v0` nor calls on either side
    is decided **per incoming edge**. Each edge needs either the sentinel's literal in
    S, or a P-only forward taint analysis (`valued_taint`) saying that no path reaching
    the edge can carry a return value: no sentinel, no copy, no memory round trip of
    one, and no call result when C has call returns.
* **`v0-unread-by-callers`.** `$v0` is not compared if no caller in the retail images
  reads it after the call, and no data word or `lui`/`addiu` pair forms the function's
  address. A caller that returns it is checked recursively, or is void in its own C.

**Fallbacks**, each a complete re-proof that is checked end to end:

* **Path mode** (`path-mode`). Every path between cut points (the entry and loop
  headers) is executed as one straight line, so code moved across a branch is compared
  per path.
* **Local path mode** (`local-path-mode`). Path mode over a cut set of every block
  except a small region after the block where block mode failed. It is sound for any
  cut set that holds the entry and a block of every cycle.
* **Indeterminate stack** (`indeterminate-stack`). Uninitialised local cells at entry
  are paired by the instruction that reads them. This only affects cells read before
  they are written.
* **Mode B** after mode C fails, when the frame layout is identical.

## Why a PROVEN verdict is sound

The checked relation R is a simulation, by induction over executions:

* At function entry the states are equal. Every register is paired with itself, and
  the frame memory is the same physical memory.
* If the P and S states are R_b-related at entry to block pair b, then:
  * symbolic execution gives each side's exact effect, because the semantics are
    sequential and there are no hazards;
  * obligations 2–5 make the observable effects identical: control decisions, callee
    identities and inputs (all argument words), the multiset and aliasing order of
    stores, and every non-frame load;
  * obligation 6 makes the successor states R-related.
* At a return, obligation 7 makes everything the caller can observe equal. The only
  exception is `$v0` on a path that the rules above show to be value-less in the C,
  where C itself leaves the value indeterminate.
* A call is an opaque function of what it can observe: its input registers, argument
  words and memory, with escaped frame objects reachable only in mode B/C. Those are
  proven equal before the call, so both sides receive identical callee results.

For the C'/C'' rules, the proof is against a compile of a refinement of C. C' fixes an
indeterminate value. C'' renames literals, and the renaming is undone at the
comparison. The trust in P' and P'' is the trust already placed in P: a stock compile
of C.

## Assumptions (all stated, none hidden)

* **ABI.** A function entry reads only the argument words of its C prototype, plus
  memory. Each call that relies on this carries the note `c-arity`. A K&R callee with
  no prototype takes the smallest argument count over its call sites, and each argument
  is counted as one word. A K&R call that passes an aggregate by value is not detected.
* **Unknown-arity calls** (no prototype, no call site, an indirect call whose
  function-pointer types disagree). All four `$aN` are compared. The stack words
  compared are those stored at `sp+16..` in the call's own block.
* **Link values.** Symbol addresses are those the build links with (`name_to_addr`).
* **Debug info.** A `-g3` compile has P's frame layout. This is checked by the `$sp`
  word sets.
* **Stack contents.** Uninitialised stack bytes are indeterminate. This is used by the
  noted `indeterminate-stack` and `moved-escaped-object` proofs.
* **Return values.** A literal return value is materialised as that constant on its
  path (used by C'' together with the sentinel's uniqueness). A void callee's leftover
  `$v0` is not a C value.
* **Sequential RAM.** RAM has sequential semantics. Interrupt handlers that race with
  reordered C-`volatile` RAM accesses are not modelled (the `c-volatile-order` notes).
  A load through a pointer is RAM unless its address is a constant in the hardware
  range.

## Self-test

`--self-test` runs 145 checks, all of which must pass:

* **39 equivalences that must be PROVEN.** Among them:
  * renames, slot remaps, a frame resize and loop swaps;
  * escaped objects and a store sunk to a join;
  * every indeterminate-return route (entry value, void call, C' marker, C''
    sentinel, block-mode per-edge, no valued return);
  * the outgoing-argument area and an unlocated local;
  * a dynamic frame load before a call;
  * a dynamic frame address attributed to the merged candidate-local object, also
    with a non-candidate slot remapped.
* **60 dishonest variants that must NOT be PROVEN.** Among them:
  * dependent stores swapped, a changed constant, branch or call target;
  * a load moved across an aliasing store, a dataflow paint;
  * a changed `return K;` value on a function that can fall off its end, with and
    without C'/C'';
  * an undeclared or header-only "void" callee;
  * a call result that may be the value;
  * a changed or unmeasured by-value struct argument word;
  * a wrong slot for an unlocated local, or an unlocated scalar that is not a
    candidate;
  * a dynamic frame load after a call;
  * for the merged candidate-local object: a store moved past the dynamic load, a
    changed displacement, no candidate local, and a save slot inside the hull.
* **46 unit checks.** Among them:
  * the interval fuzz;
  * the affine solver against brute force;
  * `lo(mult)` linearity;
  * `.mdebug` sizes;
  * return-site, valued-kind and sentinel instrumentation;
  * the return-value taint;
  * aggregate argument words measured by IDO, including an 8-aligned struct;
  * jump tables, link shifts and window trimming.

## Cache

`--cache DIR` keys each collected record by a **content hash**:
* the C source, which carries its `@` directives;
* the `.sym` map, or the registry entry plus `main_symbol_boundaries.json`;
* the `IDO52_*`/`L3_*`/`MASPSX*` environment;
* a digest of every file under `tools/analysis`, `tools/overlay`, `tools/build`,
  `tools/postproc`, `tools/decomp`, `tools/maspsx` and `include`, plus the IDO and
  Psy-Q compiler binaries.

Any change to any of these gives a new key, so a stale record cannot be reused. The
cache used to be keyed by function name and was never invalidated.

## Results, 2026-10-04 (full run, fresh cache, ~2.5 min on 16 jobs)

| region | group | functions | PROVEN | UNPROVEN | REFUTED |
|---|---|---:|---:|---:|---:|
| overlay | with PostProc | 2,213 | 2,213 | 0 | 0 |
| overlay | no PostProc | 3,898 | 3,898 | 0 | 0 |
| main | with PostProc | 691 | 689 | 2 | 0 |
| main | no PostProc | 683 | 683 | 0 | 0 |

All 4,581 zero-PostProc functions are word-for-word P == S, and the prover proves
every one of them without the identity shortcut.

History:
* first run: 96 UNPROVEN;
* second pass: overlay 58 → 8, with 15 real C-order defects fixed;
* this pass: overlay 8 → 4 and main 3 → 0 (the two remaining main functions belong to
  work in progress elsewhere);
* closing pass: overlay 4 → 0 (6,111 / 6,111 PROVEN), see below.

Fixed in this pass:
* by prover extensions:
  * ENDING/func_80135F6C: copy-aware intervals and `lo(multu)` linearity;
  * GYOZI/func_801348B8: cross-side offsets and the per-edge return;
  * GYOZI/func_80133118: the path-precise return;
  * TAIIKU/func_8014001C: unlocated locals;
  * main/ContDataEntry: the outgoing-argument object and struct argument words;
* by hooks: main/InitSoundSeq and InitSoundSep, through the new Hook 80
  `IDO52_UGEN_CMPBR_RIGHT_FIRST` and Hook 62e `IDO52_AS1_DIV_MACRO_MFFIRST_FLUSH` (see
  `UOPT-HOOKS-REFERENCE.md`). With these hooks the PostProc-free compile has retail's
  order, and all 53 bridging swaps are gone.

Closed in the last overlay pass (2026-10-04):

| function | was | fix |
|---|---|---|
| GEKO/func_8013E8A8 | memory-order | **C in retail order**: `r = f() % 84; e->f6 = -120; tables = 0; e->f4 = r - 159;`. Two existing gated hooks keep that order: Hook 32 `IDO52_COPYPROP_SITE=lod:10:deny` stops uopt's bb-local LOD substitution from sinking the rem to the last store, and Hook 35 `IDO52_AS1_TIE_TAKE=142` breaks as1's tie between the divide-check `bnez` and `sh f6` toward the check. The six old swaps are down to two and none crosses an aliasing access. |
| OPTION/func_801344EC | store-value | **C in retail order** with the sister OPTION/func_80133D08 recipe: loop 1 recomputes its record pointer from the global each pass (`rp = &D_80128B30[i + 11]`, exit `&D_80128B30[i] < &D_80128B30[3]`), so ugen hands as1 `.noalias` pairs and as1 itself hoists the f3 `lbu` and the sp-array reloads. Loops 2-4 use one index; declaring i/rp/f5 before the arrays gives retail's 120-byte frame natively (patch_stack + remap gone). 31 swaps down to 5. |
| GEKO/func_80142BCC | cfg-shape | **The rotate `custom_script` is retired.** The `v1 == 7U` compare is signed (`7U` made uopt CSE a (u32)v1 copy 100 words early) with 7 kept out of the const web (`CONST_NOINREG_VALUES=19,7`); Hook 58 `COPYPROP_VAR=-28` keeps `bval = v1` a real copy and Hook 21 `FORCE_LA_PULL=15` keeps as1's legal xbb pull of it; Hook 27 `XBB_VOLATILE_OK=all` lets as1 boost the volatile F6BF `lui`s as retail does, which is legal once Hook 36 `UGEN_TEMP_REG=120..128` gives two blocks retail's temp registers. Three in-block reorders remain as plain swaps. Proven in local path mode. |
| SHOUGATU/func_801403E4 | escape-unknown | **Prover extension**: the merged candidate-local object (Model, mode C). The frame layout was already native. |

Remaining UNPROVEN: main/GsSetRefView and GsSetRefView2, under active work elsewhere
(`--konami-lib-sched`); the main rows above are from the previous full run.

Regenerate with:

```
python3 tools/analysis/prove_equivalence.py --cache /tmp/peq --json out.json
```
