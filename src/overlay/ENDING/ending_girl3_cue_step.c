/* @SymbolStart: 0x801380D8 */
/* @SymbolSize: 456 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=1 */
/* ending_girl3_cue_step - ENDING overlay, Address: 0x801380E0, Size: 448 bytes (112 insns) */
#include "types.h"

extern void ending_bustup_anim_set();

extern s16 ending_now_serifu_in;
extern s32 ending_serifu_frame;
extern s16 bustup2_cw_anim;
extern s32 wink2_stop;

void ending_girl3_cue_step(void)
{
    s32 v0;
    s32 counter;

    v0 = ending_now_serifu_in;

    if (v0 == 1) {
        if (ending_serifu_frame != 0) {
            goto increment;
        }
        bustup2_cw_anim = 4;
        ending_bustup_anim_set(0, 255);
        wink2_stop = 0;
        goto increment;
    }
    if (v0 == 13) {
        if (ending_serifu_frame != 1) {
            goto increment;
        }
        bustup2_cw_anim = 5;
        ending_bustup_anim_set(1, 1);
        wink2_stop = 1;
        goto increment;
    }
    if (v0 == 14) {
        if (ending_serifu_frame != 1) {
            goto increment;
        }
        bustup2_cw_anim = 4;
        ending_bustup_anim_set(0, 0);
        wink2_stop = 0;
        goto increment;
    }
    if (v0 == 18) {
        if (ending_serifu_frame != 1) {
            goto increment;
        }
        bustup2_cw_anim = 5;
        ending_bustup_anim_set(1, 1);
        wink2_stop = 1;
        goto increment;
    }
    if (v0 == 21) {
        if (ending_serifu_frame != 1) {
            goto increment;
        }
        bustup2_cw_anim = 4;
        ending_bustup_anim_set(0, 0);
        wink2_stop = 0;
        goto increment;
    }
    if (v0 == 23) {
        if (ending_serifu_frame != 0) {
            goto increment;
        }
        bustup2_cw_anim = 6;
        ending_bustup_anim_set(2, 1);
        wink2_stop = 1;
        goto increment;
    }
    if (v0 == 25) {
        if (ending_serifu_frame != 1) {
            goto increment;
        }
        ending_bustup_anim_set(3, 0);
        wink2_stop = 0;
    }

increment:
    counter = ending_serifu_frame;
    ending_serifu_frame = counter + 1;
}
