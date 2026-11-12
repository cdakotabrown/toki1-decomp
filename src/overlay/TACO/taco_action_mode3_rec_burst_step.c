/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8014E018 */
/* @SymbolSize: 344 */
/* Address: 0x8014E018 (absorbed cross-prologue head) .. 0x8014E170 */
/* Size: 344 bytes (86 insns) */
#include "types.h"

extern u8 *taco_actor_rec;

extern void taco_action3_run(s32);
extern void taco_action_set(void);

void taco_action_mode3_rec_burst_step(void)
{
    s32 v;

    v = taco_actor_rec[2178];
    if ((u32)v < 20U || (u32)v >= 50U) {
        if ((u32)taco_actor_rec[2179] >= 3U) {
            taco_action3_run(v);
            taco_actor_rec[2178] = taco_actor_rec[2178] + 1;
            taco_actor_rec[2179] = 0;
            v = taco_actor_rec[2178];
        }
    } else if ((u32)v >= 20U && (u32)v < 50U) {
        if (taco_actor_rec[2179] != 0) {
            taco_action3_run(v);
            taco_actor_rec[2178] = taco_actor_rec[2178] + 1;
            taco_actor_rec[2179] = 0;
            v = taco_actor_rec[2178];
        }
    }

    if ((u32)v >= 80U) {
        taco_actor_rec[2178] = 0;
        taco_actor_rec[2179] = taco_actor_rec[2178];
        taco_action_set();
    } else {
        taco_actor_rec[2179] = taco_actor_rec[2179] + 1;
    }
}
