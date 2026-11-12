/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=1,4 */
/* @PostProc: scheduler_normalizer */
/* Address: 0x80152914 */
#include "types.h"

extern s32 taco_panel_ang_tbl;
extern s32 taco_panel_ang_tbl_3;
extern s32 taco_panel_ang_tbl_4;

extern void taco_pose_keyframe_ease(s32 a0, s32 a1, s32 a2, s32 a3, s32 s16);

void taco_band_row_disp(s32 a0, s32 a1)
{
    s32 *p;
    s32 n;

    if (a1 < 8) {
        p = &taco_panel_ang_tbl;
        n = a0;
        do {
            taco_pose_keyframe_ease(n, p[1], 1, 0, 0);
            n++;
            p++;
        } while (p != &taco_panel_ang_tbl_4);
    } else if (a1 >= 8 && a1 < 16) {
        p = &taco_panel_ang_tbl;
        n = a0;
        do {
            taco_pose_keyframe_ease(n, p[1], 2, 0, 0);
            n++;
            p++;
        } while (p != &taco_panel_ang_tbl_4);
    } else if (a1 >= 16 && a1 < 24) {
        p = &taco_panel_ang_tbl;
        n = a0;
        do {
            taco_pose_keyframe_ease(n, p[1], 3, 0, 0);
            n++;
            p++;
        } while (p != &taco_panel_ang_tbl_4);
    } else if (a1 >= 24 && a1 < 32) {
        p = &taco_panel_ang_tbl;
        n = a0;
        do {
            taco_pose_keyframe_ease(n, p[1], 4, 0, 0);
            n++;
            p++;
        } while (p != &taco_panel_ang_tbl_4);
    } else if (a1 >= 32 && a1 < 40) {
        p = &taco_panel_ang_tbl;
        n = a0;
        do {
            taco_pose_keyframe_ease(n, p[1], 5, 0, 0);
            n++;
            p++;
        } while (p != &taco_panel_ang_tbl_4);
    } else if (a1 >= 40 && a1 < 46) {
        p = &taco_panel_ang_tbl;
        n = a0;
        do {
            taco_pose_keyframe_ease(n, p[1], 6, 0, 0);
            n++;
            p++;
        } while (p != &taco_panel_ang_tbl_4);
    } else if (a1 >= 46 && a1 < 54) {
        p = &taco_panel_ang_tbl;
        n = a0;
        do {
            taco_pose_keyframe_ease(n, p[1], 7, 0, 0);
            n++;
            p++;
        } while (p != &taco_panel_ang_tbl_3);
    } else if (a1 >= 54 && a1 < 62) {
        p = &taco_panel_ang_tbl;
        n = a0;
        do {
            taco_pose_keyframe_ease(n, p[1], 8, 0, 0);
            n++;
            p++;
        } while (p != &taco_panel_ang_tbl_3);
        taco_pose_keyframe_ease(32, taco_panel_ang_tbl_4, 7, 0, 0);
    } else if (a1 >= 62 && a1 < 70) {
        p = &taco_panel_ang_tbl;
        n = a0;
        do {
            taco_pose_keyframe_ease(n, p[1], 9, 0, 0);
            n++;
            p++;
        } while (p != &taco_panel_ang_tbl_3);
        taco_pose_keyframe_ease(32, taco_panel_ang_tbl_4, 8, 0, 0);
    }
}
