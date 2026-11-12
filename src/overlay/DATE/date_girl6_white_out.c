/* @SymbolStart: 0x80157380 */
/* @SymbolSize: 108 */
/* @CompileFlags: -cckr */
#include "types.h"

extern u8 back_clear_r[];

void sub_sub_step_up();

date_girl6_white_out()
{
    u8 c;

    back_clear_r[0] += 16;
    back_clear_r[1] += 16;
    back_clear_r[2] += 16;
    if (back_clear_r[0] >= 240) {
        sub_sub_step_up();
    }
}
