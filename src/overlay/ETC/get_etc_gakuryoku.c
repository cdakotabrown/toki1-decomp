/* @SymbolStart: 0x8014BD60 */
/* @SymbolSize: 184 */
/* Address: 0x8014BD68 */
/* Size: 176 bytes (44 instructions) */
#include "types.h"

extern void printf(s32 *table, s32 count);
extern s16 para_bun;
extern s16 para_ri;
extern s16 para_gei;
extern s32 etc_gakuryoku_thl_fmt[];

s32 get_etc_gakuryoku(void) {
    s32 v1 = para_bun;
    s32 v0;
    s32 a1;

    v0 = 0;
    a1 = 0;
    if (v1 >= 121) {
        a1 = 1;
        v0 = v1 - 120;
    }
    v1 = para_ri;
    if (v1 >= 121) {
        v0 += v1;
        a1++;
        v0 -= 120;
    }
    v1 = para_gei;
    if (v1 >= 121) {
        v0 += v1;
        a1++;
        v0 -= 120;
    }
    if (a1 == 3 && v0 >= 201) {
        a1 = 3;
    } else if (a1 != 0) {
        a1 = a1 - 1;
    }
    printf(etc_gakuryoku_thl_fmt, a1);
    return 3 - a1;
}
