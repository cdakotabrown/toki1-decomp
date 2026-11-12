/* Address: 0x8014FD90 */
/* Size: 540 bytes (135 insns) */
#include "types.h"

extern u8 *taco_actor_rec;
extern s32 taco_node_key_tbl[][2];
extern u8 taco_node_mode_tbl[];

extern void taco_row_disp(s32);
extern void node_pose_set(s32, s32, s32, s32, s32, s32);

void taco_row_step(void)
{
    u32 cur;
    s32 z;
    s32 i;

    cur = taco_actor_rec[2450];
    if (cur < 28U || cur >= 30U) {
        if ((u32)taco_actor_rec[2451] >= 4U) {
            taco_row_disp(cur);
            taco_actor_rec[2450] = (u8)(taco_actor_rec[2450] + 1);
            taco_actor_rec[2451] = 0;
            cur = taco_actor_rec[2450];
        }
    } else if ((cur >= 28U && cur < 30U) || (cur >= 35U && cur < 37U) ||
               (cur >= 42U && cur < 44U)) {
        if ((u32)taco_actor_rec[2451] >= 2U) {
            taco_row_disp(cur);
            taco_actor_rec[2450] = (u8)(taco_actor_rec[2450] + 1);
            taco_actor_rec[2451] = 0;
            cur = taco_actor_rec[2450];
        }
    }

    if (cur >= 77U) {
        taco_actor_rec[2582] = 0;
        z = taco_actor_rec[2582];
        taco_actor_rec[2450] = z;
        taco_actor_rec[2451] = z;
        for (i = 1; i < 7; i++) {
            node_pose_set(i, 0, taco_node_key_tbl[i - 1][0], taco_node_key_tbl[i - 1][1],
                          taco_node_mode_tbl[(i - 1) * 2],
                          taco_node_mode_tbl[(i - 1) * 2 + 1]);
        }
        node_pose_set(27, 0, taco_node_key_tbl[26][0], taco_node_key_tbl[26][1],
                      taco_node_mode_tbl[52], taco_node_mode_tbl[53]);
    } else {
        taco_actor_rec[2451] = (u8)(taco_actor_rec[2451] + 1);
    }
}
