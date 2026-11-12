/* Address: 0x8008DA50 */
/* Size: 128 bytes (32 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 */
/* @CompileEnv: `IDO52_AS1_XBB_SKIP_CAND=0` vetoes as1's cross-block boost of */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_instructions --offset1 0x04 --offset2 0x0c */
/* @PostProc: swap_instructions --offset1 0x08 --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x0c --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: remap_stack_slots --map '28:44,32:48,36:52,40:56,44:60' */





































#include "types.h"

typedef union {
    u16 h;
    u8  b;
} HalfByte;

extern u8 libsnd_cur_priority;
extern s8 spuVmMaxVoice;
extern HalfByte D_8013160A;

extern s32 SpuVmAlloc(s32 arg0);
extern void vmNoiseOn2(s32 handle, u16 a, u16 b, u16 c, u16 d);

void SpuVmNoiseOnWithAdsr(u16 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    s32 v;
#line 5
    libsnd_cur_priority = 127;
#line 5
    v = SpuVmAlloc(255);
#line 5
    D_8013160A.h = v;
#line 5
    if ((s16)v < spuVmMaxVoice) {
#line 5
        vmNoiseOn2(D_8013160A.b, arg0, arg1, arg2, arg3);
#line 5
    }
}












