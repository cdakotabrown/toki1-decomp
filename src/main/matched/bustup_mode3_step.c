/* Address: 0x800857A4 - 0x800857CC (40 bytes) */
/* Region: IDO (>= 0x80081000) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s16 cal_cw_anim;

extern void sub_sub_step_up(void);

void bustup_mode3_step(void) {
    volatile s32 pad[4];
    cal_cw_anim = 3;
    sub_sub_step_up();
}
