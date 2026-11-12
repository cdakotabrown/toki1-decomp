/* Function: date_main0_serifu7_set */
/* Overlay: DATE */
/* Address: 0x8013F4A8 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern s16 date_now_serifu0;
extern s32 sub_sub_step_up();

s32 date_main0_serifu7_set(void)
{
    date_now_serifu0 = 7;
    return sub_sub_step_up();
}
