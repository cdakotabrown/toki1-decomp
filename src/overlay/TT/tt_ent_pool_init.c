/* tt_ent_pool_init - TT overlay, Address: 0x801473D8, Size: 68 bytes (17 insns) */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x10:rt:t6:v0 0x14:rs:t6:v0 0x14:rt:t6:v0 0x1c:rs:t6:v0 0x20:rs:t6:v0 0x24:rs:t6:v0 0x28:rs:t6:v0 0x2c:rs:t6:v0 0x34:rs:t6:v0 0x38:rs:t6:v0 0x3c:rs:t6:v0 0x40:rs:t6:v0 0x44:rs:t6:v0 */
#include "types.h"

extern u8 *tt_ent_buf;

void tt_ent_pool_init(void)
{
    u8 *p;

    p = tt_ent_buf;
    *(u16 *)(p + 16) = 5;
    *(u16 *)(p + 120) = 5;
    *(u16 *)(p + 328) = 5;
    *(u16 *)(p + 432) = 5;
    *(u16 *)(p + 536) = 5;
    p[0] = 0;
    p[104] = 0;
    p[312] = 0;
    p[416] = 0;
    p += 520;
    p[0] = 0;
    p[-312] = 0;
    *(u16 *)(p - 296) = 5;
}
