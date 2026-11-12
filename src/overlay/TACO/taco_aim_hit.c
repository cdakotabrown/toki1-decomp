/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=16:v1 */
/* Address: 0x8013CECC, Size: 564 bytes (141 instructions) */
#include "types.h"

extern u8  *taco_actor_rec;
extern s16 taco_throw_tbl;
extern s16 taco_throw_tbl_0_y;
extern s32 taco_score;
extern s32 taco_hit_score_tbl[];

extern s32  abs();
extern void sd_cmd_limit_send();
extern void taco_spark_spawn();

taco_aim_hit()
{
    s32 i;
    s16 *q;
    s32 d0;
    s32 d1;
    s32 hit;
    s32 v;
    s32 ay;
    s32 ax;
    s32 a;
    s32 b;

    ax = taco_throw_tbl;
    ay = taco_throw_tbl_0_y;
    for (i = 16; i < 64; i++) {
        if ((taco_actor_rec[i * 136 + 1] & 0x80) != 0) {
            if ((taco_actor_rec[i * 136 + 135] & 1) != 0) {
                if (taco_actor_rec[i * 136 + 133] != 0) {
                    if (taco_actor_rec[i * 136 + 132] != 0) {
                        q = (s16 *)(taco_actor_rec + i * 136);
                        d0 = abs(ax - q[58]);
                        q = (s16 *)(taco_actor_rec + i * 136);
                        d1 = abs(ay - q[59]);
                        a = 4096 - d0;
                        if (a < 0) {
                            a = 0;
                        }
                        b = 4096 - d1;
                        if (b < 0) {
                            b = 0;
                        }
                        hit = (a + b) / 256;
                        if (hit != 0) {
                            q = (s16 *)(taco_actor_rec + i * 136);
                            taco_spark_spawn(q[58], q[59],
                                          (s16)(q[60] - 512));
                        }
                        v = taco_actor_rec[i * 136 + 133];
                        if ((u32)hit < (u32)v) {
                            taco_actor_rec[i * 136 + 133] = (u8)(v - hit);
                        } else {
                            taco_actor_rec[i * 136 + 133] = 0;
                        }
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
