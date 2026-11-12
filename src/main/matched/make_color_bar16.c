/* Address: 0x8004BF08 */
/* Size: 1540 bytes (385 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=248:t8,257:t8,286:t7,289:t7,293:t6 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u16 r:5;
    u16 g:5;
    u16 b:5;
    u16 stp:1;
} Pix;

void make_color_bar16(dest, c1, c2, width, stp)
Pix *dest;
u32 c1;
u32 c2;
s32 width;
s32 stp;
{
    Pix *p;
    s32 i;
    s32 r1, r2, g1, g2, b1, b2;

    p = dest;
    r1 = (c1 >> 16) & 0xFF;
    g1 = (c1 >> 8) & 0xFF;
    b1 = c1 & 0xFF;
    r2 = (c2 >> 16) & 0xFF;
    g2 = (c2 >> 8) & 0xFF;
    b2 = c2 & 0xFF;
    for (i = 0; i < width / 2; i++) {
        p[i * 2 + 1].r = ((width - (i * 2 + 1)) * r2 + (i * 2 + 1) * r1) / width / 8;
        p[i * 2].r = ((width - i * 2) * r2 + i * 2 * r1) / width / 8;
        p[i * 2 + 1].g = ((width - (i * 2 + 1)) * g2 + (i * 2 + 1) * g1) / width / 8;
        p[i * 2].g = ((width - i * 2) * g2 + i * 2 * g1) / width / 8;
        p[i * 2 + 1].b = ((width - (i * 2 + 1)) * b2 + (i * 2 + 1) * b1) / width / 8;
        p[i * 2].b = ((width - i * 2) * b2 + i * 2 * b1) / width / 8;
    }
    if (stp != 0) {
        for (i = 1; i < width / 2; i++) {
            p[i * 2 + 1].stp = 1;
            p[i * 2].stp = 1;
        }
        p[1].stp = 1;
    } else {
        for (i = 0; i < width / 2; i++) {
            p[i * 2 + 1].stp = 0;
            p[i * 2].stp = 0;
        }
    }
    p[0].stp = 0;
    p[0].r = 0;
    p[0].g = 0;
    p[0].b = 0;
}
