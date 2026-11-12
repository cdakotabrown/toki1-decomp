/* @PostProc: scheduler_normalizer */
/* Address: 0x80150B0C */
#include "types.h"

extern u8 *volatile taco_actor_rec;
extern s32 taco_row_ang_tbl[][7];

extern void taco_pose_keyframe_ease(s32 a0, s32 a1, s32 a2, s32 a3, s32 s16);
extern void taco_band_bright(s32 a0, s32 a1);

void taco_row_disp2(s32 sel)
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
        } while (i != 4);
    } else if (sel >= 28 && sel < 38) {
        p = (&taco_row_ang_tbl[1][0]);
        i = 0;
        do {
            n = i + 7;
            taco_pose_keyframe_ease(n, *p, 3, 0, 0);
            taco_band_bright(n, 23);
            i++;
            p++;
        } while (i != 5);
    } else if (sel >= 42 && sel < 52) {
        p = (&taco_row_ang_tbl[1][0]);
        i = 0;
        do {
            n = i + 7;
            taco_pose_keyframe_ease(n, *p, 4, 0, 0);
            taco_band_bright(n, -23);
            i++;
            p++;
        } while (i != 5);
    } else if (sel >= 56 && sel < 66) {
        p = (&taco_row_ang_tbl[1][0]);
        i = 0;
        do {
            taco_pose_keyframe_ease(i + 7, *p, 5, 0, 0);
            i++;
            p++;
        } while (i != 4);
    } else if (sel >= 70 && sel < 80) {
        p = (&taco_row_ang_tbl[1][0]);
        i = 0;
        do {
            taco_pose_keyframe_ease(i + 7, *p, 6, 0, 0);
            i++;
            p++;
        } while (i != 6);
        taco_pose_keyframe_ease(27, taco_row_ang_tbl[1][6], 6, 0, 0);
    }

    if (sel >= 36 && sel < 44) {
        taco_actor_rec[2721] = 64;
        *(s16 *)(taco_actor_rec + 2840) = 2048;
        taco_actor_rec[2852] = 30;
    } else if (sel == 44) {
        taco_actor_rec[2721] = 128;
    }
}
