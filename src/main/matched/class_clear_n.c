/* Address: 0x800491F0 */
/* Size: 164 bytes (41 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0xc:rd:t7:t6 0x10:rt:t9:t7 0x14:rs:t9:t7 0x14:rt:t9:t7 0x18:rd:t8:t6 0x18:rt:t7:t6 0x1c:rs:t8:t6 0x1c:rt:t9:t7 0x20:rt:t0:t8 0x40:rt:t0:t8 0x5c:rt:t1:t9 0x78:rt:t1:t9 */
#include "types.h"

extern u8 class_w[];

void class_clear_n(s32 a0) {
    u8 *slot;
    s32 idx;
    if (a0 >= 160) {
        return;
    }
    idx = (a0 << 4) + a0;
    idx = idx << 2;
    slot = class_w + idx;
    slot[0] = 0;
    *(u32 *)(slot + 56) = 0;
    slot[1] = 0;
    slot[4] = 0;
    slot[5] = 0;
    slot[2] = 0;
    slot[3] = 0;
    slot[6] = 1;
    *(u16 *)(slot + 8) = 0;
    if (a0 != 0) {
        *(u32 *)(slot + 12) = 0;
        *(u32 *)(slot + 16) = 0;
        *(u32 *)(slot + 52) = 0;
    }
    *(u16 *)(slot + 20) = 0;
    *(u16 *)(slot + 22) = 0;
    *(u16 *)(slot + 24) = 0;
    *(u16 *)(slot + 26) = 4096;
    *(u16 *)(slot + 28) = 4096;
    slot[67] = 0;
    slot[7] = 128;
    *(u32 *)(slot + 32) = 0;
    *(u32 *)(slot + 36) = 0;
    *(u32 *)(slot + 40) = 0;
    *(u32 *)(slot + 44) = 0;
    *(u32 *)(slot + 48) = 0;
    slot[64] = 0;
    slot[65] = 0;
    slot[66] = 0;
}
