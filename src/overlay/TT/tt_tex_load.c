/* @SymbolSize: 68 */
/* @PostProc: scheduler_normalizer */
/* Address: 0x80142D98, Size: 68 bytes (17 insns) */
#include "types.h"

extern void LoadImage(s16 *rect, s32 color);

void tt_tex_load(s32 color, s16 x, s16 y)
{
    s16 rect[4];

    rect[0] = x;
    rect[1] = y;
    rect[2] = 32;
    rect[3] = 128;

    LoadImage(rect, color);
}
