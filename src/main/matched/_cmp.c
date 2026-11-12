/* Address: 0x80089264 */
/* Size: 32 bytes (8 instructions) */
/* Region: Library (GCC) */
/* @Compiler: gcc272 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 strncmp(void *a, void *b, s32 n);

s32 _cmp(void *a, void *b)
{
    return strncmp(a, b, 12) == 0;
}
