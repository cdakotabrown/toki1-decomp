/* Function: week_day_init0 */
/* Address: 0x80073B8C */
/* Size: 472 bytes (118 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 season_no;
extern u8 step_mode;
extern u16 byouki_state;
extern s32 Sprites;
extern s32 dec_bg_handle;

extern void day_flag_set(s32);
extern void prim_init(void);
extern void goto_sub_step(s32);
extern void class_clear_f(void);
extern void set_dec_bri(s32);
extern void back_clear_switch(s32);
extern void tpage_buf_clear(void);
extern void hizuke_init(void);
extern void icon_init(void);
extern void icon_disp_switch(s32);
extern void icon_schedule_set(void);
extern void parameter_show_init(void);
extern void cursor_init(s32);
extern void parameter_disp_switch(s32);
extern void message_window_init(void);
extern void cursor_image_load(void);
extern void sprite_state_set(void);
extern void text_sys_init(void);
extern void mes_window_switch(s32);
extern void k_disp_start(s32);
extern s32 week_day_bg_load(void);
extern void indexed_byte_lsb_set(s32, s32);
extern s32 get_CD_State2(void);
extern s32 dec_bg_init(s32);
extern void dec_bg_reset(void);
extern void sub_sub_step_up(void);

s32 week_day_init0(void) {
    s32 ret;

    day_flag_set(0);

    if (season_no != 0) {
        prim_init();
        goto_sub_step(10);
        return 0;
    }

    switch (step_mode) {
    case 0:
        class_clear_f();
        set_dec_bri(128);
        back_clear_switch(1);
        tpage_buf_clear();
        prim_init();
        hizuke_init();
        icon_init();
        icon_disp_switch(1);
        icon_schedule_set();
        parameter_show_init();
        cursor_init(0);
        parameter_disp_switch(1);
        message_window_init();
        cursor_image_load();
        if (byouki_state & 1) {
            Sprites = (s32)0x80000000;
        } else {
            sprite_state_set();
        }
        text_sys_init();
        mes_window_switch(0);
        k_disp_start(0);
        ret = week_day_bg_load();
        if (ret != -1) {
            if (ret == 0 || ret == 1) {
                step_mode = 2;
                indexed_byte_lsb_set(0, ret);
            }
        } else {
            step_mode = 1;
        }
        break;
    case 1:
        if (get_CD_State2() & 1) {
            ret = dec_bg_init(0);
            indexed_byte_lsb_set(0, ret);
            if (dec_bg_handle == -1) {
                dec_bg_reset();
                week_day_bg_load();
            } else {
                step_mode = 2;
            }
        }
        break;
    default:
    case 2:
        sub_sub_step_up();
        break;
    }
}
