/* Function: date_cw98_mode5_set */
/* Overlay: DATE */
/* Address: 0x80152AE0 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"
#include "tokimeki/class_w.h"
extern CLASS_W class_w[];

extern s32 sub_sub_step_up();

s32 date_cw98_mode5_set(void)
{
    class_w[98].anim_flag = 5;
    return sub_sub_step_up();
}
