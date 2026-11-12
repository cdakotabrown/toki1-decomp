/* shugaku_birth_load_poll -- SHUGAKU overlay, Address: 0x801325C0 */
#include "types.h"

extern u8 now_sub_step;

extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32  device_load_poll(void);
extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

void shugaku_birth_load_poll(void)
{
    device_load_start(0, 0xBFA6, 0xBF87, 0xCAA3, 0xCA5D, 0xCA4C);
    if (device_load_poll() == 1) {
        if (now_sub_step == 6) {
            device_cmd_send(512);
        }
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
