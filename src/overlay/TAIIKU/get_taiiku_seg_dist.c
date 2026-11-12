/* @SymbolSize: 180 */
#include "types.h"

typedef struct {
    s16 lo;
    s16 hi;
} Rec80149BA2;

extern Rec80149BA2 taiiku_tama_gcue[];

s16 get_taiiku_seg_dist(s32 i)
{
    s16 v;

    v = taiiku_tama_gcue[i].lo;
    if (v < 1024) {
        return v;
    }
    if (v < 1280) {
        return v - 512;
    }
    if (v < 1536) {
        return v - 512;
    }
    if (v < 2560) {
        return v - 1536;
    }
    if (v < 2816) {
        return v - 2048;
    }
    if (v < 3072) {
        return v - 2048;
    }
    return v - 3072;
}
