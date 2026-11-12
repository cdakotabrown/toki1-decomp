/* Address: 0x80095240 */
/* Size: 204 bytes (51 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @CompileEnv: IDO52_AS1_KEEP_ANDI=all IDO52_AS1_XBB_SKIP_CAND=all IDO52_UGEN_FORCE_TARGET_SAFE=all */
#include "types.h"

void SsUtBuildADSR(s16 *a0, u16 *a1, u16 *a2) {
    u16 x;
    u16 y;

    if (a0[5] == 0) {
        x = 0;
    } else {
        x = 0x8000;
    }

    if (a0[6] == 0) {
        y = 0;
    } else {
        y = 0x8000;
    }

    if (a0[8] != 0) {
        y |= 0x4000;
    }

    x |= (u16)((a0[0] << 8) & 0x7F00);
    x |= (u16)((a0[1] << 4) & 0x00F0);
    x |= (u16)(a0[2] & 0x000F);
    y |= (u16)((a0[3] << 6) & 0x1FC0);
    y |= (u16)(a0[4] & 0x001F);

    *a1 = x;
    *a2 = y;
}
