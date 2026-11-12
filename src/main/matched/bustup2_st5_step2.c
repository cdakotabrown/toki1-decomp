/* Function: bustup2_st5_step2 */
/* Address:  0x8008585C */
/* Size:     52 bytes (13 instructions, includes cross-prologue thunk) */
/* Region:   Game (0x80041000-0x80085FFF) - IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 wink2_stop;
extern u8 cw97_mode;
extern void sub_sub_step_up(void);

void bustup2_st5_step2(void) {
    wink2_stop = 1;
    cw97_mode = 5;
    sub_sub_step_up();
}
