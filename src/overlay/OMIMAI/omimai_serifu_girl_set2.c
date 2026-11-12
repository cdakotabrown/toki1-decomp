/* @RodataBase: 0x801349C4 */
/* omimai_serifu_girl_set2 - OMIMAI overlay, Address: 0x801340E4, Size: 672 bytes. */
#include "types.h"

extern u8 now_girl;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s32 omimai_serifu3_res_tbl[][13];

void omimai_serifu_girl_set2(void)
{
    switch ((u32)now_girl) {
    case 0:
        ss_serifu = omimai_serifu3_res_tbl[0][1];
        ss_serifu_kind = omimai_serifu3_res_tbl[1][1];
        sss_sound = omimai_serifu3_res_tbl[2][1];
        break;
    case 1:
        ss_serifu = omimai_serifu3_res_tbl[0][6];
        ss_serifu_kind = omimai_serifu3_res_tbl[1][6];
        sss_sound = omimai_serifu3_res_tbl[2][6];
        break;
    case 2:
        ss_serifu = omimai_serifu3_res_tbl[0][2];
        ss_serifu_kind = omimai_serifu3_res_tbl[1][2];
        sss_sound = omimai_serifu3_res_tbl[2][2];
        break;
    case 3:
        ss_serifu = omimai_serifu3_res_tbl[0][4];
        ss_serifu_kind = omimai_serifu3_res_tbl[1][4];
        sss_sound = omimai_serifu3_res_tbl[2][4];
        break;
    case 4:
        ss_serifu = omimai_serifu3_res_tbl[0][10];
        ss_serifu_kind = omimai_serifu3_res_tbl[1][10];
        sss_sound = omimai_serifu3_res_tbl[2][10];
        break;
    case 5:
        ss_serifu = omimai_serifu3_res_tbl[0][7];
        ss_serifu_kind = omimai_serifu3_res_tbl[1][7];
        sss_sound = omimai_serifu3_res_tbl[2][7];
        break;
    case 6:
        ss_serifu = omimai_serifu3_res_tbl[0][8];
        ss_serifu_kind = omimai_serifu3_res_tbl[1][8];
        sss_sound = omimai_serifu3_res_tbl[2][8];
        break;
    case 7:
        ss_serifu = omimai_serifu3_res_tbl[0][5];
        ss_serifu_kind = omimai_serifu3_res_tbl[1][5];
        sss_sound = omimai_serifu3_res_tbl[2][5];
        break;
    case 8:
        ss_serifu = omimai_serifu3_res_tbl[0][0];
        ss_serifu_kind = omimai_serifu3_res_tbl[1][0];
        sss_sound = omimai_serifu3_res_tbl[2][0];
        break;
    case 9:
        ss_serifu = omimai_serifu3_res_tbl[0][9];
        ss_serifu_kind = omimai_serifu3_res_tbl[1][9];
        sss_sound = omimai_serifu3_res_tbl[2][9];
        break;
    case 10:
        ss_serifu = omimai_serifu3_res_tbl[0][12];
        ss_serifu_kind = omimai_serifu3_res_tbl[1][12];
        sss_sound = omimai_serifu3_res_tbl[2][12];
        break;
    default:
        ss_serifu = omimai_serifu3_res_tbl[0][3];
        ss_serifu_kind = omimai_serifu3_res_tbl[1][3];
        sss_sound = omimai_serifu3_res_tbl[2][3];
        break;
    }
}
