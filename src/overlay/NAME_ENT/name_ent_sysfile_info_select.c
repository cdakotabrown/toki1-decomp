/* @SymbolStart: 0x8013DDE0 */
/* @SymbolSize: 48 */
/* name_ent_sysfile_info_select - NAME_ENT overlay, Address: 0x8013DDE8, Size: 40 bytes (10 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern s32 pad_trig;

void name_ent_sysfile_info_select(void)
{
    if (pad_trig & 0x0860) {
        sub_sub_step_up();
    }
}
