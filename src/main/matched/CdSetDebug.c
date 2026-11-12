/* Address: 0x80087900 */
/* Size: 20 bytes (5 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --fill-earliest */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 CD_debug;

s32 CdSetDebug(s32 val) {
    s32 old = CD_debug;
    CD_debug = val;
    return old;
}
