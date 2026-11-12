/* tt_pool_reset - TT overlay, Address: 0x8014AF60, Size: 72 bytes (18 insns) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_registers v0:v1 */
/* @PostProc: patch_insn_reg_field 0x18:rd:v1:v0 0x24:rs:v1:v0 0x24:rt:v1:v0 0x48:rs:v1:v0 */
#include "types.h"

extern s32 tt_slot_buf;

void tt_pool_reset(void)
{
    u8 *p;
    s32 i;

    p = (u8 *)tt_slot_buf;
    i = 0;
    do {
        i += 4;
        p[120] = 0;
        *(s16 *)(p + 136) = 4;
        p[240] = 0;
        *(s16 *)(p + 256) = 4;
        p[360] = 0;
        *(s16 *)(p + 376) = 4;
        p += 480;
        p[-480] = 0;
        *(s16 *)(p - 464) = 4;
    } while (i != 64);
}
