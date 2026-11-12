/* @SymbolStart: 0x8014AA20 */
/* @SymbolSize: 44 */
/* name_ent_prologue_serifu38_main - NAME_ENT overlay, Address: 0x8014AA28, Size: 36 bytes (9 insns) */
#include "types.h"

extern void name_ent_prologue_serifu38_body();
extern u8 now_sub_sub_step;

void name_ent_prologue_serifu38_main(void)
{
    if (now_sub_sub_step == 0) {
        name_ent_prologue_serifu38_body();
    }
}
