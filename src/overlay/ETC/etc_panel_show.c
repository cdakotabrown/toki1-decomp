/* @SymbolStart: 0x80147EF8 */
/* @SymbolSize: 112 */
/* Address: 0x80147F00, Size: 120 bytes (30 insns w/ 4-insn cross-prologue tail) */
#include "types.h"

extern void sub_step_up(void);
extern void set_flag_b3_high_bit(s32 a0);
extern void etc_panel_disp(void);
extern void etc_shade_disp(void);
extern void etc_box_shade_show(void);
extern s32 step_counter;

extern s32 pad_trig;

void etc_panel_show(void) {
    s32 flags = pad_trig;

    if (((flags & 0x20) != 0 && (u32)step_counter >= 257u) ||
        (flags & 0x40) != 0) {
        set_flag_b3_high_bit(1);
        sub_step_up();
    }
    etc_shade_disp();
    etc_box_shade_show();
    etc_panel_disp();
}
