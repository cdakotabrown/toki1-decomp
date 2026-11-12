/* @SymbolStart: 0x8014A894 */
/* @SymbolSize: 44 */
#include "types.h"

extern void name_ent_bg_load11();
extern u8 now_sub_sub_step;

void name_ent_prologue_bg_load_main(void)
{
    if (now_sub_sub_step == 0) {
        name_ent_bg_load11();
    }
}
