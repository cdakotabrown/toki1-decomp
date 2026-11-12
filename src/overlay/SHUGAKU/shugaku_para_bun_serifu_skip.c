/* @SymbolStart: 0x801341CC */
/* @SymbolSize: 68 */
#include "types.h"

extern s16 para_bun;

extern s16 with_yoshi;

extern void sub_sub_step_up(void);

void shugaku_para_bun_serifu_skip(void)
{
    s32 t6_hidden = para_bun;

    if (t6_hidden >= 120) {
        with_yoshi = (s16)(with_yoshi + 3);
    }
    sub_sub_step_up();
}
