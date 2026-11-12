/* Function: season_load_step */
/* Address: 0x80072A58 */
/* Size: 392 bytes (98 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @SymbolStart: 0x80072A58 */
/* @SymbolSize: 392 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: rename_register_chain --chain t2:t1:t0:t9:t8:t7:t6:v1 */
/* @PostProc: swap_branch_operands --offsets 0x9c 0xd4 */
#include "types.h"

extern u8 step_mode;
extern u8 now_month;
extern u8 now_year;
extern u8 now_day;
extern char season_load_step_str0[];
extern char season_load_step_str1[];
extern char p_sei[];
extern u8 anim_fade;

extern s32 cd_read_start2(s32, s32, s32);
extern s32 get_CD_State2(void);
extern void cal_font_load(u8, u8, s32);
extern s32 sub_step_up(void);
extern void text_sys_init(void);
extern void basyo_init2(char *);
extern void mes_window_switch(s32);
extern void gnsx(char *);
extern void sndisp(char *, s32, s32);
extern void k_disp_start(s32);
extern void device_cmd_send(s32);
extern void season_bgm_set(void);
extern void sub_sub_step_up(void);
extern s32 season_flag_set(void);

s32 season_load_step(void) {
    s32 mode = step_mode;
    s32 ret = mode;

    switch (mode) {
    case 0:
        ret = cd_read_start2(149, 0x801A0000, 0x9B94);
        step_mode++;
        break;
    case 1:
        ret = get_CD_State2();
        if ((ret & 1) == 0) {
            break;
        }
        cal_font_load(now_year, now_month, 1);
        {
            u32 month = now_month;
            if ((month % 3) == 0) {
                if (9 == month) {
                    if (now_day == 1) {
                        if (now_year == 96) {
                            sub_step_up();
                            goto common;
                        }
                    }
                }
                if (9 == month) {
                    text_sys_init();
                    basyo_init2(season_load_step_str0);
                    mes_window_switch(0);
                    gnsx(p_sei);
                    sndisp(season_load_step_str1, 0, 31);
                    k_disp_start(3);
                    device_cmd_send(193);
                    season_bgm_set();
                    sub_sub_step_up();
                } else {
                    device_cmd_send(193);
                    season_bgm_set();
                    sub_sub_step_up();
                }
common:
                ret = season_flag_set();
                anim_fade = 239;
                break;
            }
        }
        ret = sub_step_up();
        break;
    }
    return ret;
}
