/* @SymbolStart: 0x80139EE4 */
/* @SymbolSize: 128 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:a1 0x14:rs:v0:a1 0x14:rt:v0:a1 0x1c:rs:v0:a1 0x1c:rt:v0:a1 0x20:rs:v0:a1 0x3c:rt:v0:a1 0x48:rt:v0:a1 0x50:rt:v0:a1 0x58:rs:v0:a1 */
#include "types.h"
#include "tokimeki/sprite.h"
extern SPRITE_REC Sprites[];

extern void sprite_rgb_set();
extern void sub_sub_step_up();


void ending_fade_in(void)
{
    s32 s0;

    Sprites[4].r += 4;
    if (Sprites[4].r >= 128u) {
        Sprites[4].r = 128;
        sub_sub_step_up();
    }
    s0 = 4;
    do {
        sprite_rgb_set((s32)(s16)s0, (s32)Sprites[4].r);
        s0 = (s16)(s0 + 1);
    } while (s0 < 10);
}
