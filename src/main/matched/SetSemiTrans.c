/* Address: 0x800A2C98 */
/* Size: 60 bytes (15 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 -O2 codegen, PSY-Q-style assembly (the THIRD pipeline) */
/* ---- HOW IT CLOSED: `@Compiler: ido52-gas` ------------------------------ */
/* @Compiler: ido52-gas */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

void SetSemiTrans(u8 *p, s32 abe) {
    u32 x;

    if (abe) {
        x = p[7];
        x = x | 2;
        x = x & 0xFF;
        p[7] = x;
    } else {
        x = p[7];
        x = x & ~2;
        x = x & 0xFF;
        p[7] = x;
    }
}
