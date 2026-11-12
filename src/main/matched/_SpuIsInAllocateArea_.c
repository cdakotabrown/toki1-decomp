/* Address: 0x80098490, Size: 44 bytes (11 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* assembled by ASPSX from IDO's `cc -S` stream (`@Compiler: ido52-gas`). */
/* @Compiler: ido52-gas */
/* @CompileFlags: --ido-fill-load-delay */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=5:v1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0x8 */
/* @PostProc: swap_instructions --offset1 0x0 --offset2 0x4 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0x8 */
#include "types.h"

extern s32 spu_mem_mode;
extern void _SpuIsInAllocateArea(s32 arg);

void _SpuIsInAllocateArea_(s32 a0) {
    s32 shift;

    shift = spu_mem_mode;
    _SpuIsInAllocateArea(a0 << shift);
}
