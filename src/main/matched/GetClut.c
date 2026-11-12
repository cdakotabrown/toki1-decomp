/* Address: 0x800A2A74 */
/* Size: 24 bytes (6 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -O2 */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

s32 GetClut(s32 a0, s32 a1)
{
    return ((a1 << 6) | ((a0 >> 4) & 0x3F)) & 0xFFFF;
}
