/* Size: 152 bytes */
/* @SymbolSize: 152 */
#include "types.h"

extern u8 class_w[][68];

s32 en_nichi_target9_hit_check(s32 a0, s32 a1)
{
    u8 *p;
    u8 *q;
    s32 y1;
    s32 y2;
    s32 x1;
    s32 x2;

    p = class_w[a1];
    y1 = *(s16 *)(p + 7182);
    q = class_w[a0];
    y2 = *(s16 *)(q + 6706);
    if (y1 + 16 < y2) {
        goto fail;
    }
    if (y2 < y1 - 16) {
        goto fail;
    }
    x1 = *(s16 *)(p + 7178);
    x2 = *(s16 *)(q + 6702);
    if (x1 + 8 < x2) {
        goto fail;
    }
    if (x2 < x1 - 8) {
        goto fail;
    }
    if ((*(u8 *)(p + 7143) & 0x80) == 0) {
        goto fail;
    }
    return 1;
fail:
    return 0;
}
