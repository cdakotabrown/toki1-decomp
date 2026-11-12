/* @SymbolSize: 360 */
/* @PostProc: patch_insn_reg_field 0x8:rd:v1:v0 0x0:rt:v0:v1 0x4:rs:v0:v1 0x4:rt:v0:v1 0xc:rt:a1:a3 0x10:rt:a0:a2 0x14:rs:v0:v1 0x18:rs:v1:v0 0x18:rt:v1:v0 0x20:rs:v1:v0 0x24:rs:v0:v1 0x2c:rs:a0:a2 0x34:rs:v0:v1 0x34:rt:a1:a3 0x38:rs:v0:v1 0x3c:rs:v0:v1 0x40:rs:v0:v1 0x44:rs:v0:v1 0x48:rs:v0:v1 0x4c:rs:v0:v1 0x54:rs:v0:v1 0x54:rt:v0:v1 0x58:rt:v0:v1 0x5c:rs:v0:v1 0x5c:rt:v0:v1 0x60:rd:v1:v0 0x64:rt:a1:a3 0x68:rs:v0:v1 0x6c:rs:v1:v0 0x6c:rt:v1:v0 0x78:rs:v0:v1 0x80:rs:a0:a2 0x88:rs:v0:v1 0x8c:rs:v0:v1 0x90:rs:v0:v1 0x94:rs:v0:v1 0x98:rs:v0:v1 0x9c:rs:v0:v1 0xa0:rs:v0:v1 0xa4:rs:v0:v1 0xa8:rs:v0:v1 0xa8:rt:v0:v1 0xb4:rs:v0:v1 0xbc:rs:a0:a2 0xc4:rs:v0:v1 0xc8:rs:v0:v1 0xcc:rs:v0:v1 0xd0:rs:v0:v1 0xd4:rs:v0:v1 0xd8:rs:v0:v1 0xdc:rs:v0:v1 0xe0:rs:v0:v1 0xe4:rs:v0:v1 0xe4:rt:v0:v1 0xf0:rs:v0:v1 0xf8:rs:a0:a2 0x100:rs:v0:v1 0x104:rs:v0:v1 0x108:rs:v0:v1 0x10c:rs:v0:v1 0x110:rs:v0:v1 0x114:rs:v0:v1 0x118:rs:v0:v1 0x11c:rs:v0:v1 0x120:rs:v0:v1 0x120:rt:v0:v1 0x12c:rs:v0:v1 0x134:rs:a0:a2 0x13c:rs:v0:v1 0x140:rs:v0:v1 0x144:rs:v0:v1 0x148:rs:v0:v1 0x14c:rs:v0:v1 0x150:rs:v0:v1 0x154:rs:v0:v1 0x158:rs:v1:v0 0x158:rt:a1:a3 0x15c:rs:v0:v1 0x15c:rt:v0:v1 */
/* tt_entry_clear - TT overlay, Address: 0x80145360, Size: 360 bytes (90 insns) */
#include "types.h"

extern s32 tt_slot_buf;
extern s32 tt_shot_buf;

void tt_entry_clear(void)
{
    u8 *p;
    s32 i;

    p = (u8 *)tt_slot_buf;
    for (i = 0; i < 64; i++) {
        if (p[0] != 0 && *(u16 *)(p + 4) == 64) {
            *(u16 *)(p + 4) = 112;
            *(u16 *)(p + 10) = 0;
            *(u16 *)(p + 8) = 0;
            *(u16 *)(p + 6) = 0;
            p[15] = 0;
            p[14] = 0;
            p[13] = 0;
        }
        p += 120;
    }

    p = (u8 *)tt_shot_buf;
    for (i = 0; i != 128; i++) {
        if (p[0] != 0 && *(u16 *)(p + 4) == 64) {
            p[0] = 0;
            *(u16 *)(p + 10) = 0;
            *(u16 *)(p + 8) = 0;
            *(u16 *)(p + 6) = 0;
            p[15] = 0;
            p[14] = 0;
            p[13] = 0;
        }
        p += 104;
    }
}
