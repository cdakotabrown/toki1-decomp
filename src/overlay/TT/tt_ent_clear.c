/* tt_ent_clear - TT overlay, Address: 0x801473AC, Size: 44 bytes (11 insns) */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x10:rt:t6:v0 0x14:rs:t6:v0 0x14:rt:t6:v0 0x20:rs:t6:v0 0x24:rs:t6:v0 0x28:rs:t6:v0 0x2c:rs:t6:v0 0x30:rs:t6:v0 */
#include "types.h"

extern u8 *tt_ent_buf;

void tt_ent_clear(void)
{
    u8 *p;

    p = tt_ent_buf;
    p[0] = 0;
    p[104] = 0;
    p[312] = 0;
    p[416] = 0;
    p += 520;
    p[0] = 0;
    p[-312] = 0;
}
