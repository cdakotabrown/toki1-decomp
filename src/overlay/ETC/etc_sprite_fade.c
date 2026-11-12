/* Size: 152 bytes */
/* @SymbolSize: 152 */
/* @PostProc: patch_insn_reg_field 0x60:rt:a1:v0 0x64:rs:a1:v0 0x64:rt:a1:v0 0x7c:rt:v0:a1 0x80:rt:v0:a1 0x84:rt:v0:a1 0x8c:rt:v0:a1 0x98:rt:a1:v0 */
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
extern s32 etc_fade_dir;

void etc_sprite_fade(void)
{
    s32 dir;
    s32 i;
    s32 c;

    dir = etc_fade_dir;
    if (dir > 0) {
        for (i = 0; i < 2; i++) {
            c = Sprites[i].b20;
            if ((u32)c < 128U) {
                Sprites[i].b20 = c + 1;
                Sprites[i].b22 = c + 1;
                Sprites[i].b21 = c + 1;
            }
        }
    }
    if (dir < 0) {
        for (i = 0; i < 2; i++) {
            c = Sprites[i].b20;
            if (c != 0) {
                Sprites[i].b20 = c - 1;
                Sprites[i].b22 = c - 1;
                Sprites[i].b21 = c - 1;
            } else {
                Sprites[i].mode = 0x80000000;
            }
        }
    }
}
