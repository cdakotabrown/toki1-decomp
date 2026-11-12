/* @SymbolStart: 0x80139AD4 */
/* @SymbolSize: 100 */
/* Address: 0x80139AD4 (cross-prologue head) / 0x80139ADC (body) */
/* Size: 100 bytes (25 instructions) */
#include "types.h"

extern u8 date_late_flag;
extern s16 now_serifu;
extern u8 date_final_flag;

extern void sub_sub_step_up(void);

void date_basyo12_serifu_flag_adj(void) {
    if (date_late_flag != 0) {
        now_serifu += 16;
    }
    if (date_final_flag != 0) {
        now_serifu += 16;
    }
    sub_sub_step_up();
}
