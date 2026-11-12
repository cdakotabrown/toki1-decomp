/* @SymbolStart: 0x8013DB94 */
/* @SymbolSize: 56 */
/* name_ent_no_album_notice_select - NAME_ENT overlay, Address: 0x8013DB9C, Size: 48 bytes (12 insns) */
#include "types.h"

extern void name_ent_option_restore();
extern void goto_step();
extern s32 pad_trig;

void name_ent_no_album_notice_select(void)
{
    if ((pad_trig & 0x0820) != 0) {
        name_ent_option_restore();
        goto_step(20);
    }
}
