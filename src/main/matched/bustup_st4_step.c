/* Function: bustup_st4_step */
/* Address:  0x800857CC */
/* Size:     48 bytes (12 instructions, includes cross-prologue thunk) */
/* Region:   Game (0x80041000-0x80085FFF) - IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
/* Post-processing required:
 *   1. patch_stack_complete.py --old 24 --new 40
 *
 * Cross-prologue function: IDO hoists "lui at" before the prologue.
 * True function starts at 0x800857CC (4 bytes before map boundary).
 * Compiled output is 48 bytes; skip first 4 bytes (cross-prologue lui)
 * to get 44-byte body match.
 *
 * Sets bustup_mode = 0, cal_cw_mode = 4, then calls sub_sub_step_up.
 */
extern int bustup_mode;
extern unsigned char cal_cw_mode;
extern void sub_sub_step_up(void);

void bustup_st4_step(void) {
    bustup_mode = 0;
    cal_cw_mode = 4;
    sub_sub_step_up();
}
