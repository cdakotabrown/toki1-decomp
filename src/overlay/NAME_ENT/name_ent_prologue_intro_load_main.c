/* @SymbolStart: 0x80146850 */
/* @SymbolSize: 44 */
/* Address: 0x80146858, Size: 36 bytes (9 insns) */
#include "types.h"

extern void name_ent_prologue_intro_load_body(void);
extern u8 now_sub_sub_step;

void name_ent_prologue_intro_load_main(void)
{
    if (now_sub_sub_step != 0) {
        return;
    }
    name_ent_prologue_intro_load_body();
}
