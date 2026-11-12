/* @SymbolStart: 0x801340F0 */
/* @SymbolSize: 56 */
#include "types.h"

extern s16 para_bun;

extern void sub_sub_step_up(void);

void shugaku_para_bun_skip(void)
{
    s32 t6 = para_bun;

    if (t6 >= 120) {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
