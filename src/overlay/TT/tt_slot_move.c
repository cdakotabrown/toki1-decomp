/* @SymbolSize: 212 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:t6 0x14:rt:v1:t7 0x18:rt:a1:t9 0x1c:rt:a2:t0 0x20:rd:t6:t8 0x20:rs:v0:t6 0x20:rt:v1:t7 0x24:rd:t7:t1 0x24:rs:a1:t9 0x24:rt:a2:t0 0x28:rt:t6:t8 0x2c:rt:t7:t1 0x30:rt:t8:t2 0x34:rt:t9:t3 0x38:rt:t0:t4 0x3c:rt:t8:t2 0x40:rs:t0:t4 0x44:rt:t9:t3 0x4c:rt:t1:t5 0x80:rt:t1:t5 0xd4:rt:t2:t6 0xd8:rt:t2:t6 */
#include "types.h"

void tt_slot_move(u8 *p)
{
    s32 vx;
    s32 dx;
    s32 vy;
    s32 dy;
    s16 x;
    s16 y;

    vx = *(s32 *)(p + 32);
    dx = *(s32 *)(p + 40);
    vy = *(s32 *)(p + 36);
    dy = *(s32 *)(p + 44);
    *(s32 *)(p + 32) = vx + dx;
    *(s32 *)(p + 36) = vy + dy;
    *(s16 *)(p + 20) = *(s16 *)(p + 34);
    *(s16 *)(p + 22) = *(s16 *)(p + 38);

    if (p[81] != 0) {
        x = *(s16 *)(p + 20);
        if (x < -144) {
            goto clear;
        }
        if (x >= 145) {
            goto clear;
        }
        y = *(s16 *)(p + 22);
        if (y < -16) {
            goto clear;
        }
        if (y < 257) {
            return;
        }
    clear:
        *(s16 *)(p + 4) = 128;
        *(s16 *)(p + 10) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[15] = 0;
        p[14] = 0;
        p[13] = 0;
        p[81] = 0;
        return;
    }

    x = *(s16 *)(p + 20);
    if (x >= -127 && x < 128) {
        y = *(s16 *)(p + 22);
        if (y > 0 && y < 240) {
            p[81] = 1;
        }
    }
}
