/* Address: 0x8009ADA0 */
/* Size: 124 bytes (31 instructions); the window used to include the next function at 0x8009AE1C */
/* @SymbolSize: 124 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
/* Region: Library */
#include "types.h"

typedef struct {
    u32 w[8];
} Rec8;

extern Rec8 GsIDMATRIX;

void GsInitCoordinate2(s32 mode, u32 *dst)
{
    *(Rec8 *)(dst + 1) = GsIDMATRIX;
    dst[18] = mode;
    dst[0] = 0;
    if (mode != 0) {
        if (mode != 1) {
            ((u32 *)dst[18])[19] = (u32)dst;
        }
    }
}
