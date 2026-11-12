/* Address: 0x80148EB0 */
/* Size: 72 bytes (18 instructions) */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} Rect;

extern u8 etc_num_clut;

extern void LoadImage();

void etc_clut_load(void)
{
    Rect rect;

    rect.x = 256;
    rect.y = 496;
    rect.w = 16;
    rect.h = 2;
    LoadImage(&rect, &etc_num_clut);
}
