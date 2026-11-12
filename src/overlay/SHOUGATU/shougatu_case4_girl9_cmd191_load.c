/* Address: 0x8014095C */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

extern s32 device_load_poll(void);
extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 girl_head_tbl[];
extern s32 girl_body_tbl[];
extern s32 girl_seq_tbl[];

void shougatu_case4_girl9_cmd191_load(void)
{
    s32 v0;

    device_cmd_send(191);
    device_load_start(1, 0xBFA6, 0xBF87, girl_head_tbl[9], girl_body_tbl[9], girl_seq_tbl[9]);
    v0 = device_load_poll();
    if (v0 == 1) {
        device_cmd_send(512);
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
