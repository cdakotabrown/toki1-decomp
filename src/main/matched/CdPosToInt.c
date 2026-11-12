/* Address: 0x800881D4 */
/* Size: 128 bytes (32 instructions) -- the 3 words to 0x80088258 are alignment nops */
/* @SymbolSize: 128 */
/* Region: Library */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

s32 CdPosToInt(u8 *p)
{
    u32 b0;
    u32 b1;
    u32 b2;

    b0 = p[0];
    b1 = p[1];
    b2 = p[2];
    return (((b0 >> 4) * 10 + (b0 & 0xF)) * 60 + ((b1 >> 4) * 10 + (b1 & 0xF))) * 75
           + ((b2 >> 4) * 10 + (b2 & 0xF)) - 150;
}
