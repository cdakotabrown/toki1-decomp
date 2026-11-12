/* @SymbolSize: 76 */
/* ending_vram_save - ENDING overlay, Address: 0x801337B4, Size: 76 bytes (19 instructions) */
#include "types.h"

extern void StoreImage();
extern void sub_sub_step_up();

void ending_vram_save(void)
{
    RECT r;
    r.x = 320;
    r.y = 128;
    r.w = 384;
    r.h = 128;
    StoreImage(&r, (s32)0x80180000);
    sub_sub_step_up();
}
