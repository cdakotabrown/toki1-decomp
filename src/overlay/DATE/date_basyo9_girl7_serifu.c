/* @SymbolStart: 0x8013DF60 */
/* @SymbolSize: 100 */
/* Address: 0x8013DF60 (cross-prologue head) / 8013DF68 (body) */
/* Size: 100 bytes (25 instructions) */
#include "types.h"

extern u8 now_girl;
extern s32 serifu_no;
extern u8 date_pattern;

extern void sub_sub_step_up(void);
extern void serifu_run(void);

void date_basyo9_girl7_serifu(void) {
    if (now_girl == 7) {
        if (serifu_no == 0) {
            if (date_pattern == 1) {
                serifu_run();
                return;
            }
        }
    }
    sub_sub_step_up();
}
