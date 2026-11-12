/* @SymbolStart: 0x80134F60 */
/* @SymbolSize: 56 */
/* Address: 0x80134F68 */
/* Size: 48 bytes (12 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern u8 now_girl;

void shougatu_girl2_skip(void)
{
    if (now_girl == 2) {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
