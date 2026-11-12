/* @SymbolStart: 0x801386EC */
/* @SymbolSize: 76 */
/* Function: date_basyo15_bg_step12 */
/* Overlay: DATE */
/* Address: 0x801386EC (cross-prologue head) / 0x80138700 (body) */
/* Size: 76 bytes (19 instructions) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern s32 sub_sub_step_up(void);

extern s32 step_counter;

s32 date_basyo15_bg_step12(void) {
    s32 v;

    v = step_counter + 1;
    step_counter = v;
    if ((u32)v < 32) {
        return 0;
    }
    dec_bg_cache_step(17510);
    return sub_sub_step_up();
}
