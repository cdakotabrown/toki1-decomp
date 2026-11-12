/* @SymbolStart: 0x80132F90 */
/* @SymbolSize: 60 */
/* Address: 0x80132F98 */
/* Size: 60 bytes (15 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_sub_step(s32 a0);
extern s32 serifu_no;

void shougatu_taku_check(void)
{
    s32 skip_flag = serifu_no;

    if (skip_flag == 0) {
        goto_sub_step(3);
    } else {
        sub_sub_step_up();
    }
}
