/* @CompileEnv: IDO52_AS1_NO_LA_PULL=all */
/* @RodataBase: 0x80134310 */
/* @CompileFlags: -cckr */
/* valen_girl_cel_set4 - VALEN overlay, Address: 0x80133F80, Size: 696 bytes (174 insns) */
#include "types.h"

extern unsigned char day_flag[];
extern s32 valen_ss_s;
extern s32 valen_ss_s_kind;
extern s32 valen_sss_so;
extern s16 valen_now_s;
extern s16 valen_now_s_in;
extern s32 valen_cel_res_tbl[][13];
extern void valen_cel_init(void);
extern void sub_sub_step_up(void);

void valen_girl_cel_set4()
{
    valen_cel_init();
    day_flag[0] &= ~8;
    day_flag[0] |= 4;
    switch (day_flag[2113]) {
        case 0:
            valen_ss_s = valen_cel_res_tbl[0][1]; valen_ss_s_kind = valen_cel_res_tbl[1][1]; valen_sss_so = valen_cel_res_tbl[2][1];
            break;
        case 1:
            valen_ss_s = valen_cel_res_tbl[0][6]; valen_ss_s_kind = valen_cel_res_tbl[1][6]; valen_sss_so = valen_cel_res_tbl[2][6];
            break;
        case 2:
            valen_ss_s = valen_cel_res_tbl[0][3]; valen_ss_s_kind = valen_cel_res_tbl[1][3]; valen_sss_so = valen_cel_res_tbl[2][3];
            break;
        case 3:
            valen_ss_s = valen_cel_res_tbl[0][4]; valen_ss_s_kind = valen_cel_res_tbl[1][4]; valen_sss_so = valen_cel_res_tbl[2][4];
            break;
        case 4:
            valen_ss_s = valen_cel_res_tbl[0][10]; valen_ss_s_kind = valen_cel_res_tbl[1][10]; valen_sss_so = valen_cel_res_tbl[2][10];
            break;
        case 5:
            valen_ss_s = valen_cel_res_tbl[0][7]; valen_ss_s_kind = valen_cel_res_tbl[1][7]; valen_sss_so = valen_cel_res_tbl[2][7];
            break;
        case 6:
            valen_ss_s = valen_cel_res_tbl[0][8]; valen_ss_s_kind = valen_cel_res_tbl[1][8]; valen_sss_so = valen_cel_res_tbl[2][8];
            break;
        case 7:
            valen_ss_s = valen_cel_res_tbl[0][5]; valen_ss_s_kind = valen_cel_res_tbl[1][5]; valen_sss_so = valen_cel_res_tbl[2][5];
            break;
        case 8:
            valen_ss_s = valen_cel_res_tbl[0][0]; valen_ss_s_kind = valen_cel_res_tbl[1][0]; valen_sss_so = valen_cel_res_tbl[2][0];
            break;
        case 9:
            valen_ss_s = valen_cel_res_tbl[0][9]; valen_ss_s_kind = valen_cel_res_tbl[1][9]; valen_sss_so = valen_cel_res_tbl[2][9];
            break;
        default:
            valen_ss_s = valen_cel_res_tbl[0][12]; valen_ss_s_kind = valen_cel_res_tbl[1][12]; valen_sss_so = valen_cel_res_tbl[2][12];
            break;
    }
    valen_now_s = 1;
    valen_now_s_in = 0;
    sub_sub_step_up();
}
