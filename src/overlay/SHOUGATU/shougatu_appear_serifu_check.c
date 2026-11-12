/* @SymbolStart: 0x8013BA84 */
/* @SymbolSize: 60 */
/* shougatu_appear_serifu_check - SHOUGATU overlay, Address: 0x8013BA8C, Size: 60 bytes (15 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern void shougatu_appear_serifu_run();
extern u8 shougatu_appear_any;

void shougatu_appear_serifu_check(void)
{
    if (shougatu_appear_any != 0) {
        sub_sub_step_up();
        return;
    }
    shougatu_appear_serifu_run();
}
