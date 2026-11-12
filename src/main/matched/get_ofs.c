/* Address: 0x800A1334 */
/* Size: 104 bytes (26 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -O2 */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_XBB_SKIP_CAND=occ:0,1 IDO52_AS1_NO_FALLTHRU_FILL=occ:0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern u8 graph_env;

u32 get_ofs(s16 x, s16 y)
{
    if (graph_env == 1 || graph_env == 2) {
        return 0xE5000000 | ((y & 0xFFF) << 12) | (x & 0xFFF);
    }
    return 0xE5000000 | ((y & 0x7FF) << 11) | (x & 0x7FF);
}
