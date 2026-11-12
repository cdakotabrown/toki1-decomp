/* @SymbolStart: 0x80133E30 */
/* @SymbolSize: 60 */
/* shougatu_serifu_check - SHOUGATU overlay, Address: 0x80133E38, Size: 60 bytes (15 insns) */
#include "types.h"

extern void shougatu_serifu_run();
extern void sub_sub_step_up();
extern u8 shougatu_alone_flag;

void shougatu_serifu_check(void)
{
    if (shougatu_alone_flag != 0) {
        shougatu_serifu_run();
        return;
    }
    sub_sub_step_up();
}
