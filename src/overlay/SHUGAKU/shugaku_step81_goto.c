/* @SymbolStart: 0x8013A548 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x7c */
/* @SymbolSize: 164 */
/* Address: 0x8013A550  Size: 156 bytes (39 instructions) */
#include "types.h"

extern u8 shugaku_place;

extern void goto_step();
extern void class_clear_all();
extern void hizuke_disp_switch();
extern void set_flag_b3_high_bit();

extern s32 rpg_bat_arg;
extern u8 now_step;
extern u8 now_sub_step;
extern u8 ret_step;
extern u8 ret_sub_step;
extern u8 ret_sub_sub_step;

void shugaku_step81_goto(void) {
    s32 sel = shugaku_place;

    if (sel == 0) {
        rpg_bat_arg = 2;
    } else if (sel == 1) {
        rpg_bat_arg = 1;
    } else {
        rpg_bat_arg = 3;
    }
    class_clear_all(0);
    set_flag_b3_high_bit(0);
    ret_step = now_step;
    ret_sub_step = now_sub_step;
    ret_sub_sub_step = 50;
    hizuke_disp_switch(0);
    goto_step(129);
}
