/* @SymbolStart: 0x801324BC */
/* @SymbolSize: 136 */
#include "types.h"

extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern void sub_sub_step_up(void);
extern u8 girl_no;
extern s32 girl_head_tbl[];
extern s32 girl_body_tbl[];
extern s32 girl_seq_tbl[];

void omimai_girl_load(void)
{
    s32 idx = girl_no;
    device_load_start(1, 0xBFA6, 0xBF87, girl_head_tbl[idx],
                  girl_body_tbl[idx], girl_seq_tbl[idx]);
    if (device_load_poll() == 1) {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
