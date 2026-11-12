/* @SymbolSize: 352 */
/* geko_main -- GEKO overlay, Address: 0x80134040, Size: 352 bytes (88 insns) */
#include "types.h"

extern void bustup_ani(void);
extern void geko_step_dispatch(void);
extern void game_reset(void);
extern void k_scroll_set(void);
extern void k_disp_wait(void);
extern void update_anim_state(s32 a0);
extern void message_window_show(void);
extern void basyo_show(void);
extern void hizuke_show(void);
extern void dir_scroll_main(void);
extern void dec_bg_show(s32 a0);
extern void parameter_show(void);
extern void step_directional_scroll_once(void);

extern s32 pad_data;
extern u8 now_sub_step;
extern u8 cursor_cw_disp;
extern s32 date_sprite_on;
extern s32 script_on;
extern s32 menu_on;
extern s32 dec_bg_on;

void geko_main(void)
{
    u8 val;

    bustup_ani();
    if (now_sub_step == 0) {
        geko_step_dispatch();
    } else {
        game_reset();
    }
    k_scroll_set();
    k_disp_wait();
    if (dec_bg_on == 0) {
        update_anim_state(2);
    }
    message_window_show();
    basyo_show();
    hizuke_show();
    if (dec_bg_on == 0) {
        dir_scroll_main();
    }
    if (dec_bg_on != 0) {
        update_anim_state(1);
        dec_bg_show(0);
        parameter_show();
        if (script_on == 0) {
            if (date_sprite_on == 0) {
                if (((pad_data & 0xF000F000) != 0) ||
                    (menu_on == 1)) {
                    cursor_cw_disp = cursor_cw_disp | 0x80;
                }
            }
        }
        if (date_sprite_on != 0) {
            cursor_cw_disp = cursor_cw_disp & 0x7F;
        }
        if ((cursor_cw_disp & 0x80) != 0) {
            step_directional_scroll_once();
        }
    }
}
