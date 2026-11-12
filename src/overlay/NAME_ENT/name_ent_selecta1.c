/* @SymbolStart: 0x8013E50C */
/* @SymbolSize: 56 */
/* name_ent_selecta1 - NAME_ENT overlay, Address: 0x8013E514, Size: 48 bytes (12 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern void name_ent_menu_select();
extern s32 pad_trig;

void name_ent_selecta1(void)
{
    if ((pad_trig & 0x0860) != 0) {
        sub_sub_step_up();
    }
    name_ent_menu_select();
}
