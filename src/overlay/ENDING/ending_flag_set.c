/* @SymbolSize: 40 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 */
#include "types.h"
extern u8 bustup_cw_mode;
s32 ending_flag_set(void)
{
    u8 flags;

    flags = bustup_cw_mode;
    if ((flags & 1) == 0) {
        flags |= 3;
        bustup_cw_mode = flags;
    }
}
