/* @PostProc: patch_insn_reg_field 0x10:rt:v1:t6 0x18:rs:v1:t6 0x1c:rt:a2:v1 0x20:rd:t6:t7 0x28:rt:t6:t7 0x30:rd:t7:t8 0x34:rs:t7:t8 0x34:rt:t8:t9 0x38:rd:v1:a0 0x38:rs:t8:t9 0x3c:rs:t8:t9 0x3c:rt:a2:v1 0x40:rs:a2:v1 0x4c:rd:t9:t0 0x50:rd:t0:t1 0x54:rt:t9:t0 0x5c:rt:t0:t1 */
/* tt_secs_set - TT overlay, Address: 0x80148704, Size: 80 bytes (20 insns) */
#include "types.h"

extern u8 *tt_str_ptr_tbl[];

void tt_secs_set(u32 value, s32 slot)
{
    u8 *r;
    u32 n;

    r = tt_str_ptr_tbl[slot];
    n = (value & 0xFFFF) % 100;
    n = n & 0xFFFF;
    r[6] = n / 10;
    r[7] = n % 10;
}
