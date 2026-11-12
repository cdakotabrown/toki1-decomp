/* @SymbolStart: 0x8013F82C */
/* @SymbolSize: 96 */
/* name_ent_selectc5 - NAME_ENT overlay, Address: 0x8013F834, Size: 88 bytes (22 insns) */
#include "types.h"

extern void device_cmd_send();
extern void sub_sub_step_up();
extern void goto_sub_sub_step();
extern u8 step_mode;
extern s32 pad_trig;

void name_ent_selectc5(void)
{
    if (pad_trig & 0x0020) {
        device_cmd_send(1281);
        sub_sub_step_up();
    } else if (pad_trig & 0x0040) {
        goto_sub_sub_step(0);
        step_mode = 2;
    }
}
