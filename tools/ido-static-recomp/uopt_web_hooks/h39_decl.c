
/* --- IDO52_FORCE_WEB_SPILL (Hook 39, 2026-08-14) --------------------------
 * Force a named web to be SPILLED (assigned_reg = -1) instead of coloured.
 * Hook 37 names the colour a web gets; Hook 28 forbids colours one at a
 * time; neither can say "give this web NO register".
 *
 * Mechanism: f_globalcolor's phase-1 loop reads the liverange's "worth"
 * byte at lr+35 (f_compute_save writes 1 when adjsave > 0, else 0):
 *     v0 = MEM_U8(s5 + 35);
 *     if (v0 == 0) f_compute_save(s5), v0 = MEM_U8(s5 + 35);
 *     if (v0 != 2)  -> normal colour scan  (L46eb48)
 *     else          -> L46ea34, the UNCOLOURABLE epilogue
 * L46ea34 is the compiler's own spill path: it stores 0xff into
 * lr+32 (assigned_reg), f_resetbit()s the web out of the phase-2 worklist
 * bvect at 0x100223a8, and walks the lr+56 chain decrementing each node's
 * +36 pressure counter.  Setting lr+35 = 2 before f_globalcolor therefore
 * routes the web down that existing path with every downstream invariant
 * (colour masks, class high-water marks, phase-2 worklist, pressure
 * counters) maintained by the compiler itself -- this hook writes no
 * assigned_reg and touches no mask, which is why it does not need the
 * separate epilogue the Hook 37 report anticipated.
 *
 * IDO52_FORCE_WEB_SPILL=BITPOS[,BITPOS...]   (Hook 28 / 37 key space)
 * ------------------------------------------------------------------- */
#define FORCE_WEB_SPILL_MAX 24
static int ido52_force_web_spill[FORCE_WEB_SPILL_MAX];
static int ido52_force_web_spill_count;

/* IDO52_FORCE_WEB_NOSPILL=BITPOS[,...] -- the inverse.  A web the allocator
 * already marked uncolourable (lr+35 == 2, e.g. because f_compute_save gave
 * it a NEGATIVE adjsave) never reaches the colour pick at all, so Hook 37
 * cannot name a colour for it.  Setting lr+35 = 1 puts it back on the
 * colour-scan path; pair with IDO52_FORCE_WEB_ADJSAVE to give it a cost the
 * scan will accept and IDO52_FORCE_WEB_COLOR to name the register. */
#define FORCE_WEB_NOSPILL_MAX2 24
static int ido52_force_web_nospill[FORCE_WEB_NOSPILL_MAX2];
static int ido52_force_web_nospill_count;
