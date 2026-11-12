/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8014AE0C */
/* @SymbolSize: 156 */
/* Address: 0x8014AE14, Size: 148 bytes (37 insns) */
#include "types.h"

extern u8 step_mode;
extern s32 sub_sub_step_up(void);
extern void k_sub_reset(void);
extern void gnsx(void *);
extern s32 sndisp(void *, s32, s32);
extern s32 xa_wait(void);
extern u32 p_sei;
extern u32 etc_soudan_main1_0_str0;

s32 etc_soudan_main1_0(void)
{
    s32 ret;

    ret = step_mode;
    switch (step_mode) {
    case 0:
        k_sub_reset();
        gnsx(&p_sei);
        ret = sndisp(&etc_soudan_main1_0_str0, 0, 15);
        step_mode = step_mode + 1;
        break;
    case 1:
        ret = xa_wait();
        break;
    case 2:
        ret = sub_sub_step_up();
        break;
    }
    return ret;
}
