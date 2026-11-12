/* @SymbolSize: 148 */
#include "types.h"

typedef struct {
    s16 lo;
    s16 hi;
} Rec80149BA2;

extern Rec80149BA2 taiiku_tama_gcue[];

s32 get_taiiku_seg(s32 i)
{
    s16 v;

    v = taiiku_tama_gcue[i].lo;
    if (v < 1024) {
        return 0;
    }
    if (v < 1280) {
        return 1;
    }
    if (v < 1536) {
        return 2;
    }
    if (v < 2560) {
        return 3;
    }
    if (v < 2816) {
        return 4;
    }
    if (v < 3072) {
        return 5;
    }
    return 6;
}
