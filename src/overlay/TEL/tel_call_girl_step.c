/* @CompileEnv: IDO52_COPYPROP_VAR=-16 IDO52_FORCE_WEB_COLOR=24:v1 */
/* @RodataBase: 0x801401BC */
/* tel_call_girl_step - TEL overlay, Address: 0x8013BF0C, Size: 1020 bytes (255 insns) */
#include "types.h"

extern s32  get_tel_call_girl(void);
extern s32  get_g_zyotai_s(s32);
extern void k_sub_reset(void);
extern void gnsx(s32);
extern void sndisp(s32, s32, s32);
extern void xa_set_m(s32, s32, s32, s32);
extern void k_sub_disp_start(s32);
extern void xa_wait(void);
extern void xa_wait_m(void);
extern void sub_sub_step_up(void);

extern u8  step_mode;
extern u8  p_sei[];
extern s32 date_name_set_tbl0[];
extern u8  screen_w[];

extern s32 tel_sisyuu1_serifu_tbl[];
extern u8  tel_sisyuu1_xa_tbl[];
extern u8  tel_sisyuu1_xa_tbl2[];
extern u8  tel_call_girl_step_str0[];
extern s32 tel_sisyuu2_serifu_tbl[];
extern u8  tel_sisyuu2_xa_tbl[];
extern u8  tel_sisyuu2_xa_tbl2[];
extern u8  tel_call_girl_step_str1[];
extern s32 tel_sisyuu3_serifu_tbl[];
extern u8  tel_sisyuu3_xa_tbl[];
extern u8  tel_sisyuu3_xa_tbl2[];
extern u8  tel_call_girl_step_str2[];

void tel_call_girl_step(void)
{
    s32 pad0;
    u32 id;
    s32 idx;

    idx = get_tel_call_girl();
    id = step_mode;

    switch (id) {
    case 0:
        xa_wait();
        return;

    case 1:
        if (get_g_zyotai_s(idx) & 0x80) {
            step_mode = 10;
            return;
        }
        if (get_g_zyotai_s(idx) == 3) {
            step_mode = 50;
            return;
        }
        step_mode = 30;
        return;

    case 10:
        k_sub_reset();
        gnsx(date_name_set_tbl0[idx]);
        sndisp(tel_sisyuu1_serifu_tbl[idx], 0, idx);
        xa_set_m((s32)tel_sisyuu1_xa_tbl, (s32)tel_sisyuu1_xa_tbl2, idx, idx);
        k_sub_disp_start(0);
        step_mode += 1;
        return;

    case 11:
        xa_wait_m();
        return;

    case 12:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp((s32)tel_call_girl_step_str0, 0, 31);
        k_sub_disp_start(0);
        step_mode += 1;
        return;

    case 13:
        xa_wait();
        return;

    case 14:
        step_mode = 240;
        return;

    case 30:
        k_sub_reset();
        gnsx(date_name_set_tbl0[idx]);
        sndisp(tel_sisyuu2_serifu_tbl[idx], 0, idx);
        xa_set_m((s32)tel_sisyuu2_xa_tbl, (s32)tel_sisyuu2_xa_tbl2, idx, idx);
        k_sub_disp_start(0);
        step_mode += 1;
        return;

    case 31:
        xa_wait_m();
        return;

    case 32:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp((s32)tel_call_girl_step_str1, 0, 31);
        k_sub_disp_start(0);
        step_mode += 1;
        return;

    case 33:
        xa_wait();
        return;

    case 34:
        step_mode = 240;
        return;

    case 50:
        k_sub_reset();
        gnsx(date_name_set_tbl0[idx]);
        sndisp(tel_sisyuu3_serifu_tbl[idx], 0, idx);
        xa_set_m((s32)tel_sisyuu3_xa_tbl, (s32)tel_sisyuu3_xa_tbl2, idx, idx);
        k_sub_disp_start(0);
        step_mode += 1;
        return;

    case 51:
        xa_wait_m();
        return;

    case 52:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp((s32)tel_call_girl_step_str2, 0, 31);
        k_sub_disp_start(0);
        step_mode += 1;
        return;

    case 53:
        xa_wait();
        return;

    case 54:
        step_mode = 240;
        return;

    case 240:
        step_mode = step_mode + 1;
        return;

    default:
        screen_w[idx * 56 + 457] &= ~0x10;
        sub_sub_step_up();
        return;
    }
}
