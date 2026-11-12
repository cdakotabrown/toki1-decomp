/* Address: 0x8014C948 */
/* Size: 428 bytes (107 insns) */
#include "types.h"

extern u8 *taco_actor_rec;
extern s32 taco_rest_key_tbl[30][2];
extern u8 taco_rest_mode_tbl[];

extern void taco_cut(s32);
extern void node_pose_set(s32, s32, s32, s32, s32, s32);

void taco_cut_seq(u32 arg)
{
    u32 cur;
    s32 z;
    s32 i;

    cur = taco_actor_rec[3402];
    if (cur < 30U || cur >= 40U) {
        if (arg < (u32)taco_actor_rec[3403]) {
            taco_cut(cur);
            taco_actor_rec[3402] = (u8)(taco_actor_rec[3402] + 1);
            taco_actor_rec[3403] = 0;
            cur = taco_actor_rec[3402];
        }
    } else if ((cur >= 30U) & (cur < 40U)) {
        taco_cut(cur);
        taco_actor_rec[3402] = (u8)(taco_actor_rec[3402] + 1);
        taco_actor_rec[3403] = 0;
        cur = taco_actor_rec[3402];
    }

    if (cur >= 74U) {
        taco_actor_rec[3534] = 0;
        z = taco_actor_rec[3534];
        taco_actor_rec[3402] = z;
        taco_actor_rec[3403] = z;
        for (i = 1; i < 31; i++) {
            node_pose_set(i, 29, taco_rest_key_tbl[i - 1][0], taco_rest_key_tbl[i - 1][1],
                          taco_rest_mode_tbl[(i - 1) * 2],
                          taco_rest_mode_tbl[(i - 1) * 2 + 1]);
        }
    } else {
        taco_actor_rec[3403] = (u8)(taco_actor_rec[3403] + 1);
    }
}
