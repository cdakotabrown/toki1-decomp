/* @SymbolStart: 0x8013E824 */
/* @SymbolSize: 116 */
/* name_ent_selecta5 - NAME_ENT overlay, Address: 0x8013E82C, Size: 108 bytes (27 insns) */
#include "types.h"

extern void device_cmd_send();
extern void goto_sub_sub_step();
extern u8 card_sys_present;
extern s32 pad_trig;

void name_ent_selecta5(void)
{
    if (pad_trig & 0x0040) {
        device_cmd_send(1281);
        goto_sub_sub_step(0);
    } else if (pad_trig & 0x0020) {
        if (card_sys_present != 0) {
            goto_sub_sub_step(166);
        } else {
            goto_sub_sub_step(167);
        }
    }
}
