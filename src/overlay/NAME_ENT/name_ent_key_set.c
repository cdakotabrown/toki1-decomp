/* name_ent_key_set - NAME_ENT overlay, Address: 0x801366FC, Size: 60 bytes (15 insns) */
#include "types.h"

typedef struct {
    u8  b0[2];
    u8  f2;
    u8  b3[21];
    s16 f24;
    u8  b26[42];
} Rec68;

extern Rec68 class_w[];

void name_ent_key_set(s32 idx)
{
    Rec68 *p;

    p = &class_w[idx];
    if ((p->f2 & 1) == 0) {
        p->f24 = 0;
        p->f2 = 1;
    }
}
