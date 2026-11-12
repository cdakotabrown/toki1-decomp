/* Address: 0x8008FEB8 - 0x8008FEF4 (60 bytes) */
/* Region: IDO (>= 0x80081000) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: fix_sll_sra_pattern */
#include "types.h"

extern void Snd_SetPlayMode(s16, s16, s8, s16);

void SsSepPlay(s16 a0, s16 a1, s8 a2, s16 a3) {
    Snd_SetPlayMode(a0, a1, a2, a3);
}
