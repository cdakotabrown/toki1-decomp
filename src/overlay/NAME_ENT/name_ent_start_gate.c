/* @SymbolStart: 0x801461C4 */
/* @SymbolSize: 44 */
/* name_ent_start_gate - NAME_ENT overlay, Address: 0x801461CC, Size: 36 bytes (9 insns) */
#include "types.h"

extern void name_ent_start_step();
extern u8 now_sub_sub_step;

void name_ent_start_gate(void)
{
    if (now_sub_sub_step == 0) {
        name_ent_start_step();
    }
}
