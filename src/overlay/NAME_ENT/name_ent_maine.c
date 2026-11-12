/* @SymbolStart: 0x80149A10 */
/* @SymbolSize: 44 */
/* name_ent_maine - NAME_ENT overlay, Address: 0x80149A18, Size: 36 bytes (9 insns) */
#include "types.h"

extern void _name_ent_maine();
extern u8 now_sub_sub_step;

void name_ent_maine(void)
{
    if (now_sub_sub_step == 0) {
        _name_ent_maine();
    }
}
