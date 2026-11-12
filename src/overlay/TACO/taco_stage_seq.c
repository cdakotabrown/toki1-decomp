/* Address: 0x8014C384 */
/* Size: 452 bytes (113 insns) */
#include "types.h"

extern u8 *taco_actor_rec;
extern s32 taco_rest_key_tbl[30][2];
extern u8 taco_rest_mode_tbl[];

extern void taco_stage_pose_set(s32);
extern void node_pose_set(s32, s32, s32, s32, s32, s32);

void taco_stage_seq(u32 arg)
{
    u32 cur;
    s32 z;
    s32 i;

    cur = taco_actor_rec[3266];
    if (cur < 16U || cur >= 18U) {
        if (arg < (u32)taco_actor_rec[3267]) {
            taco_stage_pose_set(cur);
            taco_actor_rec[3266] = (u8)(taco_actor_rec[3266] + 1);
            taco_actor_rec[3267] = 0;
            cur = taco_actor_rec[3266];
        }
    } else if (cur >= 16U && cur < 18U) {
        if (taco_actor_rec[3267] != 0) {
            taco_stage_pose_set(cur);
            taco_actor_rec[3266] = (u8)(taco_actor_rec[3266] + 1);
            taco_actor_rec[3267] = 0;
            cur = taco_actor_rec[3266];
        }
    }

    if (cur >= 42U) {
        taco_actor_rec[3398] = 0;
        z = taco_actor_rec[3398];
        taco_actor_rec[3266] = z;
        taco_actor_rec[3267] = z;
        for (i = 1; i < 31; i++) {
            node_pose_set(i, 29, taco_rest_key_tbl[i - 1][0], taco_rest_key_tbl[i - 1][1],
                          taco_rest_mode_tbl[(i - 1) * 2],
                          taco_rest_mode_tbl[(i - 1) * 2 + 1]);
        }
    } else {
        taco_actor_rec[3267] = (u8)(taco_actor_rec[3267] + 1);
    }
}
