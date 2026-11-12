/* @SymbolSize: 40 */
/* Address: 0x80137FC4, Size: 52 bytes (13 insns) */
/* Status: BYTE-PERFECT 🎉 (via @SymbolSize: 40 to exclude trailing cross-prologue) */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();

void dec_bg_cache4045_step(void)
{
    dec_bg_cache_step(16453);
    sub_sub_step_up();
}
