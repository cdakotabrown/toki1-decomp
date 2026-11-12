/* Address: 0x8013FB60 */
/* Size: 96 bytes */
/* @SymbolSize: 96 */
/* @CompileFlags: -Wo,-loopunroll,0 */
#include "types.h"

extern u8 D_800EC190[];

void func_8013FB60(s16 dx, s16 dy)
{
    s16 i;

    for (i = 0; i < 6; i++) {
        *(s16 *)&D_800EC190[i * 36 + 148] += dx;
        *(s16 *)&D_800EC190[i * 36 + 150] += dy;
    }
}
