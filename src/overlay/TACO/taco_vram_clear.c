/* Address: 0x80135954 */
/* Size: 80 bytes (20 insns) */
#include "types.h"

typedef struct {
    s16 field0;
    s16 field2;
    s16 field4;
    s16 field6;
} Rect;

extern void DrawSync();
extern void ClearImage();

void taco_vram_clear(void)
{
    Rect rect;
    rect.field0 = 704;
    rect.field2 = 0;
    rect.field4 = 320;
    rect.field6 = 20;
    ClearImage(&rect, 0, 0, 0);
    DrawSync(0);
}
