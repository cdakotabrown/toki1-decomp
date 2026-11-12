/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v0 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x18:rd:v1:v0 0x20:rs:v1:v0 0x20:rt:v1:v0 0x24:rs:v0:v1 0x24:rt:v0:v1 0x28:rs:v1:v0 0x2c:rs:v0:v1 */
/* trail_clear_all - TT overlay, Address: 0x80147CD0, Size: 40 bytes (10 insns) */
#include "types.h"

extern u8 *tt_trail_buf;

void trail_clear_all(void)
{
    u8 *p;
    s32 i;

    p = tt_trail_buf;
    i = 0;
    do {
        i++;
        p += 164;
        p[-164] = 0;
    } while (i != 3);
}
