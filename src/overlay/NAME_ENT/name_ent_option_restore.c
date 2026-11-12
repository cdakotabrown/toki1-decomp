/* @SymbolStart: 0x8013B90C */
/* @SymbolSize: 536 */
/* name_ent_option_restore - NAME_ENT overlay, Address: 0x8013B914, Size: 528 bytes */
#include "types.h"

extern u32 name_ent_keep_opt_on;
extern s32 name_ent_menu_lock;
extern u8 name_ent_keep_k_speed;
extern u8 name_ent_keep_icon_mode;
extern u8 name_ent_keep_menu_mode;
extern u8 name_ent_keep_csr_speed;
extern u8 name_ent_keep_voice_level;
extern u8 name_ent_keep_click_mode;
extern u8 name_ent_keep_click_speed;
extern u8 name_ent_keep_mouse_btn;

extern u8 icon_mode;
extern u8 icon_num;
extern u8 csr_speed;
extern u8 click_mode;
extern u8 click_speed;
extern u8 mouse_btn_mode;
extern u8 menu_mode;
extern u8 voice_level;

extern u8 name_ent_option_restore_str0[];
extern u8 name_ent_optback_kspeed_fmt[];
extern u8 name_ent_optback_icon_fmt[];
extern u8 name_ent_optback_menu_fmt[];
extern u8 name_ent_optback_csr_fmt[];
extern u8 name_ent_optback_sound_fmt[];
extern u8 name_ent_optback_click_fmt[];
extern u8 name_ent_optback_clkspd_fmt[];
extern u8 name_ent_optback_mouse_fmt[];

extern void printf();
extern void k_speed_set(u32);
extern void icon_init(void);
extern void icon_disp_switch(s32);
extern void name_ent_option_restore_noop(void);
extern void name_ent_icon_reset(void);

void name_ent_option_restore(void)
{

    if (name_ent_keep_opt_on != 0) {
        printf(name_ent_option_restore_str0);
        if (name_ent_keep_opt_on & 0x80) {
            k_speed_set(name_ent_keep_k_speed);
        }
        printf(name_ent_optback_kspeed_fmt, name_ent_keep_k_speed);

        if ((u32)name_ent_keep_icon_mode >= 3) {
            icon_mode = (u32)name_ent_keep_icon_mode % 3;
        }
        printf(name_ent_optback_icon_fmt, (u32)name_ent_keep_icon_mode);

        if ((u32)name_ent_keep_menu_mode >= 4) {
            menu_mode = (u32)name_ent_keep_menu_mode % 4;
        }
        printf(name_ent_optback_menu_fmt, (u32)name_ent_keep_menu_mode);

        if (name_ent_keep_opt_on & 8) {
            csr_speed = name_ent_keep_csr_speed;
        }
        printf(name_ent_optback_csr_fmt, name_ent_keep_csr_speed);

        if ((u32)name_ent_keep_voice_level >= 3) {
            voice_level = (u32)name_ent_keep_voice_level % 3;
        }
        printf(name_ent_optback_sound_fmt, (u32)name_ent_keep_voice_level);

        if (name_ent_keep_opt_on & 0x40) {
            click_mode = name_ent_keep_click_mode;
        }
        printf(name_ent_optback_click_fmt, name_ent_keep_click_mode);

        if (name_ent_keep_opt_on & 0x20) {
            click_speed = name_ent_keep_click_speed;
        }
        printf(name_ent_optback_clkspd_fmt, name_ent_keep_click_speed);

        if (name_ent_keep_opt_on & 0x10) {
            mouse_btn_mode = name_ent_keep_mouse_btn;
        }
        printf(name_ent_optback_mouse_fmt, name_ent_keep_mouse_btn);

        if (icon_mode == 2) {
            icon_num = 15;
        } else {
            icon_num = 14;
        }
        icon_init();
        icon_disp_switch(0);
    }
    name_ent_menu_lock = 0;
    name_ent_option_restore_noop();
    name_ent_icon_reset();
}
