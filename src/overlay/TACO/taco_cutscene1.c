/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=1 */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern u8 ref_view[];
struct cs { s32 cnt; u8 pad[5]; u8 step; };
extern struct cs step_counter;
extern u8 cursor_work_bak[];
extern u8 ovl_work_buf[];
extern s32 taco_cut_key_tbl[];
extern LIBGS_COORD2 taco_coord[];
extern LIBGS_DOBJ2 taco_obj[];

extern void GsSetRefView2(u8 *);
extern void taco_star_disp(u8 *, s32, s32);
extern void taco_pose_keyframe_set(s32, s32, s32);
extern void sd_cmd_limit_send(s32, s32);
extern void sub_sub_step_up(void);

s32 taco_cutscene1(void)
{
    s32 t;
    s32 i;

    (*(LIBGS_RVIEW2 *)ref_view).vrx = 0;
    (*(LIBGS_RVIEW2 *)ref_view).vry = taco_coord[1].coord.t[1];
    t = step_counter.cnt;
    (*(LIBGS_RVIEW2 *)ref_view).vrz = -2000;
    GsSetRefView2(ref_view);
    switch (step_counter.step) {
    case 0:
        taco_star_disp(cursor_work_bak, 0, 10);
        taco_star_disp(ovl_work_buf, 0, 8);
        if (t < 30) {
            for (i = 0; i < 2; i++) {
                taco_pose_keyframe_set(i + 1, taco_cut_key_tbl[i], 1);
            }
        } else if (t >= 30 && t < 90) {
            for (i = 0; i < 2; i++) {
                taco_pose_keyframe_set(i + 1, taco_cut_key_tbl[i], 2);
            }
        } else if (t >= 90 && t < 150) {
            for (i = 0; i < 2; i++) {
                taco_pose_keyframe_set(i + 1, taco_cut_key_tbl[i], 3);
            }
        } else if (t >= 150 && t < 180) {
            for (i = 0; i < 2; i++) {
                taco_pose_keyframe_set(i + 1, taco_cut_key_tbl[i], 4);
            }
        } else if (t >= 180 && t < 210) {
            for (i = 0; i < 2; i++) {
                taco_pose_keyframe_set(i + 1, taco_cut_key_tbl[i], 5);
            }
        } else if (t >= 210 && t < 270) {
            for (i = 0; i < 2; i++) {
                taco_pose_keyframe_set(i + 1, taco_cut_key_tbl[i], 6);
            }
        } else if (t >= 270 && t < 330) {
            for (i = 0; i < 2; i++) {
                taco_pose_keyframe_set(i + 1, taco_cut_key_tbl[i], 7);
            }
        } else if (t >= 330 && t < 360) {
            for (i = 0; i < 2; i++) {
                taco_pose_keyframe_set(i + 1, taco_cut_key_tbl[i], 8);
            }
        } else if (t == 360) {
            step_counter.cnt = 0;
            step_counter.step = step_counter.step + 1;
        }
        if (t == 30) {
            sd_cmd_limit_send(0, 1285);
        }
        if (t == 170) {
            sd_cmd_limit_send(0, 1287);
        }
        if (t == 300) {
            sd_cmd_limit_send(0, 1286);
        }
        break;
    case 1:
        if (t == 1) {
            (*(s32 *)&taco_obj[1].attribute) = 0x80000000;
            (*(s32 *)&taco_obj[2].attribute) = 0x80000000;
        } else if (t >= 60) {
            sub_sub_step_up();
        }
        break;
    }
    step_counter.cnt = step_counter.cnt + 1;
}
