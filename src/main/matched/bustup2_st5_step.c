/* Function: bustup2_st5_step */
/* Address:  0x8008582C */
/* Size:     48 bytes (12 instructions, includes cross-prologue thunk) */
/* Region:   Game (0x80041000-0x80085FFF) - IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
/* Post-processing required:
 *   1. patch_stack_complete.py --old 24 --new 40
 *
 * Cross-prologue function: IDO hoists "lui at" before the prologue.
 * True function starts at 0x8008582C (4 bytes before map boundary).
 * Compiled output is 48 bytes; skip first 4 bytes (cross-prologue lui)
 * to get 44-byte body match.
 *
 * Sets wink2_stop = 0, cw97_mode = 5, then calls sub_sub_step_up.
 */
extern int wink2_stop;
extern unsigned char cw97_mode;
extern void sub_sub_step_up(void);

void bustup2_st5_step(void) {
    wink2_stop = 0;
    cw97_mode = 5;
    sub_sub_step_up();
}
