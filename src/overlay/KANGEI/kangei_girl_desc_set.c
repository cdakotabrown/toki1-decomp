/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x50:rt:t9:t0 */
/* kangei_girl_desc_set -- KANGEI overlay, Address: 0x80135644, 17 insns. */
#include "types.h"

extern volatile s32 kangei_cal_desc_tbl_1_2;
extern volatile s16 kangei_cal_desc_tbl_1_3;
extern volatile s16 kangei_cal_desc_aux_src;
extern s32 kangei_cal_desc_tbl[][4];

void kangei_girl_desc_set(void)
{
    kangei_cal_desc_tbl[1][0] = 0x801D4074;
    kangei_cal_desc_tbl[1][1] = 0x801D4078;
    kangei_cal_desc_tbl_1_2 = 0x801D4088;
    kangei_cal_desc_tbl_1_3 = kangei_cal_desc_aux_src;
}
