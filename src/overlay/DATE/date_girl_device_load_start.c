/* @SymbolStart: 0x8014C5AC */
/* @SymbolSize: 136 */
/* Function: date_girl_device_load_start */
/* Overlay: DATE */
/* Address: 0x8014C5AC (cross-prologue head) / 0x8014C5D4 (body) */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);

extern s32 event_girl_no;
extern s32 girl_head_tbl[];
extern s32 girl_body_tbl[];
extern s32 girl_seq_tbl[];

void date_girl_device_load_start(void)
{
    s32 idx;

    idx = event_girl_no;
    device_load_start(1, 0xC051, 0xC02B, girl_head_tbl[idx], girl_body_tbl[idx],
                  girl_seq_tbl[idx]);
    if (device_load_poll() == 1) {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
