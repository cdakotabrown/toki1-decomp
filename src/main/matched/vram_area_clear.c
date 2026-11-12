/* Address: 0x80041328 - 0x800414A0 */
/* Size: 376 bytes (94 instructions) */
/* Compiler: IDO 5.2 with -O2 -G0 -mips1 -EL -non_shared */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern void ClearImage(RECT *rect, s32 r, s32 g, s32 b);
extern void DrawSync(s32 mode);

void vram_area_clear(s32 arg0)
{

    s32 pad1;
    s32 pad2;
    s32 pad3;
    RECT rect;
    s32 pad4;
    s32 pad5;
    s32 i;
    s32 v0;

    if (arg0 == 255) {

        rect.x = 0;
        rect.y = 0;
        rect.w = 64;
        rect.h = 128;
        ClearImage(&rect, 0, 0, 0);

        for (i = 1; i != 64; i++) {
            vram_area_clear(i);
        }
    } else if (arg0 == 254) {

        for (i = 0; i != 5; i++) {
            vram_area_clear(15 - i);
            vram_area_clear(47 - i);
            vram_area_clear(i + 21);
            vram_area_clear(i + 53);
        }
    } else if (arg0 == 0) {

        for (i = 0; i != 32; i++) {
            v0 = i & 0xF;

            if (v0 >= 5 && v0 < 15 && i < 22) {
                vram_area_clear(i);
                vram_area_clear(i + 32);
            }
        }
    } else {

        rect.x = (arg0 & 0xF) << 6;
        rect.y = ((arg0 & 0x10) << 4) + ((arg0 & 0x20) << 2);
        rect.w = 64;
        rect.h = 128;
    }

    ClearImage(&rect, 0, 0, 0);
    DrawSync(0);
}
