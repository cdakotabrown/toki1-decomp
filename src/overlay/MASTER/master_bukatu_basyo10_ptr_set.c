/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t0:t1 0x50:rt:t9:t0 0x58:rs:t0:t1 0x58:rt:t0:t1 0x60:rt:t0:t1 */
/* master_bukatu_basyo10_ptr_set - MASTER overlay, Address: 0x80139000 */
#include "types.h"

extern u32 master_bukatu_cw_ptr[];
extern volatile u32 master_bukatu_cw_ptr_2;
extern volatile s16 master_bukatu_cw_ptr_3;
extern volatile s16 master_bukatu_basyo10_aux_src;

void master_bukatu_basyo10_ptr_set(void)
{
    master_bukatu_cw_ptr[0] = 0x801A42D4;
    master_bukatu_cw_ptr[1] = 0x801A42E0;
    master_bukatu_cw_ptr_2 = 0x801A4330;
    master_bukatu_cw_ptr_3 = master_bukatu_basyo10_aux_src;
    master_bukatu_cw_ptr[4] = 0x80197000;
}
