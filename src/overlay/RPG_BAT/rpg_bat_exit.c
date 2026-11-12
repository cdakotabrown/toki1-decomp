/* @SymbolSize: 160 */
/* rpg_bat_exit - RPG_BAT overlay, Address: 0x8014E8F4, Size: 160 bytes (40 insns) */
#include "types.h"

extern volatile u8 ret_step;
extern u8 ret_sub_step;
extern u8 ret_sub_sub_step;
extern volatile s8 now_girl;
extern s32 rpg_bat_arg;
extern s32 rpg_bat_result;
extern s32 rpg_bat_girl_save;

extern s32 tpage_buf_clear();
extern void dec_bg_reset();
extern s32 text_sys_init();
extern s32 class_clear_all();
extern s32 cursor_init();
extern s32 set_flag_b3_high_bit();
extern s32 back_clear_switch();
extern void goto_step();
extern void goto_sub_step();
extern void goto_sub_sub_step();

void rpg_bat_exit(void)
{
    tpage_buf_clear();
    dec_bg_reset();
    text_sys_init();
    class_clear_all(0);
    cursor_init(0);
    set_flag_b3_high_bit(0);
    back_clear_switch(1);
    rpg_bat_arg = rpg_bat_result;
    now_girl = (s8)rpg_bat_girl_save;
    goto_step(ret_step);
    goto_sub_step(ret_sub_step);
    goto_sub_sub_step(ret_sub_sub_step);
}
