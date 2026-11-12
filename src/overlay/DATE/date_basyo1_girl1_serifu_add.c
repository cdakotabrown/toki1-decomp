/* @SymbolStart: 0x8013D1C8 */
/* @SymbolSize: 140 */
/* Address: 0x8013D1C8 (cross-prologue head) / 0x8013D1D0 (body) */
/* Size: 140 bytes (35 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern u8 now_girl;
extern u8 now_month;
extern u8 date_pattern;
extern s16 now_serifu;

void date_basyo1_girl1_serifu_add(void) {
    s32 v0;
    if (now_girl == 1) {
        v0 = now_month;
        if ((u32)v0 >= 6u && (u32)v0 < 9u) {
            v0 = date_pattern;
            if (v0 == 0) {
                now_serifu += 9;
            } else if (v0 == 1) {
                now_serifu += 9;
            }
        }
    }
    sub_sub_step_up();
}
