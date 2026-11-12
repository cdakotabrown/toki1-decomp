/* @SymbolStart: 0x80138DF0 */
/* @PostProc: patch_insn_reg_field 0xf8:rs:t3:v0 0xf8:rt:v0:t3 0x108:rs:t4:v0 0x108:rt:v0:t4 0x118:rs:t5:v1 0x118:rt:v1:t5 */
/* @SymbolSize: 512 */
#include "types.h"

extern s16 ending_now_serifu_in;

extern void ending_bustup_anim_set(s32, s32);

extern s32 ending_serifu_frame;
extern s16 bustup2_cw_anim;
extern s32 wink2_stop;
extern s16 bustup_cw_anim;
extern s16 bustup_cw_frame;
extern s16 cw30_tick;

void ending_girl9_cue_step(void)
{
    s32 counter;
    s32 v1;
    s32 t6;
    s16 t3h, t4h, t5h;

    counter = ending_now_serifu_in;

    v1 = 1;

    if (counter == v1) {
        t6 = ending_serifu_frame;
        if (t6 != 0) {
            goto increment;
        }
        bustup2_cw_anim = 8;
        ending_bustup_anim_set(0, 255);
        wink2_stop = 0;
        goto increment;
    }

    if (counter == 6) {
        t6 = ending_serifu_frame;
        if (t6 != 0) {
            goto increment;
        }
        ending_bustup_anim_set(1, 5);
        wink2_stop = 1;
        goto increment;
    }

    if (counter == 8) {
        t6 = ending_serifu_frame;
        if (t6 != 0) {
            goto increment;
        }
        ending_bustup_anim_set(0, 0);
        wink2_stop = 0;
        goto increment;
    }

    if (counter == 11) {
        t6 = ending_serifu_frame;
        if (t6 == 0) {
            ending_bustup_anim_set(2, 5);
            wink2_stop = 1;
        } else {
            t3h = bustup_cw_anim;
            if (t3h == 2) {
                t4h = bustup_cw_frame;
                if (t4h == 2) {
                    t5h = cw30_tick;
                    if (t5h == v1) {
                        ending_bustup_anim_set(3, 0);
                        wink2_stop = 0;
                    }
                }
            }
        }
        goto increment;
    }

    if (counter == 13) {
        t6 = ending_serifu_frame;
        if (t6 != 0) {
            goto increment;
        }
        ending_bustup_anim_set(4, 5);
        wink2_stop = 1;
        goto increment;
    }

    if (counter == 15) {
        t6 = ending_serifu_frame;
        if (t6 == 0) {
            ending_bustup_anim_set(5, 5);
        }
        t6 = ending_serifu_frame;
        if (t6 == 210) {
            bustup2_cw_anim = 9;
            ending_bustup_anim_set(6, 0);
            wink2_stop = 0;
        }
        goto increment;
    }

    if (counter == 20) {
        t6 = ending_serifu_frame;
        if (t6 != 200) {
            goto increment;
        }
        ending_bustup_anim_set(4, 5);
        wink2_stop = 1;
    }

increment:
    t6 = ending_serifu_frame;
    ending_serifu_frame = t6 + 1;
}
