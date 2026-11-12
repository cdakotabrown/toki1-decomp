/* @CompileEnv: IDO52_UGEN_TEMP_REG=3:t9,4:t0,5:t1,6:t2,7:t3,8:t4,9:t5,10:t6,11:t7,12:t8,13:t9,14:t0,15:t1,16:t2,17:t3,18:t4,19:t5,20:t6,21:t7,22:t8,23:t9,24:t0,25:t1,26:t2,27:t3,28:t4,29:t5,30:t6,31:t7,32:t8,33:t9,34:t0,35:t1,36:t2,37:t3,38:t4,39:t5,40:t6,41:t7,42:t8,43:t9,44:t0,45:t1,46:t2,47:t3,48:t4 */
/* Address: 0x8014AB20, 1876 bytes (469 instructions) */
/* @PostProc: patch_insn_reg_field 0x384:rt:v0:v1 0x394:rt:v0:v1 0x440:rt:v0:s0 0x444:rs:v0:s0 0x444:rt:v0:s0 0x458:rs:v0:s0 0x460:rs:v0:s0 0x570:rt:v0:v1 0x574:rt:t5:t6 0x578:rt:v0:v1 0x57c:rs:t5:t6 0x57c:rt:t5:t6 0x584:rs:t5:t6 0x584:rt:v0:v1 0x5a0:rt:t6:t5 0x5a4:rt:t6:t5 0x5c0:rt:v0:v1 0x5cc:rs:v0:v1 0x640:rt:t5:t6 0x644:rt:t6:t5 0x648:rt:t6:t5 0x64c:rt:t5:t6 */
#include "types.h"

extern s16 taco_angle_tbl[][4];

extern s32 taco_rest_key_tbl[][2];
extern u8 taco_rest_mode_tbl[][2];
extern u8 *taco_actor_rec;

extern s32 rand();
extern void sd_cmd_limit_send();
extern void node_pose_set();
extern void taco_pose_keyframe_ease();
extern void taco_node_shot_spawn();
extern void taco_roster_up();
extern void taco_roster_down();
extern void taco_step_timer2();
extern void taco_angles_set();

void
taco_timeline_step(s32 t)
{
    s32 i;
    s32 r0;
    s32 r1;
    u8 *p;
    u8 *q;

    if (t == 10) {
        sd_cmd_limit_send(0, 1280);
    }

    if (t < 100) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 0, 50, 100);
        return;
    }
    if (t == 199) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 1, 40, 40);
        taco_angle_tbl[0][2] = 0;
        taco_angle_tbl[0][0] = 0;
        taco_angle_tbl[0][1] = 0;
        sd_cmd_limit_send(0, 1280);
        return;
    }
    if (t >= 200 && t < 240) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 2, 50, 100);
        return;
    }
    if (t >= 240 && t < 280) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 3, 50, 100);
        return;
    }
    if (t >= 280 && t < 320) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 4, 50, 100);
        return;
    }
    if (t >= 320 && t < 340) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 5, 50, 90);
        return;
    }
    if (t >= 340 && t < 370) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 6, 50, 80);
        return;
    }
    if (t >= 370 && t < 400) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 7, 50, 70);
        return;
    }
    if (t >= 400 && t < 420) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 8, 50, 60);
        return;
    }
    if (t >= 420 && t < 440) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 9, 50, 50);
        return;
    }
    if (t >= 440 && t < 450) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 10, 30, 30);
        return;
    }
    if (t >= 450 && t < 470) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 11, 20, 20);
        return;
    }
    if (t >= 470 && t < 500) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 12, 10, 10);
        return;
    }
    if (t == 500) {
        for (i = 1; i < 31; i++) {
            node_pose_set(i, 1, taco_rest_key_tbl[i - 1][0], taco_rest_key_tbl[i - 1][1],
                          taco_rest_mode_tbl[i - 1][0], taco_rest_mode_tbl[i - 1][1]);
        }
        taco_actor_rec[2854] = 1;
        taco_angles_set(13, 0);
        taco_angles_set(9, 3);
        return;
    }
    if (t >= 500 && t < 515) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 13, 5, 5);
        return;
    }
    if (t >= 515 && t < 555) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 14, 10, 10);
        p = taco_actor_rec;
        if (p[2854] != 0) {
            taco_step_timer2(p + 2587, p + 2586, 13, 0, 3);
            q = taco_actor_rec;
            taco_step_timer2(q + 2451, q + 2450, 9, 3, 3);
        }
        return;
    }
    if (t >= 555 && t < 570) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 15, 5, 100);
        return;
    }
    if (t >= 570 && t < 615) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 13, 5, 100);
        return;
    }
    if (t >= 615 && t < 649) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 16, 20, 200);
        return;
    }
    if (t == 649) {
        for (i = 1; i < 31; i++) {
            node_pose_set(i, 1, taco_rest_key_tbl[i - 1][0], taco_rest_key_tbl[i - 1][1],
                          taco_rest_mode_tbl[i - 1][0], taco_rest_mode_tbl[i - 1][1]);
        }
        taco_actor_rec[2314] = 1;
        taco_actor_rec[2446] = 1;
        return;
    }
    if (t >= 650 && t < 810) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 16, 20, 200);
        if (taco_actor_rec[2446] == 1U) {
            taco_roster_up(2);
        }
        return;
    }
    if (t >= 810 && t < 950) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 16, 20, 100);
        r0 = rand();
        r1 = rand();
        taco_node_shot_spawn(0, 27, (s16)((r0 & 0x3FF) - 512),
                      (s16)((r1 & 0x3FF) - 512), 2000, 400, 129);
        return;
    }
    if (t == 950) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 16, 20, 100);
        for (i = 1; i < 31; i++) {
            node_pose_set(i, 29, taco_rest_key_tbl[i - 1][0], taco_rest_key_tbl[i - 1][1],
                          taco_rest_mode_tbl[i - 1][0], taco_rest_mode_tbl[i - 1][1]);
        }
        taco_actor_rec[2446] = 2;
        return;
    }
    if (t >= 951 && t < 1150) {
        taco_pose_keyframe_ease(0, (void *)0x801E5BD0, 16, 20, 100);
        if (taco_actor_rec[2446] == 2) {
            taco_roster_down(2);
        }
    }
}
