/* @SymbolStart: 0x8013EEA8 */
/* @SymbolSize: 132 */
/* @PostProc: patch_insn_reg_field 0x38:rt:v1:v0 0x3c:rs:v1:v0 */
/* Function: date_basyo2_sd_send */
/* Overlay: DATE */
/* Address: 0x8013EEA8 (cross-prologue head) / 0x8013EEB0 (body) */
/* Size: 132 bytes (33 instructions) */
#include "types.h"

extern void device_cmd_send();
extern void sub_sub_step_up();
extern u8 now_sub_sub_step;
extern u8 date_shop_no;

void date_basyo2_sd_send(void)
{
    u32 a0;
    u8 v0;

    a0 = (u32)now_sub_sub_step;
    if (a0 < 20U) goto L1;
    device_cmd_send(515);
    goto END;
L1:
    v0 = date_shop_no;
    if (v0 != 0) goto L2;
    device_cmd_send(512);
    goto END;
L2:
    if (v0 != 1) goto L3;
    device_cmd_send(513);
    goto END;
L3:
    device_cmd_send(514);
END:
    sub_sub_step_up();
}
