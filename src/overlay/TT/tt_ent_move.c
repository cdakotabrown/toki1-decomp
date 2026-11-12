/* @PostProc: patch_insn_reg_field 0x10:rt:t6:t9 0x14:rt:t7:t0 0x18:rt:t9:t6 0x1c:rd:t8:t1 0x1c:rs:t6:t9 0x1c:rt:t7:t0 0x20:rt:t8:t1 0x24:rt:t0:t7 0x28:rt:t2:t3 0x2c:rd:t1:t8 0x2c:rs:t9:t6 0x2c:rt:t0:t7 0x30:rt:t2:t3 0x34:rt:v0:t4 0x38:rt:t1:t8 0x3c:rt:t3:t2 0x40:rs:v0:t4 0x48:rt:t3:t2 0x78:rt:t4:t5 0x90:rt:t4:t5 0x9c:rt:t5:t6 0xa4:rt:t5:t6 0xa8:rt:t6:t7 0xb0:rd:t7:t8 0xb0:rt:t6:t7 0xb4:rd:t7:t8 0xb4:rs:t7:t8 0xb4:rt:t6:t7 0xb8:rd:t7:t8 0xb8:rt:t7:t8 0xbc:rd:t8:t9 0xbc:rs:t7:v0 0xbc:rt:v0:t8 0xc0:rt:t8:t9 */
/* tt_ent_move - TT overlay, Address: 0x801474FC, Size: 188 bytes (47 insns) */
#include "types.h"

void tt_ent_move(u8 *p)
{
    s16 y;

    *(s32 *)(p + 36) = *(s32 *)(p + 44) + *(s32 *)(p + 36);
    *(s32 *)(p + 32) = *(s32 *)(p + 40) + *(s32 *)(p + 32);
    *(s16 *)(p + 22) = *(s16 *)(p + 38);
    y = *(s16 *)(p + 22);
    *(s16 *)(p + 20) = *(s16 *)(p + 34);

    if (y >= 257) {
        p[0] = 0;
        *(s16 *)(p + 4) = 0;
        p[12] = 0;
        *(s16 *)(p + 10) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[15] = 0;
        p[14] = 0;
        p[13] = 0;
        return;
    }

    if (*(s16 *)(p + 82) < 2561) {
        *(s8 *)(p + 84) = 1;
    } else if (*(s16 *)(p + 82) >= 4096) {
        *(s8 *)(p + 84) = -1;
    }
    *(s16 *)(p + 82) = *(s16 *)(p + 82) + *(s8 *)(p + 84) * 48;
}
