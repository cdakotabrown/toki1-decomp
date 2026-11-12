/* Address: 0x801480AC */
/* Size: 72 bytes (18 insns) */
#include "types.h"

extern void taiiku_racer_collide(s32);
extern s32 taiiku_racer_reset(s32);

void taiiku_racers_collide(void) {
    s32 i;

    for (i = 0; i != 4; i++) {
        taiiku_racer_collide(i);
        taiiku_racer_reset(i);
    }
}
