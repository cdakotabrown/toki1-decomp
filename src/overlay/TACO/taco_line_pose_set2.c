/* @PostProc: scheduler_normalizer */
/* Address: 0x8014C1AC */
/* Size: 264 bytes (66 insns) */
#include "types.h"

extern s32 taco_line_ang_tbl[];
extern s32 taco_line_ang_tbl_5[];
extern s32 taco_line_ang_tbl_14[];
extern s32 taco_line_ang_tbl_24[];

extern void taco_pose_keyframe_ease(s32, s32, s32, s32, s32);

void taco_line_pose_set2(s32 arg0)
{
    s32 i;

    if (arg0 < 10) {
        i = 0;
        do {
            taco_pose_keyframe_ease(i + 1, taco_line_ang_tbl[i], 2, 0, 0);
            i++;
        } while (i < 5);
        i = 0;
        do {
            taco_pose_keyframe_ease(i + 19, taco_line_ang_tbl_5[i], 2, 0, 0);
            i++;
        } while (i < 9);
        i = 0;
        do {
            taco_pose_keyframe_ease(i + 7, taco_line_ang_tbl_14[i], 2, 0, 0);
            i++;
        } while (i < 10);
        i = 0;
        do {
            taco_pose_keyframe_ease(i + 29, taco_line_ang_tbl_24[i], 2, 0, 0);
            i++;
        } while (i != 2);
    }
}
