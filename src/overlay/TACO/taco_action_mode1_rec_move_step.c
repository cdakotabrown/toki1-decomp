/* @SymbolStart: 0x8014D8E8 */
/* @SymbolSize: 340 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* Address: 0x8014D8F0 */
/* Size: 332 bytes (83 insns) */
#include "types.h"

extern u8 *taco_actor_rec;

extern s32 taco_action1_run(s32);
extern void taco_action_set(void);

void taco_action_mode1_rec_move_step(void)
{
    u32 v0;

    v0 = taco_actor_rec[2178];
    if (v0 < 25) {
        goto shared;
    }
    if (v0 < 125) {
        goto run;
    }
shared:
    if ((u32)taco_actor_rec[2179] >= 2) {
        taco_action1_run(v0);
        taco_actor_rec[2178] += 1;
        taco_actor_rec[2179] = 0;
        v0 = taco_actor_rec[2178];
    }
    goto tail;
run:
    if (v0 >= 25 && v0 < 125) {
        taco_action1_run(v0);
        taco_actor_rec[2178] += 1;
        taco_actor_rec[2179] = 0;
        v0 = taco_actor_rec[2178];
    }
tail:
    if (v0 >= 150 || (u32)taco_actor_rec[2446] == 1) {
        taco_actor_rec[2178] = 0;
        taco_actor_rec[2179] = taco_actor_rec[2178];
        taco_action_set();
    } else {
        taco_actor_rec[2179] += 1;
    }
}
