/* @SymbolStart: 0x8014D384 */
/* @SymbolSize: 380 */
/* Address: 0x8014D384 (absorbed cross-prologue head) .. 0x8014D500 */
/* Size: 380 bytes (95 insns) */
#include "types.h"

extern u8 *taco_actor_rec;

extern s32 rand(void);
extern void taco_node_shot_spawn(s32, s32, s32, s32, s32, s32, s32);
extern void taco_offset_set(s32, s32);
extern void taco_action_set(void);

void taco_action_mode0_sweep_step(s32 arg0)
{
    if ((u32)taco_actor_rec[2179] >= 3U) {
        taco_offset_set(taco_actor_rec[2178], arg0);
        taco_actor_rec[2178] = taco_actor_rec[2178] + 1;
        taco_actor_rec[2179] = 0;
        if ((taco_actor_rec[2178] & 1) == 0) {
            taco_node_shot_spawn(0, 27, 500,
                          (s16)((rand() & 0xFF) - 128),
                          2000, 300, 0);
            taco_node_shot_spawn(0, 27, -500,
                          (s16)((rand() & 0xFF) - 128),
                          2000, 300, 0);
        }
    }

    if ((u32)taco_actor_rec[2178] >= 60U || rand() % 200 == 0) {
        taco_actor_rec[2178] = 0;
        taco_actor_rec[2179] = taco_actor_rec[2178];
        taco_action_set();
    } else {
        taco_actor_rec[2179] = taco_actor_rec[2179] + 1;
    }
}
