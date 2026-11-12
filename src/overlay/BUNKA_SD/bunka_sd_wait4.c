/* bunka_sd_wait4 - BUNKA_SD overlay, Address: 0x80134800, Size: 80 bytes (20 insns) */
/* @SymbolSize: 80 */
#include "types.h"

extern void bunka_sd_cue_send(void);
extern void bunka_sd_anim(void);
extern void sub_sub_step_up(void);

extern s32 step_counter;

void bunka_sd_wait4(void)
{
    s32 val;

    bunka_sd_cue_send();
    bunka_sd_anim();
    val = step_counter;
    val++;
    step_counter = val;
    if ((u32)val >= 2072U) {
        sub_sub_step_up();
    }
}
