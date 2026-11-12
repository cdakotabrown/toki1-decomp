/* @CompileFlags: -Wo,-loopunroll,0 */
/* Size: 52 bytes */
/* @SymbolSize: 52 */
#include "types.h"

extern u8 *tt_text_buf;

void tt_text_clear(void)
{
    s32 i;
    u8 *p;

    for (i = 0, p = tt_text_buf; i != 128; i += 4) {
        p[20] = 0;
        p[40] = 0;
        p[60] = 0;
        p += 80;
        p[-80] = 0;
    }
}
