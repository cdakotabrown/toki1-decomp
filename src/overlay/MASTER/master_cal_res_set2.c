/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x50:rt:t9:t0 */
/* master_cal_res_set2 - MASTER overlay, Address: 0x80138f70 */
#include "types.h"

extern u32 master_cal2_res_tbl[];
extern volatile u32 master_cal2_res_tbl_2;
extern volatile s16 master_cal2_res_tbl_3;
extern volatile s16 master_cal_res_aux_src;

void master_cal_res_set2(void)
{
    master_cal2_res_tbl[0] = 0x8019DCFC;
    master_cal2_res_tbl[1] = 0x8019DD00;
    master_cal2_res_tbl_2 = 0x8019DDC0;
    master_cal2_res_tbl_3 = master_cal_res_aux_src;
}
