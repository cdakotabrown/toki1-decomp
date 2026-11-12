/* @SymbolStart: 0x8013394C */
/* @SymbolSize: 52 */
/* Address: 0x80133954 */
/* Size: 44 bytes (11 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern s32 serifu_no;

void shougatu_taku_check2(void)
{
    if (serifu_no == 0) {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
