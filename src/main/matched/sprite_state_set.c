/* Address: 0x8005B000 */
/* Size: 240 bytes (60 instructions) */
/* Region: Game */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 byouki_kind;
extern u8 sprite_state;

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
} PrimState8005B000;

extern PrimState8005B000 Sprites;

void sprite_state_set(void) {
    if ((byouki_kind & 0xF) != 3) {
        return;
    }
    Sprites.tag   = 0x01000000u;
    Sprites.x0    = -12;
    Sprites.y0    = -10;
    Sprites.x1    = 30;
    Sprites.y1    = 24;
    Sprites.x2    = 30;
    Sprites.r     = 224;
    Sprites.g     = 96;
    Sprites.b     = 0;
    Sprites.tpage = 495;
    Sprites.u0    = 128;
    Sprites.u1    = 128;
    Sprites.u2    = 128;
    Sprites.pad0  = 0;
    Sprites.pad1  = 0;
    Sprites.cluts = 4096;
    Sprites.cluth = 4096;
    Sprites.next  = 0;
    sprite_state       = 10;
}
