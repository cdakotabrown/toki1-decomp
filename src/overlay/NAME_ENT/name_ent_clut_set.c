/* @CompileFlags: -Wo,-loopunroll,2 */
/* @PostProc: swap_registers v0:v1 --exclude-offsets 0x10,0x14,0x18,0x20,0x34,0x38,0x3c,0x44,0x58,0x5c */
/* @PostProc: patch_insn_reg_field 0x74:rt:v1:v0 0x7c:rs:v1:v0 0x84:rs:v1:v0 0x8c:rd:v1:v0 0x90:rs:v1:v0 0x98:rs:v1:v0 0xa0:rt:v1:v0 0xa8:rs:v1:v0 0xb0:rs:v1:v0 0xb8:rd:v1:v0 0xbc:rs:v1:v0 0xc4:rs:v1:v0 */
/* Address: 0x80145DE0 */
/* Size: 208 bytes (52 instructions) */
#include "types.h"

void name_ent_clut_set(void)
{
    u32 *p;

    for (p = (u32 *)0x801BC004; p != (u32 *)0x801BC200; p++) {
        if ((*p & 0x7FFF0000) == 0) {
            *p |= 0x00017FFF;
        }
        if ((*p & 0x00007FFF) == 0) {
            *p |= 0x7FFF0001;
        }
    }
}
