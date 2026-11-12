/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 48 --new 40 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: custom_script custom_func_80091A24.py */
/* Function: hidden_face_change4 */
/* Address: 0x80091A24 */
/* Size: 140 bytes (35 instructions) */
/* Region: Library (0x80086000+) */
#include "types.h"

extern u8 *_ss_score[];
extern void SpuVmSeqKeyOff(s32 arg0);

void hidden_face_change4(s32 arg0, s32 arg1) {
    u8 **tablePtr;
    u8 *base;
    u8 *entry;
    s32 off;

    tablePtr = &_ss_score[arg0];
    off = ((arg1 * 4 + arg1) * 4 + arg1) * 8;
    base = *tablePtr;
    entry = base + off;

    SpuVmSeqKeyOff(arg0);

    base = *tablePtr;
    entry[43] = 0;
    off = ((arg1 + (arg1 << 2)) * 4 + arg1) * 8;
    *(u32 *)(base + off + 144) &= ~2;
}
