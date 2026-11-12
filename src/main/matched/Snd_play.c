/* Debug Symbol: Snd_play (from o.bin, high match) */
/* Address: 0x800919E0 - 0x80091A10 (48 bytes) */
/* Region: IDO (>= 0x80086000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: fix_sll_sra_pattern */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern void SeqPlay(s32 a0, s32 a1);

void Snd_play(s16 a0, s16 a1) {
    SeqPlay(a0, a1);
}
