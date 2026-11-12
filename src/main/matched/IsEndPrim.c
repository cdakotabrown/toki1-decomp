/* Address: 0x800A2BB0 */
/* Size: 32 bytes (8 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 (as1-assembled; the 0xFFFFFF */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_UGEN_TEMP_REG=1:v0 IDO52_UGEN_TEMP_REG_INPLACE=occ:1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

s32 IsEndPrim(u32 *p) {
    return ((*p << 8) >> 8) == 0xFFFFFF;
}
