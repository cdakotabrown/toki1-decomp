/* Address: 0x800A1258 */
/* Size: 220 bytes (55 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -O2 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_XBB_SKIP_CAND=0,1,2,3,4,6,8,9 IDO52_AS1_NO_FALLTHRU_FILL=occ:1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern u8  graph_env;
extern s16 D_800EBD64;
extern s16 D_800EBD66;

u32 get_ce(s16 x, s16 y)
{
    s32 t;

    if (x < 0) {
        x = 0;
    } else {
        if (D_800EBD64 < x) t = D_800EBD64; else t = x;
        x = t;
    }
    if (y < 0) {
        y = 0;
    } else {
        if (D_800EBD66 < y) t = D_800EBD66; else t = y;
        y = t;
    }
    if (graph_env == 1 || graph_env == 2) {
        return 0xE4000000 | ((y & 0xFFF) << 12) | (x & 0xFFF);
    }
    return 0xE4000000 | ((y & 0x3FF) << 10) | (x & 0x3FF);
}
