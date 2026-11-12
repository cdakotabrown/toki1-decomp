/* Address: 0x80146124 */
/* @SymbolStart: 0x8014611C */
/* @SymbolSize: 1512 */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern s32 step_counter;
extern volatile s32 step_counter_v;
extern s32 ref_view;
extern s32 taco_cut_key_tbl;
extern u8 cursor_work_bak[];
extern u8 ovl_work_buf[];
extern LIBGS_DOBJ2 taco_obj[];

extern void sub_sub_step_up(void);
extern void GsSetRefView2(s32 *p);
extern void sd_cmd_limit_send(s32 a0, s32 a1);
extern void taco_star_disp(void *ptr, s32 a1, s32 a2);
extern void taco_rec_clear(s32 a0);
extern void taco_grad_band_disp(s32 a0, s32 a1, s32 a2, s32 a3, s32 kind, s32 ctx);
extern void taco_pose_keyframe_set(s32 a0, s32 a1, s32 a2);

void
taco_event1(void)
{
    s32 day;

    day = step_counter;
    if (day == 1) {
        taco_rec_clear(1);
        taco_pose_keyframe_set(1, taco_cut_key_tbl, 9);
        (*(s32 *)&taco_obj[1].attribute) = 0;
        (*(LIBGS_RVIEW2 *)&ref_view).vrz = 0;
        (*(LIBGS_RVIEW2 *)&ref_view).vry = 0;
        (*(LIBGS_RVIEW2 *)&ref_view).vrx = 0;
        GsSetRefView2(&ref_view);
    } else {
        if (day >= 2 && day < 50) {
            taco_pose_keyframe_set(1, taco_cut_key_tbl, 10);
        } else if (day >= 50 && day < 100) {
            taco_pose_keyframe_set(1, taco_cut_key_tbl, 11);
        } else if (day >= 100 && day < 120) {
            taco_pose_keyframe_set(1, taco_cut_key_tbl, 12);
        } else if (day >= 120 && day < 145) {
            taco_pose_keyframe_set(1, taco_cut_key_tbl, 13);
        } else if (day >= 145 && day < 180) {
            taco_pose_keyframe_set(1, taco_cut_key_tbl, 14);
        } else if (day >= 180 && day < 200) {
            taco_pose_keyframe_set(1, taco_cut_key_tbl, 16);
        } else if (day >= 200 && day < 235) {
            taco_pose_keyframe_set(1, taco_cut_key_tbl, 17);
        } else if (day >= 235 && day < 285) {
            taco_pose_keyframe_set(1, taco_cut_key_tbl, 18);
        } else if (day >= 285 && day < 335) {
            taco_pose_keyframe_set(1, taco_cut_key_tbl, 19);
        } else if (day >= 335 && day < 450) {
            taco_pose_keyframe_set(1, taco_cut_key_tbl, 20);
        } else if (day >= 460 && day < 490) {
            taco_pose_keyframe_set(1, taco_cut_key_tbl, 21);
        } else if (day >= 500 && day < 520) {
            taco_pose_keyframe_set(1, taco_cut_key_tbl, 22);
        }
    }

    if (day >= 100 && day < 190) {
        taco_grad_band_disp(0, -65, 50, 17, day - 85, 0);
    }
    if (day >= 180 && day < 280) {
        taco_grad_band_disp(0, -30, 30, 75, day - 170, 0);
    }
    if (day >= 170 && day < 270) {
        taco_grad_band_disp(0, 50, 60, 10, day - 170, 0);
    }
    if (day >= 190 && day < 290) {
        taco_grad_band_disp(0, -80, 10, 80, day - 190, 0);
    }
    if (day >= 235 && day < 335) {
        taco_grad_band_disp(0, -60, 25, 20, day - 225, 0);
    }
    if (day >= 270 && day < 360) {
        taco_grad_band_disp(0, -23, 28, 30, day - 250, 0);
    }
    if (day >= 285 && day < 385) {
        taco_grad_band_disp(0, 20, 35, 40, day - 275, 0);
    }
    if (day >= 310 && day < 410) {
        taco_grad_band_disp(0, 60, 45, 30, day - 300, 0);
    }
    if (day >= 340 && day < 440) {
        taco_grad_band_disp(0, -80, 60, 20, day - 320, 0);
    }
    if (day >= 360 && day < 460) {
        taco_grad_band_disp(0, 0, 20, 30, day - 360, 0);
    }
    if (day >= 390 && day < 490) {
        taco_grad_band_disp(0, 40, 50, 30, day - 390, 0);
    }
    if (day >= 430 && day < 530) {
        taco_grad_band_disp(0, -90, 20, 30, day - 430, 0);
    }
    if (day >= 460 && day < 560) {
        taco_grad_band_disp(0, -40, 40, 30, day - 460, 0);
    }
    if (day >= 480 && day < 580) {
        taco_grad_band_disp(0, 50, 60, 5, day - 480, 0);
    }

    if (day == 10) {
        sd_cmd_limit_send(0, 1285);
    }
    if (day == 100) {
        sd_cmd_limit_send(0, 1286);
    }
    if (day == 220) {
        sd_cmd_limit_send(0, 1286);
    }
    if (day == 400) {
        sd_cmd_limit_send(0, 1287);
    }
    if (day == 480) {
        sd_cmd_limit_send(0, 1285);
    }

    if (day < 580) {
        taco_star_disp(cursor_work_bak, -10, 0);
        taco_star_disp(ovl_work_buf, -8, 0);
    }

    if (day == 640) {
        (*(s32 *)&taco_obj[1].attribute) = 0x80000000;
        sub_sub_step_up();
    }

    step_counter_v = step_counter_v + 1;
}
