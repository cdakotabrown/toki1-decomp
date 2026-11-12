/* tt_widget_y_set - TT overlay, Address: 0x8013C740. */
#include "types.h"

extern u8 *tt_scroll_wk;
extern u8 *tt_anim_buf;

void tt_widget_y_set(void)
{
    u8 *a = tt_scroll_wk;
    u8 *b = tt_anim_buf;
    *(s16 *)(b + 22) = (a[10] << 4) + 96;
}
