/* bunka_sd_state_init - BUNKA_SD overlay, Address: 0x801361B8, Size: 172 bytes (43 insns) */
/* @SymbolSize: 172 */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 step_counter;

extern u8 cal_cw_mode;
extern u8 cw97_mode;
extern CLASS_W class_w[];

void bunka_sd_state_init(void)
{
    s32 scene;

    scene = step_counter;

    if (scene == 0) {
        cal_cw_mode = 5;
        cw97_mode = 5;
        class_w[98].anim_flag = 5;
    }

    if (scene == 934) {
        class_w[99].disp_flag = 128;
        class_w[99].anim_flag = 5;
        class_w[99].bright = 128;
        class_w[100].disp_flag = 128;
        class_w[100].anim_flag = 5;
        class_w[100].bright = 128;
    }

    if ((class_w[99].anim_flag & 1) == 0) {
        class_w[99].disp_flag = 0;
        class_w[100].disp_flag = 0;
    }
}
