/* Address: 0x801377C4 */
/* Size: 76 bytes (19 instructions) */
#include "types.h"

extern void sub_step_up();
extern void device_cmd_send();
extern s32 device_load_poll();
extern void season_bgm_set();

void shougatu_bgm_wait(void)
{
    device_cmd_send(207);
    season_bgm_set();
    if (device_load_poll() == 1) {
        sub_step_up();
    }
    sub_step_up();
}
