/* Address: 0x800A2CD4 */
/* Size: 60 bytes (15 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 -O2 codegen, PSY-Q-style assembly (the THIRD pipeline) */
/* Closed the same way as its sister, the same day: `@Compiler: ido52-gas` */
/* @Compiler: ido52-gas */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

void SetShadeTex(u8 *p, s32 on)
{
    s32 v;

    if (on != 0) {
        v = p[7] | 1;
        v = v & 0xFF;
        p[7] = v;
        return;
    }
    v = p[7] & ~1;
    v = v & 0xFF;
    p[7] = v;
}
