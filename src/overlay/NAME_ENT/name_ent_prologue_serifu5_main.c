/* @SymbolStart: 0x801472BC */
/* @SymbolSize: 44 */
#include "types.h"

extern void name_ent_prologue_serifu5_body();
extern u8 now_sub_sub_step;

void name_ent_prologue_serifu5_main(void)
{
    if (now_sub_sub_step == 0) {
        name_ent_prologue_serifu5_body();
    }
}
