/* @SymbolStart: 0x801416EC */
/* @SymbolSize: 92 */
/* Function: etc_album_save_wait3 */
/* Overlay: ETC */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern void device_state_init(void);
extern void dec_bg_show(s32);

extern s32 pad_trig;

void etc_album_save_wait3(void)
{
    s32 v0_in = pad_trig;

    if (v0_in & 0x20) {
        sub_sub_step_up();
    } else if (v0_in & 0x40) {
        device_state_init();
        goto_step(196);
    }
    dec_bg_show(2);
}
