/* Address: 0x8008F414 */
/* Size: 88 bytes (22 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 + as1 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_XBB_SKIP_CAND=1 IDO52_AS1_TIE_REJECT=6,7,8,9 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_stack_complete --old 24 --new 32 --shift-ra */
/* @PostProc: remap_stack_slots --map '24:32,36:44,40:48' --keep-frame */
#include "types.h"

extern s32 SpuVmPitchBend();

s32 SsUtPitchBend(s32 a0, s16 x, s16 y, s32 a3, u16 str)
{
    if ((s16)SpuVmPitchBend(32, x, y, str) == 0) {
        return -1;
    }
    return 0;
}
