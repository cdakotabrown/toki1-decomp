/* Address: 0x8015C678 */
/* Size: 128 bytes (32 insns) */
#include "types.h"

extern u8 *taco_actor_rec;

void taco_actor_clear(void)
{
    s32 i;

    for (i = 16; i < 63; i++) {
        taco_actor_rec[i * 136 + 133] = 0;
    }
}
