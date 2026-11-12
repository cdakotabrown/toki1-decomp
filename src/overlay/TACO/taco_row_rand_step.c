/* @PostProc: patch_insn_reg_field 0x304:rt:v0:v1 0x30c:rs:v0:v1 0x328:rt:v0:v1 0x334:rs:v0:v1 */
/* Address: 0x8014F270 */
/* Size: 848 bytes (212 insns) */
#include "types.h"

extern u8 *taco_actor_rec;
extern s32 taco_node_key_tbl[][2];
extern u8 taco_node_mode_tbl[];

extern void taco_anim(s32);
extern void taco_roll_step(void);
extern s32 rand(void);
extern void taco_angle_row_set(s32, s32);
extern void taco_row_step(void);
extern void taco_row_step2(void);
extern void taco_row_step3(void);
extern void node_pose_set(s32, s32, s32, s32, s32, s32);

void taco_row_rand_step(void)
{
    s32 r;
    s32 i;

    taco_anim(5);
    taco_roll_step();

    if ((u32)taco_actor_rec[2722] < 75U && taco_actor_rec[2582] == 0 &&
        taco_actor_rec[2717] != 0) {
        if (rand() % 200 == 0) {
            taco_actor_rec[2582] = 1;
            for (i = 1; i < 7; i++) {
                node_pose_set(i, 0, taco_node_key_tbl[i - 1][0],
                              taco_node_key_tbl[i - 1][1], taco_node_mode_tbl[(i - 1) * 2],
                              taco_node_mode_tbl[(i - 1) * 2 + 1]);
            }
            node_pose_set(26, 0, taco_node_key_tbl[25][0], taco_node_key_tbl[25][1],
                          taco_node_mode_tbl[50], taco_node_mode_tbl[51]);
            taco_angle_row_set(1, 0);
        }
    }

    if ((u32)taco_actor_rec[2722] < 75U && taco_actor_rec[2718] == 0 &&
        taco_actor_rec[2853] != 0) {
        r = rand() % 200;
        if (r == 0) {
            taco_actor_rec[2718] = 1;
            for (i = 7; i < 13; i++) {
                node_pose_set(i, 0, taco_node_key_tbl[i - 1][0],
                              taco_node_key_tbl[i - 1][1], taco_node_mode_tbl[(i - 1) * 2],
                              taco_node_mode_tbl[(i - 1) * 2 + 1]);
            }
            node_pose_set(27, 0, taco_node_key_tbl[26][0], taco_node_key_tbl[26][1],
                          taco_node_mode_tbl[52], taco_node_mode_tbl[53]);
            taco_angle_row_set(7, 7);
        } else if (r == 1) {
            taco_actor_rec[2718] = 2;
            for (i = 7; i < 13; i++) {
                node_pose_set(i, 0, taco_node_key_tbl[i - 1][0],
                              taco_node_key_tbl[i - 1][1], taco_node_mode_tbl[(i - 1) * 2],
                              taco_node_mode_tbl[(i - 1) * 2 + 1]);
            }
            node_pose_set(27, 0, taco_node_key_tbl[26][0], taco_node_key_tbl[26][1],
                          taco_node_mode_tbl[52], taco_node_mode_tbl[53]);
            taco_angle_row_set(7, 7);
        }
    }

    if (taco_actor_rec[2582] != 0) {
        taco_row_step();
    }
    if (taco_actor_rec[2718] == 1) {
        taco_row_step2();
    }
    if (taco_actor_rec[2718] == 2) {
        taco_row_step3();
    }
}
