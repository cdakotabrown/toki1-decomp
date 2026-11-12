/* Address: 0x80132220 */
/* Size: 140 bytes (35 insns) */
#include "types.h"

extern u8 *taco_actor_rec;

void taco_actor_reset(void)
{
    *(s16 *)(taco_actor_rec + 116) = 0;
    *(s16 *)(taco_actor_rec + 118) = 0;
    *(s16 *)(taco_actor_rec + 120) = 2048;
    *(s16 *)(taco_actor_rec + 1204) = 0;
    *(s16 *)(taco_actor_rec + 1206) = 0;
    *(s16 *)(taco_actor_rec + 1208) = 2048;
    taco_actor_rec[2] = 0;
    taco_actor_rec[1090] = 0;
    taco_actor_rec[3] = 0;
    *(s16 *)(taco_actor_rec + 104) = 0;
    *(s16 *)(taco_actor_rec + 1192) = 0;
}
