/* @SymbolStart: 0x8014E474 */
/* @SymbolSize: 340 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* Address: 0x8014E47C */
/* Size: 332 bytes (83 insns) */
#include "types.h"

extern u8 *taco_actor_rec;

extern s32 taco_act5_run(s32);
extern void taco_action_set(void);

void taco_action_mode5_run_step(void)
{
    u32 v0;

    v0 = taco_actor_rec[2178];
    if (v0 < 40) {
        goto shared;
    }
    if (v0 < 70) {
        goto run;
    }
shared:
    if ((u32)taco_actor_rec[2179] >= 3) {
        taco_act5_run(v0);
        taco_actor_rec[2178] += 1;
        taco_actor_rec[2179] = 0;
        v0 = taco_actor_rec[2178];
    }
    goto tail;
run:
    if (v0 >= 40 && v0 < 70) {
        taco_act5_run(v0);
        taco_actor_rec[2178] += 1;
        taco_actor_rec[2179] = 0;
        v0 = taco_actor_rec[2178];
    }
tail:
    if (v0 >= 121 || (u32)taco_actor_rec[2446] == 1) {
        taco_actor_rec[2178] = 0;
        taco_actor_rec[2179] = taco_actor_rec[2178];
        taco_action_set();
    } else {
        taco_actor_rec[2179] += 1;
    }
}
