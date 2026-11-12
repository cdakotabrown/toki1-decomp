/* Likely Symbol: bustup_modes31_step (from o.bin, offset +236 bytes from 0x80085690) */
/* Address: 0x8008577C - 0x800857A4 (40 bytes) */
/* Region: IDO (>= 0x80081000) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s16 cal_cw_anim;

extern void sub_sub_step_up(void);

void bustup_mode2_step(void) {
    volatile s32 pad[4];
    cal_cw_anim = 2;
    sub_sub_step_up();
}
