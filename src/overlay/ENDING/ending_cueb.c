/* @SymbolStart: 0x8013920C */
/* @SymbolSize: 692 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=50:v1 */
/* ending_cueb - ENDING overlay, Address: 0x80139214, Size: 684 bytes */
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
extern volatile s32 frame_counter;
extern u8  now_sub_sub_step;
extern CLASS_W class_w[];

void ending_cueb(void)
{
    s32 sel;
    u32 m;
    u32 x;

    sel = ending_now_serifu_in;

    if (sel == 1) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 4;
            ending_bustup_anim_set(0, 255);
            sel = ending_now_serifu_in;
            wink2_stop = 0;
        }
        goto gate;
    }
    if (sel == 19) {
        if (ending_serifu_frame == 170) {
            ending_bustup_anim_set(1, 1);
            sel = ending_now_serifu_in;
        }
        goto gate;
    }
    if (sel == 23) {
        if (ending_serifu_frame == 0) {
            ending_bustup_anim_set(0, 0);
            sel = ending_now_serifu_in;
        }
        goto gate;
    }
    if (sel == 32) {
        if (ending_serifu_frame == 0) {
            ending_bustup_anim_set(2, 1);
            sel = ending_now_serifu_in;
        }
        goto gate;
    }
    if (sel == 34) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 5;
        }
        goto gate;
    }
    if (sel == 37) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 4;
            ending_bustup_anim_set(3, 0);
            sel = ending_now_serifu_in;
        }
        goto gate;
    }
    if (sel == 42) {
        if (ending_serifu_frame == 0) {
            ending_bustup_anim_set(0, 0);
            sel = ending_now_serifu_in;
        }
        goto gate;
    }
    if (sel == 44) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 5;
            ending_bustup_anim_set(2, 1);
            sel = ending_now_serifu_in;
        }
    }

gate:
    if (sel < 19) {
        goto chk34;
    }
    if (sel < 23) {
        goto do_divu;
    }
chk34:
    if (sel < 34) {
        goto chk44;
    }
    if (sel < 37) {
        goto do_divu;
    }
chk44:
    if (sel < 44) {
        goto tail;
    }
do_divu:
    x = frame_counter;
    m = x % 240U;
    if (m == 0) {
        if (bustup_cw_anim == 2) {
            bustup_cw_anim = 3;
            bustup_cw_frame = 0;
            class_w[30].tick = 0;
        }
    }
    if (m >= 10U) {
        if (bustup_cw_anim == 3) {
            bustup_cw_anim = 2;
            bustup_cw_frame = 0;
            class_w[30].tick = 0;
            bustup_cw_mode = 1;
        }
    }
tail:
    if (now_sub_sub_step == 2) {
        bustup_cw_anim = 2;
        bustup_cw_mode = 1;
    }
    ending_serifu_frame = ending_serifu_frame + 1;
}
