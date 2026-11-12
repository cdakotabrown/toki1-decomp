/* @SymbolStart: 0x801394C0 */
/* @SymbolSize: 524 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=1 */
/* ending_cuec - ENDING overlay, Address: 0x801394C8, Size: 516 bytes */
#include "types.h"

extern s16 ending_now_serifu_in;

extern void ending_bustup_anim_set();

extern s32 ending_serifu_frame;
extern s16 bustup2_cw_anim;
extern s32 wink2_stop;
extern u8  ending_wait_done;
extern u8  ending_wait_cnt;
extern s16 bustup_cw_anim;

void ending_cuec(void)
{
    s32 sel;

    sel = ending_now_serifu_in;
    if (sel == 1) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 5;
            ending_bustup_anim_set(0, 255);
            wink2_stop = 0;
            ending_wait_done = 0;
        }
        goto increment;
    }
    if (sel == 7) {
        if (ending_serifu_frame == 0) {
            ending_bustup_anim_set(1, 0);
            wink2_stop = 1;
        }
        goto increment;
    }
    if (sel == 8) {
        if (ending_serifu_frame == 200) {
            ending_bustup_anim_set(0, 5);
            wink2_stop = 0;
        }
        goto increment;
    }
    if (sel == 11) {
        if (ending_wait_done == 0 && ending_wait_cnt == 10) {
            ending_wait_done = 1;
            ending_wait_cnt = 0;
        }
        if (ending_wait_cnt == 30) {
            if (bustup_cw_anim == 0) {
                if (ending_wait_done == 1) {
                    ending_bustup_anim_set(2, 0);
                    bustup2_cw_anim = 6;
                    wink2_stop = 1;
                }
            }
        }
        goto increment;
    }
    if (sel == 14) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 7;
            ending_bustup_anim_set(3, 1);
        } else {
            if (ending_serifu_frame == 140) {
                ending_bustup_anim_set(4, 5);
            }
        }
        goto increment;
    }
    if (sel == 15) {
        if (ending_serifu_frame == 0) {
            ending_bustup_anim_set(3, 1);
        }
        goto increment;
    }
    if (sel == 16) {
        if (ending_serifu_frame == 0) {
            ending_bustup_anim_set(4, 5);
        }
    }

increment:
    ending_serifu_frame = ending_serifu_frame + 1;
}
