/* tt_obj_clear - TT overlay, Address: 0x80132F4C, Size: 52 bytes (13 insns) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_registers v0:v1 */
/* @PostProc: patch_insn_reg_field 0x18:rd:v1:v0 0x20:rs:v1:v0 0x20:rt:v1:v0 0x34:rs:v1:v0 */
#include "types.h"

extern s32 tt_obj_pool;

void tt_obj_clear(void)
{
    u8 *p;
    s32 i;

    p = (u8 *)tt_obj_pool;
    i = 0;
    do {
        i += 4;
        p[120] = 0;
        p[240] = 0;
        p[360] = 0;
        p += 480;
        p[-480] = 0;
    } while (i != 32);
}
