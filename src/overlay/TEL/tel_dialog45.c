/* @SymbolStart: 0x801392DC */
/* @RodataBase: 0x8013F8EC */
/* @SymbolSize: 836 */
/* tel_dialog45 - TEL overlay, Address: 0x801392E4, Size: 836 bytes (209 insns) */
#include "types.h"

extern u8  step_mode;
extern s32 get_g_name_tbl0;
extern s32 p_sei;
extern s32 tel_ijuuin_tukemono_serifu_tbl;
extern s32 tel_ijuuin_tukemono_serifu_tbl_1;
extern s32 tel_ijuuin_tukemono_serifu_tbl_2;
extern s32 tel_ijuuin_tukemono_serifu_tbl_3;
extern s32 tel_ijuuin_tukemono_serifu_tbl_4;
extern s32 tel_ijuuin_tukemono_serifu_tbl_5;
extern s32 tel_ijuuin_tukemono_xa_tbl[];
extern s32 tel_ijuuin_tukemono_xa_tbl2[];
extern s32 tel_dialog45_str0;

extern void k_sub_reset(void);
extern void gnsx(s32);
extern void sndisp(s32, s32, s32);
extern void xa_set_m(s32, s32, s32, s32);
extern void k_sub_disp_start(s32);
extern void xa_wait_m(void);
extern void xa_wait(void);
extern void sndi(s32, s32, s32);
extern void device_cmd_send(s32);
extern void text_sys_init(void);
extern void class_clear_f(void);
extern void set_dec_bri(s32);
extern void return_schedule();
extern void goto_sub_sub_step(s32);
extern void menu_bar_show(s32);
extern void cal_base_show(void);

void tel_dialog45(void)
{
    s32 hidden_t6 = step_mode;
    switch (hidden_t6) {
        case 0:
            k_sub_reset();
            gnsx(get_g_name_tbl0);
            sndisp(tel_ijuuin_tukemono_serifu_tbl, 0, 12);
            xa_set_m((s32)tel_ijuuin_tukemono_xa_tbl, (s32)tel_ijuuin_tukemono_xa_tbl2, 0, 12);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 1:
            xa_wait_m();
            break;
        case 2:
            k_sub_reset();
            gnsx((s32)&p_sei);
            sndisp(tel_ijuuin_tukemono_serifu_tbl_1, 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 3:
            xa_wait();
            break;
        case 4:
            k_sub_reset();
            gnsx(get_g_name_tbl0);
            sndisp(tel_ijuuin_tukemono_serifu_tbl_2, 0, 12);
            xa_set_m((s32)tel_ijuuin_tukemono_xa_tbl, (s32)tel_ijuuin_tukemono_xa_tbl2, 2, 12);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 5:
            xa_wait_m();
            break;
        case 6:
            k_sub_reset();
            gnsx((s32)&p_sei);
            sndi(tel_ijuuin_tukemono_serifu_tbl_3, 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 7:
            xa_wait();
            break;
        case 8:
            k_sub_reset();
            gnsx(get_g_name_tbl0);
            sndisp(tel_ijuuin_tukemono_serifu_tbl_4, 0, 12);
            xa_set_m((s32)tel_ijuuin_tukemono_xa_tbl, (s32)tel_ijuuin_tukemono_xa_tbl2, 4, 12);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 9:
            xa_wait_m();
            break;
        case 10:
            k_sub_reset();
            device_cmd_send(1292);
            sndi((s32)&tel_dialog45_str0, 0, 47);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;

        case 11:
            xa_wait();
            break;
        case 12:
            k_sub_reset();
            gnsx((s32)&p_sei);
            sndisp(tel_ijuuin_tukemono_serifu_tbl_5, 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 13:
            xa_wait();
            break;
        case 14:
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
