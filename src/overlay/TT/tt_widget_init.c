/* Size: 104 bytes */
/* @SymbolSize: 104 */
#include "types.h"
extern u8 *tt_anim_buf;
void tt_widget_init(void)
{
    u8 *p;
    p = tt_anim_buf;
    p[0] = 1;
    p[1] = 0;
    *(s16 *)(p + 68) = 8;
    *(s16 *)(p + 20) = -56;
    *(s16 *)(p + 22) = 72;
    *(s16 *)(p + 60) = 112;
    *(s16 *)(p + 62) = 16;
    p[70] = 0;
    p[71] = 96;
    *(s16 *)(p + 64) = 10;
    *(s16 *)(p + 66) = 15368;
    p[59] = 101;
    *(s16 *)(p + 16) = 0;
}
