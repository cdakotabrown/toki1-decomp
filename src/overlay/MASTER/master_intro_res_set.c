/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t0:t1 0x50:rt:t9:t0 0x58:rs:t0:t1 0x58:rt:t0:t1 0x60:rt:t0:t1 */
/* master_intro_res_set - MASTER overlay, Address: 0x80138f10 */
#include "types.h"

extern s32 master_intro_res_tbl[];
extern volatile u32 master_intro_res_tbl_2;
extern volatile s16 master_intro_res_tbl_3;
extern volatile s16 master_intro_res_aux_src;
extern u32 master_intro_pal_ptr;

void master_intro_res_set(void)
{
    (*(u32 *)&master_intro_res_tbl[0]) = 0x801A403C;
    (*(u32 *)&master_intro_res_tbl[1]) = 0x801A4048;
    master_intro_res_tbl_2 = 0x801A4098;
    master_intro_res_tbl_3 = master_intro_res_aux_src;
    master_intro_pal_ptr = 0x80197000;
}
