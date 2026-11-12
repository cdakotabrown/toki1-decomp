/* Address: 0x8014BF08 */
/* Size: 412 bytes (103 insns) */
#include "types.h"

extern u8 *taco_actor_rec;
extern s32 taco_rest_key_tbl[30][2];
extern u8 taco_rest_mode_tbl[];

extern void taco_line_pose_set(s32);
extern void taco_line_pose_set2(s32);
extern void node_pose_set(s32, s32, s32, s32, s32, s32);

void taco_line_seq(u32 arg)
{
    u32 cur;
    s32 flag;
    s32 z;
    s32 i;

    cur = taco_actor_rec[2994];
    if (cur < 10U && arg < (u32)taco_actor_rec[2995]) {
        flag = taco_actor_rec[3262];
        if (flag == 0) {
            taco_line_pose_set(cur);
            cur = taco_actor_rec[2994];
        } else if (flag != 0) {
            taco_line_pose_set2(cur);
            cur = taco_actor_rec[2994];
        }
        taco_actor_rec[2994] = (u8)(cur + 1);
        taco_actor_rec[2995] = 0;
        cur = taco_actor_rec[2994];
    }

    if (cur >= 10U) {
        if (taco_actor_rec[3262] != 0) {
            taco_actor_rec[3262] = 0;
            z = taco_actor_rec[3262];
            taco_actor_rec[3126] = z;
            taco_actor_rec[2994] = z;
            taco_actor_rec[2995] = z;
            for (i = 1; i < 31; i++) {
                node_pose_set(i, 29, taco_rest_key_tbl[i - 1][0],
                              taco_rest_key_tbl[i - 1][1],
                              taco_rest_mode_tbl[(i - 1) * 2],
                              taco_rest_mode_tbl[(i - 1) * 2 + 1]);
            }
        }
    } else {
        taco_actor_rec[2995] = (u8)(taco_actor_rec[2995] + 1);
    }
}
