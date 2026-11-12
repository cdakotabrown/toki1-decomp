/* Address: 0x80140D70, Size: 96 bytes (24 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);

void date_device_load_start(void) {
    device_load_start(0, 0xC051, 0xC02B, 0, 0, 0);
    if (device_load_poll() == 1) {
        device_cmd_send(191);
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
