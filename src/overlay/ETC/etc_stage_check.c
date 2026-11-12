/* @SymbolSize: 160 */
/* Address: 0x8013F3C8 */
/* Size: 160 bytes (40 instructions) */
#include "types.h"

extern s32 etc_rec_kind;
extern s32 etc_rec_id;
extern s32 etc_rec_variant;
extern s32 etc_rec_girl;

s32 etc_stage_check(void)
{
    s32 v0;

    if (etc_rec_kind != 21) {
        goto ret0;
    }
    v0 = etc_rec_id;
    if (v0 != 6 && v0 != 7 && v0 != 8 && v0 != 9 && v0 != 10) {
        goto ret0;
    }
    v0 = etc_rec_girl;
    if (v0 != 0 && v0 != 1 && v0 != 4 && v0 != 6 && v0 != 9 && v0 != 10) {
        goto ret0;
    }
    if (etc_rec_variant != 5) {
        goto ret0;
    }
    return 1;
ret0:
    return 0;
}
