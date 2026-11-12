/* Address: 0x8013C810 */
/* Size: 204 bytes (51 insns) */
/* @SymbolSize: 204 */
#include "types.h"

extern u8 class_w[];
extern s16 taiiku_tama_gdst;
extern s16 taiiku_tama_gdst_1;
extern s16 cw97_x;
extern s16 cw97_y;

void bg_layer_scroll(s32 delta) {
    s16 limit;

    limit = taiiku_tama_gdst;
    if (limit + 2 < cw97_x) {
        *(volatile s32 *)(class_w + 6632) = *(volatile s32 *)(class_w + 6632) - delta;
    } else {
        if (*(volatile s16 *)(class_w + 6634) + 2 < limit) {
            *(volatile s32 *)(class_w + 6632) = *(volatile s32 *)(class_w + 6632) + delta;
        }
    }

    limit = taiiku_tama_gdst_1;
    if (limit + 2 < cw97_y) {
        *(volatile s32 *)(class_w + 6636) = *(volatile s32 *)(class_w + 6636) - delta;
        return;
    }

    if (*(volatile s16 *)(class_w + 6638) + 2 < limit) {
        *(volatile s32 *)(class_w + 6636) = *(volatile s32 *)(class_w + 6636) + delta;
    }
}
