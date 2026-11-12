/* Address: 0x800A117C */
/* Size: 220 bytes (55 instructions) */
/* Region: Library */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_XBB_SKIP_CAND=1 IDO52_AS1_NO_FALLTHRU_FILL=occ:1 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s16 D_800EBD64;
extern s16 D_800EBD66;
extern u8  graph_env;

u32 get_cs(s16 arg0, s16 arg1) {
    s16 xmax;
    s16 ymax;
    u8 mode;
    s32 t;

    if (arg0 < 0) {
        arg0 = 0;
    } else {
        xmax = D_800EBD64;
        t = (xmax < arg0) ? xmax : arg0;
        arg0 = t;
    }
    if (arg1 < 0) {
        arg1 = 0;
    } else {
        ymax = D_800EBD66;
        t = (ymax < arg1) ? ymax : arg1;
        arg1 = t;
    }
    mode = graph_env;
    if (mode == 1 || mode == 2) {
        return 0xE3000000u | ((u32)(arg1 & 0xFFF) << 12) | (u32)(arg0 & 0xFFF);
    }
    return 0xE3000000u | ((u32)(arg1 & 0x3FF) << 10) | (u32)(arg0 & 0x3FF);
}
