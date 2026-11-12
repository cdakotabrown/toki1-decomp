/* Function: schedule_main39 */
/* Address: 0x80072670 */
/* Size: 320 bytes (80 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0:v1 --exclude 0xFC */
#include "types.h"

extern u8 now_sub_step;
extern u8 schedule_cursor;
extern u8 cw16_disp;
extern u8 mes_win_cw_disp;
extern u8 cw32_disp;
extern u8 mascot_cw_disp;
extern u8 cursor_cw_disp;
extern void _schedule_main39(void);
extern void goto_step(s32);
extern void hizuke_show(void);
extern void message_window_show(void);
extern void parameter_show(void);
extern void mascot_move(void);
extern void step_directional_scroll_once(void);
extern s32 get_last_gamen_mode(void);
extern void dec_bg_show(s32);
extern void icon_cursor_move(s32);
extern void update_anim_state(s32);

void schedule_main39(void) {
    s32 mode = now_sub_step;
    switch (mode) {
    case 0:
        _schedule_main39();
        break;
    case 1:
        schedule_cursor++;
        goto_step(49);
        break;
    }
    if (cw16_disp & 0x80) {
        hizuke_show();
    }
    if (mes_win_cw_disp & 0x80) {
        message_window_show();
    }
    if (cw32_disp & 0x80) {
        parameter_show();
    }
    if (mascot_cw_disp & 0x80) {
        mascot_move();
    }
    if (cursor_cw_disp & 0x80) {
        step_directional_scroll_once();
    }
    dec_bg_show(get_last_gamen_mode());
    parameter_show();
    message_window_show();
    hizuke_show();
    icon_cursor_move(0);
    mascot_move();
    update_anim_state(0);
}
