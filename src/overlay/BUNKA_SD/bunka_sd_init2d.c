/* @PostProc: patch_insn_reg_field 0x30:rt:t7:at 0x38:rs:t7:at */
/* bunka_sd_init2d - BUNKA_SD overlay, Address: 0x801364B4, Size: 60 bytes (15 insns) */
#include "types.h"

extern void device_port_init(s32, s32);
extern void bunka_sd_panel_init(void);
extern void sub_sub_step_up(void);

void bunka_sd_init2d(void)
{
    device_port_init(1, 513);
    bunka_sd_panel_init();
    *(u8 *)0x80128B76U = 1;
    sub_sub_step_up();
}
