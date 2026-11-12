/* Address: 0x80097550 */
/* Size: 36 bytes (9 instructions); 0x80097574..7F is alignment padding */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 -O2 codegen, PSY-Q-style assembly (@Compiler: ido52-gas) */
/* @Compiler: ido52-gas */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern volatile u16 * volatile _spu_RXX;

u16 SysSpu_getRegister(s32 idx)
{
    return _spu_RXX[idx] & 0xFFFF;
}
