/* Function: _schedule_main39 */
/* Address: 0x800727B0 */
/* Size: 436 bytes (109 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=4:v1,14:v1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 now_sub_sub_step;
extern u8 season_no;
extern u8 now_day;
extern u8 now_month;

extern u8 _schedule_main39_str0;
extern u8 _schedule_main39_str1;
extern u8 _schedule_main39_str2;
extern u8 _schedule_main39_str3;
extern u8 _schedule_main39_str4;
extern u8 _schedule_main39_str5;
extern u8 p_sei;

extern void text_sys_init(void);
extern void basyo_init2(void *);
extern void mes_window_switch(s32);
extern void gnsx(void *);
extern void sndisp(void *, s32, s32);
extern void season_bgm_set(void);
extern void k_disp_start(s32);
extern void sub_sub_step_up(void);
extern s32 device_load_poll(void);
extern void device_port_init(s32, s32);
extern void sub_step_up(void);
extern void game_reset(void);

void _schedule_main39(void) {
    s32 state = now_sub_sub_step;
    u32 f;

    switch (state) {
    case 0:
        text_sys_init();
        basyo_init2(&_schedule_main39_str0);
        mes_window_switch(0);
        gnsx(&p_sei);
        f = season_no;
        if (f != 0 && now_day == 2) {
            season_no = 0;
            sndisp(&_schedule_main39_str1, 0, 31);
        } else if (f != 0) {
            season_no = 0;
            sndisp(&_schedule_main39_str2, 0, 31);
        } else if (now_month == 7) {
            sndisp(&_schedule_main39_str3, 0, 31);
            season_no = f = 1;
        } else if (now_month == 12) {
            sndisp(&_schedule_main39_str4, 0, 31);
            season_no = f = 2;
        } else if (now_month == 3) {
            sndisp(&_schedule_main39_str5, 0, 31);
            season_no = f = 3;
        } else {
            season_no = 0;
        }
        season_bgm_set();
        k_disp_start(3);
        sub_sub_step_up();
        break;
    case 1:
        if (device_load_poll() == 1) {
            device_port_init(0, 0);
            sub_step_up();
        }
        break;
    default:
        game_reset();
        break;
    }
}
