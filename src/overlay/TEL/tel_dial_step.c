/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 IDO52_FORCE_WEB_COLOR=0:a3,2:v1 */
/* @SymbolStart: 0x8013ACF0 */
/* @RodataBase: 0x80140014 */
/* @SymbolSize: 1932 */
/* Address: 0x8013ACF8, Size: 1924 bytes (481 insns) + absorbed 8-byte */
#include "types.h"

extern u8 step_mode;
extern u32 step_counter;

extern u8 p_sei[];

extern u8 tel_dial_step_str0[];
extern u8 tel_dial_step_str1[];
extern u8 tel_dial_step_str2[];
extern u8 tel_dial_step_str3[];
extern u8 tel_dial_step_str4[];
extern u8 tel_dial_step_str5[];

extern u8 *tel_yumi_kyohi_serifu_tbl;
extern u8 *tel_yumi_kyohi_serifu_tbl_1;
extern u8 *tel_yumi_kyohi_serifu_tbl_2;
extern u8 *tel_yumi_kyohi_serifu_tbl_3;
extern u8 tel_yumi_kyohi_xa_tbl[];
extern u8 tel_yumi_kyohi_xa_tbl2[];

extern u8 *tel_uwasa_serifu_tbl;
extern u8 *tel_uwasa_serifu_tbl_1;
extern u8 *tel_uwasa_serifu_tbl_2;
extern u8 *tel_uwasa_serifu_tbl_3;
extern u8 *tel_uwasa_serifu_tbl_4;
extern u8 *tel_uwasa_serifu_tbl_6;
extern u8 tel_uwasa_xa_tbl[];
extern u8 tel_uwasa_xa_tbl2[];
extern s32 get_g_name_tbl0[];

extern s32 sub_sub_step_up(void);
extern s32 device_cmd_send(s32);
extern s32 device_port_init(s32, s32);
extern u8 *get_k_work(s32);
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
extern s32 get_tel_ring_girl(void);

s32 tel_dial_step(void)
{
    s32 ret;
    u8 buf[256];
    u8 num[32];
    u8 *p;

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
        sndi(tel_dial_step_str0, 0, 47);
        ret = k_sub_disp_start(1);
        step_mode += 1;
        break;
    case 2:
        ret = k_disp_inc();
        if (step_counter >= 76) {
            step_counter = 0;
            step_mode += 1;
        }
        break;
    case 3:
        k_sub_reset();
        device_cmd_send(1289);
        sndi(tel_dial_step_str1, 0, 47);
        ret = k_sub_disp_start(1);
        step_mode += 1;
        break;
    case 4:
        ret = k_disp_inc();
        if (step_counter >= 76) {
            step_mode += 1;
        }
        break;
    case 5:
        k_sub_reset();
        device_cmd_send(1290);
        gnsx(get_g_name_tbl0[1]);
        sndisp(tel_uwasa_serifu_tbl, 0, 13);
        xa_set_m(tel_uwasa_xa_tbl, tel_uwasa_xa_tbl2, 0, 13);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 6:
        ret = xa_wait_m();
        break;
    case 7:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp(tel_uwasa_serifu_tbl_1, 0, 15);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 8:
        ret = xa_wait();
        break;
    case 9:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndisp(tel_uwasa_serifu_tbl_2, 0, 13);
        xa_set_m(tel_uwasa_xa_tbl, tel_uwasa_xa_tbl2, 2, 13);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 10:
        ret = xa_wait_m();
        break;
    case 11:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp(tel_uwasa_serifu_tbl_3, 0, 15);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 12:
        ret = xa_wait();
        break;
    case 13:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndisp(tel_uwasa_serifu_tbl_4, 0, 13);
        xa_set_m(tel_uwasa_xa_tbl, tel_uwasa_xa_tbl2, 4, 13);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 14:
        ret = xa_wait_m();
        break;
    case 15:
        k_sub_reset();
        sndisp(tel_dial_step_str2, 0, 47);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 16:
        ret = xa_wait();
        break;
    case 17:
        device_port_init(0, 513);
        k_sub_reset();
        gnsx((s32)p_sei);
        strcpy(buf, tel_dial_step_str3);
        get_g_name(num, get_tel_ring_girl());
        strcat(buf, num);
        strcat(buf, tel_dial_step_str4);
        sndisp(buf, 0, 31);
        ret = k_sub_disp_start(0);
        p = get_k_work(3);
        p[4] = 1;
        step_mode += 1;
        break;
    case 18:
        ret = xa_wait();
        break;
    case 19:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp(tel_dial_step_str5, 0, 31);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 20:
        ret = xa_wait();
        break;
    case 21:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndisp(tel_uwasa_serifu_tbl_6, 0, 13);
        ret = xa_set_m(tel_uwasa_xa_tbl, tel_uwasa_xa_tbl2, 6, 13);
        step_mode += 1;
        break;
    case 22:
        ret = xa_wait_m();
        break;
    case 23:
        step_mode = 240;
        break;
    case 208:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndisp(tel_yumi_kyohi_serifu_tbl, 0, 13);
        xa_set_m(tel_yumi_kyohi_xa_tbl, tel_yumi_kyohi_xa_tbl2, 0, 13);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 209:
        ret = xa_wait_m();
        break;
    case 210:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp(tel_yumi_kyohi_serifu_tbl_1, 0, 13);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 211:
        ret = xa_wait();
        break;
    case 212:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndisp(tel_yumi_kyohi_serifu_tbl_2, 0, 13);
        xa_set_m(tel_yumi_kyohi_xa_tbl, tel_yumi_kyohi_xa_tbl2, 2, 13);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 213:
        ret = xa_wait_m();
        break;
    case 214:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp(tel_yumi_kyohi_serifu_tbl_3, 0, 15);
        ret = k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 215:
        ret = xa_wait();
        break;
    case 216:
        ret = device_cmd_send(1291);
        step_mode = 240;
        break;
    case 240:
        device_cmd_send(1291);
        ret = device_port_init(0, 512);
        step_mode += 1;
        break;
    default:
        ret = sub_sub_step_up();
        break;
    }
    return ret;
}
