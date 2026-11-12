/* @SymbolStart: 0x801335C4 */
/* @SymbolSize: 136 */
#include "types.h"

extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern void sub_sub_step_up(void);
extern u8 now_girl;
extern s32 girl_head_tbl[];
extern s32 girl_body_tbl[];
extern s32 girl_seq_tbl[];

void omimai_girl_load2(void)
{
    s32 idx = now_girl;
    device_load_start(1, 0xBFA6, 0xBF87, girl_head_tbl[idx],
                  girl_body_tbl[idx], girl_seq_tbl[idx]);
    if (device_load_poll() == 1) {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
