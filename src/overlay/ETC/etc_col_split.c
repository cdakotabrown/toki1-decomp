/* Address: 0x80132880 */
/* Size: 36 bytes (9 instructions) */
#include "types.h"

typedef struct {
    u16 color;
    u16 pad;
    u32 r;
    u32 g;
    u32 b;
} ColorSplit;

void etc_col_split(ColorSplit *obj)
{
    u32 c;

    c = obj->color;
    obj->r = c & 0x1F;
    obj->g = c & 0x3E0;
    obj->b = c & 0x7C00;
}
