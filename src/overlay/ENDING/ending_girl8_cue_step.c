/* @SymbolStart: 0x80138C98 */
/* @SymbolSize: 344 */
/* ending_girl8_cue_step - ENDING overlay, Address: 0x80138CA0, Size: 336 bytes */
#include "types.h"

extern void ending_bustup_anim_set();

extern s32 ending_serifu_frame;
extern s16 ending_now_serifu_in;
extern s16 bustup2_cw_anim;
extern s32 wink2_stop;

void ending_girl8_cue_step(void)
{
    s32 v0 = ending_now_serifu_in;

    if (v0 == 1) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 5;
            ending_bustup_anim_set(0, 255);
            wink2_stop = 0;
        }
        goto increment;
    }
    if (v0 == 7) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 6;
        }
        goto increment;
    }
    if (v0 == 9) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 5;
            ending_bustup_anim_set(1, 5);
            wink2_stop = 1;
        }
        goto increment;
    }
    if (v0 == 11) {
        if (ending_serifu_frame == 0) {
            ending_bustup_anim_set(2, 0);
            wink2_stop = 0;
        }
        goto increment;
    }
    if (v0 == 15) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 7;
            ending_bustup_anim_set(3, 5);
            wink2_stop = 1;
        } else if (ending_serifu_frame == 60) {
            ending_bustup_anim_set(4, 0);
            wink2_stop = 0;
        }
    }

increment:
    ending_serifu_frame = ending_serifu_frame + 1;
}
