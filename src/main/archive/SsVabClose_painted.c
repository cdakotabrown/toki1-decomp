/* Function: SsVabClose */
/* Address: 0x80090CC8 */
/* Size: 120 bytes (30 instructions) */
/* Region: Library (0x80086000+) */
/* Likely Symbol: SsUtFlush (from o.bin) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: custom_script custom_func_80090CC8.py */
/* Compiler: IDO 5.2 */
#include "types.h"

extern u8 _svm_vab_used[16];
extern s32 _svm_vab_start[16];
extern s16 _svm_vab_count;
extern void SpuFree(s32);

void SsVabClose(s32 ch) {
    u8 *flag;
    if (ch < 0) return;
    if (ch >= 16) return;
    flag = &_svm_vab_used[ch];
    if (*flag != 1) return;
    SpuFree(_svm_vab_start[ch]);
    *flag = 0;
    _svm_vab_count--;
}
