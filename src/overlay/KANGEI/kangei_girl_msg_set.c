/* @RodataBase: 0x80139938 */
/* kangei_girl_msg_set - KANGEI overlay, Address: 0x801392DC, 620 bytes. */
#include "types.h"

extern u8 now_girl;
extern s32 kangei_bank1_tbl[][13];
extern s32 kangei_ss_s;
extern s32 kangei_ss_s_kind;
extern s32 kangei_sss_so;

void kangei_girl_msg_set(void)
{
    switch (now_girl) {
    case 0:
        kangei_ss_s = kangei_bank1_tbl[0][1];
        kangei_ss_s_kind = kangei_bank1_tbl[1][1];
        kangei_sss_so = kangei_bank1_tbl[2][1];
        break;
    case 1:
        kangei_ss_s = kangei_bank1_tbl[0][6];
        kangei_ss_s_kind = kangei_bank1_tbl[1][6];
        kangei_sss_so = kangei_bank1_tbl[2][6];
        break;
    case 2:
        kangei_ss_s = kangei_bank1_tbl[0][3];
        kangei_ss_s_kind = kangei_bank1_tbl[1][3];
        kangei_sss_so = kangei_bank1_tbl[2][3];
        break;
    case 3:
        kangei_ss_s = kangei_bank1_tbl[0][4];
        kangei_ss_s_kind = kangei_bank1_tbl[1][4];
        kangei_sss_so = kangei_bank1_tbl[2][4];
        break;
    case 4:
        kangei_ss_s = kangei_bank1_tbl[0][10];
        kangei_ss_s_kind = kangei_bank1_tbl[1][10];
        kangei_sss_so = kangei_bank1_tbl[2][10];
        break;
    case 5:
        kangei_ss_s = kangei_bank1_tbl[0][7];
        kangei_ss_s_kind = kangei_bank1_tbl[1][7];
        kangei_sss_so = kangei_bank1_tbl[2][7];
        break;
    case 6:
        kangei_ss_s = kangei_bank1_tbl[0][8];
        kangei_ss_s_kind = kangei_bank1_tbl[1][8];
        kangei_sss_so = kangei_bank1_tbl[2][8];
        break;
    case 7:
        kangei_ss_s = kangei_bank1_tbl[0][5];
        kangei_ss_s_kind = kangei_bank1_tbl[1][5];
        kangei_sss_so = kangei_bank1_tbl[2][5];
        break;
    case 8:
        kangei_ss_s = kangei_bank1_tbl[0][0];
        kangei_ss_s_kind = kangei_bank1_tbl[1][0];
        kangei_sss_so = kangei_bank1_tbl[2][0];
        break;
    case 9:
        kangei_ss_s = kangei_bank1_tbl[0][9];
        kangei_ss_s_kind = kangei_bank1_tbl[1][9];
        kangei_sss_so = kangei_bank1_tbl[2][9];
        break;
    default:
        kangei_ss_s = kangei_bank1_tbl[0][12];
        kangei_ss_s_kind = kangei_bank1_tbl[1][12];
        kangei_sss_so = kangei_bank1_tbl[2][12];
        break;
    }
}
