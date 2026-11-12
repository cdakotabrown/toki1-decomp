/* date_girl6_white_out_exit -- DATE overlay, Address: 0x801573FC, 44 insns. BYTE-PERFECT. */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @SymbolStart: 0x801573EC */
/* @SymbolSize: 176 */
/* @PostProc: scheduler_normalizer --block-aware */
#include "types.h"

extern s32 sub_sub_step_up();

extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern s32 Sprites[][9];
extern u8 cal_cw_disp;
extern u8 cw97_disp;
extern s32 fade_flag;

s32 date_girl6_white_out_exit(void)
{
    short i;

    back_clear_g = 0;
    back_clear_r = 0;
    back_clear_b = 0;
    for (i = 0; i < 6; i++) {
        Sprites[i][36] &= 0x7FFFFFFF;
    }
    cal_cw_disp = cal_cw_disp | 0x80;
    cw97_disp = cw97_disp | 0x80;
    fade_flag = 1;
    return sub_sub_step_up();
}
