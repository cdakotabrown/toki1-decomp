/* @SymbolStart: 0x8014EE0C */
/* @SymbolSize: 216 */
/* @PostProc: patch_insn_reg_field 0x9c:rt:t6:t7 0xa4:rs:t6:t7 0xa4:rt:t6:t7 0xac:rs:t6:t7 0xac:rt:t7:t6 0xb4:rs:t6:t7 0xb4:rt:t7:t6 */
/* Address: 0x8014EE14 */
#include "types.h"

extern u8 *taco_actor_rec;

extern void taco_cut_run(s32);
extern void taco_action_set(void);

void
taco_action_mode8_cut_run_step(void)
{
    u8 *g;

    g = taco_actor_rec;

    if ((u8)g[2179] >= 3U) {
        taco_cut_run(g[2178]);
        g = taco_actor_rec;
        g[2178] = (u8)g[2178] + 1;
        g = taco_actor_rec;
        g[2179] = 0;
    }
    g = taco_actor_rec;
    if ((u8)g[2178] >= 115U || (u8)g[2446] == 1) {
        g = taco_actor_rec;
        g[2178] = 0;
        g = taco_actor_rec;
        g[2179] = (u8)g[2178];
        taco_action_set();
    } else {
        g = taco_actor_rec;
        g[2179] = (u8)g[2179] + 1;
    }
}
