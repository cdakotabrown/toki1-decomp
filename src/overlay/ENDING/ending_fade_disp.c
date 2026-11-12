/* ending_fade_disp - ENDING overlay, Address: 0x80139AC0, Size: 168 bytes (42 instructions)  * Status: BYTE-PERFECT 🎉 */
#include "types.h"

extern void ending_quad_disp();
extern void dtd_on();

void ending_fade_disp(s32 a0)
{
    ending_quad_disp(-100, -80, 200, 16, a0, 0, 1);
    ending_quad_disp(-100, 64, 200, 16, a0, 0, 1);
    dtd_on(0);
    ending_quad_disp(-100, -64, 200, 128, a0, 11, 1);
    dtd_on(11);
}
