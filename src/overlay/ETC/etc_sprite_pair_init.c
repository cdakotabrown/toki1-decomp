/* Size: 156 bytes */
/* @SymbolSize: 156 */
#include "types.h"

typedef struct {
    u32 mode;
    s16 x;
    s16 y;
    s16 w;
    s16 h;
    s16 prio;
    u8  b14;
    u8  b15;
    s16 h16;
    s16 h18;
    u8  b20;
    u8  b21;
    u8  b22;
    u8  pad23;
    s16 h24;
    s16 h26;
    s16 h28;
    s16 h30;
    u32 w32;
} Spr;

extern Spr Sprites[2];
extern u8 screen_w[];

void etc_sprite_pair_init(void)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        Sprites[i].x = i * 128 - 96;
        Sprites[i].mode = 0x01000000;
        Sprites[i].y = -64;
        Sprites[i].w = 128;
        Sprites[i].h = 128;
        Sprites[i].prio = i + 21;
        Sprites[i].b14 = 0;
        Sprites[i].b15 = 0;
        Sprites[i].h16 = 0;
        Sprites[i].h18 = 511;
        Sprites[i].b20 = 0;
        Sprites[i].b21 = 0;
        Sprites[i].b22 = 0;
        Sprites[i].h24 = 0;
        Sprites[i].h26 = 0;
        Sprites[i].h28 = 4096;
        Sprites[i].h30 = 4096;
        Sprites[i].w32 = 0;
        screen_w[i + 4261] = 0;
    }
}
