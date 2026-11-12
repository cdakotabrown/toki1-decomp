/* Address: 0x800A411C */
/* Size: 164 bytes (41 instructions) */
/* Region: Library (0x80086000-0x800BFFFF) */
/* @Compiler: gcc260 */
/* @CompileFlags: --delay-slot-load-nop */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s16 rsin_tbl[];
extern s16 libgte_rcos_q2_view[];
extern s16 libgte_rcos_q4_view[];

s32 rcos(s32 a)
{
    if (a < 0) {
        a = -a;
    }
    a = a & 0xfff;
    if (a < 2049) {
        if (a < 1025) {
            return rsin_tbl[1024 - a];
        }
        return -libgte_rcos_q2_view[a];
    }
    if (a < 3073) {
        return -rsin_tbl[3072 - a];
    }
    return libgte_rcos_q4_view[a];
}
