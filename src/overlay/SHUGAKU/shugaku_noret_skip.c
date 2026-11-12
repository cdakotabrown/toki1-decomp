/* @SymbolStart: 0x80132C1C */
/* @SymbolSize: 72 */
/* shugaku_noret_skip - SHUGAKU overlay, Address: 0x80132C24, Size: 64 bytes (16 insns) */
#include "types.h"

extern u8 ret_sub_step;

extern u8 now_sub_sub_step;
extern void sub_sub_step_up(void);

void shugaku_noret_skip(void)
{
    u32 a0_param = ret_sub_step;

    if (a0_param == 6) {
        now_sub_sub_step = now_sub_sub_step + 6;
    } else {
        sub_sub_step_up();
    }
}
