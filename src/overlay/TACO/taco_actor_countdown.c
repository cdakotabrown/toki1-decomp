/* @CompileFlags: -cckr */
#include "types.h"

extern u8 *taco_actor_rec;
extern s32 taco_score;
extern s32 taco_hit_score_tbl[];

extern void sd_cmd_limit_send();
extern void taco_spark_spawn();

taco_actor_countdown()
{
    s32 i;
    s16 *q;
    s32 v;

    for (i = 16; i < 64; i++) {
        if ((taco_actor_rec[i * 136 + 1] & 0x80) != 0) {
            if ((taco_actor_rec[i * 136 + 135] & 1) != 0) {
                v = taco_actor_rec[i * 136 + 133];
                if (v != 0) {
                    if (taco_actor_rec[i * 136 + 132] != 0) {
                        if ((u32)v >= 3U) {
                            taco_actor_rec[i * 136 + 133] = (u8)(v - 2);
                        } else {
                            taco_actor_rec[i * 136 + 133] = 0;
                        }
                        q = (s16 *)(taco_actor_rec + i * 136);
                        taco_spark_spawn(q[58], q[59], (s16)(q[60] - 512));
                        if (taco_actor_rec[i * 136 + 133] == 0) {
                            sd_cmd_limit_send(1, 1288);
                            taco_score += taco_hit_score_tbl[taco_actor_rec[i * 136 + 134] % 11U];
                            if (taco_score >= 10000000) {
                                taco_score = 9999999;
                            }
                        }
                    }
                }
            }
        }
    }
}
