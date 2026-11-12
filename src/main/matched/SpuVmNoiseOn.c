/* Function: SpuVmNoiseOn */
/* Address:  0x8008DB64 */
/* Size:     44 bytes (11 instructions) */
/* Region:   Library (0x80086000+), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
#include "types.h"

extern void SpuVmNoiseOnWithAdsr(u32 a0, u32 a1, s32 a2, s32 a3);

void SpuVmNoiseOn(u32 a0, u32 a1) {
    a0 &= 0xFFFF;
    a1 &= 0xFFFF;
    SpuVmNoiseOnWithAdsr(a0, a1, 0x80FF, 24520);
}
