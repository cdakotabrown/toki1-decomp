/* @SymbolStart: 0x801413FC */
/* @SymbolSize: 88 */
/* Address: 0x80141404 */
/* Size: 88 bytes (22 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32 a0);
extern void device_state_init(void);
extern void text_sys_init(void);
extern void set_dec_bri(s32 a0);

extern u8 screen_bri;

extern u8 option_on;

void etc_card_init0(void)
{
    s32 t6 = option_on;

    if (t6 != 0) {
        device_state_init();
        goto_step(18);
        return;
    }
    text_sys_init();
    set_dec_bri(0);
    screen_bri = 0;
    sub_sub_step_up();
}
