/* @RodataBase: 0x80139678 */
/* kangei_girl_ptr_set -- KANGEI overlay, Address: 0x8013260C, 116 insns. */
#include "types.h"

extern u8 now_girl;

extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;

extern s32 kangei_girl_res_tbl[][8];

void kangei_girl_ptr_set(void)
{
    switch (now_girl) {
    case 0:
        ss_serifu = kangei_girl_res_tbl[0][0];
        ss_serifu_kind = kangei_girl_res_tbl[1][0];
        sss_sound = kangei_girl_res_tbl[2][0];
        break;
    case 1:
        ss_serifu = kangei_girl_res_tbl[0][3];
        ss_serifu_kind = kangei_girl_res_tbl[1][3];
        sss_sound = kangei_girl_res_tbl[2][3];
        break;
    case 2:
        ss_serifu = kangei_girl_res_tbl[0][1];
        ss_serifu_kind = kangei_girl_res_tbl[1][1];
        sss_sound = kangei_girl_res_tbl[2][1];
        break;
    case 3:
        ss_serifu = kangei_girl_res_tbl[0][2];
        ss_serifu_kind = kangei_girl_res_tbl[1][2];
        sss_sound = kangei_girl_res_tbl[2][2];
        break;
    case 4:
        ss_serifu = kangei_girl_res_tbl[0][6];
        ss_serifu_kind = kangei_girl_res_tbl[1][6];
        sss_sound = kangei_girl_res_tbl[2][6];
        break;
    case 5:
        ss_serifu = kangei_girl_res_tbl[0][4];
        ss_serifu_kind = kangei_girl_res_tbl[1][4];
        sss_sound = kangei_girl_res_tbl[2][4];
        break;
    case 6:
        ss_serifu = kangei_girl_res_tbl[0][5];
        ss_serifu_kind = kangei_girl_res_tbl[1][5];
        sss_sound = kangei_girl_res_tbl[2][5];
        break;
    default:
        ss_serifu = kangei_girl_res_tbl[0][7];
        ss_serifu_kind = kangei_girl_res_tbl[1][7];
        sss_sound = kangei_girl_res_tbl[2][7];
        break;
    }
}
