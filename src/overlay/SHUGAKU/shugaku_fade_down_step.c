/* @SymbolStart: 0x80138D30 */
/* @SymbolSize: 64 */
#include "types.h"

extern u8 shugaku_fade_skip;

extern void sub_sub_step_up(void);
extern void fade_down_main(void);

void shugaku_fade_down_step(void)
{
    s32 param_t6 = shugaku_fade_skip;

    if (param_t6 != 1) goto lbl_80138D58;
    sub_sub_step_up();
    return;
lbl_80138D58:
    fade_down_main();
    return;
}
