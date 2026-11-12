/* @PostProc: patch_insn_reg_field 0x1c:rt:t7:at 0x24:rs:t7:at */
/* kangei_serifu_mode_set2 - KANGEI overlay, Address: 0x80136F24, Size: 40 bytes (10 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

void
kangei_serifu_mode_set2(void)
{
    *(u8 *)(0x80140000 - 25868) = 2;
    sub_sub_step_up();
}
