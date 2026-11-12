/* @SymbolStart: 0x8013DCDC */
/* @SymbolSize: 116 */
/* Address: 0x8013DCDC (cross-prologue head) / 0x8013DCE4 (body) */
/* Size: 116 bytes (29 instructions) */
#include "types.h"

extern u8 now_girl;
extern s32 serifu_no;
extern u8 date_pattern;

extern void sub_sub_step_up(void);

void date_basyo5_girl7_skip(void) {
    if (now_girl == 7 && serifu_no == 0 && date_pattern == 0) {
        sub_sub_step_up();
    } else {
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
    }
}
