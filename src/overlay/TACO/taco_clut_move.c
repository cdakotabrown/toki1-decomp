/* @SymbolSize: 104 */
/* @PostProc: scheduler_normalizer --store-fp */
#include "types.h"

extern void MoveImage(void *a0, s32 a1, s32 a2);

void taco_clut_move(s32 idx)
{
    s16 desc[4];

    if (idx < 0) {
        idx = 0;
    } else {
        if (idx >= 8) {
            idx = 7;
        }
    }

    desc[0] = (s16)(idx << 3);
    desc[1] = 497;
    desc[2] = 8;
    desc[3] = 1;
    MoveImage(desc, 248, 496);
}
