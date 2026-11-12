/* Address: 0x8014FBA4 */
/* Size: 492 bytes (123 insns) */
#include "types.h"

extern u8 *taco_actor_rec;
extern s32 taco_node_key_tbl[][2];
extern u8 taco_node_mode_tbl[];

extern void node_pose_set(s32, s32, s32, s32, s32, s32);

void taco_anim_disp(u32 arg)
{
    s32 i;

    if (arg < (u32)taco_actor_rec[2315]) {
        if (taco_actor_rec[2582] == 1U || taco_actor_rec[2718] == 1U ||
            taco_actor_rec[2718] == 2) {
            for (i = 13; i < 26; i++) {
                node_pose_set(i, taco_actor_rec[2314], taco_node_key_tbl[i - 1][0],
                              taco_node_key_tbl[i - 1][1], taco_node_mode_tbl[(i - 1) * 2],
                              taco_node_mode_tbl[(i - 1) * 2 + 1]);
            }
            for (i = 28; i < 34; i++) {
                node_pose_set(i, taco_actor_rec[2314], taco_node_key_tbl[i - 1][0],
                              taco_node_key_tbl[i - 1][1], taco_node_mode_tbl[(i - 1) * 2],
                              taco_node_mode_tbl[(i - 1) * 2 + 1]);
            }
        } else {
            for (i = 1; i < 34; i++) {
                node_pose_set(i, taco_actor_rec[2314], taco_node_key_tbl[i - 1][0],
                              taco_node_key_tbl[i - 1][1], taco_node_mode_tbl[(i - 1) * 2],
                              taco_node_mode_tbl[(i - 1) * 2 + 1]);
            }
        }
        taco_actor_rec[2314] = (u8)(taco_actor_rec[2314] + 1);
        taco_actor_rec[2315] = 0;
    }

    if ((u32)taco_actor_rec[2314] >= 48U) {
        taco_actor_rec[2314] = 0;
        taco_actor_rec[2315] = taco_actor_rec[2314];
    } else {
        taco_actor_rec[2315] = (u8)(taco_actor_rec[2315] + 1);
    }
}
