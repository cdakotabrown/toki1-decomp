/* @CompileEnv: IDO52_UGEN_TEMP_REG=3:v0,5:v0,8:v0,10:v0,12:v0,14:v0,16:v0 IDO52_UGEN_TEMP_REG_INPLACE=occ:3,5,8,10,12,14,16 */
/* @SymbolStart: 0x8013800C */
/* @SymbolSize: 1048 */
/* @RodataBase: 0x8013F79C */
#include "types.h"

extern u8 denwa_cnt;
extern u8 step_mode;
extern s32 get_g_name_tbl0;
extern s32 tel_ijuuin_hima_serifu_tbl;
extern s32 tel_ijuuin_hima_serifu_tbl_1;
extern s32 tel_ijuuin_hima_xa_tbl;
extern s32 tel_ijuuin_hima_xa_tbl2;
extern s32 tel_dialog40_str0;
extern s32 p_sei;
extern s32 tel_ijuuin_hima_serifu_tbl_2;
extern s32 tel_ijuuin_hima_serifu_tbl_3;
extern s32 tel_ijuuin_hima_serifu_tbl_4;
extern s32 tel_dialog40_str1;

extern void k_sub_reset(void);
extern void gnsx(s32);
extern void sndisp(s32, s32, s32);
extern void xa_set_m(s32, s32, s32, s32);
extern void k_sub_disp_start(s32);
extern void xa_wait_m(void);
extern void xa_wait(void);
extern void goto_sub_sub_step(s32);
extern void device_cmd_send();
extern void sndi(s32, s32, s32);
extern void text_sys_init(void);
extern void class_clear_f(void);
extern void set_dec_bri(s32);
extern void return_schedule(s32);
extern void menu_bar_show(s32);
extern void cal_base_show(void);

void tel_dialog40(void)
{
    u8 v1;

    switch (step_mode) {
        case 0:
            k_sub_reset();
            denwa_cnt = denwa_cnt + 1;
            gnsx(get_g_name_tbl0);
            sndisp(tel_ijuuin_hima_serifu_tbl, 0, 12);
            xa_set_m((s32)&tel_ijuuin_hima_xa_tbl, (s32)&tel_ijuuin_hima_xa_tbl2, 0, 12);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 1:
            xa_wait_m();
            break;
        case 2:
            k_sub_reset();
            gnsx((s32)&p_sei);
            sndisp((s32)&tel_dialog40_str0, 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 3:
            xa_wait();
            break;
        case 4:
            v1 = denwa_cnt;
            if ((u32)v1 >= 41) {

                switch ((u32)v1 % 3) {
                    case 0:
                        goto_sub_sub_step(67);
                        break;
                    case 1:
                        goto_sub_sub_step(68);
                        break;
                    case 2:
                        goto_sub_sub_step(69);
                        break;
                }
            } else if ((u32)v1 >= 31) {
                goto_sub_sub_step(66);
            } else if ((u32)v1 >= 21) {
                goto_sub_sub_step(65);
            } else {
                step_mode = step_mode + 1;
            }
            break;
        case 5:
            k_sub_reset();
            gnsx(get_g_name_tbl0);
            sndisp(tel_ijuuin_hima_serifu_tbl_1, 0, 12);
            xa_set_m((s32)&tel_ijuuin_hima_xa_tbl, (s32)&tel_ijuuin_hima_xa_tbl2, 1, 12);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 6:
            xa_wait_m();
            break;
        case 7:
            k_sub_reset();
            gnsx((s32)&p_sei);
            sndisp(tel_ijuuin_hima_serifu_tbl_2, 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 8:
            xa_wait();
            break;
        case 9:
            k_sub_reset();
            gnsx(get_g_name_tbl0);
            sndisp(tel_ijuuin_hima_serifu_tbl_3, 0, 12);
            xa_set_m((s32)&tel_ijuuin_hima_xa_tbl, (s32)&tel_ijuuin_hima_xa_tbl2, 3, 12);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 10:
            xa_wait_m();
            break;
        case 11:
            k_sub_reset();
            device_cmd_send(1291);
            sndi((s32)&tel_dialog40_str1, 0, 47);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 12:
            xa_wait();
            break;
        case 13:
            k_sub_reset();
            device_cmd_send(1292);
            gnsx((s32)&p_sei);
            sndisp(tel_ijuuin_hima_serifu_tbl_4, 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 14:
            xa_wait();
            break;
        case 15:
            text_sys_init();
            class_clear_f();
            set_dec_bri(0);
            return_schedule(0);
            break;
        default:
            goto_sub_sub_step(16);
            break;
    }
    menu_bar_show(0);
    cal_base_show();
}
