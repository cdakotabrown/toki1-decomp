/* Function: SsUtGetDetVVol */
/* Address: 0x8008F648 */
/* Size: 84 bytes (21 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x8008F648 */
/* @SymbolSize: 84 */
/* @Compiler: ido52-gas */
/* @CompileFlags: --ido-copy-and-bump --ido-fill-load-delay --ido-fill-macro-lui --ido-branch-zero-imm --ido-jr-store-slot */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u16 *libsnd_spu_regs_ptr;

s32 SsUtGetDetVVol(s16 vc, u16 *out0, u16 *out1)
{
    u16 *r;

    if (vc < 24) {
        if (vc >= 0) {
            r = libsnd_spu_regs_ptr;
            r += vc * 8;
            if (1) {
                *out0 = r[0];
                *out1 = r[1];
                return 0;
            }
        }
    }
    return -1;
}
