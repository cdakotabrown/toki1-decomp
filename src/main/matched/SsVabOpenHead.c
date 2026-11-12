/* Address: 0x8008C090 - 0x8008C0BC (44 bytes) */
/* Region: IDO (>= 0x80086000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: fix_sll_sra_pattern */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern void SsVabOpenHeadWithMode(s32 a0, s32 a1, s32 a2, s32 a3);

void SsVabOpenHead(s32 a0, s16 a1) {
    SsVabOpenHeadWithMode(a0, a1, 0, 0);
}
