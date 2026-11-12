/* @CompileEnv: IDO52_AS1_TIE_TAKE=20,23,25,54,57,59,304,307,309,323,326,328,355,358,360,374,377,379,637,640,642,656,659,661,688,691,693,707,710,712,970,973,975,989,992,994,1021,1024,1026,1040,1043,1045,1072,1075,1077,1121,1124,1126 IDO52_COPYPROP_SITE=lod:1:deny,lod:2:deny,lod:12:deny,lod:14:deny,lod:25:deny,lod:27:deny,lod:38:deny,lod:40:deny,lod:42:deny,lod:43:deny IDO52_FORCE_WEB_COLOR=4:s2,7:s1 */
/* Address: 0x8014FFAC */
#include "types.h"

extern s32 taco_rest_mode_tbl[];
extern s32 taco_row_ang_tbl[][7];

extern s32 rand(void);
extern void taco_pose_keyframe_ease();
extern void taco_node_shot_spawn();

void
taco_row_disp(s32 arg0)
{
    s32 *p;
    s32 *q;
    s32 i;
    s32 j;
    s32 cnt;
    s32 r1;
    s32 r2;
    s32 dx;
    s32 dy;

    if (arg0 < 10) {
        p = &taco_rest_mode_tbl[155];
        i = 0;
        do {
            j = i + 1;
            taco_pose_keyframe_ease(j, *p, 1, 0, 0);
            i = j;
            p++;
        } while (j != 6);
        taco_pose_keyframe_ease(26, taco_row_ang_tbl[0][6], 1, 0, 0);
        return;
    }
    if (arg0 >= 14 && arg0 < 24) {
        i = 0;
        p = &taco_rest_mode_tbl[155];
        do {
            j = i + 1;
            taco_pose_keyframe_ease(j, *p, 2, 0, 0);
            i = j;
            p++;
        } while (j != 4);
        return;
    }
    if (arg0 == 27) {
        r1 = rand();
        r2 = rand();
        dx = (s16)((r1 & 0x7F) - 64);
        dy = (s16)((r2 & 0x3F) - 32);
        taco_node_shot_spawn(0, 5, dx, dy, 1000, 400, 193);
        r1 = rand();
        r2 = rand();
        dx = (s16)((r1 & 0x7F) - 64);
        dy = (s16)((r2 & 0x3F) - 32);
        taco_node_shot_spawn(0, 5, dx, dy, 1000, 400, 193);
        r1 = rand();
        r2 = rand();
        dx = (s16)((r1 & 0x7F) - 64);
        dy = (s16)((r2 & 0x3F) - 32);
        taco_node_shot_spawn(0, 5, dx, dy, 1000, 400, 193);
        return;
    }
    if (arg0 >= 28 && arg0 < 30) {
        i = 0;
        p = &taco_rest_mode_tbl[155];
        do {
            j = i + 1;
            taco_pose_keyframe_ease(j, *p, 3, 0, 0);
            i = j;
            p++;
        } while (j < 4);
        q = &taco_rest_mode_tbl[159];
        i = 0;
        do {
            taco_pose_keyframe_ease(i + 5, *q, 2, 0, 0);
            i = i + 1;
            q++;
        } while (i != 2);
        taco_pose_keyframe_ease(26, taco_row_ang_tbl[0][6], 2, 0, 0);
        return;
    }
    if (arg0 >= 30 && arg0 < 34) {
        i = 0;
        p = &taco_rest_mode_tbl[155];
        do {
            j = i + 1;
            taco_pose_keyframe_ease(j, *p, 4, 0, 0);
            i = j;
            p++;
        } while (j < 4);
        q = &taco_rest_mode_tbl[159];
        i = 0;
        cnt = 2;
        do {
            taco_pose_keyframe_ease(i + 5, *q, 3, 0, 0);
            i = i + 1;
            q++;
        } while (i != cnt);
        taco_pose_keyframe_ease(26, taco_row_ang_tbl[0][6], 3, 0, 0);
        return;
    }
    if (arg0 == 34) {
        r1 = rand();
        r2 = rand();
        dx = (s16)((r1 & 0x7F) - 64);
        dy = (s16)((r2 & 0x3F) - 32);
        taco_node_shot_spawn(0, 5, dx, dy, 1000, 400, 193);
        r1 = rand();
        r2 = rand();
        dx = (s16)((r1 & 0x7F) - 64);
        dy = (s16)((r2 & 0x3F) - 32);
        taco_node_shot_spawn(0, 5, dx, dy, 1000, 300, 193);
        r1 = rand();
        r2 = rand();
        dx = (s16)((r1 & 0x7F) - 64);
        dy = (s16)((r2 & 0x3F) - 32);
        taco_node_shot_spawn(0, 5, dx, dy, 1000, 500, 193);
        return;
    }
    if (arg0 >= 35 && arg0 < 37) {
        i = 0;
        p = &taco_rest_mode_tbl[155];
        do {
            j = i + 1;
            taco_pose_keyframe_ease(j, *p, 3, 0, 0);
            i = j;
            p++;
        } while (j < 4);
        q = &taco_rest_mode_tbl[159];
        i = 0;
        do {
            taco_pose_keyframe_ease(i + 5, *q, 2, 0, 0);
            i = i + 1;
            q++;
        } while (i != 2);
        taco_pose_keyframe_ease(26, taco_row_ang_tbl[0][6], 2, 0, 0);
        return;
    }
    if (arg0 >= 37 && arg0 < 41) {
        i = 0;
        p = &taco_rest_mode_tbl[155];
        do {
            j = i + 1;
            taco_pose_keyframe_ease(j, *p, 4, 0, 0);
            i = j;
            p++;
        } while (j < 4);
        q = &taco_rest_mode_tbl[159];
        i = 0;
        cnt = 2;
        do {
            taco_pose_keyframe_ease(i + 5, *q, 3, 0, 0);
            i = i + 1;
            q++;
        } while (i != cnt);
        taco_pose_keyframe_ease(26, taco_row_ang_tbl[0][6], 3, 0, 0);
        return;
    }
    if (arg0 == 41) {
        r1 = rand();
        r2 = rand();
        dx = (s16)((r1 & 0x7F) - 64);
        dy = (s16)((r2 & 0x3F) - 32);
        taco_node_shot_spawn(0, 5, dx, dy, 1000, 400, 193);
        r1 = rand();
        r2 = rand();
        dx = (s16)((r1 & 0x7F) - 64);
        dy = (s16)((r2 & 0x3F) - 32);
        taco_node_shot_spawn(0, 5, dx, dy, 1000, 400, 193);
        r1 = rand();
        r2 = rand();
        dx = (s16)((r1 & 0x7F) - 64);
        dy = (s16)((r2 & 0x3F) - 32);
        taco_node_shot_spawn(0, 5, dx, dy, 1000, 300, 193);
        return;
    }
    if (arg0 >= 42 && arg0 < 44) {
        i = 0;
        p = &taco_rest_mode_tbl[155];
        do {
            j = i + 1;
            taco_pose_keyframe_ease(j, *p, 3, 0, 0);
            i = j;
            p++;
        } while (j < 4);
        q = &taco_rest_mode_tbl[159];
        i = 0;
        do {
            taco_pose_keyframe_ease(i + 5, *q, 2, 0, 0);
            i = i + 1;
            q++;
        } while (i != 2);
        taco_pose_keyframe_ease(26, taco_row_ang_tbl[0][6], 2, 0, 0);
        return;
    }
    if (arg0 >= 44 && arg0 < 49) {
        i = 0;
        p = &taco_rest_mode_tbl[155];
        do {
            j = i + 1;
            taco_pose_keyframe_ease(j, *p, 4, 0, 0);
            i = j;
            p++;
        } while (j < 4);
        q = &taco_rest_mode_tbl[159];
        i = 0;
        cnt = 2;
        do {
            taco_pose_keyframe_ease(i + 5, *q, 3, 0, 0);
            i = i + 1;
            q++;
        } while (i != cnt);
        taco_pose_keyframe_ease(26, taco_row_ang_tbl[0][6], 3, 0, 0);
        return;
    }
    if (arg0 >= 53 && arg0 < 63) {
        i = 0;
        p = &taco_rest_mode_tbl[155];
        do {
            j = i + 1;
            taco_pose_keyframe_ease(j, *p, 5, 0, 0);
            i = j;
            p++;
        } while (j != 4);
        taco_pose_keyframe_ease(6, taco_row_ang_tbl[0][5], 4, 0, 0);
        taco_pose_keyframe_ease(26, taco_row_ang_tbl[0][6], 4, 0, 0);
        return;
    }
    if (arg0 >= 67 && arg0 < 77) {
        i = 0;
        p = &taco_rest_mode_tbl[155];
        do {
            j = i + 1;
            taco_pose_keyframe_ease(j, *p, 6, 0, 0);
            i = j;
            p++;
        } while (j != 4);
        taco_pose_keyframe_ease(5, taco_row_ang_tbl[0][4], 4, 0, 0);
        taco_pose_keyframe_ease(6, taco_row_ang_tbl[0][5], 5, 0, 0);
        taco_pose_keyframe_ease(26, taco_row_ang_tbl[0][6], 5, 0, 0);
    }
}
