/* @SymbolStart: 0x8013E474 */
/* @SymbolSize: 100 */
/* Address: 0x8013E474 (cross-prologue head) / 8013E47C (body) */
/* Size: 100 bytes (25 instructions) */
#include "types.h"

extern u8 now_girl;
extern s32 serifu_no;
extern u8 date_pattern;

extern void sub_sub_step_up(void);
extern void serifu_run(void);

void date_basyo7_girl10_serifu(void) {
    if (now_girl == 10) {
        if (serifu_no == 2) {
            if (date_pattern == 4) {
                serifu_run();
                return;
            }
        }
    }
    sub_sub_step_up();
}
