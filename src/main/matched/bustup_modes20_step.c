/* Function: bustup_modes20_step */
/* Address:  0x800856B0 */
/* Size:     48 bytes (12 instructions, includes cross-prologue thunk) */
/* Region:   Game (0x80041000-0x80085FFF) - IDO 5.2 */
/* Debug Symbol: bustup_modes20_step (REVERTED - offset 88, uncertain) (from o.bin) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s16 cal_cw_anim;
extern s16 cw97_anim;

extern void sub_sub_step_up(void);

void bustup_modes20_step(void) {
    volatile s32 pad[4];
    cal_cw_anim = 2;
    cw97_anim = 0;
    sub_sub_step_up();
}
