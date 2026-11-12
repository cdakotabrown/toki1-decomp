/* @RodataBase: 0x8013985C */
/* kangei_girl_msg_init - KANGEI overlay, Address: 0x801349E4, Size: 496 bytes (124 insns) */
#include "types.h"

extern void kangei_ptr_tbl2_init(void);
extern void kangei_girl0_desc_set(void);

extern u8 now_girl;

extern s32 kangei_ss_s;
extern s32 kangei_ss_s_kind;
extern s32 kangei_sss_so;

extern s32 kangei_girl0_res_tbl[];
extern s32 kangei_msg_res_tbl[][8];

void
kangei_girl_msg_init(void)
{
    u8 idx;

    kangei_ptr_tbl2_init();
    kangei_girl0_desc_set();

    idx = now_girl;
    switch (idx) {
    case 0:
        kangei_ss_s = kangei_girl0_res_tbl[0];
        kangei_ss_s_kind = kangei_girl0_res_tbl[1];
        kangei_sss_so = kangei_girl0_res_tbl[2];
        break;
    case 1:
        kangei_ss_s = kangei_msg_res_tbl[0][2];
        kangei_ss_s_kind = kangei_msg_res_tbl[1][2];
        kangei_sss_so = kangei_msg_res_tbl[2][2];
        break;
    case 2:
        kangei_ss_s = kangei_msg_res_tbl[0][0];
        kangei_ss_s_kind = kangei_msg_res_tbl[1][0];
        kangei_sss_so = kangei_msg_res_tbl[2][0];
        break;
    case 3:
        kangei_ss_s = kangei_msg_res_tbl[0][1];
        kangei_ss_s_kind = kangei_msg_res_tbl[1][1];
        kangei_sss_so = kangei_msg_res_tbl[2][1];
        break;
    case 4:
        kangei_ss_s = kangei_msg_res_tbl[0][5];
        kangei_ss_s_kind = kangei_msg_res_tbl[1][5];
        kangei_sss_so = kangei_msg_res_tbl[2][5];
        break;
    case 5:
        kangei_ss_s = kangei_msg_res_tbl[0][3];
        kangei_ss_s_kind = kangei_msg_res_tbl[1][3];
        kangei_sss_so = kangei_msg_res_tbl[2][3];
        break;
    case 6:
        kangei_ss_s = kangei_msg_res_tbl[0][4];
        kangei_ss_s_kind = kangei_msg_res_tbl[1][4];
        kangei_sss_so = kangei_msg_res_tbl[2][4];
        break;
    default:
        kangei_ss_s = kangei_msg_res_tbl[0][7];
        kangei_ss_s_kind = kangei_msg_res_tbl[1][7];
        kangei_sss_so = kangei_msg_res_tbl[2][7];
        break;
    }
}
