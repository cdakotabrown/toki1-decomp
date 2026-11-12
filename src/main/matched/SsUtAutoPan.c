/* Function: SsUtAutoPan */
/* Address: 0x8008F864 */
/* Size: 56 bytes (14 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x8008F864 */
/* @SymbolSize: 56 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field --patch 0x0:rd:t6:a0 --patch 0x4:rt:t6:a0 --patch 0x4:rd:t7:a0 --patch 0x8:rs:t7:a0 --patch 0x1C:rs:t7:a0 */
#include "types.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} SpuVoiceArg12;

s32 SsUtAutoPan(s16 vc, SpuVoiceArg12 arg)
{
    if (vc < 24) {
        if (vc >= 0) {
            return 0;
        }
    }
    return -1;
}
