/* @SymbolStart: 0x8013E53C */
/* @SymbolSize: 100 */
/* Address: 0x8013E53C (cross-prologue head) / 8013E544 (body) */
/* Size: 100 bytes (25 instructions) */
#include "types.h"

extern u8 now_girl;
extern s32 serifu_no;
extern u8 date_pattern;

extern void sub_sub_step_up(void);
extern void serifu_run(void);

void date_basyo10_girl10_serifu(void) {
    s32 a, b, c;

    c = (s32)now_girl;
    if (c != 10) goto fallback;
    a = serifu_no;
    if (a != 1) goto fallback;
    b = (s32)date_pattern;
    if ((u32)b != 1U) goto fallback;
    serifu_run();
    return;
fallback:
    sub_sub_step_up();
}
