/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: patch_stack_complete --old 64 --new 80 */
/* @SymbolStart: 0x8014EC1C */
/* @SymbolSize: 384 */
/* Address: 0x8014EC1C (absorbed cross-prologue head) .. 0x8014ED9C */
/* Size: 384 bytes (96 insns) */
#include "types.h"

extern u8 *taco_actor_rec;

extern s32 rand(void);
extern void taco_node_shot_spawn(s32, s32, s32, s32, s32, s32, s32);
extern void taco_band_run(s32);
extern void taco_action_set(void);

void taco_action_mode7_band_shot_step(void)
{
    s32 x;
    s32 y;

    if ((u32)taco_actor_rec[2179] >= 3U) {
        taco_band_run(taco_actor_rec[2178]);
        taco_actor_rec[2178] = taco_actor_rec[2178] + 1;
        taco_actor_rec[2179] = 0;

        x = rand();
        y = rand();
        taco_node_shot_spawn(0, 27, (s16)((x & 0x3FF) - 312),
                      (s16)((y & 0x1FF) - 512), 2000, 250, 64);

        x = rand();
        y = rand();
        taco_node_shot_spawn(0, 27, (s16)((x & 0x3FF) - 712),
                      (s16)((y & 0x1FF) - 512), 2000, 250, 64);
    }

    if ((u32)taco_actor_rec[2178] >= 50U || taco_actor_rec[2446] == 1) {
        taco_actor_rec[2178] = 0;
        taco_actor_rec[2179] = taco_actor_rec[2178];
        taco_action_set();
    } else {
        taco_actor_rec[2179] = taco_actor_rec[2179] + 1;
    }
}
