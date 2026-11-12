/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v0 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x18:rd:v1:v0 0x20:rs:v1:v0 0x20:rt:v1:v0 0x24:rs:v0:v1 0x28:rs:v0:v1 0x2c:rs:v0:v1 0x30:rs:v0:v1 0x30:rt:v0:v1 0x34:rs:v1:v0 0x38:rs:v0:v1 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* tt_record_clear - TT overlay, Address: 0x8014CBC0, Size: 52 bytes (13 insns) */
#include "types.h"

extern u8 *tt_anim_buf;

void tt_record_clear(void)
{
    u8 *p;
    s32 i;

    p = tt_anim_buf;
    i = 0;
    do {
        i += 4;
        p[100] = 0;
        p[200] = 0;
        p[300] = 0;
        p += 400;
        p[-400] = 0;
    } while (i != 128);
}
