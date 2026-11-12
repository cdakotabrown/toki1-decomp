/* @PostProc: patch_insn_reg_field 0x74:rt:t2:t3 0x7c:rt:t3:t4 0x80:rt:t2:t3 0x84:rs:t3:t4 0x84:rt:t3:t5 0x8c:rt:t4:t6 0x90:rt:t3:t5 0x98:rs:t4:t6 0x98:rt:t4:t6 0xa0:rt:t4:t6 */
/* kangei_work_desc_set -- KANGEI overlay, Address: 0x80135F84, 37 insns. */
#include "types.h"

extern s32 kangei_work_desc_tbl[][2];
extern volatile s32 kangei_work_desc_tbl_2_1;
extern volatile s16 kangei_work_desc_tbl_3_0;
extern volatile s16 kangei_work_desc_tbl_3_1;
extern s32 kangei_work_pal_ptr;
extern volatile s16 kangei_work_desc_aux0_src;
extern volatile s16 kangei_work_desc_aux1_src;

void kangei_work_desc_set(void)
{
    kangei_work_desc_tbl[0][0] = 0x801EAFC4;
    kangei_work_desc_tbl[0][1] = 0x801EB144;
    kangei_work_desc_tbl[1][0] = 0x801EAFF0;
    kangei_work_desc_tbl[1][1] = 0x801EB14C;
    kangei_work_desc_tbl[2][0] = 0x801EB030;
    kangei_work_desc_tbl_2_1 = 0x801EB174;
    kangei_work_desc_tbl_3_0 = kangei_work_desc_aux0_src;
    kangei_work_desc_tbl_3_1 = kangei_work_desc_aux1_src;
    kangei_work_pal_ptr = 0x801B4400;
}
