/* Size: 124 bytes */
/* @SymbolSize: 124 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x4c */
#include "types.h"

extern u8 class_w[];

void taiiku_anim_offset(void) {
    u8 *p;
    u8 *end;

    end = class_w + 272;
    for (p = class_w; p < end; p += 68) {
        *(s16 *)(p + 6552) += 1;
    }

    *(s16 *)(class_w + 6824) -= 4;
    *(s16 *)(class_w + 6892) -= 3;
    *(s16 *)(class_w + 6960) -= 2;
    *(s16 *)(class_w + 7028) -= 1;
}
