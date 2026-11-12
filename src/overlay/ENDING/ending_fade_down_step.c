/* @SymbolStart: 0x8013B8D8 */
/* @SymbolSize: 60 */
/* ending_fade_down_step - ENDING overlay, Address: 0x8013B8D8, Size: 60 bytes (15 instructions) */
#include "types.h"

extern u8 ev_flag52;
extern void sub_sub_step_up();
extern void fade_down_main();

void ending_fade_down_step(void)
{
    u8 flag = ev_flag52;

    if (flag != 0) {
        sub_sub_step_up();
        return;
    }
    fade_down_main();
}
