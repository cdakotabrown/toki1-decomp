/* @SymbolStart: 0x8013C94C */
/* @SymbolSize: 116 */
/* name_ent_msg5_select - NAME_ENT overlay, Address: 0x8013C954, Size: 108 bytes (27 insns) */
#include "types.h"

extern void name_ent_option_restore();
extern void goto_step();
extern void sub_sub_step_up();
extern s32 pad_trig;

void name_ent_msg5_select(void)
{
    s32 buttons = pad_trig;

    if (buttons & 0x0800) {
        name_ent_option_restore();
        goto_step(20);
    } else {
        s32 mask_0040 = buttons & 0x0040;
        if (buttons & 0x0020) {
            name_ent_option_restore();
            goto_step(20);
        } else if (mask_0040) {
            sub_sub_step_up();
        }
    }
}
