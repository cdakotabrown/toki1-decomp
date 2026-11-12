/* Address: 0x800857FC */
/* Size: 48 bytes (12 instructions) */
/* Region: IDO (0x80086000-0x800BFFFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack 24 40 */
/* Compiler: IDO 5.2 */
#include "types.h"

extern s32 bustup_mode;
extern s8 cal_cw_mode;
extern void sub_sub_step_up(void);

void bustup_st0_step(void) {
    bustup_mode = 1;
    cal_cw_mode = 0;
    sub_sub_step_up();
}
