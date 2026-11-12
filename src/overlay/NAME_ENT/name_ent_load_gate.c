/* @SymbolStart: 0x80146534 */
/* @SymbolSize: 44 */
/* name_ent_load_gate - NAME_ENT overlay, Address: 0x8014653C, Size: 36 bytes (9 insns) */
#include "types.h"

extern void name_ent_load_step();
extern u8 now_sub_sub_step;

void name_ent_load_gate(void)
{
    if (now_sub_sub_step == 0) {
        name_ent_load_step();
    }
}
