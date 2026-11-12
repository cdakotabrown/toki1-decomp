/* @CompileEnv: IDO52_FORCE_WEB_SPILL=0 IDO52_AS1_TIE_TAKE=31,34,36,63,66,68,103,106,108,139,142,144,248,265,278 */
/* @CompileFlags: -cckr */
/* Address: 0x80150F14 */
#include "types.h"
extern s32 taco_row_ang_tbl[][7];


extern void taco_pose_keyframe_ease(s32 a0, s32 a1, s32 a2, s32 a3, s32 s16);
extern void taco_band_bright(s32 a0, s32 a1);
extern void taco_node_shot_spawn(s32 a0, s32 a1, s32 a2, s32 a3, s32 s16, s32 s20,
                          s32 s24);

taco_row_disp3(sel)
int sel;
{
    s32 *p;
    s32 i;
    s32 n;

    if (sel < 10) {
        p = (&taco_row_ang_tbl[1][0]);
        i = 0;
        do {
            taco_pose_keyframe_ease(i + 7, *p, 1, 0, 0);
            i++;
            p++;
        } while (i != 6);
        taco_pose_keyframe_ease(27, taco_row_ang_tbl[1][6], 1, 0, 0);
    } else if (sel >= 14 && sel < 24) {
        p = (&taco_row_ang_tbl[1][0]);
        i = 0;
        do {
            taco_pose_keyframe_ease(i + 7, *p, 2, 0, 0);
            i++;
            p++;
        } while (i != 6);
        taco_pose_keyframe_ease(27, taco_row_ang_tbl[1][6], 2, 0, 0);
    } else if (sel >= 28 && sel < 60) {
        i = 0;
        do {
            taco_band_bright(i + 7, 7);
            i++;
        } while (i != 6);
        taco_band_bright(27, 7);
    } else if (sel >= 80 && sel < 90) {
        p = (&taco_row_ang_tbl[1][0]);
        i = 0;
        do {
            n = i + 7;
            taco_pose_keyframe_ease(n, *p, 5, 0, 0);
            taco_band_bright(n, -23);
            i++;
            p++;
        } while (i != 6);
        taco_pose_keyframe_ease(27, taco_row_ang_tbl[1][6], 5, 0, 0);
        taco_band_bright(27, -23);
    } else if (sel >= 90 && sel < 100) {
        p = (&taco_row_ang_tbl[1][0]);
        i = 0;
        do {
            taco_pose_keyframe_ease(i + 7, *p, 6, 0, 0);
            i++;
            p++;
        } while (i != 6);
        taco_pose_keyframe_ease(27, taco_row_ang_tbl[1][6], 6, 0, 0);
    }

    if (sel >= 50 && sel < 70) {
        taco_node_shot_spawn(0, 11, 0, 0, 1000, 500, 134);
    }
}
