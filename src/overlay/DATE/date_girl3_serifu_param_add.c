/* @SymbolStart: 0x80152CE0 */
/* Function: date_girl3_serifu_param_add */
/* Overlay: DATE */
/* Address: 0x80152CE0 (cross-prologue head) / 0x80152CE8 (body) */
/* @SymbolSize: 220 */
#include "types.h"

extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern s32 serifu_no;
extern u16 now_serifuev;
extern u16 now_serifuev_in;
extern volatile s16 girl3_tokimeki;
extern volatile s16 girl3_yuukou;
extern volatile s16 girl3_syoushin;

void date_girl3_serifu_param_add(void)
{
    if (serifu_no == 1) {
        now_serifuev = now_serifuev + 2;
        now_serifuev_in = 0;
        girl3_tokimeki = girl3_tokimeki + 2;
        girl3_yuukou = girl3_yuukou + 2;
        girl3_syoushin = girl3_syoushin - 20;
        param_clamp();
        sub_sub_step_up();
    } else {
        girl3_tokimeki = girl3_tokimeki + 3;
        girl3_yuukou = girl3_yuukou + 1;
        girl3_syoushin = girl3_syoushin - 20;
        param_clamp();
    }
    sub_sub_step_up();
}
