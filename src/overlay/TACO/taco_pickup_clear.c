/* @CompileFlags: -cckr */
#include "types.h"

typedef struct { s16 vx, vy, vz, pad; } Svec;

typedef struct {
    Svec p;
    u8 pad8[9];
    u8 f11;
    u8 f12;
    u8 f13;
} Item;

extern Item taco_pickup_tbl[16];

void taco_pickup_clear()
{
    s32 i;

    for (i = 0; i < 16; i++) {
        taco_pickup_tbl[i].f13 = 0;
    }
}
