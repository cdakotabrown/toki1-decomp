/* @SymbolSize: 196 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* Address: 0x8015C218 */
/* Size: 196 bytes (49 insns) */
#include "types.h"

extern u8 *taco_actor_rec;
extern u8 taco_node_own[];
extern u8 taco_kind_mdl_tbl[];

extern void taco_actor_disp_switch(s32 a0);

void taco_actor_exit(s32 id)
{
    u8 *p;

    taco_actor_disp_switch(0);

    taco_actor_rec[id * 136 + 1] = 0;
    taco_actor_rec[id * 136 + 134] = 0;

    p = taco_node_own;
    do {
        if (id == p[0]) {
            p[0] = 0;
        }
        if (id == p[1]) {
            p[1] = 0;
        }
        if (id == p[2]) {
            p[2] = 0;
        }
        if (id == p[3]) {
            p[3] = 0;
        }
        p += 4;
    } while (p != taco_kind_mdl_tbl);
}
