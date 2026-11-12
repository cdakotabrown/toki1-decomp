/* Address: 0x800A139C */
/* Size: 120 bytes (30 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -- libgpu, recompiled */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_XBB_SKIP_CAND=all */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

u32 get_tw(RECT *tw) {
    s32 x;
    s32 y;
    s32 w;
    s32 h;

    if (tw != 0) {
    x = tw->x;
    w = tw->w;
    y = tw->y;
    h = tw->h;
    y &= 0xFF;
    h -= 1;
    y >>= 3;
    h = ~h;
    x &= 0xFF;
    x >>= 3;
    h &= 0xFF;
    w -= 1;
    w = ~w;
    h >>= 3;
    w &= 0xFF;
    w >>= 3;
    return 0xE2000000 | (y << 15) | (x << 10) | (h << 5) | w;
    }
    return 0;
}
