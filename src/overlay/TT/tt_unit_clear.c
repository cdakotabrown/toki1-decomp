/* Size: 52 bytes */
/* @SymbolSize: 52 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: patch_insn_reg_field 0x4:rt:v0:a0 0x8:rt:a0:v0 0x14:rs:v0:a0 0x18:rs:v0:a0 0x1c:rs:v0:a0 0x20:rs:v0:a0 0x20:rt:v0:a0 0x24:rt:a0:v0 0x28:rs:v0:a0 */
#include "types.h"

extern u8 *tt_boss_wk;

void tt_unit_clear(void)
{
    s32 i;
    u8 *p;

    p = tt_boss_wk;
    for (i = 0; i < 24; i++) {
        p[784] = 0;
        p++;
    }
}
