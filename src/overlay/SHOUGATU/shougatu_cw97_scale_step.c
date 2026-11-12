/* @SymbolStart: 0x801402AC */
/* @SymbolSize: 68 */
#include "types.h"

extern void sub_sub_step_up();

extern s16 cw97_scaley;

s32 shougatu_cw97_scale_step(void)
{
    cw97_scaley += 512;
    if (cw97_scaley >= 4096) {
        sub_sub_step_up();
    }
}
