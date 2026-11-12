/* taco_band_step - TACO overlay, Address: 0x8014BCBC, Size: 296 bytes (74 insns) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: scheduler_normalizer --block-aware */
#include "types.h"

extern void taco_band_spawn(s32);
extern s32 node_pose_set(s32, s32, s32, s32, s32, s32);

extern u8 *taco_actor_rec;
extern s32 taco_rest_key_tbl[];
extern u8 taco_rest_mode_tbl[];

void
taco_band_step(s32 a0)
{
    s32 i;
    s32 *w;
    u8 *b;
    s32 v;

    if ((u32)a0 < taco_actor_rec[2859]) {
        taco_band_spawn(taco_actor_rec[2858]);
        taco_actor_rec[2858] += 1;
        taco_actor_rec[2859] = 0;
    }
    if (taco_actor_rec[2858] >= 36U) {
        taco_actor_rec[2990] = 0;
        v = taco_actor_rec[2990];
        taco_actor_rec[2858] = v;
        taco_actor_rec[2859] = v;
        i = 1;
        w = &taco_rest_key_tbl[2];
        b = &taco_rest_mode_tbl[2];
        do {
            node_pose_set(i, 1, w[-2], w[-1], b[-2], b[-1]);
            i++;
            w += 2;
            b += 2;
        } while (i != 4);
    } else {
        taco_actor_rec[2859] += 1;
    }
}
