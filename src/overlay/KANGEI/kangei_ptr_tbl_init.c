/* kangei_ptr_tbl_init -- KANGEI overlay, Address: 0x80132090, 97 insns. */
#include "types.h"

extern s32 kangei_girl_res_tbl[][8];

void kangei_ptr_tbl_init(void)
{
    kangei_girl_res_tbl[0][0] = 0x801973AC;
    kangei_girl_res_tbl[0][1] = 0x80197910;
    kangei_girl_res_tbl[0][2] = 0x80197C44;
    kangei_girl_res_tbl[0][3] = 0x80197F34;
    kangei_girl_res_tbl[0][4] = 0x80198130;
    kangei_girl_res_tbl[0][5] = 0x801982DC;
    kangei_girl_res_tbl[0][6] = 0x801984F4;
    kangei_girl_res_tbl[0][7] = 0x8019865C;
    kangei_girl_res_tbl[1][0] = 0x8019740C;
    kangei_girl_res_tbl[1][1] = 0x80197930;
    kangei_girl_res_tbl[1][2] = 0x80197C74;
    kangei_girl_res_tbl[1][3] = 0x80197F4C;
    kangei_girl_res_tbl[1][4] = 0x80198140;
    kangei_girl_res_tbl[1][5] = 0x801982F4;
    kangei_girl_res_tbl[1][6] = 0x8019850C;
    kangei_girl_res_tbl[1][7] = 0x80198668;
    kangei_girl_res_tbl[2][0] = 0x801977AC;
    kangei_girl_res_tbl[2][1] = 0x80197A70;
    kangei_girl_res_tbl[2][2] = 0x80197E44;
    kangei_girl_res_tbl[2][3] = 0x80198034;
    kangei_girl_res_tbl[2][4] = 0x801981F0;
    kangei_girl_res_tbl[2][5] = 0x801983DC;
    kangei_girl_res_tbl[2][6] = 0x801985F4;
    kangei_girl_res_tbl[2][7] = 0x801986DC;
}
