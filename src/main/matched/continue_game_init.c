/* Function: continue_game_init */
/* Address: 0x8005DA4C */
/* Size: 424 bytes (106 instructions)  -> window is now 432 bytes / 108 insns */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @SymbolStart: 0x8005DA44 */
/* @SymbolSize: 432 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_AS1_PEEPREG_KEEP=0 */
#include "types.h"

extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 rec_idx;
extern u8 step_mode;

extern void _game_work_init_set(void);
extern void work_clear(void);
extern void goto_step(s32);
extern void goto_sub_step(s32);
extern void device_cmd_send(s32);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern void parameter_disp_switch(s32);
extern void hizuke_disp_switch(s32);
extern void message_disp_switch(s32);
extern void mes_window_switch(s32);
extern void mascot_disp_switch(s32);
extern void season_bgm_set(void);
extern void device_port_init(s32, s32);

s32 continue_game_init(void) {
    u32 mode = step_mode;

    switch (mode) {
    case 0:
        if (now_year == 98 && now_month == 3 && now_day == 1) {
            step_mode = step_mode + 1;
        } else if (now_year == 95 && now_month == 4 && now_day == 4) {
            step_mode = step_mode + 1;
        } else {
            device_cmd_send(177);
            step_mode = 192;
        }
        break;

    case 1:
        device_cmd_send(5);
        device_cmd_send(177);
        device_load_start(0, 0xBFA6, 0xBF87, 0xCD2F, 0xCCE7, 0xCCDF);
        step_mode = step_mode + 1;
        break;

    case 2:
        if (device_load_poll() == 1) {
            _game_work_init_set();
            work_clear();
            rec_idx = 0;
            parameter_disp_switch(0);
            hizuke_disp_switch(0);
            message_disp_switch(0);
            mascot_disp_switch(0);
            device_port_init(0, 0);
            goto_step(20);
        }
        break;

    case 255:
        mes_window_switch(0);
        goto_step(52);
        goto_sub_step(255);
        season_bgm_set();
        break;

    default:
        step_mode = mode + 1;
        break;
    }
}
