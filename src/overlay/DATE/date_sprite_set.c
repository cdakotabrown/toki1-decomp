/* Address: 0x801491D8 */
/* Size: 160 bytes (40 insns) */
#include "types.h"

typedef struct { s32 w[9]; } Spr;
extern void date_sprite_init(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern u8 screen_w[];
extern Spr Sprites[];

s32 date_sprite_set(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    short i;

    date_sprite_init(a0, a1, a2, a3, a4, a5);
    for (i = 0; i < 6; i++) {
        Sprites[i + 4] = Sprites[i + 58];
        screen_w[i + 4265] = screen_w[i + 4319];
    }
}
