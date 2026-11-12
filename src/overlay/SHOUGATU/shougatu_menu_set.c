/* @CompileFlags: -Wc,-pic0 */
/* Address: 0x8013FB50 */
/* Size: 168 bytes (42 instructions) */
/* @SymbolSize: 168 */
#include "types.h"

extern void sub_sub_step_up();
extern void menu_set(s32 id, s32 count, s16 *coords, s16 *xValues,
                          s16 *wValues, s16 *hValues);
extern volatile s32 menu_on;

void shougatu_menu_set(void)
{
    s16 coords[4];
    s16 xValues[4];
    s16 wValues[4];
    s16 hValues[4];
    s16 unused[8];

    coords[0] = -150;
    coords[1] = -72;
    coords[2] = 6;
    coords[3] = 84;

    xValues[0] = xValues[1] = xValues[2] = xValues[3] = -64;
    wValues[0] = wValues[1] = wValues[2] = wValues[3] = 64;
    hValues[0] = hValues[1] = hValues[2] = hValues[3] = 64;

    menu_set(0, 4, coords, xValues, wValues, hValues);
    menu_on = 1;
    sub_sub_step_up();
}
