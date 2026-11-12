/* @SymbolStart: 0x801422D4 */
/* @SymbolSize: 44 */
/* name_ent_mainc0 - NAME_ENT overlay, Address: 0x801422D4, Size: 44 bytes (11 insns) */
#include "types.h"

extern void name_ent_card_format();
extern u8 now_sub_sub_step;

void name_ent_mainc0(void)
{
    if (now_sub_sub_step == 0) {
        name_ent_card_format();
    }
}
