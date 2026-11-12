/* Address: 0x80097530 */
/* Size: 32 bytes (8 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 -O2 codegen, PSY-Q-style assembly (@Compiler: ido52-gas) */
/* @Compiler: ido52-gas */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x0 --offset2 0x4 */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0x8 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
#include "types.h"

extern volatile u16 * volatile _spu_RXX;

s32 SysSpu_setRegister(s32 idx, u16 val)
{
    _spu_RXX[idx] = val;
    return 0;
}
