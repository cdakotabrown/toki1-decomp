
/* --- IDO52_FORCE_WEB_HOME (Hook 38, 2026-08-14) ---------------------------
 * Positive per-web WEB-HOME-STORE force.  Hook 37 chooses a web's colour;
 * this decides whether ugen writes the value INTO that colour at the def
 * (the `or $rHome,$rTemp,$zero` copy) when uopt's own cost model priced the
 * copy out.
 *
 * Mechanism (measured 2026-08-14, GYOZI/func_8013306C + the uopt listing):
 *   f_compute_save (L46c530..) accumulates a per-liverange benefit into
 *   f22 and divides it by an occurrence count at lr+28, storing the result
 *   as the float `adjsave` at lr+48; `hasstore` is the u8 at lr+34 and the
 *   "worth colouring" flag is lr+35.  f_globalcolor consults adjsave three
 *   times: L46eb48 (pick the highest-adjsave uncoloured web), L46ef64
 *   (`adjsave * lr[28] <= threshold` -> range-split), L46fa20
 *   (`threshold < adjsave * lr[28]` -> allocate).  The per-LiveUnit
 *   decisions that survive into the u-code re-emission are the four u8
 *   flags printed by f_printregs as
 *       firstisstr(lu+20)  deadout(lu+23)  needreglod(lu+21)  needregsave(lu+22)
 *   and the emission pass f_reemit (called right after globalcolor) is the
 *   consumer.  needregsave == the home store.
 *
 * IDO52_FORCE_WEB_HOME=BITPOS[:BB][=MASK][,...]
 *   BITPOS  bittab index (the SAME key space as Hooks 28 and 37)
 *   BB      restrict to the LiveUnit(s) whose block number (node+8, u16)
 *           equals BB; omitted = every LiveUnit of the web
 *   MASK    bit 1  set needregsave (lu+22) = 1    [default]
 *           bit 2  set needreglod  (lu+21) = 1
 *           bit 4  set hasstore    (lr+34) = 1
 *           bit 8  clear deadout   (lu+23) = 0
 *           bit 16 clear firstisstr(lu+20) = 0
 *           0      probe only (trace, no write)
 *
 * IDO52_WEB_HOME_TRACE=1  dump every web's adjsave / hasstore / assigned_reg
 *                         and each LiveUnit's bb + four flags, post-globalcolor.
 *                         (=2 also dumps before the force is applied.)
 *
 * SOUNDNESS: a forced home store is an EXTRA copy into a register the
 * allocator already owns for that web -- it never changes which register
 * any reader reads, and it never removes an instruction.  It is exactly the
 * copy uopt itself emits when adjsave clears its own gate.  The hook
 * refuses to act on a web with no assigned colour (assigned_reg 0 or 0xff),
 * because a home store into "no register" is meaningless.
 * ------------------------------------------------------------------- */
#define FORCE_WEB_HOME_MAX 24
static struct { int bitpos; int bb; int mask; } ido52_force_web_home[FORCE_WEB_HOME_MAX];
static int ido52_force_web_home_count;
static int ido52_web_home_trace;
/* stage: 1 = before f_globalcolor, 2 = after, 3 = both (default) */
static int ido52_web_home_stage = 3;

/* IDO52_FORCE_WEB_ADJSAVE=BITPOS:VALUE[,...] (Hook 38b)
 * f_compute_save divides the accumulated per-liverange benefit by the
 * occurrence count at lr+28 and stores the quotient as the float `adjsave`
 * at lr+48; f_globalcolor then multiplies it back by lr+28 and compares the
 * product against the running best-colour cost.  This overrides the stored
 * quotient at f_compute_save's exit, i.e. exactly the number the listing
 * prints as "adjsave, hasstore:". */
#define FORCE_WEB_ADJSAVE_MAX 24
static struct { int bitpos; float value; } ido52_force_web_adjsave[FORCE_WEB_ADJSAVE_MAX];
static int ido52_force_web_adjsave_count;

