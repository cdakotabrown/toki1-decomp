/* @SymbolStart: 0x801394B4 */
/* @SymbolSize: 64 */
/* Address: 0x801394BC, Size: 64 bytes (14 body insns + 2-insn cross-prologue tail) */
#include "types.h"

extern s32 serifu_no;

extern void sub_sub_step_up(void);

extern u8 now_sub_sub_step;

void shugaku_sel_skip(void)
{
    s32 t6 = serifu_no;

    if (t6 == 0) goto label0;
    now_sub_sub_step = (now_sub_sub_step + 17);

label0:
    sub_sub_step_up();
}
