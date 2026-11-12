/* @SymbolStart: 0x80132B50 */
/* @SymbolSize: 124 */
#include "types.h"

extern u8 basho_num;

extern u8 step_mode;

extern void sprite_fade_main(void);
extern void sub_sub_step_up(void);
extern void indexed_byte_toggle(s32 a0);
extern void wipe_main(void);

void shugaku_wipe_step(void)
{
    s32 sel = basho_num;

    u8 flag;
    s32 cond;

    if (sel == 1) {
        sprite_fade_main();
        return;
    }
    if (sel == 2) {
        sub_sub_step_up();
        return;
    }
    flag = step_mode;
    cond = ((u32)flag < 1U);
    step_mode = (u8)(flag + 1);
    if (cond) {
        indexed_byte_toggle(1);
    }
    wipe_main();
}
