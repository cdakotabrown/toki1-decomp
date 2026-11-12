/* @SymbolStart: 0x8013FA94 */
/* Address: 0x8013FA9C */
/* Size: 180 bytes (45 instructions) */
/* @SymbolSize: 188 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void sub_sub_step_up(void);

extern u8 shougatu_phase_on;
extern u8 shougatu_menu_ev_girl;
extern CLASS_W class_w[];

void shougatu_menu_disp_on(void)
{
    s32 v0 = shougatu_menu_ev_girl;

    if (v0 != 0 && v0 != 7 && v0 != 9) {
        class_w[98].disp_flag |= 0x80;
    }
    if (v0 != 0 && v0 != 7 && v0 != 9) {
        class_w[99].disp_flag |= 0x80;
    }
    if (v0 != 0 && v0 != 7 && v0 != 9) {
        class_w[100].disp_flag |= 0x80;
    }
    class_w[101].disp_flag |= 0x80;
    shougatu_phase_on = 1;
    sub_sub_step_up();
}
