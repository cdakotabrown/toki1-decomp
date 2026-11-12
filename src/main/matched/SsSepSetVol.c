/* Address: 0x8008C738 - 0x8008C774 (60 bytes) */
/* Region: IDO (>= 0x80081000) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: fix_sll_sra_pattern */
#include "types.h"

extern void Snd_SetVol(s16, s16, s16, s16);

void SsSepSetVol(s16 a0, s16 a1, s16 a2, s16 a3) {
    Snd_SetVol(a0, a1, a2, a3);
}
