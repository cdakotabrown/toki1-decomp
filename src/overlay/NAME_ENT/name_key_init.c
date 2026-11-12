/* name_key_init - NAME_ENT overlay, Address: 0x80139C68, Size: 200 bytes (50 insns) */
#include "types.h"

typedef struct {
    s32 f0;
    s16 f4;
    s16 f6;
    s16 f8;
    s16 f10;
    s16 f12;
    s8  f14;
    s8  f15;
    s16 f16;
    s16 f18;
    u8  f20;
    u8  f21;
    u8  f22;
    s16 f24;
    s16 f26;
    s16 f28;
    s16 f30;
    s32 f32;
} Ent;

extern Ent Sprites[];
extern u32 frame_counter;
extern u8  sprite_state[];

void name_key_init(s32 idx, s32 x, s32 y)
{
    Ent *e;

    e = &Sprites[idx];
    e->f20 = 128;
    e->f21 = 128;
    e->f22 = 128;
    e->f4 = x;
    e->f6 = y;
    e->f0 = 0;
    e->f8 = 16;
    e->f10 = 16;
    e->f14 = ((idx / 8) * 48) + (((frame_counter >> 5) % 3) * 16) + 128;
    e->f15 = (idx & 7) * 16;
    e->f12 = 31;
    e->f16 = idx * 16;
    e->f24 = 8;
    e->f26 = 8;
    e->f18 = 504;
    sprite_state[idx] = 2;
}
