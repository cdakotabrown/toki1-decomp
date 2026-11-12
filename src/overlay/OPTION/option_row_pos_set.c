/* Address: 0x801325A4, Size: 132 bytes (33 instructions) */
/* @PostProc: scheduler_normalizer --const-fp --store-fp --block-aware --fence 10-10,26-26 */
/* @PostProc: patch_insn_reg_field 0x10:rd:v0:v1 0x14:rt:t0:a0 0x18:rt:v1:a1 0x1c:rt:a1:a2 0x20:rt:a2:a3 0x24:rt:a3:t0 0x30:rt:a0:v0 0x34:rs:v0:v1 0x34:rt:a3:t0 0x38:rs:a3:t0 0x38:rt:a3:t0 0x3c:rs:v0:v1 0x3c:rt:a1:a2 0x40:rs:v0:v1 0x40:rt:a2:a3 0x44:rs:v0:v1 0x44:rt:v1:a1 0x4c:rs:v1:a3 0x4c:rt:v1:a3 0x54:rs:v0:v1 0x54:rt:a0:v0 0x58:rs:v0:v1 0x58:rt:t0:a0 0x5c:rs:v0:v1 0x60:rs:v0:v1 0x60:rt:a0:v0 0x64:rs:v0:v1 0x64:rt:t0:a0 0x68:rs:v0:v1 0x6c:rs:v0:v1 0x6c:rt:a0:v0 0x70:rs:v0:v1 0x70:rt:t0:a0 0x74:rs:v0:v1 0x78:rs:v0:v1 0x78:rt:v0:v1 0x7c:rs:v0:v1 0x7c:rt:a0:v0 0x80:rs:v0:v1 0x80:rt:t0:a0 0x84:rs:a3:t0 0x88:rs:v0:v1 */
#include "types.h"

void option_row_pos_set(s16 *base)
{
    s16 *p;
    s32 c0;
    s32 c1;
    s32 c2;
    s32 c3;

    p = base;
    c0 = -88;
    c1 = -68;
    c2 = -48;
    c3 = -28;
    do {
        p[11] = (s16)c3;
        c3 += 80;
        p[9] = (s16)c1;
        p[10] = (s16)c2;
        p[8] = (s16)c0;
        c0 += 80;
        c2 += 80;
        c1 += 80;
        p[1] = -120;
        p[17] = 96;
        p[25] = 20;
        p[2] = -120;
        p[18] = 96;
        p[26] = 20;
        p[3] = -120;
        p[19] = 96;
        p[27] = 20;
        p += 4;
        p[-4] = -120;
        p[12] = 96;
        p[20] = 20;
    } while (c3 != 132);
}
