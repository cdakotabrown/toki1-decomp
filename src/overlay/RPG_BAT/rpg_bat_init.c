/* @RodataBase: 0x8015CF90 */
/* @SymbolSize: 372 */
/* rpg_bat_init - RPG_BAT overlay, Address: 0x8014E780, Size: 372 bytes (93 insns) */
#include "types.h"

extern s32 prim_init();
extern s32 sub_step_up();
extern s32 draw2d3d();
extern s32 back_clear_switch();
extern s32 device_cmd_send();
extern s32 tpage_buf_clear();
extern s32 class_switch();
extern s32 class_clear_all();
extern s32 text_sys_init();
extern s32 hizuke_disp_switch();
extern s32 message_disp_switch();
extern s32 mes_window_switch();
extern s32 cursor_init();
extern s32 srand();
extern s32 rpg_bat_state_clear();

extern s32 vblnk_counter;
extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern u8 now_girl;
extern s32 rpg_bat_arg;
extern s32 rpg_bat_mode;
extern s32 rpg_bat_girl;
extern s32 rpg_bat_girl_save;
extern s32 rpg_bat_variant_flag;

void rpg_bat_init(void)
{
    s32 id;
    s32 v0;

    srand(vblnk_counter);
    class_clear_all(0);
    cursor_init(0);
    text_sys_init();
    tpage_buf_clear();
    back_clear_r = 0;
    back_clear_b = 0;
    back_clear_g = 0;
    back_clear_switch(1);
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    hizuke_disp_switch(0);
    mes_window_switch(0);
    message_disp_switch(0);
    device_cmd_send(127);
    rpg_bat_state_clear();

    v0 = now_girl;
    id = rpg_bat_arg;
    rpg_bat_girl_save = v0;
    rpg_bat_girl = v0;

    switch (id) {
    case 1:
        rpg_bat_mode = 0x10000;
        break;
    case 2:
        rpg_bat_mode = 0x20000;
        break;
    case 3:
        rpg_bat_mode = 0x40000;
        break;
    case 4:
        rpg_bat_mode = 0x100000;
        break;
    case 5:
        rpg_bat_variant_flag = 1;
        rpg_bat_mode = 0x100000;
        break;
    case 6:
        rpg_bat_mode = 0x8000000;
        break;
    case 7:
    default:
        rpg_bat_mode = 0x80000000;
        break;
    }

    sub_step_up();
}
