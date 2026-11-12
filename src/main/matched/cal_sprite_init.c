/* Address: 0x80069384 */
/* Size: 828 bytes (207 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u32 tag;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s16 x2;
    u8  r;
    u8  g;
    s16 b;
    s16 tpage;
    u8  u0;
    u8  u1;
    u8  u2;
    s16 pad0;
    s16 pad1;
    s16 cluts;
    s16 cluth;
    u32 next;
} SpriteRec;

extern SpriteRec Sprites[4];
extern u8 sprite_state;
extern u8 sprite_state1;
extern u8 sprite_state2;
extern u8 sprite_state3;

void cal_sprite_init(void)
{
    Sprites[2].tag   = 0;
    Sprites[2].x0    = -136;
    Sprites[2].y0    = -88;
    Sprites[2].x1    = 126;
    Sprites[2].y1    = 126;
    Sprites[2].x2    = 30;
    Sprites[2].r     = 128;
    Sprites[2].g     = 0;
    Sprites[2].b     = 192;
    Sprites[2].tpage = 480;
    Sprites[2].u0    = 128;
    Sprites[2].u1    = 128;
    Sprites[2].u2    = 128;
    Sprites[2].pad0  = 0;
    Sprites[2].pad1  = 0;
    Sprites[2].cluts = 4096;
    Sprites[2].cluth = 4096;
    Sprites[2].next  = 0;
    sprite_state2 = 132;
    Sprites[3].tag   = 0;
    Sprites[3].x0    = 0;
    Sprites[3].y0    = -88;
    Sprites[3].x1    = 127;
    Sprites[3].y1    = 127;
    Sprites[3].x2    = 30;
    Sprites[3].r     = 128;
    Sprites[3].g     = 128;
    Sprites[3].b     = 192;
    Sprites[3].tpage = 480;
    Sprites[3].u0    = 128;
    Sprites[3].u1    = 128;
    Sprites[3].u2    = 128;
    Sprites[3].pad0  = 0;
    Sprites[3].pad1  = 0;
    Sprites[3].cluts = 4096;
    Sprites[3].cluth = 4096;
    Sprites[3].next  = 0;
    sprite_state3 = 132;
    Sprites[0].tag   = 0;
    Sprites[0].x0    = -88;
    Sprites[0].y0    = -88;
    Sprites[0].x1    = 24;
    Sprites[0].y1    = 16;
    Sprites[0].x2    = 30;
    Sprites[0].r     = 176;
    Sprites[0].g     = 0;
    Sprites[0].b     = 176;
    Sprites[0].tpage = 480;
    Sprites[0].u0    = 128;
    Sprites[0].u1    = 128;
    Sprites[0].u2    = 128;
    Sprites[0].pad0  = 0;
    Sprites[0].pad1  = 0;
    Sprites[0].cluts = 4096;
    Sprites[0].cluth = 4096;
    Sprites[0].next  = 0;
    sprite_state = 132;
    Sprites[1].tag   = 0;
    Sprites[1].x0    = 48;
    Sprites[1].y0    = -88;
    Sprites[1].x1    = 24;
    Sprites[1].y1    = 16;
    Sprites[1].x2    = 30;
    Sprites[1].r     = 176;
    Sprites[1].g     = 128;
    Sprites[1].b     = 176;
    Sprites[1].tpage = 480;
    Sprites[1].u0    = 128;
    Sprites[1].u1    = 128;
    Sprites[1].u2    = 128;
    Sprites[1].pad0  = 0;
    Sprites[1].pad1  = 0;
    Sprites[1].cluts = 4096;
    Sprites[1].cluth = 4096;
    Sprites[1].next  = 0;
    sprite_state1 = 132;
}
