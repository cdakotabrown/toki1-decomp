/* @SymbolStart: 0x801551A4 */
/* @SymbolSize: 92 */
/* Function: date_girl1_serifu_girl14 */
/* Overlay: DATE */
/* Address: 0x801551A4 (cross-prologue head) / 0x801551AC (body) */
/* Size: 92 bytes (23 instructions) */
#include "types.h"

extern void func_8014C530(void);

extern s32 step_counter;
extern u8 now_girl;
extern u8 girl_no;

void date_girl1_serifu_girl14(void)
{
    s32 state;
    s32 isZero;

    state = step_counter;
    isZero = (u32)state < 1U;
    step_counter = state + 1;
    if (isZero != 0) {
        now_girl = 14;
    } else {
        now_girl = girl_no;
    }
    func_8014C530();
}
