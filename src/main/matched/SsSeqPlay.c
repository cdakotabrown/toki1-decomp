/* Address: 0x8008FE80, Size: 56 bytes (14 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileEnv: IDO52_UGEN_NO_ARG_COALESCE=1 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_registers t6:a0 --unidirectional */
/* @PostProc: swap_registers t8:a3 --unidirectional */
#include "types.h"

extern void Snd_SetPlayMode(s16, s32, s8, s16);

void SsSeqPlay(s16 a, s8 b, s16 c) {
    Snd_SetPlayMode(a, 0, b, c);
}
