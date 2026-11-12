/* @PostProc: patch_insn_reg_field 0x10:rt:t6:t9 0x14:rt:t7:t0 0x18:rt:t9:t6 0x1c:rd:t8:t1 0x1c:rs:t6:t9 0x1c:rt:t7:t0 0x20:rt:t8:t1 0x24:rt:t0:t7 0x28:rt:t2:t3 0x2c:rd:t1:t8 0x2c:rs:t9:t6 0x2c:rt:t0:t7 0x30:rt:t2:t3 0x38:rt:t1:t8 0x3c:rt:t3:t2 0x48:rt:t3:t2 */
/* obj_move - TT overlay, Address: 0x80134924, Size: 140 bytes (35 insns) */
#include "types.h"

void obj_move(u8 *p)
{
    s16 y;
    s16 x;

    *(s32 *)(p + 36) = *(s32 *)(p + 44) + *(s32 *)(p + 36);
    *(s32 *)(p + 32) = *(s32 *)(p + 40) + *(s32 *)(p + 32);
    *(s16 *)(p + 22) = *(s16 *)(p + 38);
    y = *(s16 *)(p + 22);
    *(s16 *)(p + 20) = *(s16 *)(p + 34);

    if (!(y < 257)) {
        goto kill;
    }
    if (y < -16) {
        goto kill;
    }
    x = *(s16 *)(p + 20);
    if (x < -136) {
        goto kill;
    }
    if (x < 137) {
        return;
    }
kill:
    *(s16 *)(p + 4) = 0;
    *(s16 *)(p + 10) = 0;
    *(s16 *)(p + 8) = 0;
    *(s16 *)(p + 6) = 0;
    p[15] = 0;
    p[14] = 0;
    p[13] = 0;
    p[0] = 0;
}
