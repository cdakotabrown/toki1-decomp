/* tt_sprite_pool_init - TT overlay, Address: 0x8013BD80, Size: 72 bytes (18 insns) */
/* @PostProc: swap_registers v0:v1 */
/* @PostProc: patch_insn_reg_field 0x18:rd:v1:v0 0x24:rs:v1:v0 0x24:rt:v1:v0 0x48:rs:v1:v0 */
#include "types.h"

extern u8 *tt_bell_buf;

void tt_sprite_pool_init(void)
{
    u8 *p;
    s32 i;

    p = tt_bell_buf;
    for (i = 0; i < 32; i++) {
        p[0] = 0;
        *(s16 *)(p + 16) = 6;
        p += 108;
    }
}
