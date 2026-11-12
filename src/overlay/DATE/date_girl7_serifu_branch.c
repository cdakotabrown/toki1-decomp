/* @SymbolStart: 0x80153E7C */
/* @SymbolSize: 128 */
/* Address: 0x80153E7C (cross-prologue head) / 0x80153E84 (body) */
/* Size: 128 bytes (32 instructions) */
#include "types.h"

extern void goto_sub_sub_step(s32);
extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern s32 serifu_no;
extern s16 now_serifuev;
extern volatile u8  ev_flag31;
extern volatile s16 girl7_yuukou;
extern volatile s16 girl7_syoushin;

void date_girl7_serifu_branch(void)
{
    if (serifu_no != 0) {
        now_serifuev = 6;
        goto_sub_sub_step(36);
        return;
    }
    ev_flag31 = 1;
    girl7_yuukou = girl7_yuukou - 1;
    girl7_syoushin = girl7_syoushin + 10;
    param_clamp();
    sub_sub_step_up();
}
