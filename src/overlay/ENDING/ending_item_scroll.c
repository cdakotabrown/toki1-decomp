/* @SymbolSize: 144 */
/* ending_item_scroll - ENDING overlay, Address: 0x80136CF0, Size: 144 bytes (36 insns) */
#include "types.h"

extern u8 Sprites[];
extern u8 class_w[];

void ending_item_scroll(s16 delta)
{
    s16 i;
    u8 *rec;

    for (i = 0; i < 3; i++) {
        rec = Sprites + i * 36;
        *(s16 *)(rec + 582) += delta;
    }
    for (i = 0; i < 14; i++) {
        rec = class_w + i * 68;
        *(s16 *)(rec + 6570) += delta;
    }
}
