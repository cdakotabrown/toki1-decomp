/* Function: SpuVmKeyOff */
/* Address: 0x8008E420 */
/* Size: 316 bytes (79 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: ido52 */
/* @CompileFlags: -O2 */
/* @PostProc: custom_script custom_func_8008E420.py */
#include "types.h"

extern s8 spuVmMaxVoice;
extern u16 D_8013160A;
extern s16 _svm_voice[];

extern void vmNoiseOff(s32 a0);
extern void SpuVmKeyOffNow(s32 a0);

s32 SpuVmKeyOff(s16 arg0, s16 arg1, s16 arg2, u16 arg3) {
    s32 i;
    s32 j;
    s32 count;
    s32 const255;
    s16 *base;
    s16 *entry;

    count = spuVmMaxVoice;
    if (count == 0) {
        return 0;
    }

    base = _svm_voice;
    const255 = 255;
    i = 0;
    j = 0;
    do {
        entry = base + i * 12;
        if (entry[4] != (s16)arg3) {
            goto next;
        }
        if (entry[7] != arg2) {
            goto next;
        }
        if (entry[5] != arg1) {
            goto next;
        }
        if (entry[9] != arg0) {
            goto next;
        }
        if (entry[0] == (s16)const255) {
            D_8013160A = i;
            SpuVmKeyOffNow(0);
        } else {
            vmNoiseOff(i);
        }
next:
        j = (j + 1) & 0xff;
        count = spuVmMaxVoice;
        i = (i + 1) & 0xff;
    } while (i < count);

    return j;
}
