/* Function: vacation_day_screen_init */
/* Address: 0x80074E94  (registered address corrected 2026-09-17; see below) */
/* Size: 324 bytes (81 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @SymbolStart: 0x80074E94 */
/* @SymbolSize: 324 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v1 */
#include "types.h"

extern u8 step_mode;
extern s32 Sprites;

extern void class_clear_f(void);
extern void set_dec_bri(s32);
extern void back_clear_switch(s32);
extern void tpage_buf_clear(void);
extern void hizuke_init(void);
extern void icon_init(void);
extern void icon_disp_switch(s32);
extern void icon_schedule_set(void);
extern void parameter_show_init(void);
extern void parameter_disp_switch(s32);
extern void message_window_init(void);
extern void text_sys_init(void);
extern void mes_window_switch(s32);
extern void k_disp_start(s32);
extern s32 vacation_day_bg_load(void);
extern void indexed_byte_lsb_set(s32, s32);
extern s32 get_CD_State2(void);
extern s32 dec_bg_init(s32);
extern void sub_sub_step_up(void);

void vacation_day_screen_init(void) {
    s32 result;

    switch ((u32)step_mode) {
    case 0:
        class_clear_f();
        set_dec_bri(128);
        back_clear_switch(1);
        tpage_buf_clear();
        hizuke_init();
        icon_init();
        icon_disp_switch(1);
        icon_schedule_set();
        parameter_show_init();
        parameter_disp_switch(1);
        message_window_init();
        Sprites = (s32)0x80000000;
        text_sys_init();
        mes_window_switch(0);
        k_disp_start(0);
        result = vacation_day_bg_load();
        if (result != -1) {
            if (result == 0 || result == 1) {
                step_mode = 2;
                indexed_byte_lsb_set(0, result);
                return;
            }
        } else {
            step_mode = 1;
        }
        break;
    case 1:
        if (get_CD_State2() & 1) {
            result = dec_bg_init(0);
            indexed_byte_lsb_set(0, result);
            step_mode = 2;
        }
        break;
    case 2:
    default:
        sub_sub_step_up();
        break;
    }
}
