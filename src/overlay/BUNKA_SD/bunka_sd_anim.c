/* @CompileEnv: IDO52_COPYPROP_VAR=-12 IDO52_COPYPROP_SITE=lod:1,lod:3,lod:9,lod:4,lod:5,lod:6,lod:7,lod:8 IDO52_FORCE_WEB_COLOR=41:a0,35:a1,138:v1,0:t0 IDO52_AS1_TIE_TAKE=307 */
/* @CompileFlags: -cckr */
/* @SymbolStart: 0x80134D94 */
/* @SymbolSize: 960 */
/* bunka_sd_anim - BUNKA_SD overlay, Address: 0x80134D9C (true entry */
#include "types.h"
#include "tokimeki/class_w.h"

extern void _sprite_set_box_shade(s32, s32, s32, s32, s32, s32, s32);
extern void dtd_on_tpage(s32, s32, s32, s32, s32);

extern s32 step_counter;
extern u8  scene_entry_c;
extern u8  step_mode;
extern s32 bunka_sd_kind;

extern u8  cal_cw_bright;
extern u8  cw97_mode;
extern u8  cw97_disp;
extern u8  cw97_bright;
extern s16 cw98_frame;
extern CLASS_W class_w[];

bunka_sd_anim()
{
    s32 mode;
    u32 cnt;
    u32 frame;
    u32 q;
    u32 q2;

    if (step_counter == 0) {
        class_w[99].anim_flag = 5;
    }
    if (step_counter == 254) {
        cw97_disp = 128;
        cw97_mode = 5;
        cw97_bright = 128;
    }
    if (!(cw97_mode & 1)) {
        cw97_disp = 0;
    }
    if (step_counter == 668) {
        class_w[98].disp_flag = 128;
        class_w[98].anim_flag = 5;
        class_w[98].bright = 128;
    }

    if ((u32)step_counter >= 255U && (u32)step_counter < 1443U) {
        cnt = step_mode;
        class_w[99].anim_flag = 64;
        q = cnt / 14;
        if (q >= 2U) {
            cnt = 0;
            q = 0;
        }
        frame = q;
        switch (bunka_sd_kind) {
        case 0:
            class_w[99].frame = (s16)(frame + 93);
            break;
        case 1:
            class_w[99].frame = (s16)(frame + 114);
            break;
        case 2:
            class_w[99].frame = (s16)(frame + 135);
            break;
        case 3:
        default:
            if ((u32)step_counter < 1218U) {
                class_w[99].frame = (s16)(frame + 157);
            } else {
                class_w[99].frame = (s16)(frame + 159);
            }
            break;
        }
        step_mode = (u8)(cnt + 1);
    }

    mode = bunka_sd_kind;
    if (step_counter == 1218) {
        if (mode != 3) {
            class_w[99].disp_flag = 0;
            class_w[100].disp_flag = 128;
            class_w[100].anim_flag = 5;
            class_w[100].bright = 128;
        }
    }
    if (step_counter == 1423) {
        if (mode == 3) {
            class_w[99].disp_flag = 0;
            class_w[100].disp_flag = 128;
            class_w[100].anim_flag = 5;
            class_w[100].bright = 128;
        }
    }
    if (step_counter == 1542 || step_counter == 1545 || step_counter == 1548) {
        _sprite_set_box_shade(-160, -120, 320, 160, 2, 0x00FFFFFF, 129);
        dtd_on_tpage(0, 0, 2, 0, 0);
    }

    mode = bunka_sd_kind;
    if (step_counter == 1542) {
        cw98_frame = 72;
        class_w[98].tick = 0;
    }
    if ((u32)step_counter >= 1551U && (u32)step_counter < 1678U) {
        frame = 1678 - step_counter;
        frame += frame << 8;
        frame += frame << 8;
        _sprite_set_box_shade(-304, -120, 320, 160, 2, frame, 134);
        _sprite_set_box_shade(16, -120, 320, 160, 2, frame, 133);
        dtd_on_tpage(0, 0, 2, 1, 0);
        mode = bunka_sd_kind;
    }
    if (step_counter == 1662) {
        cal_cw_bright = 128;
        class_w[98].bright = 128;
        class_w[100].bright = 128;
    }
    if ((u32)step_counter >= 1827U) {
        if (mode == 3) {
            q = scene_entry_c;
            class_w[100].anim_flag = 64;
            q2 = q / 14;
            if (q2 >= 2U) {
                q = 0;
                q2 = 0;
            }
            frame = q2;
            class_w[100].frame = (s16)(frame + 162);
            scene_entry_c = (u8)(q + 1);
            return frame;
        }
    }
}
