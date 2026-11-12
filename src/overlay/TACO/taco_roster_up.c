/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: scheduler_normalizer --block-aware */
/* Address: 0x8014B344 */
/* Size: 320 bytes (80 instructions) */
#include "types.h"

extern u8 *taco_actor_rec;
extern s32 taco_rest_key_tbl[];
extern u8 taco_rest_mode_tbl[];

extern void sd_cmd_limit_send(s32 a0, s32 a1);
extern void node_pose_set(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void taco_roster_up(s32 arg0)
{
    u8 *p0;
    u8 *pl;
    u8 *p2;
    u8 *p3;
    u8 *p4;
    u8 *p5;
    u8 *p6;
    u8 *p7;
    s32 *q1;
    u8 *q2;
    s32 s0;

    p0 = taco_actor_rec;
    if ((u32)arg0 < (u32)p0[2315]) {
        if (p0[2314] == 1) {
            sd_cmd_limit_send(0, 1281);
        }
        s0 = 1;
        q1 = &taco_rest_key_tbl[2];
        q2 = &taco_rest_mode_tbl[2];
        for (; s0 != 31; s0++) {
            pl = taco_actor_rec;
            node_pose_set(s0, pl[2314], q1[-2], q1[-1], q2[-2], q2[-1]);
            q1 += 2;
            q2 += 2;
        }
        p2 = taco_actor_rec;
        p2[2314] = p2[2314] + 1;
        p3 = taco_actor_rec;
        p3[2315] = 0;
    }
    p4 = taco_actor_rec;
    if (!((u32)p4[2314] < 30U)) {
        p4[2446] = 0;
        p5 = taco_actor_rec;
        p5[2315] = p5[2446];
        p6 = taco_actor_rec;
        p6[2314] = 29;
    } else {
        p7 = taco_actor_rec;
        p7[2315] = p7[2315] + 1;
    }
}
