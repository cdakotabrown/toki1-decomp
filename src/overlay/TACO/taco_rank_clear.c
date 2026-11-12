/* Address: 0x80141FAC */
/* Size: 208 bytes (52 insns) */
#include "types.h"

extern u8 *taco_actor_rec;

s32 taco_rank_clear(void)
{
    if (*(s16 *)(taco_actor_rec + 128) == 3 || *(s16 *)(taco_actor_rec + 128) == 4) {
        *(s16 *)(taco_actor_rec + 128) = 0;
        return *(s16 *)(taco_actor_rec + 128);
    }
    if (*(s16 *)(taco_actor_rec + 126) == 3 || *(s16 *)(taco_actor_rec + 126) == 4) {
        *(s16 *)(taco_actor_rec + 126) = 0;
        return *(s16 *)(taco_actor_rec + 126);
    }
    if (*(s16 *)(taco_actor_rec + 124) == 3 || *(s16 *)(taco_actor_rec + 124) == 4) {
        *(s16 *)(taco_actor_rec + 124) = 0;
        return *(s16 *)(taco_actor_rec + 124);
    }
    return 0;
}
