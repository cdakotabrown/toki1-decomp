/* Address: 0x801342AC */
/* Size: 180 bytes (45 instructions) */
/* @SymbolSize: 180 */
#include "types.h"

extern void shougatu_serifu_run(void);
extern void device_cmd_send(s32);

extern s16 shougatu_meet_now_serifu_in;
extern u8 step_mode;
extern u8 yazirushi_cw_disp;

void shougatu_serifu_in_check(void)
{
    shougatu_serifu_run();
    if (shougatu_meet_now_serifu_in == 2 && step_mode == 0 && (yazirushi_cw_disp & 0x80) != 0) {
        device_cmd_send(514);
        step_mode = step_mode + 1;
    }
    if (step_mode == 0 && shougatu_meet_now_serifu_in == 3) {
        device_cmd_send(514);
        step_mode = step_mode + 1;
    }
}
