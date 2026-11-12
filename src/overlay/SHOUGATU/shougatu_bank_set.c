/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0xa0:rt:t5:t6 0xa4:rt:t5:t6 0xa8:rt:t6:t7 0xac:rs:t6:t7 0xac:rt:t6:t7 0xb4:rt:t6:t7 0xb8:rt:t7:t8 0xbc:rs:t7:t8 0xbc:rt:t7:t8 0xc4:rt:t7:t8 0xcc:rt:t8:t9 0xd0:rt:t8:t9 0xd4:rt:t9:t0 0xd8:rs:t9:t0 0xd8:rt:t9:t0 0xe0:rt:t9:t0 0xe4:rt:t0:t1 0xe8:rs:t0:t1 0xe8:rt:t0:t1 0xf0:rt:t0:t1 0xf4:rt:t1:t2 0xf8:rs:t1:t2 0xf8:rt:t1:t2 0x100:rt:t1:t2 0x108:rt:t2:t3 0x10c:rt:t2:t3 0x110:rt:t3:t4 0x114:rs:t3:t4 0x114:rt:t3:t4 0x120:rt:t3:t4 */
/* Address: 0x801390D0 */
/* Size: 276 bytes (69 instructions) */
#include "types.h"

extern s32 shougatu_date_res[];
extern volatile s32 shougatu_date_res_2;
extern volatile s16 shougatu_date_res_3;
extern volatile s16 shougatu_date_res_aux_src;

void shougatu_bank_set(void)
{
    shougatu_date_res[0] = 0x801E8088;
    shougatu_date_res[1] = 0x801E808C;
    shougatu_date_res_2 = 0x801E80A0;
    shougatu_date_res_3 = shougatu_date_res_aux_src;
    shougatu_date_res[4] = 0x801B0000;
    shougatu_date_res[5] = 0x801B2000;
    shougatu_date_res[6] = 0x801B4000;
    shougatu_date_res[7] = 0x801B8000;
    shougatu_date_res[8] = 0x801BC000;
    shougatu_date_res[9] = 0x801C0000;
    shougatu_date_res[10] = 0x801C4000;
    shougatu_date_res[11] = 0x801C8000;
    shougatu_date_res[12] = 0x801D0000;
    shougatu_date_res[13] = 0x801D4000;
    shougatu_date_res[14] = 0x801D8000;
    shougatu_date_res[15] = 0x801DC000;
    shougatu_date_res[16] = 0x801E0000;
    shougatu_date_res[17] = 0x801E4000;
}
