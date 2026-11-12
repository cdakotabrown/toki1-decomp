/* Address: 0x80135288 */
#include "types.h"

extern s32 pad_trig;

extern void sub_sub_step_up();
extern void goto_sub_sub_step(s32);
extern s32 taco_slot_fill();

void
taco_slot_wait2(void)
{
    s32 v0;
    s32 a0;
    s32 t6;
    s32 t7;

    v0 = taco_slot_fill();
    if (v0 != 0) goto lbl_801352B0;
    sub_sub_step_up();
    goto lbl_801352D0;
lbl_801352B0:
    t6 = (s32)pad_trig;
    t7 = t6 & 0x860;
    if (t7 == 0) goto lbl_801352D0;
    goto_sub_sub_step(5);
lbl_801352D0:
    ;
}
