/* @SymbolStart: 0x8013DBE8 */
/* @SymbolSize: 100 */
/* Address: 0x8013DBE8 (cross-prologue head) / 0x8013DBF0 (body) */
/* Size: 100 bytes (25 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern u8 now_girl;
extern u8 girl6_spot4_cnt;
extern s32 serifu_no;
extern s16 now_serifu;

void date_basyo2_girl6_serifu_add(void) {
    if (now_girl == 6 && girl6_spot4_cnt != 0 && serifu_no == 2) {
        now_serifu += 1;
    }
    sub_sub_step_up();
}
