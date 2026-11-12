/* get_etc_best_idx - ETC overlay, Address: 0x8014B050 */
/* Size: 228 bytes (57 instructions) */
/* @SymbolSize: 228 */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x60 */
/* @PostProc: patch_insn_reg_field 0xa4:rd:t9:t0 0xa8:rs:t9:t0 */
#include "types.h"

extern void get_grp(void *a, void *b);
extern void printf(void *arr, s32 idx, s32 c);
extern u8 screen_w[][56];
extern u8 etc_best_idx_fmt[];

s32 get_etc_best_idx(void)
{
    u8 buf_a[32];
    u8 buf_b[32];
    s32 i;
    s32 best;
    u32 prev;
    u32 cur;
    u8 *p;

    get_grp(buf_a, buf_b);
    prev = 60;
    best = 0;
    i = 1;
    p = &buf_a[1];
    do {
        printf(etc_best_idx_fmt, i, *p);
        cur = *p;
        if (prev < cur) {
            if ((*(s32 *)(screen_w[i] + 456) << 30) < 0) {
                best = i;
                prev = cur;
            }
        }
        i++;
        p++;
    } while (i != 11);
    return best;
}
