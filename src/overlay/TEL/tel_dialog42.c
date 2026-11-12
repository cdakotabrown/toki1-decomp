/* @RodataBase: 0x8013F818 */
/* @SymbolStart: 0x80138770 */
/* @SymbolSize: 1252 */
/* tel_dialog42 - TEL overlay, Address: 0x80138778 (true entry 0x80138770) */
#include "types.h"

extern s32 get_g_name_tbl0;
extern s32 p_sei;
extern u8 step_mode;
extern s32 tel_ijuuin_jiman_serifu_tbl;
extern s32 tel_ijuuin_jiman_serifu_tbl_1;
extern s32 tel_ijuuin_jiman_serifu_tbl_2;
extern s32 tel_ijuuin_jiman_serifu_tbl_3;
extern s32 tel_ijuuin_jiman_serifu_tbl_4;
extern s32 tel_ijuuin_jiman_serifu_tbl_5;
extern s32 tel_ijuuin_jiman_serifu_tbl_6;
extern s32 tel_ijuuin_jiman_serifu_tbl_7;
extern s32 tel_ijuuin_jiman_serifu_tbl_8;
extern s32 tel_ijuuin_jiman_serifu_tbl_9;
extern s32 tel_ijuuin_jiman_xa_tbl;
extern s32 tel_ijuuin_jiman_xa_tbl2;
extern s32 tel_dialog42_str0;

extern void device_cmd_send(s32);
extern void class_clear_f(void);
extern void text_sys_init(void);
extern void k_sub_reset(void);
extern void k_sub_disp_start(s32);
extern void goto_sub_sub_step(s32);
extern void menu_bar_show(s32);
extern void sndi(s32, s32, s32);
extern void gnsx(s32);
extern void sndisp(s32, s32, s32);
extern void xa_set_m(s32 *, s32 *, s32, s32);
extern void xa_wait_m(void);
extern void xa_wait(void);
extern void set_dec_bri(s32);
extern void cal_base_show(void);
extern void return_schedule(s32);

void tel_dialog42(void)
{
    switch (step_mode) {
    case 0:
        k_sub_reset();
        gnsx(get_g_name_tbl0);
        sndisp(tel_ijuuin_jiman_serifu_tbl, 0, 12);
        xa_set_m(&tel_ijuuin_jiman_xa_tbl, &tel_ijuuin_jiman_xa_tbl2, 0, 12);
        k_sub_disp_start(0);
        step_mode++;
        break;
    case 1:
        xa_wait_m();
        break;
    case 2:
        k_sub_reset();
        gnsx((s32)&p_sei);
        sndisp(tel_ijuuin_jiman_serifu_tbl_1, 0, 15);
        k_sub_disp_start(0);
        step_mode++;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(get_g_name_tbl0);
        sndisp(tel_ijuuin_jiman_serifu_tbl_2, 0, 12);
        xa_set_m(&tel_ijuuin_jiman_xa_tbl, &tel_ijuuin_jiman_xa_tbl2, 2, 12);
        k_sub_disp_start(0);
        step_mode++;
        break;
    case 5:
        xa_wait_m();
        break;
    case 6:
        k_sub_reset();
        gnsx((s32)&p_sei);
        sndi(tel_ijuuin_jiman_serifu_tbl_3, 0, 15);
        k_sub_disp_start(0);
        step_mode++;
        break;
    case 7:
        xa_wait();
        break;
    case 8:
        k_sub_reset();
        gnsx(get_g_name_tbl0);
        sndisp(tel_ijuuin_jiman_serifu_tbl_4, 0, 12);
        xa_set_m(&tel_ijuuin_jiman_xa_tbl, &tel_ijuuin_jiman_xa_tbl2, 4, 12);
        k_sub_disp_start(0);
        step_mode++;
        break;
    case 9:
        xa_wait_m();
        break;
    case 10:
        k_sub_reset();
        gnsx((s32)&p_sei);
        sndisp(tel_ijuuin_jiman_serifu_tbl_5, 0, 15);
        k_sub_disp_start(0);
        step_mode++;

    case 11:
        xa_wait();
        break;
    case 12:
        k_sub_reset();
        gnsx(get_g_name_tbl0);
        sndisp(tel_ijuuin_jiman_serifu_tbl_6, 0, 12);
        xa_set_m(&tel_ijuuin_jiman_xa_tbl, &tel_ijuuin_jiman_xa_tbl2, 6, 12);
        k_sub_disp_start(0);
        step_mode++;
        break;
    case 13:
        xa_wait_m();
        break;
    case 14:
        k_sub_reset();
        gnsx((s32)&p_sei);
        sndisp(tel_ijuuin_jiman_serifu_tbl_7, 0, 15);
        k_sub_disp_start(0);
        step_mode++;
        break;
    case 15:
        xa_wait();
        break;
    case 16:
        k_sub_reset();
        gnsx(get_g_name_tbl0);
        sndisp(tel_ijuuin_jiman_serifu_tbl_8, 0, 12);
        xa_set_m(&tel_ijuuin_jiman_xa_tbl, &tel_ijuuin_jiman_xa_tbl2, 8, 12);
        k_sub_disp_start(0);
        step_mode++;
        break;
    case 17:
        xa_wait_m();
        break;
    case 18:
        k_sub_reset();
        device_cmd_send(1292);
        sndi((s32)&tel_dialog42_str0, 0, 47);
        k_sub_disp_start(0);
        step_mode++;
        break;
    case 19:
        xa_wait();
        break;
    case 20:
        k_sub_reset();
        gnsx((s32)&p_sei);
        sndisp(tel_ijuuin_jiman_serifu_tbl_9, 0, 15);
        k_sub_disp_start(0);
        step_mode++;
        break;
    case 21:
        xa_wait();
        break;
    case 22:
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
