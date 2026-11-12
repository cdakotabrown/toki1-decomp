/* @PostProc: patch_insn_reg_field 0x18:rt:t6:at 0x20:rs:t6:at */
/* kangei_serifu_mode_set0 - KANGEI overlay, Address: 0x80136F74, Size: 36 bytes (9 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

void
kangei_serifu_mode_set0(void)
{
    *(u8 *)(0x80140000 - 25868) = 0;
    sub_sub_step_up();
}
