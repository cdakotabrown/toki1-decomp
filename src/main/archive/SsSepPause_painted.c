/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_arg_homes --offsets 0x28 0x2c --shrink */
/* @PostProc: strip_trailing_instructions --count 1 --nop-only */
/* @PostProc: patch_stack_complete --old 40 --new 32 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: custom_script custom_func_800908FC.py */
/* Function: func_800908FC */
/* Address: 0x800908FC */
/* Size: 220 bytes (55 instructions) */
/* Region: Library (0x80086000+) */
#include "types.h"

extern s8 *_ss_score[];
extern void SpuVmGetSeqVol(s32 a0, u16 *vol1, u16 *vol2);

void func_800908FC(s32 a0, s32 unused1, s32 unused2, s32 a3)
{
    s8 **v1;
    s32 off;
    s32 *p;

    v1 = (s8 **)&_ss_score[a0];

    off = ((a3 * 4 + a3) * 4 + a3) * 8;

    SpuVmGetSeqVol(a0, (u16 *)(*v1 + off + 118), (u16 *)(*v1 + off + 120));

    off = ((a3 + (a3 << 2)) * 4 + a3) * 8;
    p = (s32 *)(*v1 + off);
    p[36] &= ~1;

    off = (((a3 << 2) + a3) << 2) + a3;
    p = (s32 *)(*v1 + (off << 3));
    p[36] &= ~8;

    off = ((a3 * 5 << 2) + a3) << 3;
    p = (s32 *)(*v1 + off);
    p[36] |= 2;
}
