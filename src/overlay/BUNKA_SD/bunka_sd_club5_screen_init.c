/* @PostProc: patch_insn_reg_field 0x18:rt:t6:at 0x20:rs:t6:at */
/* bunka_sd_club5_screen_init - BUNKA_SD overlay, Address: 0x80138C3C, Size: 44 bytes (11 insns) */
#include "types.h"

extern void bunka_sd_screen_init(void);
extern void sub_sub_step_up(void);

void bunka_sd_club5_screen_init(void)
{
    *(s32 *)0x800EF860U = 0;
    bunka_sd_screen_init();
    sub_sub_step_up();
}
