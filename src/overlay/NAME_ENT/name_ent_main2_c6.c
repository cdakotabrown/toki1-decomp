/* @SymbolStart: 0x8013F88C */
/* @SymbolSize: 60 */
/* name_ent_main2_c6 - NAME_ENT overlay, Address: 0x8013F894, Size: 52 bytes (13 insns) */
#include "types.h"

extern void goto_sub_sub_step();
extern u8 card_sys_present;

void name_ent_main2_c6(void)
{
    if (card_sys_present != 0) {
        goto_sub_sub_step(199);
    } else {
        goto_sub_sub_step(200);
    }
}
