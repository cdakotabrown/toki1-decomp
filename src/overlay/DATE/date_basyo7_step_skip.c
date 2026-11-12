/* @SymbolStart: 0x801393FC */
/* @SymbolSize: 80 */
/* Address: 0x801393FC (cross-prologue head) / 0x80139404 (body) */
/* Size: 80 bytes (20 instructions) */
#include "types.h"

extern u8 date_pattern;
extern u8 now_sub_sub_step;

extern void sub_sub_step_up(void);

void date_basyo7_step_skip(void) {
    if (date_pattern == 5) {
        sub_sub_step_up();
        now_sub_sub_step += 9;
    } else {
        sub_sub_step_up();
    }
}
