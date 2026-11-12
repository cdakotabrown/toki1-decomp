/* @SymbolSize: 152 */
/* Address: 0x80134F9C, Size: 152 bytes (38 instructions) */
#include "types.h"

extern void menu_set();
extern void option_menu_box_show();
extern void goto_sub_sub_step();

void option_menu3_init2(void)
{
    s16 unused[4];
    s16 x[4];
    s16 y[4];
    s16 w[4];
    s16 h[4];
    s32 i;

    for (i = 0; i < 3; i++) {
        x[i] = -120;
        y[i] = i * 40 - 32;
        w[i] = 108;
        h[i] = 16;
    }

    menu_set(1, 3, x, y, w, h);
    option_menu_box_show();
    goto_sub_sub_step(1);
}
