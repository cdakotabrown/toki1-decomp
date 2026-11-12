/* @SymbolStart: 0x801382A0 */
/* @SymbolSize: 740 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=1 */
/* @PostProc: patch_insn_reg_field 0x25c:rt:t7:t6 0x264:rt:v0:v1 0x274:rs:v0:v1 0x274:rt:v0:v1 0x27c:rs:v0:v1 0x284:rt:t4:t3 0x288:rt:t5:t4 0x28c:rt:t6:t5 0x29c:rt:t4:t3 0x2bc:rt:t5:t4 0x2d4:rt:t6:t5 0x2d8:rs:t7:t6 0x2d8:rt:t7:t6 0x2e4:rs:t7:t6 0x2e4:rt:t8:t7 0x2e8:rt:t8:t7 */
/* ending_girl4_cue_step - ENDING overlay, Address: 0x801382A8, Size: 732 bytes */
#include "types.h"
#include "tokimeki/class_w.h"

extern void ending_bustup_anim_set();

extern s32 ending_serifu_frame;
extern s16 ending_now_serifu_in;
extern s16 bustup2_cw_anim;
extern s32 wink2_stop;
extern s16 bustup_cw_anim;
extern s16 bustup_cw_frame;
extern u8  bustup_cw_mode;
extern s32 frame_counter;
extern u8  now_sub_sub_step;
extern s32 step_counter;
extern CLASS_W class_w[];

void ending_girl4_cue_step(void)
{
    s16 g;
    u32 m;

    if (ending_now_serifu_in == 1) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 6;
            ending_bustup_anim_set(0, 255);
            wink2_stop = 0;
        }
        goto tail;
    }
    if (ending_now_serifu_in == 8) {
        if (ending_serifu_frame == 0) {
            ending_bustup_anim_set(1, 255);
        }
        goto tail;
    }
    if (ending_now_serifu_in == 9) {
        if (ending_serifu_frame == 0) {
            ending_bustup_anim_set(0, 255);
        }
        goto tail;
    }
    if (ending_now_serifu_in == 10) {
        if (ending_serifu_frame == 240) {
            bustup2_cw_anim = 7;
            ending_bustup_anim_set(1, 255);
        }
        goto tail;
    }
    if (ending_now_serifu_in == 12) {
        if (ending_serifu_frame == 0) {
            ending_bustup_anim_set(0, 255);
        }
        goto tail;
    }
    if (ending_now_serifu_in == 15) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 6;
            ending_bustup_anim_set(1, 255);
        }
        goto tail;
    }
    if (ending_now_serifu_in == 17) {
        if (ending_serifu_frame == 1) {
            bustup2_cw_anim = 8;
            ending_bustup_anim_set(2, 1);
            wink2_stop = 1;
        }
        if (ending_serifu_frame == 140) {
            ending_bustup_anim_set(4, 5);
        }
        goto tail;
    }
    if (ending_now_serifu_in == 18) {
        if (ending_serifu_frame == 0) {
            ending_bustup_anim_set(2, 1);
            wink2_stop = 0;
        }
        goto tail;
    }

    if (now_sub_sub_step == 2) {
        if (step_counter == 0) {
            ending_bustup_anim_set(5, 5);
            wink2_stop = 1;
        }
    }

tail:
    g = ending_now_serifu_in;
    if (g >= 18) {
        if (bustup_cw_anim < 4) {
            m = (u32)frame_counter % 240U;
            if (m == 0) {
                bustup_cw_anim = 3;
                bustup_cw_frame = 0;
                class_w[30].tick = 0;
            }
            if (m >= 8U) {
                bustup_cw_anim = 2;
                bustup_cw_frame = 0;
                class_w[30].tick = 0;
                bustup_cw_mode = 1;
            }
        }
    }
    ending_serifu_frame = ending_serifu_frame + 1;
}
