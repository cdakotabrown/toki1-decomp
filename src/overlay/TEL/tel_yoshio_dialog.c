/* @SymbolStart: 0x801399D4 */
/* @RodataBase: 0x8013FE48 */
/* @SymbolSize: 920 */
/* tel_yoshio_dialog - TEL overlay, Address: 0x801399DC, Size: 1004 bytes (251 insns) */
#include "types.h"

extern u8 step_mode;
extern s32 step_counter;
extern s32 p_sei;
extern s32 tel_yumi_kirai_serifu_tbl;
extern s32 tel_yumi_kirai_serifu_tbl_1;
extern s32 tel_yumi_kirai_serifu_tbl_2;
extern s32 tel_yumi_kirai_serifu_tbl_3;
extern s32 tel_yumi_kirai_serifu_tbl_4;
extern s32 tel_yumi_kirai_serifu_tbl_5;
extern s32 tel_yumi_kirai_serifu_tbl_6;
extern u8 tel_yumi_kirai_xa_tbl;
extern u8 tel_yumi_kirai_xa_tbl2;
extern s32 get_g_name_tbl0[];

extern void k_disp_inc(void);
extern void device_cmd_send(s32);
extern void k_sub_reset(void);
extern void gnsx(s32);
extern void sndisp(s32, s32, s32);
extern void xa_set_m(s32, s32, s32, s32);
extern void k_sub_disp_start(s32);
extern void xa_wait_m(void);
extern void xa_wait(void);
extern void goto_sub_sub_step(s32);

void tel_yoshio_dialog(void)
{
    u8 hidden_t6;

    hidden_t6 = step_mode;
    switch (hidden_t6) {
        case 0:
            k_disp_inc();
            if ((u32)step_counter >= 76) {
                step_mode = step_mode + 1;
                step_counter = 0;
                device_cmd_send(1290);
                return;
            }
            break;
        case 1:
            k_sub_reset();
            gnsx(get_g_name_tbl0[1]);
            sndisp(tel_yumi_kirai_serifu_tbl, 0, 13);
            xa_set_m((s32)&tel_yumi_kirai_xa_tbl, (s32)&tel_yumi_kirai_xa_tbl2, 0, 13);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 2:
            xa_wait_m();
            return;
        case 3:
            k_sub_reset();
            gnsx((s32)&p_sei);
            sndisp(tel_yumi_kirai_serifu_tbl_1, 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 4:
            xa_wait();
            return;
        case 5:
            k_sub_reset();
            gnsx(get_g_name_tbl0[1]);
            sndisp(tel_yumi_kirai_serifu_tbl_2, 0, 13);
            xa_set_m((s32)&tel_yumi_kirai_xa_tbl, (s32)&tel_yumi_kirai_xa_tbl2, 2, 13);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 6:
            xa_wait_m();
            return;
        case 7:
            k_sub_reset();
            gnsx((s32)&p_sei);
            sndisp(tel_yumi_kirai_serifu_tbl_3, 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 8:
            xa_wait();
            return;
        case 9:
            k_sub_reset();
            gnsx(get_g_name_tbl0[1]);
            sndisp(tel_yumi_kirai_serifu_tbl_4, 0, 13);
            xa_set_m((s32)&tel_yumi_kirai_xa_tbl, (s32)&tel_yumi_kirai_xa_tbl2, 4, 13);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 10:
            xa_wait_m();
            return;
        case 11:
            k_sub_reset();
            gnsx((s32)&p_sei);
            sndisp(tel_yumi_kirai_serifu_tbl_5, 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 12:
            xa_wait();
            return;
        case 13:
            k_sub_reset();
            gnsx(get_g_name_tbl0[1]);
            sndisp(tel_yumi_kirai_serifu_tbl_6, 0, 13);
            xa_set_m((s32)&tel_yumi_kirai_xa_tbl, (s32)&tel_yumi_kirai_xa_tbl2, 6, 13);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 14:
            xa_wait_m();
            return;
        case 15:
            device_cmd_send(1291);
            step_mode = step_mode + 1;
            break;
        default:
            goto_sub_sub_step(2);
            break;
    }
}
