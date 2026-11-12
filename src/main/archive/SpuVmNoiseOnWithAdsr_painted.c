/* Function: SpuVmNoiseOnWithAdsr */
/* Address:  0x8008DA50 */
/* Size:     128 bytes (32 instructions) */
/* Region:   Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: custom_script custom_func_8008DA50.py */
#include "types.h"

extern s32 SpuVmAlloc(s32);
extern void vmNoiseOn2(s32, u16, u16, u16, s32);
extern s8 spuVmMaxVoice;
extern s16 D_8013160A;
extern u8 libsnd_cur_priority;

void SpuVmNoiseOnWithAdsr(s32 a0, s32 a1, s32 a2, s32 a3) {
    s32 result;

    libsnd_cur_priority = 127;
    result = SpuVmAlloc(255);
    D_8013160A = (s16)result;

    if ((s16)result < spuVmMaxVoice) {
        vmNoiseOn2((u8)D_8013160A, (u16)a0, (u16)a1, (u16)a2, (u16)a3);
    }
}
