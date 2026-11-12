/* bunka_sd_wait4b - BUNKA_SD overlay, Address: 0x80135C70, Size: 100 bytes (25 insns) */
#include "types.h"

extern void bunka_sd_cue_send2(void);
extern void bunka_sd_state_init(void);
extern void sub_sub_step_up(void);

extern s32 step_counter;
extern u8 cal_cw_mode;

void bunka_sd_wait4b(void)
{
    s32 val;
    u8 flag;

    bunka_sd_cue_send2();
    bunka_sd_state_init();
    val = step_counter;
    val++;
    step_counter = val;
    if ((u32)val >= 33U) {
        flag = cal_cw_mode;
        if (!(flag & 1)) {
            sub_sub_step_up();
        }
    }
}
