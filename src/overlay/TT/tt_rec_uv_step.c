/* tt_rec_uv_step - TT overlay, Address: 0x80146DB8, Size: 72 bytes (18 insns) */
/* @PostProc: patch_insn_reg_field 0x0:rt:v0:t6 0x4:rt:t6:t8 0x8:rs:v0:t6 0x8:rt:v0:t7 0xc:rs:v0:t7 0xc:rt:v1:v0 0x10:rs:v1:v0 0x18:rt:v0:t7 0x20:rt:t6:t8 0x24:rs:v1:v0 0x2c:rt:t8:t0 0x30:rt:t7:t9 0x38:rt:t7:t9 0x3c:rt:t8:t0 */
#include "types.h"

void tt_rec_uv_step(u8 *arg0)
{
    u32 t7;
    u32 v0;

    t7 = *(u16 *)(arg0 + 70) + 1;
    v0 = t7 & 0xFFFF;
    *(u16 *)(arg0 + 70) = t7;
    if (v0 >= 21) {
        arg0[68] = 192;
    } else if (v0 >= 11) {
        arg0[68] = 176;
    } else {
        arg0[68] = 160;
    }
}
