/* Address: 0x801508A0 */
/* Size: 620 bytes (155 insns) */
#include "types.h"

extern u8 *taco_actor_rec;
extern s32 taco_node_key_tbl[][2];
extern u8 taco_node_mode_tbl[];

extern void taco_row_disp2(s32);
extern void node_pose_set(s32, s32, s32, s32, s32, s32);

void taco_row_step2(void)
{
    u32 cur;
    s32 z;
    s32 i;

    cur = taco_actor_rec[2586];
    if (cur < 28U || cur >= 52U) {
        if ((u32)taco_actor_rec[2587] >= 4U) {
            taco_row_disp2(cur);
            taco_actor_rec[2586] = (u8)(taco_actor_rec[2586] + 1);
            taco_actor_rec[2587] = 0;
            cur = taco_actor_rec[2586];
        }
    } else if (cur >= 28U && cur < 38U) {
        if (taco_actor_rec[2587] != 0) {
            taco_row_disp2(cur);
            taco_actor_rec[2586] = (u8)(taco_actor_rec[2586] + 1);
            taco_actor_rec[2587] = 0;
            cur = taco_actor_rec[2586];
        }
    } else if (cur >= 38U && cur < 52U) {
        if ((u32)taco_actor_rec[2587] >= 3U) {
            taco_row_disp2(cur);
            taco_actor_rec[2586] = (u8)(taco_actor_rec[2586] + 1);
            taco_actor_rec[2587] = 0;
            cur = taco_actor_rec[2586];
        }
    }

    if (cur >= 90U) {
        taco_actor_rec[2718] = 0;
        z = taco_actor_rec[2718];
        taco_actor_rec[2586] = z;
        taco_actor_rec[2587] = z;
        for (i = 7; i < 13; i++) {
            node_pose_set(i, 0, taco_node_key_tbl[i - 1][0], taco_node_key_tbl[i - 1][1],
                          taco_node_mode_tbl[(i - 1) * 2],
                          taco_node_mode_tbl[(i - 1) * 2 + 1]);
        }
        node_pose_set(27, 0, taco_node_key_tbl[26][0], taco_node_key_tbl[26][1],
                      taco_node_mode_tbl[52], taco_node_mode_tbl[53]);
    } else {
        taco_actor_rec[2587] = (u8)(taco_actor_rec[2587] + 1);
    }
}
