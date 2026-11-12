/* Address: 0x800A10D8 */
/* Size: 164 bytes (41 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_LA_PULL=all */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 graph_env;

u32 get_mode(s32 dfe, s32 dtd, s32 tpage) {
    s32 t;
    s32 d;

    tpage = tpage & 0xFFFF;
    if (graph_env == 1 || graph_env == 2) {
        if (dtd) {
            t = 2048;
        } else {
            t = 0;
        }
        if (dfe) {
            d = 4096;
        } else {
            d = 0;
        }
        return 0xE1000000 | t | d | (tpage & 0x27FF);
    }
    if (dtd) {
        t = 512;
    } else {
        t = 0;
    }
    if (dfe) {
        d = 1024;
    } else {
        d = 0;
    }
    return 0xE1000000 | t | d | (tpage & 0x9FF);
}
