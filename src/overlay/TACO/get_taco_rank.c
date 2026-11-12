/* Address: 0x8014207C */
/* Size: 116 bytes (29 insns) */
#include "types.h"

extern u8 *taco_actor_rec;

s32 get_taco_rank(void)
{
    if (*(s16 *)(taco_actor_rec + 128) == 3 || *(s16 *)(taco_actor_rec + 128) == 4) {
        return *(s16 *)(taco_actor_rec + 128);
    }
    if (*(s16 *)(taco_actor_rec + 126) == 3 || *(s16 *)(taco_actor_rec + 126) == 4) {
        return *(s16 *)(taco_actor_rec + 126);
    }
    if (*(s16 *)(taco_actor_rec + 124) == 3 || *(s16 *)(taco_actor_rec + 124) == 4) {
        return *(s16 *)(taco_actor_rec + 124);
    }
    return 0;
}
