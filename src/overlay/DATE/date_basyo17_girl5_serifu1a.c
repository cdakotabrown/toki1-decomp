/* @SymbolStart: 0x8013E178 */
/* @SymbolSize: 156 */
/* Address: 0x8013E178 (cross-prologue head) / 0x8013E180 (body) */
/* Size: 156 bytes (39 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void serifu_run(void);
extern u8 now_girl;
extern s32 serifu_no;
extern u8 date_pattern;

void date_basyo17_girl5_serifu1a(void) {
    if (now_girl == 5) {
        if (date_pattern == 0) {
            if (serifu_no == 2) {
                serifu_run();
                return;
            }
        }
    }
    if (now_girl == 5) {
        if (date_pattern == 1) {
            if (serifu_no == 0) {
                serifu_run();
                return;
            }
        }
    }
    sub_sub_step_up();
}
