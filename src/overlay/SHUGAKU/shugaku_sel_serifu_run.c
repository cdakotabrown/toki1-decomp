/* @SymbolStart: 0x8013A288 */
/* @SymbolSize: 60 */
#include "types.h"

extern s32 serifu_no;

extern void sub_sub_step_up(void);
extern void func_80138664(void);

void shugaku_sel_serifu_run(void)
{
    s32 param_t6 = serifu_no;

    if (param_t6 != 0) goto lbl_8013A2AC;
    sub_sub_step_up();
    goto lbl_8013A2B8;
lbl_8013A2AC:
    func_80138664();
lbl_8013A2B8:
    return;
}
