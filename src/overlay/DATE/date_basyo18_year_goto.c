/* @SymbolStart: 0x8013A48C */
/* @SymbolSize: 64 */
/* Function: date_basyo18_year_goto */
/* Overlay: DATE */
/* Address: 0x8013A48C (cross-prologue head) / 0x8013A494 (body) */
/* Size: 64 bytes (16 instructions) */
#include "types.h"

extern u8 now_year;

extern void goto_sub_sub_step(s32 a0);

void date_basyo18_year_goto(void)
{
    if (now_year == 95) {
        goto_sub_sub_step(12);
        return;
    }
    goto_sub_sub_step(23);
}
