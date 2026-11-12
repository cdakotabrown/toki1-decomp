/* en_nichi_obj_octant_get - EN_NICHI overlay, Address: 0x80134184, Size: 196 bytes (49 insns) */
#include "types.h"

extern u8 en_nichi_obj_wk[][48];

s32 en_nichi_obj_octant_get(s32 a0)
{
    s32 v;

    v = ((s32 *)en_nichi_obj_wk[a0])[7];
    if (v < 256) {
        return 0;
    }
    if (v < 768) {
        return 1;
    }
    if (v < 1280) {
        return 2;
    }
    if (v < 1792) {
        return 3;
    }
    if (v < 2304) {
        return 4;
    }
    if (v < 2816) {
        return 5;
    }
    if (v < 3328) {
        return 6;
    }
    if (v < 3840) {
        return 7;
    }
    return 0;
}
