/* @SymbolStart: 0x80133A8C */
/* Address: 0x80133A94 */
/* Size: 180 bytes (45 instructions) */
/* @PostProc: scheduler_normalizer --const-fp --store-fp --branch-fp */
/* @SymbolSize: 188 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void param_clamp(void);

extern volatile s16 shougatu_meet_now_serifu;
extern volatile s16 girl0_tokimeki;
extern volatile s16 girl0_yuukou;
extern volatile s16 girl0_syoushin;
extern u8 shougatu_girl0_good;
extern s32 serifu_no;

void shougatu_girl0_para_set(void)
{
    if (serifu_no != 0) {
        shougatu_meet_now_serifu = shougatu_meet_now_serifu + 1;
        girl0_tokimeki = girl0_tokimeki - 1;
        girl0_yuukou = girl0_yuukou - 1;
        girl0_syoushin = girl0_syoushin + 10;
        shougatu_girl0_good = 0;
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
    } else {
        girl0_tokimeki = girl0_tokimeki + 1;
    }
    param_clamp();
    sub_sub_step_up();
}
