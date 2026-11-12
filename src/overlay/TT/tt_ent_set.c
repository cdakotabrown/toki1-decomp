/* tt_ent_set - TT overlay, Address: 0x8014741C, Size: 16 bytes (4 insns) */
/* @PostProc: patch_insn_reg_field 0x10:rt:t8:t7 0x14:rt:t8:t7 */
#include "types.h"

void tt_ent_set(u8 *arg0, s16 arg1)
{
    arg0[0] = 1;
    *(s16 *)(arg0 + 2) = arg1;
}
