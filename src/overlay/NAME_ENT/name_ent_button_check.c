/* @SymbolStart: 0x801429A0 */
/* @SymbolSize: 128 */
/* name_ent_button_check - NAME_ENT overlay, Address: 0x801429A8, Size: 120 bytes (30 insns) */
#include "types.h"

extern void device_cmd_send();
extern void goto_sub_step();

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;

extern s32 pad_trig;

s32 name_ent_button_check(void)
{
    s16 y;
    s16 x;

    if (pad_trig & 0x20) {
        y = cursor_cw_x;
        if (y < -80 && y >= -143) {
            x = cursor_cw_y;
            if (x < -24 && x >= -39) {
                device_cmd_send(1295);
                goto_sub_step(208);
                return 1;
            }
        }
    }
    return 0;
}
