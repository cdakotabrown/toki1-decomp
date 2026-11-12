/* @PostProc: patch_insn_reg_field 0x1c:rt:t7:at 0x24:rs:t7:at */
/* kangei_serifu_set34 - KANGEI overlay, Address: 0x80138864, Size: 40 bytes (10 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

void
kangei_serifu_set34(void)
{
    *(s16 *)(0x80140000 - 25876) = 34;
    sub_sub_step_up();
}
