/* Address: 0x800A30B0 */
/* Size: 84 bytes (21 instructions) */
/* Region: Library (0x80086000+) */
/* @CompileEnv: IDO52_AS1_KEEP_ANDI=occ:2 IDO52_AS1_XBB_SKIP_CAND=0,1 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x40:rs:a2:t2 0x40:rt:t2:a2 */
#include "types.h"

s32 MargePrim(u32 *p0, u32 *p1)
{
    s32 len = ((*p1 >> 24) & 0xFF) + ((*p0 >> 24) & 0xFF) + 1;

    if (len >= 33) {
        return -1;
    }
    *p0 = (((len & 0xFF) ^ (*p0 >> 24)) << 24) ^ *p0;
    return 0;
}
