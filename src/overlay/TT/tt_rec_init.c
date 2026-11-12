/* @SymbolSize: 340 */
#include "types.h"

extern u8 *tt_player_wk;
extern u8 *tt_trail_buf;

void tt_rec_init(u8 *dst, s32 idx)
{
    u8 *src;
    s32 n;

    n = idx >> 1;
    src = tt_player_wk;
    if (idx < 2) {
        if (dst[61] != 0) {
            *(s16 *)(dst + 20) = *(s16 *)(src + 20) + 6;
        } else {
            *(s16 *)(dst + 20) = *(s16 *)(src + 20) - 6;
        }
        *(s32 *)(dst + 32) = *(s16 *)(dst + 20) << 16;
        *(s16 *)(dst + 22) = *(s16 *)(src + 22) - 16;
        *(s32 *)(dst + 36) = *(s16 *)(dst + 22) << 16;
        *(s32 *)(dst + 44) = 0xFFFC0000;
        dst[68] = 160;
        dst[69] = 208;
        *(s16 *)(dst + 16) = 3;
        *(s16 *)(dst + 64) = 10;
        *(s16 *)(dst + 66) = 15489;
        *(s16 *)(dst + 70) = 0;
    } else {
    src = tt_trail_buf + 164 * n - 164;
    if (dst[61] != 0) {
        *(s16 *)(dst + 20) = *(s16 *)(src + 20) + 6;
    } else {
        *(s16 *)(dst + 20) = *(s16 *)(src + 20) - 6;
    }
    *(s32 *)(dst + 32) = *(s16 *)(dst + 20) << 16;
    *(s16 *)(dst + 22) = *(s16 *)(src + 22) - 16;
    *(s32 *)(dst + 36) = *(s16 *)(dst + 22) << 16;
    *(s32 *)(dst + 44) = 0xFFFC0000;
    dst[68] = 160;
    dst[69] = 208;
    *(s16 *)(dst + 16) = 3;
    *(s16 *)(dst + 64) = 10;
    *(s16 *)(dst + 66) = 15489;
    *(s16 *)(dst + 70) = 0;
    }
}
