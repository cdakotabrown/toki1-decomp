/* @SymbolSize: 384 */
/* @PostProc: patch_insn_reg_field 0x28:rd:v1:a1 0x2c:rt:a1:a3 0x30:rt:a0:a2 0x38:rs:v1:a1 0x38:rt:v1:a1 0x40:rs:v1:a1 0x4c:rs:a0:a2 0x54:rt:a1:a3 0x80:rd:v1:a1 0x84:rt:a2:t0 0x8c:rs:v1:a1 0x8c:rt:v1:a1 0xa0:rs:a0:a2 0xa8:rt:a1:a3 0xc4:rt:t0:t1 0xcc:rs:t0:t1 0xd4:rt:t1:t2 0xdc:rs:a0:a2 0xdc:rt:t1:t2 0xe4:rt:a1:a3 0x100:rt:t2:t3 0x108:rs:t2:t3 0x110:rt:t3:t4 0x118:rs:a0:a2 0x118:rt:t3:t4 0x120:rt:a1:a3 0x13c:rt:t4:t5 0x144:rs:t4:t5 0x14c:rt:t5:t6 0x154:rs:a0:a2 0x154:rt:t5:t6 0x15c:rt:a1:a3 0x178:rs:v1:a1 0x178:rt:a2:t0 */
/* tt_sprite_pool_clear - TT overlay, Address: 0x80142560, Size: 384 bytes (96 insns) */
#include "types.h"

extern void tt_cmd_once(s32 arg0);

extern u8 *tt_slot_buf;
extern u8 *tt_obj_pool;

void tt_sprite_pool_clear(void)
{
    u8 *p;
    s32 i;

    tt_cmd_once(1548);

    p = tt_slot_buf;
    i = 0;
    do {
        i++;
        if (p[0] != 0 && *(u16 *)(p + 4) == 64) {
            *(s16 *)(p + 4) = 112;
            *(s16 *)(p + 10) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[15] = 0;
            p[14] = 0;
            p[13] = 0;
        }
        p += 120;
    } while (i < 64);

    p = tt_obj_pool;
    i = 0;
    do {
        i += 4;

        if (p[0] != 0 && *(u16 *)(p + 4) == 64) {
            *(s16 *)(p + 4) = 112;
            *(s16 *)(p + 10) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[15] = 0;
            p[14] = 0;
            p[13] = 0;
        }
        p += 120;

        if (p[0] != 0 && *(u16 *)(p + 4) == 64) {
            *(s16 *)(p + 4) = 112;
            *(s16 *)(p + 10) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[15] = 0;
            p[14] = 0;
            p[13] = 0;
        }
        p += 120;

        if (p[0] != 0 && *(u16 *)(p + 4) == 64) {
            *(s16 *)(p + 4) = 112;
            *(s16 *)(p + 10) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[15] = 0;
            p[14] = 0;
            p[13] = 0;
        }
        p += 120;

        if (p[0] != 0 && *(u16 *)(p + 4) == 64) {
            *(s16 *)(p + 4) = 112;
            *(s16 *)(p + 10) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[15] = 0;
            p[14] = 0;
            p[13] = 0;
        }
        p += 120;
    } while (i != 32);
}
