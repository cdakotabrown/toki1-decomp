/* @SymbolStart: 0x801346F0 */
/* @SymbolSize: 72 */
/* Address: 0x801346F8 */
/* Size: 64 bytes (16 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern s32 serifu_no;

void shougatu_taku_check3(void)
{
    if (serifu_no != 2) {
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
