/* @SymbolStart: 0x80138584 */
/* @SymbolSize: 596 */
/* @CompileFlags: -cckr */
#include "types.h"
#include "tokimeki/class_w.h"

extern s16 ending_now_serifu_in;
extern u32 ending_serifu_frame;
extern s16 bustup2_cw_anim;
extern s32 wink2_stop;
extern s16 bustup_cw_anim;
extern s16 bustup_cw_frame;
extern u8  bustup_cw_mode;
extern u32 frame_counter;
extern CLASS_W class_w[];

void ending_girl5_cue_step()
{
    u32 m;
    if (ending_now_serifu_in == 1) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 7;
            ending_bustup_anim_set(0, 255);
            wink2_stop = 0;
        }
    } else if (ending_now_serifu_in == 11) {
        if (ending_serifu_frame == 250) {
            ending_bustup_anim_set(1, 255);
        }
    } else if (ending_now_serifu_in == 13) {
        if (ending_serifu_frame == 1) {
            bustup2_cw_anim = 8;
            ending_bustup_anim_set(2, 5);
            wink2_stop = 1;
        } else if (ending_serifu_frame == 100) {
            ending_bustup_anim_set(3, 5);
        } else if (bustup_cw_anim == 3 && (bustup_cw_frame | 0) == 1 && (class_w[30].tick | 0) == 1) {
            ending_bustup_anim_set(4, 1);
            wink2_stop = 0;
        }
    } else if (ending_now_serifu_in == 17) {
        if (ending_serifu_frame == 370) {
            ending_bustup_anim_set(6, 5);
            wink2_stop = 1;
        }
    }
    if (ending_now_serifu_in >= 13) {
        m = frame_counter;
        if (m % 240 == 0 && bustup_cw_anim == 4) {
            bustup_cw_anim = 5;
            bustup_cw_frame = 0;
            class_w[30].tick = 0;
        }
        if (m % 240 >= 12 && bustup_cw_anim == 5) {
            bustup_cw_anim = 4;
            bustup_cw_frame = 0;
            bustup_cw_mode = 1;
            class_w[30].tick = 0;
        }
    }
    ending_serifu_frame++;
    if (ending_now_serifu_in == 9 && ending_serifu_frame != 0) {
        k_speed_set(14);
    }
}
