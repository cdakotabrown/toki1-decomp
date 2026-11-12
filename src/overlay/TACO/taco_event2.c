/* Address: 0x8014670C */
/* @SymbolStart: 0x80146704 */
/* @SymbolSize: 1540 */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern s32 step_counter;
extern volatile s32 step_counter_v;
extern s32 ref_view;
extern s32 taco_cut_key_tbl_1;
extern u8 cursor_work_bak[];
extern u8 ovl_work_buf[];
extern LIBGS_DOBJ2 taco_obj[];

extern void goto_sub_sub_step(s32 a0);
extern void GsSetRefView2(s32 *p);
extern void sd_cmd_limit_send(s32 a0, s32 a1);
extern void taco_star_disp(void *ptr, s32 a1, s32 a2);
extern void taco_rec_clear(s32 a0);
extern void taco_grad_band_disp(s32 a0, s32 a1, s32 a2, s32 a3, s32 kind, s32 ctx);
extern void taco_pose_keyframe_set(s32 a0, s32 a1, s32 a2);

void
taco_event2(void)
{
    s32 day;
    s32 t8;

    day = step_counter;
    if (day == 1) {
        taco_rec_clear(2);
        taco_pose_keyframe_set(2, taco_cut_key_tbl_1, 9);
        (*(s32 *)&taco_obj[2].attribute) = 0;
        (*(LIBGS_RVIEW2 *)&ref_view).vrz = 0;
        (*(LIBGS_RVIEW2 *)&ref_view).vry = 0;
        (*(LIBGS_RVIEW2 *)&ref_view).vrx = 0;
        GsSetRefView2(&ref_view);
    } else {
        if (day >= 2 && day < 100) {
            taco_pose_keyframe_set(2, taco_cut_key_tbl_1, 10);
        } else if (day >= 101 && day < 120) {
            taco_pose_keyframe_set(2, taco_cut_key_tbl_1, 11);
        } else if (day >= 121 && day < 140) {
            taco_pose_keyframe_set(2, taco_cut_key_tbl_1, 12);
        } else if (day >= 141 && day < 160) {
            taco_pose_keyframe_set(2, taco_cut_key_tbl_1, 13);
        } else if (day >= 161 && day < 180) {
            taco_pose_keyframe_set(2, taco_cut_key_tbl_1, 14);
        } else if (day >= 181 && day < 200) {
            taco_pose_keyframe_set(2, taco_cut_key_tbl_1, 15);
        } else if (day >= 201 && day < 220) {
            taco_pose_keyframe_set(2, taco_cut_key_tbl_1, 16);
        } else if (day >= 221 && day < 250) {
            taco_pose_keyframe_set(2, taco_cut_key_tbl_1, 17);
        } else if (day >= 251 && day < 290) {
            taco_pose_keyframe_set(2, taco_cut_key_tbl_1, 18);
        } else if (day >= 291 && day < 350) {
            taco_pose_keyframe_set(2, taco_cut_key_tbl_1, 19);
        } else if (day >= 351 && day < 390) {
            taco_pose_keyframe_set(2, taco_cut_key_tbl_1, 20);
        } else if (day >= 391 && day < 420) {
            taco_pose_keyframe_set(2, taco_cut_key_tbl_1, 21);
        } else if (day >= 421 && day < 500) {
            taco_pose_keyframe_set(2, taco_cut_key_tbl_1, 22);
        }
    }

    if (day >= 50 && day < 150) {
        taco_grad_band_disp(20, -170, 30, 20, day - 30, 60);
    }
    if (day >= 105 && day < 200) {
        taco_grad_band_disp(170, -170, 80, 8, day - 80, 60);
    }
    if (day >= 120 && day < 220) {
        taco_grad_band_disp(-30, -170, 80, 10, day - 120, 60);
    }
    if (day >= 180 && day < 280) {
        taco_grad_band_disp(200, -170, 50, 15, day - 170, 60);
    }
    if (day >= 220 && day < 320) {
        taco_grad_band_disp(90, -170, 60, 10, day - 210, 60);
    }
    if (day >= 290 && day < 310) {
        taco_grad_band_disp(15, -150, 45, 10, day - 280, 60);
    }
    if (day >= 310 && day < 390) {
        taco_grad_band_disp((day << 1) - 605, -150, 45, 10, day - 280,
                      (day - 310) / 3 + 60);
    }
    if (day >= 400 && day < 500) {
        taco_grad_band_disp(350, -150, 80, 10, day - 390, 120);
    }
    if (day >= 420 && day < 500) {
        taco_grad_band_disp(220, -150, 60, 20, day - 410, 120);
    }
    if (day >= 440 && day < 540) {
        taco_grad_band_disp(150, -150, 40, 20, day - 430, 120);
    }

    if (day == 100) {
        sd_cmd_limit_send(0, 1287);
    }
    if (day == 230) {
        sd_cmd_limit_send(0, 1285);
    }
    if (day == 300) {
        sd_cmd_limit_send(0, 1286);
    }
    if (day == 400) {
        sd_cmd_limit_send(0, 1285);
    }

    if (day < 300) {
        taco_star_disp(cursor_work_bak, -5, -10);
        taco_star_disp(ovl_work_buf, -5, -8);
    } else if (day >= 300 && day < 400) {
        t8 = (day - 300) / 10;
        taco_star_disp(cursor_work_bak, t8 - 5, -10);
        taco_star_disp(ovl_work_buf, t8 - 5, -8);
    } else if (day >= 400) {
        taco_star_disp(cursor_work_bak, 5, -10);
        taco_star_disp(ovl_work_buf, 5, -8);
    }

    if (day == 550) {
        goto_sub_sub_step(4);
    }

    step_counter_v = step_counter_v + 1;
}
