/* Address: 0x800951E0 */
/* Size: 96 bytes (24 instructions) -- the old 92 dropped the last word */
/* Region: Library (0x80086000+) */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_KEEP_ANDI=all */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x4c */
#include "types.h"

void SsUtResolveADSR(u16 x, u16 y, s16 *out)
{
    out[5] = x & 0x8000;
    out[6] = y & 0x8000;
    out[8] = y & 0x4000;
    out[7] = y & 0x20;
    out[0] = (x >> 8) & 0x7F;
    out[1] = (x >> 4) & 0xF;
    out[2] = x & 0xF;
    out[3] = (y >> 6) & 0x7F;
    out[4] = y & 0x1F;
}
