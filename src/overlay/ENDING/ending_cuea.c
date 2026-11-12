/* @SymbolStart: 0x80138FF0 */
/* @SymbolSize: 540 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=1 */
/* @PostProc: patch_insn_reg_field 0x178:rt:v0:v1 0x184:rt:t9:t8 0x188:rs:v0:v1 0x188:rt:v0:v1 0x190:rs:v0:v1 0x194:rt:t4:t3 0x198:rt:t6:t5 0x1a8:rs:t4:t3 0x1a8:rt:t4:t3 0x1b0:rs:t4:t3 0x1b4:rt:t5:t4 0x1bc:rt:t5:t4 0x1dc:rs:t6:t5 0x1dc:rt:t6:t5 0x1e4:rs:t6:t5 0x1e8:rt:t7:t6 0x1f0:rt:t7:t6 0x208:rt:t8:t7 0x20c:rt:t8:t7 0x210:rs:t9:t8 0x210:rt:t9:t8 0x21c:rs:t9:t8 0x21c:rt:t0:t9 0x220:rt:t0:t9 */
/* ending_cuea - ENDING overlay, Address: 0x80138FF8, Size: 532 bytes */
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
extern CLASS_W class_w[];

void ending_cuea(void)
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
    if (ending_now_serifu_in == 9) {
        if (ending_serifu_frame == 220) {
            ending_bustup_anim_set(1, 1);
            wink2_stop = 1;
        }
        goto tail;
    }
    if (ending_now_serifu_in == 12) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 7;
            ending_bustup_anim_set(2, 1);
            wink2_stop = 0;
        }
        goto tail;
    }
    if (ending_now_serifu_in == 16) {
        if (ending_serifu_frame == 1) {
            ending_bustup_anim_set(4, 5);
            wink2_stop = 1;
        } else if (ending_serifu_frame == 90) {
            ending_bustup_anim_set(2, 1);
            wink2_stop = 0;
        }
        goto tail;
    }
    if (ending_now_serifu_in == 20) {
        if (ending_serifu_frame == 200) {
            ending_bustup_anim_set(5, 5);
            wink2_stop = 1;
        }
    }

tail:
    g = ending_now_serifu_in;
    if (g >= 12) {
        m = (u32)frame_counter % 240U;
        if (m == 0) {
            if (bustup_cw_anim == 2) {
                bustup_cw_anim = 3;
                bustup_cw_frame = 0;
                class_w[30].tick = 0;
            }
        }
        if (m >= 11U) {
            if (bustup_cw_anim == 3) {
                bustup_cw_anim = 2;
                bustup_cw_frame = 0;
                class_w[30].tick = 0;
                bustup_cw_mode = 1;
            }
        }
    }
    ending_serifu_frame = ending_serifu_frame + 1;
}
