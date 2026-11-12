/* tt_widget_init2 - TT overlay, Address: 0x8013C6D0. */
#include "types.h"

extern u8 *tt_scroll_wk;
extern u8 *tt_anim_buf;

void tt_widget_init2(void)
{
    u8 *a = tt_scroll_wk;
    u8 *b = tt_anim_buf;
    b[0] = 1;
    b[1] = 0;
    *(s16 *)(b + 68) = 0;
    *(s16 *)(b + 20) = -96;
    *(s16 *)(b + 22) = (a[10] << 4) + 96;
    *(s16 *)(b + 60) = 8;
    *(s16 *)(b + 62) = 8;
    b[70] = 0;
    b[71] = 24;
    *(s16 *)(b + 64) = 10;
    *(s16 *)(b + 66) = 15375;
    b[59] = 101;
    *(s16 *)(b + 16) = 0;
}
