/* @SymbolStart: 0x8013E4D8 */
/* @SymbolSize: 100 */
/* Address: 0x8013E4D8 (cross-prologue head) / 8013E4E0 (body) */
/* Size: 100 bytes (25 instructions) */
#include "types.h"

extern u8 now_girl;
extern s32 serifu_no;
extern u8 date_pattern;

extern void sub_sub_step_up(void);
extern void serifu_run(void);

void date_girl10_serifu_run(void) {
    if (now_girl == 10) {
        if (serifu_no == 0) {
            if (date_pattern == 0) {
                serifu_run();
                return;
            }
        }
    }
    sub_sub_step_up();
}
