/* @SymbolStart: 0x80137F8C */
/* @SymbolSize: 332 */
/* @PostProc: patch_insn_reg_field 0xb4:rs:t2:v1 0xb4:rt:v1:t2 */
/* ending_girl2_cue_step - ENDING overlay, Address: 0x80137F94, Size: 324 bytes (81 insns) */
#include "types.h"

extern void ending_bustup_anim_set();

extern s16 ending_now_serifu_in;
extern u32 ending_serifu_frame;
extern s16 bustup2_cw_anim;
extern s32 wink2_stop;
extern s16 bustup_cw_anim;
extern s16 bustup_cw_frame;

void ending_girl2_cue_step(void)
{
    s32 v0;
    s32 counter;

    v0 = ending_now_serifu_in;

    if (v0 == 1) {
        if (ending_serifu_frame != 0) {
            goto increment;
        }
        bustup2_cw_anim = 5;
        ending_bustup_anim_set(0, 255);
        wink2_stop = 0;
        goto increment;
    }
    if (v0 == 23) {
        if (ending_serifu_frame == 0) {
            goto increment;
        }
        if (ending_serifu_frame != 1) {
            goto check_23b;
        }
        bustup2_cw_anim = 6;
        ending_bustup_anim_set(1, 5);
        wink2_stop = 1;
        goto increment;
    check_23b:
        if (bustup_cw_anim == 1 && bustup_cw_frame == 3) {
            ending_bustup_anim_set(2, 1);
        }
        goto increment;
    }
    if (v0 == 24) {
        if (ending_serifu_frame != 0) {
            goto increment;
        }
        ending_bustup_anim_set(3, 0);
        wink2_stop = 0;
        goto increment;
    }
    if (v0 == 27) {
        if (ending_serifu_frame != 0) {
            goto increment;
        }
        ending_bustup_anim_set(4, 5);
        wink2_stop = 1;
    }

increment:
    counter = ending_serifu_frame;
    ending_serifu_frame = counter + 1;
}
