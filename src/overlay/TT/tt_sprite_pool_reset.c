/* tt_sprite_pool_reset - TT overlay, Address: 0x8013BD4C, Size: 52 bytes (13 insns) */
/* @PostProc: swap_registers v0:v1 */
/* @PostProc: patch_insn_reg_field 0x18:rd:v1:v0 0x20:rs:v1:v0 0x20:rt:v1:v0 0x34:rs:v1:v0 */
#include "types.h"

extern u8 *tt_bell_buf;

void tt_sprite_pool_reset(void)
{
    u8 *p;
    s32 i;

    p = tt_bell_buf;
    for (i = 0; i < 32; i++) {
        p[0] = 0;
        p += 108;
    }
}
