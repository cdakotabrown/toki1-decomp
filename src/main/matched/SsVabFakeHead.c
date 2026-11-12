/* Address: 0x8008C0E8 - 0x8008C114 (44 bytes) */
/* Region: IDO (>= 0x80086000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: fix_sll_sra_pattern */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern void SsVabOpenHeadWithMode(s32, s16, s32, s32);

void SsVabFakeHead(s32 a0, s16 a1, s32 a2) {
    SsVabOpenHeadWithMode(a0, a1, 1, a2);
}
