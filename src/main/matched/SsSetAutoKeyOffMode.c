/* Address: 0x80090C80 */
/* Size: 20 bytes (5 instructions); 0x80090C94..9F is alignment padding */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 -O2 codegen, PSY-Q-style assembly (@Compiler: ido52-gas) */
/* @Compiler: ido52-gas */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 _svm_auto_kof_mode;

void SsSetAutoKeyOffMode(s16 v)
{
    _svm_auto_kof_mode = v;
}
