/* @SymbolStart: 0x80133FA0 */
/* @SymbolSize: 64 */
/* shougatu_step_skip4 -- SHOUGATU overlay, Address: 0x80133FA8, Size: 64 bytes (16 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

extern u8 now_sub_sub_step;
extern u8 shougatu_alone_flag;

void shougatu_step_skip4(void)
{
    if (shougatu_alone_flag == 0) goto label0;
    now_sub_sub_step = (now_sub_sub_step + 4);

label0:
    sub_sub_step_up();
}
