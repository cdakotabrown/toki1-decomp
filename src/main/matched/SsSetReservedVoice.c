/* Address: 0x80090C40, Size: 56 bytes (14 instructions) */
/* Region: Library (0x80086000-0x800BFFFF), compiler IDO 5.2 */
/* @CompileEnv: IDO52_UGEN_DEST_TEMP=1 IDO52_AS1_NO_LA_PULL=1 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x0:rd:t6:a0 0x4:rt:t6:a0 */
#include "types.h"
extern s8 spuVmMaxVoice;
s32 SsSetReservedVoice(s32 a) {
    a = (s32)(s8)a;
    if (a >= 25 || a <= 0) return -1;
    spuVmMaxVoice = (s8)a;
    return a;
}
