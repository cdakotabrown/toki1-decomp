/* @SymbolSize: 352 */
/* Address: 0x80143FE4 */
/* Size: 352 bytes (88 instructions) */
#include "types.h"
#include "tokimeki/sprite.h"

typedef struct {
    s32 f0;
    s16 f4;
    s16 f6;
    s16 f8;
    s16 f10;
    s16 f12;
    s8 f14;
    s8 f15;
    s16 f16;
    s16 f18;
    s8 f20;
    s8 f21;
    s8 f22;
    s16 f24;
    s16 f26;
    s16 f28;
    s16 f30;
    s32 f32;
} Rec;

extern s32 rand(void);

extern u8 sprite_state2;
extern u8 sprite_state3;
extern u8 sprite_state4;
extern SPRITE_REC Sprites[];

void etc_sprite_init(s32 arg)
{
    Rec *r;
    s32 i;
    s32 x;
    s32 d;

    r = ((Rec *)&Sprites[2].attribute);

    d = 112;
    for (i = 2, x = -176; i != 5; i++, r++, x += 108) {
        r->f0 = 0;
        r->f4 = x;
        r->f6 = (rand() % d) - 120;
        r->f8 = 128;
        r->f10 = 128;
        r->f12 = i + 3;
        r->f14 = 0;
        r->f15 = 0;
        r->f16 = 0;
        r->f18 = 497;
        r->f20 = 0;
        r->f21 = 0;
        r->f22 = 0;
        r->f24 = 0;
        r->f26 = 0;
        r->f28 = 4096;
        r->f30 = 4096;
        r->f32 = 0;
    }

    sprite_state2 = 255;
    sprite_state3 = 254;
    sprite_state4 = 255;

    if (arg == 14) {
        ((Rec *)&Sprites[2].attribute)[0].f6 = -24;
        ((Rec *)&Sprites[2].attribute)[1].f6 = -100;
        ((Rec *)&Sprites[2].attribute)[2].f6 = -40;
    }
}
