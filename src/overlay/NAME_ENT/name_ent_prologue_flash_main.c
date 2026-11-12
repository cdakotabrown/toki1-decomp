/* @SymbolStart: 0x8014AC4C */
/* @SymbolSize: 44 */
#include "types.h"

extern void name_ent_prologue_flash_body();
extern u8 now_sub_sub_step;

void name_ent_prologue_flash_main(void)
{
    if (now_sub_sub_step == 0) {
        name_ent_prologue_flash_body();
    }
}
