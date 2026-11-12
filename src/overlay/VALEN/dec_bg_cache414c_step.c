/* @SymbolSize: 40 */
/* dec_bg_cache414c_step - VALEN overlay, Address: 0x80133B94, Size: 48 bytes (12 insns) */
/* Status: BYTE-PERFECT 🎉 (via @SymbolSize: 40 to exclude cross-prologue lui) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void dec_bg_cache_step(s32);

void dec_bg_cache414c_step(void)
{
    dec_bg_cache_step(16716);
    sub_sub_step_up();
}
