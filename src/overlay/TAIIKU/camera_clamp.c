/* Address: 0x8013C8DC */
/* Size: 236 bytes (59 insns) */
/* @SymbolSize: 236 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
#include "types.h"

extern s16 taiiku_tama_sx;
extern s16 taiiku_tama_sy;
extern volatile s16 cw97_x;
extern s16 cw97_y;

void camera_clamp(void) {
    s32 lim;
    s32 cur;
    s32 mid;
    s32 lo;
    s32 hi;

    lim = (taiiku_tama_sy >> 3) + 16;
    if (cw97_y < lim) {
        cur = cw97_x;
        mid = taiiku_tama_sx >> 3;
        if (cur < mid + 80 && mid - 80 < cur) {
            cw97_y = lim;
        }
    }

    cur = cw97_x;
    lo = (taiiku_tama_sx >> 3) - 80;
    hi = (taiiku_tama_sx >> 3) + 80;
    mid = taiiku_tama_sx >> 3;
    if (lo < cur && cur < (taiiku_tama_sx >> 3) && cw97_y < lim) {
        cur = (s16)lo;
    }
    cw97_x = cur;
    if (cur < hi) {
        cw97_x = cur;
        if (mid < cur && cw97_y < lim) {
            cw97_x = hi;
        }
    }
}
