/* @SymbolStart: 0x801334AC */
/* @SymbolSize: 84 */
/* omimai_girl4_serifu_step - OMIMAI overlay, Address: 0x801334B4, Size: 84 bytes (21 insns) */
#include "types.h"

extern s16 now_serifu;
extern u8 now_girl;
extern void serifu_run_step(void);
extern void sub_sub_step_up(void);

void omimai_girl4_serifu_step(void)
{
    u8 t6_val = now_girl;

    if (t6_val == 4) {
        if (now_serifu == 5) {
            serifu_run_step();
            return;
        }
    }
    sub_sub_step_up();
}
