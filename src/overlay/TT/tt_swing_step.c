/* @SymbolSize: 100 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8  pad000[82];
    u8  b82;
    u8  pad053[855];
    u16 h938;
    u16 h940;
} Serve;

extern Serve *tt_boss_wk;

void tt_swing_step()
{
    Serve *p;

    p = tt_boss_wk;
    p->h938 += p->h940;
    if (p->h938 >= 2048) {
        p->h938 = 0;
    }
    p->b82 = (p->h938 >> 8) + 1;
    if (p->h940 < 256) {
        p->h940++;
    } else {
        p->h940 = 256;
    }
}
