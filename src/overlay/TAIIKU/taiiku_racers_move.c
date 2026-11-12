/* Address: 0x801476BC */
/* Size: 76 bytes (19 insns) */
#include "types.h"

extern void taiiku_cpu(void);
extern s32 taiiku_racer_nudge(s32);
extern s32 taiiku_racer_pos_set2(s32);

void taiiku_racers_move(void) {
    s32 i;

    taiiku_cpu();
    for (i = 0; i != 4; i++) {
        taiiku_racer_nudge(i);
        taiiku_racer_pos_set2(i);
    }
}
