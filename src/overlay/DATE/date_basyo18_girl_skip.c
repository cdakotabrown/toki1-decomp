/* @SymbolStart: 0x8013DA08 */
/* @SymbolSize: 92 */
/* Address: 0x8013DA08 (cross-prologue head) / 0x8013DA10 (body) */
/* Size: 92 bytes (23 instructions) */
#include "types.h"

extern u8 now_girl;
extern u8 now_sub_sub_step;

extern void sub_sub_step_up(void);

void date_basyo18_girl_skip(void) {
    if (now_girl == 2 || now_girl == 8 || now_girl == 5 ||
        now_girl == 10) {
        now_sub_sub_step += 5;
    }
    sub_sub_step_up();
}
