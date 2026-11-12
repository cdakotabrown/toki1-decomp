/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x84 */
/* @PostProc: patch_insn_reg_field 0x74:rt:t2:t3 0x7c:rt:t3:t4 0x80:rt:t2:t3 0x84:rs:t3:t4 0x84:rt:t3:t5 0x8c:rt:t4:t6 0x90:rt:t3:t5 0x98:rs:t4:t6 0x98:rt:t4:t6 0xa0:rt:t4:t6 */
/* @SymbolSize: 148 */
/* Address: 0x80133724, Size: 148 bytes (37 insns) */
#include "types.h"

extern s32 master_result_desc_tbl[][2];
extern s32 master_result_pal_ptr;

extern s16 master_result_desc_aux0_src;
extern s16 master_result_desc_aux1_src;

void master_result_res_set(void)
{
    master_result_desc_tbl[0][0] = 0x801EAFC4;
    master_result_desc_tbl[0][1] = 0x801EB144;
    master_result_desc_tbl[1][0] = 0x801EAFF0;
    master_result_desc_tbl[1][1] = 0x801EB14C;
    master_result_desc_tbl[2][0] = 0x801EB030;
    master_result_desc_tbl[2][1] = 0x801EB174;
    (*(s16 *)&master_result_desc_tbl[3][0]) = master_result_desc_aux0_src;
    (*(s16 *)&master_result_desc_tbl[3][1]) = master_result_desc_aux1_src;
    master_result_pal_ptr = 0x801B4400;
}
