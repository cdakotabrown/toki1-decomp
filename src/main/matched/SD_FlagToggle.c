/* Address: 0x8007C04C */
/* Size: 60 bytes (15 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x8007C048 */
/* @SymbolSize: 64 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x30 */
/* @PostProc: patch_insn_reg_field 0x8:rt:at:v1 0x30:rd:t8:v0 0x30:rt:at:v1 0x34:rt:t8:v0 */
#include "types.h"

extern s32 gSDInfo;

void SD_FlagToggle(void) {
    s32 v;

    v = gSDInfo;
    if ((v << 3) < 0) {
        gSDInfo &= ~0x10000000;
    } else {
        gSDInfo |= 0x10000000;
    }
}
