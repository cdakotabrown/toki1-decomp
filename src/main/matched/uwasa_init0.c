/* Function: uwasa_init0 */
/* Address: 0x80060B90 */
/* Size: 300 bytes (75 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void text_sys_init(void);
extern void basyo_init(u8 *);
extern void gnsx(u8 *);
extern void parameter_show_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void parameter_disp_switch(s32);
extern void cursor_init(s32);
extern void icon_init(void);
extern void icon_disp_switch(s32);
extern void icon_schedule_set(void);
extern void icon_can_use_set(s32, s32);
extern void tpage_buf_clear(void);
extern void sndisp(u8 *, s32, s32);
extern void k_disp_start(s32);
extern s32 get_season(void);
extern s32 value_cache_find_or_load(s32, s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void set_dec_bri(s32);
extern void sub_step_up(void);
extern void sub_sub_step_up(void);

extern u8 uwasa_init0_str0[];
extern u8 uwasa_init0_str1[];
extern u8 p_sei[];
extern s32 step34_load_tbl0[];
extern s32 sched_icon;

void uwasa_init0(void) {
    s32 idx;
    s32 result;

    text_sys_init();
    basyo_init(uwasa_init0_str0);
    gnsx(p_sei);
    parameter_show_init();
    hizuke_init();
    message_window_init();
    parameter_disp_switch(1);
    cursor_init(0);
    icon_init();
    icon_disp_switch(1);
    icon_schedule_set();
    icon_can_use_set(0, 0);
    icon_can_use_set(sched_icon & 0xf, 1);
    tpage_buf_clear();
    sndisp(uwasa_init0_str1, 0, 31);
    k_disp_start(1);
    idx = get_season();
    result = value_cache_find_or_load(step34_load_tbl0[idx], 0);
    if (result != -1) {
        if (result == 0 || result == 1) {
            indexed_byte_lsb_set(0, result);
            set_dec_bri(128);
            sub_step_up();
        }
    } else {
        sub_sub_step_up();
    }
    k_disp_start(2);
}
