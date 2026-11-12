/* @SymbolStart: 0x80138AC4 */
/* @SymbolSize: 468 */
/* @PostProc: patch_insn_reg_field 0x144:rs:t6:v1 0x144:rt:v1:t6 */
/* ending_girl7_cue_step - ENDING overlay, Address: 0x80138ACC, Size: 460 bytes */
#include "types.h"

extern s16 ending_now_serifu_in;

extern void ending_bustup_anim_set();

extern u32 ending_serifu_frame;
extern s16 bustup2_cw_anim;
extern s32 wink2_stop;
extern s16 bustup_cw_anim;
extern s16 bustup_cw_frame;
extern s16 cw30_tick;

void ending_girl7_cue_step(void)
{
    s32 sel;

    sel = ending_now_serifu_in;
    if (sel == 1) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 7;
            ending_bustup_anim_set(0, 255);
            wink2_stop = 0;
        }
        goto increment;
    }
    if (sel == 6) {
        if (ending_serifu_frame == 1) {
            ending_bustup_anim_set(1, 5);
            wink2_stop = 1;
        }
        goto increment;
    }
    if (sel == 8) {
        if (ending_serifu_frame == 1) {
            ending_bustup_anim_set(2, 5);
        }
        goto increment;
    }
    if (sel == 9) {
        if (ending_serifu_frame == 0) {
            ending_bustup_anim_set(0, 0);
            wink2_stop = 0;
        }
        goto increment;
    }
    if (sel == 24) {
        if (ending_serifu_frame == 0) {
            ending_bustup_anim_set(3, 5);
            wink2_stop = 1;
        } else {
            if (bustup_cw_anim == 3) {
                if (bustup_cw_frame == 2) {
                    if (1 == cw30_tick) {
                        ending_bustup_anim_set(4, 0);
                        wink2_stop = 0;
                    }
                }
            }
        }
        goto increment;
    }
    if (sel == 28) {
        bustup2_cw_anim = 8;
        if (ending_serifu_frame == 0) {
            ending_bustup_anim_set(5, 255);
        }
        goto increment;
    }
    if (sel == 30) {
        if (ending_serifu_frame == 365) {
            ending_bustup_anim_set(6, 5);
            wink2_stop = 1;
        }
    }

increment:
    ending_serifu_frame = ending_serifu_frame + 1;
}
