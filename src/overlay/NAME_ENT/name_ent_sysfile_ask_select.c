/* @SymbolStart: 0x8013DED4 */
/* @SymbolSize: 76 */
/* name_ent_sysfile_ask_select - NAME_ENT overlay, Address: 0x8013DEDC, Size: 68 bytes (17 insns) */
#include "types.h"

extern void goto_sub_sub_step(s32 arg);
extern void sub_sub_step_up(void);
extern s32 pad_trig;

void name_ent_sysfile_ask_select(void)
{
    if (pad_trig & 0x0820) {
        goto_sub_sub_step(76);
    } else if (pad_trig & 0x0040) {
        sub_sub_step_up();
    }
}
