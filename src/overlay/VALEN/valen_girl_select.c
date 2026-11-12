/* @CompileEnv: IDO52_FORCE_WEB_COLOR=10:a1,148:v1 IDO52_AS1_XBB_SKIP_CAND=occ:0 */
/* @RodataBase: 0x801342CC */
/* @SymbolStart: 0x80132D0C */
/* @SymbolSize: 872 */
/* @CompileFlags: -cckr -Wo,-noprecolor */
/* valen_girl_select - VALEN overlay, Address: 0x80132D1C (window from the absorbed */
#include "types.h"

extern u8 now_girl;
extern u8 valen_girl9_flag;
extern s16 valen_now_s;
extern s32 valen_ss_s;
extern s32 valen_ss_s_kind;
extern s32 valen_sss_so;
extern s16 girl0_tokimeki;
extern u8 valen_girl_list[];
extern s16 valen_list_no;
extern s16 valen_girl_list_cnt;

extern s32 valen_msg_res_tbl[][13];
extern unsigned get_h_tokimeki();

valen_girl_select()
{
    unsigned charId;

    if (valen_list_no == valen_girl_list_cnt) {
        sub_step_up();
        return 0;
    }
    now_girl = valen_girl_list[valen_list_no * 2];
    date_name_set(now_girl);
    valen_now_s = get_h_tokimeki(now_girl) >= 70;
    if (now_girl == 9 && valen_girl9_flag == 1) {
        valen_now_s = 3;
    }
    if (now_girl == 0 && valen_girl9_flag == 1 && girl0_tokimeki >= 70) {
        valen_now_s = 3;
    }
    if (now_girl == 0 && valen_girl9_flag == 1 && girl0_tokimeki < 70) {
        valen_now_s = 5;
    }
    now_girl = now_girl;
    switch (now_girl) {
    case 0:
        valen_ss_s = valen_msg_res_tbl[0][2];
        valen_ss_s_kind = valen_msg_res_tbl[1][2];
        valen_sss_so = valen_msg_res_tbl[2][2];
        break;
    case 1:
        valen_ss_s = valen_msg_res_tbl[0][7];
        valen_ss_s_kind = valen_msg_res_tbl[1][7];
        valen_sss_so = valen_msg_res_tbl[2][7];
        break;
    case 2:
        valen_ss_s = valen_msg_res_tbl[0][3];
        valen_ss_s_kind = valen_msg_res_tbl[1][3];
        valen_sss_so = valen_msg_res_tbl[2][3];
        break;
    case 3:
        valen_ss_s = valen_msg_res_tbl[0][5];
        valen_ss_s_kind = valen_msg_res_tbl[1][5];
        valen_sss_so = valen_msg_res_tbl[2][5];
        break;
    case 4:
        valen_ss_s = valen_msg_res_tbl[0][11];
        valen_ss_s_kind = valen_msg_res_tbl[1][11];
        valen_sss_so = valen_msg_res_tbl[2][11];
        break;
    case 5:
        valen_ss_s = valen_msg_res_tbl[0][8];
        valen_ss_s_kind = valen_msg_res_tbl[1][8];
        valen_sss_so = valen_msg_res_tbl[2][8];
        break;
    case 6:
        valen_ss_s = valen_msg_res_tbl[0][9];
        valen_ss_s_kind = valen_msg_res_tbl[1][9];
        valen_sss_so = valen_msg_res_tbl[2][9];
        break;
    case 7:
        valen_ss_s = valen_msg_res_tbl[0][6];
        valen_ss_s_kind = valen_msg_res_tbl[1][6];
        valen_sss_so = valen_msg_res_tbl[2][6];
        break;
    case 8:
        valen_ss_s = valen_msg_res_tbl[0][0];
        valen_ss_s_kind = valen_msg_res_tbl[1][0];
        valen_sss_so = valen_msg_res_tbl[2][0];
        break;
    case 9:
        valen_ss_s = valen_msg_res_tbl[0][10];
        valen_ss_s_kind = valen_msg_res_tbl[1][10];
        valen_sss_so = valen_msg_res_tbl[2][10];
        break;
    case 10:
        valen_ss_s = valen_msg_res_tbl[0][12];
        valen_ss_s_kind = valen_msg_res_tbl[1][12];
        valen_sss_so = valen_msg_res_tbl[2][12];
        break;
    }
    sub_sub_step_up();
}
