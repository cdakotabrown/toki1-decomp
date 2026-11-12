/* @SymbolStart: 0x8013DAE0 */
/* @SymbolSize: 76 */
/* name_ent_new_album_ask_select - NAME_ENT overlay, Address: 0x8013DAE8, Size: 68 bytes (17 insns) */
#include "types.h"

extern void goto_sub_sub_step();
extern void sub_sub_step_up();
extern s32 pad_trig;

void name_ent_new_album_ask_select(void)
{
    if (pad_trig & 0x0020) {
        goto_sub_sub_step(68);
    } else if (pad_trig & 0x0040) {
        sub_sub_step_up();
    }
}
