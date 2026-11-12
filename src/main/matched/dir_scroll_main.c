/* Function: dir_scroll_main */
/* Address: 0x80083C18 */
/* Size: 196 bytes */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 fade_flag;
extern s32 script_on;
extern s32 date_sprite_on;
extern s32 pad_data;
extern u8  cursor_cw_disp;
extern s32 menu_on;

extern void dec_bg_show(s32);
extern void step_directional_scroll_once(void);

void dir_scroll_main(void) {
    s32 temp20c;

    if (fade_flag != 0) {
        dec_bg_show(1);
    }

    temp20c = script_on;
    if (temp20c == 0) {
        if (date_sprite_on == 0) {
            if ((pad_data & 0xF000F000) != 0) {
                cursor_cw_disp = cursor_cw_disp | 0x80;
            }
        }
    }

    if (date_sprite_on != 0 || temp20c != 0) {
        cursor_cw_disp &= 0x7F;
    }

    if (menu_on == 1) {
        cursor_cw_disp = (u8)(cursor_cw_disp | 0x80);
    }

    if (cursor_cw_disp & 0x80) {
        step_directional_scroll_once();
    }
}
