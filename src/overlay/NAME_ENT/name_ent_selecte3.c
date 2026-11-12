/* @SymbolStart: 0x8013FF38 */
/* @SymbolSize: 120 */
/* name_ent_selecte3 - NAME_ENT overlay, Address: 0x8013FF40, Size: 112 bytes (28 insns), Semi-matched */
#include "types.h"

extern void device_cmd_send();
extern void goto_sub_sub_step();

extern u8 card_sys_present;
extern s32 pad_trig;

void name_ent_selecte3(void)
{

    if (pad_trig & 0x0020) {
        device_cmd_send(1281);
        if (card_sys_present != 0) {
            goto_sub_sub_step(228);
        } else {
            goto_sub_sub_step(229);
        }
    } else if (pad_trig & 0x0040) {
        goto_sub_sub_step(0);
    }
}
