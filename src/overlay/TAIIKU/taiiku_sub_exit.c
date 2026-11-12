/* @SymbolStart: 0x80133494 */
/* @SymbolSize: 432 */
/* @RodataBase: 0x80148BF4 */
#include "types.h"

extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void device_cmd_send(s32 a0);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void game_reset(void);
extern void class_clear_all(s32 a0);
extern void debug_font_load(void);
extern void text_sys_init(void);
extern void k_reset(s32 a0);
extern void set_dec_bri(s32 a0);
extern void dec_bg_reset(void);
extern void default_data_vram_load(void);
extern void boot_data_load(void);
extern void parameter_disp_switch(s32 a0);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void icon_disp_switch(s32 a0);
extern void mes_window_switch(s32 a0);
extern void basyo_init(s32 *a0);
extern void mascot_disp_switch(s32 a0);
extern void cal_font_load(s32 a0, s32 a1, s32 a2);
extern void cursor_init(s32 a0);
extern void set_flag_b3_high_bit(s32 a0);
extern void date_name_set(s32 a0);
extern void yazirushi_off(void);
extern void common_disp(void);

extern s32 taiiku_sub_exit_str0[];

extern u8 screen_bri;
extern u8 now_year;
extern u8 now_month;

extern u8 now_sub_sub_step;

void taiiku_sub_exit(void) {
    s32 v0;

    switch (now_sub_sub_step) {
    case 0:
        device_cmd_send(191);
        class_clear_all(0);
        sub_sub_step_up();
        break;
    case 1:
        boot_data_load();
        break;
    case 2:
        default_data_vram_load();
        break;
    case 3:
        debug_font_load();
        dec_bg_reset();
        set_dec_bri(0);
        cursor_init(0);
        yazirushi_off();
        hizuke_init();
        message_window_init();
        date_name_set(0);
        text_sys_init();
        k_reset(0);
        basyo_init(taiiku_sub_exit_str0);
        mes_window_switch(0);
        screen_bri = 128;
        common_disp();
        sub_sub_step_up();
        break;
    case 4:
        cd_read_start2(149, 0x801A0000, 0x9B94);
        common_disp();
        sub_sub_step_up();
        break;
    case 5:
        v0 = get_CD_State2();
        if (v0 & 1) {
            cal_font_load(now_year, now_month, 0);
            sub_sub_step_up();
        }
        common_disp();
        break;
    case 6:
        common_disp();
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }

    mascot_disp_switch(0);
    set_flag_b3_high_bit(0);
    icon_disp_switch(0);
    parameter_disp_switch(0);
}
