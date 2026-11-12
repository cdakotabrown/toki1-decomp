/* @SymbolSize: 700 */
/* Address: 0x8014DC64 */
/* Size: 700 bytes (175 insns) */
#include "types.h"

extern u8 *taco_actor_rec;
extern u8 taco_main2_hp_tbl[];

extern s32 rand(void);
extern void taco_phase_pan(s32);
extern void taco_action_set(void);
extern s32 get_taco_percent(s32, u8 *);
extern void taco_node_shot_spawn(s32, s32, s16, s16, s32, s32, s32);

void taco_action_mode2_phase_pan_step(void)
{
    s32 r1;
    s32 r2;

    if ((u32)taco_actor_rec[2179] >= 3U) {
        taco_phase_pan(taco_actor_rec[2178]);
        taco_actor_rec[2178] = taco_actor_rec[2178] + 1;
        taco_actor_rec[2179] = 0;
        if ((taco_actor_rec[2178] & 1) == 0) {
            if (taco_actor_rec[2314] == 1) {
                r1 = rand();
                r2 = rand();
                taco_node_shot_spawn(0, 21, (s16)((r1 & 0x1FF) + 244),
                              (s16)((r2 & 0x1FF) - 256), 2000, 250, 130);
                r1 = rand();
                r2 = rand();
                taco_node_shot_spawn(0, 21, (s16)((r1 & 0x1FF) - 756),
                              (s16)((r2 & 0x1FF) - 256), 2000, 250, 130);
            } else if (get_taco_percent(11, taco_main2_hp_tbl) < 50) {
                r1 = rand();
                r2 = rand();
                taco_node_shot_spawn(0, 21, (s16)((r1 & 0x1FF) + 44),
                              (s16)((r2 & 0x1FF) + 744), 2000, 250, 130);
                r1 = rand();
                r2 = rand();
                taco_node_shot_spawn(0, 21, (s16)((r1 & 0x1FF) - 556),
                              (s16)((r2 & 0x1FF) + 744), 2000, 250, 130);
            } else {
                r1 = rand();
                r2 = rand();
                taco_node_shot_spawn(0, 27, (s16)((r1 & 0x3FF) - 512),
                              (s16)((r2 & 0x3FF) - 512), 2000, 400, 129);
            }
        }
    }

    if ((u32)taco_actor_rec[2178] >= 20U) {
        taco_actor_rec[2178] = 0;
        taco_actor_rec[2179] = taco_actor_rec[2178];
        taco_action_set();
    } else {
        taco_actor_rec[2179] = taco_actor_rec[2179] + 1;
    }
}
