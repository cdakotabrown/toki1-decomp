/* Address: 0x80142764 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

typedef struct {
    u8  pad0[536];
    s32 w536[12];
} Rec;

extern Rec *tt_boss_wk;

void tt_shape_clear(void) {
    Rec *s;
    s32 i;

    s = tt_boss_wk;
    for (i = 0; i < 12; i++) {
        s->w536[i] = 0;
    }
}
