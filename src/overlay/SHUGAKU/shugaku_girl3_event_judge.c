/* @SymbolStart: 0x80138FB8 */
/* @SymbolSize: 108 */
#include "types.h"

extern u8 now_girl;

extern void fade_down_main(void);
extern void goto_sub_step(s32 a0);

extern u8  shugaku_place;
extern u8  now_sub_step;
extern u8  ret_sub_step;
extern u8  ret_sub_sub_step;

void shugaku_girl3_event_judge(void)
{
    s32 param_t6 = now_girl;

    s32 t7, t8, t9;

    if (param_t6 != 3) goto lbl_call;
    t7 = (s32)shugaku_place;
    if (t7 == 1) goto lbl_80138FF4;
lbl_call:
    fade_down_main();
    goto lbl_80139018;
lbl_80138FF4:
    t8 = (s32)now_sub_step;
    ret_sub_step = (u8)(t8 + 1);
    ret_sub_sub_step = 0;
    goto_sub_step(9);
    return;
lbl_80139018:
    return;
}
