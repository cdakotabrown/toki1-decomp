/* Size: 104 bytes */
/* @SymbolSize: 104 */
#include "types.h"
#include "tokimeki/class_w.h"

extern u8 now_sub_sub_step;
extern u8 cal_cw_bright;
extern u32 frame_counter;
extern CLASS_W class_w[];

void geko_cw_bright_set(void)
{
    if ((u32)now_sub_sub_step >= 35U) {
        class_w[98].bright = cal_cw_bright;
    }
    if ((u32)now_sub_sub_step >= 35U) {
        if (frame_counter % 300 == 20) {
            class_w[98].anim_flag |= 3;
        }
    }
}
