/* @SymbolStart: 0x80132BCC */
/* @SymbolSize: 80 */
#include "types.h"

extern u8 ret_sub_step;

extern u8 ret_sub_sub_step;

extern void goto_sub_step(s32 a0);
extern void goto_sub_sub_step(s32 a0);
extern void shugaku_exit(void);

void shugaku_ret_goto_or_exit(void)
{
    if (ret_sub_step != 6) {
        goto_sub_step(ret_sub_step);
        goto_sub_sub_step(ret_sub_sub_step);
        return;
    }
    shugaku_exit();
}
