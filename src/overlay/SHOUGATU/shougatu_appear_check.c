/* @SymbolStart: 0x8013BAC0 */
/* @SymbolSize: 64 */
/* shougatu_appear_check - SHOUGATU overlay, Address: 0x8013BAC8, Size: 56 bytes (14 insns) */
#include "types.h"

extern void sub_step_up();
extern void sub_sub_step_up();
extern u8 shougatu_appear_any;

void shougatu_appear_check(void)
{
    if (shougatu_appear_any != 0) {
        sub_step_up();
        return;
    }
    sub_sub_step_up();
}
