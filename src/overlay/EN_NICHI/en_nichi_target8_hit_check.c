/* Size: 240 bytes */
/* @SymbolSize: 240 */
#include "types.h"

extern u8 class_w[][68];

s32 en_nichi_target8_hit_check(s32 a0, s32 a1)
{
    u8 *p;
    u8 *q;
    s32 y1;
    s32 y2;
    s32 x1;
    s32 x2;
    s32 hi;

    p = class_w[a1];
    y1 = *(s16 *)(p + 7182);
    hi = y1 + 8;
    q = class_w[a0];
    y2 = *(s16 *)(q + 6706);
    if (hi < y2) {
        goto second;
    }
    if (y2 < y1 - 8) {
        goto second;
    }
    x1 = *(s16 *)(p + 7178);
    x2 = *(s16 *)(q + 6702);
    if (x1 + 16 < x2) {
        goto second;
    }
    if (x2 < x1 - 16) {
        goto second;
    }
    if ((*(u8 *)(p + 7143) & 0x80) == 0) {
        goto second;
    }
    return 1;
second:
    if (y1 + 16 < y2) {
        goto fail;
    }
    if (y2 < hi) {
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
