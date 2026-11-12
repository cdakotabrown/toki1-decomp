/* @SymbolStart: 0x80152DBC */
/* @SymbolSize: 60 */
/* Address: 0x80152DBC (cross-prologue head) / 0x80152DC4 (body) */
/* Size: 60 bytes (15 instructions) */
#include "types.h"

extern s32 serifu_no;

extern void sub_sub_step_up(void);

void date_serifu0_skip_step(void)
{
    if (serifu_no == 0) {
        sub_sub_step_up();
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
