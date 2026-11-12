
/* --- IDO52_VETO_WEB (Hook 40, 2026-08-14) ---------------------------------
 * Stop a VALUE from being register-allocated by uopt at all, so that every
 * reference stays a plain access to the value's own home and ugen supplies
 * whatever scratch register it needs from its temp free list (t6-t9, the
 * colourless pool -- Hook 36's domain).  Hooks 28/37/39 all operate INSIDE
 * the allocator's colour space and therefore cannot express "this value is
 * not an allocator candidate".
 *
 * Mechanism: f_makelivranges builds one LiveRange per web and hangs it off
 * the bittab (0x1001cbd0) at `bittab + bitpos*8 + 4`; f_globalcolor drives
 * itself off the candidate bitvector at 0x100223a8 (phase 1, tested with
 * f_bvectin0) and the leftover set at 0x100223a0 (phase 2).  Removing a
 * bitpos from BOTH bitvectors -- and only then nulling its bittab liverange
 * pointer -- takes the web out of every colouring loop without leaving a
 * dangling pointer for any loop to dereference.
 *
 * ⚠️ STAGE MATTERS.  The hook runs immediately after f_makelivranges (the
 * same point IDO52_PRECOLOR_WEB uses), i.e. BEFORE f_regdataflow and
 * f_localcolor, so the vetoed value is invisible to the local colourer too.
 *
 * IDO52_VETO_WEB=BITPOS[,BITPOS...]   (Hook 28 / 37 / 39 key space)
 * IDO52_VETO_WEB_STAGE=1|2|3  -- 1 = right after f_makelivranges, 2 = right
 *     before f_globalcolor, 3 = both (DEFAULT).  Stage 1 alone is NOT
 *     enough: f_regdataflow and f_localcolor repopulate 0x100223a8 between
 *     the two points, so a stage-1-only veto is output-inert (measured).
 * IDO52_VETO_WEB_DROPLR=1  -- additionally null the bittab liverange
 *     pointer.  ⚠️ MEASURED TO ABORT uopt (the colour loops dereference the
 *     slot unconditionally once its bit is set again); kept only as a
 *     diagnostic, never as a working mode.
 * ------------------------------------------------------------------- */
#define VETO_WEB_MAX 24
static int ido52_veto_web[VETO_WEB_MAX];
static int ido52_veto_web_count;
static int ido52_veto_web_droplr;
static int ido52_veto_web_stage = 3;

/* --- IDO52_VETO_FORMLIV (Hook 40b) ----------------------------------------
 * The GENERAL (non-constant) web-formation veto.  Hook 30's IDO52_FORM_SITE
 * only reaches the f_constinreg / f_ldainreg candidacy queries, i.e. it can
 * deny a web to a CONSTANT or an address (LDA) but not to an ordinary value.
 * Every live range, constant or not, is materialised by f_formlivbb (33 call
 * sites inside the makelivranges walker, already tagged with the
 * ido52_pform_flbb_site id Hook 30 installed).  Skipping the call for a
 * named occurrence denies the web outright, leaving the value's references
 * as accesses to its own home and its expression temporaries to ugen's
 * colourless t6-t9 free list (Hook 36's domain).
 *
 * IDO52_VETO_FORMLIV=OCC[:SITE][,...]
 *   OCC   0-based ordinal of the f_formlivbb call within the current
 *         procedure (the counter resets per proc via IDO52_FORMLIV_TRACE's
 *         proc marker)
 *   SITE  optional: only veto when ido52_pform_flbb_site == SITE
 * IDO52_FORMLIV_TRACE=1  print every call as `occ site a0 a1`.
 * ------------------------------------------------------------------- */
#define VETO_FORMLIV_MAX 32
static struct { int occ; int site; } ido52_veto_formliv[VETO_FORMLIV_MAX];
static int ido52_veto_formliv_count;
static int ido52_formliv_trace;
static int ido52_formliv_occ;
