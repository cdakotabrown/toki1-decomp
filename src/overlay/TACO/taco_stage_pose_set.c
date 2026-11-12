/* @PostProc: scheduler_normalizer */
/* Address: 0x8014C548 */
/* Size: 868 bytes (217 insns) */
#include "types.h"

extern s32 taco_stage_ang_tbl[];

extern void taco_pose_keyframe_ease(s32, s32, s32, s32, s32);
extern void taco_node_shot_spawn(s32, s32, s32, s32, s32, s32, s32);
extern void sd_cmd_limit_send(s32, s32);

void taco_stage_pose_set(s32 arg0)
{
    s32 *p;
    s32 i;

    if (arg0 < 10) {
        taco_pose_keyframe_ease(18, taco_stage_ang_tbl[0], 1, 0, 0);
        p = (&taco_stage_ang_tbl[1]);
        i = 0;
        do {
            taco_pose_keyframe_ease(i + 25, *p, 1, 0, 0);
            i++;
            p++;
        } while (i < 2);
        taco_pose_keyframe_ease(8, taco_stage_ang_tbl[3], 1, 0, 0);
        p = (&taco_stage_ang_tbl[4]);
        i = 0;
        do {
            taco_pose_keyframe_ease(i + 4, *p, 1, 0, 0);
            i++;
            p++;
        } while (i != 2);
    } else if (arg0 >= 16 && arg0 < 18) {
        taco_pose_keyframe_ease(18, taco_stage_ang_tbl[0], 2, 0, 0);
        p = (&taco_stage_ang_tbl[1]);
        i = 0;
        do {
            taco_pose_keyframe_ease(i + 25, *p, 2, 0, 0);
            i++;
            p++;
        } while (i < 2);
        taco_pose_keyframe_ease(8, taco_stage_ang_tbl[3], 2, 0, 0);
        p = (&taco_stage_ang_tbl[4]);
        i = 0;
        do {
            taco_pose_keyframe_ease(i + 4, *p, 2, 0, 0);
            i++;
            p++;
        } while (i != 2);
    } else if (arg0 >= 18 && arg0 < 28) {
        taco_pose_keyframe_ease(18, taco_stage_ang_tbl[0], 3, 0, 0);
        p = (&taco_stage_ang_tbl[1]);
        i = 0;
        do {
            taco_pose_keyframe_ease(i + 25, *p, 3, 0, 0);
            i++;
            p++;
        } while (i < 2);
        taco_pose_keyframe_ease(8, taco_stage_ang_tbl[3], 3, 0, 0);
        p = (&taco_stage_ang_tbl[4]);
        i = 0;
        do {
            taco_pose_keyframe_ease(i + 4, *p, 3, 0, 0);
            i++;
            p++;
        } while (i != 2);
    } else if (arg0 >= 32 && arg0 < 42) {
        taco_pose_keyframe_ease(18, taco_stage_ang_tbl[0], 4, 0, 0);
        p = (&taco_stage_ang_tbl[1]);
        i = 0;
        do {
            taco_pose_keyframe_ease(i + 25, *p, 4, 0, 0);
            i++;
            p++;
        } while (i != 2);
        taco_pose_keyframe_ease(8, taco_stage_ang_tbl[3], 4, 0, 0);
    }

    if (arg0 >= 16 && arg0 < 28) {
        taco_node_shot_spawn(0, 18, 0, 0, 5000, 800, 132);
    }
    if (arg0 == 1) {
        sd_cmd_limit_send(0, 1282);
    } else if (arg0 == 16) {
        sd_cmd_limit_send(0, 1283);
    }
}
