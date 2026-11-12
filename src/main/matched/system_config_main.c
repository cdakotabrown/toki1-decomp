/* Address: 0x8005DBF4 */
/* Size: 664 bytes (166 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0:v1 --unidirectional */
#include "types.h"

extern u8 now_sub_sub_step;

extern void system_config_menu_init(void);
extern void system_config_menu(void);
extern void system_config_speed_init(void);
extern void system_config_moji_speed(void);
extern void system_config_moji_speed_exit(void);
extern void system_config_icon_mode_init(void);
extern void system_config_icon_mode(void);
extern void system_config_icon_mode_exit(void);
extern void system_config_mouse_menu_init(void);
extern void system_config_mouse_menu(void);
extern void system_config_mouse_menu_exit(void);
extern void system_config_mouse_botton_mode_init(void);
extern void system_config_mouse_botton_mode(void);
extern void system_config_mouse_botton_mode_exit(void);
extern void config_click_mode_init(void);
extern void config_click_mode(void);
extern void config_click_mode_exit(void);
extern void config_click_speed_init(void);
extern void config_click_speed(void);
extern void config_click_speed_exit(void);
extern void config_menu_mode_init(void);
extern void config_menu_mode(void);
extern void config_menu_mode_exit(void);
extern void cursor_speed_init(void);
extern void cursor_speed(void);
extern void cursor_speed_exit(void);
extern void voice_mode_init(void);
extern void voice_mode(void);
extern void voice_mode_exit(void);
extern void mascot_sel_init(void);
extern void mascot_sel(void);
extern void mascot_sel_exit(void);
extern void system_config_exit(void);
extern void basyo_show(void);
extern void cal_base_show(void);
extern void message_window_show(void);
extern void parameter_show(void);
extern void hizuke_show(void);
extern void step_directional_scroll_once(void);

void system_config_main(void) {
    u8 v1 = now_sub_sub_step;

    if ((u32)v1 >= 115U) {
        if (v1 != 0xFF) {
            goto tail;
        }
        goto call_ff;
    }

    switch (v1) {
        case 0:   system_config_menu_init(); break;
        case 1:   system_config_menu(); break;
        case 16:  system_config_speed_init(); break;
        case 17:  system_config_moji_speed(); break;
        case 18:  system_config_moji_speed_exit(); break;
        case 32:  system_config_icon_mode_init(); break;
        case 33:  system_config_icon_mode(); break;
        case 34:  system_config_icon_mode_exit(); break;
        case 48:  system_config_mouse_menu_init(); break;
        case 49:  system_config_mouse_menu(); break;
        case 50:  system_config_mouse_menu_exit(); break;
        case 51:  system_config_mouse_botton_mode_init(); break;
        case 52:  system_config_mouse_botton_mode(); break;
        case 53:  system_config_mouse_botton_mode_exit(); break;
        case 54:  config_click_mode_init(); break;
        case 55:  config_click_mode(); break;
        case 56:  config_click_mode_exit(); break;
        case 57:  config_click_speed_init(); break;
        case 58:  config_click_speed(); break;
        case 59:  config_click_speed_exit(); break;
        case 64:  config_menu_mode_init(); break;
        case 65:  config_menu_mode(); break;
        case 66:  config_menu_mode_exit(); break;
        case 80:  cursor_speed_init(); break;
        case 81:  cursor_speed(); break;
        case 82:  cursor_speed_exit(); break;
        case 96:  voice_mode_init(); break;
        case 97:  voice_mode(); break;
        case 98:  voice_mode_exit(); break;
        case 112: mascot_sel_init(); break;
        case 113: mascot_sel(); break;
        case 114: mascot_sel_exit(); break;
        default:  break;
    }
    goto tail;

call_ff:
    system_config_exit();

tail:

    basyo_show();
    cal_base_show();
    message_window_show();
    parameter_show();
    hizuke_show();
    step_directional_scroll_once();
}
