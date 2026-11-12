/* @SymbolStart: 0x80139364 */
/* @SymbolSize: 96 */
/* taco_wait4 -- TACO overlay, Address: 0x8013936C */
#include "types.h"

extern u8 step_mode;

extern void sub_sub_step_up(void);
extern void taco_drop(s32, s32);

void taco_wait4(void)
{
    u8 t6_in = step_mode;

    u8 newVal;
    u32 prod;

    newVal = (u8)(t6_in + 1);
    step_mode = newVal;
    taco_drop(0, 0);
    prod = (u32)step_mode * 3;
    if (prod >= 129U) {
        sub_sub_step_up();
    }
}
