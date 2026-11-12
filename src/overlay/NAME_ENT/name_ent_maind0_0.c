/* @SymbolStart: 0x801410A0 */
/* @SymbolSize: 60 */
/* name_ent_maind0_0 - NAME_ENT overlay, Address: 0x801410A8, Size: 52 bytes (13 insns) */
#include "types.h"

extern void goto_sub_sub_step();
extern u8 card_sys_present;

void name_ent_maind0_0(void)
{
    if (card_sys_present != 0) {
        goto_sub_sub_step(1);
    } else {
        goto_sub_sub_step(2);
    }
}
