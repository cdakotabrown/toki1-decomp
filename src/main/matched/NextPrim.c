/* Address: 0x800A2B98 */
/* Size: 24 bytes (6 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 (as1 expands the wide `or` */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

u32 NextPrim(u32 *p) {
    return ((*p << 8) >> 8) | 0x80000000;
}
