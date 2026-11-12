/* `@SymbolStart: 0x8004E930`, which test_single_match honours but the */
/* Debug Symbol: vram_rect_clear (from o.bin, high match) */
/* Address: 0x8004E930 - 0x8004E9BC */
/* Size: 140 bytes (35 instructions) */
/* Compiler: IDO 5.2 -O2 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern void ClearImage(s16 *rect, s32 a1, s32 a2, s32 a3);
extern void DrawSync(s32 arg);

void vram_rect_clear(s32 count) {
    s32 pad1;
    s16 rect[4];
    s32 x;
    s32 i;
    s32 pad2[2];

    i = 0;
    if (count > 0) {
        x = 800;
        do {
            rect[0] = (s16)x;
            rect[1] = 256;
            rect[2] = 32;
            rect[3] = 256;
            ClearImage(rect, 0, 0, 0);
            i = i + 1;
            x = x + 64;
        } while (i != count);
    }
    DrawSync(0);
}
