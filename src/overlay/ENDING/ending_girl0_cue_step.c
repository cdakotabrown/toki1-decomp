/* @SymbolStart: 0x80137BDC */
/* @SymbolSize: 652 */
/* @CompileFlags: -cckr */
#include "types.h"
#include "tokimeki/class_w.h"

extern void ending_bustup_anim_set();
extern s16 ending_now_serifu_in;
extern u32 ending_serifu_frame;
extern s16 bustup2_cw_anim;
extern s32 wink2_stop;
extern u8  ending_wait_done;
extern u8  ending_wait_cnt;
extern s16 bustup_cw_anim;
extern s16 bustup_cw_frame;
extern u8  bustup_cw_mode;
extern u32 frame_counter;
extern u8  now_sub_sub_step;
extern CLASS_W class_w[];

ending_girl0_cue_step()
{
    u32 m;

    if (ending_now_serifu_in == 1) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 7;
            ending_bustup_anim_set(0, 255);
            wink2_stop = 0;
            ending_wait_done = 0;
        }
    } else if (ending_now_serifu_in == 14) {
        if (ending_wait_done == 0 && ending_wait_cnt == 50) {
            ending_wait_done = 1;
            ending_wait_cnt = 0;
        }
        if (ending_wait_cnt == 1 && bustup_cw_anim == 0 && ending_wait_done == 1) {
            ending_bustup_anim_set(1, 5);
            wink2_stop = 1;
        }
    } else if (ending_now_serifu_in == 17) {
        if (ending_serifu_frame != 0) {
            if (ending_serifu_frame == 1) {
                bustup2_cw_anim = 8;
                ending_bustup_anim_set(2, 1);
                wink2_stop = 1;
            } else if (ending_serifu_frame == 40) {
                ending_bustup_anim_set(3, 5);
            } else if (bustup_cw_anim == 3 && bustup_cw_frame == 5) {
                ending_bustup_anim_set(2, 1);
            } else if (ending_serifu_frame == 120) {
                ending_bustup_anim_set(4, 5);
            } else if (bustup_cw_anim == 4 && bustup_cw_frame == 7) {
                ending_bustup_anim_set(5, 1);
                wink2_stop = 0;
            }
        }
    }
    if (ending_now_serifu_in >= 18) {
        m = frame_counter;
        if (m % 240 == 0) {
            bustup_cw_anim = 6;
            bustup_cw_frame = 0;
            class_w[30].tick = 0;
        }
        if (m % 240 >= 12) {
            bustup_cw_frame = 0;
            bustup_cw_mode = 1;
            class_w[30].tick = 0;
            bustup_cw_anim = 5;
        }
    }
    if (now_sub_sub_step == 2) {
        bustup_cw_anim = 5;
        bustup_cw_mode = 1;
    }
    ending_serifu_frame++;
}
