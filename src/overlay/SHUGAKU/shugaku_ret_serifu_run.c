/* @SymbolStart: 0x80132DE0 */
/* @SymbolSize: 64 */
#include "types.h"

extern u8 ret_sub_step;

extern void func_80138664(void);
extern void sub_sub_step_up(void);

void shugaku_ret_serifu_run(void)
{
    s32 t6 = ret_sub_step;

    if (t6 != 6) {
        func_80138664();
        return;
    }
    sub_sub_step_up();
}
