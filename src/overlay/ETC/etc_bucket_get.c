/* @SymbolSize: 148 */
/* Address: 0x8014BE18 */
/* Size: 148 bytes (37 instructions) */
#include "types.h"

extern s32 get_etc_gakuryoku(void);
extern s16 para_zat;
extern s16 para_you;

s32 etc_bucket_get(void) {
    s32 a0;
    s32 v1;

    a0 = -get_etc_gakuryoku() * 2 + 6;
    v1 = a0;
    if (para_zat >= 61) {
        v1 = a0 + 1;
    }
    if (para_you >= 61) {
        v1 = v1 + 1;
    }
    if (v1 >= 7) {
        return 0;
    }
    if (v1 >= 5) {
        return 1;
    }
    if (v1 >= 3) {
        return 2;
    }
    return 3;
}
