/* Likely Symbol: bustup_modes31_step (from o.bin, offset +80 bytes from 0x80085690) */
/* Address: 0x800856E0 - 0x80085714 (52 bytes) */
/* Debug Symbol: bustup_modes31_step (REVERTED - offset 80, uncertain) (from o.bin, high match) */
/* Region: IDO (>= 0x80081000) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s16 cal_cw_anim;
extern s16 cw97_anim;

extern void sub_sub_step_up(void);

void bustup_modes31_step(void) {
    volatile s32 pad[4];
    cal_cw_anim = 3;
    cw97_anim = 1;
    sub_sub_step_up();
}
