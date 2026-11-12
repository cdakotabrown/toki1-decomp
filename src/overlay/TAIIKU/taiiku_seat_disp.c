/* @SymbolSize: 92 */
/* Address: 0x80133438 */
/* Size: 92 bytes (23 insns) */
#include "types.h"

extern void taiiku_seat_set(s32 a0, s32 a1);
extern void scroll_poly_disp(s32 a0, s32 a1, s32 a2, s32 a3, s32 stack0);

void taiiku_seat_disp(void)
{
    taiiku_seat_set(0, 1);
    scroll_poly_disp(0, 15, 0, 112, 640);
    scroll_poly_disp(0, 15, 112, 128, 128);
}
