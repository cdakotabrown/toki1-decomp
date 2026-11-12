/* @PostProc: scheduler_normalizer */
/* Address: 0x80136E80 */
/* Size: 220 bytes (55 insns) */
#include "types.h"

extern void taco_glyph_disp(s32 glyph, s32 pal, s32 mode, s32 scale,
                          s32 x, s32 y, s32 d0, s32 d1, s32 extra);

void glyph_disp(s32 glyph, s32 pal, s32 col, s32 row, s32 depth, s32 extra)
{
    s32 scale;
    s32 x;
    s32 y;

    glyph = glyph & 0xFF;
    if (depth < 6144) {
        scale = (depth << 7) / 4096;
    } else {
        scale = 192;
    }

    x = ((col * 12 - 108) * depth) / 4096;
    y = (((row * 22 - 66) * depth) / 4096) - 20;

    taco_glyph_disp(glyph, pal, 3, scale, x, y, depth, depth, extra);
}
