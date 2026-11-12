/* @SymbolSize: 116 */
/* kangei_icon_init - KANGEI overlay, Address: 0x80136094, Size: 124 bytes (31 insns), Semi-matched */
#include "types.h"

extern void icon_init(void);
extern void icon_can_use_set(s32, s32);
extern void icon_cursor_move(s32);
extern void parameter_show_init(void);
extern void sub_sub_step_up(void);

extern u8 fade_bri;

extern u8 kangei_icon_flag;

void
kangei_icon_init(void)
{
    s16 i;

    icon_init();
    for (i = 0; i < 15; i++) {
        icon_can_use_set(i, 0);
    }
    icon_cursor_move(1);
    parameter_show_init();
    parameter_show_init();
    fade_bri = 0;
    kangei_icon_flag = 0;
    sub_sub_step_up();
}
