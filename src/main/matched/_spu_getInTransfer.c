/* Address: 0x800955F4 */
/* Size: 40 bytes (10 instructions; the 52-byte figure included the next */
/* Region: Library (0x80086000+), IDO 5.2 -O2 */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=occ:0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern s32 _spu_inTransfer;

s32 _spu_getInTransfer(void)
{
    if (_spu_inTransfer == 0) {
        return 1;
    }
    return 0;
}
