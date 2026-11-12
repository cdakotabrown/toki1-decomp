/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 IDO52_FORCE_WEB_COLOR=0:a3,2:v1 */
/* @SymbolStart: 0x8013B47C */
/* @PostProc: swap_instructions --offset1 0x1e4 --offset2 0x1ec */
/* @PostProc: swap_instructions --offset1 0x1e8 --offset2 0x1f0 */
/* @PostProc: swap_instructions --offset1 0x1ec --offset2 0x1f4 */
/* @PostProc: swap_instructions --offset1 0x1f0 --offset2 0x1f4 */
/* @PostProc: swap_instructions --offset1 0x1f8 --offset2 0x200 */
/* @PostProc: swap_instructions --offset1 0x1fc --offset2 0x200 */
/* @PostProc: remap_stack_slots --map '340:332,80:76,48:44' */
/* @RodataBase: 0x801400F8 */
/* @SymbolSize: 2192 */
/* Address: 0x8013B484, Size: 2184 bytes (546 insns) + absorbed 8-byte */
#include "types.h"

extern u8 step_mode;
extern u32 step_counter;
extern s32 reserved_date;

extern u8 p_sei[];

extern u8 tel_yoshio_reserved_dialog_str0[];
extern u8 tel_yoshio_reserved_dialog_str1[];
extern u8 tel_yoshio_reserved_dialog_str2[];
extern u8 tel_yoshio_reserved_dialog_str3[];
extern u8 tel_yoshio_reserved_dialog_str4[];

extern u8 *tel_uwasa_serifu_tbl;
extern u8 *tel_uwasa_serifu_tbl_1;
extern u8 *tel_uwasa_serifu_tbl_2;
extern u8 *tel_uwasa_serifu_tbl_3;
extern u8 *tel_uwasa_serifu_tbl_4;
extern u8 *tel_uwasa_serifu_tbl_5;
extern u8 *tel_uwasa_serifu_tbl_6;
extern u8 tel_uwasa_xa_tbl[];
extern u8 tel_uwasa_xa_tbl2[];
extern u8 *tel_yoshio_kirai_serifu_tbl[];
extern u8 tel_yoshio_kirai_xa_tbl[];
extern u8 tel_yoshio_kirai_xa_tbl2[];
extern u8 *tel_yumi_kirai_serifu_tbl_2;
extern u8 *tel_yumi_kirai_serifu_tbl_3;
extern u8 *tel_yumi_kirai_serifu_tbl_4;
extern u8 *tel_yumi_kirai_serifu_tbl_5;
extern u8 *tel_yumi_kirai_serifu_tbl_6;
extern u8 tel_yumi_kirai_xa_tbl[];
extern u8 tel_yumi_kirai_xa_tbl2[];
extern s32 get_g_name_tbl0[];

extern s32 sub_sub_step_up(void);
extern s32 device_cmd_send(s32);
extern void device_port_init(s32, s32);
extern void k_sub_reset(void);
extern s32 k_sub_disp_start(s32);
extern s32 k_disp_inc(void);
extern void gnsx(s32);
extern void sndisp(u8 *, s32, s32);
extern void sndi(u8 *, s32, s32);
extern void get_g_name(u8 *, s32);
extern s32 xa_wait(void);
extern s32 xa_set_m(u8 *, u8 *, s32, s32);
extern s32 xa_wait_m(void);
extern void strcpy(u8 *, u8 *);
extern void strcat(u8 *, u8 *);

s32 tel_yoshio_reserved_dialog(void)
{
    s32 idx;
    s32 ret;
    u8 buf[256];
    u8 num[32];

    ret = step_mode;
    switch ((u32)step_mode) {
    case 0:
        ret = k_disp_inc();
        if (step_counter >= 76) {
            step_counter = 0;
            step_mode += 1;
        }
        break;
    case 1:
        k_sub_reset();
        device_cmd_send(1289);
        sndi(tel_yoshio_reserved_dialog_str0, 0, 47);
        ret = k_sub_disp_start(1);
        step_mode += 1;
        break;
    case 2:
        ret = k_disp_inc();
        if (step_counter >= 76) {
            step_mode += 1;
        }
        break;
    case 3:
        k_sub_reset();
        device_cmd_send(1290);
        gnsx(get_g_name_tbl0[1]);
        sndisp(tel_uwasa_serifu_tbl, 0, 13);
        xa_set_m(tel_uwasa_xa_tbl, tel_uwasa_xa_tbl2, 0, 13);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 4:
        ret = xa_wait_m();
        break;
    case 5:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp(tel_uwasa_serifu_tbl_1, 0, 15);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        if (((u32)(reserved_date << 3) >> 28) == 10) {
            step_mode = 30;
        }
        break;
    case 6:
        ret = xa_wait();
        break;
    case 7:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndisp(tel_uwasa_serifu_tbl_2, 0, 13);
        xa_set_m(tel_uwasa_xa_tbl, tel_uwasa_xa_tbl2, 2, 13);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 8:
        ret = xa_wait_m();
        break;
    case 9:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp(tel_uwasa_serifu_tbl_3, 0, 15);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 10:
        ret = xa_wait();
        break;
    case 11:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndisp(tel_uwasa_serifu_tbl_4, 0, 15);
        xa_set_m(tel_uwasa_xa_tbl, tel_uwasa_xa_tbl2, 4, 13);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 12:
        ret = xa_wait_m();
        break;
    case 13:
        k_sub_reset();
        idx = (u32)(reserved_date << 3) >> 28;
        gnsx(get_g_name_tbl0[1]);
        sndisp(tel_yoshio_kirai_serifu_tbl[idx], 0, 13);
        xa_set_m(tel_yoshio_kirai_xa_tbl, tel_yoshio_kirai_xa_tbl2, idx, 13);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 14:
        ret = xa_wait_m();
        break;
    case 15:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp(tel_uwasa_serifu_tbl_5, 0, 15);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 16:
        ret = xa_wait();
        break;
    case 17:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndisp(tel_uwasa_serifu_tbl_6, 0, 13);
        ret = xa_set_m(tel_uwasa_xa_tbl, tel_uwasa_xa_tbl2, 6, 13);
        step_mode += 1;
        break;
    case 18:
        ret = xa_wait_m();
        break;
    case 19:
        step_mode = 240;
        break;
    case 30:
        ret = xa_wait();
        break;
    case 31:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndisp(tel_yumi_kirai_serifu_tbl_2, 0, 13);
        xa_set_m(tel_yumi_kirai_xa_tbl, tel_yumi_kirai_xa_tbl2, 2, 13);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 32:
        ret = xa_wait_m();
        break;
    case 33:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp(tel_yumi_kirai_serifu_tbl_3, 0, 15);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 34:
        ret = xa_wait();
        break;
    case 35:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndisp(tel_yumi_kirai_serifu_tbl_4, 0, 13);
        xa_set_m(tel_yumi_kirai_xa_tbl, tel_yumi_kirai_xa_tbl2, 4, 13);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 36:
        ret = xa_wait_m();
        break;
    case 37:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp(tel_yumi_kirai_serifu_tbl_5, 0, 15);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 38:
        ret = xa_wait();
        break;
    case 39:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndisp(tel_yumi_kirai_serifu_tbl_6, 0, 13);
        xa_set_m(tel_yumi_kirai_xa_tbl, tel_yumi_kirai_xa_tbl2, 6, 13);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 40:
        ret = xa_wait_m();
        break;
    case 41:
        step_mode = 240;
        break;
    case 240:
        ret = device_cmd_send(1291);
        step_mode += 1;
        break;
    case 241:
        k_sub_reset();
        gnsx((s32)p_sei);
        strcpy(buf, tel_yoshio_reserved_dialog_str1);
        get_g_name(num, (u32)(reserved_date << 3) >> 28);
        strcat(buf, num);
        strcat(buf, tel_yoshio_reserved_dialog_str2);
        sndisp(buf, 0, 31);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 242:
        ret = xa_wait();
        break;
    case 243:
        k_sub_reset();
        gnsx((s32)p_sei);
        device_port_init(0, 514);
        sndisp(tel_yoshio_reserved_dialog_str3, 0, 31);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 244:
        ret = xa_wait();
        break;
    case 245:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp(tel_yoshio_reserved_dialog_str4, 0, 31);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 246:
        ret = xa_wait();
        break;
    default:
        ret = sub_sub_step_up();
        break;
    }
    return ret;
}
