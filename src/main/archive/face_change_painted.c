/* Function: face_change */
/* Address: 0x80091A20 */
/* Size: 144 bytes (36 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: scheduler_normalizer */
/* @PostProc: custom_script custom_func_80091A20.py */
#include "types.h"

extern u8 *_ss_score[];
extern void SpuVmSeqKeyOff(void);

void face_change(s32 arg0, s32 arg1) {
    u8 **ptr;
    u8 *base;
    u8 *entry;
    s32 off;

    ptr = &_ss_score[arg0];
    base = *ptr;
    off = (arg1 * 5 * 4 + arg1) * 8;
    entry = base + off;

    SpuVmSeqKeyOff();

    entry[43] = 0;

    base = *ptr;
    off = ((arg1 + arg1 * 4) * 4 + arg1) * 8;
    entry = base + off;
    *(u32 *)(entry + 144) &= ~2;
}
