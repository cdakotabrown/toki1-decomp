/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: scheduler_normalizer */
/* @PostProc: custom_script custom_func_800901C8.py */
/* Function: func_800901C8 */
/* Address: 0x800901C8 */
/* Size: 216 bytes (54 instructions) */
/* Region: Library (0x80086000+) */
#include "types.h"

extern s8 *_ss_score[];
extern void Snd_stop(void);

void func_800901C8(s32 a0, s32 a1)
{
    s8 **v1;
    s32 *p;
    s32 off;

    v1 = (s8 **)&_ss_score[a0];

    off = ((a1 * 4 + a1) * 4 + a1) * 8;
    p = (s32 *)(*v1 + off);
    p[36] &= ~1;

    off = ((a1 + (a1 << 2)) * 4 + a1) * 8;
    p = (s32 *)(*v1 + off);
    p[36] &= ~2;

    off = (((a1 << 2) + a1) << 2) + a1;
    p = (s32 *)(*v1 + (off << 3));
    p[36] &= ~8;

    off = ((a1 * 5 << 2) + a1) << 3;
    p = (s32 *)(*v1 + off);
    p[36] |= 4;

    Snd_stop();
}
