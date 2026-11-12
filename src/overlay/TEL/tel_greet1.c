/* @CompileFlags: -cckr */
/* @SymbolStart: 0x80134F10 */
/* @RodataBase: 0x8013F4F0 */
/* @SymbolSize: 780 */
#include "types.h"

extern u8 tel_serifu_girl;
extern u8 step_mode;

extern u8 screen_w[];
extern u8 p_sei[];
extern s32 date_name_set_tbl0[];
extern s32 tel_nanori_serifu_tbl[];
extern u8 tel_nanori_xa_tbl[];
extern u8 tel_nanori_xa_tbl2[];
extern u8 tel_greet1_str0[];

extern void goto_sub_sub_step(s32);
extern void k_sub_reset(void);
extern void k_sub_disp_start(s32);
extern void menu_bar_show(s32);
extern s32 get_g_zyotai_s(s32);
extern void gnsx(s32);
extern void sndisp(u8 *, s32, s32);
extern void sndi(s32, s32, s32);
extern void set_c_girl(s32);
extern void xa_wait(void);
extern void xa_set_m(u8 *, u8 *, s32, s32);
extern void xa_wait_m(void);
extern void cal_base_show(void);

void tel_greet1(void)
{
    u32 cv;
    s32 n;
    s32 v;
    s32 v1;
    u8 masked;

    switch (step_mode) {
    case 0:
        k_sub_reset();
        set_c_girl(tel_serifu_girl);
        screen_w[tel_serifu_girl * 56 + 459] += 1;
        gnsx(date_name_set_tbl0[tel_serifu_girl]);
        sndi(tel_nanori_serifu_tbl[tel_serifu_girl], 0, tel_serifu_girl);
        k_sub_disp_start(0);
        cv = tel_serifu_girl;
        step_mode += 1;
        xa_set_m(tel_nanori_xa_tbl, tel_nanori_xa_tbl2, cv, cv);
        break;
    case 1:
        xa_wait_m();
        break;
    case 2:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp(tel_greet1_str0, 0, 15);
        k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        n = get_g_zyotai_s(tel_serifu_girl);
        if (n == 4) {
            *(u16 *)&screen_w[tel_serifu_girl * 56 + 450] = 16;
            goto_sub_sub_step(3);
            if (tel_serifu_girl == 7) {
                step_mode = 0;
            } else {
                step_mode = 4;
            }
        } else {
            v = get_g_zyotai_s(tel_serifu_girl);
            v1 = v;
            masked = (u8)(v1 & 0x7F);
            v1 = masked;
            if ((u32)v1 < 2U) {
                cv = tel_serifu_girl;
                if (cv == 0) {
                    goto_sub_sub_step(8);
                } else if (cv == 7 && (screen_w[cv * 56 + 459] & 1)) {
                    goto_sub_sub_step(9);
                } else if (cv == 9) {
                    goto_sub_sub_step(10);
                } else {
                    goto_sub_sub_step(4);
                }
            } else if (v1 == 2) {
                cv = tel_serifu_girl;
                if (cv == 10 && (screen_w[cv * 56 + 459] & 1)) {
                    goto_sub_sub_step(7);
                }
                goto_sub_sub_step(5);
            } else {
                goto_sub_sub_step(6);
            }
        }
        break;
    default:
        goto_sub_sub_step(16);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}
