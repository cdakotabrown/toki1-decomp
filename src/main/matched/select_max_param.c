/* Address: 0x8009AA14 */
/* Size: 204 bytes (51 instructions).  ⚠️ The recorded 236 was WRONG: it ran */
/* Region: Library */
/* @SymbolStart: 0x8009AA14 */
/* @SymbolSize: 204 */
/* @PostProc: strip_leading_instructions --count auto */
/* @Compiler: gcc260 */
/* @CompileFlags: --jal-load-nop */
#include "types.h"
#define ABS(x) ((x) < 0 ? -(x) : (x))
s32 select_max_param(s32 *p)
{
    s32 best, v, t;
    t = p[0];
    v = p[1];
    best = ABS(t);
    v = ABS(v);
    if (best < v) best = v;
    v = ABS(p[2]); if (best < v) best = v;
    v = ABS(p[3]); if (best < v) best = v;
    v = ABS(p[4]); if (best < v) best = v;
    v = ABS(p[5]); if (best < v) best = v;
    return best;
}
