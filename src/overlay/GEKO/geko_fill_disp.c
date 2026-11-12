/* geko_fill_disp - GEKO overlay, Address: 0x8013FC9C, Size: 76 bytes (19 insns) */
#include "types.h"

extern void geko_rect_disp(s32 x, s32 y, s32 w, s32 h, s32 color, s32 stack14, s32 stack18);
extern void dtd_on(s32 a);

void geko_fill_disp(void)
{
    geko_rect_disp(-160, -120, 320, 160, 0x00FFFFFF, 10, 0);
    dtd_on(9);
}
