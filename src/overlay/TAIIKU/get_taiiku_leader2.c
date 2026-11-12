/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: remap_stack_slots --from 32 --to 28 --from 34 --to 30 --from 36 --to 32 --from 38 --to 34 */
/* Address: 0x8014001C */
/* Size: 172 bytes (43 insns) */
#include "types.h"

extern s16 taiiku_tama_cpu_pos;

s32 get_taiiku_leader2(void)
{
    s16 stack[3];
    s32 i;
    s16 v;

    for (i = 0; i < 3; i++) {
        v = (&taiiku_tama_cpu_pos)[i * 2 + 1];
        if (v < 3584) {
            stack[i] = v;
        } else {
            stack[i] = -1;
        }
    }

    if (stack[0] < stack[1]) goto max_v1_v2;
    if (stack[0] < stack[2]) goto max_v1_v2;
    *(&taiiku_tama_cpu_pos + 1) = -1;
    return 1;
max_v1_v2:
    if (stack[1] >= stack[2]) {
        *(&taiiku_tama_cpu_pos + 3) = -1;
        return 2;
    }
    *(&taiiku_tama_cpu_pos + 5) = -1;
    return 3;
}
