/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x2c:rs:v0:v1 0x44:rs:v0:v1 0x48:rt:v1:v0 0x4c:rs:v1:v0 0x54:rs:v0:v1 0x5c:rs:v1:v0 */
/* tt_ready_check - TT overlay, Address: 0x80148754, Size: 108 bytes (27 insns) */
#include "types.h"

extern u8 *tt_ready_wk;

s32 tt_ready_check(void)
{
    volatile u8 *p;

    p = tt_ready_wk;
    if (p[5] == 0 && p[6] == 0) {
        return 1;
    }
    if (p[5] == 3 && p[6] == 3) {
        return 2;
    }
    return 0;
}
