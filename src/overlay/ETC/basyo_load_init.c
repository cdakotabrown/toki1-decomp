/* @SymbolSize: 168 */
/* Address: 0x801324B8 */
/* Size: 168 bytes (42 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32  device_load_poll(void);
extern void class_clear_all(s32);
extern void text_sys_init(void);
extern void k_disp_start(s32);
extern void basyo_init(void *);

extern u8 step_mode;
extern u8 basyo_load_init_str0;

void basyo_load_init(void)
{
    s32 t6 = step_mode;

    s32 v0;

    if (t6 == 0) {
        device_load_start(0, 0, 0, 52859, 52801, 52780);
        step_mode = step_mode + 1;
        return;
    }
    v0 = device_load_poll();
    if (v0 != 1) {
        return;
    }
    device_cmd_send(513);
    class_clear_all(0);
    text_sys_init();
    basyo_init(&basyo_load_init_str0);
    k_disp_start(1);
    sub_sub_step_up();
}
