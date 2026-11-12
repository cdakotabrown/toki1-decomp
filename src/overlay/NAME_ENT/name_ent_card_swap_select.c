/* @SymbolStart: 0x80140FE8 */
/* @SymbolSize: 56 */
/* name_ent_card_swap_select - NAME_ENT overlay, Address: 0x80140FF0, Size: 48 bytes (12 insns) */
#include "types.h"

extern void goto_sub_step();
extern void goto_sub_sub_step();
extern s32 pad_trig;

void name_ent_card_swap_select(void)
{
    if ((pad_trig & 0x0860) != 0) {
        goto_sub_step(1);
        goto_sub_sub_step(6);
    }
}
