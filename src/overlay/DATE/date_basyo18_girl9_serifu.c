/* @SymbolStart: 0x8013E298 */
/* @SymbolSize: 84 */
/* Address: 0x8013E298 (cross-prologue head) / 0x8013E2A0 (body) */
/* Size: 84 bytes (21 instructions) */
#include "types.h"

extern u8 now_girl;
extern s32 serifu_no;

extern void sub_sub_step_up(void);
extern void serifu_run(void);

void date_basyo18_girl9_serifu(void) {
    if (now_girl == 9) {
        if (serifu_no == 2) {
            serifu_run();
            return;
        }
    }
    sub_sub_step_up();
}
