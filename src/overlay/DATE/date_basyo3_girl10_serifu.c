/* @SymbolStart: 0x8013E2EC */
/* @SymbolSize: 100 */
/* Address: 0x8013E2EC (cross-prologue head) / 8013E2F4 (body) */
/* Size: 100 bytes (25 instructions) */
#include "types.h"

extern u8 now_girl;
extern s32 serifu_no;
extern u8 date_pattern;

extern void sub_sub_step_up(void);
extern void serifu_run(void);

void date_basyo3_girl10_serifu(void) {
    if (now_girl == 10) {
        if (serifu_no == 1) {
            if (date_pattern == 2) {
                serifu_run();
                return;
            }
        }
    }
    sub_sub_step_up();
}
