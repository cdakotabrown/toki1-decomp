/* Function: date_main0_step_up */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();

s32 date_main0_step_up(s32 a0, s32 a1, s32 a2, s32 a3)
{
    s32 v0 = 0;

    v0 = sub_sub_step_up(a0, a1, a2, a3);
    return v0;
}
