/* Function: overlay_load_disp */
/* Address: 0x80078B2C  -> window now starts at 0x80078B24 */
/* Size: 428 bytes (107 instructions)  -> window 436 bytes / 109 insns */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x80078B24 */
/* @SymbolSize: 436 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots 28:40 */
#include "types.h"

extern void hizuke_show(void);
extern void message_window_show(void);
extern void parameter_show(void);
extern void mascot_move(void);
extern void step_directional_scroll_once(void);
extern void cal_base_show(void);
extern void update_anim_state(s32);
extern void date_light_effect(void);
extern void basyo_show(void);
extern s32 get_last_gamen_mode(void);
extern void dec_bg_show(s32);
extern void goto_step_bg_out(void);
extern void back_clear_switch(s32);

extern u8 cw16_disp;
extern u8 mes_win_cw_disp;
extern u8 cw32_disp;
extern u8 mascot_cw_disp;
extern u8 cursor_cw_disp;
extern u8 prev_step;
extern u8 overlay_load_disp_tbl0;
extern s32 date_disp_flag;

void overlay_load_disp(void) {
    s32 v0;

    v0 = 0;
    if (cw16_disp & 0x80) {
        hizuke_show();
        v0 = 1;
    }
    if (mes_win_cw_disp & 0x80) {
        message_window_show();
        v0 = v0 + 1;
    }
    if (cw32_disp & 0x80) {
        parameter_show();
        v0 = v0 + 1;
    }
    if (mascot_cw_disp & 0x80) {
        mascot_move();
        v0 = v0 + 1;
    }
    if (cursor_cw_disp & 0x80) {
        step_directional_scroll_once();
        v0 = v0 + 1;
    }
    if (prev_step == 0x45) {
        cal_base_show();
    }
    if (v0 != 0) {
        update_anim_state(0);
        switch (overlay_load_disp_tbl0) {
        case 0:
            if (date_disp_flag & 1) {
                date_light_effect();
            }
            if (date_disp_flag & 2) {
                basyo_show();
            }
            dec_bg_show(get_last_gamen_mode());
            break;
        case 1:
            goto_step_bg_out();
            dec_bg_show(1);
            break;
        default:
            back_clear_switch(1);
            break;
        }
    }
}
