/* @SymbolStart: 0x80154D68 */
/* @SymbolSize: 220 */
/* Function: date_girl1_serifu_judge -- DATE overlay */
/* Address: 0x80154D68 (cross-prologue head) / 0x80154D70 (body) */
/* Size: 220 bytes (55 insns) */
#include "types.h"

extern void bustup_face_change(s32 a);
extern void goto_sub_sub_step(s32 a);
extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern s32 serifu_no;
extern u8  ev_flag09;
extern volatile s16 girl1_tokimeki;
extern volatile s16 girl1_yuukou;
extern volatile s16 girl1_syoushin;
extern s16 kao2[];
extern s16 ans[];

void date_girl1_serifu_judge(void)
{
    if (serifu_no == 1) {
        bustup_face_change(2);
        goto_sub_sub_step(46);
    } else {
        ev_flag09 = 1;
        kao2[0] = 3;
        kao2[1] = 3;
        kao2[2] = 3;
        ans[0] = 2;
        ans[1] = 2;
        ans[2] = 2;
        girl1_tokimeki = girl1_tokimeki - 2;
        girl1_yuukou = girl1_yuukou - 1;
        girl1_syoushin = girl1_syoushin + 20;
        param_clamp();
        sub_sub_step_up();
    }
}
