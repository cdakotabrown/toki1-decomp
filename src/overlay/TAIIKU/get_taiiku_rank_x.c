/* Address: 0x801389E0 (true symbol start 0x801389D0, cross-prologue) */
/* Size: 108 bytes (27 insns incl. the 4 hoisted head words) */
/* @SymbolStart: 0x801389D0 */
/* @SymbolSize: 108 */
#include "types.h"

extern u8 taiiku_kari_rank[];

s32 get_taiiku_rank_x(s32 arg0) {
    s32 v0 = arg0 + 1;
    s32 v1;

    if (v0 == taiiku_kari_rank[0]) v1 = 0;
    if (v0 == taiiku_kari_rank[1]) v1 = 1;
    if (v0 == taiiku_kari_rank[2]) v1 = 2;
    if (v0 == taiiku_kari_rank[3]) v1 = 3;

    return (s16)((v1 << 6) + 160);
}
