/* @PostProc: patch_insn_reg_field 0x1c:rt:t7:at 0x24:rs:t7:at */
/* kangei_serifu_mode_set1 - KANGEI overlay, Address: 0x80136F4C, Size: 40 bytes (10 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

void
kangei_serifu_mode_set1(void)
{
    *(u8 *)(0x80140000 - 25868) = 1;
    sub_sub_step_up();
}
