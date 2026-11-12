/* Function: SpuVmSetProgVol */
/* Address: 0x8008E9EC */
/* Size: 120 bytes (30 instructions) */
/* Region: Library (0x80086000+) */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_TIE_TAKE=13 IDO52_AS1_TIE_REJECT=14 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 24 --new 32 */
/* @PostProc: remap_stack_slots --keep-frame --map '20:28,28:36,32:40' */
#include "types.h"

extern u8 *_svm_pg;
s32 SpuVmVSetUp(s16, s16);

s32 SpuVmSetProgVol(s16 arg0, s16 arg1, u16 arg2)
{
    if (SpuVmVSetUp(arg0, arg1) == 0) {
        _svm_pg[arg1 * 16 + 1] = arg2;
        return (_svm_pg + 1)[(u32)arg1 * 16];
    }
    return -1;
}
